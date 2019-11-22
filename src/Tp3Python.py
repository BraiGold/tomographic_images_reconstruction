import numpy as np#; np.random.seed(0)
import sys
from numpy import linalg as LA

import subprocess
import matplotlib.pyplot as plt
from PIL import Image
from scipy.misc import toimage

# def dibujarImagenPGM(I,nombreArchivo):
#     archivo = open(nombreArchivo,'w')
#     archivo.write('P5 '+str(len(I)) +' '+ str(len(I))+ ' 255\n')
#     archivo = open(nombreArchivo,'ab')
#     I=I.astype('B').reshape(-1)
#     for pixel in I:
#         archivo.write(pixel)

def generarPuntosRandom(nDiscret,n):
    paresDepuntos=[]
    for x in range(0,nDiscret):
        for y in range(0,nDiscret):
            par = np.array([0,np.random.random_integers(0,n-1),n-1,np.random.random_integers(0,n-1)])
            paresDepuntos.append(par)
    for x in range(0,nDiscret):
        for y in range(0,nDiscret):
            par = np.array([0,np.random.random_integers(0,n-1),np.random.random_integers(1,n-1),np.random.random_integers(0,n-1)])
            paresDepuntos.append(par)
    for x in range(0,nDiscret):
        for y in range(0,nDiscret):
            par = np.array([n-1,np.random.random_integers(0,n-1),np.random.random_integers(0,n-2),np.random.random_integers(0,n-1)])
            paresDepuntos.append(par)
    return paresDepuntos

def dibujarImagenCSV(I,nombreArchivo):
    print('I.shape:')
    print(I.shape)
    np.savetxt(nombreArchivo, I, delimiter=',')


def negativosA0(num):
    if(num < 0):
        return 0
    else:
        return num

#funcion para generar una imagen predecible
def generarImagenDeTesteo(nombreArchivo,n):
    I = []
    I= np.array(list(range(0,n**2))).reshape((n,n))
    np.savetxt(nombreArchivo, I, delimiter=',')

def leerCSV(nombreCSV):
    archivo = open(nombreCSV,'r')
    M = []
    for linea in archivo:
        fila = [negativosA0(float(x)) for x in linea[:-1].split(',')]
        M.append(np.array(fila))

    return np.array(M)


def generarRayo(p1,p2,n):
    x1 = p1[0]
    y1 = p1[1]
    x2 = p2[0]
    y2 = p2[1]
    rayo = np.zeros((n,n))
    dx = x2 - x1
    dy = y2 - y1
    for x in range(x1,x2+1):
        y = y1 + dy * (x - x1) / dx
        rayo[int(x)][int(y)] = 1

    return rayo

def generarMultiplesRayos(paresDepuntos,n):
    rayos = []
    for par in paresDepuntos:
        puntoA = (par[0],par[1])
        puntoB = (par[2],par[3])
        rayo = generarRayo(puntoA,puntoB,n)
        rayos.append(rayo)
    return np.array(rayos)


def crearD(rayos,n):
    #return np.array([r.reshape(-1) for r in rayos])
    print('rayos shape:')
    print(rayos.shape)
    D = np.zeros((len(rayos),n,n))
    for r in range(0,len(rayos)):
        for i in range(0,len(rayos[0])):
            for j in range(0,len(rayos[0])):
                #print('r: ' + str(r) + ' i: ' + str(i) +' j:' +str(j))
                D[r][int(i/(len(rayos[0])/n))][int(j/(len(rayos[0])/n))]  += rayos[r][i][j]
    return np.array([ray.reshape(-1) for ray in D])

def calcularT(rayos,imagen):
    t = np.zeros(len(rayos))
    for i in range(0,len(rayos)):
        t[i] = np.sum( rayos[i] * imagen) #atenti que es hadamar product osea cordenada a cordenada
    return t

def agregarRuido(t,ruido):
    return t + np.random.uniform(0,ruido,len(t))

def rayoCanonico(i,n):
    r = np.zeros((n,n))
    r[int(i / n)][i % n] = 1
    return r

def errorCuadraticoMedio(imOriginal,imObtenida,n):
    return np.sum((imOriginal-imObtenida)*(imOriginal-imObtenida)) / n**2#atenti que es hadamar osea cordenada a cordenada



#generarImagenDeTesteo('imgtest.csv',4)

imagen = leerCSV(sys.argv[1])
n = len(imagen)
nDiscret = 25
# print('rayo1')
# rayo1=generarRayo((0,2),(9,7),10) #cambiar n por 10 para tener visbilizar que ande
# print(rayo1)
#
# print('rayo2 (el 1 transpuesto)')
# rayo2 = np.transpose(rayo1)
# print(rayo2)

#rayoCurvaDePeano = np.ones((n,n))
#rayos=np.array([rayo1,rayo2])
# paresDepuntosAzarosos = []
# for i in range(0,5*n+2):
#     paresDepuntosAzarosos.append( (((0,np.random.randint(0,n-1)),(n-1,np.random.randint(0,n-1)))) )
# print('cant paresDepuntosAzarosos: ' + str(len(paresDepuntosAzarosos )))

