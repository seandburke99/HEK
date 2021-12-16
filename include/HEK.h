#ifndef HEK_H
#define HEK_H
#include <stdint.h>
#include <uart.h>
#define ECB 0
#define CTR 0

/*
 * Able to be changed later but to start, the aes key used to encrypt each file will be made more
 * or less unique by being the first 16 bytes of a given file. If a file is less than 16 bytes or
 * not a multiple of 16, it is padded with 0 characters (0d48). This should be done driver side
 * 
*/

void HEK_init(void);
uint8_t generate_rsa_key(uint8_t *k);
uint8_t get_rsa_key(uint8_t *k, uint8_t idx);
uint8_t generate_aes_key(uint8_t *k);
uint8_t encrypt_block(const char *blk, char *eblk, uint8_t *k);

#endif