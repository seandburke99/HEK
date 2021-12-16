#include <memory.h>
#include <avr/io.h>

uint8_t store_key(const char key[]){
    // Wait for the ability to write
    // while(EECR & (1<<EEPE));
    //Set address in register EEAR
    
    //Set data to write EEDR

    // EECR |= (1<<EEMPE); // Enable master write
    // EECR |= (1<<EEPE); // Start write
    return 0;
}

uint8_t retrieve_key(uint16_t index, char *key){
    // Wait for ability to read
    // while(EECR & (1<<EEPE));
    //Set address in register EEAR
    
    //Set data to write EEDR

    // EECR |= (1<<EEMPE); // Enable master write
    // EECR |= (1<<EEPE); // Start write
    return 0;
}