# paresDepuntos = [((0,x),(n-1,n-1-x)) for x in list(range(0,n-1,1))]
# print('cant paresDepuntos no azar: ' +str(len(paresDepuntos)))
# paresDepuntos =np.concatenate( (paresDepuntos,paresDepuntosAzarosos),axis=0)
# print('cant puntos totales: ' +str(len(paresDepuntos)))
# paresDepuntos = []
# step =max(1,int(0.01 * n)) #cuanto mas bajo ete numero mejor si es 1 pixel va a tirar rayos en todas las lineas
# for x in range(0,n,step):
#     for y in range(0,n,step):
#         paresDepuntos.append(((0,x),(n-1,y)))

paresDepuntos = generarPuntosRandom(nDiscret,n)
rayos = generarMultiplesRayos(paresDepuntos,n)
print('cantidad de rayos (faltan transpuestos) '+ str(len(rayos)))

print('shape rayos:')
print(rayos.shape)
rayosTransp = np.array([np.transpose(r) for r in rayos])
print('shape rayosT:')
print(rayosTransp.shape)
rayos = np.concatenate((rayos,rayosTransp),axis=0)
print('shape rayos:')
print(rayos.shape)

# print('rayo 41')
# for i in range(len(rayos[41])):
#     for j in range(len(rayos[41])):
#         if (rayos[41][i][j] != 0):
#             print('x:'+ str(i) + 'y:'+str(j))

#np.append(rayos,[rayoCurvaDePeano])

#rayos = np.array( [rayoCanonico(i,n) for i in range(0,n**2)])

# m := len(rayos)
t = calcularT(rayos,imagen)
print('t: ')
print(t)

ruido = float(sys.argv[2])
t = agregarRuido(t,ruido)

print('len(t):')
print(len(t))

##empiezo la reconstruccion
print('rayos[0]')
print(rayos[0])
print('rayos[1]')
print(rayos[1])
print('rayos[2]')
print(rayos[2])
n = int(sys.argv[3]) # importante que n divida a len(imagen) y a len(imagen[0])

#print('len rayos:')
#print(len(rayos))
D = crearD(rayos,n)

#print('D shape:')
#print(D)

V = LA.solve(np.matmul(np.transpose(D),D),np.matmul(np.transpose(D),t))
#V = LA.lstsq(D,t)[0]
#print('V:')
#print(V)
# for i in range(0,len(V)): #le bajo un poco el brillo a la imagen
#     V[i]= (V[i]**2)
V = V.reshape((n,n))

dibujarImagenCSV(V,"res.csv")

imagen = leerCSV('./imagenesConv/tomo25x25.csv')
if(len(imagen) == n):
    print('el error cuadratico medio es:' )
    print(errorCuadraticoMedio(imagen,V,n))

csvSalida = './res.csv'
nombreArchivoImagenSalida = './reconstruccionConPy.png'
subprocess.call(['python3', './conversor_csv/csv_visualizer.py', csvSalida , nombreArchivoImagenSalida])
#print(D)

#Duri = leerCSV('./brianesunpelotudo.csv')

#print(Duri)
# print("Resta D- Dcalculada")
# print('shape Duri')
# print(Duri.shape)
#
# print(LA.norm(Duri-D))
# print(Duri)
# print(D)
# f=open('./brianesunpelotudo2.csv')
# tiemposUri = [float(x) for x in f.readline().split(',')]
# dif = t-tiemposUri
# for i in range(len(dif)):
#     if(dif[i] < 0.01):
#         dif[i] = 0
#     else:
#         dif[i] = i
# print(dif)
#agrego transpuestos


#ahora veo que hago con esto
# V = imagen.reshape(-1,1)
# print('rayos shape:')
# print(rayos.shape)
#
# D= crearD(rayos)



# #Para verificar de estan bien encontrados los t_k resuelvo D.V= t   D:="rayos como col" v en R**(N**2)
# V = imagen.reshape(-1,1)
# print('rayos shape:')
# print(rayos.shape)
#
# D= crearD(rayos)
#
# print(D.shape)
# print(V.shape)
# print(np.dot(D,V))

# verifico resolviendo D**t D . X = t
# para verificar que funciona necesito que mis rayos generen SCD pues es la unica forma de asegurar unicos vij
# # me creo entonces el sistema de rayos a partir de las matrices canonicas
# print (sys.maxsize)
# print('------------------------------------------------')
# rayosCanonicos = []
# for i in range(0,50):
#     rayoCanonico = np.zeros((len(imagen),len(imagen)))
#     rayoCanonico[int(i / len(imagen))][i % len(imagen)] = 1
#     rayosCanonicos.append(rayoCanonico)
#
# rayosCanonicos = np.array(rayosCanonicos)
# t=calcularT(rayosCanonicos,imagen)
#
# #print('t:')
# #print(t)
# D = crearD(rayosCanonicos)
# vijCalculados  = LA.solve(np.matmul(np.transpose(D),D),np.matmul(np.transpose(D),t))
#
#
# print((vijCalculados.reshape(-1)==imagen.reshape(-1)))
# print('--------------------------------------------------')
# print(n)
# print(len(imagen[0]))
# print(np.trace(imagen))
