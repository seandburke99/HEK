#include <HEK.h>
#include <avr/io.h>
#include <aes.h>

uint8_t HEK_flags = 0;

void HEK_init(void){
    init_uart(115200);
    //enable interrupts
    // sei();
}

void HEK_set_parse();

uint8_t encrypt_file_block(const char *blk, char *eblk){
    return 0;
}