# This Python file uses the following encoding: utf-8
import os, sys

from tkinter import *
from tkinter.ttk import *
from tkinter.filedialog import asksaveasfilename
from tkinter import messagebox

import matplotlib
matplotlib.use('TkAgg')

from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg

from matplotlib.figure import Figure

from math import pow

import serial  # pyserial
import numpy as np
import serial.tools.list_ports
import pandas as pd


class Janela:

    def __init__(self, raiz):
        self.raiz = raiz
        self.raiz.title('Presença automatizada')

        # Criação da interface Gráfica
        raiz.focus_set()
        # Criando Interface utilizando layout grid
        # First Line

        # Escolha da porta
        self.ArduinoEncontrado = False

        if(self.ArduinoEncontrado == False):
            self.variables_dropdown_ports = StringVar(self.raiz)
            OPTIONS = ["Nenhum dispositivo conectado"]
            self.variables_dropdown_ports.set(OPTIONS[0])
            self.dropdown_nodes = OptionMenu(
                self.raiz, self.variables_dropdown_ports, *OPTIONS)
            self.dropdown_nodes.grid(row=0, column=0, sticky='NSEW')

            # Botão para atualizar portas
            self.button_update_ports = Button(
                self.raiz, text='Conectar', command=self.update_ports)
            self.button_update_ports.grid(row=0, column=1, sticky='NSEW')

        # Second Line
        self.button_truco = Button(
            self.raiz, text='Cadastrar aluno', command=self.cadastro)
        self.button_truco.grid(row=1, column=0, sticky='NSEW')

        # Second Line
        self.button_truco = Button(
            self.raiz, text='Gerar planilha', command=self.geracsv)
        self.button_truco.grid(row=1, column=1, sticky='NSEW')

        self.update_ports()


    def cadastro(self):
        if(self.ArduinoEncontrado):
            leitura = 0

            def callback():
                leitura = e.get()
                print(leitura)
                master.destroy()

            master = Tk()
            e = Entry(master)
            e.pack()

            b = Button(master, text="get", width=10, command=callback)
            b.pack()

            b.focus_set()

            mainloop()

            print(leitura)
        else:
            messagebox.showinfo("Erro", "Por favor, conecte o arduino")

    def geracsv(self):
        if(self.ArduinoEncontrado):
            messagebox.showinfo("Titulo", "Trucooooooo")
        else:
            messagebox.showinfo("Erro", "Por favor, conecte o arduino")

    def update_ports(self):
        res = serial.tools.list_ports.comports()
        devices = [item.device for item in res]

        self.ArduinoEncontrado = False

        if len(devices) == 0:
            devices.append("Nenhum dispositivo conectado")
        elif len(devices) == 1:
            devices.append("Arduino conectado")
            self.ArduinoEncontrado = True
            del(devices[0])

        self.variables_dropdown_ports.set('')
        self.dropdown_nodes['menu'].delete(0, 'end')

        if len(devices) != 1:
            for choice in devices:
                self.dropdown_nodes['menu'].add_command(
                    label=choice, command=lambda temp=choice: self.variables_dropdown_ports.set(temp))

        self.variables_dropdown_ports.set(devices[0])



raiz = Tk()
raiz.resizable(width=False, height=False)
Janela(raiz)
raiz.mainloop()
