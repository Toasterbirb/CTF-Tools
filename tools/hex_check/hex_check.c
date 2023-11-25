#include <stdio.h>
#include <unistd.h>
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
	if (argc > 2)
	{
		printf("Usage: hex_check [suspected hex string]\n");
		printf("\nYou can also pipe text to stdin\n");
		return 1;
	}

	/* Reading from stdin */
	if (argc == 1)
	{
		char c;
		int char_count = 0;
		while (read(STDIN_FILENO, &c, 1) > 0)
		{
			/* Don't count newlines or the end of the string */
			if (c == '\0' || c == '\n')
				continue;

			char_count++;
			if (!is_hex_char(c))
			{
				printf("The provided string was not a hex string. Problematic char: %c\n", c);
				return 1;
			}
		}

		/* Hex values are always divisible by two (assuming they come in pairs of two) */
		if ((char_count) % 2 != 0)
		{
			printf("The provided string was not a hex string\n");
			return 1;
		}

		printf("The provided string is a hex string\n");
		return 0;
	}

	/* Reading from an argument */
	if (argc == 2)
	{
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
	}

	return 0;
}
