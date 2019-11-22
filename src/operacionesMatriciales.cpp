#include "operacionesMatriciales.h"

matrix::matrix() {
	cout << "constr sin parametros" << endl;
	this->_matrix = (vector < vector < double > > ());
}

matrix::matrix(int rows, int columns) {
	vector < double > ancho(columns, 0.0);
	vector < vector < double > > informacion(rows, ancho);
	(this->_matrix) = informacion;
}

matrix::matrix(const matrix& mat) {
	vector < double > ancho(mat.numcolumnas(), 0);
	vector < vector < double > > informacion(mat.numfilas(), ancho);
	(this->_matrix) = informacion;
	for (int i = 0; i < mat.numfilas(); i++) {
		for (int j = 0; j < mat.numcolumnas(); j++) {
			((*this)._matrix)[i][j] = (mat._matrix)[i][j];
		}
	}
}

int matrix::numfilas() const {
	return (this->_matrix).size();
}

int matrix::numcolumnas() const {
	if(this->numfilas() == 0){
		return 0;
	}else{
		return (this->_matrix)[0].size();
	}
}

//asumo que son del mismo tamaño
matrix& matrix::operator+(const matrix& mat) {
	for (int i = 0; i < mat.numfilas(); i++) {
		for (int j = 0; j < mat.numcolumnas(); j++) {
			((*this)._matrix)[i][j] += (mat._matrix)[i][j];
		}
	}

	return (*this);
}

//asumo que son del mismo tamaño
matrix& matrix::operator-(const matrix& mat) {
	for (int i = 0; i < mat.numfilas(); i++) {
		for (int j = 0; j < mat.numcolumnas(); j++) {
			((*this)._matrix)[i][j] -= (mat._matrix)[i][j];
		}
	}

	return (*this);
}

matrix matrix::operator*(const matrix& mat) {
	matrix res(numfilas(), mat.numcolumnas());
	for(int i = 0; i < numfilas(); i++){
		for(int j = 0; j < mat.numcolumnas(); j++){
			for (int k = 0; k < numcolumnas(); k++) {
				(res._matrix)[i][j] += ((*this)._matrix)[i][k]*(mat._matrix)[k][j];
			}
		}
		cout << "multiplique la fila i: " << i << endl;
	}

	return res;
}

matrix& matrix::operator*(double num){
	for(int i = 0; i < numfilas(); i++){
		for (int j = 0; j < numcolumnas(); j++) {
			(*this)._matrix[i][j] = (*this)._matrix[i][j]*num;
		}
	}
	return (*this);
}

matrix& matrix::operator=(const matrix& mat) {
	cout << "estoy usando la asignacion"<<endl;
	//cout << mat.numcolumnas() << " " << mat.numfilas() << " ";

	if (this != &mat) {
		vector < double > ancho(mat.numcolumnas(), 0);
		vector < vector < double > > informacion(mat.numfilas(), ancho);
		(this->_matrix) = informacion;

		for (int i = 0; i < mat.numfilas(); i++) {
			for (int j = 0; j < mat.numcolumnas(); j++) {
				((*this)._matrix)[i][j] = (mat._matrix)[i][j];
			}
		}
	}
	//cout << "termine la asignacion"<<endl;

	return (*this);
}

//asumo que la entrada no es la matriz nula
matrix matrix::transponer() {
	//cout << "entre a trasponer" << endl;
	int row = (this->_matrix)[0].size();
	int col = (this->_matrix).size();
	//cout << "pepe: " << row << " etet: " << col << endl;
	matrix res(row, col);
	//cout << "antes de for" << endl;
	for (int i = 0; i < (this->_matrix).size(); i++) {
		//cout << "estoy1" << endl;
		for (int j = 0; j < this->numcolumnas(); j++) {
			//cout << "estoy2" << endl;
			res._matrix[j][i] = (this)->_matrix[i][j];
			//cout << "estoy3" << endl;
		}
		//cout << "estoy4" << endl;
	}
	//cout << "termine trasponer" << endl;
	return res;
}

void matrix::eliminacionGaussianaSinPivoteo(vector<double> &terminoInd) {
	for (int i = 0; i < (*this).numfilas(); i++) {
		for (int j = i + 1; j < (*this).numfilas(); j++) {
			if (!eq((*this)._matrix[j][i], 0)) {
				double k = (*this)._matrix[j][i] / (*this)._matrix[i][i];
				//a la fila j le resto k por fila i
				for (int col = i; col < (*this).numcolumnas(); col++) {//resto filas
					(*this)._matrix[j][col] = (*this)._matrix[j][col] - k*(*this)._matrix[i][col];
				}
				terminoInd[j] = (terminoInd[j] - k*terminoInd[i]);//resto los numeros a la derecha del igual
			}
		}
	}
}


