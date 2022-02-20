#include <HEK.h>
#include <avr/io.h>
#ifndef BUILD
	#include <avr/iom328p.h>
#endif
#include <string.h>

int main(void){
	uint8_t c;
	HEK_init();
	uart_handshake();

	DDRD |= 0b11000000;

	while(1){
		PORTD |= (1<<PIND7);
		if(recv_char(&c)){
			continue;
		}
		switch(c){
			case NEWRSAKEY:
				1==1;
				break;
			case USERSAKEY:
				1==1;
				break;
			case ENCRYPT:
				PORTD &= ~(1<<PIND7);
				encrypt_file();
				break;
			case DECRYPT:
				decrypt_file();
				break;
		}
		c = 0;
	}
	return 0;
}