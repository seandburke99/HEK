#ifndef MEMORY_H
#define MEMORY_H
#include <stdint.h>
#define FIRSTKEYLOC 0 //Location of the current first key location value in eeprom
#define NUMKEYLOC 2 //Location of the number of keys that are saved in the eeprom

/**
 * @brief Function to store an RSA key in the EEPROM
 * 
 * @param key The key to store in the EEPROM
 * @return uint8_t Boolean indication of function success
 */
uint8_t store_key(const uint8_t key[]);

/**
 * @brief Function to retrieve an RSA key from the EEPROM
 * 
 * @param index Index of the key to retrieve
 * @param key Location to put the key
 * @return uint8_t Boolean indication of function success
 */
uint8_t retrieve_key(uint16_t index, uint8_t *key);


#endif