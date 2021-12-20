#ifndef UART_H
#define UART_H
#include <stdint.h>
#define F_CPU 16000000
#define MAX_LINE 50

/*
 * Program UART will use 8 databits with single stop bit, single parity bit. This should give reasonable communication in most circumstance especially
 * when connection is converted to straight USB from current default mini-USB on board.
 * This is important to remember when making the driver. Official driver will be python for ease of dev of GUI.
 */

uint8_t init_uart(uint32_t baud);
void uart_handshake(void);
uint8_t send_block(const uint8_t *blk);
uint8_t send_char(const uint8_t c);
uint8_t recv_block(uint8_t *blk);
uint8_t recv_char(uint8_t *c);
uint8_t compute_crc8(const uint8_t *ln, uint16_t len);
#endif