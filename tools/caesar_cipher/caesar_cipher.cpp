#include <algorithm>
#include <execution>
#include <iostream>
#include <string>

void printhelp()
{
	puts("Usage: caesar_cipher [text] [rotation]");
	puts("For example: ./caesar_cipher \"hello world\" 13, would rotate the string 'hello world' by 13");
}

char rotate_char(char c, char min, char max, int rotation_amount)
{
	if (c >= min && c <= max)
	{
		/* Check if should wrap around */
		if (c + rotation_amount > max)
		{
			int wrap = (c + rotation_amount) - max;
			c = min - 1 + wrap;
			return c;
		}
		else if (c + rotation_amount < min)
		{
			int wrap = min - (c + rotation_amount);
			c = max + 1 - wrap;
			return c;
		}

		c += rotation_amount;
	}

	return c;
}

int main(int argc, char** argv)
{
	if (argc != 3)
	{
		printhelp();
		return 1;
	}

	std::string text = argv[1];
	int rotation_amount = std::stoi(argv[2]);

	/* Process the text */
	std::for_each(std::execution::par, text.begin(), text.end(), [rotation_amount](char& c) {
		/* Handle uppercase letters */
		c = rotate_char(c, 'A', 'Z', rotation_amount);

		/* Handle lowercase letters */
		c = rotate_char(c, 'a', 'z', rotation_amount);
	});

	std::cout << text << "\n";

	return 0;
}
