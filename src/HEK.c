#include <HEK.h>
#include <aes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
// #include <avr/iom328p.h>

void HEK_init(void){
    init_uart(115200);
    //enable interrupts
    DDRD |= (1<<PIND7)|(1<<PIND6);
    sei();
}

uint8_t generate_rsa_key(uint8_t *k){
    return 0;
}

uint8_t get_rsa_key(uint8_t *k, uint8_t idx){
    return 0;
}

uint8_t generate_aes_key(uint8_t *k){
    return 0;
}

uint8_t encrypt_file_block(const char *blk, char *eblk, uint8_t *k){
    return 0;
}