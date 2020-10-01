find_package(cpprestsdk REQUIRED IMPORTED)

# Add a target to use in the server
add_library(Cpprest INTERFACE)
target_link_libraries(Cpprest INTERFACE cpprestsdk::cpprest)