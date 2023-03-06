#!/bin/bash
EXEC_PATH=$(dirname $0)
HASHCAT_POTFILE="$HOME/.local/share/hashcat/hashcat.potfile"

# Make sure that all of the required applications are installed
which hashcat &>/dev/null || { echo "hashcat not installed"; exit 1; }
which curl &>/dev/null || { echo "curl not installed"; exit 1; }

# Check if the rockyou-list is available in ~/.cache
WORD_LIST_DIR="$HOME/.cache/ctf_tools/wordlists"
ROCKYOU_LIST="$WORD_LIST_DIR/rockyou.txt"
mkdir -p $WORD_LIST_DIR

_fetch_rockyou() {
	echo "Fetching the rockyou word list"
	curl -o "${ROCKYOU_LIST}.gz" https://gitlab.com/kalilinux/packages/wordlists/-/raw/kali/master/rockyou.txt.gz
	echo "Extracting..."
	gunzip "${ROCKYOU_LIST}.gz"
}

[ -f "$ROCKYOU_LIST" ] || _fetch_rockyou

# Cache the hashcat help command to make things a bit quicker
HASHCAT_HELP="$HOME/.cache/ctf_tools/hashcat_help.txt"
[ -f $HASHCAT_HELP ] || hashcat --help > $HASHCAT_HELP

HEX_CHECK="$EXEC_PATH/../build/tools/hex_check"
[ -f $HEX_CHECK ] || { echo "hex_check not found! Did you remember to compile all of the tools?"; exit 1; }

HASH="$1"

# Check the potfile before doing anything
[ -f $HASHCAT_POTFILE ] && POTFILE_RESULT="$(awk "/^$HASH/" $HASHCAT_POTFILE)"
[ -n "$POTFILE_RESULT" ] && echo "Password: $(echo $POTFILE_RESULT | cut -d':' -f2)" && exit 0

# Make sure that the hash is a hex string
$HEX_CHECK "$HASH" || exit 1

HASH_LENGTH="$(printf $HASH | wc -m)"
echo "Length: $HASH_LENGTH"

_crack() {
	echo "Cracking mode: $(awk -F'|' "/^[[:space:]]*$1 / { print \$2 }" $HASHCAT_HELP | head -n 1 | tr -d ' ')"

	echo "Starting with a dictionary attack. Hit 's' to see statistics"

	# Dictionary attack
	hashcat --quiet -O -a 0 -m $1 $2 $ROCKYOU_LIST

	# Brute force
	[ $? -eq 1 ] && echo "Brute forcing lower and uppercase alphabet + numbers" && hashcat --quiet -O -a 3 -m $1 -i --increment-min=1 --increment-max=7 -1 abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 $2 ?1?1?1?1?1?1?1?1

	# Check the results from the potfile
	RESULT="$(awk -F':' "/^$HASH/ { print \$2 }" $HASHCAT_POTFILE)"

	[ -n "$RESULT" ] && echo "Password: $RESULT" || echo "Couldn't crack the hash :("
}

case $HASH_LENGTH in
	32) # md5
		_crack "0" "$HASH"

		# If no results were found, give md4 a try
		[ -z "$RESULT" ] && _crack "900" "$HASH"
		;;

	40) # sha1
		_crack "100" "$HASH"
		;;

	56) # sha224
		_crack "1300" "$HASH"
		;;

	64) # sha256
		_crack "1400" "$HASH"
		;;

	96) # sha384
		_crack "10800" "$HASH"
		;;

	128) # sha512 :(
		_crack "1700" "$HASH"
		;;

	*) # Coudn't do any hash type guesses
		echo "Couldn't guess the hash type! :("
		;;
esac
