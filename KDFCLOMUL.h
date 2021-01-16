#include "KDFCLOMUL.c"

void * KDFCLOMUL(SHA256_CTX * sha256_ctx,
              const uint8_t * password, const size_t password_len,
                    uint8_t * key,      const size_t key_len);