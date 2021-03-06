#include <uart.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#ifndef BUILD
#include <avr/iom328p.h>
#endif

#define BLOCKLEN 16
#define UCSR0B_INIT 0b00011000 //Sets up UART without receive interrupt
#define UCSR0C_INIT 0b00001110

uint8_t init_uart(uint32_t baud){
    // Set Baud Rate (UBRR)
    // UBRR = (F_CPU / (16*BAUD)) - 1
    // uint16_t baudReg = (F_CPU / (16*baud)) - 1; // Asynchronus baud rate register calculation regular speed
    UBRR0L = 16;
    UBRR0H = 0;
    //Set USCR0(ABC)
    UCSR0A = (1<<U2X0);
    UCSR0B = UCSR0B_INIT;
    UCSR0C = UCSR0C_INIT;
    return 0;
}
//Transmit buffer (8 bit register) = UDR0

void uart_handshake(void){
    char c;
    while(1){
        recv_char(&c);
        if(c=='2'){
            break;
        }
        send_char('2');
    }
}

uint8_t send_char(const uint8_t c){
    while(!(UCSR0A & (1<<UDRE0))); //Wait for communications buffer to be empty
    UDR0 = c; //Place new character into buffer
    return 0;
}

uint8_t send_block(const uint8_t *blk){
    for(int i=0;i<BLOCKLEN;i++){
        send_char(blk[i]);
    }
    return 0;
}

uint8_t send_line(const uint8_t *ln){
    for(int i=0;ln[i]!='\n';i++){
        send_char(ln[i]);
    }
    send_char('\n');
    return 0;
}

uint8_t recv_char(uint8_t *c){
    while(!(UCSR0A & (1<<RXC0)));
    if(!(UCSR0A & ((1<<DOR0) | (1<<FE0)))){
        *c = UDR0;
    }
    else{
        return 1;
    }
    return 0;
}

uint8_t recv_block(uint8_t *blk){
    for(int i=0;i<BLOCKLEN;i++){
        if(recv_char(&blk[i])){
            return 1;
        }
    }
    return 0;
}

uint8_t recv_line(uint8_t *ln){
    uint16_t i = 0;
    do{
        if(recv_char(&ln[i])){
            return 1;
        }
    }while(ln[i++]!='\n');
    return 0;
}

uint8_t compute_crc8(const uint8_t *ln, uint16_t len){
    uint8_t crc = 0;
    const uint8_t *data = ln;
    while(len--){
        crc ^= *data++;
    }
    return crc;
}