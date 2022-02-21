from hashlib import sha256
import serial
from serial.serialutil import SerialException

import logging as log
from os import makedirs, getcwd
from os.path import isdir, basename, expanduser

from time import time, sleep

import tkinter as tk
from tkinter import Tk, filedialog
from tkinter.ttk import Progressbar

import bcrypt

def compute_crc(data):
	crc = 0;
	for b in data:
		crc ^= b
		crc &= 0xFF
	return crc

def init_logging():
	# logFile = "~/.HEK/logs/{}.log".format(time())
	logFile = "log/{}.log".format(time())
	if not isdir(expanduser("~/.HEK/logs")):
		makedirs(expanduser("~/.HEK/logs"))
	log.basicConfig(filename=logFile, encoding='utf-8', level=log.INFO)

init_logging()

class HEKApplication(Tk):
	def __init__(self, keyName="/dev/ttyUSB0"):
		"""Hardware encryption key application

		Args:
			keyName (str, optional): _description_. Defaults to "/dev/ttyUSB0".
		"""
		# Tk setup
		super(HEKApplication, self).__init__()
		self.title("Hardware Encryption Key")

		self.userPassword = tk.StringVar(self)
		self.loginForm = tk.Entry(self, textvariable=self.userPassword)
		# self.loginButton = tk.Button(self, text="Login", command=set_rsa_key)

		# Driver creation
		self.driver = HEKDriver()

		# Button and label creation and init for selecting files
		self.selectFilesButton = tk.Button(self, text="Select Files", command=self.select_files)
		self.selectedFilesLabel = tk.Label(self)

		# Button and label creation for output directory
		self.selOutputDirButton = tk.Button(self, text = "Select Output File Directory", command=self.select_output_dir)
		self.outputDirLabel = tk.Label(self)

		# Button and label creation for encrypt and decrypt actions
		self.encryptButton = tk.Button(self, text="Encrypt selected files", command=self.encrypt_files)
		self.decryptButton = tk.Button(self, text="Decrypt selected files", command=self.decrypt_files)

		# Init vars for later
		self.inputFiles = list
		self.outputDir = getcwd()

		# Add all widgets to window
		self.selectFilesButton.pack()
		self.selectedFilesLabel.pack()
		self.selOutputDirButton.pack()
		self.outputDirLabel.pack()
		self.encryptButton.pack()
		self.decryptButton.pack()

		# Driver setup
		if not self.driver.connect_to_key(port = keyName):
			log.error("Unable to connect to hardware key")
		if not self.driver.handshake_key():
			log.error("Unable to complete handshake with hardware key")
		return
	
	def select_output_dir(self) -> None:
		self.outputDir = filedialog.askdirectory(initialdir=self.outputDir, title="Select directory")
		self.outputDirLabel.config(text = self.outputDir)

	def select_files(self) -> None:
		self.inputFiles = filedialog.askopenfilenames(initialdir=getcwd())
		
		all = "Selected Files:\n"
		for i in self.inputFiles:
			all += i + "\n"
		self.selectedFilesLabel.config(text=all)
	
	def encrypt_files(self):
		pBar = Progressbar(self, orient=tk.HORIZONTAL, length=100)
		pBar.pack(expand=True)
		for f in self.inputFiles:
			# Update the window without getting user input
			self.update_idletasks()
			# Perform encryption
			outFName = self.outputDir + "/" + str(basename(f)).replace('.', '_') + ".hef"
			self.driver.encrypt_file(f, outFName)
			# Update bar value
			pBar['value'] += int(100/len(self.inputFiles))
			sleep(0.1)
		pBar.destroy()

	def decrypt_files(self):
		pBar = Progressbar(self, orient=tk.HORIZONTAL, length=100)
		pBar.pack(expand=True)
		for f in self.inputFiles:
			# Update the window without getting user input
			self.update_idletasks()
			# Perform decryption
			outFName = self.outputDir + "/" + str(basename(f)).replace(".hef", '').replace('_', '.')
			self.driver.decrypt_file(f, outFName)
			# Update bar value
			pBar['value'] += int(100/len(self.inputFiles))
			sleep(0.1)
		pBar.destroy()


