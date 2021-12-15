import serial
from time import sleep

def main():
    com = serial.Serial("/dev/ttyUSB0", 115200, parity=serial.PARITY_ODD,timeout=3)
    if not com.isOpen():
        com.open()
    i = 0
    sleep(0.5)
    while(True):
        msg = "{}".format(i%10).encode("utf-8")
        com.write(msg)
        sleep(0.05)
        ret = com.read()
        print(i, ret)
        if ret == msg:
            print("Handshake confirmed")
            break
        i+=1
    sleep(1)
    com.write(b'Hello\n')
    sleep(1)
    ret = 0
    while True:
        ret = com.read(2)
        print(ret)
        com.flushInput()
        sleep(1)
    # print(com.read_all())

    
if __name__ == "__main__":
    main()