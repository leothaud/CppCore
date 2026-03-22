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
export module ast:tokens;

import core;

namespace ast::tokens {

export enum class TokenType {
  DOUBLE_MINUS,
  MINUS_EQUAL,
  ARROW,
  MINUS,
  COMMA,
  SEMI,
  COLON,
  EXCLAMATION_EQUAL,
  EXCLAMATION,
  QUESTION,
  DOT,
  LP,
  RP,
  LSB,
  RSB,
  LB,
  RB,
  STAR_EQUAL,
  STAR,
  SLASH_EQUAL,
  SLASH,
  DOUBLE_AMPERSAND_EQUAL,
  DOUBLE_AMPERSAND,
  AMPERSAND_EQUAL,
  AMPERSAND,
  PERCENT_EQUAL,
  PERCENT,
  DOUBLE_WEDGE_EQUAL,
  DOUBLE_WEDGE,
  WEDGE_EQUAL,
  WEDGE,
  DOUBLE_PLUS,
  PLUS_EQUAL,
  PLUS,
  DOUBLE_LAB_EQUAL,
  DOUBLE_LAB,
  LAB_EQUAL,
  LAB,
  DOUBLE_EQUAL,
  EQUAL,
  DOUBLE_RAB_EQUAL,
  DOUBLE_RAB,
  RAB_EQUAL,
  RAB,
  DOUBLE_PIPE_EQUAL,
  DOUBLE_PIPE,
  PIPE_EQUAL,
  PIPE,
  TILDE,
  ALIAS,
  BIT,
  BITCAST,
  BREAK,
  CASE,
  CAST,
  CONTINUE,
  DEFAULT,
  DO,
  ELSE,
  FALSE,
  FOR,
  FUN,
  IF,
  OPERATOR,
  OTHER,
  RETURN,
  SIZEOF,
  STRUCT,
  SWITCH,
  SYSCALL,
  THIS,
  TRUE,
  UNION,
  VAR,
  VOID,
  WHILE,

  INT,
  CHAR,
  STRING,
  ID,

  UNKNOWN
};

namespace {
bool noArgToken(TokenType type) {
  switch (type) {
  case TokenType::STRING:
  case TokenType::ID:
  case TokenType::INT:
  case TokenType::CHAR:
    return false;
  default:
    return true;
  }
}

bool stringArgToken(TokenType type) {
  return (type == TokenType::STRING) || (type == TokenType::ID);
}

bool intArgToken(TokenType type) { return type == TokenType::INT; }

bool charArgToken(TokenType type) { return type == TokenType::CHAR; }

} // namespace

export struct Token {
  TokenType type;
  union {
    u64 i;
    char c;
    core::String s;
  };
  u64 start_line, start_column, end_line, end_column;

  ~Token() {
    switch (type) {
    case TokenType::STRING:
    case TokenType::ID:
      s.~String();
    default:;
    }
  }

  Token()
      : type(TokenType::UNKNOWN), i(0), start_line(0), start_column(0),
        end_line(0), end_column(0) {}

  Token(const Token &other)
      : type(other.type), start_line(other.start_line),
        start_column(other.start_column), end_line(other.end_line),
        end_column(other.end_column) {
    switch (type) {
    case TokenType::STRING:
    case TokenType::ID:
      s = other.s;
      break;
    case TokenType::INT:
      i = other.i;
      break;
    case TokenType::CHAR:
      c = other.c;
      break;
    default:
      i = 0;
    }
  }

  Token(Token &&other)
      : type(core::move(other.type)), start_line(other.start_line),
        start_column(other.start_column), end_line(other.end_line),
        end_column(other.end_column) {
    switch (type) {
    case TokenType::STRING:
    case TokenType::ID:
      s = core::move(other.s);
      break;
    case TokenType::INT:
      i = core::move(other.i);
      break;
    case TokenType::CHAR:
      c = core::move(other.c);
      break;
    default:
      i = 0;
    }
  }

