project(sm3_unroll)

add_library(${PROJECT_NAME} src/sm3.c)
add_library(sm3::unroll ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)