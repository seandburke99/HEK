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

    def connect_to_key(self, port, baud):
        NotImplemented
    
    def handshake_key(self):
        NotImplemented
    
    def get_files(self, path):
        NotImplemented
    
    def encrypt_file(self, f):
        NotImplemented
    

def main():
    com = serial.Serial("/dev/ttyUSB0", 115200, stopbits=serial.STOPBITS_TWO,timeout=3)
    if not com.isOpen():
        com.open()
    sleep(0.5)
    msg = b'1\n'
    while True:
        com.write(msg)
        ret = com.read_until()
        if msg==ret:
            break
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