  Token &operator=(const Token &other) {
    type = other.type;
    start_line = other.start_line;
    start_column = other.start_column;
    end_line = other.end_line;
    end_column = other.end_column;
    switch (type) {
    case TokenType::STRING:
    case TokenType::ID:
      s.~String();
      s = other.s;
      break;
    case TokenType::INT:
      i = other.i;
      break;
    case TokenType::CHAR:
      c = other.c;
      break;
    default:
      i = 0;
    }
    return *this;
  }

  Token &operator=(Token &&other) {
    type = core::move(other.type);
    start_line = other.start_line;
    start_column = other.start_column;
    end_line = other.end_line;
    end_column = other.end_column;
    switch (type) {
    case TokenType::STRING:
    case TokenType::ID:
      s.~String();
      s = core::move(other.s);
      break;
    case TokenType::INT:
      i = core::move(other.i);
      break;
    case TokenType::CHAR:
      c = core::move(other.c);
      break;
    default:
      i = 0;
    }
    return *this;
  }

  Token(TokenType type, u64 start_line, u64 start_column, u64 end_line,
        u64 end_column)
      : type(type), i(0), start_line(start_line), start_column(start_column),
        end_line(end_line), end_column(end_column) {
    core::assert(noArgToken(type));
  }

  Token(TokenType type, u64 i, u64 start_line, u64 start_column, u64 end_line,
        u64 end_column)
      : type(type), i(i), start_line(start_line), start_column(start_column),
        end_line(end_line), end_column(end_column) {
    core::assert(intArgToken(type));
  }

  Token(TokenType type, char c, u64 start_line, u64 start_column, u64 end_line,
        u64 end_column)
      : type(type), c(c), start_line(start_line), start_column(start_column),
        end_line(end_line), end_column(end_column) {
    core::assert(charArgToken(type));
  }

