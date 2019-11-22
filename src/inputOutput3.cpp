#include "inputOutput.h"

/*
 * src = Image pointer
 * pt  = Image type
 *			PPM_LOADER_PIXEL_TYPE_RGB_8B:  Image is organized by rows with colors interleaved
 *			PPM_LOADER_PIXEL_TYPE_GRAY_XB: Image is organized by rows with no colors and X bits per pixel
*/
bool SavePPMFile(const char *filename, const void *src, int width, int height, PPM_LOADER_PIXEL_TYPE pt, const char* comments)
{
	if (filename == NULL || src == NULL)
	{
		return false;
	}

	FILE* fid = fopen(filename,"wb");
	if(!fid){
		printf("ERROR opening file %s\n",filename);
		return false;
	}
	int pix_size = 0;
	switch( pt )
	{
		case PPM_LOADER_PIXEL_TYPE_RGB_8B:
			fprintf(fid, "P6\n");
			if (comments)
			{
				fprintf(fid,"#%s\n",comments);
			}
			fprintf(fid, "%i %i\n%i\n", width, height, 255);
			pix_size = 3; // 3 bytes
			break;
		case PPM_LOADER_PIXEL_TYPE_RGB_16B:
			fprintf(fid, "P6\n");
			if (comments)
			{
				fprintf(fid,"#%s\n",comments);
			}
			fprintf(fid, "%i %i\n%i\n", width, height, 65535);
			pix_size = 3*2; // 6 bytes
			break;
		case PPM_LOADER_PIXEL_TYPE_RGB_32B:
			fprintf(fid, "P6\n");
			if (comments)
			{
				fprintf(fid,"#%s\n",comments);
			}
			fprintf(fid, "%i %i\n%li\n", width, height, 4294967295);
			pix_size = 3*4; // 12 bytes
			break;
		case PPM_LOADER_PIXEL_TYPE_RGB_64B:
			fprintf(fid, "P6\n");
			if (comments)
			{
				fprintf(fid,"#%s\n",comments);
			}
			fprintf(fid, "%i %i\n%li\n", width, height, 18446744073709551615U);
			pix_size = 3*8; // 24 bytes
			break;

		case PPM_LOADER_PIXEL_TYPE_GRAY_8B:
			fprintf(fid, "P5\n");
			if (comments)
			{
				fprintf(fid,"#%s\n",comments);
			}
			fprintf(fid, "%i %i\n%i\n", width, height, 255);
			pix_size = 1; // 1 bytes
			break;
		case PPM_LOADER_PIXEL_TYPE_GRAY_16B:
			fprintf(fid, "P5\n");
			if (comments)
			{
				fprintf(fid,"#%s\n",comments);
			}
			fprintf(fid, "%i %i\n%i\n", width, height, 65535);
			pix_size = 2; // 2 bytes
			break;
		case PPM_LOADER_PIXEL_TYPE_GRAY_32B:
			fprintf(fid, "P5\n");
			if (comments)
			{
				fprintf(fid,"#%s\n",comments);
			}
			fprintf(fid, "%i %i\n%li\n", width, height, 4294967295);
			pix_size = 4; // 4 bytes
			break;
		case PPM_LOADER_PIXEL_TYPE_GRAY_64B:
			fprintf(fid, "P5\n");
			if (comments)
			{
				fprintf(fid,"#%s\n",comments);
			}
			fprintf(fid, "%i %i\n%li\n", width, height, 18446744073709551615U);
			pix_size = 8; // 8 bytes
			break;
		case PPM_LOADER_PIXEL_TYPE_INVALID:
			printf("ERROR invalid PPM_LOADER_PIXEL_TYPE\n");
			return false;
			break;
		default:
			printf("ERROR unrecognized PPM_LOADER_PIXEL_TYPE\n");
			return false;
			break;
	}
	int bytes_written = fwrite(src,1,width*height*pix_size,fid);
	bool ret = bytes_written == width*height*pix_size;

	fclose(fid);

	return ret;
}


