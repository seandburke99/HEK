#include <uart.h>
#include <avr/io.h>
// #include <avr/iom328p.h>

#define UCSR0B_INIT 0b00011000
#define UCSR0C_INIT 0b00110110

uint8_t init_uart(uint32_t baud){
    // Set Baud Rate (UBRR)
    // UBRR = (F_CPU / (16*BAUD)) - 1
    // uint16_t baudReg = (F_CPU / (16*baud)) - 1; // Asynchronus baud rate register calculation regular speed
    UBRR0L = 8;
    UBRR0H = 0;
    //Set USCR0(ABC)
    UCSR0B = UCSR0B_INIT;
    UCSR0C = UCSR0C_INIT;
    return 0;
}
//Transmit buffer (8 bit register) = UDR0

uint8_t send_line(const char *ln){
    for(int i=0;ln[i]!=0;i++){
        send_char(&ln[i]);
    }
    return 0;
}

uint8_t send_char(const char *c){
    while(!(UCSR0A & (1<<UDRE0))); //Wait for communications buffer to be empty
    UDR0 = *c; //Place new character into buffer
    return 0;
}

uint8_t recv_line(char *ln){
    for(int i=0;ln[i]!='\n';i++){
        if(recv_char(&ln[i])){
            return 1;
        }
    }
    return 0;
}

uint8_t recv_char(char *c){
    while(!(UCSR0A & (1<<RXC0))){
        if(UCSR0A & ((1<<FE0) | (1<<DOR0) | (1<UPE0))){
            *c = '\n';
            return 1;
        }
    }
    *c = UDR0;
    return 0;
}

uint8_t compute_crc8(const char *ln){
    uint8_t crc = ln[0];
    for(int i=1;ln[i]!='\n';i++){
        crc ^= ln[i];
    }
    return crc;
}