#include <iostream>
#include <random>
#include "5.hpp"
#include "scanning_and_printing.hpp"
using namespace std;

// Globals

uint8_t first_plain_text[16] = {};
uint8_t second_plain_text[16] = {};

double avalanche_effect_sum = 0;
const int total_rounds = 1E4;

int random(int max)
{
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> distr(0, max);
	return distr(gen);
}

void generate_texts()
{
	int altered_index = random(15);

	// Generate random plain texts and one random key

	for (int i = 0; i < 16; i++) {
		// Add a random byte to the first plain text

		first_plain_text[i] = random(255);

		// Set a byte of the second plain text

		if (i == altered_index) {
			// Alter this byte

			second_plain_text[i] = random(255);
		} else {
			// Copy the byte

			second_plain_text[i] = first_plain_text[i];
		}

		key[i] = random(255);
	}
}

void copy_message(uint8_t *src, uint8_t *dest)
{
	for (int i = 0; i < 16; i++) {
		dest[i] = src[i];
	}
}

int calculate_avalanche_effect(uint8_t *cipher_text1, uint8_t *cipher_text2)
{
	int count = 0;

	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 8; j++) {
			if ((cipher_text1[i] ^ cipher_text2[i]) & (1 << j)) count++;
		}
	}

	return count;
}

void perform_round()
{
	generate_texts();

	// Encrypt the first plain text

	copy_message(first_plain_text, message);
	encrypt_message_with_key();

	// Hold the first ciphertext

	uint8_t held_cipher_text[16];
	copy_message(message, held_cipher_text);

	// Encrypt the second plain text

	copy_message(second_plain_text, message);
	encrypt_message_with_key();

	double avalanche_effect = calculate_avalanche_effect(message, held_cipher_text);
	avalanche_effect_sum += avalanche_effect / 128;
}

int main()
{
	for (int i = 0; i < total_rounds; i++) {
		perform_round();

		if (i % 1000 == 0) {
			printf("Performed %d rounds\n", i);
		}
	}

	printf("Average avalanche effect: %lf\n", avalanche_effect_sum / total_rounds);
}