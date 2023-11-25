# NoSleep

Overloads `unsigned int sleep(unsigned int seconds);` and `int usleep(useconds_t usec)` functions from unistd.h. The sleep function is made to not do anything and simply return a zero basically skipping all sleeping in the program.
