cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
project(cms_pcs_optimizer)
set(CMAKE_CXX_STANDARD 17)
set(BINARY ${CMAKE_PROJECT_NAME})
set(CMAKE_INCLUDE_CURRENT_DIR ${CMAKE_SOURCE_DIR}/includes)

# options
option(INCLUDE_TESTS "add this to run the tests" ON)
option(PYTHON "This will compile the python interface" OFF)
option(ENABLE_NATIVE_BUILD_OPTIMIZATIONS "Enable native build optimization flags" ON)


message("\nBuilding CMS PointCloud Optimizer")
add_subdirectory(./modules)
add_subdirectory(./src)
add_subdirectory(./tests)

