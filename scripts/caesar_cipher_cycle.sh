#!/bin/sh

[ -z "$1" ] && echo "Usage: ./caesar_cipher_cycle.sh [word]" && exit 1

EXEC_PATH=$(dirname $0)
CAESAR_CIPHER="$EXEC_PATH/../build/tools/caesar_cipher"
[ -f "$CAESAR_CIPHER" ] || { echo "Caesar cipher tool missing! Did you remember to compile all of the tools?"; exit 1; }

for i in $(seq 0 25)
do
	"$CAESAR_CIPHER" "$1" "$i"
done
