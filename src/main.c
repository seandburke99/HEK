#include <HEK.h>
#include <avr/io.h>
#include <stdio.h>
#include <string.h>

char rbuf[4*32+1];

int main(void){
    uint8_t newkey[32];
    char c;
    HEK_init();
    // uart_handshake();
    uart_handshake_stream();
    while(1){
        if(fscanf(tstream, "%s", newkey)){
            fprintf(tstream, "%s", newkey);
        }
        // generate_aes_key(newkey);
        // for(int i=0;i<32;i++){
        //     sprintf(&rbuf[i*4], "%03d ", newkey[i]);
        // }
        // sprintf(&rbuf[4*32], "\n");
        // send_line(rbuf);
    }
    return 0;
}