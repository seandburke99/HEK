#include <eeprom.h>
#include <avr/io.h>
#ifndef BUILD
    #include <avr/iom328p.h>
#endif

uint8_t init_eeprom(void){
    EECR &= ~(1<<EEPM0);
    EECR &= ~(1<<EERIE);
    EECR &= ~(1<<EEPM1);
    return 0;
}

uint8_t write_byte(const uint16_t addr, const uint8_t c){
    while(EECR & (1<<EEPE));
    EEARH = addr & 0x01;
    EEARL = addr & 0xFF;
    EEDR = c;
    EECR |= (1<<EEMPE);
    EECR |= (1<<EEPE);
    return 0;
}

uint8_t read_byte(const uint16_t addr, uint8_t *c){
    while(EECR & (1<<EEPE));
    EEARH = addr & 0x01;
    EEARL = addr & 0xFF;
    EECR |= (1<<EERE);
    *c = EEDR;
    return 0;
}

uint8_t store_hash(const uint8_t hashSize, const uint8_t hash[hashSize]){
    for(uint16_t i=0;i<hashSize;i++){
        if(write_byte(HASHLOC + i, hash[i])){
            return 1;
        }
    }
    return 0;
}

uint8_t compare_hash(const uint8_t hashSize, const uint8_t hash[hashSize]){
    uint8_t b=0, stat = 0;
    for(uint16_t i=0;i<hashSize;i++){
        if(read_byte(HASHLOC + i, &b)){
            return 255;
        }
        if(b != hash[i]){
            stat++;
        }
    }
    return stat;
}

uint8_t get_hash(const uint8_t hashSize, uint8_t hash[hashSize]){
    for(uint16_t i=0;i<hashSize;i++){
        if(read_byte(HASHLOC + i, &hash[i])){
            return 1;
        }
    }
}

uint8_t store_key(const uint8_t keySize, const uint8_t key[keySize], const uint8_t ivSize, const uint8_t iv[ivSize]){
    for(uint16_t i=0;i<keySize;i++){
        if(write_byte(KEYLOC + i, key[i])){
            return 1;
        }
        if(i<ivSize){
            if(write_byte(IVLOC + i, iv[i])){
                return 2;
            }
        }
    }
    return 0;
}

uint8_t retrieve_key(const uint8_t keySize, uint8_t key[keySize], const uint8_t ivSize, uint8_t iv[ivSize]){
    for(int i=0;i<keySize;i++){
        if(read_byte(KEYLOC + i, &key[i])){
            return 1;
        }
        if(i<ivSize){
            if(read_byte(IVLOC + i, &iv[i])){
                return 2;
            }
        }
    }
    return 0;
}