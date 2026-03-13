cmake_minimum_required(VERSION 4.2.3)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

macro(set_alternate_compiler compiler)
  find_program(COMPILER_EXECUTABLE ${compiler})

  if(COMPILER_EXECUTABLE)
    message(STATUS "Found alternate compiler ${compiler} at ${COMPILER_EXECUTABLE}")
    set(CMAKE_CXX_COMPILER ${COMPILER_EXECUTABLE})
    set(CMAKE_ASM_COMMPILER ${COMPILER_EXECUTABLE})
  else()
    message(STATUS "Failed to find alternate compiler ${compiler}")
    set(USE_ALTERNATE_COMPILER "" CACHE STRING "Use alternate compiler. Leave empty for system default" FORCE)
  endif()
endmacro()

set(USE_ALTERNATE_COMPILER "clang++" CACHE STRING "Use alternate compiler. Leave empty for system default" FORCE)

if(NOT "${USE_ALTERNATE_COMPILER}" STREQUAL "")
  set_alternate_compiler("${USE_ALTERNATE_COMPILER}")
endif()

macro(set_alternate_linker linker)
  find_program(LINKER_EXECUTABLE ld.${linker} ${linker})

  if(LINKER_EXECUTABLE)
    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" AND "${CMAKE_CXX_COMPILER_VERSION}" VERSION_LESS 12.0.0)
      add_link_options("-ld-path=${USE_ALTERNATE_LINKER}")
    else()
      add_link_options("-fuse-ld=${USE_ALTERNATE_LINKER}")
    endif()
  else()
    message(STATUS "Failed to find alternate linker ${linker}")
    set(USE_ALTERNATE_LINKER "" CACHE STRING "Use alternate linker" FORCE)
  endif()
endmacro()

set(USE_ALTERNATE_LINKER "mold" CACHE STRING "Use alternate linker. Leave empty for system default; alternatives are 'gold', 'lld', 'bfd' or 'mold'" FORCE)

if(NOT "${USE_ALTERNATE_LINKER}" STREQUAL "")
  set_alternate_linker("${USE_ALTERNATE_LINKER}")
endif()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(Cidmol LANGUAGES CXX ASM)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -march=native \
  -mtune=native -fno-rtti -fno-exceptions -nostdlib -ffreestanding -nostdinc \
  -fno-lax-vector-conversions -fno-math-errno -fno-trapping-math")

function(addSource target)
  set(sources "")

  foreach(arg ${ARGN})
    list(APPEND sources ${CMAKE_CURRENT_SOURCE_DIR}/${arg})
  endforeach()

  set(varName "${target}Srcs")
  get_property(srcs GLOBAL PROPERTY ${varName})
  list(APPEND srcs ${sources})
  set_property(GLOBAL PROPERTY ${varName} "${srcs}")
endfunction(addSource)

function(addModule target)
  set(sources "")

  foreach(arg ${ARGN})
    list(APPEND sources ${CMAKE_CURRENT_SOURCE_DIR}/${arg})
  endforeach()

  set(varName "${target}Mods")
  get_property(srcs GLOBAL PROPERTY ${varName})
  list(APPEND srcs ${sources})
  set_property(GLOBAL PROPERTY ${varName} "${srcs}")
endfunction(addModule)

function(addExecutable name main)
  set(mods "")
  set(srcs ${CMAKE_CURRENT_SOURCE_DIR}/${main})

  foreach(arg ${ARGN})
    set(argModsName "${arg}Mods")
    get_property(argMods GLOBAL PROPERTY ${argModsName})
    set(argSrcsName "${arg}Srcs")
    get_property(argSrcs GLOBAL PROPERTY ${argSrcsName})
    list(APPEND mods ${argMods})
    list(APPEND srcs ${argSrcs})
  endforeach()

  set(libName "${name}Lib")
  add_library(${libName})
  target_sources(${libName} PUBLIC FILE_SET CXX_MODULES BASE_DIRS ${PROJECT_SOURCE_DIR}/src FILES ${mods})
  add_executable(${name} ${srcs})
  target_link_libraries(${name} PRIVATE ${libName})
  target_link_options(${name} PRIVATE -nostdlib -static)
endfunction(addExecutable)

add_subdirectory(src)