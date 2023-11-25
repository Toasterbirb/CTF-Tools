#!/bin/bash

# Look for plaintext flags from different file formats (ELF, PNG, JPEG etc.)
# The script will deduce the file format and try to carve the flag out

# Tool paths
EXEC_PATH=$(dirname $0)
BASE_CHECK="$EXEC_PATH/../build/tools/base_check"
BASE_FILTER="$EXEC_PATH/../build/tools/base_filter"
HEX_CHECK="$EXEC_PATH/../build/tools/hex_check"

PASSWORD_LIST="/usr/share/wordlists/rockyou.txt"

# Print program usage help
function print_usage()
{
	echo "Usage: $0 [flag format ex. flag{%}] [file path]"
	exit 1
}

# Check if a dependency program exists on the system
function check_program()
{
	unset MISSING_DEPS
	for i in $@
	do
		[ ! -f "/usr/bin/$i" ] && [ ! -f "/usr/local/bin/$i" ] && [ ! -f "/usr/bin/vendor_perl/$i" ] && echo "$i is not installed!" && MISSING_DEPS="true"
	done

	if [ -n "$MISSING_DEPS" ]
	then
		echo "Missing dependencies were found! Aborting mission..."
		exit 1
	fi
}

function check_file()
{
	unset MISSING_FILES
	for i in $@
	do
		[ ! -f "$i" ] && echo "$i was not found!" && MISSING_FILES="true"
	done

	if [ -n "$MISSING_FILES" ]
	then
		echo "Missing files were found! Aborting mission..."
		exit 1
	fi
}

function check_flag()
{
	# Print the flag if the input is non-empty
	#
	# If the flag was found, commit suicide with exit code 141
	# This is to avoid any unnecessary checks that might waste time

	read flag_input
	[ -n "$flag_input" ] && echo -e "\e[31mThe flag was found with [$1]!\e[0m" && echo "$flag_input" && kill -SIGPIPE "$$"
}

# Usage: strings_check [file path to check] [parent check]
check_program strings
function strings_check()
{
	[ -n "$2" ] && PARENT_CHECK="$2, "
	flag_finder "${PARENT_CHECK}strings" "$(strings "$1")"
	flag_finder "${PARENT_CHECK}strings with whitespace removed" "$(strings "$1" | tr -d ' ')"
	flag_finder "${PARENT_CHECK}strings with special characters removed" "$(strings "$1" | tr -d ',.!@$%&')"
}

# Check if all args are set correctly
[ -z "$1" ] && print_usage
[ -z "$2" ] && print_usage
[ ! -f "$2" ] && echo "File $2 doesn't exist!" && exit 2

FLAG_FORMAT="$1"
FILE="$2"

# Create a glob version of the flag pattern
FLAG_GLOB="$(echo $FLAG_FORMAT | sed 's/%/\.\*/g')"

# Check the flag format length in base64
FLAG_BASE64_LEN="$(echo $FLAG_FORMAT | base64 | wc -c)"

# Check for some basic programs
check_program file base64 base32

# Define some more functions
flag_finder()
{
	DATA="$2"

	# Look for plain text flags
	echo "$DATA" | grep -o "$FLAG_GLOB" | check_flag "$1, plaintext"

	# Only process lines that are longer than the length of the flag format
	# in base64. This should save lots of time when there's lots of data to go through
	FILTERED_DATA="$(echo "$DATA" | awk "length(\$0) > $FLAG_BASE64_LEN")"

	# Write the filtered data into a text file to get around argument length
	# problems with filtering
	FLAG_FIND_DATA_FILE="$(mktemp "/tmp/flag_finder_dataXXXXXX")"
	echo "$FILTERED_DATA" > $FLAG_FIND_DATA_FILE

	# Find all base64 candidates
	BASE64_STRINGS="$("$BASE_FILTER" "$FLAG_FIND_DATA_FILE" base64)"

	# Find all base32 candidates
	BASE32_STRINGS="$("$BASE_FILTER" "$FLAG_FIND_DATA_FILE" base32)"

	# Check base64 strings for flags
	for i in $BASE64_STRINGS
	do
		echo -n "$i" | base64 -d | grep -o "$FLAG_GLOB" | check_flag "$1, base64"
	done

	# Check base32 strings for flags
	for i in $BASE32_STRINGS
	do
		echo -n "$i" | base32 -d | grep -o "$FLAG_GLOB" | check_flag "$1, base32"
	done

	# Look for hex strings
	for i in $FILTERED_DATA
	do
		if "$HEX_CHECK" "$i" &>/dev/null
		then
			echo -n "$i" | xxd -r -p | grep -o "$FLAG_GLOB" | check_flag "$1, hex"
		fi
	done

	rm $FLAG_FIND_DATA_FILE
}

