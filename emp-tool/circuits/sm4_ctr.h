#ifndef EMP_SM4_CTR_HPP__
#define EMP_SM4_CTR_HPP__

#include "emp-tool/execution/circuit_execution.h"
#include "emp-tool/execution/protocol_execution.h"
#include "emp-tool/utils/block.h"
#include "emp-tool/circuits/bit.h"
#include "emp-tool/circuits/integer.h"
#include "emp-tool/circuits/circuit_file.h"
#include <stdio.h>
#include <fstream>

#include <openssl/evp.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

namespace emp {

// Calculate the SM4_CTR encryption of some bytes
// (length refers to the number of elements of type T in the array "input").
template<typename T>
int sm4_ctr(const __m128i key,
		const __m128i iv,
		T * input, // if this is null, we'll just do a blind of length length
		uint8_t * output = nullptr, // if this is null, we'll encrypt in place
		const size_t length = 1,
		const uint64_t start_chunk = 0) {
	const size_t num_bytes = (input == nullptr) ? length : (length * sizeof(T));
	__m128i counter = iv;
	if (start_chunk != 0) { // increment iv, but it's big-endian, so it's funky
		uint64_t count;
		for(size_t i = 0; i < 8; ++i) {
			((uint8_t *)(&count))[i] = ((uint8_t *)(&counter))[15 - i];
		}
		count += start_chunk;
		for(size_t i = 0; i < 8; ++i) {
			((uint8_t *)(&counter))[15 - i] = ((uint8_t *)(&count))[i];
		}
	}
	if (input == nullptr && output == nullptr) {
		std::cerr << "input and output of sm4_ctr can't both be null pointers\n"<<std::flush;
	}
	if (output == nullptr) { // then we're encrypting in place
		output = (uint8_t *) input;
	}
	if (input == nullptr) { // then we're just doing a blind
		input = output;
		for (size_t i = 0; i < num_bytes; ++i) {
			output[i] = 0;
		}
	}
	EVP_CIPHER_CTX *ctx;
	int len;
	// Create and initialise the context 
	if(!(ctx = EVP_CIPHER_CTX_new())) {
		std::cerr<< "EVP_CIPHER_CTX_new gave me a null pointer\n"<<std::flush;
		return -1;
	}
	// Initialise the encryption operation. IMPORTANT - ensure you use a key
	// and IV size appropriate for your cipher
	if(1 != EVP_EncryptInit_ex(ctx, EVP_sm4_ctr(), NULL,
				(const unsigned char *) (&key), (const unsigned char *) (&counter))) {
		std::cerr<< "EVP_EncryptInit_ex gave something besides 1\n"<<std::flush;
		EVP_CIPHER_CTX_free(ctx);
		return -2;
	}
	// Provide the message to be encrypted, and obtain the encrypted output.
	// EVP_EncryptUpdate can be called multiple times if necessary
	if(1 != EVP_EncryptUpdate(ctx, (unsigned char *) output, &len, (unsigned char *) input, num_bytes)) {
		std::cerr<< "EVP_EncryptUpdate gave something besides 1\n"<<std::flush;
		EVP_CIPHER_CTX_free(ctx);
		return -3;
	}
	size_t ciphertext_len = len;
	// Finalise the encryption. Further ciphertext bytes may be written at
	// this stage.
	if(1 != EVP_EncryptFinal_ex(ctx, ((unsigned char *) output) + len, &len)) {
		std::cerr<< "EVP_EncryptFinal gave something besides 1\n"<<std::flush;
		EVP_CIPHER_CTX_free(ctx);
		return -4;
	}
	ciphertext_len += len;
	EVP_CIPHER_CTX_free(ctx);
	if (ciphertext_len != num_bytes) {
		std::cerr << "ciphertext length did not end up being the correct number of bytes\n" <<std::flush;
	}
	return 0;
}
}
#endif
