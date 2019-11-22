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
    step = max(1,int(0.04 * n)) #cuanto mas bajo ete numero mejor si es 1 pixel va a tirar rayos en todas las lineas
    for x in range(0,n,step):
        for y in range(0,n,step):
            for z in range(1,n,step):
                par = np.array([0,x,z,y])
                paresDepuntos.append(par)
    return paresDepuntos





def saturar255(numero):
    if(numero > 255):
        return 255
    else:
        return numero
def dibujarRayos(rayos):
    todosLosRayos =np.sum(rayos,axis=0)
    for i in range(len(todosLosRayos)):
        for j in range(len(todosLosRayos)):
            todosLosRayos[i][j]=saturar255(todosLosRayos[i][j])
    print('len(todosLosRayos):'+ str(len(todosLosRayos))+' len(todosLosRayos[0]): ' + str(len(todosLosRayos[0])))
    im = toimage(np.ones((len(todosLosRayos),len(todosLosRayos[0]))).astype(int) *todosLosRayos.max() - todosLosRayos.astype(int))
    im.save("disposicionRayos.png")
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


ruido = 1
csvEntrada ='./imagenesConv/tomo25x25.csv'
csvSalida ='salida.csv'
nDiscret = 25
trasponerONo = 1
imagen = leerCSV(csvEntrada)
n = len(imagen)
nombreArchivoImagenSalida = 'tomo25x25_'

# paresDepuntos = []
# step = max(1,int(0.01 * n)) #cuanto mas bajo ete numero mejor si es 1 pixel va a tirar rayos en todas las lineas
# for x in range(0,n,step):
#     for y in range(0,n,step):
#         par = np.array([0,x,n-1,y])
#         paresDepuntos.append(par)

# paresDepuntos = []
# step = int(n/nDiscret)#max(1,int(0.01 * n)) #cuanto mas bajo ete numero mejor si es 1 pixel va a tirar rayos en todas las lineas
# for x in range(0,n,step):
#     for y in range(0,n,step):
#         par = np.array([0,x,n-1,y])
#         paresDepuntos.append(par)

# paresDepuntos=[]
# step = 1
# for x in range(0,n,step):
#     par = np.array([0,x,n-1,x])
#     paresDepuntos.append(par)
#----------------------------------------------------------------------------
# Genero de las distintas maneras los rayos y para cada uno corro con un conjunto de ruido, y con
# una discretizacion diferente, luego comparamos las calidades de las imagenes.
#----------------------------------------------------------------------------
paresDepuntos = generarPuntosRandom(nDiscret,n)


dibujarImagenCSVint(np.array(paresDepuntos).astype(int),'puntosParaLeer.csv')

rayos = generarMultiplesRayos(paresDepuntos,n)
rayosTransp = np.array([np.transpose(r) for r in rayos])
print(rayosTransp.shape)
if(trasponerONo == 1):
    rayos = np.concatenate((rayos,rayosTransp),axis=0)
dibujarRayos(rayos)

vecECM = []
imagen = leerCSV('./imagenesConv/tomo25x25.csv')
for ruido in range(1,901,50):
    subprocess.call(['./main','-r',str(ruido), '-i' ,csvEntrada, '-s',csvSalida,'tipoRayo',str(4),'transpongoONO',str(trasponerONo),'usoMatrizEsparsaONO',str(0),'-n',str(nDiscret)])
    ecmAprox = errorCuadraticoMedio(imagen,leerCSV(csvSalida),nDiscret)
    subprocess.call(['python3', './conversor_csv/csv_visualizer.py', csvSalida , './Exp5Resultados/'+'random_rec_'+nombreArchivoImagenSalida+'Ruido_'+str(ruido)+'_Ecm_'+str(int(ecmAprox))+'.png'])
    # diffImagenes = toimage(leerCSV(csvSalida)-imagen, cmin=0.0, cmax=255)
    # diffImagenes.save('./Exp5Resultados/'+'Random_diff_' + nombreArchivoImagenSalida +'Ruido_'+str(ruido)+'_Ecm_'+str(ecmAprox)+'.png')
    vecECM.append(ecmAprox)




