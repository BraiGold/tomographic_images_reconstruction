#ifndef operacionesMatriciales_H
#define operacionesMatriciales_H

#include <vector>
#include <map>
#include <math.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

const double eps = 0.001;

class matrix {

	public:
		matrix();
		matrix(int rows, int columns);
		matrix(const matrix& mat);
		int numfilas() const;
		int numcolumnas() const;
		matrix& operator+(const matrix& mat);
		matrix& operator-(const matrix& mat);
		matrix operator*(const matrix& mat);
		matrix& operator*(double num);
		vector < double > operator*(const vector<double>& v);
		matrix& operator=(const matrix& mat);
		matrix transponer();
		vector<double> resolverConGauss(vector<double> terminoInd);
		void eliminacionGaussianaSinPivoteo(vector < double > &terminoInd);
		void eliminacionGaussiana(vector < double > &terminoInd);
		vector < double > sustitucionParaAtras(const vector < double > &b); //para resolver sistema
		vector<double>& operator[] (int i);
		const vector<double>& operator[] (int i) const;
		void print_matrix();

	private:
		vector < vector < double > > _matrix;
};

class matrixEsparsa {
	public:
		matrixEsparsa();
		matrixEsparsa(int rows, int columns);
		matrixEsparsa(const matrixEsparsa &mat);
		int numfilas() const;
		int numcolumnas() const;
		matrixEsparsa &operator+(const matrixEsparsa &mat);
		matrixEsparsa &operator-(const matrixEsparsa &mat);
		matrixEsparsa operator*(const matrixEsparsa &mat);
		matrixEsparsa &operator*(double num);
		vector<double> operator*(const vector<double> &v);
		matrixEsparsa &operator=(const matrixEsparsa &mat);
		matrixEsparsa transponer();
		double get(const int x, const int y) const;
		void set(const int x, const int y, double elem);
		vector<double> resolverConGauss(vector<double> terminoInd);
		void eliminacionGaussianaSinPivoteo(vector<double> &terminoInd);
		void eliminacionGaussiana(vector<double> &terminoInd);
		vector<double> sustitucionParaAtras(const vector<double> &b); //para resolver sistema
		void print_matrixEsparsa();
		double multiplicarFilaPorColumna(int fil, const matrixEsparsa &mat, int col);

	  private:
		int col;
		int row;
		vector < map < int, double > > filas;
		vector < map < int, double > > columnas;
};

bool eq(double a, double b);
void print_vector(const vector<double> &vec);
matrix prodVecColxVecFila(const vector<double> &v, const vector<double> &w);
double prodInterno(const vector<double> &v, const vector<double> &w);
void divisorVectorNum(vector<double> &vec, const double d);
bool esBaseOrtonormal(const vector < vector < double > > &vectores);
matrixEsparsa prodVecColxVecFilaEsparsa(const vector<double> &v, const vector<double> &w);
void sumarFilas(int i, double k, const matrixEsparsa &mat, int j);

#endif
