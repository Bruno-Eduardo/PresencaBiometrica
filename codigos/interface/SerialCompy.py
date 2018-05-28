#!/usr/bin/python
# -*- coding: ascii -*-

import serial
import serial.tools.list_ports
import time
import csv

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
    while leitura != "1":
        leitura = str(ser.read(1))[2]
        print(leitura)
    print("Meu read foi: " + leitura)
    if(leitura == "1"):
        return "Cadastrou"
    else:
        return "ERRO"
def buscaPlanilha(ser):
    ser.write("b".encode('utf-8'))

    time.sleep(1)

    texto = (str(ser.read(ser.inWaiting())))
    texto = str(texto[2:-1])
    textoSeparado = texto.split("\\r\\n")

    file  = open("entradas.csv","w")
    i = 0
    tam = len(textoSeparado)
    for i in range(0,tam):
        if(i < tam-1):
            file.write(textoSeparado[i] + '\r\n')
        else:
            file.write(textoSeparado[i])
        i += 1
    file.close()

    return True

def fechar(ser):
    ser.close()             # close port
