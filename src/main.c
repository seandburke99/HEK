#include <HEK.h>
#include <avr/io.h>
#include <string.h>

int main(void){
    uint8_t key[33];
    uint8_t iv[17];
    key[32] = 0;
    iv[16] = 0;
    char crc;
    HEK_init();
    uart_handshake();
    while(1){
        generate_aes_ctx(key, iv);
        send_block(key);
        send_block(&key[16]);
        crc = compute_crc8(key);
        send_char(crc);
    }
    return 0;
}