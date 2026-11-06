// Seleccion de archivos: mochila 0/1 con dos restricciones (tamanio y lineas), A2 2023 - Ejercicio 7.
// Explicacion completa paso a paso en Letra.md.
#include <iostream>
#include <cstring>

using namespace std;

// Matriz filas x columnas inicializada en 0 (por los "()").
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


// matriz[j][k] = maximo puntaje con a lo sumo j lineas y tamanio a lo sumo k,
//                usando los archivos procesados hasta el momento.
//
// La version "de libro" seria T[i][j][k] (3 dimensiones), pero la capa i solo
// depende de la i-1, asi que se usa UNA sola matriz que se sobrescribe por archivo.
int max_puntaje_archivos(int** archivos, int N, int S, int L) {
    // Caso base: sin archivos el puntaje es 0 en todas las celdas.
    int** matriz = initMatriz(L+1, S+1);

    for (int i = 1; i <= N; i++) {
        int t = archivos[i-1][0];   // tamanio
        int l = archivos[i-1][1];   // lineas
        int p = archivos[i-1][2];   // puntaje

        // Las dos capacidades se recorren de MAYOR a MENOR.
        // matriz[j-l][k-t] esta "arriba a la izquierda" y todavia no se
        // actualizo en esta vuelta, asi que tiene el valor SIN el archivo i
        // (capa i-1). Por eso cada archivo se usa a lo sumo una vez.
        // En orden creciente se podria sumar el mismo archivo varias veces.
        //
        // Los for cortan en j >= l y k >= t: para capacidades menores el
        // archivo no entra y la celda queda como estaba (= no usarlo).
        for (int j = L; j >= l; j--) {
            for (int k = S; k >= t; k--) {
                // max(no usar el archivo, usarlo y gastar l lineas y t de tamanio)
                matriz[j][k] = max(matriz[j][k], matriz[j-l][k-t] + p);
            }
        }
    }

    int resultado = matriz[L][S];
    deleteMatriz(matriz, L+1);
    return resultado;
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

    // O(N * L * S) temporal, O(L * S) espacial para la tabla.
    int puntaje_maximo = max_puntaje_archivos(archivos, N, S, L);

    cout << puntaje_maximo << endl;

    deleteMatriz(archivos, N);

    return 0;
}
