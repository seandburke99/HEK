#include <uart.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
// #include <avr/iom328p.h>

#define UCSR0B_INIT 0b10011000
#define UCSR0C_INIT 0b00001110

static uint8_t rbufferIdx = 0;
static uint8_t rFlags = 0;
static char rbuffer[100] = {0};

ISR(USART_RX_vect){
    if(UCSR0A & ((1<<DOR0) | (1<<FE0))){
        char c = UDR0;
    }else{
        rbuffer[rbufferIdx++] = UDR0;
        rFlags |= (1<<NEW_CHAR);
        if(rbuffer[rbufferIdx-1]=='\n'){
            rFlags |= (1<<NEW_LINE);
        }
        rbuffer[rbufferIdx] = 0;
    }
}

uint8_t init_uart(uint32_t baud){
    // Set Baud Rate (UBRR)
    // UBRR = (F_CPU / (16*BAUD)) - 1
    // uint16_t baudReg = (F_CPU / (16*baud)) - 1; // Asynchronus baud rate register calculation regular speed
    UBRR0L = 25;
    UBRR0H = 0;
    //Set USCR0(ABC)
    UCSR0A = (1<<U2X0);
    UCSR0B = UCSR0B_INIT;
    UCSR0C = UCSR0C_INIT;
    return 0;
}
//Transmit buffer (8 bit register) = UDR0

void uart_handshake(void){
    char c = 0;
    while(recv_char_async(&c));
    send_char(c);
}

uint8_t send_line(const char *ln){
    for(int i=0;ln[i]!=0;i++){
        send_char(ln[i]);
    }
    return 0;
}

uint8_t send_char(char c){
    while(!(UCSR0A & (1<<UDRE0))); //Wait for communications buffer to be empty
    UDR0 = c; //Place new character into buffer
    return 0;
}

uint8_t recv_line_async(char *ln, uint8_t n){
    int i, am;
    if(rFlags & (1<<NEW_LINE)){
        if(n>rbufferIdx){
            am = rbufferIdx;
        }else{
            am = n;
        }
        rbufferIdx = 0;
        for(i=0;i<am;i++){
            ln[i] = rbuffer[i];
            rbuffer[i] = 0;
        }
        rFlags &= ~(1<<NEW_LINE);
    }else{
        return 1;
    }
    return 0;
}

uint8_t recv_char_async(char *c){
    if(rbufferIdx){
        *c = rbuffer[0];
    }else{
        return 1;
    }
    return 0;
}

uint8_t compute_crc8(const char *ln){
    uint8_t crc = ln[0];
    for(int i=1;ln[i]!='\n';i++){
        crc ^= ln[i];
    }
    return crc;
}