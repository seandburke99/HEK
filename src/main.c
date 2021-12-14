#include <HEK.h>
#include <avr/io.h>

char rbuf[20];
const char *err = "Error in receiving\n";
const char *good = "Received\n";

int main(void){
    HEK_init();
    handshake_uart();
    while(1){
        if(recv_line(rbuf, 20)){
            send_line(err);
        }else{
            send_line(good);
        }
    }
    return 0;
}