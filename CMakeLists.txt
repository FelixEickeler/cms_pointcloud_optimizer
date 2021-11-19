cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
project(cms_pcs_optimizer)
set(CMAKE_CXX_STANDARD 17)
set(BINARY ${CMAKE_PROJECT_NAME})
set(PUBLIC_INCLUDE_DIRECTORY ${CMAKE_SOURCE_DIR}/includes)

# options
option(INCLUDE_TESTS "add this to run the tests" ON)
option(PYTHON "This will compile the python interface" OFF)
option(ENABLE_NATIVE_BUILD_OPTIMIZATIONS "Enable native build optimization flags" OFF)

message("\nBuilding CMS PointCloud Optimizer")
add_subdirectory(./modules)
add_subdirectory(./src)
if ($(PYTHON))
    add_subdirectory(./tests)
endif()

if (INCLUDE_TESTS)
    MESSAGE("Option INCLUDE_TESTS: ON")
    add_subdirectory(./tests)
else()
    MESSAGE("Option INCLUDE_TESTS: OFF")
endif()
#Copy default config from docs
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/docs/default_config.json ${CMAKE_CURRENT_BINARY_DIR}/bin/default_config.json COPYONLY)

#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
#set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")