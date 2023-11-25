#include <string.h>
#include <stdio.h>

int strcmp(const char *s1, const char *s2)
{
	printf("strcmp(%s, %s)\n", s1, s2);

	int s1_len = strlen(s1);
	int s2_len = strlen(s2);

	if (s1_len != s2_len)
		return 1;

	for (int i = 0; i < s1_len; ++i)
	{
		if (s1[i] < s2[i])
			return -1;

		if (s1[i] > s2[i])
			return 1;
	}

	return 0;
}