  Token(TokenType type, core::String s, u64 start_line, u64 start_column,
        u64 end_line, u64 end_column)
      : type(type), s(s), start_line(start_line), start_column(start_column),
        end_line(end_line), end_column(end_column) {
    core::assert(stringArgToken(type));
  }
};

export core::String to_string(TokenType type) {
  switch (type) {
  case TokenType::DOUBLE_MINUS:
    return "doubleMinus";
  case TokenType::MINUS_EQUAL:
    return "minusEqual";
  case TokenType::ARROW:
    return "arrow";
  case TokenType::MINUS:
    return "minus";
  case TokenType::COMMA:
    return "comma";
  case TokenType::SEMI:
    return "semi";
  case TokenType::COLON:
    return "colon";
  case TokenType::EXCLAMATION_EQUAL:
    return "exclamationEqual";
  case TokenType::EXCLAMATION:
    return "exclamation";
  case TokenType::QUESTION:
    return "question";
  case TokenType::DOT:
    return "dot";
  case TokenType::LP:
    return "lp";
  case TokenType::RP:
    return "rp";
  case TokenType::LSB:
    return "lsb";
  case TokenType::RSB:
    return "rsb";
  case TokenType::LB:
    return "lb";
  case TokenType::RB:
    return "rb";
  case TokenType::STAR_EQUAL:
    return "starEqual";
  case TokenType::STAR:
    return "star";
  case TokenType::SLASH_EQUAL:
    return "slashEqual";
  case TokenType::SLASH:
    return "slash";
  case TokenType::DOUBLE_AMPERSAND_EQUAL:
    return "doubleAmpersandEqual";
  case TokenType::DOUBLE_AMPERSAND:
    return "doubleAmpersand";
  case TokenType::AMPERSAND_EQUAL:
    return "ampersandEqual";
  case TokenType::AMPERSAND:
    return "ampersand";
  case TokenType::PERCENT_EQUAL:
    return "percentEqual";
  case TokenType::PERCENT:
    return "percent";
  case TokenType::DOUBLE_WEDGE_EQUAL:
    return "doubleWedgeEqual";
  case TokenType::DOUBLE_WEDGE:
    return "doubleWedge";
  case TokenType::WEDGE_EQUAL:
    return "wedgeEqual";
  case TokenType::WEDGE:
    return "wedge";
  case TokenType::DOUBLE_PLUS:
    return "doublePlus";
  case TokenType::PLUS_EQUAL:
    return "plusEqual";
  case TokenType::PLUS:
    return "plus";
  case TokenType::DOUBLE_LAB_EQUAL:
    return "doubleLabEqual";
  case TokenType::DOUBLE_LAB:
    return "doubleLab";
  case TokenType::LAB_EQUAL:
    return "labEqual";
  case TokenType::LAB:
    return "lab";
  case TokenType::DOUBLE_EQUAL:
    return "doubleEqual";
  case TokenType::EQUAL:
    return "equal";
  case TokenType::DOUBLE_RAB_EQUAL:
    return "doubleRabEqual";
  case TokenType::DOUBLE_RAB:
    return "doubleRab";
  case TokenType::RAB_EQUAL:
    return "rabEqual";
  case TokenType::RAB:
    return "rab";
  case TokenType::DOUBLE_PIPE_EQUAL:
    return "doublePipeEqual";
  case TokenType::DOUBLE_PIPE:
    return "doublePipe";
  case TokenType::PIPE_EQUAL:
    return "pipeEqual";
  case TokenType::PIPE:
    return "pipe";
  case TokenType::TILDE:
    return "tidle";
  case TokenType::ALIAS:
    return "alias";
  case TokenType::BIT:
    return "bit";
  case TokenType::BITCAST:
    return "bitcast";
  case TokenType::BREAK:
    return "break";
  case TokenType::CASE:
    return "case";
  case TokenType::CAST:
    return "cast";
  case TokenType::CONTINUE:
    return "continue";
  case TokenType::DEFAULT:
    return "default";
  case TokenType::DO:
    return "do";
  case TokenType::ELSE:
    return "else";
  case TokenType::FALSE:
    return "false";
  case TokenType::FOR:
    return "for";
  case TokenType::FUN:
    return "fun";
  case TokenType::IF:
    return "if";
  case TokenType::OPERATOR:
    return "operator";
  case TokenType::OTHER:
    return "other";
  case TokenType::RETURN:
    return "return";
  case TokenType::SIZEOF:
    return "sizeof";
  case TokenType::STRUCT:
    return "struct";
  case TokenType::SWITCH:
    return "switch";
  case TokenType::SYSCALL:
    return "syscall";
  case TokenType::THIS:
    return "this";
  case TokenType::TRUE:
    return "true";
  case TokenType::UNION:
    return "union";
  case TokenType::VAR:
    return "var";
  case TokenType::VOID:
    return "void";
  case TokenType::WHILE:
    return "while";
  case TokenType::INT:
    return "int";
  case TokenType::CHAR:
    return "char";
  case TokenType::STRING:
    return "string";
  case TokenType::ID:
    return "id";
  case TokenType::UNKNOWN:
    return "unknown";
    break;
  }
}

export const core::String to_string(Token tkn) {
  core::String result = to_string(tkn.type) + "(";
  switch (tkn.type) {
  case TokenType::INT: {
    result += core::String::of(tkn.i) + ", ";
    break;
  }
  case TokenType::CHAR: {
    result += "'" + core::String::of(tkn.c) + "', ";
    break;
  }
  case TokenType::STRING: {
    result += "\"" + tkn.s + "\", ";
    break;
  }
  case TokenType::ID: {
    result += tkn.s + ", ";
    break;
  }
  default: {
    break;
  }
  }
  return result + core::String::of(tkn.start_line) + ", " +
         core::String::of(tkn.start_column) + ", " +
         core::String::of(tkn.end_line) + ", " +
         core::String::of(tkn.end_column) + ")";
}

} // namespace ast::tokens
