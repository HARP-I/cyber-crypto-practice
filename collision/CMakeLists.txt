
add_executable(birthday_attack
birthday_attack/src/sm3_birthday_attack.c)
target_link_libraries(birthday_attack
PRIVATE sm3::reduced
)

add_executable(rho_attack
rho_attack/src/sm3_rho_attack.c)
target_link_libraries(birthday_attack
PRIVATE sm3::reduced
)
