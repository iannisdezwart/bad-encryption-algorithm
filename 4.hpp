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

	message[16 - 1] = held_char;
}

// This function unshifts the message

void unshift_message()
{
	// Hold the last character

	uint8_t held_char = message[16 - 1];

	// Shift the message

	for (int i = 16 - 1; i > 0; i--) {
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

		for (int i = 0; i < 16; i++) {
			message[i] = substitute_byte(message[i], s_box);
		}

		// 1.2) Shift the message

		shift_message();

		// 1.3) XOR the i-th byte of the message with the i-th byte of the key

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

		// 1.2) Unshift the message

		unshift_message();

		// 1.3) Substitute each byte using the inverse s-box

		for (int i = 0; i < 16; i++) {
			message[i] = substitute_byte(message[i], inv_s_box);
		}
	}
}