// Remember to free data
bool LoadPPMFile(uchar** data, int *width, int *height, PPM_LOADER_PIXEL_TYPE* pt, const char *filename)
{
	if (data == NULL || width== NULL || height == NULL || pt == NULL || filename == NULL)
	{
		return false;
	}
	// Open file
	FILE* fid = fopen(filename, "rb");
	if (!fid) {
		printf("PPM load error: file access denied %s\n", filename);
		return false;
	}
	// Read PPM/PGM header P5/P6
	int channels = 0;
	char line[256];
  char *ptr;
	fgets(line, 256, fid);

	if ( line[0]=='P' && line[1]=='5' ) {
		channels = 1;
	} else if ( line[0]=='P' && line[1]=='6' ) {
		channels = 3;
	} else {
		printf("Wrong image type\n");
		return false;
	}

  if (line[2] == '\n') {
//    fgets(line, 256, fid);

    // Parse comments
    fgets(line, 256, fid);
    while(line[0]=='#')
      fgets(line, 256, fid);

    ptr = line;
  } else
    ptr = line + 3;

  // Read dimensions
  *width = strtol(ptr, &ptr, 10);
  *height = strtol(ptr, &ptr, 10);
  if(ptr == NULL || *ptr == '\n'){
    fgets(line, 256, fid);
    ptr = line;
  }

	// Read pixel depth
  int levels = strtol(ptr, &ptr, 10);
  int pixel_depth = 0;
	if (channels==3 && (levels == 255)) {
		*pt = PPM_LOADER_PIXEL_TYPE_RGB_8B;
		pixel_depth = 1;
	}else if (channels==3 && levels == 65535) {
		*pt = PPM_LOADER_PIXEL_TYPE_RGB_16B;
		pixel_depth = 2;
	} else if (channels==3 && levels == 4294967295) {
		*pt = PPM_LOADER_PIXEL_TYPE_RGB_32B;
		pixel_depth = 4;
	} else if (channels==3 && levels == 18446744073709551615U) {
		*pt = PPM_LOADER_PIXEL_TYPE_RGB_64B;
		pixel_depth = 8;
	}else if (levels == 255) {
		*pt = PPM_LOADER_PIXEL_TYPE_GRAY_8B;
		pixel_depth = 1;
	}else if (levels == 65535) {
		*pt = PPM_LOADER_PIXEL_TYPE_GRAY_16B;
		pixel_depth = 2;
	} else if (levels == 4294967295) {
		*pt = PPM_LOADER_PIXEL_TYPE_GRAY_32B;
		pixel_depth = 4;
	} else if (levels == 18446744073709551615U) {
		*pt = PPM_LOADER_PIXEL_TYPE_GRAY_64B;
		pixel_depth = 8;
	} else {
		printf("ERROR: Wrong number of levels\n");
		return false;
	}

	// Read raw data from file
	int size = (*width) * (*height) * channels * pixel_depth;
	*data = new uchar[size];
	fread(*data, 1, size, fid);
	fclose(fid);

	return true;
}

vector < string > split(const char* sentence, char separador) {
	vector < string > result;

    do
    {
        const char *begin = sentence;

        while(*sentence != separador && *sentence)
            sentence++;

        result.push_back(string(begin, sentence));
    } while (0 != *sentence++);

    return result;
}

matrix read_image(string filename, int *width, int *height)
{
	ifstream file(filename.c_str());
	string value;
	int i, j;
	if (file.is_open()) {
		while (file.good()) {
			i = 0;
			while (getline( file, value )) {
				j = split( value.c_str(), ',' ).size();
				i++;
			}
		}
		file.close();
	}
	cout << "height: " << i << " width: " << j << endl;
	matrix imagen(i, j);
	*height = i;
	*width = j;

	file.open(filename.c_str());
	if (file.is_open()) {
		while (file.good()) {
			i = 0;
			while (getline( file, value )) {
				j = 0;
				vector < string > numbers = split( value.c_str(), ',' );
				while ( j < numbers.size() ) {
					if (atoi(numbers[j].c_str()) < 0) {
						imagen[i][j] = 0.0;
					} else {
						imagen[i][j] = atoi(numbers[j].c_str());
					}
					j++;
				}
				i++;
			}
		}
		file.close();
	}

	return imagen;
}

// REVISAR COMO OBTENER PIXEL DE ARCHIVOS DICOM
int get_pixel_pgm(uchar *data, int i, int j, int height, int width)
{
	if (i > height)
		throw std::runtime_error("El direccionamiento vertical no puede ser mayor a la altura.");
	if (j > width)
		throw std::runtime_error("El direccionamiento horizontal no puede ser mayor al ancho.");
	return data[i * width + j];
}

void construirCSV(vector<double>& reconstruccion, string imgSalida, int n_discretizacion) {
	//print_vector(reconstruccion);
	ofstream res;
	res.open(imgSalida.c_str());
	for (int i = 0; i < n_discretizacion; i++) {
		for (int j = 0; j < n_discretizacion; j++) {
			res << (int)reconstruccion[i*n_discretizacion + j];
			 if (j < n_discretizacion - 1) {
			 	 res << ",";
			 }
		}

			res << "\n";
		
	}

	res.close();
}



void construirCSV2(matrix m,string imgSalida) {
	//print_vector(reconstruccion);
	ofstream res;
	res.open(imgSalida.c_str());
	for (int i = 0; i < m.numfilas(); i++) {
		for (int j = 0; j < m.numcolumnas(); j++) {
			res << (int)m[i][j];
			 if (j < m[i].size() - 1) {
			 	 res << ",";
			 }
		}
		res << "\n";
	}

	res.close();
}


void construirCSV3(vector <double >tiempos,string imgSalida,int n){
	ofstream res;
	res.open(imgSalida.c_str());
	for (int i = 0; i < n; i++) {
		res << (int)tiempos[i];
		 if (i < n - 1) {
			 res << ",";
		 }
		 //res << "\n";

	}


}
