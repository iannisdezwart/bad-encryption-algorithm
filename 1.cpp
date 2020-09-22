/*
	Algorithm description:

	Encryption:
		1) XOR message with key

	Decryption:
		1) XOR message with key

	Why is it bad?
		- It maps byte to byte, therefore there is no diffusion
		- There is no confusion
		- Edge cases like having the same message and key result in
			predictable outcomes
		- If the key is smaller than 16 bytes, the remaining bytes
			will not be encrypted at all, since ORIG XOR 0 = ORIG
		- If we have a sample of the original and its encrypted form,
			we can calculate the key, since ENCR XOR ORIG = KEY
		- Small changes in the input will result in small changes in the output
			(Avalanche effect is very low)
*/

#include <iostream>
#include <stdint.h>
using namespace std;

// Globals

const int len = 16;

uint8_t message[len] = {};
uint8_t key[len] = {};

// This function is the placeholder for the encryption procedure

void encrypt_message_with_key()
{
	// 1) XOR the i-th byte of the message with the i-th byte of the key

	for (int i = 0; i < len; i++) {
		message[i] ^= key[i];
	}
}

// This function is the placeholder for the decryption procedure

void decrypt_message_with_key()
{
	// 1) XOR the i-th byte of the message with the i-th byte of the key

	for (int i = 0; i < len; i++) {
		message[i] ^= key[i];
	}
}

// This function will scan at max len bytes from stdin
// It is used to retrieve the plain message and the key

void scan(uint8_t *dest)
{
	int read_bytes = 0;

	while (read_bytes < len) {
		// Get one char from stdin

		uint8_t next_char = fgetc(stdin);

		// Stop when the next_char is a newline

		if (next_char == '\n') break;

		dest[read_bytes] = next_char;
		read_bytes++;
	}

	// Remove trailing newline from stdin

	if (read_bytes == len) {
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

// This function terminates the program
// It is called when the user gives bad input to our program

void die()
{
	cout << "U good?\n";
	exit(1);
}

// Our dear entry point

int main()
{
	// Get the choice from stdin, this should be either '1', '2', '3' or '4'

	char choice = fgetc(stdin);

	// Expect a newline from stdin, else die

	if (fgetc(stdin) != '\n') {
		die();
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

		scan(message);
		scan(key);

		// Decrypt the cihpertext

		decrypt_message_with_key();

		// Print the plaintext to stdout

		if (choice == '2') print_message_hex();
		else print_message();
	} else {
		// User made a choice that does not exist

		die();
	}
}