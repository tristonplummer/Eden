# Collect the source files
file(GLOB_RECURSE SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/*.cpp)

# Add the server build target
add_executable(loginserver ${SRC})

# Define the include directories
target_include_directories(loginserver
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:include>
        PRIVATE
            src)

# Link the target
target_link_libraries(loginserver
        PRIVATE
            common
            pthread
            Boost
            gameapiprotocol
            Protobuf
            Grpc
            ${PQXX_LIB}
            ${PQ_LIB}
            ${CRYPTOPP_LIBRARY}
            ${GLOG_LIBRARY})