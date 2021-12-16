#include <HEK.h>
#include <avr/io.h>

char rbuf[50];
const char *err = "Error in receiving\n";
const char *good = "Received\n";

int main(void){
    char c;
    HEK_init();
    uart_handshake();
    while(1){
        if(!recv_line_async(rbuf, 50)){
            send_line(rbuf);
        }
    }
    return 0;
}