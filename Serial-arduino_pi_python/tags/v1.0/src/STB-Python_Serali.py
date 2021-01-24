import serial
import time

s = serial.Serial('/dev/ttyUSB0', 9600)
s.isOpen()
time.sleep(5)

s.write(b'1')
