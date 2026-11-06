#include <iostream>
#include <cstring>

using namespace std;

int ** initMatriz(int filas, int columnas){
	int ** m = new int*[filas]();
	for(int i=0; i < filas; i++)
	{
		m[i] = new int[columnas]();
	}
	return m;
}

void deleteMatriz(int** matriz, int filas) {
    for (int i = 0; i < filas; i++) {
        delete[] matriz[i];
    }
    delete[] matriz;
}


int max_puntaje_archivos(int** archivos, int N, int S, int L) {
    int** matriz = initMatriz(L+1, S+1);

    for (int i = 1; i <= N; i++) {
        int t = archivos[i-1][0];
        int l = archivos[i-1][1];
        int p = archivos[i-1][2];

        for (int j = L; j >= l; j--) {
            for (int k = S; k >= t; k--) {
                matriz[j][k] = max(matriz[j][k], matriz[j-l][k-t] + p);
            }
        }
    }

    return matriz[L][S];
}

int main() {
    int N; // Cantidad de archivos
    int S; // Cantidad maxima de tamanio
    int L; // Cantidad maxima de lineas de codigo
    cin >> N;
    cin >> S;
    cin >> L;

    int** archivos = initMatriz(N, 3); // Matriz con los N archivos en las filas y 3 columnas (tamanio, lineas, puntaje)
    for (int i = 0; i < N; i++) {
        cin >> archivos[i][0] >> archivos[i][1] >> archivos[i][2];
    }

    int puntaje_maximo = max_puntaje_archivos(archivos, N, S, L);

    cout << puntaje_maximo << endl;

    deleteMatriz(archivos, N);

    return 0;
}