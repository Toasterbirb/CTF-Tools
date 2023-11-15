add_executable(hex_check ./hex_check/hex_check.c)
add_executable(caesar_cipher ./Caesar-cipher/caesar_cipher.c)
add_executable(char_manipulation ./char_manipulation/char_manipulation.c)

add_library(is_base ./base_check/is_base.c)
add_executable(base_check ./base_check/base_check.c)
add_executable(base_filter ./base_check/base_filter.c)
target_link_libraries(base_check is_base)
target_link_libraries(base_filter is_base)

add_executable(vigenere_decode ./vigenere_decode/vigenere_decode.c)
