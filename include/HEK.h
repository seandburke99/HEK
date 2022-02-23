#ifndef HEK_H
#define HEK_H
#include <stdint.h>
#include <uart.h>
#define ECB 0
#define CTR 0
#define KEYLEN (uint8_t)32
#define BLOCKLEN (uint8_t)16
#define HASHLEN (uint8_t)32

extern uint8_t unlocked;

/**
 * @brief Enumeration for the different commands performable
 * 
 */
typedef enum HEKCMD{
    ENCRYPT = 'e',
    DECRYPT = 'd',
    NEWUSERKEY = 'n',
    UNLOCKED = 'u',
    LOCKED = 'l',
    FAIL = 'f',
    SIZE = 's',
    KEY = 'k',
    HASH = 'h',
    GOOD = 'g'
}HEKCMD;

/**
 * @brief Union to convert 8 byte size into 64 bit unsigned integer
 * 
 */
union BUF2SIZE{
    uint64_t size;
    uint8_t buffer[2];
};

/**
 * @brief Initialization for the system
 * 
 */
void HEK_init(void);

uint8_t new_user_key(void);

uint8_t lock_key(void);

uint8_t unlock_key(void);

/**
 * @brief Generate an aes context for the aes library
 * 
 * @param k Pointer to the key location
 * @param iv Pointer to the initialization vector location
 * @return uint8_t Boolean indication of function success
 */
uint8_t generate_aes_ctx(uint8_t k[KEYLEN], uint8_t iv[BLOCKLEN]);

/**
 * @brief Function that follows the file encryption process
 * 
 * @return uint8_t Boolean indication of function success
 */
uint8_t encrypt_file(void);

/**
 * @brief Function that follows the file decryption process
 * 
 * @return uint8_t Boolean indication of function success
 */
uint8_t decrypt_file(void);

/**
 * @brief Read from the floating analog to digital converter. Attach an antenna to this pin to further randomize.
 * 
 * @param channel ADC channel to read from
 * @return uint16_t Value read 0-1023
 */
uint16_t read_adc(uint8_t channel);

#endif