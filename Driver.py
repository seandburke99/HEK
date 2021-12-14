import serial
from time import sleep

def main():
    com = serial.Serial("/dev/ttyUSB0", 115200, parity=serial.PARITY_ODD,timeout=3)
    if not com.isOpen():
        com.open()
    i = 0
    while(True):
        com.write("{}".format(i%10).encode("utf-8"))
        sleep(0.2)
        ret = com.read_all()
        print(i, ret)
        if("{}".format(i%10).encode("utf-8") == ret):
            print("Handshake confirmed")
            break
        i+=1
    com.write(b'H\n')
    sleep(0.2)
    ret = com.read()
    print(ret)



    
if __name__ == "__main__":
    main()