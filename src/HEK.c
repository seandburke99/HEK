#include <HEK.h>
#include <avr/io.h>

void HEK_init(void){
    init_uart(115200);
    //enable interrupts
    // sei();
}

uint8_t encrypt_file_block(const char *blk, char *eblk){
    
    return 0;
}