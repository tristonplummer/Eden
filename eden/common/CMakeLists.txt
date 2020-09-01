# Collect the source files
file(GLOB_RECURSE SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/*.cpp)

# Add the library build target
add_library(common ${SRC})

# Define the include directories
target_include_directories(common
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:include>
        PRIVATE
            src)

# Define the linking language
set_target_properties(common PROPERTIES LINKER_LANGUAGE CXX)