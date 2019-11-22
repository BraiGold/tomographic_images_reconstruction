#include "simulacionRayos.h"
#include <math.h>
vector <vector < int > > transponerPuntos(vector < vector < int > > vecP ){
    vector <vector < int > > vecResult;
    for(int i = 0; i< vecP.size();i++){
        int item1 = vecP[i][0];
        int item2 = vecP[i][1];
        vector <int> vecPunt;
        vecPunt.push_back(item2);
        vecPunt.push_back(item1);
        vecResult.push_back(vecPunt);
    }
    return vecResult;
}



void simularRayos(int tipoRayos, int cantidadRayos, matrix& data, int n_discretizacion, double ruido, int minimoHW, matrix &matrizDistancias, vector < double > &tiempos, vector < vector < int > > & puntos)
{
    srand (static_cast <unsigned> (time(0)));
    for (int i = 0; i < cantidadRayos; i++)
    {

        int xPunto1 = puntos[i][0];
        int yPunto1 = puntos[i][1];
        int xPunto2 = puntos[i][2];
        int yPunto2 = puntos[i][3];
        //cout << "pepe2" << endl;
        //aca ya tengo los 2 puntos que me van a determinar la recta.

        vector < vector < int > > vecPuntos;
        /* if(tipoRayos == 1){
            vecPuntos = damePuntos(xPunto1, yPunto1, xPunto2, yPunto2, minimoHW);
        }else{
            vecPuntos = damePuntos2(xPunto1, yPunto1, xPunto2, yPunto2);
        }
        int cantRayossobre2 = (int)cantidadRayos/2;
        vector < vector < int > > vecPuntos2;
        if(tipoRayos == 1){
            xPunto1 = puntos[i+cantRayossobre2][0];
            yPunto1 = puntos[i+cantRayossobre2][1];
            xPunto2 = puntos[i+cantRayossobre2][2];
            yPunto2 = puntos[i+cantRayossobre2][3];
            vecPuntos2 = damePuntos(xPunto1, yPunto1, xPunto2, yPunto2, minimoHW);
        }else{
            vecPuntos2 = transponerPuntos(vecPuntos);
        } */
        cout << "rayo numero: " << i << endl;
        vecPuntos = damePuntos(xPunto1, yPunto1, xPunto2, yPunto2, minimoHW);
        if (i == 4) {
            for (int p = 0; p < vecPuntos.size(); p++) {
                for (int j = 0; j < vecPuntos[p].size(); j++) {
                    cout << "vecPuntos por donde pasa " << p << " " << vecPuntos[p][j] << endl;
                }
            }
        }
        //cout << vecPuntos.size()1 << endl;
        //cout << " xp1 "<< xPunto1 << " yp1 "<< yPunto1 << " xp2 " << xPunto2 << " yp2 " << yPunto2<< endl;
        for (int j = 0; j < vecPuntos.size(); j++)
        {

            //cout << j << endl;
            //cout << vecPuntos[j][0] << endl;
            //cout << vecPuntos[j][1] << endl;
            double intensidad = (data[vecPuntos[j][0]][vecPuntos[j][1]]);

            tiempos[i] += intensidad;
            int i_discre =(int)vecPuntos[j][0]*n_discretizacion/minimoHW;
            int j_discre =(int)vecPuntos[j][1]*n_discretizacion/minimoHW;


            matrizDistancias[i][i_discre * n_discretizacion + j_discre] += 1;
        }
        //double ruido_aleatorio = static_cast <double> (rand()) / (static_cast <double> (RAND_MAX/ruido));

        double ruido_aleatorio = (rand() % (int) ruido*10 + 1)/10.0;
        int masomenos = (rand()%2);
        if(masomenos == 0){
          tiempos[i] += ruido_aleatorio;
        }else{
          tiempos[i] -=ruido_aleatorio;
        }
    }
}

