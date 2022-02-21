#ifndef EEPROM_H
#define EEPROM_H
#include <stdint.h>
#define HASHLOC 0 //Location of the user hash in memory
#define KEYLOC 32 //Location of the user key in memory
#define IVLOC 64 //Location of the user init vector in memory

uint8_t init_eeprom(void);

uint8_t write_byte(const uint16_t addr, const uint8_t c);

uint8_t read_byte(const uint16_t addr, uint8_t *c);

uint8_t store_hash(const uint8_t hashSize, const uint8_t hash[hashSize]);

uint8_t compare_hash(const uint8_t hashSize, const uint8_t hash[hashSize]);

uint8_t get_hash(const uint8_t hashSize, uint8_t hash[hashSize]);

uint8_t store_key(const uint8_t keySize, const uint8_t key[keySize], const uint8_t ivSize, const uint8_t iv[ivSize]);

uint8_t retrieve_key(const uint8_t keySize, uint8_t key[keySize], const uint8_t ivSize, uint8_t iv[ivSize]);


#endif