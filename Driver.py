import serial
from time import sleep

def main():
    com = serial.Serial("/dev/ttyUSB0", 115200, parity=serial.PARITY_ODD)
    if not com.isOpen():
        com.open()
    i = 0
    while(True):
        com.write("{}".format(i%10).encode("utf-8"))
        print(com.read_all())
        sleep(0.2)
        i+=1
        # print(com.read_until("\n".encode("utf-8")))


    
if __name__ == "__main__":
    main()