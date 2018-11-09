# This Python file uses the following encoding: utf-8
import os, sys

##########################################
# DSIF - FEEC - Unicamp
# Autor: Ricardo Mazza Zago
# Email: ricardozago91@gmail.com
##########################################

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
        self.raiz.title('Análise de Dados')

        # Criação da interface Gráfica

        menu = Menu(raiz)
        raiz.config(menu=menu)
        filemenu = Menu(menu)
        menu.add_cascade(label="Arquivo", menu=filemenu)
        filemenu.add_command(label="Sair", command=self.quit)

        helpmenu = Menu(menu)
        menu.add_cascade(label="Ajuda", menu=helpmenu)
        helpmenu.add_command(label="Sobre", command=self.about)

        # Criando Interface utilizando layout grid
        # First Line

        # Escolha da porta
        self.variables_dropdown_ports = StringVar(self.raiz)
        OPTIONS = ["Nenhum dispositivo conectado"]
        self.variables_dropdown_ports.set(OPTIONS[0])
        self.dropdown_nodes = OptionMenu(
            self.raiz, self.variables_dropdown_ports, *OPTIONS)
        self.dropdown_nodes.grid(row=0, column=0, sticky='NSEW')

        # Botão para atualizar portas
        self.button_update_ports = Button(
            self.raiz, text='Atualizar portas', command=self.update_ports)
        self.button_update_ports.grid(row=0, column=1, sticky='NSEW')

        # Second Line
        # Start
        self.button_rodar = Button(
            self.raiz, text='Start', command=self.start)
        self.button_rodar.grid(row=1, column=0, sticky='NSEW')

        # Stop
        self.button_stop = Button(
            self.raiz, text='Stop', state=DISABLED, command=self.stop)
        self.button_stop.grid(row=1, column=1, sticky='NSEW')

        # Salvar
        self.button_save = Button(
            self.raiz, text='Salvar', state=DISABLED, command=self.save)
        self.button_save.grid(row=1, column=2, sticky='NSEW')

        # Third line (Canvas)
        self.fig = Figure(figsize=(12, 6), facecolor='white')
        self.canvas = FigureCanvasTkAgg(self.fig, master=raiz)
        self.canvas.get_tk_widget().grid(row=2, column=0, columnspan=3, sticky='NSEW')

        self.update_ports()

    def about(self):
        messagebox.showinfo(
            "Info", "##########################################\n# DSIF - FEEC - Unicamp\n# Autor: Ricardo Mazza Zago\n# Email: ricardozago91@gmail.com\n##########################################")

    def quit(self):
        self.raiz.destroy()

    def update_ports(self):
        res = serial.tools.list_ports.comports()
        devices = [item.device for item in res]
        if len(devices) == 0:
            devices.append("Nenhum dispositivo conectado")

        self.variables_dropdown_ports.set('')
        self.dropdown_nodes['menu'].delete(0, 'end')

        for choice in devices:
            self.dropdown_nodes['menu'].add_command(
                label=choice, command=lambda temp=choice: self.variables_dropdown_ports.set(temp))

        self.variables_dropdown_ports.set(devices[0])

    def save(self):
        ftypes = [('.png (PNG)', '*.png')]
        f = asksaveasfilename(filetypes=ftypes, defaultextension=".png")

        print(f)

        if f != '':
            self.fig.savefig(f)

        ftypes = [('.xls (Excel)', '*.xls')]
        f = asksaveasfilename(filetypes=ftypes, defaultextension=".xls")

        print(f)

        if f != '':
            df = pd.DataFrame(self.dados)
            df.columns = ['ADC_1', 'ADC_2']

            df.to_excel(f)

    def stop(self):

        self.button_update_ports['state'] = 'NORMAL'
        self.button_rodar['state'] = 'NORMAL'
        self.button_save['state'] = 'NORMAL'
        self.button_stop['state'] = DISABLED

        if self.cancel_id is not None:
            raiz.after_cancel(self.cancel_id)
            self.cancel_id = None
            self.ser.close()

    def start(self):

        self.button_update_ports['state'] = DISABLED
        self.button_rodar['state'] = DISABLED
        self.button_save['state'] = DISABLED
        self.button_stop['state'] = 'NORMAL'

        porta = self.variables_dropdown_ports.get()
        self.ser = serial.Serial(porta, 9600, timeout=2)
        self.i = 0
        self.cancel_id = None
        self.dados = np.zeros((1000, 2))
        self.rodar()

    # Funções da interface Gráfica
    def rodar(self):

        x = np.linspace(0, 1000, num=1001)

        self.ser.flushInput()
        self.ser.readline()
        temp = self.ser.readline()
        leituras = re.findall(r'\d+', temp.decode("ascii"))

        # Aqui se define o fundo de escala
        # Neste caso temos 5 V e um adc com 10 bits
        # Então 5/(2^10) por divisão
        self.dados[self.i, :] = [(5 / pow(2, 10)) * int(leitura)
                                 for leitura in leituras]

        self.fig.clear()
        axis = self.fig.add_subplot(111)
        axis.plot(x[:self.i], self.dados[:self.i, 0], label='ADC 1')
        #axis.plot(x[:self.i], self.dados[:self.i, 1], label='ADC 2')
        axis.legend()
        axis.set_ylabel('Tensão (V)')
        self.fig.subplots_adjust(
            left=0.05, bottom=0.05, right=0.98, top=0.95, wspace=None, hspace=None)
        self.canvas.draw()

        self.i += 1

        if self.i >= 1000:
            self.i = 0

        self.cancel_id = raiz.after(100, self.rodar)


raiz = Tk()
raiz.resizable(width=False, height=False)
Janela(raiz)
raiz.mainloop()
