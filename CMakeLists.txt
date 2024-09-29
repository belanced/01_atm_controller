cmake_minimum_required(VERSION 3.16)

project(
    atm_controller
    VERSION 1.0.0
    DESCRIPTION "Code submission for atm_controller"
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(GTest REQUIRED)
include(GoogleTest)
include_directories(${GTEST_INCLUDE_DIRS})

file(GLOB_RECURSE LIB_FILES "src/**/*.hpp" "src/**/*.cpp")
ADD_LIBRARY(libcode STATIC ${LIB_FILES})
target_include_directories(libcode PUBLIC src)

enable_testing()
file(GLOB_RECURSE TEST_FILES "test/*.cpp")
foreach(TEST_FILE ${TEST_FILES})
    get_filename_component(FILENAME ${TEST_FILE} NAME_WE)
    add_executable(${FILENAME}_test ${TEST_FILE})
    target_link_libraries(${FILENAME}_test PRIVATE libcode GTest::GTest GTest::Main pthread)
    gtest_discover_tests(${FILENAME}_test)
endforeach()

