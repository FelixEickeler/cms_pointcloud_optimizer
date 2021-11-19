# add tests

enable_testing()
include(GoogleTest)
set(TEST_TARGET ${BINARY}_test)

# setup test targets
file(GLOB_RECURSE TEST_SOURCES LIST_DIRECTORIES false *.h *.cpp)
MESSAGE(${TEST_SOURCES})
add_executable(${TEST_TARGET} ${TEST_SOURCES})
target_link_libraries(${TEST_TARGET} PRIVATE gtest gmock gtest_main spdlog::spdlog cilantro)
target_link_libraries(${TEST_TARGET} PUBLIC ${CMAKE_PROJECT_NAME}_lib )
target_include_directories(${TEST_TARGET} PUBLIC ${PUBLIC_INCLUDE_DIRECTORY})

# copy testfiles
file(GLOB TEST_FILES ${CMAKE_CURRENT_SOURCE_DIR}/files/*.txt ${CMAKE_CURRENT_SOURCE_DIR}/files/*.ply ${CMAKE_CURRENT_SOURCE_DIR}/files/*.json)
foreach(test_source ${TEST_FILES})
    get_filename_component(test_file_name "${test_source}" NAME)
    configure_file(${test_source} ${CMAKE_CURRENT_BINARY_DIR}/files/${test_file_name} COPYONLY)
endforeach()