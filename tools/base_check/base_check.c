#include <stdio.h>
#include <string.h>

int is_base64_char(char c, int index, int len)
{
	return (c >= '0' && c <= '9') ||
		   (c >= 'A' && c <= 'Z') ||
		   (c >= 'a' && c <= 'z') ||
		   (c == '=' && index >= len - 2);
}

int is_base32_char(char c, int index, int len)
{
	return (c >= '2' && c <= '7') ||
		   (c >= 'A' && c <= 'Z') ||
		   (c == '=' && index >= len - 6);
}

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		printf("Usage: %s [suspected base string]\n", argv[0]);
		printf("The output will list suspected base types (for example base64, base32 etc.)\n");
		return 1;
	}

	/* Base string length */
	int len = strlen(argv[1]);

	/* The length of the string should be more than 3 to be base64
	 * or more than 7 for base32, but that we can check later */
	if (len < 4)
		return 1;


	/** Check for base64 **/

	/* The length of the string should be divisble by 4 */
	if (len % 4 != 0)
		return 1;

	/* Check for base64 and base32 chars */
	int is_base64 = 0, is_base32 = 0;
	int padding_chars = 0;
	for (int i = 0; i < len; ++i)
	{
		/* Make sure that there can't be non-padding chars
		 * if there already has been padding chars in the strings */
		padding_chars += argv[1][i] == '=';
		is_base64 -= padding_chars * (argv[1][i] != '=');
		is_base32 -= padding_chars * (argv[1][i] != '=');

		is_base64 += is_base64_char(argv[1][i], i, len);
		is_base32 += is_base32_char(argv[1][i], i, len);
	}

	if (is_base64 == len)
		printf("base64\n");

	/* The string needs to be divisible by 8 to be base32 */
	if (is_base32 == len && len % 8 == 0)
		printf("base32\n");

	return 0;
}
