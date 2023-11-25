# LD_PRELOAD libraries

If there's a program that makes calls to external functions, the functionality of those said functions can be modified for better or for worse.

This directory contains several utility functions that can be preloaded with the `LD_PRELOAD` env variable. Simply point LD_PRELOAD to the shared library you want to use.
