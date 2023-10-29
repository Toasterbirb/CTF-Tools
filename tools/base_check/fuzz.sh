#!/bin/sh

# Script for making sure that base32 and base64 strings detected properly
# False positives aren't that big of a deal, but missed strings are a big problem

# Try 100 strings
for i in $(seq 1 100)
do
	# Test base64
	STRING="$(head -n5 /dev/urandom | base64)"
	if [ -z "$(./base_check $STRING | grep base64)" ]
	then
		echo $STRING
	fi

	# Test base32
	STRING="$(head -n5 /dev/urandom | base32)"
	if [ -z "$(./base_check $STRING | grep base32)" ]
	then
		echo $STRING
	fi
done
