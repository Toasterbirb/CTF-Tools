#include <stdio.h>
#include <string.h>

#include "is_base.h"

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

	int had_results = 0;

	/** Check for base64 **/
	if (is_base64_str(argv[1], len))
	{
		printf("base64\n");
		had_results++;
	}

	/* The string needs to be divisible by 8 to be base32 */
	if (is_base32_str(argv[1], len))
	{
		printf("base32\n");
		had_results++;
	}

	if (had_results == 0)
		return 1;

	return 0;
}
