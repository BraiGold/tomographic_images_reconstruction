#ifndef __inputOutput_H__
#define __inputOutput_H__

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include "operacionesMatriciales.h"

using namespace std;

typedef unsigned char uchar;

typedef
enum _PPM_LOADER_PIXEL_TYPE {
	PPM_LOADER_PIXEL_TYPE_INVALID = -1,
	PPM_LOADER_PIXEL_TYPE_RGB_8B = 0,
	PPM_LOADER_PIXEL_TYPE_RGB_16B = ( PPM_LOADER_PIXEL_TYPE_RGB_8B + 1 ),
	PPM_LOADER_PIXEL_TYPE_RGB_32B = ( PPM_LOADER_PIXEL_TYPE_RGB_16B + 1 ),
	PPM_LOADER_PIXEL_TYPE_RGB_64B = ( PPM_LOADER_PIXEL_TYPE_RGB_32B + 1 ),
	PPM_LOADER_PIXEL_TYPE_GRAY_8B = ( PPM_LOADER_PIXEL_TYPE_RGB_64B + 1 ),
	PPM_LOADER_PIXEL_TYPE_GRAY_16B = ( PPM_LOADER_PIXEL_TYPE_GRAY_8B + 1 ),
	PPM_LOADER_PIXEL_TYPE_GRAY_32B = ( PPM_LOADER_PIXEL_TYPE_GRAY_16B + 1 ),
	PPM_LOADER_PIXEL_TYPE_GRAY_64B = ( PPM_LOADER_PIXEL_TYPE_GRAY_32B + 1 )
} PPM_LOADER_PIXEL_TYPE;

bool LoadPPMFile(uchar** data, int *width, int *height, PPM_LOADER_PIXEL_TYPE* pt, const char *filename);
bool SavePPMFile(const char *filename, const void *src, int width, int height, PPM_LOADER_PIXEL_TYPE pt, const char* comments = NULL);
matrix read_image(std::string filename, int *width, int *height);
int get_pixel_pgm(uchar *data, int i, int j, int height, int width);
vector < string > split(const char* sentence, char separador);
void construirCSV(vector<double>& reconstruccion, string imgSalida, int n_discretizacion);
void construirCSV2(matrix m,string imgSalida);
void construirCSV3(vector <double >tiempos,string imgSalida,int n);

#endif //__PPMLOADER_H__
