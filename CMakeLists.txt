
CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

PROJECT(
  PlorthCLI
  VERSION 1.0.0
  DESCRIPTION "CLI for Plorth programming language."
  HOMEPAGE_URL "https://github.com/plorth/interpreter"
  LANGUAGES C CXX
)

OPTION(
  PLORTH_CLI_ENABLE_REPL
  "Enable if you want to use REPL."
  ON
)

INCLUDE(CheckIncludeFile)
INCLUDE(CheckFunctionExists)

CHECK_INCLUDE_FILE(sysexits.h HAVE_SYSEXITS_H)

CHECK_FUNCTION_EXISTS(fork HAVE_FORK)
CHECK_FUNCTION_EXISTS(isatty HAVE_ISATTY)

CONFIGURE_FILE(
  ${CMAKE_CURRENT_SOURCE_DIR}/include/plorth/cli/config.hpp.in
  ${CMAKE_CURRENT_SOURCE_DIR}/include/plorth/cli/config.hpp
)

ADD_EXECUTABLE(
  plorth-cli
  src/api.cpp
  src/main.cpp
  src/repl.cpp
  src/terminal.cpp
  src/utils.cpp
)

TARGET_COMPILE_OPTIONS(
  plorth-cli
  PRIVATE
    -Wall -Werror
)

TARGET_COMPILE_FEATURES(
  plorth-cli
  PRIVATE
    cxx_std_17
)

TARGET_INCLUDE_DIRECTORIES(
  plorth-cli
  PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/cget/include>
)

TARGET_LINK_LIBRARIES(
  plorth-cli
  plorth
)

SET_TARGET_PROPERTIES(
  plorth-cli
  PROPERTIES OUTPUT_NAME plorth
)

INSTALL(
  TARGETS
    plorth-cli
  RUNTIME DESTINATION
    bin
)
