#pragma once

int is_base64_char(char c, int index, int len);
int is_base32_char(char c, int index, int len);

int is_base64_str(char* str, int len);
int is_base32_str(char* str, int len);
