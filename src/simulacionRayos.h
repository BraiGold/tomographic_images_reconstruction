#ifndef __simulacionRayos_H__
#define __simulacionRayos_H__

#include <stdio.h>  /* printf, scanf, puts, NULL */
#include <stdlib.h> /* srand, rand */
#include <time.h>   /* time */
#include <vector>
#include <iostream>
#include "operacionesMatriciales.h"
#include "inputOutput.h"

typedef unsigned char uchar;

using namespace std;

vector < double > cuadradosMinimosLineales(matrix &matrizDistancias, vector < double > &tiempos);
vector < double > cuadradosMinimosLineales(matrixEsparsa &matrizDistancias, vector<double> &tiempos);
void simularRayos(int tipoRayos, int cantidadRayos, matrix& data, int cantidadCeldas, double ruido, int minimoHW, matrix &matrizDistancias, vector < double > &tiempos, vector < vector < int > >& puntos);
void simularRayos(int tipoRayos, int cantidadRayos, matrix &data, int cantidadCeldas, double ruido, int minimoHW, matrixEsparsa &matrizDistancias, vector<double> &tiempos, vector < vector < int > > &puntos);
vector < vector < int > > generarPuntosDesdeCSV(string nombreArchivo);
vector < vector <int> > damePuntos(int p11, int p12, int p21, int p22, int n);
vector < vector <int> > generarPuntosMagicos(int minimoHW);
vector< vector < int > > generoPuntosCanonicos(int minimoHW);
vector < vector < int > > damePuntos2(int x1,int y1, int x2,int y2);
void darRayos(int tipoRayos, bool traspongo, int &cantidadRayos, vector < vector < int > > &puntos, int minimoHW, int n);

#endif
