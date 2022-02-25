#include <HEK.h>
#include <aes.h>
#include <avr/io.h>
#include <string.h>
#ifndef BUILD
	#include <avr/iom328p.h>
#endif
#include <avr/eeprom.h>

uint8_t unlocked = 0;

void HEK_init(void){
	init_uart(115200);

	//enable interrupts
	ADMUX |= (1<<REFS0);
	ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)|(1<<ADEN);
}

uint8_t new_user_key(void){
	uint8_t hash[HASHLEN], k[KEYLEN], iv[BLOCKLEN];
	send_char(HASH);
	recv_block(&hash[0]);
	recv_block(&hash[16]);
	eeprom_write_block((void*)hash, (void*)HASHLOC, HASHLEN);
	if(generate_aes_ctx(k, iv)){
		send_char(FAIL);
		return 1;
	}
	eeprom_write_block((void*)k, (void*)KEYLOC, KEYLEN);
	eeprom_write_block((void*)iv, (void*)IVLOC, BLOCKLEN);
	unlocked = 1;
	send_char(UNLOCKED);
	return 0;
}

uint8_t lock_key(void){
	unlocked = 0;
	send_char(LOCKED);
	return 0;
}

uint8_t unlock_key(void){
	uint8_t recvHash[HASHLEN], usrHash[HASHLEN];
	send_char(HASH);
	recv_block(&recvHash[0]);
	recv_block(&recvHash[16]);
	uint8_t diff = 0;
	eeprom_read_block((void*)usrHash, (void*)HASHLOC, HASHLEN);
	if(diff){
		send_char(FAIL);
		return 1;
	}else{
		send_char(UNLOCKED);
		unlocked = 1;
	}
	return 0;
}

uint8_t get_user_key(uint8_t k[KEYLEN], uint8_t iv[BLOCKLEN]){
	eeprom_read_block((void*)k, (void*)KEYLOC, KEYLEN);
	eeprom_read_block((void*)iv, (void*)IVLOC, BLOCKLEN);
	return 0;
}

uint8_t generate_aes_ctx(uint8_t k[KEYLEN], uint8_t iv[BLOCKLEN]){
	for(int i=0;i<KEYLEN;i++){
		k[i] = read_adc(i%8) & 0xFF; //Reading off unconnected ADC gives near true random #'s
	}
	for(int i=0;i<BLOCKLEN;i++){
		iv[i] = read_adc(i%8) & 0xFF; //Reading off unconnected ADC gives near true random #'s
	}
	return 0;
}

uint16_t read_adc(uint8_t channel){
	//select ADC channel with safety mask
	ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);
	//single conversion mode
	ADCSRA |= (1<<ADSC);
	// wait until ADC conversion is complete
	while( ADCSRA & (1<<ADSC) );
	return ADC;
}

uint8_t encrypt_file(void){
	union BUF2SIZE cnv;
	send_char(SIZE);
	for(int i=0;i<8;i++){
		recv_char(&cnv.buffer[i]);
	}
	uint8_t key[KEYLEN], blk[BLOCKLEN];
	struct AES_ctx usrCtx, fileCtx;
	get_user_key(key, blk);
	AES_init_ctx_iv(&usrCtx, key, blk);
	generate_aes_ctx(key, blk);
	AES_init_ctx_iv(&fileCtx, key, blk);
	AES_CBC_encrypt_buffer(&usrCtx, key, KEYLEN);
	AES_CBC_encrypt_buffer(&usrCtx, blk, BLOCKLEN);
	send_block(key);
	send_block(&key[16]);
	send_block(blk);
	while(cnv.size>0){
		if(!recv_block(blk)){
			AES_CBC_encrypt_buffer(&fileCtx, blk, BLOCKLEN);
			send_block(blk);
			cnv.size -= 16;
		}
	}
	return 0;
}

uint8_t decrypt_file(void){
	union BUF2SIZE cnv;
	send_char(SIZE);
	for(int i=0;i<8;i++){
		recv_char(&cnv.buffer[i]);
	}
	struct AES_ctx usrCtx, fileCtx;
	uint8_t key[KEYLEN], blk[BLOCKLEN];
	get_user_key(key, blk);
	AES_init_ctx_iv(&usrCtx, key, blk);
	send_char(KEY);
	recv_block(key);
	recv_block(&key[16]);
	recv_block(blk);
	AES_CBC_decrypt_buffer(&usrCtx, key, KEYLEN);
	AES_CBC_decrypt_buffer(&usrCtx, blk, BLOCKLEN);
	AES_init_ctx_iv(&fileCtx, key, blk);
	send_char(GOOD);
	while(cnv.size>0){
		if(!recv_block(blk)){
			AES_CBC_decrypt_buffer(&fileCtx, blk, BLOCKLEN);
			send_block(blk);
			cnv.size -= 16;
		}
	}
	return 0;
}