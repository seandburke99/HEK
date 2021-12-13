#include <HEK.h>

const char *msg = "Hello";

int main(void){

    HEK_init();

    while(1){
        send_line(msg);        
    }
    return 0;
}