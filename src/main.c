#include <HEK.h>
#include <avr/io.h>
#include <string.h>

int main(void){
    uint8_t c;
    HEK_init();
    uart_handshake();
    while(1){
        if(recv_char(&c)){
            continue;
        }
        switch(c){
            case NEWRSAKEY:
                new_rsa_key();
                break;
            case 'k':
                use_rsa_key();
                break;
            case 'e':
                encrypt_file();
                break;
            case 'd':
                decrypt_file();
                break;
        }
    }
    return 0;
}