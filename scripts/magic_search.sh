#!/bin/bash

# This script is supposed to help with going through
# Cyber Chef magic operation output. This is done by
# using dictionaries

# Check dependencies
which curl &>/dev/null || { echo "curl not installed!"; exit 1; }

# Download the dictionaries if they aren't in the cache
DICTIONARY_CACHE_DIR="$HOME/.cache/ctf_tools/dictionaries"
mkdir -p $DICTIONARY_CACHE_DIR

KOTUS_DICT="$DICTIONARY_CACHE_DIR/finnish.txt"
ENGLISH_DICT="$DICTIONARY_CACHE_DIR/english.txt"

_fetch_kotus() {
	echo "Downloading the kotus word list..."
	curl -s -o "${KOTUS_DICT}.tar.gz" https://kaino.kotus.fi/sanat/nykysuomi/kotus-sanalista-v1.tar.gz

	echo "Extracting..."
	tar -C "$DICTIONARY_CACHE_DIR" -xvf "${KOTUS_DICT}.tar.gz"

	echo "Parsing the word list xml file..."
	sed 's/^<[a-z]*><[a-z]*>//g; s/<.*//g; /[<>]/d; /^-/d; /^.*[[:space:]].*/d' "$DICTIONARY_CACHE_DIR/kotus-sanalista_v1/kotus-sanalista_v1.xml" | sort | uniq -u > "$KOTUS_DICT"

	echo "Cleaning up..."
	rm -rv "$DICTIONARY_CACHE_DIR/kotus-sanalista_v1" "${KOTUS_DICT}.tar.gz"
}

_fetch_english() {
	echo "Downloading English word list from github"
	curl -s -o "$ENGLISH_DICT" "https://raw.githubusercontent.com/dwyl/english-words/master/words.txt"
}

[ -f "$KOTUS_DICT" ] || _fetch_kotus
[ -f "$ENGLISH_DICT" ] || _fetch_english

[ -f "$1" ] || { echo "Usage: ./magic_search.sh [path to CyberChef magic output file]"; exit 1; }

DATA="$(cat "$1" | jq -r '.[].data' | sort | uniq -u | sed 's/_/ /g; s/-/ /g; s/\./ /g')"

echo "Trying dictionaries..."
for i in $DATA
do
	grep -F -- "$i" $KOTUS_DICT &>/dev/null
	[ $? -eq 0 ] && echo "Match for [$i] found from a Finnish dictionary"

	grep -F -- "$i" $ENGLISH_DICT &>/dev/null
	[ $? -eq 0 ] && echo "Match for [$i] found from an English dictionary"
done
