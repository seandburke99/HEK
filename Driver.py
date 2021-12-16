import serial
from time import sleep, time_ns
import tkinter as tk

def compute_crc(data):
    crc = data[0] & 0xFF
    for b in data:
        crc ^= b
        crc &= 0xFF
    return crc

class HEK:
    def __init__(self):
        NotImplemented

    def connect_to_key(self, port="/dev/ttyUSB0", baud=115200):
        self.com = serial.Serial("/dev/ttyUSB0", 115200, stopbits=serial.STOPBITS_TWO,timeout=3)
        if not self.com.isOpen():
            self.com.open()
        return
    
    def handshake_key(self):
        i = 0
        while True:
            msg = bytes([i%10])+b'\n'
            self.com.write(msg)
            ret = self.com.read_until()
            print(msg, ret)
            if msg==ret:
                break
            i+=1
        return
    
    def get_files(self, path):
        NotImplemented
    
    def encrypt_file(self, f):
        NotImplemented
    

def main():
    K = HEK()
    K.connect_to_key()
    K.handshake_key()

def main2():
    
    sleep(0.5)
    
    j = 0
    k = 0
    msg = b'0123456789'
    print(compute_crc(msg))
    msg = b'0123456789\n'
    a = time_ns()
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