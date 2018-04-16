#!/usr/bin/python
# -*- coding: ascii -*-

import serial
import serial.tools.list_ports
import time

def conectar():
    try:
        ser = serial.Serial('/dev/ttyACM0',9600,timeout=2)  # open serial port
        time.sleep(2)
        print("Conectado ao " + ser.name)         # check which port was really used
        return ser
    except:
        try:
            ser = serial.Serial('/dev/ttyUSB0',9600,timeout=2)  # open serial port
            time.sleep(2)
            print("Conectado ao " + ser.name)         # check which port was really used
            return ser
        except:
            try:
                ser = serial.Serial('COM3',9600,timeout=2)  # open serial port
                time.sleep(2)
                print("Conectado ao " + ser.name)         # check which port was really used
                return ser
            except:
                ser = serial.Serial("/dev/" + serial.tools.list_ports.comports()[0].name,9600,timeout=2)
                time.sleep(2)
                print("Conectado ao " + ser.name)         # check which port was really used
                return ser

def cadastrar(ser, RA):
    ser.write("a".encode('utf-8'))
    ser.write(RA.encode('utf-8'))
    #ser.write("cadastro IN".encode('utf-8'))     # write a string
    #ser.write(RA.encode('utf-8'))     # write a string
    leitura = "0"
    leitura = str(ser.read(1))[2]
    print("Meu read foi: " + leitura)
    if(leitura == "1"):
        return "Cadastrou"
    else:
        return "ERRO"


def fechar(ser):
    ser.close()             # close port
