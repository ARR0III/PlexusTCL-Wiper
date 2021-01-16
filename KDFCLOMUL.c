#include "src/sha256.h"
#include "src/crc32.h"
#include "src/xtalw.h"

#define CLOMUL_CONST 0x00000001U

void * KDFCLOMUL(SHA256_CTX * sha256_ctx,
              const uint8_t * password, const size_t password_len,
                    uint8_t * key,      const size_t key_len) {

  uint32_t i, j, k;
  uint32_t count = 0;
  uint8_t  hash[SHA256_BLOCK_SIZE];

  for (i = 0; i < password_len; ++i) {
    count ^= (uint32_t)(CRC32(password, i) + CLOMUL_CONST);
    count -= (password_len + key_len + CLOMUL_CONST + i);
  }

  count  &= CRC32(password, password_len);
  count >>= 18; /* MAX = 262,143 == 0x0003FFFF */
  count  |= ((uint32_t)1 << 14);
  count  *= CLOMUL_CONST;

  sha256_init(sha256_ctx);

  for (i = k = 0; i < key_len; ++i, ++k) {
    for (j = 0; j < count; ++j) {
      sha256_update(sha256_ctx, password, password_len);
    }

    sha256_final(sha256_ctx, hash);

    if (k == SHA256_BLOCK_SIZE) {
      k = 0;
    }

    key[i] = hash[k];
  }

  meminit((void *)hash, 0x00, SHA256_BLOCK_SIZE);
  return key;
}
