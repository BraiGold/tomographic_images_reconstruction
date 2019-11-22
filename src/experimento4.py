import numpy as np#; np.random.seed(0)
import sys
from numpy import linalg as LA
import subprocess
import matplotlib.pyplot as plt
from PIL import Image
from scipy.misc import toimage

##EN este codigo para que se generen las imagenes con disposiciones de rayos
##es necesario descomentar los bloques en el cuerpo ("main") que generan multiples rayos
## y los bloques que guardan eso como imagen
## una pc con poca memoria necesita tenerlo comentado para correr bien la segunda parte sin memory error

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
            par = np.array([0,np.random.random_integers(0,n-1),np.random.random_integers(1,n-1),np.random.random_integers(0,1)*(n-1)])
            paresDepuntos.append(par)
    for x in range(0,nDiscret):
        for y in range(0,nDiscret):
            par = np.array([n-1,np.random.random_integers(0,n-1),np.random.random_integers(0,n-2),np.random.random_integers(0,1)*(n-1)])
            paresDepuntos.append(par)
    return paresDepuntos

def generarPuntosCatetosOpuestos(nDiscret,n):
    paresDepuntos = []
    step = max(1,int(0.01 * n)) #cuanto mas bajo ete numero mejor si es 1 pixel va a tirar rayos en todas las lineas
    for x in range(0,n,step):
        for y in range(0,n,step):
            par = np.array([0,x,n-1,y])
            paresDepuntos.append(par)
    return paresDepuntos

def generarPuntosRectos(nDiscret,n):
    paresDepuntos=[]
    step = 1
    for x in range(0,n,step):
        par = np.array([0,x,n-1,x])
        paresDepuntos.append(par)
    return paresDepuntos

def generarPuntosTodos(nDiscret,n):
    paresDepuntos = []
    step = max(1,int(0.01 * n)) #cuanto mas bajo ete numero mejor si es 1 pixel va a tirar rayos en todas las lineas
    for x in range(0,n,step):
        for y in range(0,n,step):
            par = np.array([0,x,n-1,y])
            paresDepuntos.append(par)
    for x in range(0,n-1,step):
        for y in range(1,n,step):
            par = np.array([0,x,y,n-1])
            paresDepuntos.append(par)
    for x in range(1,n,step):
        for y in range(1,n,step):
            par = np.array([0,x,y,0])
            paresDepuntos.append(par)
    for x in range(0,n-1,step):
        for y in range(0,n,step):
            par = np.array([x,0,n-1,y])
            paresDepuntos.append(par)
    return paresDepuntos
def saturar255(numero):
    if(numero > 255):
        return 255
    else:
        return numero
def dibujarRayos(rayos,ruta):
    todosLosRayos =np.sum(rayos,axis=0)
    for i in range(len(todosLosRayos)):
        for j in range(len(todosLosRayos)):
            todosLosRayos[i][j]=saturar255(todosLosRayos[i][j])
    print('len(todosLosRayos):'+ str(len(todosLosRayos))+' len(todosLosRayos[0]): ' + str(len(todosLosRayos[0])))
    im = toimage(np.ones((len(todosLosRayos),len(todosLosRayos[0]))).astype(int) *todosLosRayos.max() - todosLosRayos.astype(int))
    im.save(ruta)
    #plt.gray()
    #plt.imshow(todosLosRayos.astype(int))
    #im = Image.fromarray(todosLosRayos.astype(np.uint8) * 255,mode='L')
    #im.show()

def dibujarImagenCSVint(I,nombreArchivo):
    print('I.shape:')
    print(I.shape)
    np.savetxt(nombreArchivo, I, delimiter=',',fmt='%i')

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


ruido = 1000
csvEntrada ='./imagenesConv/tomo50x50.csv'
csvSalida ='salida.csv'
nDiscret = 25
imagen = leerCSV(csvEntrada)
n = len(imagen)



# paresDepuntos = []
# step = int(n/nDiscret)#max(1,int(0.01 * n)) #cuanto mas bajo ete numero mejor si es 1 pixel va a tirar rayos en todas las lineas
# for x in range(0,n,step):
#     for y in range(0,n,step):
#         par = np.array([0,x,n-1,y])
#         paresDepuntos.append(par)


