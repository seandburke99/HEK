#include <HEK.h>
#include <aes.h>
#include <avr/io.h>
#include <string.h>
#include <eeprom.h>
#ifndef BUILD
#include <avr/iom328p.h>
#endif

uint8_t unlocked = 0;

void HEK_init(void){
	init_uart(115200);
	init_eeprom();

	//enable interrupts
	ADMUX |= (1<<REFS0);
	ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)|(1<<ADEN);
}

uint8_t new_user_key(void){
	uint8_t hash[HASHLEN], k[KEYLEN], iv[BLOCKLEN];
	send_char(HASH);
	recv_block(hash);
	recv_block(&hash[16]);
	if(store_hash(HASHLEN, hash)){
		send_char(FAIL);
		return 1;
	}
	if(generate_aes_ctx(k, iv)){
		send_char(FAIL);
		return 1;
	}
	if(store_key(KEYLEN, k, BLOCKLEN, iv)){
		send_char(FAIL);
		return 1;
	}
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
	uint8_t hash[HASHLEN];
	send_char(HASH);
	recv_block(hash);
	recv_block(&hash[16]);
	if(compare_hash(HASHLEN, hash)){
		send_char(FAIL);
		return 1;
	}
	unlocked = 1;
	send_char(UNLOCKED);
	return 0;
}

uint8_t get_user_key(uint8_t hash[HASHLEN], uint8_t k[KEYLEN], uint8_t iv[BLOCKLEN]){
	if(compare_hash(HASHLEN, hash)){
		return 1;
	}
	return retrieve_key(KEYLEN, k, BLOCKLEN, iv);
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
	generate_aes_ctx(key, blk);
	struct AES_ctx ectx;
	AES_init_ctx_iv(&ectx, key, blk);
	send_block(key);
	send_block(&key[16]);
	send_block(blk);
	while(cnv.size>0){
		if(!recv_block(blk)){
			AES_CBC_encrypt_buffer(&ectx, blk, BLOCKLEN);
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
	uint8_t key[KEYLEN], blk[BLOCKLEN];
	send_char(KEY);
	recv_block(key);
	recv_block(&key[16]);
	recv_block(blk);
	struct AES_ctx ectx;
	AES_init_ctx_iv(&ectx, key, blk);
	send_char(GOOD);
	while(cnv.size>0){
		if(!recv_block(blk)){
			AES_CBC_decrypt_buffer(&ectx, blk, BLOCKLEN);
			send_block(blk);
			cnv.size -= 16;
		}
	}
	return 0;
}