vector<double> matrix::resolverConGauss(vector<double> terminoInd) {
	int n = this->numfilas();
	vector<double> linea(n+1,0);
	vector< vector<double> > A(n,linea);

	for (int i=0; i<n; i++) {
			 for (int j=0; j<n; j++) {
					 A[i][j] = (this)->_matrix[i][j] ;
			 }
	 }

	 for (int i=0; i<n; i++) {
			 A[i][n] = terminoInd[i] ;
	 }


    for (int i=0; i<n; i++) {
        double elemMax = abs(A[i][i]);
        int filaMax = i;
        for (int k=i+1; k<n; k++) {
            if (abs(A[k][i]) > elemMax) {
                elemMax = abs(A[k][i]);
                filaMax = k;
            }
        }

        for (int k=i; k<n+1;k++) {
            double temp = A[filaMax][k];
            A[filaMax][k] = A[i][k];
            A[i][k] = temp;
        }

        for (int k=i+1; k<n; k++) {
            double c = -A[k][i]/A[i][i];
            for (int j=i; j<n+1; j++) {
                if (i==j) {
                    A[k][j] = 0;
                } else {
                    A[k][j] += c * A[i][j];
                }
            }
        }
    }

    vector<double> x(n);
    for (int i=n-1; i>=0; i--) {
        x[i] = A[i][n]/A[i][i];
        for (int k=i-1;k>=0; k--) {
            A[k][n] -= A[k][i] * x[i];
        }
    }
		cout << A[n-1][n-1]<<endl;
    return x;
}



void matrix::eliminacionGaussiana(vector<double> &terminoInd) {
	int h = 0;
	int k = 0;
	while (h < (*this).numfilas() && k < (*this).numcolumnas()) {
	  	/* Encontrar el k-pivote: */
		int i_max = h;
		double elem_max = this->_matrix[h][k];
		for (int p = h + 1; p < this->numfilas(); p++) {
			double elem_comparar = this->_matrix[p][k];
			if (fabs(elem_comparar) > fabs(elem_max)) {
				elem_max = elem_comparar;
				i_max = p;
			}
		}
		if (eq(this->_matrix[i_max][k], 0)) {
			/* No hay pivote en esta columna, paso a la siguiente */
			k++;
		} else {
			//swapRows(h, i_max);
			vector < double > guardar = this->_matrix[h];
			this->_matrix[h] = this->_matrix[i_max];
			this->_matrix[i_max] = guardar;
			/* Hacer para todas las filas debajo: */
			for (int i = h + 1; i < this->numfilas(); i++) {
				if (!(eq(this->_matrix[i][k], 0))) {
					double f = this->_matrix[i][k] / this->_matrix[h][k];
					/* Llenar con ceros la parte baja de la columna: */
					this->_matrix[i][k] = 0.0;
					/* Los elementos restantes de la matriz: */
					for (int j = k + 1; j < this->numcolumnas(); j++) {
					   double elem = this->_matrix[i][j];
					   double elemARestar = (this->_matrix[h][j] * f);
					   //cout << "elem: " << elem << " elemARestar: " << elemARestar << endl;
					   this->_matrix[i][j] = elem - elemARestar;

					}
					terminoInd[i] = terminoInd[i] - f*terminoInd[h];
				}
			}
				//veo como quedo hasta ahora la matriz
				/*cout << "hasta ahora: " << endl;
				for (int i = 0; i < this->numfilas(); i++) {
					for (int j = 0; j < this->numcolumnas(); j++) {
						cout << this->get(i, j) << "  ";
					}
					cout << endl;
				}*/
			/* incrementar pivote columna y fila */
			h++;
			k++;
		}
	}
}

vector < double > matrix::sustitucionParaAtras(const vector < double > &b) {
	//supongo  es matriz triangular superior
	vector < double > res((*this).numcolumnas(), 0);
	double suma = 0;
	for (int i = (*this).numfilas() - 1; i >= 0; i--) {
		cout << "matrix[" << i << "][" << i << "]:" << this->_matrix[i][i] << endl;
		if (eq(this->_matrix[i][i], 0)) throw runtime_error("encontre un cero!! estas haciendo las cosas mal");
		suma = 0;
  	double ultimoDeFila = this->_matrix[i][this->numcolumnas() - 1];
  	int j = this->numcolumnas() - 1;
		while (eq(ultimoDeFila, 0) || j > i) {
			suma += this->_matrix[i][j] * res[j];
			j--;
			ultimoDeFila = this->_matrix[i][j];
		}
		res[i] = (b[i] - suma) / (this->_matrix)[i][i];
	}

	return res;
}