void simularRayos(int tipoRayos, int cantidadRayos, matrix &data, int n_discretizacion, double ruido, int minimoHW, matrixEsparsa &matrizDistancias, vector<double> &tiempos, vector < vector < int > > &puntos)
{
    srand(static_cast<unsigned>(time(0)));
    for (int i = 0; i < cantidadRayos; i++)
    {

        int xPunto1 = puntos[i][0];
        int yPunto1 = puntos[i][1];
        int xPunto2 = puntos[i][2];
        int yPunto2 = puntos[i][3];
        //cout << "pepe2" << endl;
        //aca ya tengo los 2 puntos que me van a determinar la recta.

        vector < vector < int > > vecPuntos;
        /* if(tipoRayos == 1){
            vecPuntos = damePuntos(xPunto1, yPunto1, xPunto2, yPunto2, minimoHW);
        }else{
            vecPuntos = damePuntos2(xPunto1, yPunto1, xPunto2, yPunto2);
        }
        int cantRayossobre2 = (int)cantidadRayos/2;
        vector < vector < int > > vecPuntos2;
        if(tipoRayos == 1){
            xPunto1 = puntos[i+cantRayossobre2][0];
            yPunto1 = puntos[i+cantRayossobre2][1];
            xPunto2 = puntos[i+cantRayossobre2][2];
            yPunto2 = puntos[i+cantRayossobre2][3];
            vecPuntos2 = damePuntos(xPunto1, yPunto1, xPunto2, yPunto2, minimoHW);
        }else{
            vecPuntos2 = transponerPuntos(vecPuntos);
        } */
        cout << "rayo numero: " << i << endl;
        vecPuntos = damePuntos(xPunto1, yPunto1, xPunto2, yPunto2, minimoHW);
        if (i == 4)
        {
            for (int p = 0; p < vecPuntos.size(); p++)
            {
                for (int j = 0; j < vecPuntos[p].size(); j++)
                {
                    cout << "vecPuntos por donde pasa " << p << " " << vecPuntos[p][j] << endl;
                }
            }
        }
        //cout << vecPuntos.size()1 << endl;
        //cout << " xp1 "<< xPunto1 << " yp1 "<< yPunto1 << " xp2 " << xPunto2 << " yp2 " << yPunto2<< endl;
        for (int j = 0; j < vecPuntos.size(); j++)
        {

            //cout << j << endl;
            //cout << vecPuntos[j][0] << endl;
            //cout << vecPuntos[j][1] << endl;
            double intensidad = (data[vecPuntos[j][0]][vecPuntos[j][1]]);

            tiempos[i] += intensidad;
            int i_discre = (int)vecPuntos[j][0] * n_discretizacion / minimoHW;
            int j_discre = (int)vecPuntos[j][1] * n_discretizacion / minimoHW;

            double elem = matrizDistancias.get(i, i_discre * n_discretizacion + j_discre) + 1;
            matrizDistancias.set(i, i_discre * n_discretizacion + j_discre, elem);
        }

        //double ruido_aleatorio = static_cast<double>(rand()) / (static_cast<double>(RAND_MAX / ruido));
        double ruido_aleatorio = (double)(rand() % ((int)ruido*10 + 1)) / 10.0;
        tiempos[i] += ruido_aleatorio;
    }
}

