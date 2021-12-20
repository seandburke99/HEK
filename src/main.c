#include <HEK.h>
#include <avr/io.h>
#include <stdio.h>
#include <string.h>

int main(void){
    uint8_t cmdbuf[25];
    uint8_t cmd;
    HEK_init();
    uart_handshake();
    while(1){
        fscanf(tstream, "%s", cmd);
        switch(parse_cmd(cmdbuf)){
            
        }
    }
    return 0;
}