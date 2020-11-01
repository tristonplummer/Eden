# Collect the source files
file(GLOB_RECURSE SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)

# Add the server build target
add_executable(convert_svmaps ${SRC})

# Define the include directories
target_include_directories(convert_svmaps
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:include>
        PRIVATE
            src)

# Link the target
target_link_libraries(convert_svmaps
        PRIVATE
            common
            YAML
            Boost
            ${GLOG_LIBRARY})