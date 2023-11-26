#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>

size_t fread (void *__restrict __ptr, size_t __size, size_t __n, FILE *__restrict __stream) __wur
{
	static size_t (*fread_func)(void*, size_t, size_t, FILE*) = NULL;
	if (!fread_func)
    {
        fread_func = (size_t(*)(void*, size_t, size_t, FILE*)) dlsym(RTLD_NEXT, "fread");
    }

	int result = fread_func(__ptr, __size, __n, __stream);
	printf("fread() called with return value: %s\n", (char*)__ptr);
	return result;
}