trasponerONo = 1
paresDepuntos = generarPuntosRandom(nDiscret,n)
#rayos = generarMultiplesRayos(paresDepuntos,n)
dibujarImagenCSVint(np.array(paresDepuntos).astype(int),'puntosParaLeer.csv')
#dibujo rayos
# rayosTransp = np.array([np.transpose(r) for r in rayos])
# print(rayosTransp.shape)
# if(trasponerONo == 1):
#     rayos = np.concatenate((rayos,rayosTransp),axis=0)
# dibujarRayos(rayos,'./Exp4Resultados/disposicionRayosRandomTrans.png')

imagen = leerCSV('./imagenesConv/tomo25x25.csv')
subprocess.call(['./main','-r',str(ruido), '-i' ,csvEntrada, '-s',csvSalida,'tipoRayo',str(4),'transpongoONO',str(trasponerONo),'usoMatrizEsparsaONO',str(0),'-n',str(nDiscret)])
ecmRandomTrans = errorCuadraticoMedio(imagen,leerCSV(csvSalida),nDiscret)
print('cantidad rayos random: ' + str(len(paresDepuntos)))
cantRayosRandomTrans = len(paresDepuntos)
#------------------------------
trasponerONo = 0
paresDepuntos = generarPuntosRandom(nDiscret,n)
#rayos = generarMultiplesRayos(paresDepuntos,n)
dibujarImagenCSVint(np.array(paresDepuntos).astype(int),'puntosParaLeer.csv')
#dibujo rayos
# rayosTransp = np.array([np.transpose(r) for r in rayos])
# print(rayosTransp.shape)
# if(trasponerONo == 1):
#     rayos = np.concatenate((rayos,rayosTransp),axis=0)
# dibujarRayos(rayos,'./Exp4Resultados/disposicionRayosRandom.png')

imagen = leerCSV('./imagenesConv/tomo25x25.csv')
subprocess.call(['./main','-r',str(ruido), '-i' ,csvEntrada, '-s',csvSalida,'tipoRayo',str(4),'transpongoONO',str(trasponerONo),'usoMatrizEsparsaONO',str(0),'-n',str(nDiscret)])
ecmRandom = errorCuadraticoMedio(imagen,leerCSV(csvSalida),nDiscret)
print('cantidad rayos random: ' + str(len(paresDepuntos)))
#----------------------

#------------------------------
trasponerONo = 0
paresDepuntos = generarPuntosCatetosOpuestos(nDiscret,n)
#rayos = generarMultiplesRayos(paresDepuntos,n)
dibujarImagenCSVint(np.array(paresDepuntos).astype(int),'puntosParaLeer.csv')
#dibujo rayos
# rayosTransp = np.array([np.transpose(r) for r in rayos])
# print(rayosTransp.shape)
# if(trasponerONo == 1):
#     rayos = np.concatenate((rayos,rayosTransp),axis=0)
# dibujarRayos(rayos,'./Exp4Resultados/disposicionRayosCatetosOpuestos.png')

imagen = leerCSV('./imagenesConv/tomo25x25.csv')
subprocess.call(['./main','-r',str(ruido), '-i' ,csvEntrada, '-s',csvSalida,'tipoRayo',str(4),'transpongoONO',str(trasponerONo),'usoMatrizEsparsaONO',str(0),'-n',str(nDiscret)])
ecmOpuestos = errorCuadraticoMedio(imagen,leerCSV(csvSalida),nDiscret)
print('cantidad rayos rectos: ' + str(len(paresDepuntos)))
#----------------------
#------------------------------
trasponerONo = 1
paresDepuntos = generarPuntosCatetosOpuestos(nDiscret,n)
#rayos = generarMultiplesRayos(paresDepuntos,n)
dibujarImagenCSVint(np.array(paresDepuntos).astype(int),'puntosParaLeer.csv')
#dibujo rayos
# rayosTransp = np.array([np.transpose(r) for r in rayos])
# print(rayosTransp.shape)
# if(trasponerONo == 1):
#     rayos = np.concatenate((rayos,rayosTransp),axis=0)
# dibujarRayos(rayos,'./Exp4Resultados/disposicionRayosCatetosOpuestosTrans.png')