vector<double>& matrix::operator[] (int i){
	return this->_matrix[i];
}

const vector<double>& matrix::operator[] (int i) const{
	return this->_matrix[i];
}

void print_vector(const vector<double>& vec) {
	for (int i = 0; i < vec.size(); i++) {
		cout << vec[i] << " ";
	}
	cout << endl;
}

void matrix::print_matrix() {
	for (int i = 0; i < this->numfilas(); i++) {
		for (int j = 0; j < this->numcolumnas(); j++) {
			cout << this->_matrix[i][j] << "  ";
		}
		cout << endl;
	}
}

vector < double > matrix::operator*(const vector<double>& v){
	vector<double> res(numfilas(), 0);

	for(int i=0; i<numfilas(); i++){
		res[i]=prodInterno((this->_matrix)[i], v);
	}

	return res;
}

//asumo que se puede hacer el producto entre vectores de nx1 y 1xn
matrix prodVecColxVecFila(const vector<double>& v, const vector<double>& w){
	matrix V(v.size(), 1);
	matrix W(1, w.size());

	for(int i=0; i<v.size(); i++){//paso v a matriz
		V[i][0]=v[i];
	}


	for(int i=0; i<v.size(); i++){//paso w a matriz
		W[0][i]=w[i];
	}


	matrix res(V*W);

	return res;
}

void divisorVectorNum(vector < double > &vec, const double d) {
	for (int i = 0; i < vec.size(); i++) {
		vec[i] = vec[i] / d;
	}
}

//asumo que se puede hacer el prod interno entre v y w
double prodInterno(const vector < double > & v, const vector < double > & w){
	double res=0.0;

	for(int i=0; i<v.size(); i++){
		res+=v[i]*w[i];
	}

	return res;
}

bool esBaseOrtonormal(const vector < vector <double> > &vectores){
	bool esBaseOrt=true;
	for(int k=0; k<vectores.size(); k++){
		if( !eq(prodInterno(vectores[k], vectores[k]), 1.0) ){
			esBaseOrt &= false;
			cout << "el prod inter de un vec por si mismo no es 1" << endl;
		}
	}

	for(int i=0; i<vectores.size(); i++){
		for(int j=i+1; j<vectores.size(); j++){
			if( !eq(prodInterno(vectores[i], vectores[j]), 0.0) ){
				esBaseOrt &= false;
				cout << "el prod inter de un vec por otro distinto no es 0" << endl;
			}
		}
	}
	return esBaseOrt;
}

//operaciones matriz esparsa


matrixEsparsa::matrixEsparsa() : row(0), col(0), filas(0), columnas(0)
{
}

matrixEsparsa::matrixEsparsa(int rows, int columns) : filas(rows, map<int, double>()), columnas(columns, map<int, double>())
{
	this->col = columns;
	this->row = rows;
}

double matrixEsparsa::get(const int x, const int y) const
{
	if (this->filas[x].find(y) != this->filas[x].end())
	{
		return (this->filas)[x].at(y);
	}
	else
	{
		return 0;
	}
}

void matrixEsparsa::set(const int x, const int y, double elem)
{
	if (fabs(elem) > eps)
	{ //entra elem distinto a 0
		(this->filas[x])[y] = elem;
		(this->columnas[y])[x] = elem;
	}
	else if (this->filas[x].find(y) != this->filas[x].end())
	{ //en x,y hay algo distinto a 0 y elem es 0
		(this->filas[x]).erase(y);
		(this->columnas[y]).erase(x);
	}
}

matrixEsparsa::matrixEsparsa(const matrixEsparsa &mat) : row(mat.numfilas()), col(mat.numcolumnas()), filas(row, map<int, double>()), columnas(col, map<int, double>())
{
	//cout << "estoy usando constr. x copia" << endl;
	//cout << mat.numcolumnas() << " " << mat.numfilas()<<" pepito"<<endl;
	//cout << numcolumnas() << " " << numfilas()<<" pepito"<<endl;
	//this->col = mat.numcolumnas();
	//this->row = mat.numfilas();
	for (int i = 0; i < mat.numfilas(); i++)
	{
		map<int, double>::const_iterator itFila = mat.filas[i].begin();
		map<int, double>::const_iterator itFilaFin = mat.filas[i].end();
		while (itFila != itFilaFin)
		{
			(*this).set(i, itFila->first, itFila->second);
			itFila++;
		}
	}
	//cout << "termine constr. x copia" << endl;
}

