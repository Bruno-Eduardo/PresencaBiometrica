#!/usr/bin/python
# -*- coding: ascii -*-

import serial
import time

try:
    ser = serial.Serial('/dev/ttyACM0',9600)  # open serial port
except:
    try:
        ser = serial.Serial('/dev/ttyUSB0',9600)  # open serial port
    except:
        ser = serial.Serial('COM3',9600)  # open serial port

time.sleep(2)

print(ser.name)         # check which port was really used
print("Ola mundo".encode('utf-8'))
ser.write("165215".encode('utf-8'))     # write a string
ser.close()             # close port