void darRayos(int tipoRayos, bool traspongo, int &cantidadRayos, vector < vector < int > > &puntos, int minimoHW, int n) {
	switch (tipoRayos) {
		case 1:
		{
			//rayos aleatorios
			cout << "INICIO CASO 1" << endl;
            for (int p = 0; p < cantidadRayos; p++) {

				int xPunto1;
				int yPunto1;
				int xPunto2;
				int yPunto2;
				do {
					xPunto1 = rand() % minimoHW;
					yPunto1 = rand() % minimoHW;
					xPunto2 = rand() % minimoHW;
					yPunto2 = rand() % minimoHW;
				} while(xPunto1 == xPunto2 || yPunto1 == yPunto2);

				int ints[] = { xPunto1, yPunto1, xPunto2, yPunto2 };

				vector < int > parPuntos(ints, ints + sizeof(ints) / sizeof(int));

				puntos.push_back(parPuntos);
			}
            if(traspongo){
				for (int p = 0; p < cantidadRayos; p++) {
					vector < int > parPuntos = puntos[p];
	                int ints[] = { parPuntos[1], parPuntos[0], parPuntos[3], parPuntos[2] };
	                vector < int > parPuntosTraspuesto(ints, ints + sizeof(ints) / sizeof(int));
	                puntos.push_back(parPuntosTraspuesto);
	            }
			}
            cantidadRayos = puntos.size();
            cout << "FIN CASO 1" << endl;
            break;
		}
        case 2:
        {
			//puntos canonicos
            cout << "INICIO CASO 2" << endl;
            for (int j = (minimoHW/n) / 2; j < minimoHW; j = j+(minimoHW/n)) {

                int yPunto1 = 0;
                int xPunto1 = j;
                int yPunto2 = minimoHW-1;
                int xPunto2 = j;

                int ints[] = { xPunto1, yPunto1, xPunto2, yPunto2 };

                vector < int > parPuntos(ints, ints + sizeof(ints) / sizeof(int));

                puntos.push_back(parPuntos);
                //cout << j << endl;
            }

            int puntosSize = puntos.size();
            if(traspongo){
	            for (int i = 0; i < puntosSize; i++) {
	                vector < int > parPuntos = puntos[i];
	                int ints[] = { parPuntos[1], parPuntos[0], parPuntos[3], parPuntos[2] };
	                vector < int > parPuntosTraspuesto(ints, ints + sizeof(ints) / sizeof(int));
	                puntos.push_back(parPuntosTraspuesto);
	                //cout << i << endl;
	            }
			}
            cantidadRayos = puntos.size();
            /*for (int p = 0; p < puntos.size(); p++) {
                for (int j = 0; j < puntos[p].size(); j++) {
                    cout << "parPuntos " << p << " " << puntos[p][j] << endl;
                }
            }*/
            cout << "FIN CASO 2" << endl;
            break;
        }
        case 3:
        {
			//puntos magicos
			cout << "INICIO CASO 3" << endl;
            for (int x = 0; x < minimoHW; x++) {
                for (int y = 0; y < minimoHW; y++) {
                    int yPunto1 = 0;
                    int xPunto1 = x;
                    int yPunto2 = minimoHW-1;
                    int xPunto2 = y;

                    int ints[] = { xPunto1, yPunto1, xPunto2, yPunto2 };

                    vector < int > parPuntos(ints, ints + sizeof(ints) / sizeof(int));

                    puntos.push_back(parPuntos);
                }
            }
            int puntosSize = puntos.size();
            if(traspongo){
	            for (int i = 0; i < puntosSize; i++) {
	                vector < int > parPuntos = puntos[i];
	                int ints[] = { parPuntos[1], parPuntos[0], parPuntos[3], parPuntos[2] };
	                vector < int > parPuntosTraspuesto(ints, ints + sizeof(ints) / sizeof(int));
	                puntos.push_back(parPuntosTraspuesto);
	            }
			}
			cantidadRayos = puntos.size();
			cout << "FIN CASO 3" << endl;
			break;
        }
        case 4:
        {
			//puntos desde csv
			cout << "INICIO CASO 4" << endl;
			puntos = generarPuntosDesdeCSV("./puntosParaLeer.csv");
			int puntosSize = puntos.size();
			if(traspongo){
				for (int i = 0; i < puntosSize; i++) {
	                vector < int > parPuntos = puntos[i];
	                int ints[] = { parPuntos[1], parPuntos[0], parPuntos[3], parPuntos[2] };
	                vector < int > parPuntosTraspuesto(ints, ints + sizeof(ints) / sizeof(int));
	                puntos.push_back(parPuntosTraspuesto);
	            }
			}
			cantidadRayos = puntos.size();
			cout << "FIN CASO 4" << endl;
		}

    }

    cout << "tam de puntos desp de rayos " << puntos.size() << endl;
}

