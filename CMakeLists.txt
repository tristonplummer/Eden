# The minimum required version of CMake
cmake_minimum_required(VERSION 3.15)

# The project details
project("Eden"
        VERSION     0.1.0
        LANGUAGES   CXX)

# Disallow in-source builds
if (PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
    message(FATAL_ERROR "In-source builds are not allowed. Please make a new directory (called a build directory) and run CMake from there.")
endif()

# Set macro directory
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/macros")

# Make use of CCache
include(cmake/Ccache.cmake)

# Enable compiler warnings
include(cmake/CompilerWarnings.cmake)

# Enable Doxygen
include(cmake/Doxygen.cmake)

# Set the language standard
set(CMAKE_CXX_STANDARD 20)

# Include the dependencies
add_subdirectory(deps)

# Add the project subdirectory
add_subdirectory(eden)