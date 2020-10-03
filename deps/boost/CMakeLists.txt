set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_LIBS        ON)

find_package(Boost 1.71 REQUIRED COMPONENTS system filesystem program_options thread iostreams fiber)

# Add a target to use in the client
add_library(Boost INTERFACE)
target_include_directories(Boost INTERFACE ${Boost_INCLUDE_DIRS})
target_link_libraries(Boost INTERFACE ${Boost_LIBRARIES})