#include <HEK.h>
#include <avr/io.h>
#include <stdio.h>

char rbuf[4*32+1];

int main(void){
    uint8_t newkey[32];
    HEK_init();
    uart_handshake();
    while(1){
        generate_aes_key(newkey);
        for(int i=0;i<32;i++){
            sprintf(&rbuf[i*4], "%03d ", newkey[i]);
        }
        sprintf(&rbuf[4*32], "\n");
        send_line(rbuf);
    }
    return 0;
}