#include "is_base.h"

int is_base64_char(char c, int index, int len)
{
	return (c >= '0' && c <= '9') ||
		   (c >= 'A' && c <= 'Z') ||
		   (c >= 'a' && c <= 'z') ||
		   c == '+' || c == '/'   ||
		   (c == '=' && index >= len - 2);
}

int is_base32_char(char c, int index, int len)
{
	return (c >= '2' && c <= '7') ||
		   (c >= 'A' && c <= 'Z') ||
		   (c == '=' && index >= len - 6);
}

int is_base64_str(char* str, int len)
{
	/* The length of the string should be divisible by 4 */
	if (len % 4 != 0)
		return 0;

	int is_base64 = 0, padding_chars = 0;
	for (int i = 0; i < len; ++i)
	{
		/* Make sure that there are non-padding chars after padding chars */
		padding_chars += str[i] == '=';
		is_base64 -= padding_chars * (str[i] != '=');

		/* Check if the char fits the base64 spec */
		is_base64 += is_base64_char(str[i], i, len);
	}
	return is_base64 == len;
}

int is_base32_str(char* str, int len)
{
	/* The length of the string should be divisble by 8 */
	if (len % 8 != 0)
		return 0;

	int is_base32 = 0, padding_chars = 0;
	for (int i = 0; i < len; ++i)
	{
		/* Make sure that there are non-padding chars after padding chars */
		padding_chars += str[i] == '=';
		is_base32 -= padding_chars * (str[i] != '=');

		/* Check if the char fits the base32 spec */
		is_base32 += is_base32_char(str[i], i, len);
	}
	return is_base32 == len;
}
