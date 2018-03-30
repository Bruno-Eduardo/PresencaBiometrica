# This Python file uses the following encoding: utf-8
import os, sys

from tkinter import *
from tkinter.ttk import *
from tkinter.filedialog import asksaveasfilename
from tkinter import messagebox

import serial  # pyserial
import serial.tools.list_ports
import txttoplanilha
import SerialCompy

class Janela:

    def __init__(self, raiz):
        self.raiz = raiz
        self.raiz.title('Presença automatizada')

        # Criação da interface Gráfica
        menu = Menu(raiz)
        raiz.config(menu=menu)
        filemenu = Menu(menu)
        menu.add_cascade(label="Arquivo", menu=filemenu)
        filemenu.add_command(label="Sair", command=self.quit)

        # Criando Interface utilizando layout grid
        # First Line

        # Escolha da porta
        self.ArduinoEncontrado = False
        self.portaSerial = ""

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
                enviarleitura(self.portaSerial, leitura)
                cadst.destroy()

            cadst = Tk()
            e = Entry(cadst)
            e.pack()

            b = Button(cadst, text="Enviar", width=30, command=callback)
            b.pack()

            mainloop()

        else:
            messagebox.showinfo("Erro", "Por favor, conecte o arduino")

    def geracsv(self):
        if(self.ArduinoEncontrado == True):

            if gerarPlanilha:
                messagebox.showinfo("", "Planilha gerada")

                def callback():
                    plan.destroy()
                def abrirPlanilha():
                    os.system("libreoffice planilha_de_presenca.csv")
                    plan.destroy()


                plan = Tk()

                b1 = Button(plan, text="Abrir", width=30, command=abrirPlanilha)
                b1.grid(row=0, column=0, sticky='NSEW')

                b2 = Button(plan, text="Retornar", width=30, command=callback)
                b2.grid(row=0, column=1, sticky='NSEW')

                mainloop()


                plan = Tk()
                b1 = Button(plan, text="Abrir", width=30, command=plan.destroy())
                b1.pack()

                b2 = Button(plan, text="Retornar", width=30, command=plan.destroy())
                b2.pack()

                mainloop()
            else:
                messagebox.showinfo("", "Não consegui gerar a planilha")
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
            self.arduinoadress = devices[0]
            del(devices[0])

        self.variables_dropdown_ports.set('')
        self.dropdown_nodes['menu'].delete(0, 'end')

        if len(devices) != 1:
            for choice in devices:
                self.dropdown_nodes['menu'].add_command(
                    label=choice, command=lambda temp=choice: self.variables_dropdown_ports.set(temp))

        self.variables_dropdown_ports.set(devices[0])

        print(self.variables_dropdown_ports.get())

        if self.variables_dropdown_ports.get() != "Nenhum dispositivo conectado":
            self.portaSerial = SerialCompy.conectar()
    def quit(self):
        SerialCompy.fechar(self.portaSerial)
        self.raiz.destroy()


def enviarleitura(porta,leitura):
    print("enviando: " + str(leitura))
    SerialCompy.cadastrar(porta,leitura)

def gerarPlanilha():
    return txttoplanilha.gerar('planilha_de_presenca.csv')

raiz = Tk()
raiz.resizable(width=False, height=False)
Janela(raiz)
raiz.mainloop()
