import serial
from time import sleep, time_ns

def main():
    com = serial.Serial("/dev/ttyUSB0", 115200, stopbits=serial.STOPBITS_TWO,timeout=3)
    if not com.isOpen():
        com.open()
    sleep(0.5)
    
    a = time_ns()
    j = 0
    k = 0
    msg = b'0123456789\n'
    for i in range(50):
        com.write(msg)
        ret = com.read_until()
        if msg != ret:
            print("Error - Sent {} and received {}".format(msg, ret))
        j+= len(msg)-1
        k+= len(ret) - 1
    b = time_ns()
    c = (b-a)/1.0e9
    print("Bytes sent: {}\nBytes read: {}\nTime Spent: {}\nBytes Per Second: {}".format(j, k, c, j/c))

    
if __name__ == "__main__":
    main()