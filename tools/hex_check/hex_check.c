#include <stdio.h>
#include <string.h>

int is_hex_char(char c)
{
	return  (c >= '0' && c <= '9') ||
			(c >= 'A' && c <= 'F') ||
			(c >= 'a' && c <= 'f');
}

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		printf("Usage: %s [suspected hex string]\n", argv[0]);
		return 1;
	}

	int len = strlen(argv[1]);

	for (int i = 0; i < len; ++i)
	{
		if (!is_hex_char(argv[1][i]))
		{
			printf("%s is NOT a hex string\n", argv[1]);
			return 1;
		}
	}

	printf("%s is a hex string\n", argv[1]);
	return 0;
}
