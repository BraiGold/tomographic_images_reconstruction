# Analisis de metodos para reconstruccion de imagenes tomograficas

Trabajo practico 3 de metodos Numericos
Se comparan diferentes metodos numericos de reconstruccion de imagenes a partir de intensidad de rayos. Se estudia tambien diferentes disposiciones de estos rayos y su densidad sugerida.
Se implementaro usando librerias de algebra lineal en python y tambien sin librerias en c++.

para mas detalle ver el [INFORME](https://gitlab.com/atun/xrays-metodosnumericos/blob/master/tp3-metnum.pdf)



detalles para ejecucion:
Para compilar desde /src/   :
make

para ejecutar:
./main -r ruido -i csvEntrada -s csvSalida -tipoRayo num1al4 -transpongoONO num1o0Tras -usoMatrizEsparsaONO num1o0Rala -n nDiscret

ruido: numero mayor a 0 que indica el maximo valor de ruido posible
csvEntrada:  imagen de entrada (en formato csv)
csvSalida: imagen resultados (en formato csv)
num1al4: Que configuracion de rayo se quiere usar (1-random) (2-rectos) (3-catetos opuestos) (4-enviados por csv)
num1o0Tras: 1 para trasponer los rayos, 0 para no trasponerlos
num1o0rRala: 1 para usar matriz rala 0 para usar la de vector<vector<double> >
nDiscret: n de la discretizacion

Para ejecutar experimentos:
ejecutarlos desde la carpeta /src/

el experimento 1 se realiza a partir de la ejecucion del exp2 experimento2.py

el experimento 2 se realiza con experimento2.py

el experimento 3 se realiza la primera parte con experimento3.py y la segunda (tiempos en fun de rayos) con experimento6.py (leer las primeras lineas las instrucciones de como utilizarlo para el exp3 y como para el exp5)

el experimento 4 se realiza con experimento4.py , si se quiere que se dibujen las disposiciones de rayos deben descomentarse esas lineas, pero tenerlas comentadas ayuda a la eficiencia de memoria en particular para imagenes grandes.

el experimento 5 se realiza con experimento6.py (parte 2, leer las primeras lineas para saber como utilizarlo para el exp5 y como para el 3)

el experimento 6 se realiza con experimento7.py

disculpen el cambio de nombres(numeros) en los exp  pero escribiendo el informe no parecio mas natural que los experiementos se muestren de esa forma si bien no se desarrollaron en ese orden.
