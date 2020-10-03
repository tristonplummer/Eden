# Find Protobuf and gRPC
find_package(Protobuf REQUIRED)
find_package(GRPC REQUIRED)

# Collect the protocol files
file(GLOB_RECURSE proto_files RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.proto)

# Ensure the proto directory exists
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/proto)

# Generate the message classes
set(PROTOBUF_GENERATE_CPP_APPEND_PATH OFF)
include_directories(${PROTOBUF_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR})
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${proto_files})
grpc_generate_cpp(GRPC_SRCS GRPC_HDRS ${CMAKE_CURRENT_BINARY_DIR}/proto ${proto_files})

# Add a library for the api
add_library(gameapiprotocol ${PROTO_SRCS} ${PROTO_HDRS} ${GRPC_SRCS} ${GRPC_HDRS})

# Expose the include directory
target_include_directories(gameapiprotocol
        PUBLIC
            ${CMAKE_CURRENT_BINARY_DIR})