vector<double> cuadradosMinimosLineales(matrixEsparsa &matrizDistancias, vector<double> &tiempos) {
    matrixEsparsa matrizDistanciasTraspuesta = matrizDistancias.transponer();

    vector<double> tiempoConRuidoPorTraspuesta = matrizDistanciasTraspuesta * tiempos;

    matrixEsparsa matrizDistPorMatrizDistTraspuesta = matrizDistanciasTraspuesta * matrizDistancias;

    matrizDistPorMatrizDistTraspuesta.eliminacionGaussiana(tiempoConRuidoPorTraspuesta);

    vector<double> solucion = matrizDistPorMatrizDistTraspuesta.sustitucionParaAtras(tiempoConRuidoPorTraspuesta);

    return solucion;
}

vector<double> cuadradosMinimosLineales(matrix &matrizDistancias, vector<double> &tiempos) {
    //matrizDistancias.print_matrix();
    //cout << "paso1" << endl;
    //print_vector(tiempos);
    //construirCSV2(matrizDistancias,"Z_matrizDistancias.csv");
    //construirCSV3(tiempos,"Z_vectorTiempos.csv",tiempos.size());

    matrix matrizDistanciasTraspuesta = matrizDistancias.transponer();
    //cout << "paso2" << endl;
    //matrizDistanciasTraspuesta.print_matrix();
    vector < double > tiempoConRuidoPorTraspuesta = matrizDistanciasTraspuesta * tiempos;
    //cout << "paso3" << endl;
    //print_vector(tiempoConRuidoPorTraspuesta);
    matrix matrizDistPorMatrizDistTraspuesta = matrizDistanciasTraspuesta * matrizDistancias;

    //cout << "paso4" << endl;
    //matrizDistPorMatrizDistTraspuesta.eliminacionGaussiana(tiempoConRuidoPorTraspuesta);
    /*matrizDistPorMatrizDistTraspuesta.print_matrix();
    print_vector(tiempoConRuidoPorTraspuesta);*/
    //vector < double > solucion = matrizDistPorMatrizDistTraspuesta.sustitucionParaAtras(tiempoConRuidoPorTraspuesta);
    vector < double > solucion = matrizDistPorMatrizDistTraspuesta.resolverConGauss(tiempoConRuidoPorTraspuesta);
        //cout <<"tamaño solicion: "<< solucion.size()<<endl;


    // matrizDistanciasTraspuesta.print_matrix();
    // matrizDistPorMatrizDistTraspuesta.eliminacionGaussiana(tiempoConRuidoPorTraspuesta);
    // matrizDistanciasTraspuesta.print_matrix();
    // vector < double > solucion = matrizDistPorMatrizDistTraspuesta.sustitucionParaAtras(tiempoConRuidoPorTraspuesta);
    // //cout << "paso5" << endl;
    //print_vector(solucion);
    return solucion;
}


vector < vector < int > > damePuntos2(int x1,int y1, int x2,int y2){
  vector < vector < int > > retPuntos;
  vector < int > punto2;
  punto2.push_back(x1);
  punto2.push_back(y1);
  //retPuntos.push_back(punto2);//COMENTAR
  int dx = x2-x1;
  int dy = y2-y1;
  for(int x = x1;x<(x2+1) ; x++){
    double y =  (y1+dy*(x-x1)/(double)dx);
    vector < int > punto;
    punto.push_back((int)x);
    punto.push_back((int)trunc(y));
    retPuntos.push_back(punto);
  }
  return retPuntos;
}

// def generarRayo(p1,p2,n):
//     x1 = p1[0]
//     y1 = p1[1]
//     x2 = p2[0]
//     y2 = p2[1]
//     rayo = np.zeros((n,n))
//     dx = x2 - x1
//     dy = y2 - y1
//     for x in range(x1,x2+1):
//         y = y1 + dy * (x - x1) / dx
//         rayo[int(x)][int(y)] = 1
//
//     return rayo




