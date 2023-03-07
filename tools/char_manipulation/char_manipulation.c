#include <stdio.h>
#include <string.h>

/* Skip all totally nonsense results */
int validate_result(char c)
{
	return (c >= 0) && (c < 127);
}

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		printf("Usage: %s [string]\n", argv[0]);
		return 1;
	}

	int len = strlen(argv[1]);

	/* Print out some information */
	printf("String length: %d\n", len);

	printf("String chars: [");
	for (int i = 0; i < len; ++i)
	{
		if (i != len - 1)
			printf("%d, ", argv[1][i]);
		else
			printf("%d", argv[1][i]);
	}
	printf("]\n\n");

	/* Multiply char values */
	printf("Note! Lines ending with '¦' would have had char values outside of the normal range\n");
	char c;

	printf("> Offset char values\n");
	for (int i = -50; i < 50; ++i)
	{
		/* Skip zero */
		if (i == 0)
			continue;

		printf("%d: ", i);
		for (int j = 0; j < len; ++j)
		{
			c = argv[1][j] + i;
			if (!validate_result(c))
			{
				printf("¦");
				break;
			}

			printf("%c", c);
		}

		printf("\n");
	}
	printf("\n");

	printf("> Multiply char values\n");
	for (int i = -5; i < 6; ++i)
	{
		/* Skip zero */
		if (i == 0)
			continue;

		printf("%d: ", i);
		for (int j = 0; j < len; ++j)
		{
			c = argv[1][j] * i;
			if (!validate_result(c))
			{
				printf("¦");
				break;
			}

			printf("%c", c);
		}
		printf("\n");
	}
	printf("\n");

	printf("> Divide char values\n");
	for (int i = -5; i < 6; ++i)
	{
		/* Skip zero */
		if (i == 0)
			continue;

		printf("%d: ", i);
		for (int j = 0; j < len; ++j)
		{
			c = argv[1][j] / i;
			if (!validate_result(c))
			{
				printf("¦");
				break;
			}

			printf("%c", c);
		}
		printf("\n");
	}
	printf("\n");


	return 0;
}
