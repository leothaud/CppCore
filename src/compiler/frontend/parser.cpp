/* Copyright 2026 Dylan Leothaud
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not
 * use this file except in compliance with the License. You may obtain a copy of
 * the License at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations under
 * the License.
 */
export module frontend:parser;

import core;
import ast;
import :lexer;

namespace compiler::frontend {

namespace {

core::LogicalResult<core::UniquePtr<int>> parsingError(core::String message,
                                                       u64 line, u64 column) {
  return core::LogicalResult<core::UniquePtr<int>>::failure(
      "Parsing error at " + core::String::of(line) + ":" +
      core::String::of(column) + ":\n" + message + "\n");
}

core::LogicalResult<core::UniquePtr<int>>
parsingError(core::String message,
             core::Vector<ast::tokens::Token>::Iterator &it) {
  return parsingError(message, it->start_line, it->start_column);
}

core::LogicalResult<core::UniquePtr<int>> eofError(u64 line, u64 column) {
  return parsingError("Unexpected end-of-file.", line, column);
}

core::LogicalResult<core::UniquePtr<int>>
eofError(core::Vector<ast::tokens::Token>::Iterator &it) {
  return eofError(it->end_line, it->end_column);
}

template <ast::tokens::TokenType... expected> struct GenerateExpected;

template <ast::tokens::TokenType head, ast::tokens::TokenType... tail>
struct GenerateExpected<head, tail...> {
  static inline const core::String value() {
    return "`" + ast::tokens::to_string(head) + "`, " +
           GenerateExpected<tail...>::value();
  }
};

template <ast::tokens::TokenType expected> struct GenerateExpected<expected> {
  static inline const core::String value() {
    return "`" + ast::tokens::to_string(expected) + "`";
  }
};

template <ast::tokens::TokenType expected1, ast::tokens::TokenType expected2>
struct GenerateExpected<expected1, expected2> {
  static inline core::String value() {
    return "`" + ast::tokens::to_string(expected1) + "`, or `" +
           ast::tokens::to_string(expected2) + "`";
  }
};

template <ast::tokens::TokenType... expected>
core::LogicalResult<core::UniquePtr<int>>
wrongTokenError(core::Vector<ast::tokens::Token>::Iterator &it) {
  return parsingError("Wrong token type. Expected " +
                          GenerateExpected<expected...>::value() +
                          " but got `" + to_string(it->type) + "`.",
                      it);
}

} // namespace

core::LogicalResult<core::UniquePtr<ast::CompoundStmt>>
parseCompoundStmt(core::Vector<ast::tokens::Token>::Iterator &it,
                  core::Vector<ast::tokens::Token>::Iterator end,
                  core::UniquePtr<ast::symbolTable::Table> &table);

core::LogicalResult<core::UniquePtr<ast::VariableDeclaration>>
parseVariableDeclaration(core::Vector<ast::tokens::Token>::Iterator &it,
                         core::Vector<ast::tokens::Token>::Iterator end,
                         core::UniquePtr<ast::symbolTable::Table> &table);

core::LogicalResult<core::UniquePtr<ast::Statement>>
parseStatement(core::Vector<ast::tokens::Token>::Iterator &it,
               core::Vector<ast::tokens::Token>::Iterator end,
               core::UniquePtr<ast::symbolTable::Table> &table);

core::LogicalResult<core::UniquePtr<ast::Argument>>
parseArgument(core::Vector<ast::tokens::Token>::Iterator &it,
              core::Vector<ast::tokens::Token>::Iterator end,
              core::UniquePtr<ast::symbolTable::Table> &table);

core::LogicalResult<core::UniquePtr<ast::FunctionDeclaration>>
parseFunctionDeclaration(core::Vector<ast::tokens::Token>::Iterator &it,
                         core::Vector<ast::tokens::Token>::Iterator end,
                         core::UniquePtr<ast::symbolTable::Table> &table);

core::LogicalResult<core::UniquePtr<ast::StructDeclaration>>
parseStructDeclaration(core::Vector<ast::tokens::Token>::Iterator &it,
                       core::Vector<ast::tokens::Token>::Iterator end,
                       core::UniquePtr<ast::symbolTable::Table> &table);

core::LogicalResult<core::UniquePtr<ast::UnionDeclaration>>
parseUnionDeclaration(core::Vector<ast::tokens::Token>::Iterator &it,
                      core::Vector<ast::tokens::Token>::Iterator end,
                      core::UniquePtr<ast::symbolTable::Table> &table);

core::LogicalResult<core::UniquePtr<ast::Expression>>
parseExpression(core::Vector<ast::tokens::Token>::Iterator &it,
                core::Vector<ast::tokens::Token>::Iterator end,
                core::UniquePtr<ast::symbolTable::Table> &table);

core::LogicalResult<core::UniquePtr<ast::Type>>
parseType(core::Vector<ast::tokens::Token>::Iterator &it,
          core::Vector<ast::tokens::Token>::Iterator end,
          core::UniquePtr<ast::symbolTable::Table> &table);

// core::LogicalResult<core::UniquePtr<XXX>>
// parseXXX(core::Vector<ast::tokens::Token>::Iterator &it,
//          core::Vector<ast::tokens::Token>::Iterator end,
//          core::UniquePtr<ast::symbolTable::Table> &table) {}

template <typename CURRENT_TYPE, auto PARSE_BASE>
core::LogicalResult<core::UniquePtr<ast::Expression>>
parseBinexprAux(core::Vector<ast::tokens::Token>::Iterator &it,
                core::Vector<ast::tokens::Token>::Iterator end,
                core::UniquePtr<ast::symbolTable::Table> &table,
                core::UniquePtr<ast::Expression> current) {
  if (it + 1 == end) {
    return eofError(it);
  }
  ++it;
  auto rval = PARSE_BASE(it, end, table);
  if (rval.failed())
    return rval;
  auto result = core::UniquePtr<CURRENT_TYPE>::create();
  result->line_start = current->getLineStart();
  result->column_start = current->getColumnStart();
  result->lval = core::move(current);
  result->associatedLevel = table->current;
  result->rval = core::move(rval.getValue());
  result->line_end = result->rval->getLineEnd();
  result->column_end = result->rval->getColumnEnd();
  auto resultExpr = core::UniquePtr<ast::Expression>::create();
  *resultExpr = core::move(*result);
  return resultExpr;
}

core::LogicalResult<core::UniquePtr<ast::Expression>>
parseBaseExpression(core::Vector<ast::tokens::Token>::Iterator &it,
                    core::Vector<ast::tokens::Token>::Iterator end,
                    core::UniquePtr<ast::symbolTable::Table> &table) {
  switch (it->type) {
  case ast::tokens::TokenType::ID: {
    ast::VarExpression result;
    result.line_start = it->start_line;
    result.column_start = it->start_column;
    result.associatedLevel = table->current;
    result.line_end = it->end_line;
    result.column_end = it->end_column;
    result.name = it->s;
    if (table->lookup(result.name) == nullptr) {
      return parsingError("Unknown variable `" + result.name + "`.", it);
    }
    auto resExpr = core::UniquePtr<ast::Expression>::create();
    *resExpr = core::move(result);
    ++it;
    return resExpr;
  }

  case ast::tokens::TokenType::INT: {
    ast::IntExpression result;
    result.line_start = it->start_line;
    result.column_start = it->start_column;
    result.associatedLevel = table->current;
    result.line_end = it->end_line;
    result.column_end = it->end_column;
    result.value = it->i;
    auto resExpr = core::UniquePtr<ast::Expression>::create();
    *resExpr = core::move(result);
    ++it;
    return resExpr;
  }

  case ast::tokens::TokenType::CHAR: {
    ast::CharExpression result;
    result.line_start = it->start_line;
    result.column_start = it->start_column;
    result.associatedLevel = table->current;
    result.line_end = it->end_line;
    result.column_end = it->end_column;
    result.value = it->c;
    auto resExpr = core::UniquePtr<ast::Expression>::create();
    *resExpr = core::move(result);
    ++it;
    return resExpr;
  }

  case ast::tokens::TokenType::STRING: {
    ast::StringExpression result;
    result.line_start = it->start_line;
    result.column_start = it->start_column;
    result.associatedLevel = table->current;
    result.line_end = it->end_line;
    result.column_end = it->end_column;
    result.value = it->s;
    auto resExpr = core::UniquePtr<ast::Expression>::create();
    *resExpr = core::move(result);
    ++it;
    return resExpr;
  }

  case ast::tokens::TokenType::TRUE: {
    ast::TrueExpression result;
    result.line_start = it->start_line;
    result.column_start = it->start_column;
    result.associatedLevel = table->current;
    result.line_end = it->end_line;
    result.column_end = it->end_column;
    auto resExpr = core::UniquePtr<ast::Expression>::create();
    *resExpr = core::move(result);
    ++it;
    return resExpr;
  }

  case ast::tokens::TokenType::FALSE: {
    ast::FalseExpression result;
    result.line_start = it->start_line;
    result.column_start = it->start_column;
    result.associatedLevel = table->current;
    result.line_end = it->end_line;
    result.column_end = it->end_column;
    auto resExpr = core::UniquePtr<ast::Expression>::create();
    *resExpr = core::move(result);
    ++it;
    return resExpr;
  }

  case ast::tokens::TokenType::THIS: {
    ast::ThisExpression result;
    result.line_start = it->start_line;
    result.column_start = it->start_column;
    result.associatedLevel = table->current;
    result.line_end = it->end_line;
    result.column_end = it->end_column;
    auto resExpr = core::UniquePtr<ast::Expression>::create();
    *resExpr = core::move(result);
    ++it;
    return resExpr;
  }

  case ast::tokens::TokenType::OTHER: {
    ast::OtherExpression result;
    result.line_start = it->start_line;
    result.column_start = it->start_column;
    result.associatedLevel = table->current;
    result.line_end = it->end_line;
    result.column_end = it->end_column;
    auto resExpr = core::UniquePtr<ast::Expression>::create();
    *resExpr = core::move(result);
    ++it;
    return resExpr;
  }

  case ast::tokens::TokenType::LP: {
    if (it + 1 == end) {
      return eofError(it);
    }
    u64 line_start = it->start_line;
    u64 column_start = it->start_column;
    ++it;
    auto base = parseExpression(it, end, table);
    if (base.failed())
      return base;
    if (it == end) {
      auto &value = base.getValue();
      return eofError(value->getLineEnd(), value->getColumnEnd());
    }
    auto result = core::move(base.getValue());
    if (it->type != ast::tokens::TokenType::RP) {
      return wrongTokenError<ast::tokens::TokenType::RP>(it);
    }
    result->visit([&](auto &&elt) {
      elt.line_start = line_start;
      elt.column_start = column_start;
      elt.line_end = it->end_line;
      elt.column_end = it->end_column;
    });
    ++it;
    return result;
  }

  default: {
    return wrongTokenError<
        ast::tokens::TokenType::ID, ast::tokens::TokenType::INT,
        ast::tokens::TokenType::CHAR, ast::tokens::TokenType::STRING,
        ast::tokens::TokenType::TRUE, ast::tokens::TokenType::FALSE,
        ast::tokens::TokenType::THIS, ast::tokens::TokenType::OTHER,
        ast::tokens::TokenType::LP>(it);
  }
  }
}

core::LogicalResult<core::UniquePtr<ast::Expression>>
parsePostfixExpression(core::Vector<ast::tokens::Token>::Iterator &it,
                       core::Vector<ast::tokens::Token>::Iterator end,
                       core::UniquePtr<ast::symbolTable::Table> &table) {
  auto base = parseBaseExpression(it, end, table);
  if (base.failed())
    return base;
  core::UniquePtr<ast::Expression> current = core::move(base.getValue());

  while (it != end) {
    switch (it->type) {
    case ast::tokens::TokenType::LSB: {
      ast::ArrayExpression result;
      result.line_start = current->getLineStart();
      result.column_start = current->getColumnStart();
      result.associatedLevel = table->current;
      result.base = core::move(current);
      if (it + 1 == end) {
        return eofError(it);
      }
      ++it;
      auto index = parseExpression(it, end, table);
      if (index.failed())
        return index;
      if (it == end) {
        auto &value = index.getValue();
        return eofError(value->getLineEnd(), value->getColumnEnd());
      }
      result.index = core::move(index.getValue());
      if (it->type != ast::tokens::TokenType::RSB) {
        return wrongTokenError<ast::tokens::TokenType::RSB>(it);
      }
      result.line_end = it->end_line;
      result.column_end = it->end_column;
      current = core::UniquePtr<ast::Expression>::create();
      *current = core::move(result);
      ++it;
      break;
    }

    case ast::tokens::TokenType::DOT: {
      ast::FieldExpression result;
      result.line_start = current->getLineStart();
      result.column_start = current->getColumnStart();
      result.associatedLevel = table->current;
      result.base = core::move(current);
      if (it + 1 == end) {
        return eofError(it);
      }
      ++it;
      if (it->type != ast::tokens::TokenType::ID) {
        return wrongTokenError<ast::tokens::TokenType::ID>(it);
      }
      result.name = it->s;
      result.line_end = it->end_line;
      result.column_end = it->end_column;
      current = core::UniquePtr<ast::Expression>::create();
      *current = core::move(result);
      ++it;
      break;
    }

    case ast::tokens::TokenType::ARROW: {
      ast::PtrFieldExpression result;
      result.line_start = current->getLineStart();
      result.column_start = current->getColumnStart();
      result.associatedLevel = table->current;
      result.base = core::move(current);
      if (it + 1 == end) {
        return eofError(it);
      }
      ++it;
      if (it->type != ast::tokens::TokenType::ID) {
        return wrongTokenError<ast::tokens::TokenType::ID>(it);
      }
      result.name = it->s;
      result.line_end = it->end_line;
      result.column_end = it->end_column;
      current = core::UniquePtr<ast::Expression>::create();
      *current = core::move(result);
      ++it;
      break;
    }

    case ast::tokens::TokenType::DOUBLE_PLUS: {
      ast::PostIncrExpression result;
      result.line_start = current->getLineStart();
      result.column_start = current->getColumnStart();
      result.associatedLevel = table->current;
      result.line_end = it->end_line;
      result.column_end = it->end_column;
      result.base = core::move(current);
      current = core::UniquePtr<ast::Expression>::create();
      *current = core::move(result);
      ++it;
      break;
    }

    case ast::tokens::TokenType::DOUBLE_MINUS: {
      ast::PostDecrExpression result;
      result.line_start = current->getLineStart();
      result.column_start = current->getColumnStart();
      result.associatedLevel = table->current;
      result.line_end = it->end_line;
      result.column_end = it->end_column;
      result.base = core::move(current);
      current = core::UniquePtr<ast::Expression>::create();
      *current = core::move(result);
      ++it;
      break;
    }

    case ast::tokens::TokenType::LP: {
      ast::FunExpression result;
      result.line_start = current->getLineStart();
      result.column_start = current->getColumnStart();
      result.associatedLevel = table->current;
      result.base = core::move(current);
      if (it + 1 == end) {
        return eofError(it);
      }
      bool finished = it->type == ast::tokens::TokenType::RP;
      while (!finished) {
        auto arg = parseExpression(it, end, table);
        if (arg.failed())
          return arg;
        if (it == end) {
          auto &value = arg.getValue();
          return eofError(value->getLineEnd(), value->getColumnEnd());
        }
        result.args.pushBack(core::move(arg.getValue()));
        switch (it->type) {
        case ast::tokens::TokenType::COMMA: {
          if (it + 1 == end) {
            return eofError(it);
          }
          ++it;
          break;
        }

        case ast::tokens::TokenType::RP: {
          finished = true;
          break;
        }

        default: {
          return wrongTokenError<ast::tokens::TokenType::COMMA,
                                 ast::tokens::TokenType::RP>(it);
        }
        }
      }
      result.line_end = it->end_line;
      result.column_end = it->end_column;
      ++it;
      current = core::UniquePtr<ast::Expression>::create();
      *current = core::move(result);
      break;
    }

    case ast::tokens::TokenType::CAST: {
      ast::CastExpression result;
      result.line_start = current->getLineStart();
      result.column_start = current->getColumnStart();
      result.associatedLevel = table->current;
      result.base = core::move(current);
      if (it + 1 == end) {
        return eofError(it);
      }
      ++it;
      auto body = parseType(it, end, table);
      if (body.failed())
        return body;
      result.newType = core::move(body.getValue());
      result.line_end = result.newType->getLineEnd();
      result.column_end = result.newType->getColumnEnd();
      current = core::UniquePtr<ast::Expression>::create();
      *current = core::move(result);
      break;
    }

    case ast::tokens::TokenType::BITCAST: {
      ast::BitCastExpression result;
      result.line_start = current->getLineStart();
      result.column_start = current->getColumnStart();
      result.associatedLevel = table->current;
      result.base = core::move(current);
      if (it + 1 == end) {
        return eofError(it);
      }
      ++it;
      auto body = parseType(it, end, table);
      if (body.failed())
        return body;
      result.newType = core::move(body.getValue());
      result.line_end = result.newType->getLineEnd();
      result.column_end = result.newType->getColumnEnd();
      current = core::UniquePtr<ast::Expression>::create();
      *current = core::move(result);
      break;
    }

    default: {
      return current;
    }
    }
  }
  return current;
}

core::LogicalResult<core::UniquePtr<ast::Expression>>
parsePrefixExpression(core::Vector<ast::tokens::Token>::Iterator &it,
                      core::Vector<ast::tokens::Token>::Iterator end,
                      core::UniquePtr<ast::symbolTable::Table> &table) {
  switch (it->type) {
  case ast::tokens::TokenType::DOUBLE_PLUS: {
    if (it + 1 == end) {
      return eofError(it);
    }
    ast::PreIncrExpression result;
    result.line_start = it->start_line;
    result.column_start = it->start_column;
    result.associatedLevel = table->current;
    ++it;
    auto base = parsePrefixExpression(it, end, table);
    if (base.failed())
      return base;
    result.base = core::move(base.getValue());
    result.line_end = result.base->getLineEnd();
    result.column_end = result.base->getColumnEnd();
    auto resExpr = core::UniquePtr<ast::Expression>::create();
    *resExpr = core::move(result);
    return resExpr;
  }

  case ast::tokens::TokenType::DOUBLE_MINUS: {
    if (it + 1 == end) {
      return eofError(it);
    }
    ast::PreDecrExpression result;
    result.line_start = it->start_line;
    result.column_start = it->start_column;
    result.associatedLevel = table->current;
    ++it;
    auto base = parsePrefixExpression(it, end, table);
    if (base.failed())
      return base;
    result.base = core::move(base.getValue());
    result.line_end = result.base->getLineEnd();
    result.column_end = result.base->getColumnEnd();
    auto resExpr = core::UniquePtr<ast::Expression>::create();
    *resExpr = core::move(result);
    return resExpr;
  }

  case ast::tokens::TokenType::STAR: {
    if (it + 1 == end) {
      return eofError(it);
    }
    ast::AddrofExpression result;
    result.line_start = it->start_line;
    result.column_start = it->start_column;
    result.associatedLevel = table->current;
    ++it;
    auto base = parsePrefixExpression(it, end, table);
    if (base.failed())
      return base;
    result.base = core::move(base.getValue());
    result.line_end = result.base->getLineEnd();
    result.column_end = result.base->getColumnEnd();
    auto resExpr = core::UniquePtr<ast::Expression>::create();
    *resExpr = core::move(result);
    return resExpr;
  }

  case ast::tokens::TokenType::SYSCALL: {
    ast::SyscallExpression result;
    result.line_start = it->start_line;
    result.column_start = it->start_column;
    result.associatedLevel = table->current;
    if (it + 1 == end) {
      return eofError(it);
    }
    ++it;
    if (it->type != ast::tokens::TokenType::LP) {
      return wrongTokenError<ast::tokens::TokenType::LP>(it);
    }
    if (it + 1 == end) {
      return eofError(it);
    }
    ++it;
    auto code = parseExpression(it, end, table);
    if (it == end) {
      auto &value = code.getValue();
      return eofError(value->getLineEnd(), value->getColumnEnd());
    }
    if (code.failed())
      return code;
    result.code = core::move(code.getValue());
    bool finished = it->type == ast::tokens::TokenType::RP;
    while (!finished) {
      switch (it->type) {
      case ast::tokens::TokenType::RP: {
        finished = true;
        break;
      }

      case ast::tokens::TokenType::COMMA: {
        if (it + 1 == end) {
          return eofError(it);
        }
        ++it;
        auto arg = parseExpression(it, end, table);
        if (arg.failed())
          return arg;
        if (it == end) {
          auto &value = arg.getValue();
          return eofError(value->getLineEnd(), value->getColumnEnd());
        }
        result.args.pushBack(core::move(arg.getValue()));
        break;
      }

      default: {
        return wrongTokenError<ast::tokens::TokenType::RP,
                               ast::tokens::TokenType::COMMA>(it);
      }
      }
    }
    result.line_end = it->end_line;
    result.column_end = it->end_column;
    ++it;
    auto resExpr = core::UniquePtr<ast::Expression>::create();
    *resExpr = core::move(result);
    return resExpr;
  }

  case ast::tokens::TokenType::SIZEOF: {
    ast::SizeofExpression result;
    result.line_start = it->start_line;
    result.column_start = it->start_column;
    result.associatedLevel = table->current;

    if (it + 1 == end) {
      return eofError(it);
    }
    ++it;
    if (it->type != ast::tokens::TokenType::LP) {
      return wrongTokenError<ast::tokens::TokenType::LP>(it);
    }
    if (it + 1 == end) {
      return eofError(it);
    }
    ++it;
    auto t = parseType(it, end, table);
    if (t.failed())
      return t;
    if (it == end) {
      auto &value = t.getValue();
      return eofError(value->getLineEnd(), value->getColumnEnd());
    }
    result.arg = core::move(t.getValue());
    if (it->type != ast::tokens::TokenType::RP) {
      return wrongTokenError<ast::tokens::TokenType::RP>(it);
    }
    result.line_end = it->end_line;
    result.column_end = it->end_column;
    ++it;
    auto resExpr = core::UniquePtr<ast::Expression>::create();
    *resExpr = core::move(result);
    return resExpr;
  }

  case ast::tokens::TokenType::EXCLAMATION: {
    if (it + 1 == end) {
      return eofError(it);
    }
    ast::LnotExpression result;
    result.line_start = it->start_line;
    result.column_start = it->start_column;
    result.associatedLevel = table->current;
    ++it;
    auto base = parsePrefixExpression(it, end, table);
    if (base.failed())
      return base;
    result.base = core::move(base.getValue());
    result.line_end = result.base->getLineEnd();
    result.column_end = result.base->getColumnEnd();
    auto resExpr = core::UniquePtr<ast::Expression>::create();
    *resExpr = core::move(result);
    return resExpr;
  }

  case ast::tokens::TokenType::TILDE: {
    if (it + 1 == end) {
      return eofError(it);
    }
    ast::BnotExpression result;
    result.line_start = it->start_line;
    result.column_start = it->start_column;
    result.associatedLevel = table->current;
    ++it;
    auto base = parsePrefixExpression(it, end, table);
    if (base.failed())
      return base;
    result.base = core::move(base.getValue());
    result.line_end = result.base->getLineEnd();
    result.column_end = result.base->getColumnEnd();
    auto resExpr = core::UniquePtr<ast::Expression>::create();
    *resExpr = core::move(result);
    return resExpr;
  }

  default: {
    return parsePostfixExpression(it, end, table);
  }
  }
}

core::LogicalResult<core::UniquePtr<ast::Expression>>
parseMulExpression(core::Vector<ast::tokens::Token>::Iterator &it,
                   core::Vector<ast::tokens::Token>::Iterator end,
                   core::UniquePtr<ast::symbolTable::Table> &table) {
  auto base = parsePrefixExpression(it, end, table);
  if (base.failed())
    return base;
  core::UniquePtr<ast::Expression> current = core::move(base.getValue());
  while (it != end) {
    switch (it->type) {
    case ast::tokens::TokenType::STAR: {
      auto newCurrent =
          parseBinexprAux<ast::MulExpression, parsePrefixExpression>(
              it, end, table, core::move(current));
      if (newCurrent.failed())
        return newCurrent;
      current = core::move(newCurrent.getValue());
      break;
    }

    case ast::tokens::TokenType::SLASH: {
      auto newCurrent =
          parseBinexprAux<ast::DivExpression, parsePrefixExpression>(
              it, end, table, core::move(current));
      if (newCurrent.failed())
        return newCurrent;
      current = core::move(newCurrent.getValue());
      break;
    }

    case ast::tokens::TokenType::PERCENT: {
      auto newCurrent =
          parseBinexprAux<ast::ModExpression, parsePrefixExpression>(
              it, end, table, core::move(current));
      if (newCurrent.failed())
        return newCurrent;
      current = core::move(newCurrent.getValue());
      break;
    }

    default: {
      return current;
    }
    }
  }
  return current;
}

core::LogicalResult<core::UniquePtr<ast::Expression>>
parseAddExpression(core::Vector<ast::tokens::Token>::Iterator &it,
                   core::Vector<ast::tokens::Token>::Iterator end,
                   core::UniquePtr<ast::symbolTable::Table> &table) {
  auto base = parseMulExpression(it, end, table);
  if (base.failed())
    return base;
  core::UniquePtr<ast::Expression> current = core::move(base.getValue());
  while (it != end) {
    switch (it->type) {
    case ast::tokens::TokenType::PLUS: {
      auto newCurrent = parseBinexprAux<ast::AddExpression, parseMulExpression>(
          it, end, table, core::move(current));
      if (newCurrent.failed())
        return newCurrent;
      current = core::move(newCurrent.getValue());
      break;
    }

    case ast::tokens::TokenType::MINUS: {
      auto newCurrent = parseBinexprAux<ast::SubExpression, parseMulExpression>(
          it, end, table, core::move(current));
      if (newCurrent.failed())
        return newCurrent;
      current = core::move(newCurrent.getValue());
      break;
    }

    default: {
      return current;
    }
    }
  }
  return current;
}

core::LogicalResult<core::UniquePtr<ast::Expression>>
parseShiftExpression(core::Vector<ast::tokens::Token>::Iterator &it,
                     core::Vector<ast::tokens::Token>::Iterator end,
                     core::UniquePtr<ast::symbolTable::Table> &table) {
  auto base = parseAddExpression(it, end, table);
  if (base.failed())
    return base;
  core::UniquePtr<ast::Expression> current = core::move(base.getValue());
  while (it != end) {
    switch (it->type) {
    case ast::tokens::TokenType::DOUBLE_LAB: {
      auto newCurrent =
          parseBinexprAux<ast::LshiftExpression, parseAddExpression>(
              it, end, table, core::move(current));
      if (newCurrent.failed())
        return newCurrent;
      current = core::move(newCurrent.getValue());
      break;
    }

    case ast::tokens::TokenType::DOUBLE_RAB: {
      auto newCurrent =
          parseBinexprAux<ast::RshiftExpression, parseAddExpression>(
              it, end, table, core::move(current));
      if (newCurrent.failed())
        return newCurrent;
      current = core::move(newCurrent.getValue());
      break;
    }

    default: {
      return current;
    }
    }
  }
  return current;
}

core::LogicalResult<core::UniquePtr<ast::Expression>>
parseComparisonExpression(core::Vector<ast::tokens::Token>::Iterator &it,
                          core::Vector<ast::tokens::Token>::Iterator end,
                          core::UniquePtr<ast::symbolTable::Table> &table) {
  auto base = parseShiftExpression(it, end, table);
  if (base.failed())
    return base;
  core::UniquePtr<ast::Expression> current = core::move(base.getValue());
  while (it != end) {
    switch (it->type) {
    case ast::tokens::TokenType::LAB: {
      auto newCurrent =
          parseBinexprAux<ast::LtExpression, parseShiftExpression>(
              it, end, table, core::move(current));
      if (newCurrent.failed())
        return newCurrent;
      current = core::move(newCurrent.getValue());
      break;
    }

    case ast::tokens::TokenType::LAB_EQUAL: {
      auto newCurrent =
          parseBinexprAux<ast::LeExpression, parseShiftExpression>(
              it, end, table, core::move(current));
      if (newCurrent.failed())
        return newCurrent;
      current = core::move(newCurrent.getValue());
      break;
    }
    case ast::tokens::TokenType::RAB: {
      auto newCurrent =
          parseBinexprAux<ast::GtExpression, parseShiftExpression>(
              it, end, table, core::move(current));
      if (newCurrent.failed())
        return newCurrent;
      current = core::move(newCurrent.getValue());
      break;
    }

    case ast::tokens::TokenType::RAB_EQUAL: {
      auto newCurrent =
          parseBinexprAux<ast::GeExpression, parseShiftExpression>(
              it, end, table, core::move(current));
      if (newCurrent.failed())
        return newCurrent;
      current = core::move(newCurrent.getValue());
      break;
    }

    default: {
      return current;
    }
    }
  }
  return current;
}

core::LogicalResult<core::UniquePtr<ast::Expression>>
parseEqualityExpression(core::Vector<ast::tokens::Token>::Iterator &it,
                        core::Vector<ast::tokens::Token>::Iterator end,
                        core::UniquePtr<ast::symbolTable::Table> &table) {
  auto base = parseComparisonExpression(it, end, table);
  if (base.failed())
    return base;
  core::UniquePtr<ast::Expression> current = core::move(base.getValue());
  while (it != end) {
    switch (it->type) {
    case ast::tokens::TokenType::DOUBLE_EQUAL: {
      auto newCurrent =
          parseBinexprAux<ast::EqExpression, parseComparisonExpression>(
              it, end, table, core::move(current));
      if (newCurrent.failed())
        return newCurrent;
      current = core::move(newCurrent.getValue());
      break;
    }

    case ast::tokens::TokenType::EXCLAMATION_EQUAL: {
      auto newCurrent =
          parseBinexprAux<ast::NeqExpression, parseComparisonExpression>(
              it, end, table, core::move(current));
      if (newCurrent.failed())
        return newCurrent;
      current = core::move(newCurrent.getValue());
      break;
    }

    default: {
      return current;
    }
    }
  }
  return current;
}

core::LogicalResult<core::UniquePtr<ast::Expression>>
parseBandExpression(core::Vector<ast::tokens::Token>::Iterator &it,
                    core::Vector<ast::tokens::Token>::Iterator end,
                    core::UniquePtr<ast::symbolTable::Table> &table) {
  auto base = parseEqualityExpression(it, end, table);
  if (base.failed())
    return base;
  core::UniquePtr<ast::Expression> current = core::move(base.getValue());
  while (it != end) {
    switch (it->type) {
    case ast::tokens::TokenType::AMPERSAND: {
      auto newCurrent =
          parseBinexprAux<ast::BandExpression, parseEqualityExpression>(
              it, end, table, core::move(current));
      if (newCurrent.failed())
        return newCurrent;
      current = core::move(newCurrent.getValue());
      break;
    }

    default: {
      return current;
    }
    }
  }
  return current;
}

core::LogicalResult<core::UniquePtr<ast::Expression>>
parseBxorExpression(core::Vector<ast::tokens::Token>::Iterator &it,
                    core::Vector<ast::tokens::Token>::Iterator end,
                    core::UniquePtr<ast::symbolTable::Table> &table) {
  auto base = parseBandExpression(it, end, table);
  if (base.failed())
    return base;
  core::UniquePtr<ast::Expression> current = core::move(base.getValue());
  while (it != end) {
    switch (it->type) {
    case ast::tokens::TokenType::WEDGE: {
      auto newCurrent =
          parseBinexprAux<ast::BxorExpression, parseBandExpression>(
              it, end, table, core::move(current));
      if (newCurrent.failed())
        return newCurrent;
      current = core::move(newCurrent.getValue());
      break;
    }

    default: {
      return current;
    }
    }
  }
  return current;
}

core::LogicalResult<core::UniquePtr<ast::Expression>>
parseBorExpression(core::Vector<ast::tokens::Token>::Iterator &it,
                   core::Vector<ast::tokens::Token>::Iterator end,
                   core::UniquePtr<ast::symbolTable::Table> &table) {
  auto base = parseBxorExpression(it, end, table);
  if (base.failed())
    return base;
  core::UniquePtr<ast::Expression> current = core::move(base.getValue());
  while (it != end) {
    switch (it->type) {
    case ast::tokens::TokenType::PIPE: {
      auto newCurrent =
          parseBinexprAux<ast::BorExpression, parseBxorExpression>(
              it, end, table, core::move(current));
      if (newCurrent.failed())
        return newCurrent;
      current = core::move(newCurrent.getValue());
      break;
    }

    default: {
      return current;
    }
    }
  }
  return current;
}

core::LogicalResult<core::UniquePtr<ast::Expression>>
parseLandExpression(core::Vector<ast::tokens::Token>::Iterator &it,
                    core::Vector<ast::tokens::Token>::Iterator end,
                    core::UniquePtr<ast::symbolTable::Table> &table) {
  auto base = parseBorExpression(it, end, table);
  if (base.failed())
    return base;
  core::UniquePtr<ast::Expression> current = core::move(base.getValue());
  while (it != end) {
    switch (it->type) {
    case ast::tokens::TokenType::DOUBLE_AMPERSAND: {
      auto newCurrent =
          parseBinexprAux<ast::LandExpression, parseBorExpression>(
              it, end, table, core::move(current));
      if (newCurrent.failed())
        return newCurrent;
      current = core::move(newCurrent.getValue());
      break;
    }

    default: {
      return current;
    }
    }
  }
  return current;
}

core::LogicalResult<core::UniquePtr<ast::Expression>>
parseLxorExpression(core::Vector<ast::tokens::Token>::Iterator &it,
                    core::Vector<ast::tokens::Token>::Iterator end,
                    core::UniquePtr<ast::symbolTable::Table> &table) {
  auto base = parseLandExpression(it, end, table);
  if (base.failed())
    return base;
  core::UniquePtr<ast::Expression> current = core::move(base.getValue());
  while (it != end) {
    switch (it->type) {
    case ast::tokens::TokenType::DOUBLE_WEDGE: {
      auto newCurrent =
          parseBinexprAux<ast::LxorExpression, parseLandExpression>(
              it, end, table, core::move(current));
      if (newCurrent.failed())
        return newCurrent;
      current = core::move(newCurrent.getValue());
      break;
    }

    default: {
      return current;
    }
    }
  }
  return current;
}

core::LogicalResult<core::UniquePtr<ast::Expression>>
parseLorExpression(core::Vector<ast::tokens::Token>::Iterator &it,
                   core::Vector<ast::tokens::Token>::Iterator end,
                   core::UniquePtr<ast::symbolTable::Table> &table) {
  auto base = parseLxorExpression(it, end, table);
  if (base.failed())
    return base;
  core::UniquePtr<ast::Expression> current = core::move(base.getValue());
  while (it != end) {
    switch (it->type) {
    case ast::tokens::TokenType::DOUBLE_PIPE: {
      auto newCurrent =
          parseBinexprAux<ast::LorExpression, parseLxorExpression>(
              it, end, table, core::move(current));
      if (newCurrent.failed())
        return newCurrent;
      current = core::move(newCurrent.getValue());
      break;
    }

    default: {
      return current;
    }
    }
  }
  return current;
}

core::LogicalResult<core::UniquePtr<ast::Expression>>
parseTernaryExpression(core::Vector<ast::tokens::Token>::Iterator &it,
                       core::Vector<ast::tokens::Token>::Iterator end,
                       core::UniquePtr<ast::symbolTable::Table> &table) {
  auto cond = parseLorExpression(it, end, table);
  if (cond.failed())
    return cond;
  if ((it != end) && (it->type == ast::tokens::TokenType::QUESTION)) {
    auto result = core::UniquePtr<ast::TernaryExpression>::create();
    result->cond = core::move(cond.getValue());
    result->line_start = result->cond->getLineStart();
    result->column_start = result->cond->getColumnStart();
    result->associatedLevel = table->current;
    if (it + 1 == end) {
      return eofError(it);
    }
    ++it;
    auto thenPart = parseExpression(it, end, table);
    if (thenPart.failed())
      return thenPart;
    if (it == end) {
      auto &value = thenPart.getValue();
      return eofError(value->getLineEnd(), value->getColumnEnd());
    }
    result->thenPart = core::move(thenPart.getValue());
    if (it->type != ast::tokens::TokenType::COLON) {
      return wrongTokenError<ast::tokens::TokenType::COLON>(it);
    }
    if (it + 1 == end) {
      return eofError(it);
    }
    ++it;
    auto elsePart = parseTernaryExpression(it, end, table);
    if (elsePart.failed())
      return elsePart;
    result->elsePart = core::move(elsePart.getValue());
    result->line_end = result->elsePart->getLineEnd();
    result->column_end = result->elsePart->getColumnEnd();
    auto resultExpr = core::UniquePtr<ast::Expression>::create();
    *resultExpr = core::move(*result);
    return resultExpr;
  }
  return cond;
}

core::LogicalResult<core::UniquePtr<ast::Expression>>
parseAssignExpression(core::Vector<ast::tokens::Token>::Iterator &it,
                      core::Vector<ast::tokens::Token>::Iterator end,
                      core::UniquePtr<ast::symbolTable::Table> &table) {
  auto base = parseTernaryExpression(it, end, table);
  if (base.failed())
    return base;
  core::UniquePtr<ast::Expression> current = core::move(base.getValue());

  while (it != end) {
    switch (it->type) {
    case ast::tokens::TokenType::EQUAL: {
      auto newCurrent =
          parseBinexprAux<ast::AffecExpression, parseTernaryExpression>(
              it, end, table, core::move(current));
      if (newCurrent.failed())
        return newCurrent;
      current = core::move(newCurrent.getValue());
      break;
    }
    case ast::tokens::TokenType::PLUS_EQUAL: {
      auto newCurrent =
          parseBinexprAux<ast::AddAffecExpression, parseTernaryExpression>(
              it, end, table, core::move(current));
      if (newCurrent.failed())
        return newCurrent;
      current = core::move(newCurrent.getValue());
      break;
    }
    case ast::tokens::TokenType::MINUS_EQUAL: {
      auto newCurrent =
          parseBinexprAux<ast::SubAffecExpression, parseTernaryExpression>(
              it, end, table, core::move(current));
      if (newCurrent.failed())
        return newCurrent;
      current = core::move(newCurrent.getValue());
      break;
    }
    case ast::tokens::TokenType::STAR_EQUAL: {
      auto newCurrent =
          parseBinexprAux<ast::MulAffecExpression, parseTernaryExpression>(
              it, end, table, core::move(current));
      if (newCurrent.failed())
        return newCurrent;
      current = core::move(newCurrent.getValue());
      break;
    }
    case ast::tokens::TokenType::PERCENT_EQUAL: {
      auto newCurrent =
          parseBinexprAux<ast::ModAffecExpression, parseTernaryExpression>(
              it, end, table, core::move(current));
      if (newCurrent.failed())
        return newCurrent;
      current = core::move(newCurrent.getValue());
      break;
    }
    case ast::tokens::TokenType::SLASH_EQUAL: {
      auto newCurrent =
          parseBinexprAux<ast::DivAffecExpression, parseTernaryExpression>(
              it, end, table, core::move(current));
      if (newCurrent.failed())
        return newCurrent;
      current = core::move(newCurrent.getValue());
      break;
    }
    case ast::tokens::TokenType::DOUBLE_AMPERSAND_EQUAL: {
      auto newCurrent =
          parseBinexprAux<ast::LandAffecExpression, parseTernaryExpression>(
              it, end, table, core::move(current));
      if (newCurrent.failed())
        return newCurrent;
      current = core::move(newCurrent.getValue());
      break;
    }
    case ast::tokens::TokenType::DOUBLE_PIPE_EQUAL: {
      auto newCurrent =
          parseBinexprAux<ast::LorAffecExpression, parseTernaryExpression>(
              it, end, table, core::move(current));
      if (newCurrent.failed())
        return newCurrent;
      current = core::move(newCurrent.getValue());
      break;
    }
    case ast::tokens::TokenType::DOUBLE_WEDGE_EQUAL: {
      auto newCurrent =
          parseBinexprAux<ast::LxorAffecExpression, parseTernaryExpression>(
              it, end, table, core::move(current));
      if (newCurrent.failed())
        return newCurrent;
      current = core::move(newCurrent.getValue());
      break;
    }
    case ast::tokens::TokenType::AMPERSAND_EQUAL: {
      auto newCurrent =
          parseBinexprAux<ast::BandAffecExpression, parseTernaryExpression>(
              it, end, table, core::move(current));
      if (newCurrent.failed())
        return newCurrent;
      current = core::move(newCurrent.getValue());
      break;
    }
    case ast::tokens::TokenType::PIPE_EQUAL: {
      auto newCurrent =
          parseBinexprAux<ast::BorAffecExpression, parseTernaryExpression>(
              it, end, table, core::move(current));
      if (newCurrent.failed())
        return newCurrent;
      current = core::move(newCurrent.getValue());
      break;
    }
    case ast::tokens::TokenType::WEDGE_EQUAL: {
      auto newCurrent =
          parseBinexprAux<ast::BxorAffecExpression, parseTernaryExpression>(
              it, end, table, core::move(current));
      if (newCurrent.failed())
        return newCurrent;
      current = core::move(newCurrent.getValue());
      break;
    }
    case ast::tokens::TokenType::DOUBLE_LAB_EQUAL: {
      auto newCurrent =
          parseBinexprAux<ast::LshiftAffecExpression, parseTernaryExpression>(
              it, end, table, core::move(current));
      if (newCurrent.failed())
        return newCurrent;
      current = core::move(newCurrent.getValue());
      break;
    }
    case ast::tokens::TokenType::DOUBLE_RAB_EQUAL: {
      auto newCurrent =
          parseBinexprAux<ast::RshiftAffecExpression, parseTernaryExpression>(
              it, end, table, core::move(current));
      if (newCurrent.failed())
        return newCurrent;
      current = core::move(newCurrent.getValue());
      break;
    }
    default: {
      return current;
    }
    }
  }
  return current;
}

core::LogicalResult<core::UniquePtr<ast::Expression>>
parseExpression(core::Vector<ast::tokens::Token>::Iterator &it,
                core::Vector<ast::tokens::Token>::Iterator end,
                core::UniquePtr<ast::symbolTable::Table> &table) {
  return parseAssignExpression(it, end, table);
}

core::LogicalResult<core::UniquePtr<ast::Type>>
parseType(core::Vector<ast::tokens::Token>::Iterator &it,
          core::Vector<ast::tokens::Token>::Iterator end,
          core::UniquePtr<ast::symbolTable::Table> &table) {
  auto current = core::UniquePtr<ast::Type>::create();
  switch (it->type) {
  case ast::tokens::TokenType::VOID: {
    ast::VoidType t;
    t.associatedLevel = table->current;
    t.line_start = it->start_line;
    t.column_start = it->start_column;
    t.line_end = it->end_line;
    t.column_end = it->end_column;
    *current = t;
    ++it;
    break;
  }

  case ast::tokens::TokenType::BIT: {
    ast::BitType t;
    t.associatedLevel = table->current;
    t.line_start = it->start_line;
    t.column_start = it->start_column;
    t.line_end = it->end_line;
    t.column_end = it->end_column;
    *current = t;
    ++it;
    break;
  }

  case ast::tokens::TokenType::STRUCT: {
    ast::StructType t;
    t.associatedLevel = table->current;
    t.line_start = it->start_line;
    t.column_start = it->start_column;
    if (it + 1 == end) {
      return eofError(it);
    }
    ++it;
    if (it->type != ast::tokens::TokenType::ID) {
      return wrongTokenError<ast::tokens::TokenType::ID>(it);
    }
    t.name = it->s;
    t.line_end = it->end_line;
    t.column_end = it->end_column;
    *current = t;
    ++it;
    break;
  }

  case ast::tokens::TokenType::UNION: {
    ast::UnionType t;
    t.associatedLevel = table->current;
    t.line_start = it->start_line;
    t.column_start = it->start_column;
    if (it + 1 == end) {
      return eofError(it);
    }
    ++it;
    if (it->type != ast::tokens::TokenType::ID) {
      return wrongTokenError<ast::tokens::TokenType::ID>(it);
    }
    t.name = it->s;
    t.line_end = it->end_line;
    t.column_end = it->end_column;
    *current = t;
    ++it;
    break;
  }

  case ast::tokens::TokenType::ID: {
    auto alias = table->getAlias(it->s);
    if (alias.failed())
      return alias;
    current = core::move(alias.getValue());
    ++it;
    break;
  }

  default: {
    return wrongTokenError<
        ast::tokens::TokenType::VOID, ast::tokens::TokenType::BIT,
        ast::tokens::TokenType::STRUCT, ast::tokens::TokenType::UNION,
        ast::tokens::TokenType::ID>(it);
  }
  }
  while (it != end) {
    switch (it->type) {
    case ast::tokens::TokenType::STAR: {
      auto ptrType = core::UniquePtr<ast::PtrType>::create();
      ptrType->line_start = current->getLineStart();
      ptrType->column_start = current->getColumnStart();
      ptrType->associatedLevel = table->current;
      ptrType->line_end = it->end_line;
      ptrType->column_end = it->end_column;
      ptrType->base = core::move(current);
      current = core::UniquePtr<ast::Type>::create();
      *current = core::move(*ptrType);
      ++it;
      break;
    }

    case ast::tokens::TokenType::LSB: {
      auto arrayType = core::UniquePtr<ast::ArrayType>::create();
      arrayType->line_start = current->getLineStart();
      arrayType->column_start = current->getColumnStart();
      arrayType->associatedLevel = table->current;
      arrayType->base = core::move(current);

      if (it + 1 == end) {
        return eofError(it);
      }
      ++it;
      auto size = parseExpression(it, end, table);
      if (size.failed())
        return size;
      if (it == end) {
        auto &value = size.getValue();
        return eofError(value->getLineEnd(), value->getColumnEnd());
      }
      arrayType->size = core::move(size.getValue());
      if (it->type != ast::tokens::TokenType::RSB) {
        return wrongTokenError<ast::tokens::TokenType::RSB>(it);
      }
      arrayType->line_end = it->end_line;
      arrayType->column_end = it->end_column;
      ++it;
      current = core::UniquePtr<ast::Type>::create();
      *current = core::move(*arrayType);
      break;
    }

    default: {
      return current;
    }
    }
  }
  return current;
}

core::LogicalResult<core::UniquePtr<ast::ProgramItem>>
parseAliasDeclaration(core::Vector<ast::tokens::Token>::Iterator &it,
                      core::Vector<ast::tokens::Token>::Iterator end,
                      core::UniquePtr<ast::symbolTable::Table> &table) {
  if (it->type != ast::tokens::TokenType::ALIAS) {
    return wrongTokenError<ast::tokens::TokenType::ALIAS>(it);
  }
  if (it + 1 == end) {
    return eofError(it);
  }
  ++it;
  if (it->type != ast::tokens::TokenType::ID) {
    return wrongTokenError<ast::tokens::TokenType::ID>(it);
  }
  core::String name = it->s;
  if (it + 1 == end) {
    return eofError(it);
  }
  ++it;
  if (it->type != ast::tokens::TokenType::EQUAL) {
    return wrongTokenError<ast::tokens::TokenType::EQUAL>(it);
  }
  if (it + 1 == end) {
    return eofError(it);
  }
  ++it;
  auto base = parseType(it, end, table);
  if (base.failed())
    return base;
  table->insertAlias(name, core::move(base.getValue()));
  return static_cast<core::UniquePtr<ast::ProgramItem>>(nullptr);
}

core::LogicalResult<core::UniquePtr<ast::OperationDeclaration>>
parseOperationdeclaration(core::Vector<ast::tokens::Token>::Iterator &it,
                          core::Vector<ast::tokens::Token>::Iterator end,
                          core::UniquePtr<ast::symbolTable::Table> &table) {
  if (it->type != ast::tokens::TokenType::OPERATOR) {
    return wrongTokenError<ast::tokens::TokenType::OPERATOR>(it);
  }
  auto result = core::UniquePtr<ast::OperationDeclaration>::create();
  result->line_start = it->start_line;
  result->column_start = it->start_column;
  result->associatedLevel = table->current;
  if (it + 1 == end) {
    return eofError(it);
  }
  ++it;
  switch (it->type) {
  case ast::tokens::TokenType::PLUS: {
    result->opType = ast::OperationDeclaration::OperatorType::ADD_OP;
    break;
  }
  case ast::tokens::TokenType::MINUS: {
    result->opType = ast::OperationDeclaration::OperatorType::SUB_OP;
    break;
  }
  case ast::tokens::TokenType::STAR: {
    result->opType = ast::OperationDeclaration::OperatorType::MUL_OP;
    break;
  }
  case ast::tokens::TokenType::PERCENT: {
    result->opType = ast::OperationDeclaration::OperatorType::MOD_OP;
    break;
  }
  case ast::tokens::TokenType::SLASH: {
    result->opType = ast::OperationDeclaration::OperatorType::DIV_OP;
    break;
  }
  case ast::tokens::TokenType::DOUBLE_AMPERSAND: {
    result->opType = ast::OperationDeclaration::OperatorType::LAND_OP;
    break;
  }
  case ast::tokens::TokenType::DOUBLE_PIPE: {
    result->opType = ast::OperationDeclaration::OperatorType::LOR_OP;
    break;
  }
  case ast::tokens::TokenType::DOUBLE_WEDGE: {
    result->opType = ast::OperationDeclaration::OperatorType::LXOR_OP;
    break;
  }
  case ast::tokens::TokenType::EXCLAMATION: {
    result->opType = ast::OperationDeclaration::OperatorType::LNOT_OP;
    break;
  }
  case ast::tokens::TokenType::DOUBLE_EQUAL: {
    result->opType = ast::OperationDeclaration::OperatorType::EQ_OP;
    break;
  }
  case ast::tokens::TokenType::LAB: {
    result->opType = ast::OperationDeclaration::OperatorType::LT_OP;
    break;
  }
  default: {
    return wrongTokenError<
        ast::tokens::TokenType::PLUS, ast::tokens::TokenType::MINUS,
        ast::tokens::TokenType::STAR, ast::tokens::TokenType::PERCENT,
        ast::tokens::TokenType::SLASH, ast::tokens::TokenType::DOUBLE_AMPERSAND,
        ast::tokens::TokenType::DOUBLE_PIPE,
        ast::tokens::TokenType::DOUBLE_WEDGE,
        ast::tokens::TokenType::EXCLAMATION,
        ast::tokens::TokenType::DOUBLE_EQUAL, ast::tokens::TokenType::LAB>(it);
  }
  }
  if (it + 1 == end) {
    return eofError(it);
  }
  ++it;
  if (it->type != ast::tokens::TokenType::LP) {
    return wrongTokenError<ast::tokens::TokenType::LP>(it);
  }
  if (it + 1 == end) {
    return eofError(it);
  }
  ++it;
  if (it->type != ast::tokens::TokenType::RP) {
    return wrongTokenError<ast::tokens::TokenType::RP>(it);
  }
  if (it + 1 == end) {
    return eofError(it);
  }
  ++it;
  auto body = parseCompoundStmt(it, end, table);
  if (body.failed())
    return body;
  result->body = core::move(body.getValue());
  result->line_end = result->body->getLineEnd();
  result->column_end = result->body->getColumnEnd();
  return result;
}

core::LogicalResult<core::UniquePtr<ast::CastDeclaration>>
parseCastDeclaration(core::Vector<ast::tokens::Token>::Iterator &it,
                     core::Vector<ast::tokens::Token>::Iterator end,
                     core::UniquePtr<ast::symbolTable::Table> &table) {
  if (it->type != ast::tokens::TokenType::CAST) {
    return wrongTokenError<ast::tokens::TokenType::CAST>(it);
  }
  auto result = core::UniquePtr<ast::CastDeclaration>::create();
  result->line_start = it->start_line;
  result->column_start = it->start_column;
  result->associatedLevel = table->current;
  if (it + 1 == end) {
    return eofError(it);
  }
  ++it;
  auto t = parseType(it, end, table);
  if (t.failed())
    return t;
  if (it == end) {
    auto &value = t.getValue();
    return eofError(value->getLineEnd(), value->getColumnEnd());
  }
  result->type = core::move(t.getValue());
  auto body = parseCompoundStmt(it, end, table);
  if (body.failed())
    return body;
  result->body = core::move(body.getValue());
  result->line_end = result->body->getLineEnd();
  result->column_end = result->body->getColumnEnd();
  return result;
}

core::LogicalResult<core::UniquePtr<ast::StructDeclaration>>
parseStructDeclaration(core::Vector<ast::tokens::Token>::Iterator &it,
                       core::Vector<ast::tokens::Token>::Iterator end,
                       core::UniquePtr<ast::symbolTable::Table> &table) {
  if (it->type != ast::tokens::TokenType::STRUCT) {
    return wrongTokenError<ast::tokens::TokenType::STRUCT>(it);
  }
  auto result = core::UniquePtr<ast::StructDeclaration>::create();
  result->line_start = it->start_line;
  result->column_start = it->start_column;
  result->associatedLevel = table->current;
  if (it + 1 == end) {
    return eofError(it);
  }
  ++it;
  if (it->type != ast::tokens::TokenType::ID) {
    return wrongTokenError<ast::tokens::TokenType::ID>(it);
  }
  result->name = it->s;
  if (it + 1 == end) {
    return eofError(it);
  }
  ++it;
  if (it->type != ast::tokens::TokenType::LB) {
    return wrongTokenError<ast::tokens::TokenType::LB>(it);
  }
  if (it + 1 == end) {
    return eofError(it);
  }
  ++it;
  table->initialize();
  while (it->type != ast::tokens::TokenType::RB) {
    switch (it->type) {
    case ast::tokens::TokenType::VAR: {
      auto field = parseVariableDeclaration(it, end, table);
      if (field.failed())
        return field;
      if (it == end) {
        auto &value = field.getValue();
        return eofError(value->getLineEnd(), value->getColumnEnd());
      }
      result->fields.pushBack(core::move(field.getValue()));
      break;
    }

    case ast::tokens::TokenType::OPERATOR: {
      auto op = parseOperationdeclaration(it, end, table);
      if (op.failed())
        return op;
      if (it == end) {
        auto &value = op.getValue();
        return eofError(value->getLineEnd(), value->getColumnEnd());
      }
      result->ops.pushBack(core::move(op.getValue()));
      break;
    }

    case ast::tokens::TokenType::FUN: {
      auto method = parseFunctionDeclaration(it, end, table);
      if (method.failed())
        return method;
      if (it == end) {
        auto &value = method.getValue();
        return eofError(value->getLineEnd(), value->getColumnEnd());
      }
      result->methods.pushBack(core::move(method.getValue()));
      break;
    }

    case ast::tokens::TokenType::CAST: {
      auto cast = parseCastDeclaration(it, end, table);
      if (cast.failed())
        return cast;
      if (it == end) {
        auto &value = cast.getValue();
        return eofError(value->getLineEnd(), value->getColumnEnd());
      }
      result->casts.pushBack(core::move(cast.getValue()));
      break;
    }

    default: {
      return wrongTokenError<
          ast::tokens::TokenType::VAR, ast::tokens::TokenType::OPERATOR,
          ast::tokens::TokenType::FUN, ast::tokens::TokenType::CAST>(it);
    }
    }
  }
  table->finalize();
  result->line_end = it->end_line;
  result->column_end = it->end_column;
  ++it;
  table->structs.insert(result->name, result->clone());
  return result;
}

core::LogicalResult<core::UniquePtr<ast::UnionDeclaration>>
parseUnionDeclaration(core::Vector<ast::tokens::Token>::Iterator &it,
                      core::Vector<ast::tokens::Token>::Iterator end,
                      core::UniquePtr<ast::symbolTable::Table> &table) {
  if (it->type != ast::tokens::TokenType::UNION) {
    return wrongTokenError<ast::tokens::TokenType::UNION>(it);
  }
  auto result = core::UniquePtr<ast::UnionDeclaration>::create();
  result->line_start = it->start_line;
  result->column_start = it->start_column;
  result->associatedLevel = table->current;

  if (it + 1 == end) {
    return eofError(it);
  }
  ++it;
  if (it->type != ast::tokens::TokenType::ID) {
    return wrongTokenError<ast::tokens::TokenType::ID>(it);
  }
  result->name = it->s;
  if (it + 1 == end) {
    return eofError(it);
  }
  ++it;
  if (it->type != ast::tokens::TokenType::LB) {
    return wrongTokenError<ast::tokens::TokenType::LB>(it);
  }
  if (it + 1 == end) {
    return eofError(it);
  }
  ++it;
  bool finished = it->type == ast::tokens::TokenType::RB;
  while (!finished) {
    auto arg = parseArgument(it, end, table);
    if (arg.failed())
      return arg;
    if (it == end) {
      auto &value = arg.getValue();
      return eofError(value->getLineEnd(), value->getColumnEnd());
    }
    result->fields.pushBack(core::move(arg.getValue()));
    switch (it->type) {
    case ast::tokens::TokenType::COMMA: {
      if (it + 1 == end) {
        return eofError(it);
      }
      ++it;
      break;
    }

    case ast::tokens::TokenType::RB: {
      finished = true;
      break;
    }

    default: {
      return wrongTokenError<ast::tokens::TokenType::COMMA,
                             ast::tokens::TokenType::RB>(it);
    }
    }
  }
  result->line_end = it->end_line;
  result->column_end = it->end_column;
  ++it;
  table->unions.insert(result->name, result->clone());
  return result;
}

core::LogicalResult<core::UniquePtr<ast::WhileStmt>>
parseWhileStmt(core::Vector<ast::tokens::Token>::Iterator &it,
               core::Vector<ast::tokens::Token>::Iterator end,
               core::UniquePtr<ast::symbolTable::Table> &table) {
  if (it->type != ast::tokens::TokenType::WHILE) {
    return wrongTokenError<ast::tokens::TokenType::WHILE>(it);
  }
  auto result = core::UniquePtr<ast::WhileStmt>::create();
  result->line_start = it->start_line;
  result->column_start = it->start_column;
  result->associatedLevel = table->current;
  if (it + 1 == end) {
    return eofError(it);
  }
  ++it;
  if (it->type != ast::tokens::TokenType::LP) {
    return wrongTokenError<ast::tokens::TokenType::LP>(it);
  }
  if (it + 1 == end) {
    return eofError(it);
  }
  ++it;
  auto cond = parseExpression(it, end, table);
  if (cond.failed())
    return cond;
  if (it == end) {
    auto &value = cond.getValue();
    return eofError(value->getLineEnd(), value->getColumnEnd());
  }
  result->cond = core::move(cond.getValue());
  if (it->type != ast::tokens::TokenType::RP) {
    return wrongTokenError<ast::tokens::TokenType::RP>(it);
  }
  if (it + 1 == end) {
    return eofError(it);
  }
  ++it;
  table->initialize();
  auto body = parseStatement(it, end, table);
  if (body.failed())
    return body;
  if (it == end) {
    auto &value = body.getValue();
    return eofError(value->getLineEnd(), value->getColumnEnd());
  }
  table->finalize();
  result->body = core::move(body.getValue());
  result->line_end = result->body->getLineEnd();
  result->column_end = result->body->getColumnEnd();
  return result;
}

core::LogicalResult<core::UniquePtr<ast::DoWhileStmt>>
parseDoWhileStmt(core::Vector<ast::tokens::Token>::Iterator &it,
                 core::Vector<ast::tokens::Token>::Iterator end,
                 core::UniquePtr<ast::symbolTable::Table> &table) {
  if (it->type != ast::tokens::TokenType::DO) {
    return wrongTokenError<ast::tokens::TokenType::DO>(it);
  }
  auto result = core::UniquePtr<ast::DoWhileStmt>::create();
  result->line_start = it->start_line;
  result->column_start = it->start_column;
  result->associatedLevel = table->current;
  if (it + 1 == end) {
    return eofError(it);
  }
  ++it;
  table->initialize();
  auto body = parseStatement(it, end, table);
  table->finalize();
  if (body.failed())
    return body;
  if (it == end) {
    auto &value = body.getValue();
    return eofError(value->getLineEnd(), value->getColumnEnd());
  }
  result->body = core::move(body.getValue());
  if (it->type != ast::tokens::TokenType::WHILE) {
    return wrongTokenError<ast::tokens::TokenType::WHILE>(it);
  }
  if (it + 1 == end) {
    return eofError(it);
  }
  ++it;
  if (it->type != ast::tokens::TokenType::LP) {
    return wrongTokenError<ast::tokens::TokenType::LP>(it);
  }
  if (it + 1 == end) {
    return eofError(it);
  }
  ++it;
  auto cond = parseExpression(it, end, table);
  if (cond.failed())
    return cond;
  if (it == end) {
    auto &value = cond.getValue();
    return eofError(value->getLineEnd(), value->getColumnEnd());
  }
  if (it->type != ast::tokens::TokenType::RP) {
    return wrongTokenError<ast::tokens::TokenType::RP>(it);
  }
  result->cond = core::move(cond.getValue());
  result->line_end = it->end_line;
  result->column_end = it->end_column;
  ++it;
  return result;
}

core::LogicalResult<core::UniquePtr<ast::IfStmt>>
parseIfStmt(core::Vector<ast::tokens::Token>::Iterator &it,
            core::Vector<ast::tokens::Token>::Iterator end,
            core::UniquePtr<ast::symbolTable::Table> &table) {
  if (it->type != ast::tokens::TokenType::IF) {
    return wrongTokenError<ast::tokens::TokenType::IF>(it);
  }
  auto result = core::UniquePtr<ast::IfStmt>::create();
  result->line_start = it->start_line;
  result->column_start = it->start_column;
  result->associatedLevel = table->current;
  if (it + 1 == end) {
    return eofError(it);
  }
  ++it;
  if (it->type != ast::tokens::TokenType::LP) {
    return wrongTokenError<ast::tokens::TokenType::LP>(it);
  }
  if (it + 1 == end) {
    return eofError(it);
  }
  ++it;
  auto cond = parseExpression(it, end, table);
  if (cond.failed())
    return cond;
  if (it == end) {
    auto &value = cond.getValue();
    return eofError(value->getLineEnd(), value->getColumnEnd());
  }
  result->cond = core::move(cond.getValue());
  if (it->type != ast::tokens::TokenType::RP) {
    return wrongTokenError<ast::tokens::TokenType::RP>(it);
  }
  if (it + 1 == end) {
    return eofError(it);
  }
  ++it;
  table->initialize();
  auto thenPart = parseStatement(it, end, table);
  if (thenPart.failed())
    return thenPart;
  result->thenPart = core::move(thenPart.getValue());
  table->finalize();
  if ((it != end) && (it->type == ast::tokens::TokenType::ELSE)) {
    if (it + 1 == end) {
      return eofError(it);
    }
    ++it;
    table->initialize();
    auto elsePart = parseStatement(it, end, table);
    if (elsePart.failed())
      return elsePart;
    table->finalize();
    result->elsePart = core::move(elsePart.getValue());
    result->line_end = result->elsePart->getLineEnd();
    result->column_end = result->elsePart->getColumnEnd();
  } else {
    result->elsePart = nullptr;
    result->line_end = result->thenPart->getLineEnd();
    result->column_end = result->thenPart->getColumnEnd();
  }
  return result;
}

core::LogicalResult<core::UniquePtr<ast::SwitchCase>>
parseSwitchCase(core::Vector<ast::tokens::Token>::Iterator &it,
                core::Vector<ast::tokens::Token>::Iterator end,
                core::UniquePtr<ast::symbolTable::Table> &table) {
  if (it->type != ast::tokens::TokenType::CASE) {
    return wrongTokenError<ast::tokens::TokenType::CASE>(it);
  }
  auto result = core::UniquePtr<ast::SwitchCase>::create();
  result->line_start = it->start_line;
  result->column_start = it->start_column;
  result->associatedLevel = table->current;
  if (it + 1 == end) {
    return eofError(it);
  }
  ++it;
  auto cond = parseExpression(it, end, table);
  if (cond.failed())
    return cond;
  if (it == end) {
    auto &value = cond.getValue();
    return eofError(value->getLineEnd(), value->getColumnEnd());
  }
  result->cond = core::move(cond.getValue());
  if (it->type != ast::tokens::TokenType::COLON) {
    return wrongTokenError<ast::tokens::TokenType::COLON>(it);
  }
  if (it + 1 == end) {
    return eofError(it);
  }
  ++it;
  auto body = parseStatement(it, end, table);
  if (body.failed())
    return body;
  result->body = core::move(body.getValue());
  return result;
}

core::LogicalResult<core::UniquePtr<ast::SwitchDefault>>
parseSwitchDefault(core::Vector<ast::tokens::Token>::Iterator &it,
                   core::Vector<ast::tokens::Token>::Iterator end,
                   core::UniquePtr<ast::symbolTable::Table> &table) {
  if (it->type != ast::tokens::TokenType::DEFAULT) {
    return wrongTokenError<ast::tokens::TokenType::DEFAULT>(it);
  }
  auto result = core::UniquePtr<ast::SwitchDefault>::create();
  result->line_start = it->start_line;
  result->column_start = it->start_column;
  result->associatedLevel = table->current;
  table->initialize();
  if (it + 1 == end) {
    return eofError(it);
  }
  ++it;
  if (it->type != ast::tokens::TokenType::COLON) {
    return wrongTokenError<ast::tokens::TokenType::COLON>(it);
  }
  if (it + 1 == end) {
    return eofError(it);
  }
  ++it;
  auto body = parseStatement(it, end, table);
  if (body.failed())
    return body;
  result->body = core::move(body.getValue());
  result->line_end = result->body->getLineEnd();
  result->column_end = result->body->getColumnEnd();
  table->finalize();
  return result;
}

core::LogicalResult<core::UniquePtr<ast::SwitchItem>>
parseSwitchItem(core::Vector<ast::tokens::Token>::Iterator &it,
                core::Vector<ast::tokens::Token>::Iterator end,
                core::UniquePtr<ast::symbolTable::Table> &table) {
  switch (it->type) {
  case ast::tokens::TokenType::CASE: {
    auto res = parseSwitchCase(it, end, table);
    if (res.failed())
      return res;
    auto result = core::UniquePtr<ast::SwitchItem>::create();
    *result = core::move(*(res.getValue()));
    return result;
  }

  case ast::tokens::TokenType::DEFAULT: {
    auto res = parseSwitchDefault(it, end, table);
    if (res.failed())
      return res;
    auto result = core::UniquePtr<ast::SwitchItem>::create();
    *result = core::move(*(res.getValue()));
    return result;
  }

  default: {
    return wrongTokenError<ast::tokens::TokenType::CASE,
                           ast::tokens::TokenType::DEFAULT>(it);
  }
  }
}

core::LogicalResult<core::UniquePtr<ast::SwitchStmt>>
parseSwitchStmt(core::Vector<ast::tokens::Token>::Iterator &it,
                core::Vector<ast::tokens::Token>::Iterator end,
                core::UniquePtr<ast::symbolTable::Table> &table) {
  if (it->type != ast::tokens::TokenType::SWITCH) {
    return wrongTokenError<ast::tokens::TokenType::SWITCH>(it);
  }
  auto result = core::UniquePtr<ast::SwitchStmt>::create();
  result->line_start = it->start_line;
  result->column_start = it->start_column;
  result->associatedLevel = table->current;

  if (it + 1 == end) {
    return eofError(it);
  }
  ++it;
  if (it->type != ast::tokens::TokenType::LP)
    return wrongTokenError<ast::tokens::TokenType::LP>(it);
  if (it + 1 == end) {
    return eofError(it);
  }
  ++it;
  auto cond = parseExpression(it, end, table);
  if (cond.failed())
    return cond;
  if (it == end) {
    auto &value = cond.getValue();
    return eofError(value->getLineEnd(), value->getColumnEnd());
  }
  result->cond = core::move(cond.getValue());
  if (it->type != ast::tokens::TokenType::RP)
    return wrongTokenError<ast::tokens::TokenType::RP>(it);

  if (it + 1 == end)
    return eofError(it);
  ++it;
  if (it->type != ast::tokens::TokenType::LB)
    return wrongTokenError<ast::tokens::TokenType::LB>(it);
  if (it + 1 == end)
    return eofError(it);
  ++it;
  while (it->type != ast::tokens::TokenType::RB) {
    auto item = parseSwitchItem(it, end, table);
    if (item.failed())
      return item;
    if (it == end) {
      auto &value = item.getValue();
      return eofError(value->getLineEnd(), value->getColumnEnd());
    }
    result->items.pushBack(core::move(item.getValue()));
  }
  result->line_end = it->end_line;
  result->column_end = it->end_column;
  ++it;

  return result;
}

core::LogicalResult<core::UniquePtr<ast::ExpressionStmt>>
parseExpressionStmt(core::Vector<ast::tokens::Token>::Iterator &it,
                    core::Vector<ast::tokens::Token>::Iterator end,
                    core::UniquePtr<ast::symbolTable::Table> &table) {
  auto result = core::UniquePtr<ast::ExpressionStmt>::create();
  result->line_start = it->start_line;
  result->column_start = it->start_column;
  result->associatedLevel = table->current;
  auto value = parseExpression(it, end, table);
  if (value.failed())
    return value;
  result->value = core::move(value.getValue());
  if (it == end) {
    return eofError(result->value->getLineEnd(), result->value->getColumnEnd());
  }
  if (it->type != ast::tokens::TokenType::SEMI) {
    return wrongTokenError<ast::tokens::TokenType::SEMI>(it);
  }
  result->line_end = it->end_line;
  result->column_end = it->end_column;
  ++it;
  return result;
}

core::LogicalResult<core::UniquePtr<ast::ReturnStmt>>
parseReturnStmt(core::Vector<ast::tokens::Token>::Iterator &it,
                core::Vector<ast::tokens::Token>::Iterator end,
                core::UniquePtr<ast::symbolTable::Table> &table) {
  if (it->type != ast::tokens::TokenType::RETURN) {
    return wrongTokenError<ast::tokens::TokenType::RETURN>(it);
  }
  auto result = core::UniquePtr<ast::ReturnStmt>::create();
  result->line_start = it->start_line;
  result->column_start = it->start_column;
  result->associatedLevel = table->current;
  if (it + 1 == end) {
    return eofError(it);
  }
  ++it;
  if (it->type == ast::tokens::TokenType::SEMI) {
    result->line_end = it->end_line;
    result->column_end = it->end_column;
    result->value = nullptr;
    ++it;
    return result;
  }

  auto value = parseExpression(it, end, table);
  if (value.failed())
    return value;
  result->value = core::move(value.getValue());
  if (it == end) {
    return eofError(result->value->getLineEnd(), result->value->getColumnEnd());
  }
  if (it->type != ast::tokens::TokenType::SEMI) {
    return wrongTokenError<ast::tokens::TokenType::SEMI>(it);
  }
  result->line_end = it->end_line;
  result->column_end = it->end_column;
  ++it;
  return result;
}

core::LogicalResult<core::UniquePtr<ast::Statement>>
parseStatement(core::Vector<ast::tokens::Token>::Iterator &it,
               core::Vector<ast::tokens::Token>::Iterator end,
               core::UniquePtr<ast::symbolTable::Table> &table) {
  switch (it->type) {
  case ast::tokens::TokenType::VAR: {
    auto stmt = parseVariableDeclaration(it, end, table);
    if (stmt.failed())
      return stmt;
    auto result = core::UniquePtr<ast::Statement>::create();
    *result = core::move(*stmt.getValue());
    return result;
  }
  case ast::tokens::TokenType::LB: {
    auto stmt = parseCompoundStmt(it, end, table);
    if (stmt.failed())
      return stmt;
    auto result = core::UniquePtr<ast::Statement>::create();
    *result = core::move(*stmt.getValue());
    return result;
  }
  case ast::tokens::TokenType::WHILE: {
    auto stmt = parseWhileStmt(it, end, table);
    if (stmt.failed())
      return stmt;
    auto result = core::UniquePtr<ast::Statement>::create();
    *result = core::move(*stmt.getValue());
    return result;
  }
  case ast::tokens::TokenType::DO: {
    auto stmt = parseDoWhileStmt(it, end, table);
    if (stmt.failed())
      return stmt;
    auto result = core::UniquePtr<ast::Statement>::create();
    *result = core::move(*stmt.getValue());
    return result;
  }
  case ast::tokens::TokenType::IF: {
    auto stmt = parseIfStmt(it, end, table);
    if (stmt.failed())
      return stmt;
    auto result = core::UniquePtr<ast::Statement>::create();
    *result = core::move(*stmt.getValue());
    return result;
  }
  case ast::tokens::TokenType::SWITCH: {
    auto stmt = parseSwitchStmt(it, end, table);
    if (stmt.failed())
      return stmt;
    auto result = core::UniquePtr<ast::Statement>::create();
    *result = core::move(*stmt.getValue());
    return result;
  }
  case ast::tokens::TokenType::SEMI: {
    ast::NopStmt res;
    res.line_start = it->start_line;
    res.column_start = it->start_column;
    res.line_end = it->end_line;
    res.column_end = it->end_column;
    res.associatedLevel = table->current;
    auto result = core::UniquePtr<ast::Statement>::create();
    *result = core::move(res);
    ++it;
    return result;
  }
  case ast::tokens::TokenType::CONTINUE: {
    ast::ContinueStmt res;
    res.line_start = it->start_line;
    res.column_start = it->start_column;
    res.line_end = it->end_line;
    res.column_end = it->end_column;
    res.associatedLevel = table->current;
    auto result = core::UniquePtr<ast::Statement>::create();
    *result = core::move(res);
    ++it;
    return result;
  }
  case ast::tokens::TokenType::BREAK: {
    ast::BreakStmt res;
    res.line_start = it->start_line;
    res.column_start = it->start_column;
    res.line_end = it->end_line;
    res.column_end = it->end_column;
    res.associatedLevel = table->current;
    auto result = core::UniquePtr<ast::Statement>::create();
    *result = core::move(res);
    ++it;
    return result;
  }
  case ast::tokens::TokenType::RETURN: {
    auto stmt = parseReturnStmt(it, end, table);
    if (stmt.failed())
      return stmt;
    auto result = core::UniquePtr<ast::Statement>::create();
    *result = core::move(*stmt.getValue());
    return result;
  }
  default: {
    auto stmt = parseExpressionStmt(it, end, table);
    if (stmt.failed())
      return stmt;
    auto result = core::UniquePtr<ast::Statement>::create();
    *result = core::move(*stmt.getValue());
    return result;
  }
  }
}

core::LogicalResult<core::UniquePtr<ast::CompoundStmt>>
parseCompoundStmt(core::Vector<ast::tokens::Token>::Iterator &it,
                  core::Vector<ast::tokens::Token>::Iterator end,
                  core::UniquePtr<ast::symbolTable::Table> &table) {
  if (it->type != ast::tokens::TokenType::LB) {
    return wrongTokenError<ast::tokens::TokenType::LB>(it);
  }
  auto result = core::UniquePtr<ast::CompoundStmt>::create();
  result->line_start = it->start_line;
  result->column_start = it->start_column;
  result->associatedLevel = table->current;
  table->initialize();
  if (it + 1 == end) {
    return eofError(it);
  }
  ++it;
  while (it->type != ast::tokens::TokenType::RB) {
    auto stmt = parseStatement(it, end, table);
    if (stmt.failed())
      return stmt;
    if (it == end) {
      auto &item = stmt.getValue();
      return eofError(item->getLineStart(), item->getColumnStart());
    }
    result->items.pushBack(core::move(stmt.getValue()));
  }
  table->finalize();
  result->line_end = it->end_line;
  result->column_end = it->end_column;
  ++it;
  return result;
}

core::LogicalResult<core::UniquePtr<ast::Argument>>
parseArgument(core::Vector<ast::tokens::Token>::Iterator &it,
              core::Vector<ast::tokens::Token>::Iterator end,
              core::UniquePtr<ast::symbolTable::Table> &table) {
  auto result = core::UniquePtr<ast::Argument>::create();
  result->line_start = it->start_line;
  result->column_start = it->start_column;
  auto type = parseType(it, end, table);
  if (type.failed())
    return type;
  result->type = core::move(type.getValue());
  if (it == end) {
    return eofError(result->type->getLineStart(),
                    result->type->getColumnStart());
  }
  if (it->type != ast::tokens::TokenType::ID) {
    return wrongTokenError<ast::tokens::TokenType::ID>(it);
  }
  result->name = it->s;
  result->line_end = it->end_line;
  result->column_end = it->end_column;
  ++it;
  return result;
}

core::LogicalResult<core::UniquePtr<ast::FunctionDeclaration>>
parseFunctionDeclaration(core::Vector<ast::tokens::Token>::Iterator &it,
                         core::Vector<ast::tokens::Token>::Iterator end,
                         core::UniquePtr<ast::symbolTable::Table> &table) {
  if (it->type != ast::tokens::TokenType::FUN) {
    return wrongTokenError<ast::tokens::TokenType::FUN>(it);
  }
  if (it + 1 == end) {
    return eofError(it);
  }
  auto result = core::UniquePtr<ast::FunctionDeclaration>::create();
  result->line_start = it->start_line;
  result->column_start = it->start_column;
  result->associatedLevel = table->current;
  ++it;
  auto returnType = parseType(it, end, table);
  if (returnType.failed())
    return returnType;
  result->returnType = core::move(returnType.getValue());
  if (it == end) {
    return eofError(result->returnType->getLineEnd(),
                    result->returnType->getColumnEnd());
  }
  if (it->type != ast::tokens::TokenType::ID) {
    return wrongTokenError<ast::tokens::TokenType::ID>(it);
  }
  result->name = it->s;
  if (it + 1 == end) {
    return eofError(it);
  }
  ++it;
  table->initialize();

  if (it->type != ast::tokens::TokenType::LP) {
    return wrongTokenError<ast::tokens::TokenType::LP>(it);
  }
  if (it + 1 == end) {
    return eofError(it);
  }
  ++it;
  if (it->type == ast::tokens::TokenType::RP) {
    if (it + 1 == end) {
      return eofError(it);
    }
    ++it;
  } else {
    bool finished = false;
    while (!finished) {
      auto argument = parseArgument(it, end, table);
      if (argument.failed())
        return argument;
      if (it == end) {
        auto &arg = argument.getValue();
        return eofError(arg->getLineEnd(), arg->getColumnEnd());
      }
      result->args.pushBack(core::move(argument.getValue()));
      switch (it->type) {
      case ast::tokens::TokenType::COMMA: {
        if (it + 1 == end) {
          return eofError(it);
        }
        ++it;
        break;
      }

      case ast::tokens::TokenType::RP: {
        if (it + 1 == end) {
          return eofError(it);
        }
        ++it;
        finished = true;
        break;
      }

      default: {
        return wrongTokenError<ast::tokens::TokenType::RP,
                               ast::tokens::TokenType::COMMA>(it);
      }
      }
    }
  }
  auto body = parseCompoundStmt(it, end, table);
  if (body.failed())
    return body;
  result->body = core::move(body.getValue());
  result->line_end = result->body->line_end;
  result->column_end = result->body->column_end;
  table->finalize();
  return result;
}

core::LogicalResult<core::UniquePtr<ast::Declarator>>
parseDeclarator(core::Vector<ast::tokens::Token>::Iterator &it,
                core::Vector<ast::tokens::Token>::Iterator end,
                core::UniquePtr<ast::symbolTable::Table> &table,
                core::UniquePtr<ast::Type> &varType) {
  auto result = core::UniquePtr<ast::Declarator>::create();
  result->line_start = it->start_line;
  result->column_start = it->start_column;
  result->associatedLevel = table->current;
  if (it->type != ast::tokens::TokenType::ID) {
    return wrongTokenError<ast::tokens::TokenType::ID>(it);
  }
  result->name = it->s;
  table->insert(result->name, varType->clone());
  if ((it + 1 != end) && ((it + 1)->type == ast::tokens::TokenType::EQUAL)) {
    ++it;
    if (it + 1 == end) {
      return eofError(it);
    }
    ++it;
    auto value = parseExpression(it, end, table);
    if (value.failed())
      return value;
    result->value = core::move(value.getValue());
    result->line_end = result->value->getLineEnd();
    result->column_end = result->value->getColumnEnd();
  } else {
    result->line_end = it->end_line;
    result->column_end = it->end_column;
    result->value = nullptr;
    ++it;
  }
  return result;
}

core::LogicalResult<core::UniquePtr<ast::VariableDeclaration>>
parseVariableDeclaration(core::Vector<ast::tokens::Token>::Iterator &it,
                         core::Vector<ast::tokens::Token>::Iterator end,
                         core::UniquePtr<ast::symbolTable::Table> &table) {
  if (it->type != ast::tokens::TokenType::VAR) {
    return parsingError("Unexpected token type. Expected `var`, but got `" +
                            to_string(it->type) + "`.",
                        it);
  }
  auto result = core::UniquePtr<ast::VariableDeclaration>::create();
  result->line_start = it->start_line;
  result->column_start = it->start_column;
  result->associatedLevel = table->current;
  if (it + 1 == end) {
    return eofError(it);
  }
  ++it;
  auto type = parseType(it, end, table);
  if (type.failed())
    return type;
  result->varType = core::move(type.getValue());
  if (it == end) {
    return eofError(result->varType->getLineEnd(),
                    result->varType->getColumnEnd());
  }
  auto decl = parseDeclarator(it, end, table, result->varType);
  if (decl.failed())
    return decl;
  core::UniquePtr<ast::Declarator> d = core::move(decl.getValue());
  if (it == end) {
    return eofError(d->getLineEnd(), d->getColumnEnd());
  }
  result->decls.pushBack(core::move(d));
  bool finished = false;
  while (!finished) {
    switch (it->type) {
    case ast::tokens::TokenType::COMMA: {
      if (it + 1 == end) {
        return eofError(it);
      }
      ++it;
      auto decl = parseDeclarator(it, end, table, result->varType);
      if (decl.failed())
        return decl;
      core::UniquePtr<ast::Declarator> d = core::move(decl.getValue());
      if (it == end) {
        return eofError(d->line_end, d->column_end);
      }
      result->decls.pushBack(core::move(d));
      break;
    }

    case ast::tokens::TokenType::SEMI: {
      finished = true;
      break;
    }

    default: {
      return wrongTokenError<ast::tokens::TokenType::SEMI,
                             ast::tokens::TokenType::COMMA>(it);
    }
    }
  }
  result->line_end = it->end_line;
  result->column_end = it->end_column;
  ++it;
  return result;
}

core::LogicalResult<core::UniquePtr<ast::ProgramItem>>
parseProgramItem(core::Vector<ast::tokens::Token>::Iterator &it,
                 core::Vector<ast::tokens::Token>::Iterator end,
                 core::UniquePtr<ast::symbolTable::Table> &table) {
  switch (it->type) {
  case ast::tokens::TokenType::VAR: {
    auto item = parseVariableDeclaration(it, end, table);
    if (item.failed())
      return item;
    return core::UniquePtr<ast::ProgramItem>::create(
        core::move(*item.getValue()));
  }
  case ast::tokens::TokenType::STRUCT: {
    auto item = parseStructDeclaration(it, end, table);
    if (item.failed())
      return item;
    return core::UniquePtr<ast::ProgramItem>::create(
        core::move(*item.getValue()));
  }
  case ast::tokens::TokenType::UNION: {
    auto item = parseUnionDeclaration(it, end, table);
    if (item.failed())
      return item;
    return core::UniquePtr<ast::ProgramItem>::create(
        core::move(*item.getValue()));
  }
  case ast::tokens::TokenType::FUN: {
    auto item = parseFunctionDeclaration(it, end, table);
    if (item.failed())
      return item;
    return core::UniquePtr<ast::ProgramItem>::create(
        core::move(*item.getValue()));
  }
  case ast::tokens::TokenType::ALIAS: {
    return parseAliasDeclaration(it, end, table);
  }
  default: {
    return parsingError("Unexpected token in program Item.", it);
  }
  }
}

core::LogicalResult<core::UniquePtr<ast::Program>>
parseProgram(core::Vector<ast::tokens::Token>::Iterator &it,
             core::Vector<ast::tokens::Token>::Iterator end,
             core::UniquePtr<ast::symbolTable::Table> &table) {
  auto program = core::UniquePtr<ast::Program>::create();
  program->line_start = it->start_line;
  program->column_start = it->start_column;
  while (it != end) {
    auto item = parseProgramItem(it, end, table);
    if (item.failed()) {
      return core::LogicalResult<core::UniquePtr<ast::Program>>::failure(
          item.getMessage());
    }
    auto &value = item.getValue();
    if (value) {
      program->line_end = value->getLineEnd();
      program->column_end = value->getColumnEnd();
      program->items.pushBack(core::move(value));
    }
  }
  program->associatedLevel = table->current;
  return program;
}

export [[nodiscard]] core::LogicalResult<core::Pair<
    core::UniquePtr<ast::Program>, core::UniquePtr<ast::symbolTable::Table>>>
parse(core::String content) {
  auto lexResult = lex(content);
  if (lexResult.failed()) {
    return core::LogicalResult<core::Pair<
        core::UniquePtr<ast::Program>,
        core::UniquePtr<ast::symbolTable::Table>>>::failure(lexResult
                                                                .getMessage());
  }
  auto tokens = lexResult.getValue();
  auto it = tokens.begin();
  auto end = tokens.end();
  if (it == end)
    return core::LogicalResult<
        core::Pair<core::UniquePtr<ast::Program>,
                   core::UniquePtr<ast::symbolTable::Table>>>::
        failure("Parsing error:\nEmpty program.");
  auto table = core::UniquePtr<ast::symbolTable::Table>::create();
  auto program = parseProgram(it, end, table);
  if (program.failed()) {
    return core::LogicalResult<core::Pair<
        core::UniquePtr<ast::Program>,
        core::UniquePtr<ast::symbolTable::Table>>>::failure(program
                                                                .getMessage());
  }
  core::UniquePtr<ast::Program> &prog = program.getValue();
  prog->verify();
  return core::Pair(core::move(prog), core::move(table));
}

export [[nodiscard]] core::LogicalResult<core::Pair<
    core::UniquePtr<ast::Program>, core::UniquePtr<ast::symbolTable::Table>>>
parseFile(core::String filename) {
  return parse(core::String::fromFile(filename));
}

} // namespace compiler::frontend