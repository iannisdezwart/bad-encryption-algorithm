/*
	Algorithm description:

	Encryption:
		1) Substitute each byte from the message using the Reijndael S-box
		2) XOR message with key

	Decryption:
		1) XOR message with key
		2) Substitute each byte from the message using the inverse Reijndael S-box

	Why is it bad?
		- It maps byte to byte, therefore there is no diffusion
		- If the key is smaller than 16 bytes, the remaining bytes
			will be only be substituted, since SUB XOR 0 = SUB
		- If we have a sample of the original and its encrypted form,
			we can calculate the key, since SUB XOR ENCR = KEY
		- Small changes in the input will result in small changes in the output
			(Avalanche effect is very low)

	Why is it better than the previous version?
		- There is a source of confusion, namely the Reijndael S-box
		- Edge cases like having the same message and key no longer result
			in predictable outcomes
*/

#include <iostream>
#include <stdint.h>
#include "reijndael_s_box.hpp"
using namespace std;

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

// This function is the placeholder for the encryption procedure

void encrypt_message_with_key(uint8_t *message, uint8_t *key)
{
	// 1) Substitute each byte using the s-box

	for (int i = 0; i < 16; i++) {
		message[i] = substitute_byte(message[i], s_box);
	}

	// 2) XOR the i-th byte of the message with the i-th byte of the key

	for (int i = 0; i < 16; i++) {
		message[i] ^= key[i];
	}
}

// This function is the placeholder for the decryption procedure

void decrypt_message_with_key(uint8_t *message, uint8_t *key)
{
	// 1) XOR the i-th byte of the message with the i-th byte of the key

	for (int i = 0; i < 16; i++) {
		message[i] ^= key[i];
	}

	// 2) Substitute each byte using the inverse s-box

	for (int i = 0; i < 16; i++) {
		message[i] = substitute_byte(message[i], inv_s_box);
	}
}