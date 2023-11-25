# strcmp

Implements the `int strcmp(const char *s1, const char *s2);` function from libc. The function override will leak the parameters passed to strcmp and then do the string comparison as usual.

Example output
```
toasterbirb@tux ~/projects/ctf/bits $ LD_PRELOAD=~/git/ctf-tools/build/ldpreload_libs/libstrcmp.so ./bits asdf
strcmp(xctf{S1MPL3_B1N4RY}, asdf)
Wrong
toasterbirb@tux ~/projects/ctf/bits $
```
