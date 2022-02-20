#include <HEK.h>
#include <avr/io.h>
#include <string.h>

int main(void){
	uint8_t c;
	HEK_init();
	uart_handshake();
	while(1){
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
				encrypt_file();
				break;
			case DECRYPT:
				decrypt_file();
				break;
		}
	}
	return 0;
}