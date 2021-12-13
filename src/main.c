#include <HEK.h>

char buffer[MAX_LINE];

int main(void){

    HEK_init();
    
    while(1){
        send_line(buffer);        
    }
    return 0;
}