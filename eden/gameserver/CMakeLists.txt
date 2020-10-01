# Collect the source files
file(GLOB_RECURSE SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/*.cpp)

# Add the server build target
add_executable(gameserver ${SRC})

# Define the include directories
target_include_directories(gameserver
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:include>
        PRIVATE
            src)

# Link the target
target_link_libraries(gameserver
        PRIVATE
            common
            pthread
            Boost
            Cpprest
            ${CRYPTOPP_LIBRARY}
            ${GLOG_LIBRARY})