#ifndef HEK_H
#define HEK_H
#include <stdint.h>
#include <uart.h>
#define ECB 0
#define CTR 0

/**
 * @brief Enumeration for the different commands performable
 * 
 */
typedef enum HEKCMD{
    ENCRYPT = 'e',
    DECRYPT = 'd',
    NEWRSAKEY = 'n',
    USERSAKEY = 'k'
}HEKCMD;

/**
 * @brief Initialization for the system
 * 
 */
void HEK_init(void);

/**
 * @brief Function to parse a command sent in
 * 
 * @param msg Block of characters to parse the command from
 * @return uint8_t Boolean indication of function success
 */
uint8_t parse_cmd(char msg[]);

/**
 * @brief Generate an RSA key
 * 
 * @param k Location to store the generated key
 * @return uint8_t Boolean indication of function success
 */
uint8_t generate_rsa_key(uint8_t *k);

/**
 * @brief Get the rsa key object from memory
 * 
 * @param k Location to store the retrieved key
 * @param idx Index of the key to retrieve
 * @return uint8_t Boolean indication of function success
 */
uint8_t get_rsa_key(uint8_t *k, uint8_t idx);

/**
 * @brief Generate an aes context for the aes library
 * 
 * @param k Pointer to the key location
 * @param iv Pointer to the initialization vector location
 * @return uint8_t Boolean indication of function success
 */
uint8_t generate_aes_ctx(uint8_t *k, uint8_t *iv);

/**
 * @brief Encrypt a files asymmetric data
 * 
 * @param rsakID ID of the rsa key to use
 * @param k AES key
 * @param iv AES initialization vector
 * @return uint8_t Boolean indication of function success
 */
uint8_t encrypt_key_iv(uint8_t rsakID, uint8_t *k, uint8_t *iv);

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