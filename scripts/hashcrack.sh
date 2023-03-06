#!/bin/sh

HASH="$1"
HASH_LENGTH="$(echo $HASH | wc -m)"

case $HASH_LENGTH in
	33) # md5
		echo "Guessing md5..."
		;;
esac
