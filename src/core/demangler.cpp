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
export module core:demangler;

import :string;
import :traits;
import :vector;
import :logicalResult;
import :zip;
import :map;
import :variant;
import :stringLiteral;

namespace core {

struct DemangleResult {
  Vector<String> names;
  String moduleName;
  u64 modulePosition = -1;
  Vector<String> types;
  bool hasType;
  bool isConst = false;
  bool isOperator = false;
};

void parseSize(u64 &size, const char *(&ptr)) {
  size = 0;
  while ((*ptr >= '0') && (*ptr <= '9')) {
    size = size * 10 + (*ptr - '0');
    ++ptr;
  }
}

LogicalResult<void> parseModule(DemangleResult &result, const char *(&ptr)) {
  u64 size;
  parseSize(size, ptr);
  if (size == 0) {
    return LogicalResult<void>::failure("Parsed a module name size of 0.");
  }
  String moduleName = "";
  for (u64 i = 0; i < size; ++i, ++ptr) {
    if (*ptr == 0) {
      return LogicalResult<void>::failure("Name ended during module parsing.");
    }
    moduleName += *ptr;
  }
  result.moduleName = move(moduleName);
  result.modulePosition = result.names.length();
  return LogicalResult<void>::success();
}

LogicalResult<void> demangleNestedName(DemangleResult &result,
                                       const char *(&ptr)) {
  if (*ptr == 'K') {
    result.isConst = true;
    ++ptr;
  }
  while (*ptr != 'E') {
    if (*ptr == 'W') {
      auto parsedModule = parseModule(result, ++ptr);
      if (parsedModule.failed())
        return parsedModule;
    } else {
      if ((*ptr == 'C') && ((*(ptr + 1) >= '1') && (*(ptr + 1) <= '3'))) {
        ptr += 2;
        result.names.pushBack(result.names[result.names.length() - 1]);
      } else {
        if ((*ptr == 'D') && ((*(ptr + 1) >= '1') && (*(ptr + 1) <= '3'))) {
          ptr += 2;
          result.names.pushBack("~" + result.names[result.names.length() - 1]);
        } else {
          u64 size;
          parseSize(size, ptr);
          if (size == 0) {
            return LogicalResult<void>::failure(
                "Parsed a namespace name size of 0.");
          }
          String current = "";
          for (u64 i = 0; i < size; ++i, ++ptr) {
            if (*ptr == 0) {
              return LogicalResult<void>::failure(
                  "Name ended during namespace parsing.");
            }
            current += *ptr;
          }
          result.names.pushBack(move(current));
        }
      }
    }
  }
  ++ptr;
  return LogicalResult<void>::success();
}

struct DemangledType {
  enum class type {
    BASE,
    PTR,
    RVAL,
    LVAL,
    FUN,
    CONST,
  };
  type t;
  DemangledType(type t) : t(t) {}

  String toString() const;
};

struct BaseType : public DemangledType {
  String type;

  BaseType(String type) : DemangledType(type::BASE), type(type) {}
  ~BaseType() = default;

  String toString() const { return type; }
};

struct ConstType : public DemangledType {
  DemangledType *innerType;
  ConstType(DemangledType *innerType)
      : DemangledType(type::CONST), innerType(innerType) {}
  ~ConstType() { delete innerType; }
  String toString() const;
};

struct PointerType : public DemangledType {
  DemangledType *innerType;
  int depth = 1;

  PointerType(DemangledType *innerType)
      : DemangledType(type::PTR), innerType(innerType) {}
  ~PointerType() { delete innerType; }
  String toString() const;
};

struct RvalueType : public DemangledType {
  DemangledType *innerType;

  RvalueType(DemangledType *innerType)
      : DemangledType(type::RVAL), innerType(innerType) {}
  ~RvalueType() { delete innerType; }
  String toString() const;
};

struct LvalueType : public DemangledType {
  DemangledType *innerType;

  LvalueType(DemangledType *innerType)
      : DemangledType(type::LVAL), innerType(innerType) {}
  ~LvalueType() { delete innerType; }
  String toString() const;
};

struct FunctionType : public DemangledType {
  DemangledType *returnType;
  Vector<DemangledType *> arguments;

