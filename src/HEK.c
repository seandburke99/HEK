#include <HEK.h>
#include <avr/io.h>

void HEK_init(void){
    init_uart(115200);
    //enable interrupts
    // sei();
}