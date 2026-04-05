cmake_minimum_required(VERSION 4.3.0)

find_package(Doxygen)

if(NOT CppCoreIncluded)
  include(CppCore.cmake)
endif()

project(CppCore LANGUAGES CXX ASM)

include(CTest)
enable_testing()
set(TEST_TARGETS "")

include_directories(${PROJECT_SOURCE_DIR}/src)

if(DOXYGEN_FOUND)
  set(DOXYFILE_IN ${PROJECT_SOURCE_DIR}/Doxyfile.in)
  set(DOXYFILE_GEN ${CMAKE_BINARY_DIR}/CppCore/Doxyfile)

  set(DOXYGEN_PREDEFINED "__linux__ __x86_64__")

  if(ENABLE_THREADS)
    set(DOXYGEN_PREDEFINED "${DOXYGEN_PREDEFINED} CORE_THREAD")
  endif(ENABLE_THREADS)

  configure_file(${DOXYFILE_IN} ${DOXYFILE_GEN} @ONLY)

  add_custom_target(doc
    COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE_GEN}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Generating documentation with Doxygen"
    VERBATIM
  )
endif(DOXYGEN_FOUND)

add_subdirectory(src)
add_subdirectory(tests)

get_property(testTargets GLOBAL PROPERTY TEST_TARGETS)
add_test(NAME buildTests
  COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target ${testTargets}
)

set_tests_properties(buildTests PROPERTIES FIXTURES_SETUP build)

add_custom_target(check-cppCore COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure)
