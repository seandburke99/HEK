#ifndef HEK_H
#define HEK_H
#include <stdint.h>
#include <uart.h>
#define ECB 0
#define CTR 0

typedef enum HEKCMD{
    ENCRYPT,
    NEWRSA
}HEKCMD;

void HEK_init(void);
uint8_t parse_cmd(char msg[]);
uint8_t generate_rsa_key(uint8_t *k);
uint8_t get_rsa_key(uint8_t *k, uint8_t idx);
uint8_t generate_aes_ctx(uint8_t *k, uint8_t *iv);
uint8_t encrypt_key_iv(uint8_t rsakID, uint8_t *k, uint8_t *iv);
uint8_t encrypt_file(void);
uint8_t encrypt_block(const char *blk, char *eblk, uint8_t *k);
uint16_t read_adc(uint8_t channel);

#endif