import serial
from time import sleep, time_ns
import tkinter as tk
from Crypto.Cipher import AES

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
                    sz += 16
                    tmp = 15-len(ret)
                    ret += bytes(tmp) + bytes([tmp+1])
                buf.append(ret)
            else:
                break
        self.com.write(b'e')
        print("Sent encryption cmd")
        self.com.timeout = 8
        self.com.read_until(b's')
        print("Received size cmd")
        self.com.write(bytes([sz & 0xFF]))
        for i in range(1,8):
            self.com.write(bytes([sz & (i*8<<0xFF)]))
        ecf.write(self.com.read(32))
        ecf.write(self.com.read(16))
        print("Received key and initialization vector")
        self.com.timeout = 4
        for i,b in enumerate(buf):
            print("Encrypting block {}/{}".format(i,int(sz/16)))
            self.com.write(b)
            ret = self.com.read(16)
            ecf.write(ret)
        ptf.close()
        ecf.close()

    def decrypt_file(self, fin="output_files/encrypted_file.hef", fout="ret.txt"):
        ecf = open(fin, "rb")
        ptf = open(fout, "wb")
        key = ecf.read(32)
        iv = ecf.read(16)
        print("Read key and iv\n{}\n{}".format(key, iv))
        buf = []
        sz = 0
        while True:
            ret = ecf.read(16)
            if ret:
                sz += len(ret)
                buf.append(ret)
            else:
                break
        self.com.write(b'd')
        print("Sent decryption cmd")
        self.com.timeout = 8
        self.com.read_until(b's')
        print("Received size cmd")
        self.com.write(bytes([sz & 0xFF]))
        for i in range(1,8):
            self.com.write(bytes([sz & (i*8<<0xFF)]))
        self.com.read_until(b'k')
        self.com.write(key)
        self.com.write(iv)
        print("Sent key and initialization vector")
        self.com.timeout = 4
        self.com.read_until(b'g')
        for i,b in enumerate(buf):
            print("Decrypting block {}/{}".format(i+1,int(sz/16)))
            self.com.write(b)
            ret = self.com.read(16)
            if i==len(buf)-1:
                for by in ret:
                    if by:
                        ptf.write(bytes([by]))
                    else:
                        break
            else:
                if(i==len(buf)-1):
                    print(ret[15])
                ptf.write(ret)
        ptf.close()
        ecf.close()
    
    def local_encrypt(self, f):
        ecf = open("output_files/encrypted_file.hef", "rb")
        key = ecf.read(32)
        iv = ecf.read(16)
        ecf.close()
        ecf = open("tst.hef", "wb")
        with open(f, "rb") as ptf:
            
            cipher = AES.new(key, AES.MODE_CBC, iv)
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
            print(buf)
            for b in buf:
                ecf.write(cipher.encrypt(b))
        ecf.close()

def main():
    K = HEK()
    K.connect_to_key()
    K.handshake_key()
    K.decrypt_file()

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
    K = HEK()
    K.local_encrypt("input_files/test.txt")
            

    
if __name__ == "__main__":
    main()