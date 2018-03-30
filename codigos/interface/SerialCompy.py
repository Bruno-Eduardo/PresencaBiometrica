#!/usr/bin/python
# -*- coding: ascii -*-

import serial
import time

def conectar():
    try:
        ser = serial.Serial('/dev/ttyACM0',9600)  # open serial port
        time.sleep(2)
        print("Conectado ao " + ser.name)         # check which port was really used
        return ser
    except:
        try:
            ser = serial.Serial('/dev/ttyUSB0',9600)  # open serial port
            time.sleep(2)
            print("Conectado ao " + ser.name)         # check which port was really used
            return ser
        except:
            try:
                ser = serial.Serial('COM3',9600)  # open serial port
                time.sleep(2)
                print("Conectado ao " + ser.name)         # check which port was really used
                return ser
            except:
                print("Problema ao conectar")

def cadastrar(ser, RA):
    ser.write("cadastro IN".encode('utf-8'))     # write a string
    ser.write(RA.encode('utf-8'))     # write a string

def fechar(ser):
    ser.close()             # close port
