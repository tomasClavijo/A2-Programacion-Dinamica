// Mochila 0/1 con programacion dinamica (ejemplo de clase, A2 2022).
// Explicacion completa paso a paso en Letra.md.
#include <cassert>
#include <string>
#include <iostream>
#include <limits>
using namespace std;

int cantObj = 4;
int capacidad = 13;
// El primer valor de los arrays no se usa: es el "objeto 0" (no objeto).
// Asi la fila 0 de la tabla es el caso base (sin objetos -> valor 0) y los
// objetos reales quedan indexados de 1 a cantObj.
int pesos[5] = {0, 5, 3, 8 , 4};
int valores[5] = {0, 10, 20, 25, 8};
string nombres[5] = {"", "Lingote", "Diamante", "Rubi", "Moneda"};

int max(int a, int b){
    return a > b ? a : b;
}

// Crea la tabla de (cantObj + 1) x (capacidad + 1).
// Los "()" de new inicializan todo en 0, lo que ya deja cargados los casos base:
// tab[0][c] = 0 (sin objetos) y tab[i][0] = 0 (sin capacidad).
int ** initMatriz(){
    int **m = new int* [cantObj + 1]();
    for (int i = 0; i <= cantObj; i++){
        m[i] = new int[capacidad+1]();
    }
    return m;
}

void mochila(){
    // tab[objH][capH] = maximo valor usando solo los objetos 1..objH
    //                   con una mochila de capacidad capH.
    int ** tab = initMatriz();

    // Se llena fila por fila: la fila objH solo depende de la fila objH - 1.
    for (int objH = 1; objH <= cantObj; objH++){
        int pesoObj = pesos[objH];
        int valorObj = valores[objH];
        for(int capH = 0; capH <= capacidad; capH++){
            if(pesoObj > capH){
                // Caso 1: el objeto no entra -> es como no tenerlo.
                tab[objH][capH] = tab[objH - 1][capH];
            } else {
                // Caso 2: el objeto entra -> me quedo con la mejor opcion.
                // Ponerlo: gano su valor y resuelvo el resto con la capacidad
                // que sobra y SIN este objeto (fila objH - 1, no se repite).
                int valorDePoner = valorObj + tab[objH - 1][capH - pesoObj];
                // No ponerlo: lo mismo que sin este objeto.
                int valorDeNOPoner = tab[objH-1][capH];

                tab[objH][capH] = max(valorDePoner, valorDeNOPoner);
            }
        }
    }
    // La respuesta es la celda con todos los objetos y toda la capacidad.
    cout << "El maximo valor obtenido es: " << tab[cantObj][capacidad] << endl;

    // Reconstruccion: recorro la tabla hacia atras desde tab[N][C].
    // Si el valor cambia respecto de la fila de arriba, el objeto se uso
    // (y descuento su peso); si no cambia, no hacia falta.
    int objetoIndex = cantObj;
    int capacidadIndex = capacidad;
    while(objetoIndex > 0){
        if(tab[objetoIndex][capacidadIndex] != tab[objetoIndex -1][capacidadIndex]){
            cout << "Use el objeto " << nombres[objetoIndex] << " p(" << pesos[objetoIndex] << ") v(" << valores[objetoIndex] << ")" << endl;
            capacidadIndex -= pesos[objetoIndex];
        }
        objetoIndex--;
    }

    for (int i = 0; i <= cantObj; i++){
        delete[] tab[i];
    }
    delete[] tab;
}


int main(){
    mochila();
    return 0;
}
