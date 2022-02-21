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

	while(1){
		if(recv_char(&c)){
			c = 0;
			continue;
		}
		switch(c){
			case NEWUSERKEY:
				new_user_key();
				break;
			case UNLOCKED:
				unlock_key();
				break;
			case LOCKED:
				lock_key();
				break;
			case ENCRYPT:
				if(unlocked)
					encrypt_file();
				else
					send_char(FAIL);
				break;
			case DECRYPT:
				if(unlocked)
					decrypt_file();
				else
					send_char(FAIL);
				break;
		}
	}
	return 0;
}