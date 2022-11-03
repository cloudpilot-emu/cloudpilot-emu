#ifndef _MD5_H_
#define _MD5_H_

// https://github.com/Zunawe/md5-c

#include <cstdint>
#include <cstdlib>
#include <string>

typedef struct {
    uint64_t size;       // Size of input in bytes
    uint32_t buffer[4];  // Current accumulation of hash
    uint8_t input[64];   // Input to be used in the next step
    uint8_t digest[16];  // Result of algorithm
} MD5Context;

void md5Init(MD5Context *ctx);
void md5Update(MD5Context *ctx, uint8_t *input, size_t input_len);
void md5Finalize(MD5Context *ctx);
void md5Step(uint32_t *buffer, uint32_t *input);

std::string md5(uint8_t *data, size_t len);

#endif  // _MD5_H_
