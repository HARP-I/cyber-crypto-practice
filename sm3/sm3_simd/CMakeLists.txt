project(sm3_simd)

add_library(${PROJECT_NAME} src/sm3.c)
add_library(sm3::simd ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mssse3" CACHE STRING "Set C++ Compiler Flags" FORCE)

# target_compile_definitions(${PROJECT_NAME}
# PUBLIC -msse4.1
# )