vecEntradas = [i for i in range(1,901,50)]
plt.plot(vecEntradas, vecECM, 'k-',label = "Rayos Random")
# subprocess.call(['./main','-r',str(ruido), '-i' ,csvEntrada, '-s',csvSalida,'tipoRayo',str(4),'transpongoONO',str(trasponerONo),'usoMatrizEsparsaONO',str(0),'-n',str(nDiscret)])
# subprocess.call(['python3', './conversor_csv/csv_visualizer.py', csvSalida , nombreArchivoImagenSalida])

vecRojo = [2000]*18
vecAmarillo = [1000]*18
vecVerde= [200]*18
plt.plot(vecEntradas,vecRojo,'r--',label="Categoria C")
plt.plot(vecEntradas,vecAmarillo,'y--',label="Categoria B")
plt.plot(vecEntradas,vecVerde,'g--',label="Categoria A")

plt.legend()
plt.xlabel("Ruido")
plt.ylabel("Error Cuadratico Medio")
plt.show()
#-------repito por diseño visual
plt.plot(vecEntradas, vecECM, 'ko')
plt.plot(vecEntradas,vecRojo,'r--',label="Categoria C")
plt.plot(vecEntradas,vecAmarillo,'y--',label="Categoria B")
plt.plot(vecEntradas,vecVerde,'g--',label="Categoria A")

plt.legend()
plt.xlabel("Ruido")
plt.ylabel("Error Cuadratico Medio")
plt.show()

#print('cantidad Rayos:')
#print(len(paresDepuntos))
#print('error cuad medio:')
#imagen = leerCSV('./imagenesConv/tomo25x25.csv')
#print(errorCuadraticoMedio(imagen,leerCSV(csvSalida),nDiscret))

#diffImagenes = toimage((np.ones((len(imagen),len(imagen[0]))).astype(int) * 255 - (leerCSV(csvSalida)-imagen).astype(int)))
#
# ---------------------------------------------------------------------------------------

paresDepuntos = generarPuntosTodos(nDiscret,n)


dibujarImagenCSVint(np.array(paresDepuntos).astype(int),'puntosParaLeer.csv')

rayos = generarMultiplesRayos(paresDepuntos,n)
rayosTransp = np.array([np.transpose(r) for r in rayos])
print(rayosTransp.shape)
if(trasponerONo == 1):
    rayos = np.concatenate((rayos,rayosTransp),axis=0)
dibujarRayos(rayos)

vecECM = []
imagen = leerCSV('./imagenesConv/tomo25x25.csv')
for ruido in range(1,901,50):
    subprocess.call(['./main','-r',str(ruido), '-i' ,csvEntrada, '-s',csvSalida,'tipoRayo',str(4),'transpongoONO',str(trasponerONo),'usoMatrizEsparsaONO',str(0),'-n',str(nDiscret)])
    ecmAprox = errorCuadraticoMedio(imagen,leerCSV(csvSalida),nDiscret)
    subprocess.call(['python3', './conversor_csv/csv_visualizer.py', csvSalida , './Exp5Resultados/'+'Todos_rec_'+nombreArchivoImagenSalida+'Ruido_'+str(ruido)+'_Ecm_'+str(int(ecmAprox))+'.png'])
    diffImagenes = toimage(leerCSV(csvSalida)-imagen, cmin=0.0, cmax=255)
    diffImagenes.save('./Exp5Resultados/'+'Todosdiff_' + nombreArchivoImagenSalida +'Ruido_'+str(ruido)+'_Ecm_'+str(ecmAprox)+'.png')
    vecECM.append(ecmAprox)




vecEntradas = [i for i in range(1,901,50)]
plt.plot(vecEntradas, vecECM, 'k-',label="Rayos Todos")
# subprocess.call(['./main','-r',str(ruido), '-i' ,csvEntrada, '-s',csvSalida,'tipoRayo',str(4),'transpongoONO',str(trasponerONo),'usoMatrizEsparsaONO',str(0),'-n',str(nDiscret)])
# subprocess.call(['python3', './conversor_csv/csv_visualizer.py', csvSalida , nombreArchivoImagenSalida])

