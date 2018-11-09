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
        self.button_cad = Button(
            self.raiz, text='Cadastrar aluno', command=self.cadastro)
        self.button_cad.grid(row=1, column=0, sticky='NSEW')

        # Second Line
        self.button_pla = Button(
            self.raiz, text='Gerar planilha', command=self.geracsv)
        self.button_pla.grid(row=1, column=1, sticky='NSEW')

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

            if gerarPlanilha(self.portaSerial):
                messagebox.showinfo("", "Planilha gerada")

                def callback():
                    plan.destroy()
                def abrirPlanilha():
                    try:
                        os.system("xdg-open planilha_de_presenca.csv")
                    except:
                        os.system("start planilha_de_presenca.csv")

                    print("aqui abre a planilha!")
                    plan.destroy()


                plan = Tk()

                b1 = Button(plan, text="Abrir", width=30, command=abrirPlanilha)
                b1.grid(row=0, column=0, sticky='NSEW')

                b2 = Button(plan, text="Retornar", width=30, command=callback)
                b2.grid(row=0, column=1, sticky='NSEW')

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
    tentativa = SerialCompy.cadastrar(porta,leitura)
    if(tentativa == "Cadastrou"):
        messagebox.showinfo("OK", "Aluno " + leitura + " cadastrado")
    else:
        messagebox.showinfo("Erro", "Nao consegui cadastrar")

def gerarPlanilha(porta):

    print("buscando csv no arduino...")
    planilhaDumpada = SerialCompy.buscaPlanilha(porta)
    if(planilhaDumpada):
        print("\n" + "Planilha dumpada com sucesso")
    else:
        messagebox.showinfo("Erro", "Nao consegui ler a planilha do arduino")
        return False

    return txttoplanilha.gerar('planilha_de_presenca.csv','entradas.csv')

def arrumarPos(root):
    w = 277 # width for the Tk root
    h = 44 # height for the Tk root

    ws = root.winfo_screenwidth() # width of the screen
    hs = root.winfo_screenheight()

    x = (ws/2) - (w/2)
    y = (hs/2) - (h/2)

    root.geometry('%dx%d+%d+%d' % (w, h, x, y))

raiz = Tk()
arrumarPos(raiz)
Janela(raiz)
raiz.mainloop()