class HEKDriver:
	UNLOCK = b'u'
	LOCK = b'l'
	NEWUSER = b'n'
	KEY = b'k'
	HASH = b'h'
	ENCRYPTFILE = b'e'
	DECRYPTFILE = b'd'
	SIZE = b's'
	GOOD = b'g'
	FAIL = b'f'
	def __init__(self):
		return

	def connect_to_key(self, port="/dev/ttyUSB0", baud=115200) -> bool:
		try:
			self.com = serial.Serial(port, baud, stopbits=serial.STOPBITS_TWO,timeout=3)
			if not self.com.isOpen():
				self.com.open()
		except SerialException:
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
					break
		except:
			return False
		finally:
			NotImplemented
		return True
	
	def new_user_key(self, pw : str) -> bool:
		return self.unlock_key(pw)

	def unlock_key(self, pw : str) -> bool:
		self.com.write(self.UNLOCK)
		self.com.timeout = 4
		if self.com.read(1) != self.HASH:
			return False
		self.com.write(sha256(pw.encode('utf-8'), usedforsecurity=True).digest())
		if self.com.read(1) == self.FAIL:
			return False
		return True

	def lock_key(self) -> bool:
		self.com.write(self.LOCK)
		if self.com.read(1) == self.LOCK:
			return True
		return False
	
	def encrypt_file(self, fin : str, fout : str) -> bool:
		# ptf = open(fin, "rb")
		# ecf = open(fout, "wb")
		with open(fin, "rb") as ptf, open(fout, "wb") as ecf:
			buf = []
			sz = 0
			eof = False
			while not eof:
				ret = ptf.read(16)
				if ret:
					sz += 16
					if(len(ret) < 16):
						tmp = 15-len(ret)
						ret += bytes(tmp) + bytes([tmp+1])
						eof = True
					buf.append(ret)
				else:
					break
			self.com.write(self.ENCRYPTFILE)
			log.debug("Sent encryption command")
			self.com.timeout = 4
			ret = self.com.read(1)
			if ret == self.FAIL:
				log.warn("Received failure when trying to encrypt. Check lock status")
				return False
			elif ret != self.SIZE:
				log.error("Received invalid size confirmation")
				return False
			log.debug("Received size cmd")
			ssz = sz
			for i in range(8):
				self.com.write(bytes([ssz & 0xFF]))
				ssz = ssz>>8
			log.debug("Sent size of {} bytes".format(sz))
			ecf.write(self.com.read(32))
			ecf.write(self.com.read(16))
			log.debug("Received key and initialization vector")
			self.com.timeout = 4
			for i,b in enumerate(buf):
				log.debug("Encrypting block {}/{}".format(i+1,int(sz/16)))
				self.com.write(b)
				ret = self.com.read(16)
				ecf.write(ret)
		ptf.close()
		ecf.close()

	def decrypt_file(self, fin : str, fout : str):
		# ecf = open(fin, "rb")
		# ptf = open(fout, "wb")
		with open(fin, "rb") as ecf, open(fout, "wb") as ptf:
			key = ecf.read(32)
			iv = ecf.read(16)
			buf = []
			sz = 0
			while True:
				ret = ecf.read(16)
				if ret:
					sz += len(ret)
					buf.append(ret)
				else:
					break
			self.com.write(self.DECRYPTFILE)
			log.debug("Sent decryption cmd")
			self.com.timeout = 4
			ret = self.com.read(1)
			if ret == self.FAIL:
				log.warn("Received failure when trying to decrypt. Check lock status")
				return False
			elif ret != self.SIZE:
				log.error("Received invalid size confirmation")
				return False
			log.debug("Received size cmd")
			ssz = sz
			for i in range(8):
				self.com.write(bytes([ssz & 0xFF]))
				ssz = ssz>>8
			self.com.read_until(self.KEY)
			self.com.write(key)
			self.com.write(iv)
			log.debug("Sent key and initialization vector")
			self.com.timeout = 4
			self.com.read_until(self.GOOD)
			for i,b in enumerate(buf):
				log.debug("Decrypting block {}/{}".format(i+1,int(sz/16)))
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

def main():
	d = HEKDriver()
	print("Connected:", d.connect_to_key())
	print("Handshake:", d.handshake_key())
	if d.new_user_key("TestPass"):
		print("Successful user key creation")
	else:
		print("Unable to make key")
	
if __name__ == "__main__":
	main()