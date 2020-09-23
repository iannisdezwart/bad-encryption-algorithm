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

// This function is the placeholder for the encryption procedure

void encrypt_message_with_key(uint8_t *message, uint8_t *key)
{
	// 1) XOR the i-th byte of the message with the i-th byte of the key

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
}