int matrixEsparsa::numfilas() const
{
	return this->row;
}

int matrixEsparsa::numcolumnas() const
{
	return this->col;
}

//asumo que son del mismo tamaño
matrixEsparsa &matrixEsparsa::operator+(const matrixEsparsa &mat)
{
	for (int i = 0; i < mat.numfilas(); i++)
	{
		for (int j = 0; j < mat.numcolumnas(); j++)
		{
			double elem = (*this).get(i, j);
			(*this).set(i, j, elem + mat.get(i, j));
		}
	}

	return (*this);
}

matrixEsparsa &matrixEsparsa::operator-(const matrixEsparsa &mat)
{
	for (int i = 0; i < mat.numfilas(); i++)
	{
		for (int j = 0; j < mat.numcolumnas(); j++)
		{
			double elem = (*this).get(i, j);
			(*this).set(i, j, elem - mat.get(i, j));
		}
	}

	return (*this);
}

matrixEsparsa matrixEsparsa::operator*(const matrixEsparsa &mat)
{
	matrixEsparsa res(numfilas(), mat.numcolumnas());
	for (int i = 0; i < numfilas(); i++)
	{
		for (int j = 0; j < mat.numcolumnas(); j++)
		{
			double elem = (*this).multiplicarFilaPorColumna(i, mat, j);
			res.set(i, j, elem);
		}
		cout << "termine fila: " << i << endl;
	}

	return res;
}

vector < double > matrixEsparsa::operator*(const vector<double> &v)
{
	vector<double> res2(numfilas(), 0);

	for (int i = 0; i < numfilas(); i++)
	{
		double res = 0.0;

		for (int j = 0; j < v.size(); j++)
		{
			res += this->get(i,j) * v[j];
		}

		res2[i] = res;
	}

	return res2;
}

matrixEsparsa &matrixEsparsa::operator*(double num)
{
	for (int i = 0; i < numfilas(); i++)
	{
		map<int, double>::iterator itFila = this->filas[i].begin();
		map<int, double>::iterator itFilaFin = this->filas[i].end();
		while (itFila != itFilaFin)
		{
			map<int, double>::iterator itFilaCopia = itFila++;
			(*this).set(i, itFilaCopia->first, num * (itFilaCopia->second));
		}
	}
	return (*this);
}
matrixEsparsa &matrixEsparsa::operator=(const matrixEsparsa &mat)
{
	//cout << "estoy usando la asignacion"<<endl;
	//cout << mat.numcolumnas() << " " << mat.numfilas() << " ";

	if (this != &mat)
	{
		this->col = mat.numcolumnas();
		this->row = mat.numfilas();

		vector < map < int, double > > newfilas(mat.numfilas(), map<int, double>());
		filas = newfilas;
		vector < map < int, double > > newcolumnas(mat.numcolumnas(), map<int, double>());
		columnas = newcolumnas;

		for (int i = 0; i < mat.numfilas(); i++)
		{
			map<int, double>::const_iterator itFila = mat.filas[i].begin();
			map<int, double>::const_iterator itFilaFin = mat.filas[i].end();
			while (itFila != itFilaFin)
			{
				(*this).set(i, itFila->first, itFila->second);
				itFila++;
			}
		}
	}
	//cout << "termine la asignacion"<<endl;

	return (*this);
}

matrixEsparsa matrixEsparsa::transponer()
{
	matrixEsparsa res(this->col, this->row);

	for (int i = 0; i < this->row; i++)
	{
		map<int, double>::iterator itFila = this->filas[i].begin();
		map<int, double>::iterator itFilaFin = this->filas[i].end();
		while (itFila != itFilaFin)
		{
			int j = itFila->first;
			res.set(j, i, get(i, j));
			itFila++;
		}
	}

	return res;
}

