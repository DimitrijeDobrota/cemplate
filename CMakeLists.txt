cmake_minimum_required(VERSION 3.14)

include(cmake/prelude.cmake)

project(
    cemplate
    VERSION 0.1.6
    DESCRIPTION "Simple C++ template engine"
    HOMEPAGE_URL "https://git.dimitrijedobrota.com/cemplate.git"
    LANGUAGES CXX
)

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)

# ---- Declare library ----

add_library(
    cemplate_cemplate
    source/cemplate.cpp
)
add_library(cemplate::cemplate ALIAS cemplate_cemplate)

include(GenerateExportHeader)
generate_export_header(
    cemplate_cemplate
    BASE_NAME cemplate
    EXPORT_FILE_NAME export/cemplate/cemplate_export.hpp
    CUSTOM_CONTENT_FROM_VARIABLE pragma_suppress_c4251
)

if(NOT BUILD_SHARED_LIBS)
  target_compile_definitions(cemplate_cemplate PUBLIC CEMPLATE_STATIC_DEFINE)
endif()

set_target_properties(
    cemplate_cemplate PROPERTIES
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN YES
    VERSION "${PROJECT_VERSION}"
    SOVERSION "${PROJECT_VERSION_MAJOR}"
    EXPORT_NAME cemplate
    OUTPUT_NAME cemplate
)

target_include_directories(
    cemplate_cemplate ${warning_guard}
    PUBLIC
    "\$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
)

target_include_directories(
    cemplate_cemplate SYSTEM
    PUBLIC
    "\$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/export>"
)

target_compile_features(cemplate_cemplate PUBLIC cxx_std_20)

# ---- Install rules ----

if(NOT CMAKE_SKIP_INSTALL_RULES)
  include(cmake/install-rules.cmake)
endif()

# ---- Examples ----

if(PROJECT_IS_TOP_LEVEL)
  option(BUILD_EXAMPLES "Build examples tree." "${cemplate_DEVELOPER_MODE}")
  if(BUILD_EXAMPLES)
    add_subdirectory(example)
  endif()
endif()

# ---- Developer mode ----

if(NOT cemplate_DEVELOPER_MODE)
  return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
  message(
      AUTHOR_WARNING
      "Developer mode is intended for developers of cemplate"
  )
endif()

include(cmake/dev-mode.cmake)
