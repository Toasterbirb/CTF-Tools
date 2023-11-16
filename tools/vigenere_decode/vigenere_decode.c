#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

/* Check if the key contains only letters */
int is_valid_key(const char* key, const int len)
{
	int result = 0;
	for (int i = 0; i < len; ++i)
		result += (key[i] >= 'a' && key[i] <= 'z') || (key[i] >= 'A' && key[i] <= 'Z');

	return result == len;
}

/* Converts the given string to lowercase inplace */
void to_lower(char* text, int len)
{
	for (int i = 0; i < len; ++i)
		text[i] = tolower(text[i]);
}

int decode_char(const char key_c, const char cipher_c)
{
	/* Only decode letters */
	if (cipher_c < 'a' || cipher_c > 'z')
		return cipher_c;

	int decoded = cipher_c - (key_c - 'a');

	if (decoded > 'a' - 1)
	{
		return decoded;
	}

	/* Wrap around if the subtraction went below 'a' char */
	return decoded + 26;
}

int reverse_char(const char plain_c, const char cipher_c)
{
	/* Only decode letters */
	if (cipher_c < 'a' || cipher_c > 'z')
		return 0;

	int key = cipher_c - plain_c + 'a';

	if (key > 'a' - 1)
		return key;

	return key + 26;
}

int main(int argc, char** argv)
{
	if (argc == 1)
	{
		puts("Usage: vigenere_decode [options] [cipher_text]");
		puts("");
		puts("Options:");
		puts("  -k [key]                known key that can be used to decode the cipher text");
		puts("  -g [known_plaintext]    omit the key and instead try to guess parts of it based");
		puts("                          on known plain text");
		return 0;
	}

	if (argc > 4 || argc < 4)
	{
		puts("Invalid amount of arguments were given!");
		puts("Run vigenere_decode without any args to see the help page");
		return 1;
	}

	/* Make sure that the first argument is 2 chars long */
	if (strlen(argv[1]) != 2)
	{
		printf("Invalid argument was given: %s\n", argv[1]);
		return 1;
	}


	/* Variables */
	char* key 				= argv[2];
	char* known_plaintext 	= argv[2];
	char* cipher_text 		= argv[3];
	char* plaintext;
	unsigned int key_len, cipher_len;
	int decode_mode = TRUE;

	/** Parse the arguments **/
	cipher_len 	= strlen(argv[3]);

	switch (argv[1][1])
	{
		case ('k'):
			decode_mode = TRUE;
			break;

		case ('g'):
			decode_mode = FALSE;
			break;

		default:
			printf("Invalid argument was given: %s\n", argv[1]);
			return 1;
			break;
	}

	/* Convert the cipher text to lowercase */
	to_lower(cipher_text, cipher_len);

	/* Allocate some space for plaintext */
	plaintext = (char*)malloc(sizeof(char) * cipher_len + 1);

	/* Decode the cipher text if in decoding mode with known key */
	if (decode_mode)
	{
		key_len = strlen(argv[2]);

		if (!is_valid_key(key, key_len))
		{
			printf("\"%s\" is an invalid key! The key can only contain letters.\n", key);
			return 3;
		}

		/* Convert the key to lowercase */
		to_lower(key, key_len);

		/* Stretch the key to the length of the cipher text */
		char* key_text = (char*)malloc(sizeof(char) * cipher_len + 1);
		int key_pos = 0;

		for (int i = 0; i < cipher_len; ++i)
		{
			/* Parts of the cipher that are non-alphabetic don't need decoding */
			if (cipher_text[i] > 'a' - 1 && cipher_text[i] < 'z' + 1)
			{
				key_text[i] = key[key_pos];
				key_pos++;
			}
			else
			{
				key_text[i] = 95; /* use '_' as a placeholder for debugging purposes */
			}

			if (key_pos > key_len - 1)
				key_pos = 0;
		}

		/* Loop over the cipher text and decode it using the stretched key */
		for (int i = 0; i < cipher_len; ++i)
		{
			plaintext[i] = decode_char(key_text[i], cipher_text[i]);
		}

		/* Print out the result */
		printf("%s\n", plaintext);

		/* Free any allocated memory */
		free(key_text);
	}
	/* Try to guess parts of the key when not in decode mode */
	else
	{
		/* Print out valid key chars for the length of known plaintext */
		unsigned int known_plaintext_len = strlen(known_plaintext);

		/* If there's more known plaintext than cipher text
		 * cap to cipher text length */
		if (known_plaintext_len > cipher_len)
			known_plaintext_len = cipher_len;

		for (int i = 0; i < known_plaintext_len; ++i)
			printf("%c", reverse_char(known_plaintext[i], cipher_text[i]));

		printf("\n");
	}

	/* Free any allocated heap memory */
	free(plaintext);

	return 0;
}
