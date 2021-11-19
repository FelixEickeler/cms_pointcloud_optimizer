# Load needed 3rd party modules
if (NOT modules_path)
    set(modules_path ${CMAKE_SOURCE_DIR}/modules)
endif()

# configure spdlog
add_subdirectory(${modules_path}/spdlog ./spdlog EXCLUDE_FROM_ALL)

# configure cilantro
IF(CMAKE_BUILD_TYPE MATCHES Debug)
    MESSAGE("Cilantro Debug")
    SET(ENABLE_NATIVE_BUILD_OPTIMIZATIONS OFF CACHE INTERNAL "Disable native optimization")
    SET(ENABLE_NON_DETERMINISTIC_PARALLELISM OFF CACHE INTERNAL "Disable parallelization")
ENDIF(CMAKE_BUILD_TYPE MATCHES Debug)
SET(BUILD_EXAMPLES OFF CACHE INTERNAL "No Example Target")
add_subdirectory(${modules_path}/cilantro ./cilantro EXCLUDE_FROM_ALL)

# configure json
set(JSON_BuildTests OFF CACHE INTERNAL "No tests for json")
add_subdirectory(${modules_path}/json ./json EXCLUDE_FROM_ALL)

# configure tclap
set(tclap_includes ${modules_path}/tclap/include CACHE INTERNAL "tclap_includes")

if ($(PYTHON))
    add_subdirectory(../modules/pybind11 ./pybind_bin EXCLUDE_FROM_ALL)
endif()

if (${INCLUDE_TESTS})
    add_subdirectory(${modules_path}/googletest modules/googletest EXCLUDE_FROM_ALL)
    mark_as_advanced(
            BUILD_GMOCK BUILD_GTEST BUILD_SHARED_LIBS
            gmock_build_tests gtest_build_samples gtest_build_tests
            gtest_disable_pthreads gtest_force_shared_crt gtest_hide_internal_symbols
    )
endif()