vector < vector < int > > damePuntos(int p11, int p12, int p21, int p22, int n)
{
    //cout << "entre a damePuntos"<< " p1 " << p11 << " " << p12 << " p2 " << p21 << " " << p22 << endl;
    if (p21 == p11) {
        //"---";
        cout << "pepeeeee" << endl;
        double pend = ((double)(p21 - p11)) / (p22 - p12);
        double ordenada = p11 - pend * p12;
        double eps = 1.0/n;
        vector < vector < int > > puntos;
        int ultimoY = 0;
        int ultimoX = ordenada;
        if(ultimoY < n && ultimoY >= 0){
            vector < int > coor;
            coor.push_back(ultimoX);
            coor.push_back(ultimoY);
            puntos.push_back(coor);
            //cout << ultimoX << " " << ultimoY << " " << endl;
        }
        for (double i = 0.0; i < n; i += eps)
        {
            int intY = (int)i;
            int intX = (int)(pend * i + ordenada);
            if ((intY != ultimoY || intX != ultimoX) && intY < n && intY >= 0)
            {
                vector < int > coorde;
                coorde.push_back(intX);
                coorde.push_back(intY);
                //cout << intX << " " << intY << " " << endl;
                ultimoX = intX;
                ultimoY = intY;
                puntos.push_back(coorde);
            }
        }

        return puntos;
    } else {
        //"|";
        //"|";
        //"|";
        double pend = ((double)(p22 - p12)) / (p21 - p11);
        //cout << "entre a damePuntos" << endl;
        double ordenada = p12 - pend * p11;
        double eps = 1.0/n;
        vector < vector < int > > puntos;
        int ultimoX = 0;
        int ultimoY = ordenada;
        //cout << "x0: " << p11 << " y0: " << p12 << " x1: " << p21 << " y2: " << p22 <<endl;
        //cout << "pend: " << pend << "ordenada: " << ordenada <<endl;

        //vector < int > coor(ultimoX, ultimoY);
        if(ultimoY < n && ultimoY >= 0){
            vector < int > coor;
            coor.push_back(ultimoX);
            coor.push_back(ultimoY);
            puntos.push_back(coor);
            //cout << ultimoX << " " << ultimoY << " " << endl;
        }
        for (double i = 0.0; i < n; i += eps)
        {
            int intX = (int)i;
            int intY = (int)(pend * i + ordenada);
            if ((intX != ultimoX || intY != ultimoY) && intY < n && intY >= 0)
            {
                vector < int > coorde;
                coorde.push_back(intX);
                coorde.push_back(intY);
                //cout << intX << " " << intY << " " << endl;
                ultimoX = intX;
                ultimoY = intY;
                puntos.push_back(coorde);
            }
        }

        return puntos;
    }


}

vector< vector < int > > generoPuntosCanonicos(int minimoHW){
  vector < vector < int > > retPuntos;
  for(int i = 0;i<minimoHW;i++){
    for(int j = 0;j<minimoHW;j++){
      vector < int > punto;
      punto.push_back(i);
      punto.push_back(j);
      retPuntos.push_back(punto);
    }
  }
  return retPuntos;
}

vector < vector <int> > generarPuntosMagicos(int minimoHW){
    vector < vector <int> >  puntosMagicos;
    for (int x = 0; x < minimoHW; x++) {
        for (int y = 0; y < minimoHW; y++) {
            vector< int > puntos;
            puntos.push_back(0);
            puntos.push_back(x);
            puntos.push_back(minimoHW-1);
            puntos.push_back(y);
            puntosMagicos.push_back(puntos);
        }
    }
    //cout << "puntos magicos" << puntosMagicos.size()<<endl;
    return puntosMagicos;
}

vector<vector<int> > generarPuntosDesdeCSV(string nombreArchivo)
{
  int alto=0;
  int ancho= 4;
  matrix paresDepuntos = read_image(nombreArchivo,&ancho,&alto);
  vector<vector<int> > puntosNoMagicos;
  for (int i = 0; i < paresDepuntos.numfilas(); i++) {
    vector< int > puntos;
    puntos.push_back((int)paresDepuntos[i][0]);
    puntos.push_back((int)paresDepuntos[i][1]);
    puntos.push_back((int)paresDepuntos[i][2]);
    puntos.push_back((int)paresDepuntos[i][3]);
    puntosNoMagicos.push_back(puntos);
  }

  return puntosNoMagicos;
}
