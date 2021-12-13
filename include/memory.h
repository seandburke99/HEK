#ifndef MEMORY_H
#define MEMORY_H
#include <stdint.h>
#define FIRSTKEYLOC 0 //Location of the current first key location value in eeprom
#define NUMKEYLOC 2 //Location of the number of keys that are saved in the eeprom

uint8_t store_key(const char key[]);
uint8_t retrieve_key(uint16_t index, char *key);


#endif