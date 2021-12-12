#include <uart.h>
#include <avr/io.h>
#include <avr/iom328p.h>

#define UCSR0B_INIT 0b00011000
#define UCSR0C_INIT 0b00001110

uint8_t init_uart(uint32_t baud){
    // Set Baud Rate (UBRR)
    // UBRR = (F_CPU / (16*BAUD)) - 1
    uint16_t baudReg = (F_CPU / (16*baud)) - 1; // Asynchronus baud rate register calculation regular speed
    UBRR0L =  0xFF & baudReg;
    UBRR0H = 0x0F00 & baudReg;
    //Set USCR0(ABC)
    UCSR0B = UCSR0B_INIT;
    UCSR0C = UCSR0C_INIT;
    return 0;
}

//Transmit buffer (8 bit register) = UDR0


uint8_t send_line(const char *ln){
    char *cc;
    for(cc = ln;*cc!=0;cc++){
        send_char(cc);
    }
    return 0;
}

uint8_t send_char(const char *c){
    while(!(UCSR0A && (1<<UDRE0)));
    UDR0 = *c;
    return 0;
}

uint8_t recv_line(char *ln){
    char *cc;
    cc = ln;
    while(1){
        recv_char(cc);
        if(!*cc) 
            break;
        else 
            cc++;
    }
    return 0;
}

uint8_t recv_char(char *c){
    while(!(UCSR0A && (1<<UDRE0)));
    *c = UDR0;
    return 0;
}