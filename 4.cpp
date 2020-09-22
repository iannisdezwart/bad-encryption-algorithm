/*
	Algorithm description:

	Encryption:
		1) Loop 16 times:
			1) Substitute each byte from the message using the Reijndael S-box
			2) Shift the message
			3) XOR message with key

	Decryption:
		1) Loop 16 times:
			1) XOR message with key
			2) Unshift the message
			3) Substitute each byte from the message using the inverse Reijndael S-box

	Why is it bad?
		- Small changes in the input will result in small changes in the output
			(Avalanche effect is very low)

	Why is it better than the previous version?
		- There is more diffusion, since we are doing more than one round now
		- It does not map byte to byte anymore, since every byte is calculated
			by all bytes of the key (because we do key (16) rounds)
		- The whole string will look (pseudo-)random, because we no longer
			have the issue where bytes are encrypted with the null byte because
			the key is less than 16 bytes
*/

#include <iostream>
#include <stdint.h>
using namespace std;

// Globals

const int len = 16;
const int rounds = 16;

uint8_t message[len] = {};
uint8_t key[len] = {};

// Lookup tables

// The box from our good old Reijndael

uint8_t s_box[16][16] = {
	{ 0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76 },
	{ 0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0 },
	{ 0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15 },
	{ 0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75 },
	{ 0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84 },
	{ 0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf },
	{ 0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8 },
	{ 0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2 },
	{ 0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73 },
	{ 0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb },
	{ 0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79 },
	{ 0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08 },
	{ 0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a },
	{ 0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e },
	{ 0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf },
	{ 0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 }
};

// The box from our good old Reijndael from an inverse universe

uint8_t inv_s_box[16][16] = {
	{ 0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb },
	{ 0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb },
	{ 0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e },
	{ 0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25 },
	{ 0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92 },
	{ 0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84 },
	{ 0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06 },
	{ 0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b },
	{ 0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73 },
	{ 0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e },
	{ 0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b },
	{ 0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4 },
	{ 0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f },
	{ 0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef },
	{ 0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61 },
	{ 0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d }
};

// This function does the substitution work

uint8_t substitute_byte(uint8_t byte, uint8_t lookup_table[16][16])
{
	// Get the value of the first 4 bits of the byte

	uint8_t y = byte / 16;

	// Get the value of the last 4 bits of the byte

	uint8_t x = byte % 16;

	// Return the corresponding byte

	return lookup_table[y][x];
}

// This function shifts the message

void shift_message()
{
	// Hold the first character

	uint8_t held_char = message[0];

	// Shift the message

	for (int i = 1; i < len; i++) {
		message[i - 1] = message[i];
	}

	// Place the held character at the end

	message[len - 1] = held_char;
}

// This function unshifts the message

void unshift_message()
{
	// Hold the last character

	uint8_t held_char = message[len - 1];

	// Shift the message

	for (int i = len - 1; i > 0; i--) {
		message[i] = message[i - 1];
	}

	// Place the held character at the start

	message[0] = held_char;
}

// This function is the placeholder for the encryption procedure

void encrypt_message_with_key()
{
	// 1) Do 16 rounds

	for (int round = 0; round < rounds; round++) {
		// 1.1) Substitute each byte using the s-box

		for (int i = 0; i < len; i++) {
			message[i] = substitute_byte(message[i], s_box);
		}

		// 1.2) Shift the message

		shift_message();

		// 1.3) XOR the i-th byte of the message with the i-th byte of the key

		for (int i = 0; i < len; i++) {
			message[i] ^= key[i];
		}
	}
}

// This function is the placeholder for the decryption procedure

void decrypt_message_with_key()
{
	// 1) Do 16 rounds

	for (int round = 0; round < rounds; round++) {
		// 1.1) XOR the i-th byte of the message with the i-th byte of the key

		for (int i = 0; i < len; i++) {
			message[i] ^= key[i];
		}

		// 1.2) Unshift the message

		unshift_message();

		// 1.3) Substitute each byte using the inverse s-box

		for (int i = 0; i < len; i++) {
			message[i] = substitute_byte(message[i], inv_s_box);
		}
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