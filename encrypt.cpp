#include <iostream>
#include <stdint.h>
#include "5.hpp"
#include "scanning_and_printing.hpp"
using namespace std;

// Our dear entry point

int main()
{
	// Get the choice from stdin, this should be either '1', '2', '3' or '4'

	char choice = fgetc(stdin);

	// Expect a newline from stdin, else die

	if (fgetc(stdin) != '\n') {
		cout << "Expected a newline after the choice";
		exit(1);
	}

	if (choice == '1' || choice == '3') {
		// Encrypt

		// Scan the plaintext and the key from stdin

		scan(message);
		scan(key);

		// Encrypt the plaintext

		encrypt_message_with_key();

		// Print the ciphertext to stdout

		if (choice == '1') print_message_hex();
		else print_message();
	} else if (choice == '2' || choice == '4') {
		// Decrypt

		// Scan the ciphertext and the key from stdin

		if (choice == '2') scan_hex(message);
		else scan(message);
		scan(key);

		// Decrypt the cihpertext

		decrypt_message_with_key();

		// Print the plaintext to stdout

		if (choice == '2') print_message_hex();
		else print_message();
	} else {
		// User made a choice that does not exist

		cout << "That choice does not exist";
		exit(1);
	}
}