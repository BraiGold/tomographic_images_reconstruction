#include <iostream>
#include <cstdio>
#include "inputOutput.h"
#include "simulacionRayos.h"
#include "operacionesMatriciales.h"
#include <vector>
#include <time.h>
#include <iomanip>

using namespace std;

// Para compilar: g++ operacionesMatriciales.cpp inputOutput.cpp simulacionRayos.cpp main.cpp -o NOMBRE_DE_EJECUTABLE
// formato de salida; ./NOMBRE_DE_EJECUTABLE -r  0.001 -i prueba.csv -s salida.csv -t 1 -q 0 -e 0
// -r es el ruido
// -i es la imagen de entrada
// -s es la imagen de salida
// -t es la estrategia de lanzamiento (1 es rayos random, 2 es rayos canonicos, 3 es rayos que hicimos en python, 4 es rayos desde csv)
// si se elije 1, la cantidad de rayos a lanzar debe ser > n*n (n de la discretizacion)
// si se elije 3, la cantidad de rayos no importa importa ingrese algun numero
// si se elije 4, la cantidad de rayos es igual a la cantidad de pares de puntos del csv. IMPORTATE: el csv a leer ponerlo en la func darRayos case 4
// -q es si quiere o no trasponer. 1 es trasponer rayos y 0 es no trasponer rayos
// -e es si quiere o no usar matriz esparsa. 1 es usar matriz esparsa y 0 es no usar matriz esparsa
int main(int argc, char** argv){


  char* rString = argv[2];
	double r;
	sscanf(rString,"%lf",&r);
  uchar* data = NULL;
  int width = 0, height = 0;
  string imgEntrada(argv[4]);
  matrix imagen = read_image(imgEntrada, &width, &height);

  //imagen.print_matrix();

 /* ofstream out("matrix8x8.csv");

  for(int y=60; y<68; y++){
    for(int x=90; x<98; x++){
      out << imagen[y][x] << ",";
    }
    out << endl;
  }*/

  string imgSalida(argv[6]);

  int minimoHeightWidth = min(height, width);

  // int n;
  // cout << "Escriba el n, que tiene que ser divisor de las filas o columnas."  << endl;
  // cin >> n;
  char* nString = argv[14];
  int n;
  sscanf(nString, "%d", &n);

  char* tipRayo = argv[8];
  int tipoRayos;
  sscanf(tipRayo, "%d", &tipoRayos);

  char* trasponerString = argv[10];
  int trasponer;
  sscanf(trasponerString, "%d", &trasponer);

  char* esparsaString = argv[12];
  int esparsa;
  sscanf(esparsaString, "%d", &esparsa);

  bool traspongo;
  if ( trasponer == 0) {
    traspongo = false;
  }else{
	traspongo = true;
  }

  int m;
  if (tipoRayos == 1) {
    cout << "Escriba la cantidad de rayos, que tiene que ser mayor a nxn." << endl;
    cin >> m;
  }

  vector < vector <int> > puntos /*= generarPuntosMagicos(minimoHeightWidth)*/;
  //vector < vector <int> > puntos = generoPuntosCanonicos(minimoHeightWidth);
  darRayos(tipoRayos, traspongo, m, puntos, minimoHeightWidth, n);
  int start = clock();
  if (esparsa == 0) {
    matrix distancias(m, n * n);
    cout << "imprimo m " << m << endl;
    vector<double> tiempos(m, 0.0);
    simularRayos(tipoRayos, m, imagen, n, r, minimoHeightWidth, distancias, tiempos, puntos);

    //distancias.print_matrix();
    //cout << "ARRIBA ESTA LA MATRIZ DESP DE RAYOS" << endl;
    int startCML = clock();
    vector<double> reconstruccion = cuadradosMinimosLineales(distancias, tiempos);
      //  imprimo tiempos para EXPERIMENTACION
    double difClocks=clock() - startCML;
    double difSegundos=(difClocks)/(CLOCKS_PER_SEC);
    ofstream tiempoTardadoCML;
    tiempoTardadoCML.open("./archivosGenerados/tiempoEjecucionCML.txt");
    tiempoTardadoCML << std::setprecision(9) << difSegundos;
    tiempoTardadoCML.close();
    //termino la impresion de tiempos para EXPERIMENTACION
    construirCSV(reconstruccion, imgSalida, n);

    double difClocksTotal=clock() - start;
    double difSegundosTotal=(difClocks)/(CLOCKS_PER_SEC);
    ofstream tiempoTardado;
    tiempoTardado.open("./archivosGenerados/tiempoEjecucionTotal.txt");
    tiempoTardado << std::setprecision(15) << difSegundosTotal;
    tiempoTardado.close();
  } else {
    matrixEsparsa distancias(m, n * n);
    cout << "imprimo m " << m << endl;
    vector<double> tiempos(m, 0.0);

    simularRayos(tipoRayos, m, imagen, n, r, minimoHeightWidth, distancias, tiempos, puntos);
    //distancias.print_matrix();
    //cout << "ARRIBA ESTA LA MATRIZ DESP DE RAYOS" << endl;
     //int start = clock();
     int startCML = clock();
    vector<double> reconstruccion = cuadradosMinimosLineales(distancias, tiempos);
    double difClocks=clock() - startCML;
    double difSegundos=(difClocks)/(CLOCKS_PER_SEC);
    ofstream tiempoTardadoCML;
    tiempoTardadoCML.open("./archivosGenerados/tiempoEjecucionCML.txt");
    tiempoTardadoCML << std::setprecision(15) << difSegundos;
    tiempoTardadoCML.close();
    //imprimo tiempos para EXPERIMENTACION

    construirCSV(reconstruccion, imgSalida, n);
    double difClocksTotal=clock() - start;
    double difSegundosTotal=(difClocks)/(CLOCKS_PER_SEC);
    ofstream tiempoTardado;
    tiempoTardado.open("./archivosGenerados/tiempoEjecucionTotal.txt");
    tiempoTardado << std::setprecision(15) << difSegundosTotal;
    tiempoTardado.close();
  }

  return 0;
}
