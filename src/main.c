#include <HEK.h>

const char *msg = "Hello\0";

int main(void){

    HEK_init();

    while(1){
        send_line(msg);        
    }
    return 0;
}