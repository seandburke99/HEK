#include <HEK.h>
#include <aes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
// #include <avr/iom328p.h>

void HEK_init(void){
    tstream = init_uart(115200);
    //enable interrupts
    DDRD |= (1<<PIND7)|(1<<PIND6);
    ADMUX |= (1<<REFS0);
    ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)|(1<<ADEN);
    sei();
}

uint8_t parse_cmd(char msg[]){
    return 0;
}

uint8_t generate_rsa_key(uint8_t *k){
    return 0;
}

uint8_t get_rsa_key(uint8_t *k, uint8_t idx){
    return 0;
}

uint8_t generate_aes_key(uint8_t *k){
    for(int i=0;i<32;i++){
        k[i] = read_adc(i%8) % 255;
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

uint8_t encrypt_file_block(const char *blk, char *eblk, uint8_t *k){
    return 0;
}