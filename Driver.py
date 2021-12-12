import serial

def main():
    com = serial.Serial("/dev/ttyUSB0", 115200, parity=serial.PARITY_ODD)
    if not com.isOpen():
        com.open()
    while(True):
        print(com.read(6))
    
if __name__ == "__main__":
    main()