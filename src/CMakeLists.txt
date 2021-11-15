cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

if(NOT TARGET spdlog)
    find_package(spdlog CONFIG REQUIRED)
endif()

# (1) library
file(GLOB sources LIST_DIRECTORIES true ${CMAKE_CURRENT_SOURCE_DIR} *.h *.cpp)
file(GLOB public_headers LIST_DIRECTORIES true ${CMAKE_INCLUDE_CURRENT_DIR} *.h)

add_library(${BINARY}_lib STATIC)
target_sources(${BINARY}_lib
        PRIVATE
            ${sources}
        PUBLIC
            ${public_headers})

target_link_libraries(${BINARY}_lib PRIVATE spdlog::spdlog cilantro)
target_include_directories(${BINARY}_lib PUBLIC ${CMAKE_INCLUDE_CURRENT_DIR})
if (ENABLE_NATIVE_BUILD_OPTIMIZATIONS)
    target_compile_options(${BINARY}_lib PUBLIC -O3 -march=native -mtune=native)
else()
    target_compile_options(${BINARY}_lib PUBLIC -O3)
endif()


# (2) cpp executable
add_executable(${BINARY} ${sources})
target_sources(${BINARY} PRIVATE ${sources} ${public_headers})

target_link_libraries(${BINARY} PRIVATE spdlog::spdlog cilantro ${Pangolin_LIBRARIES})
target_include_directories(${BINARY} PUBLIC ${CMAKE_INCLUDE_CURRENT_DIR})
if (ENABLE_NATIVE_BUILD_OPTIMIZATIONS)
    target_compile_options(${BINARY} PUBLIC -O3 -march=native -mtune=native)
else()
    target_compile_options(${BINARY} PUBLIC -O3)
endif()

MESSAGE("Where is this ?")