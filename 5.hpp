/*
	Algorithm description:

	Encryption:
		1) Loop 16 times:
			1) Substitute each byte from the message using the Reijndael S-box
			2) Shift the message
			3) Mix the message
			4) XOR message with key

	Decryption:
		1) Loop 16 times:
			1) XOR message with key
			3) Unmix the message
			3) Unshift the message
			4) Substitute each byte from the message using the inverse Reijndael S-box

	Why is it bad?
		- It is not

	Why is it better than the previous version?
		- There is an avalanche effect of roughly 50%, therefore, the output
			of the algorithm is completely different on small changes of the input
*/

#include <iostream>
#include <stdint.h>
#include "reijndael-s-box.hpp"
using namespace std;

// Globals

const int rounds = 16;

uint8_t message[16] = {};
uint8_t key[16] = {};

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

	for (int i = 1; i < 16; i++) {
		message[i - 1] = message[i];
	}

	// Place the held character at the end

	message[15] = held_char;
}

// This function unshifts the message

void unshift_message()
{
	// Hold the last character

	uint8_t held_char = message[15];

	// Shift the message

	for (int i = 15; i > 0; i--) {
		message[i] = message[i - 1];
	}

	// Place the held character at the start

	message[0] = held_char;
}

// This function mixes the the message, for a
// higher avalanche effect

void mix_message()
{
	// Calculate the xor_sum

	uint8_t xor_sum = 0;

	for (int i = 0; i < 16; i++) {
		xor_sum ^= message[i];
	}

	// Xor each byte of the message

	for (int i = 0; i < 16; i++) {
		message[i] ^= xor_sum;
	}
}

// The inverse of mix_message() is just itself

#define unmix_message() mix_message()

// This function is the placeholder for the encryption procedure

void encrypt_message_with_key()
{
	// 1) Do 16 rounds

	for (int round = 0; round < rounds; round++) {
		// 1.1) Substitute each byte using the s-box

		for (int i = 0; i < 16; i++) {
			message[i] = substitute_byte(message[i], s_box);
		}

		// 1.2) Shift the message

		shift_message();

		// 1.3 Mix message

		mix_message();

		// 1.4) XOR the i-th byte of the message with the i-th byte of the key

		for (int i = 0; i < 16; i++) {
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

		for (int i = 0; i < 16; i++) {
			message[i] ^= key[i];
		}

		// 1.2 Unmix message

		unmix_message();

		// 1.3) Unshift the message

		unshift_message();

		// 1.4) Substitute each byte using the inverse s-box

		for (int i = 0; i < 16; i++) {
			message[i] = substitute_byte(message[i], inv_s_box);
		}
	}
}