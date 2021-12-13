#ifndef HEK_H
#define HEK_H
#include <aes.h>
#include <rsa.h>
#include <uart.h>

/*
 * Able to be changed later but to start, the aes key used to encrypt each file will be made more
 * or less unique by being the first 32 bytes of a given file. If a file is less than 32 bytes or
 * not a multiple of 32, it is padded with 0's
 *
*/

void HEK_init(void);
uint8_t encrypt_file_block(const char *blk, char *eblk);

#endif