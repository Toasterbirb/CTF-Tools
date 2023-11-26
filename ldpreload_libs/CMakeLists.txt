# Combination library
add_library(recon SHARED
	./strcmp/strcmp.c
	./fread/fread.c
)

add_library(nosleep SHARED ./nosleep/nosleep.c)
add_library(noexit SHARED ./noexit/noexit.c)
add_library(noptrace SHARED ./ptrace/ptrace.c)
add_library(strcmp SHARED ./strcmp/strcmp.c)
add_library(fread SHARED ./fread/fread.c)