vecRojo = [2000]*18
vecAmarillo = [1000]*18
vecVerde= [200]*18
plt.plot(vecEntradas,vecRojo,'r--',label="Categoria C")
plt.plot(vecEntradas,vecAmarillo,'y--',label="Categoria B")
plt.plot(vecEntradas,vecVerde,'g--',label="Categoria A")

plt.legend()
plt.xlabel("Ruido")
plt.ylabel("Error Cuadratico Medio")
plt.show()
#-------repito por diseño visual
plt.plot(vecEntradas, vecECM, 'ko')
plt.plot(vecEntradas,vecRojo,'r--',label="Categoria C")
plt.plot(vecEntradas,vecAmarillo,'y--',label="Categoria B")
plt.plot(vecEntradas,vecVerde,'g--',label="Categoria A")

plt.legend()
plt.xlabel("Ruido")
plt.ylabel("Error Cuadratico Medio")
plt.show()

#print('cantidad Rayos:')
#print(len(paresDepuntos))
#print('error cuad medio:')
#imagen = leerCSV('./imagenesConv/tomo25x25.csv')
#print(errorCuadraticoMedio(imagen,leerCSV(csvSalida),nDiscret))

#diffImagenes = toimage((np.ones((len(imagen),len(imagen[0]))).astype(int) * 255 - (leerCSV(csvSalida)-imagen).astype(int)))



#-------------------------------------------------------------------------




paresDepuntos = generarPuntosRectos(nDiscret,n)


dibujarImagenCSVint(np.array(paresDepuntos).astype(int),'puntosParaLeer.csv')

rayos = generarMultiplesRayos(paresDepuntos,n)
rayosTransp = np.array([np.transpose(r) for r in rayos])
print(rayosTransp.shape)
if(trasponerONo == 1):
    rayos = np.concatenate((rayos,rayosTransp),axis=0)
dibujarRayos(rayos)

vecECM = []
imagen = leerCSV('./imagenesConv/tomo25x25.csv')
for ruido in range(1,901,50):
    subprocess.call(['./main','-r',str(ruido), '-i' ,csvEntrada, '-s',csvSalida,'tipoRayo',str(4),'transpongoONO',str(trasponerONo),'usoMatrizEsparsaONO',str(0),'-n',str(nDiscret)])
    ecmAprox = errorCuadraticoMedio(imagen,leerCSV(csvSalida),nDiscret)
    subprocess.call(['python3', './conversor_csv/csv_visualizer.py', csvSalida , './Exp5Resultados/'+'Rectos_rec_'+nombreArchivoImagenSalida+'Ruido_'+str(ruido)+'_Ecm_'+str(int(ecmAprox))+'.png'])
    diffImagenes = toimage(leerCSV(csvSalida)-imagen, cmin=0.0, cmax=255)
    diffImagenes.save('./Exp5Resultados/'+'Rectosdiff_' + nombreArchivoImagenSalida +'Ruido_'+str(ruido)+'_Ecm_'+str(ecmAprox)+'.png')
    vecECM.append(ecmAprox)




vecEntradas = [i for i in range(1,901,50)]
plt.plot(vecEntradas, vecECM, 'k-',label="Rayos Rectos")
# subprocess.call(['./main','-r',str(ruido), '-i' ,csvEntrada, '-s',csvSalida,'tipoRayo',str(4),'transpongoONO',str(trasponerONo),'usoMatrizEsparsaONO',str(0),'-n',str(nDiscret)])
# subprocess.call(['python3', './conversor_csv/csv_visualizer.py', csvSalida , nombreArchivoImagenSalida])

vecRojo = [2000]*18
vecAmarillo = [1000]*18
vecVerde= [200]*18
plt.plot(vecEntradas,vecRojo,'r--',label="Categoria C")
plt.plot(vecEntradas,vecAmarillo,'y--',label="Categoria B")
plt.plot(vecEntradas,vecVerde,'g--',label="Categoria A")

plt.legend()
plt.xlabel("Ruido")
plt.ylabel("Error Cuadratico Medio")
plt.show()
#-------repito por diseño visual
plt.plot(vecEntradas, vecECM, 'ko')
plt.plot(vecEntradas,vecRojo,'r--',label="Categoria C")
plt.plot(vecEntradas,vecAmarillo,'y--',label="Categoria B")
plt.plot(vecEntradas,vecVerde,'g--',label="Categoria A")

