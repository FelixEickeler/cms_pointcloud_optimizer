cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

if(NOT TARGET spdlog)
    find_package(spdlog CONFIG REQUIRED)
endif()

find_package(TBB REQUIRED)

file(GLOB_RECURSE sources LIST_DIRECTORIES true ${CMAKE_CURRENT_SOURCE_DIR} *.ipp *.h *.cpp)
file(GLOB public_headers LIST_DIRECTORIES true ${CMAKE_INCLUDE_CURRENT_DIR} *.h)

# (1) cpp executable
add_executable(${BINARY} ${sources})
target_sources(${BINARY} PRIVATE ${sources} ${public_headers})

target_link_libraries(${BINARY} PRIVATE spdlog::spdlog  nlohmann_json::nlohmann_json cilantro TBB::tbb)
target_include_directories(${BINARY} PUBLIC ${PUBLIC_INCLUDE_DIRECTORY}  ${tclap_includes})
set_target_properties(${BINARY} PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)


# (2) library
# remove app.cpp
list(FILTER sources EXCLUDE REGEX ".*app\\.cpp$")
add_library(${BINARY}_lib STATIC)
target_sources(${BINARY}_lib
        PRIVATE
        ${sources}
        PUBLIC
        ${public_headers})

target_link_libraries(${BINARY}_lib PRIVATE spdlog::spdlog nlohmann_json::nlohmann_json cilantro TBB::tbb)
target_include_directories(${BINARY}_lib PUBLIC ${PUBLIC_INCLUDE_DIRECTORY})


if (CMAKE_BUILD_TYPE EQUAL "Release")
    target_compile_options(${BINARY} PUBLIC -O3 -march=native -mtune=native)
    target_compile_options(${BINARY}_lib PUBLIC -O3 -march=native -mtune=native)
#else()
#    message("Debug Mode Activated")
#    target_compile_options(${BINARY} PUBLIC -O0 -march=native -mtune=native)
#    target_compile_options(${BINARY}_lib PUBLIC -O0 -march=native -mtune=native)
endif (CMAKE_BUILD_TYPE EQUAL "Release")
