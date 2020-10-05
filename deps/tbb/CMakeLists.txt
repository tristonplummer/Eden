find_package(TBB REQUIRED)

# Add a target to use in the client
add_library(TBB INTERFACE)
target_include_directories(TBB INTERFACE ${TBB_INCLUDE_DIRS})
target_link_libraries(TBB INTERFACE ${TBB_LIBRARIES})