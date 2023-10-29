#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "is_base.h"

int main(int argc, char** argv)
{
	if (argc != 3)
	{
		printf("Usage: %s [path to file containing suspect base strings] [base64/base32]\n", argv[0]);
		puts("The output will contain lines suspected to be either base64 or base32\ndepending on the chosen mode");
		return 1;
	}

	FILE* file = fopen(argv[1], "r");

	/* Testing string buffer */
	char str[512];

	/* Testing function pointer */
	int (*test_str_func)(char*, int);

	if (!strcmp(argv[2], "base64"))
	{
		test_str_func = &is_base64_str;
	}
	else if (!strcmp(argv[2], "base32"))
	{
		test_str_func = &is_base32_str;
	}
	else
	{
		printf("Invalid argument: %s\n", argv[2]);
		return 1;
	}

	while (fgets(str, sizeof(str), file))
	{
		int len = strlen(str);

		/* Swap the newline character at the end of the string to a null terminator */
		if (str[len - 1] == '\n')
		{
			str[len - 1] = '\0';
			len--;
		}

		/* Check if the string is a base64 or base32 string
		 * If it is, print it out */
		if (test_str_func(str, len))
			printf("%s\n", str);
	}

	/* Close the data file */
	fclose(file);

	return 0;
}
