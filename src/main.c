#include <HEK.h>
#include <avr/io.h>

char rbuf[20];
const char *err = "Error in receiving\n";
const char *good = "Received\n";

int main(void){
    char c;
    HEK_init();
    uart_handshake();
    while(1){
        send_line(rbuf);
    }
    return 0;
}