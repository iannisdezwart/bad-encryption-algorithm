#include <iostream>
#include <stdint.h>
#include "5.hpp"
using namespace std;

// This function will scan at max 16 bytes from stdin
// It is used to retrieve the plain message and the key

void scan(uint8_t *dest)
{
	int read_bytes = 0;

	while (read_bytes < 16) {
		// Get one char from stdin

		uint8_t next_char = fgetc(stdin);

		// Stop when the next_char is a newline

		if (next_char == '\n') break;

		dest[read_bytes] = next_char;
		read_bytes++;
	}

	// Remove trailing newline from stdin

	if (read_bytes == 16) {
		if (fgetc(stdin) != '\n') {
			cout << "Input buffer too large";
			exit(1);
		}
	}
}

// This function does the same as scan() but uses hex encoded
// bytes as its input

void scan_hex(uint8_t *dest)
{
	int read_bytes = 0;

	while (read_bytes < 16) {
		// Get one char from stdin

		uint8_t next_char;
		scanf("%hhx", &next_char);

		dest[read_bytes] = next_char;
		read_bytes++;
	}

	// Remove trailing newline from stdin

	if (read_bytes == 16) {
		if (fgetc(stdin) != '\n') {
			cout << "Input buffer too large";
			exit(1);
		}
	}
}

// This function prints the raw bytes of the message to stdout

void print_message()
{
	for (int i = 0; i < 16; i++) {
		printf("%c", message[i]);
	}
}

// This function prints the message as hex bytes to stdout

void print_message_hex()
{
	for (int i = 0; i < 16; i++) {
		printf("%02x ", message[i]);
	}

	cout << '\n';
}

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