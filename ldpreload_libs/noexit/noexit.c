#include <stdio.h>

void exit(int status)
{
	printf("exit(%d) cancelled\n", status);
}
