#include <HEK.h>
#include <avr/io.h>

char rbuf[20];
const char *send = "Received\n";

int main(void){

    HEK_init();
    int i=0;
    while(1){
        if(!recv_char(&rbuf[0])){
            PORTD |= (1<<PIND7);
            send_char(&rbuf[0]);
        }else{
            PORTD &= ~(1<<PIND7);
        }
    }
    return 0;
}