imagen = leerCSV('./imagenesConv/tomo25x25.csv')
subprocess.call(['./main','-r',str(ruido), '-i' ,csvEntrada, '-s',csvSalida,'tipoRayo',str(4),'transpongoONO',str(trasponerONo),'usoMatrizEsparsaONO',str(0),'-n',str(nDiscret)])
ecmOpuestosTrans = errorCuadraticoMedio(imagen,leerCSV(csvSalida),nDiscret)
print('cantidad rayos catetos opuestos transpuestos : ' + str(len(paresDepuntos)))
cantRayosCatetosTrans = len(paresDepuntos)
# #----------------------
#------------------------------
trasponerONo = 0
paresDepuntos = generarPuntosTodos(nDiscret,n)
#rayos = generarMultiplesRayos(paresDepuntos,n)
dibujarImagenCSVint(np.array(paresDepuntos).astype(int),'puntosParaLeer.csv')
#dibujo rayos
# rayosTransp = np.array([np.transpose(r) for r in rayos])
# print(rayosTransp.shape)
# if(trasponerONo == 1):
#     rayos = np.concatenate((rayos,rayosTransp),axis=0)
# dibujarRayos(rayos,'./Exp4Resultados/disposicionRayosTodos.png')

imagen = leerCSV('./imagenesConv/tomo25x25.csv')
subprocess.call(['./main','-r',str(ruido), '-i' ,csvEntrada, '-s',csvSalida,'tipoRayo',str(4),'transpongoONO',str(trasponerONo),'usoMatrizEsparsaONO',str(0),'-n',str(nDiscret)])
ecmTodos = errorCuadraticoMedio(imagen,leerCSV(csvSalida),nDiscret)
print('cantidad rayos todos: ' + str(len(paresDepuntos)))
#----------------------
#------------------------------
trasponerONo = 1
paresDepuntos = generarPuntosTodos(nDiscret,n)
#rayos = generarMultiplesRayos(paresDepuntos,n)
dibujarImagenCSVint(np.array(paresDepuntos).astype(int),'puntosParaLeer.csv')
#dibujo rayos
# rayosTransp = np.array([np.transpose(r) for r in rayos])
# print(rayosTransp.shape)
# if(trasponerONo == 1):
#     rayos = np.concatenate((rayos,rayosTransp),axis=0)
# dibujarRayos(rayos,'./Exp4Resultados/disposicionRayosTodoTrans.png')

imagen = leerCSV('./imagenesConv/tomo25x25.csv')
subprocess.call(['./main','-r',str(ruido), '-i' ,csvEntrada, '-s',csvSalida,'tipoRayo',str(4),'transpongoONO',str(trasponerONo),'usoMatrizEsparsaONO',str(0),'-n',str(nDiscret)])
ecmTodosTrans = errorCuadraticoMedio(imagen,leerCSV(csvSalida),nDiscret)
print('cantidad rayos todos transpuestos : ' + str(len(paresDepuntos)))
cantRayosTodosTrans = len(paresDepuntos)
# #----------------------
n_groups = 3
transpuestos = (ecmRandomTrans, ecmOpuestosTrans, ecmTodosTrans)
no_transpuestos = (ecmRandom,ecmOpuestos, ecmTodos)

# create plot
fig, ax = plt.subplots()
index = np.arange(n_groups)
bar_width = 0.35
opacity = 0.8

rects1 = plt.bar(index, transpuestos, bar_width,alpha=opacity,color='b',label='Incluyendo Transpuestos')

rects2 = plt.bar(index + bar_width, no_transpuestos, bar_width,alpha=opacity,color='g',label='Sin Trasponer')

plt.xlabel('Metodo')
plt.ylabel('Error Cuadratico Medio')
plt.title('analisis de disposición de rayos')
plt.xticks(index + bar_width, ('Random \n ('+str(cantRayosRandomTrans)+'/'+str(int(cantRayosRandomTrans/2))+')',  'catetos opuestos \n ('+str(cantRayosCatetosTrans)+'/'+str(int(cantRayosCatetosTrans/2))+')', 'todos \n ('+str(cantRayosTodosTrans)+'/'+str(int(cantRayosTodosTrans/2))+')'))
plt.legend()

plt.tight_layout()
plt.show()








print('termino')



#print('cantidad Rayos:')
#print(len(paresDepuntos))
#print('error cuad medio:')
#imagen = leerCSV('./imagenesConv/tomo25x25.csv')
#print(errorCuadraticoMedio(imagen,leerCSV(csvSalida),nDiscret))

#diffImagenes = toimage(leerCSV(csvSalida)-imagen, cmin=0.0, cmax=255)
#diffImagenes = toimage((np.ones((len(imagen),len(imagen[0]))).astype(int) * 255 - (leerCSV(csvSalida)-imagen).astype(int)))
#diffImagenes.save('diferencia.png')
