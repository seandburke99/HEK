#ifndef UART_H
#define UART_H
#include <avr/io.h>
// #include <avr/iom328p.h>
#include <stdint.h>

uint8_t init_uart(uint32_t baud);
uint8_t send_line(const char *ln);
uint8_t send_char(const char *c);
uint8_t recv_line(char *ln);
uint8_t recv_char(char *c);
#endif