# Check the filetype
FILETYPE="$(file -b --mime-type "$FILE")"

# Do general filetype specific checks first
case $FILETYPE in
	image/*)
		# EXIF-data check
		check_program "exiftool"
		flag_finder "EXIF-data" "$(exiftool "$FILE" | cut -d':' -f2 | tr -d '^ ')"
		;;

	application/pdf)
		# Check the EXIF-data
		check_program "exiftool"
		flag_finder "EXIF-data" "$(exiftool "$FILE" | cut -d':' -f2 | tr -d '^ ')"

		# Convert the pdf file to text and grep it
		check_program "pdftotext"
		flag_finder "PDF to text" "$(pdftotext "$FILE" -)"

		# Check the pdf metadata
		check_program "pdfinfo"
		flag_finder "PDF metadata" "$(pdfinfo "$FILE" | cut -d ':' -f2 | sed 's/^[[:space:]]*//g')"

		;;

	text/plain)
		# If the file is simply plain text (and possibly large)
		# we can try grepping for the flag
		flag_finder "grep" "$(grep "$FLAG_GLOB" "$FILE")"

		# Try hex and base64 conversions in case they result in something sensible
		TEMP_FILE="$(mktemp flag_searchXXX)"
		cat "$FILE" | xxd -r -p 										> ${TEMP_FILE}
		cat "$FILE" | xxd -r -p | tr -d '\n' 							> ${TEMP_FILE}_1
		cat "$FILE" | xxd -r -p | base64 -d 2>/dev/null 				> ${TEMP_FILE}_2
		cat "$FILE" | base64 -d 2>/dev/null | xxd -r -p 				> ${TEMP_FILE}_3
		cat "$FILE" | base64 -d 2>/dev/null | tr -d '\n' 				> ${TEMP_FILE}_4
		cat "$FILE" | xxd -r -p | base64 -d 2>/dev/null | xxd -r -p 	> ${TEMP_FILE}_5

		# Check if any sensible filetypes were found
		for i in ${TEMP_FILE} ${TEMP_FILE}_{1..5}
		do
			TYPE="$(file -b --mime-type "$i")"
			if [ "$TYPE" != "text/plain" ]
			then
				echo "Found a file of type $TYPE: $i"
				strings_check "$i" "plaintext decoding"
			else
				rm $i
			fi
		done
		;;
esac

## General strings check ##
# The strings check is done last because it might consume lots of time
# with larger files.

# No need for parent "check" source here, since this is the first strings run
strings_check "$FILE"

# Look for the flag in the hexdump of the file in case the characters
# are not all in a row but instead have empty air in between
check_program hexdump
flag_finder "hexdump" "$(hexdump -e "16 \"%_p\" \"\\n\"" "$FILE" | tr -d '\n.')"


## binwalk ##
check_program binwalk
TMP_BINWALK="$(mktemp -u -d ./binwalk_out_XXXX)"
binwalk --dd='.*' -e -r -C "$TMP_BINWALK" "$FILE"
BINWALK_FILES="$(find $TMP_BINWALK -type f)"
for i in $BINWALK_FILES
do
	# Run strings on everything
	strings_check "$i" "binwalk"
done
rm -r "${TMP_BINWALK:?}"

# Steganography tool checks, that may include time consuming password cracking
case $FILETYPE in
	image/jpeg)
		check_program "stegseek"
		check_file "$PASSWORD_LIST"

		STEGSEEK_OUTPUT="$(mktemp -u ./stegoutXXXX)"
		stegseek "$FILE" "$PASSWORD_LIST" "$STEGSEEK_OUTPUT"

		# Only check the flag directly if the output was in text format
		if [ -f "$STEGSEEK_OUTPUT" ]
		then
			if [ "$(file -b --mime-type $STEGSEEK_OUTPUT)" == "text/plain" ]
			then
				flag_finder "stegseek" "$(cat $STEGSEEK_OUTPUT)" && rm -f $STEGSEEK_OUTPUT
			else
				echo -e "\e[31mA flag was found from the JPEG file but it was in something other than plaintext format!\e[0m"
				echo "The output file can be found at $STEGSEEK_OUTPUT"
			fi
		fi
		;;
esac

