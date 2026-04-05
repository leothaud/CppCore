# CppCore

CppCore is a custom C++ library and runtime. It was initially part of the [Cidmol](https://github.com/leothaud/cidmol) project, but was then extended as a full C++ library.

Currently, CppCore only supports Linux x86_64 targets and aims to implement advanced C++ functionalities, such as multi-threading support. But it has a few limitations:

- Exceptions are not supported.
- The tests are limited for now as it was not supposed to be a complex C++ library but just some usefull functions (like _start) at first.
- The C++ symbol demangler supposes that the program was compiled with clang and is not complete. I made it complete enough to pass the test, but it lacks support for some parts of the mangling API.

