cmake_minimum_required(VERSION 3.10)
project(SpeedTestSuite C)

set(CMAKE_C_COMPILER clang)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Ofast")

add_library(speed STATIC speed.c)

function(add_speed_test test_name arch)
    set(target_name "speed-${test_name}-${arch}")
    add_executable(${target_name} ${CMAKE_CURRENT_SOURCE_DIR}/${test_name}.c)
    target_link_libraries(${target_name} PRIVATE speed)
    target_compile_options(${target_name} PRIVATE -march=${arch})
endfunction()

add_speed_test(c              rv64gc)
add_speed_test(c              rv64gcv)
add_speed_test(rvv-m2         rv64gcv)
add_speed_test(rvv-m2-vl128   rv64gcv)

add_custom_target(test
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/speed.sh
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Running speed tests using speed.sh"
)
