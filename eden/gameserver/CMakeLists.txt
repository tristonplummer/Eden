# Set the source files
set(sources
        src/Main.cpp)

# Set the header files
set(headers
        include/shaiya/Test.hpp)

# Add the executable target
add_executable(gameserver ${sources})

# Link the target
target_link_libraries(gameserver
        PRIVATE
            common
            ${Boost_LIBRARIES}
            ${GLOG_LIBRARY})