  FunctionType(DemangledType *returnType)
      : DemangledType(type::FUN), returnType(returnType) {}
  void addArgument(DemangledType *argType) { arguments.pushBack(argType); }
  ~FunctionType() {
    delete returnType;
    for (auto &arg : arguments)
      delete arg;
  }
  String toString() const { return ""; }
};

String PointerType::toString() const {
  if (innerType->t == type::FUN) {
    auto *fun = static_cast<const FunctionType *>(innerType);
    String result = fun->returnType->toString();
    result += " (";
    for (int i = 0; i < depth; ++i)
      result += "*";
    result += ") (";
    bool first = true;
    for (auto *a : fun->arguments) {
      if (!first)
        result += ", ";
      first = false;
      result += a->toString();
    }
    result += ")";
    return result;
  } else {
    return innerType->toString() + "*";
  }
}

String LvalueType::toString() const {
  if (innerType->t == type::FUN) {
    auto *fun = static_cast<const FunctionType *>(innerType);
    String result = fun->returnType->toString();
    result += " (&) (";
    bool first = true;
    for (auto *a : fun->arguments) {
      if (!first)
        result += ", ";
      first = false;
      result += a->toString();
    }
    result += ")";
    return result;
  } else {
    return innerType->toString() + "&";
  }
}

String ConstType::toString() const { return innerType->toString() + " const"; }

String RvalueType::toString() const {
  if (innerType->t == type::FUN) {
    auto *fun = static_cast<const FunctionType *>(innerType);
    String result = fun->returnType->toString();
    result += " (&&) (";
    bool first = true;
    for (auto *a : fun->arguments) {
      if (!first)
        result += ", ";
      first = false;
      result += a->toString();
    }
    result += ")";
    return result;
  } else {
    return innerType->toString() + "&&";
  }
}

String DemangledType::toString() const {
  switch (t) {
  case type::BASE:
    return static_cast<const BaseType *>(this)->toString();
  case type::PTR:
    return static_cast<const PointerType *>(this)->toString();
  case type::RVAL:
    return static_cast<const RvalueType *>(this)->toString();
  case type::LVAL:
    return static_cast<const LvalueType *>(this)->toString();
  case type::FUN:
    return static_cast<const FunctionType *>(this)->toString();
  case type::CONST:
    return static_cast<const ConstType *>(this)->toString();
  }
}

LogicalResult<DemangledType *> parseType(const char *(&ptr)) {
  switch (*ptr) {
  case 'v': {
    ++ptr;
    return new BaseType("void");
  }
  case 'w': {
    ++ptr;
    return new BaseType("wchar_t");
  }
  case 'b': {
    ++ptr;
    return new BaseType("bool");
  }
  case 'c': {
    ++ptr;
    return new BaseType("char");
  }
  case 'a': {
    ++ptr;
    return new BaseType("signed char");
  }
  case 'h': {
    ++ptr;
    return new BaseType("unsigned char");
  }
  case 's': {
    ++ptr;
    return new BaseType("short");
  }
  case 't': {
    ++ptr;
    return new BaseType("unsigned short");
  }
  case 'i': {
    ++ptr;
    return new BaseType("int");
  }
  case 'j': {
    ++ptr;
    return new BaseType("unsigned int");
  }
  case 'l': {
    ++ptr;
    return new BaseType("long");
  }
  case 'm': {
    ++ptr;
    return new BaseType("unsigned long");
  }
  case 'x': {
    ++ptr;
    return new BaseType("long long");
  }
  case 'y': {
    ++ptr;
    return new BaseType("unsigned long long");
  }
  case 'n': {
    ++ptr;
    return new BaseType("__int128");
  }
  case 'o': {
    ++ptr;
    return new BaseType("unsigned __int128");
  }
  case 'f': {
    ++ptr;
    return new BaseType("float");
  }
  case 'd': {
    ++ptr;
    return new BaseType("double");
  }
  case 'e': {
    ++ptr;
    return new BaseType("long double");
  }
  case 'g': {
    ++ptr;
    return new BaseType("__float128");
  }
  case 'z': {
    ++ptr;
    return new BaseType("...");
  }
  case 'P': {
    auto innerType = parseType(++ptr);
    if (innerType.failed())
      return innerType;
    auto *t = *innerType;
    if (t->t == DemangledType::type::PTR) {
      auto *ptr = static_cast<PointerType *>(t);
      ++ptr->depth;
      return ptr;
    }
    return new PointerType(*innerType);
  }
  case 'R': {
    auto innerType = parseType(++ptr);
    if (innerType.failed())
      return innerType;
    return new LvalueType(*innerType);
  }
  case 'O': {
    auto innerType = parseType(++ptr);
    if (innerType.failed())
      return innerType;
    return new RvalueType(*innerType);
  }
  case 'K': {
    auto innerType = parseType(++ptr);
    if (innerType.failed())
      return innerType;
    return new ConstType(*innerType);
  }
  case 'F': {
    auto returnType = parseType(++ptr);
    if (returnType.failed())
      return returnType;
    auto *result = new FunctionType(*returnType);
    while (*ptr != 'E') {
      auto argType = parseType(ptr);
      if (argType.failed())
        return argType;
      result->addArgument(*argType);
    }
    ++ptr;
    return result;
  }
  default: {
    return LogicalResult<String>::failure(String("Type starting with '") +
                                          *ptr + "' is not yet supported.");
  }
  }
}

LogicalResult<DemangleResult> demangle(const char *ptr) {
  DemangleResult result;
  auto parseOperator = [&](String op) -> LogicalResult<DemangleResult> {
    ++ptr;
    result.isOperator = true;
    result.names.pushBack(op);
    result.hasType = true;
    while (*ptr) {
      auto type = parseType(ptr);
      if (type.failed())
        return type;
      result.types.pushBack((*type)->toString());
    }
    return result;
  };
  switch (*ptr) {
  case 'a': {
    const char *savePtr = ptr;
    ++ptr;
    switch (*ptr) {
    case 'a':
      return parseOperator("&&");
    case 'd':
      return parseOperator("&");
    case 'n':
      return parseOperator("&");
    case 'N':
      return parseOperator("&=");
    case 'S':
      return parseOperator("=");
    case 'w':
      return parseOperator("co_await");
    default: {
      ptr = savePtr;
      goto defaultDemangle;
    }
    }
    break;
  }
  case 'c': {
    const char *savePtr = ptr;
    ++ptr;
    switch (*ptr) {
    case 'l':
      return parseOperator("()");
    case 'm':
      return parseOperator(",");
    case 'o':
      return parseOperator("~");
    case 'v': {
      auto to = parseType(ptr);
      if (to.failed())
        return to;
      return parseOperator((*to)->toString());
    }
    default: {
      ptr = savePtr;
      goto defaultDemangle;
    }
    }
    break;
  }
  case 'd': {
    const char *savePtr = ptr;
    ++ptr;
    switch (*ptr) {
      return parseOperator("delete[]");
    case 'e':
      return parseOperator("*");
    case 'l':
      return parseOperator("delete");
    case 'v':
      return parseOperator("/");
    case 'V':
      return parseOperator("/=");
    default: {
      ptr = savePtr;
      goto defaultDemangle;
    }
    }
    break;
  }
  case 'e': {
    const char *savePtr = ptr;
    ++ptr;
    switch (*ptr) {
    case 'o':
      return parseOperator("^");
    case 'O':
      return parseOperator("^=");
    case 'q':
      return parseOperator("==");
    default: {
      ptr = savePtr;
      goto defaultDemangle;
    }
    }
    break;
  }
  case 'g': {
    const char *savePtr = ptr;
    ++ptr;
    switch (*ptr) {
    case 'e':
      return parseOperator(">=");
    case 't':
      return parseOperator(">");
    default: {
      ptr = savePtr;
      goto defaultDemangle;
    }
    }
    break;
  }
  case 'i': {
    const char *savePtr = ptr;
    ++ptr;
    switch (*ptr) {
    case 'x':
      return parseOperator("[]");
    default: {
      ptr = savePtr;
      goto defaultDemangle;
    }
    }
    break;
  }
  case 'l': {
    const char *savePtr = ptr;
    ++ptr;
    switch (*ptr) {
    case 'e':
      return parseOperator("<=");
    case 'i': {
      ++ptr;
      u64 size;
      parseSize(size, ptr);
      String op = "\"\"";
      for (u64 i = 0; i < size; ++i, ++ptr) {
        op += *ptr;
      }
      --ptr;
      return parseOperator(op);
    }
    case 's':
      return parseOperator("<<");
    case 'S':
      return parseOperator("<<=");
    case 't':
      return parseOperator("<");
    default: {
      ptr = savePtr;
      goto defaultDemangle;
    }
    }
    break;
  }
  case 'm': {
    const char *savePtr = ptr;
    ++ptr;
    switch (*ptr) {
    case 'i':
      return parseOperator("-");
    case 'I':
      return parseOperator("-=");
    case 'l':
      return parseOperator("*");
    case 'L':
      return parseOperator("*=");
    case 'm':
      return parseOperator("--");
    default: {
      ptr = savePtr;
      goto defaultDemangle;
    }
    }
    break;
  }
  case 'n': {
    const char *savePtr = ptr;
    ++ptr;
    switch (*ptr) {
    case 'a':
      return parseOperator("new[]");
    case 'e':
      return parseOperator("!=");
    case 'g':
      return parseOperator("-");
    case 't':
      return parseOperator("!");
    case 'w':
      return parseOperator("new");
    default: {
      ptr = savePtr;
      goto defaultDemangle;
    }
    }
    break;
  }
  case 'o': {
    const char *savePtr = ptr;
    ++ptr;
    switch (*ptr) {
    case 'o':
      return parseOperator("||");
    case 'r':
      return parseOperator("|");
    case 'R':
      return parseOperator("|=");
    default: {
      ptr = savePtr;
      goto defaultDemangle;
    }
    }
    break;
  }
  case 'p': {
    const char *savePtr = ptr;
    ++ptr;
    switch (*ptr) {
    case 'l':
      return parseOperator("+");
    case 'L':
      return parseOperator("+=");
    case 'm':
      return parseOperator("->*");
    case 'p':
      return parseOperator("++");
    case 's':
      return parseOperator("+");
    case 't':
      return parseOperator("->");
    default: {
      ptr = savePtr;
      goto defaultDemangle;
    }
    }
    break;
  }
  case 'q': {
    const char *savePtr = ptr;
    ++ptr;
    switch (*ptr) {
    case 'u':
      return parseOperator("?");
    default: {
      ptr = savePtr;
      goto defaultDemangle;
    }
    }
    break;
  }
  case 'r': {
    const char *savePtr = ptr;
    ++ptr;
    switch (*ptr) {
    case 'm':
      return parseOperator("%");
    case 'M':
      return parseOperator("%=");
    case 's':
      return parseOperator(">>");
    case 'S':
      return parseOperator(">>=");
    default: {
      ptr = savePtr;
      goto defaultDemangle;
    }
    }
    break;
  }
  case 's': {
    const char *savePtr = ptr;
    ++ptr;
    switch (*ptr) {
    case 's':
      return parseOperator("<=>");
    default: {
      ptr = savePtr;
      goto defaultDemangle;
    }
    }
    break;
  }
  case 'N': {
    auto demangleNestedResult = demangleNestedName(result, ++ptr);
    if (demangleNestedResult.failed())
      return demangleNestedResult;
    break;
  }
  default: {
  defaultDemangle:
    if (*ptr == 'K') {
      result.isConst = true;
      ++ptr;
    }
    if (*ptr == 'W') {
      auto moduleResult = parseModule(result, ++ptr);
      if (moduleResult.failed())
        return moduleResult;
    }
    u64 size;
    parseSize(size, ptr);
    if (size == 0) {
      return LogicalResult<void>::failure("Parsed a symbol name size of 0.");
    }
    String name = "";
    for (u64 i = 0; i < size; ++i, ++ptr) {
      if (*ptr == 0) {
        return LogicalResult<DemangleResult>::failure(
            "Name terminated during the parsing of a value.");
      }
      name += *ptr;
    }
    result.names.pushBack(move(name));
  }
  }
  if (*ptr == 0) {
    result.hasType = false;
    return result;
  }
  result.hasType = true;
  while (*ptr != 0) {
    auto type = parseType(ptr);
    if (type.failed())
      return type;
    result.types.pushBack((*type)->toString());
  }
  return result;
}

// https://itanium-cxx-abi.github.io/cxx-abi/abi.html#mangling
export LogicalResult<String> demangle(String name) {

  // Not mangled
  if (!name.startsWith("_Z"))
    return name;

  auto demangleResult = demangle(name.ptr() + 2);
  if (demangleResult.failed())
    return demangleResult;

  core::String result = (demangleResult->isOperator) ? "operator " : "";
  for (u64 i = 0; i < demangleResult->names.length(); ++i) {
    result += ((i != 0) ? "::" : "") + demangleResult->names[i] +
              ((demangleResult->modulePosition == i)
                   ? ("@" + demangleResult->moduleName)
                   : "");
  }

  // for (auto &nmspace : demangleResult->namespaces) {
  //   result += nmspace + "::";
  // }
  // result += demangleResult->name;
  // if (demangleResult->moduleName != "") {
  //   result += "@" + demangleResult->moduleName;
  // }
  if (demangleResult->hasType) {
    if ((demangleResult->types.length() == 1) &&
        (demangleResult->types[0] == "void")) {
      result += "()";
    } else {
      result += "(";
      bool first = true;
      for (auto &t : demangleResult->types) {
        if (!first)
          result += ",";
        first = false;
        result += t;
      }
      result += ")";
    }
  }
  if (demangleResult->isConst) {
    result += " const";
  }
  return result;
}

} // namespace core