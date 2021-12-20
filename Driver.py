import serial
from time import sleep, time_ns
import tkinter as tk

def compute_crc(data):
    crc = 0;
    for b in data:
        crc ^= b
        crc &= 0xFF
    return crc

class HEK:
    def __init__(self):
        return

    def connect_to_key(self, port="/dev/ttyUSB0", baud=115200):
        self.com = serial.Serial("/dev/ttyUSB0", 115200, stopbits=serial.STOPBITS_TWO,timeout=3)
        if not self.com.isOpen():
            self.com.open()
        return
    
    def handshake_key(self):
        try:
            for i in range(10):
                self.com.write(b'1')
                ret = self.com.read_until()
                if ret==b'2':
                    self.com.write(b'2')
                    print("Handshake confirmed")
                    break
        except:
            print("Unable to connect to microcontroller")
        return
    
    def get_files(self, path):
        NotImplemented
    
    def encrypt_file(self, fin, fout="output_files/encrypted_file.hef"):
        ptf = open(fin, "rb")
        ecf = open(fout, "wb")
        buf = []
        sz = 0
        while True:
            ret = ptf.read(16)
            if ret:
                sz += len(ret)
                if(len(ret) < 16):
                    sz += 16-len(ret)
                    ret += bytes(16-len(ret))
                buf.append(ret)
            else:
                break
        self.com.write(b'e')
        print("Sent encryption cmd")
        self.com.timeout = 8
        self.com.read_until(b's')
        print("Received size cmd")
        self.com.write(sz)
        ecf.write(self.com.read(32))
        ecf.write(self.com.read(16))
        print("Received key and initialization vector")
        print(buf[0])
        self.com.timeout = 0
        for i in range(int(sz/16)):
            print("Encrypting block {}/{}".format(i,int(sz/16)))
            self.com.write(buf[i])
            ret = self.com.read(16)
            while ret==b'':
                self.com.write(buf[i])
                ret = self.com.read(16)
            ecf.write(ret)
        ptf.close()
        ecf.close()

def main():
    K = HEK()
    K.connect_to_key()
    K.handshake_key()
    K.encrypt_file("input_files/test.txt")

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
    block = b'Hello\n'
    for i in range(5):
        com.write(block)
        key = com.read_until()
        print(key)
        # print("{} byte key:".format(len(key)), [b for b in key], compute_crc(key), int.from_bytes(kcrc, 'big'))

def main3():
    ret = compute_crc([
        1,2,3,4,5,6,7,8
    ])
    print(ret)
            

    
if __name__ == "__main__":
    main()