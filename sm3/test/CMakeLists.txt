# project(sm3_test)

add_executable(sm3_test_base src/main.c)
target_link_libraries(sm3_test_base
    PRIVATE sm3::base
)

add_executable(sm3_test_unroll src/main.c)
target_link_libraries(sm3_test_unroll
    PRIVATE sm3::unroll
)

add_executable(sm3_test_simd src/main.c)
target_link_libraries(sm3_test_simd
    PRIVATE sm3::simd
)

add_executable(sm3_test_reduced src/test_reduced.c)
target_link_libraries(sm3_test_reduced
    PRIVATE sm3::reduced
)
