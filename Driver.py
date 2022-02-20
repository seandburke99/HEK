from matplotlib.pyplot import title
import serial
from serial.serialutil import SerialException

import logging as log
import os
from os import mkdir, getcwd
from os.path import isdir

from time import time

import tkinter as tk
from tkinter import Tk
from tkinter import filedialog
from Crypto.Cipher import AES

def compute_crc(data):
	crc = 0;
	for b in data:
		crc ^= b
		crc &= 0xFF
	return crc

class HEKApplication(Tk):
	def __init__(self, keyName="/dev/ttyUSB0"):
		
		# Tk setup
		super(HEKApplication, self).__init__()
		self.title("Hardware Encryption Key")
		self.geometry("500x500")		
		self.selectFilesButton = tk.Button(self, text="Select Files", command=self.select_files)
		self.selectedFilesLabel = tk.Label(self)

		self.selOutputDirButton = tk.Button(self, text = "Select Output File Directory", command=self.select_output_dir)
		self.outputDirLabel = tk.Label(self)

		self.inputFiles = list
		
		self.outputDir = getcwd()
		self.outputFile = list

		self.selectFilesButton.pack()
		self.selectedFilesLabel.pack()
		self.selOutputDirButton.pack()
		self.outputDirLabel.pack()

		# Logging setup
		self.init_logging()

		# Driver setup
		self.driver = HEKDriver()
		if not self.driver.connect_to_key(port = keyName):
			log.error("Unable to connect to hardware key")
			return
		if not self.driver.handshake_key():
			log.error("Unable to complete handshake with hardware key")
			return
	
		return
	
	def init_logging(self):
		logFile = "log/{}.log".format(time())
		if not isdir("log"):
			mkdir("log")
		log.basicConfig(filename=logFile, encoding='utf-8', level=log.DEBUG)


	def select_output_dir(self) -> None:
		self.outputDir = filedialog.askdirectory(initialdir=self.outputDir, title="Select directory")
		self.outputDirLabel.config(text = self.outputDir)

	def select_files(self) -> None:
		self.inputFiles = filedialog.askopenfilenames(initialdir=getcwd())
		
		all = "Selected Files:\n"
		for i in self.inputFiles:
			all += i + "\n"
		self.selectedFilesLabel.config(text=all)


class HEKDriver:
	def __init__(self):
		return

	def connect_to_key(self, port="/dev/ttyUSB0", baud=115200) -> bool:
		try:
			self.com = serial.Serial("/dev/ttyUSB0", 115200, stopbits=serial.STOPBITS_TWO,timeout=3)
			if not self.com.isOpen():
				self.com.open()
		except SerialException:
			print("Unable to open connection to key")
			return False
		finally:
			NotImplemented
		return True
	
	def handshake_key(self) -> bool:
		try:
			for i in range(10):
				self.com.write(b'1')
				ret = self.com.read_until()
				if ret==b'2':
					self.com.write(b'2')
					print("Handshake confirmed")
					break
		except:
			print("Unable to complete handshake with microcontroller")
			return False
		finally:
			NotImplemented
		return True
	
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
	a = HEKApplication()
	a.mainloop()
	
if __name__ == "__main__":
	main()