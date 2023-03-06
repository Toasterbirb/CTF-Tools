#include <stdio.h>
#include <string.h>

int is_hex_char(char c)
{
	return  (c >= '0' && c <= '9') ||
			(c >= 'A' && c <= 'F') ||
			(c >= 'a' && c <= 'f');
}

void print_not_hex(char* hex_value)
{
	printf("%s is NOT a hex string\n", hex_value);
}

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		printf("Usage: %s [suspected hex string]\n", argv[0]);
		return 1;
	}

	int len = strlen(argv[1]);

	/* Hex values are always divisible by two (assuming they come in pairs of two) */
	if (len % 2 != 0)
	{
		print_not_hex(argv[1]);
		return 1;
	}


	for (int i = 0; i < len; ++i)
	{
		if (!is_hex_char(argv[1][i]))
		{
			print_not_hex(argv[1]);
			return 1;
		}
	}

	printf("%s is a hex string\n", argv[1]);
	return 0;
}
