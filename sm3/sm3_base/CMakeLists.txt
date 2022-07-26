project(sm3_base)

add_library(${PROJECT_NAME} src/sm3.c)
add_library(sm3::base ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)