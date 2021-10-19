#include "emp-tool/emp-tool.h"
#include <iostream>

using namespace std;
using namespace emp;


// try hashing a fairly arbitrary byte string and see if we get the right value.
int hash_in_circuit(){

  uint8_t input[2000];
  uint8_t output_bytes[2000];
  uint8_t output_bytes2[2000];
  uint8_t decrypted_bytes[2000];
  for (size_t i = 0; i < 2000; ++i) {
    input[i] = i % 200;
  }
  __m128i key;
  __m128i iv;
  for (size_t i = 0; i < 16; ++i) {
    ((uint8_t *)(&key))[i] = (1337 * i) % 255;
    ((uint8_t *)(&iv))[i] = (31 * i) % 253;
  }


  emp::sm4_ctr(key, iv, input, output_bytes, 2000, 77777);

  // let's make sure we can decrypt this
  emp::sm4_ctr(key, iv, output_bytes, decrypted_bytes, 2000, 77777);
  for(size_t i=0; i<2000; ++i) {
    if (input[i] != decrypted_bytes[i]) {
      std::cerr << "decryption did not match input\n" << std::flush;
      return -1;
    }
  }

  std::cout << "in memory:  ";
  for (size_t i = 0; i < 32; ++i) {
    std::cout << std::setw(2) << std::setfill('0') << std::hex << (int)(output_bytes[1000 + i]) << " ";
  }
}

int main(int argc, char** argv) {
	setup_plain_prot(false, "");
	hash_in_circuit();
	finalize_plain_prot();
	return 0;
}