plt.legend()
plt.xlabel("Ruido")
plt.ylabel("Error Cuadratico Medio")
plt.show()

#print('cantidad Rayos:')
#print(len(paresDepuntos))
#print('error cuad medio:')
#imagen = leerCSV('./imagenesConv/tomo25x25.csv')
#print(errorCuadraticoMedio(imagen,leerCSV(csvSalida),nDiscret))

#diffImagenes = toimage((np.ones((len(imagen),len(imagen[0]))).astype(int) * 255 - (leerCSV(csvSalida)-imagen).astype(int)))

#------------------------------------------------------------------------------------



paresDepuntos = generarPuntosCatetosOpuestos(nDiscret,n)


dibujarImagenCSVint(np.array(paresDepuntos).astype(int),'puntosParaLeer.csv')

rayos = generarMultiplesRayos(paresDepuntos,n)
rayosTransp = np.array([np.transpose(r) for r in rayos])
print(rayosTransp.shape)
if(trasponerONo == 1):
    rayos = np.concatenate((rayos,rayosTransp),axis=0)
dibujarRayos(rayos)

vecECM = []
imagen = leerCSV('./imagenesConv/tomo25x25.csv')
for ruido in range(1,901,50):
    subprocess.call(['./main','-r',str(ruido), '-i' ,csvEntrada, '-s',csvSalida,'tipoRayo',str(4),'transpongoONO',str(trasponerONo),'usoMatrizEsparsaONO',str(0),'-n',str(nDiscret)])
    ecmAprox = errorCuadraticoMedio(imagen,leerCSV(csvSalida),nDiscret)
    subprocess.call(['python3', './conversor_csv/csv_visualizer.py', csvSalida , './Exp5Resultados/'+'catetosOp_rec_'+nombreArchivoImagenSalida+'Ruido_'+str(ruido)+'_Ecm_'+str(int(ecmAprox))+'.png'])
    diffImagenes = toimage(leerCSV(csvSalida)-imagen, cmin=0.0, cmax=255)
    diffImagenes.save('./Exp5Resultados/'+'catetosOpuestosdiff_' + nombreArchivoImagenSalida +'Ruido_'+str(ruido)+'_Ecm_'+str(ecmAprox)+'.png')
    vecECM.append(ecmAprox)




vecEntradas = [i for i in range(1,901,50)]
plt.plot(vecEntradas, vecECM, 'k-',label="Catetos Opuestos")
# subprocess.call(['./main','-r',str(ruido), '-i' ,csvEntrada, '-s',csvSalida,'tipoRayo',str(4),'transpongoONO',str(trasponerONo),'usoMatrizEsparsaONO',str(0),'-n',str(nDiscret)])
# subprocess.call(['python3', './conversor_csv/csv_visualizer.py', csvSalida , nombreArchivoImagenSalida])

vecRojo = [2000]*18
vecAmarillo = [1000]*18
vecVerde= [200]*18
plt.plot(vecEntradas,vecRojo,'r--',label="Categoria C")
plt.plot(vecEntradas,vecAmarillo,'y--',label="Categoria B")
plt.plot(vecEntradas,vecVerde,'g--',label="Categoria A")

plt.legend()
plt.xlabel("Ruido")
plt.ylabel("Error Cuadratico Medio")
plt.show()
#-------repito por diseño visual
plt.plot(vecEntradas, vecECM, 'ko')
plt.plot(vecEntradas,vecRojo,'r--',label="Categoria C")
plt.plot(vecEntradas,vecAmarillo,'y--',label="Categoria B")
plt.plot(vecEntradas,vecVerde,'g--',label="Categoria A")

plt.legend()
plt.xlabel("Ruido")
plt.ylabel("Error Cuadratico Medio")
plt.show()

#print('cantidad Rayos:')
#print(len(paresDepuntos))
#print('error cuad medio:')
#imagen = leerCSV('./imagenesConv/tomo25x25.csv')
#print(errorCuadraticoMedio(imagen,leerCSV(csvSalida),nDiscret))

#diffImagenes = toimage((np.ones((len(imagen),len(imagen[0]))).astype(int) * 255 - (leerCSV(csvSalida)-imagen).astype(int)))
