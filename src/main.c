#include <HEK.h>

int main(void){

    HEK_init();
    char buffer[20];
    while(1){
        recv_line(buffer);
        send_line(buffer);        
    }
    return 0;
}