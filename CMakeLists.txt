cmake_minimum_required(VERSION 3.10)
project(SpeedTestSuite C)

set(CMAKE_C_COMPILER clang)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Ofast")

add_library(speed STATIC speed.c)

function(add_speed_test TESTNAME IMPLSRC CFLAGS)
    set(target_name "speed-${TESTNAME}")
    add_executable(${target_name} ${CMAKE_CURRENT_SOURCE_DIR}/${IMPLSRC})
    target_link_libraries(${target_name} PRIVATE speed)
    if (CFLAGS)
        target_compile_options(${target_name} PRIVATE ${CFLAGS})
    endif()
endfunction()

function(add_speed_test_rvv LMUL VLEN)
    set(CFLAGS -march=rv64gcv -DLMUL=${LMUL})
    if (${VLEN})
        math(EXPR VLENB "${VLEN} / 8")
        set(CFLAGS ${CFLAGS} -DVLENB=${VLENB})
        set(TESTNAME "rvv-m${LMUL}-vl${VLEN}")
    else()
        set(TESTNAME "rvv-m${LMUL}")
    endif()
    add_speed_test(${TESTNAME} rvv.c "${CFLAGS}")
endfunction()

add_speed_test(c-rvv-off    c.c "")
add_speed_test(c-rvv-on     c.c "-march=rv64gcv")
add_speed_test_rvv(1 0)
add_speed_test_rvv(1 128)
add_speed_test_rvv(1 256)
add_speed_test_rvv(2 0)
add_speed_test_rvv(2 256)
add_speed_test_rvv(2 128)
add_speed_test_rvv(4 0)
add_speed_test_rvv(4 128)
add_speed_test_rvv(4 256)
add_speed_test_rvv(8 0)
add_speed_test_rvv(8 128)
add_speed_test_rvv(8 256)


add_custom_target(test
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/speed.sh
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Running speed tests using speed.sh"
)
