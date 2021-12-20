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
        while True:
            self.com.write(b'1')
            ret = self.com.read_until()
            if ret==b'2':
                self.com.write(b'2')
                print("Handshake confirmed")
                break
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
    com = serial.Serial("/dev/ttyUSB0", 115200, stopbits=serial.STOPBITS_TWO,timeout=3)
    if not com.isOpen():
        com.open()
    i = 0
    while True:
        com.write(b'1')
        ret = com.read_until()
        if ret==b'2':
            com.write(b'2')
            print("Handshake confirmed")
            break
        i+=1
    ret = com.read(32)
    print(len(ret), [b for b in ret])
    print(compute_crc(ret))

def main3():
    with open("input_files/test.txt", "rb") as f:
        while True:
            ret = f.read(16)
            if ret:
                print([b for b in ret])
            else:
                break
            

    
if __name__ == "__main__":
    main3()