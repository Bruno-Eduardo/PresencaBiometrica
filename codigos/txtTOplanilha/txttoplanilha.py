#Encoding: UTF-8

import csv

presencas = []

#Ler o input
with open('entradas.csv') as arq:
    arqCsv = csv.reader(arq)

    for linha in arqCsv:
        diaMes = linha[1].split(',')
        diaMes = diaMes[0].split('.')
        aluno = [linha[0].split(',')[0], diaMes[0], diaMes[1]]
        presencas.append(aluno)
#Ler os dados antigos
'''
try:
    with open('planilha_de_presencas.csv') as arq:
        arqCsv = csv.reader(arq)
        primeira = True
        for linha in arqCsv:
            if(primeira):
                primeira = False
                pass
            aluno = [linha[0].split(',')[0], linha[1].split(',')[0], linha[2].split(',')[0]]
            presencas.append(aluno)
except:
    pass
'''
#Ordenar
#criar vetor com dias e o de alunos
dias = []
alunos = []
for candidato in presencas:
    if candidato[1] != "dia":
        datastr = str(candidato[1]) + "/" + str(candidato[2])
        candidato.append(datastr)
        if datastr in dias:
            pass
        else:
            dias.append(datastr)
    if candidato[0] != "RA":
        if candidato[0] in alunos:
            pass
        else:
            alunos.append(candidato[0])
alunos.sort()
dias.sort()
print alunos
print "-----------"
print dias
print "-----------"
#inicializar matriz
matriz = []
for aluno in alunos:
    matriz.append([])
for i in range(len(matriz)):
    for j in range(len(dias)+1):
        matriz[i].append([])
for i in range(len(matriz)):
    for j in range(len(matriz[i])):
        matriz[i][j] = 0
        if(j == 0):
            matriz[i][j] = alunos[i]

#atribuir presencas para a matriz
for presenca in presencas:
    matriz[alunos.index(presenca[0])][dias.index(presenca[3])+1] = 1
print matriz

#limpar informacoes iguais

#Configurar primeira linha
prim = ["RA"]
for i in dias:
    prim.append(str(i))

#Gravar numa planilha
with open('planilha_de_presenca.csv', 'w') as arq:
    arqCsv = csv.writer(arq)
    arqCsv.writerow(["1 = presente && 0 = ausente"])
    arqCsv.writerow(prim)
    for linha in matriz:
        arqCsv.writerow(linha)
    arq.close()
