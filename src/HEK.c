#include <HEK.h>
#include <aes.h>
#include <avr/io.h>
// #include <avr/iom328p.h>

void HEK_init(void){
    init_uart(115200);
    //enable interrupts
    DDRD |= (1<<PIND7)|(1<<PIND6);
}

void HEK_set_parse();

uint8_t encrypt_file_block(const char *blk, char *eblk){
    return 0;
}