double matrixEsparsa::multiplicarFilaPorColumna(int fil, const matrixEsparsa &mat, int col)
{
	map<int, double>::iterator itFila = this->filas[fil].begin();
	map<int, double>::iterator itFilaFin = this->filas[fil].end();

	map<int, double>::const_iterator itCol = mat.columnas[col].begin();
	map<int, double>::const_iterator itColFin = mat.columnas[col].end();

	double res = 0;
	while (itFila != itFilaFin && itCol != itColFin)
	{
		if (itFila->first == itCol->first)
		{
			res = res + (itFila->second * itCol->second);
			itFila++;
			itCol++;
		}
		else
		{
			if (itFila->first < itCol->first)
			{
				itFila++;
			}
			else
			{
				itCol++;
			}
		}
	}
	return res;
}

void matrixEsparsa::eliminacionGaussianaSinPivoteo(vector<double> &terminoInd)
{
	for (int i = 0; i < (*this).numfilas(); i++)
	{
		for (int j = i + 1; j < (*this).numfilas(); j++)
		{
			if (!eq((*this).get(j, i), 0))
			{
				double k = (*this).get(j, i) / (*this).get(i, i);
				//a la fila j le resto k por fila i
				for (int col = i; col < (*this).numcolumnas(); col++)
				{ //resto filas
					(*this).set(j, col, (*this).get(j, col) - k * (*this).get(i, col));
				}
				terminoInd[j] = (terminoInd[j] - k * terminoInd[i]); //resto los numeros a la derecha del igual
			}
		}
	}
}

void matrixEsparsa::eliminacionGaussiana(vector<double> &terminoInd)
{
	int h = 0;
	int k = 0;
	while (h < (*this).numfilas() && k < (*this).numcolumnas())
	{
		/* Encontrar el k-pivote: */
		int i_max = h;
		double elem_max = (*this).get(h, k);
		for (int p = h + 1; p < (*this).numfilas(); p++)
		{
			double elem_comparar = (*this).get(p, k);
			if (fabs(elem_comparar) > fabs(elem_max))
			{
				elem_max = elem_comparar;
				i_max = p;
			}
		}
		if (eq((*this).get(i_max, k), 0))
		{
			/* No hay pivote en esta columna, paso a la siguiente */
			k++;
		}
		else
		{
			//swapRows(h, i_max);
			map<int, double> guardar = (*this).filas[h];
			(*this).filas[h] = (*this).filas[i_max];
			(*this).filas[i_max] = guardar;
			/* Hacer para todas las filas debajo: */
			for (int i = h + 1; i < (*this).numfilas(); i++)
			{
				if (!(eq((*this).get(i, k), 0)))
				{
					double f = (*this).get(i, k) / (*this).get(h, k);
					/* Llenar con ceros la parte baja de la columna: */
					(*this).set(i, k, 0);
					/* Los elementos restantes de la matriz: */
					for (int j = k + 1; j < (*this).numcolumnas(); j++)
					{
						double elem = (*this).get(i, j);
						double elemARestar = ((*this).get(h, j) * f);
						//cout << "elem: " << elem << " elemARestar: " << elemARestar << endl;
						(*this).set(i, j, elem - elemARestar);
					}
					terminoInd[i] = terminoInd[i] - f * terminoInd[h];
				}
			}
			//veo como quedo hasta ahora la matriz
			/*cout << "hasta ahora: " << endl;
				for (int i = 0; i < (*this).numfilas(); i++) {
					for (int j = 0; j < (*this).numcolumnas(); j++) {
						cout << (*this).get(i, j) << "  ";
					}
					cout << endl;
				}*/
			/* incrementar pivote columna y fila */
			h++;
			k++;
		}
	}
}

vector<double> matrixEsparsa::sustitucionParaAtras(const vector<double> &b)
{
	//supongo A es matriz triangular superior
	vector<double> res((*this).numcolumnas(), 0);
	double suma = 0;
	for (int i = (*this).numfilas() - 1; i >= 0; i--)
	{
		if (eq((*this).get(i, i), 0))
			throw runtime_error("encontre un cero!! estas haciendo las cosas mal");
		suma = 0;
		map<int, double>::iterator it = ((*this).filas[i]).end();
		it--;
		int j = it->first;
		while (it != ((*this).filas[i]).begin() && j > i)
		{
			suma += (*this).get(i, j) * res[j];
			it--;
			j = it->first;
		}
		res[i] = (b[i] - suma) / (*this).get(i, i);
	}

	return res;
}

bool eq(double a, double b)
{
	return fabs(a - b) < eps;
}

void matrixEsparsa::print_matrixEsparsa() {
	for (int i = 0; i < this->row; i++) {
		for (map<int, double>::iterator it = this->filas[i].begin(); it != this->filas[i].end(); it++)
		{
			cout << this->get(i, it->first) << " ";
		}
		cout << endl;
	}
}
