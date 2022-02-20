#include <HEK.h>
#include <aes.h>
#include <avr/io.h>
#include <string.h>
#ifndef BUILD
#include <avr/iom328p.h>
#endif

void HEK_init(void){
    init_uart(115200);
    //enable interrupts
    DDRD |= (1<<PIND7)|(1<<PIND6);
    ADMUX |= (1<<REFS0);
    ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)|(1<<ADEN);
}

uint8_t parse_cmd(char msg[]){

    return 0;
}

uint8_t generate_rsa_key(uint8_t *k){
    uint8_t rsakey[32];
    uint8_t rsap[16], rsaq[16];
    for(int i=0;i<16;i++){
        rsap[i] = read_adc(i%8) & 0xFF;
        rsaq[i] = read_adc(i%8) & 0xFF;
    }
    rsap[0]++;
    rsaq[0]++;
    rsap[15] |= 0x80;
    rsaq[15] |= 0x80;
    return 0;
}

uint8_t get_rsa_key(uint8_t *k, uint8_t idx){
    return 0;
}

uint8_t generate_aes_ctx(uint8_t *k, uint8_t *iv){
    for(int i=0;i<AES_KEYLEN;i++){
        k[i] = read_adc(i%8) & 0xFF; //Reading off unconnected ADC gives near true random #'s
    }
    for(int i=0;i<AES_BLOCKLEN;i++){
        iv[i] = read_adc(i%8) & 0xFF; //Reading off unconnected ADC gives near true random #'s
    }
    return 0;
}

uint16_t read_adc(uint8_t channel){
    //select ADC channel with safety mask
    ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);
    //single conversion mode
    ADCSRA |= (1<<ADSC);
    // wait until ADC conversion is complete
    while( ADCSRA & (1<<ADSC) );
    return ADC;
}

uint8_t encrypt_key_iv(uint8_t rsakID, uint8_t *k, uint8_t *iv){
    uint8_t rsaK[32];
    get_rsa_key(rsaK, rsakID);
    return 0;
}

uint8_t encrypt_file(void){
    send_char('s');
    union BUF2SIZE cnv;
    for(int i=0;i<8;i++){
        recv_char(&cnv.buffer[i]);
    }
    uint8_t key[AES_KEYLEN], blk[AES_BLOCKLEN];
    generate_aes_ctx(key, blk);
    struct AES_ctx ectx;
    AES_init_ctx_iv(&ectx, key, blk);
    send_block(key);
    send_block(&key[16]);
    send_block(blk);
    while(cnv.size>0){
        PORTD |= (1<<PIND6);
        if(!recv_block(blk)){
            AES_CBC_encrypt_buffer(&ectx, blk, AES_BLOCKLEN);
            send_block(blk);
            cnv.size -= 16;
        }
    }
    PORTD &= ~(1<<PIND6);
    return 0;
}

uint8_t decrypt_file(void){
    send_char('s');
    uint64_t sz = 0;
    uint8_t pc;
    for(int i=7;i>-1;i--){
        recv_char(&pc);
        sz |= (i*8<<pc);
    }
    uint8_t key[AES_KEYLEN], blk[AES_BLOCKLEN];
    send_char('k');
    recv_block(key);
    recv_block(&key[16]);
    recv_block(blk);
    struct AES_ctx ectx;
    AES_init_ctx_iv(&ectx, key, blk);
    send_char('g');
    while(sz>0){
        if(!recv_block(blk)){
            AES_CBC_decrypt_buffer(&ectx, blk, AES_BLOCKLEN);
            send_block(blk);
            sz -= 16;
        }
    }
    return 0;
}