#include <uart.h>
#include <avr/io.h>
#include <avr/interrupt.h>
// #include <avr/iom328p.h>

#define UCSR0B_INIT 0b00011000 //Sets up UART without receive interrupt
#define UCSR0C_INIT 0b00001110

static FILE tbuf = FDEV_SETUP_STREAM(send_char, recv_char, _FDEV_SETUP_RW);

FILE *init_uart(uint32_t baud){
    // Set Baud Rate (UBRR)
    // UBRR = (F_CPU / (16*BAUD)) - 1
    // uint16_t baudReg = (F_CPU / (16*baud)) - 1; // Asynchronus baud rate register calculation regular speed
    UBRR0L = 16;
    UBRR0H = 0;
    //Set USCR0(ABC)
    UCSR0A = (1<<U2X0);
    UCSR0B = UCSR0B_INIT;
    UCSR0C = UCSR0C_INIT;
    return &tbuf;
}
//Transmit buffer (8 bit register) = UDR0

void uart_handshake(void){
    char c;
    const char cc = '2';
    while(1){
        fscanf(&tbuf, "%c", &c);
        if(c=='2'){
            break;
        }
        fprintf(&tbuf, "%c", cc);
    }
}

int send_char(const char c, FILE *stream){
    while(!(UCSR0A & (1<<UDRE0))); //Wait for communications buffer to be empty
    UDR0 = c; //Place new character into buffer
    return 0;
}

int recv_char(FILE *stream){
    while(!(UCSR0A & (1<<RXC0)));
    if(!(UCSR0A & ((1<<DOR0) | (1<<FE0)))){
        return UDR0;
    }
    else{
        return 0;
    }
}

uint8_t compute_crc8(const char *ln){
    uint8_t crc = ln[0];
    for(int i=1;ln[i]!=0;i++){
        crc ^= ln[i];
    }
    return crc;
}