find_package(LibYaml REQUIRED)

add_library(YAML INTERFACE)
target_include_directories(YAML INTERFACE ${YAMLCPP_INCLUDE_DIR})
target_link_libraries(YAML INTERFACE ${YAMLCPP_LIBRARY})
