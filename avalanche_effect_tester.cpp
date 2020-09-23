#include <iostream>
#include <random>
#include "5.hpp"
#include "scanning_and_printing.hpp"
#include <thread>
using namespace std;

// Globals

struct Texts {
	uint8_t first_message[16];
	uint8_t second_message[16];
	uint8_t key[16];
};

double avalanche_effect_sum = 0;

int random(int max)
{
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> distr(0, max);
	return distr(gen);
}

Texts *generate_texts()
{
	Texts *texts = new Texts;

	int altered_index = random(15);

	// Generate random plain texts and one random key

	for (int i = 0; i < 16; i++) {
		// Add a random byte to the first plain text

		texts->first_message[i] = random(255);

		// Set a byte of the second plain text

		if (i == altered_index) {
			// Alter this byte

			// texts->second_message[i] = random(255);
			texts->second_message[i] = texts->first_message[i] ^ (1 << random(7));
			// texts->second_message[i] = texts->first_message[i];
		} else {
			// Copy the byte

			texts->second_message[i] = texts->first_message[i];
		}

		texts->key[i] = random(255);
	}

	return texts;
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
	Texts *texts = generate_texts();

	// Encrypt the first plain text

	encrypt_message_with_key(texts->first_message, texts->key);

	// Encrypt the second plain text

	encrypt_message_with_key(texts->second_message, texts->key);

	double avalanche_effect = calculate_avalanche_effect(
		texts->first_message,
		texts->second_message
	);
	avalanche_effect_sum += avalanche_effect / 128;
}

const int hardware_concurrency = thread::hardware_concurrency();
const int total_rounds = 1E5;
int performed_rounds = 0;

void worker()
{
	// Check if there is work to do

	while (performed_rounds < total_rounds) {
		perform_round();
		performed_rounds++;

		if (performed_rounds % 1000 == 0) {
			printf("Performed %d rounds\n", performed_rounds);
		}
	}
}

int main()
{
	thread thread_pool[hardware_concurrency];

	// Initialise threads

	for (int i = 0; i < hardware_concurrency; i++) {
		thread_pool[i] = thread(worker);
	}

	// Wait for all threads to finish

	for (int i = 0; i < hardware_concurrency; i++) {
		thread_pool[i].join();
	}

	printf("Average avalanche effect: %lf\n", avalanche_effect_sum / total_rounds);
}