# Find Protobuf and gRPC
find_package(Protobuf REQUIRED)
find_package(GRPC REQUIRED)

# Add a target to use in the server
add_library(Protobuf INTERFACE)
target_include_directories(Protobuf INTERFACE ${Protobuf_INCLUDE_DIRS})
target_link_libraries(Protobuf INTERFACE ${Protobuf_LIBRARIES})

add_library(Grpc INTERFACE)
target_include_directories(Grpc INTERFACE ${GRPC_INCLUDE_DIR})
target_link_libraries(Grpc INTERFACE gRPC::grpc gRPC::grpc++)