# Load needed 3rd party modules

add_subdirectory(../modules/spdlog ./spdlog EXCLUDE_FROM_ALL)
add_subdirectory(../modules/cilantro ./cilantro EXCLUDE_FROM_ALL)

if ($(PYTHON))
    add_subdirectory(../modules/pybind11 ./pybind_bin EXCLUDE_FROM_ALL)
endif()

if ($(INCLUDE_TESTS))
    add_subdirectory(../modules/googletest ./spdlog EXCLUDE_FROM_ALL)
endif()