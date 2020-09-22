#include <iostream>
using namespace std;

// Globals

extern uint8_t message[16];
extern uint8_t key[16];

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