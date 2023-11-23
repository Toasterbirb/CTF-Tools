#!/bin/bash

let passCount=0
let failCount=0

function _pass()
{
	echo -e "\t- OK"
	let passCount+=1
}

function _fail()
{
	echo -e "\t- FAIL"
	let failCount+=1
}

function _testcase()
{
	text="$1"
	rotation="$2"
	expectedResult="$3"

	result=$(./caesar_cipher "$text" "$rotation")
	echo -n "Input: '$text', rotation: $rotation | Output: $result"

	[ "$result" == "$expectedResult" ] && _pass || _fail
}

echo "Running tests..."

# TESTS #
_testcase "abcdefghijklmnopqrstuvwxyz" 5 "fghijklmnopqrstuvwxyzabcde"
_testcase "abcdefghijklmnopqrstuvwxyz" -5 "vwxyzabcdefghijklmnopqrstu"
_testcase "iosuerhtsouiecrbykseruiytgks" 12 "uaegqdtfeaguqodnkweqdgukfswe"
_testcase "text with spaces" 3 "whaw zlwk vsdfhv"
_testcase "FASDLKFJAWEF" 2 "HCUFNMHLCYGH"
_testcase "UppErCaseandLowerCase" -6 "OjjYlWumyuhxFiqylWumy"
_testcase "Uppercase, spaces, commas + lowercase letters and some other stuff" 8 "Cxxmzkiam, axikma, kwuuia + twemzkiam tmbbmza ivl awum wbpmz abcnn"

echo -e "\nResults:\nPass: $passCount\nFail: $failCount"
