#include <cassert>
#include <string>
#include <iostream>
#include <limits>
using namespace std;

int cantObj = 4;
int capacidad = 13;
// el primer valor de los array no se usa (es el no objeto)
int pesos[5] = {0, 5, 3, 8 , 4};
int valores[5] = {0, 10, 20, 25, 8};
string nombres[5] = {"", "Lingote", "Diamante", "Rubi", "Moneda"};

int max(int a, int b){
    return a > b ? a : b;
}

int ** initMatriz(){
    int **m = new int* [cantObj + 1]();
    for (int i = 0; i <= cantObj; i++){
        m[i] = new int[capacidad+1]();
    }
    return m;
}

void mochila(){
    int ** tab = initMatriz();
    for (int objH = 1; objH <= cantObj; objH++){
        int pesoObj = pesos[objH];
        int valorObj = valores[objH];
        for(int capH = 0; capH <= capacidad; capH++){
            // No puedo usar el obj = no tener el obj
            if(pesoObj > capH){
                tab[objH][capH] = tab[objH - 1][capH];
            } else {
                // Calculo el valor de la mochila si podria el objeto en consideracion
                int valorDePoner = valorObj + tab[objH - 1][capH - pesoObj];
                // calculo el valor de la mochila si ignoro el objeto
                int valorDeNOPoner = tab[objH-1][capH];

                tab[objH][capH] = max(valorDePoner, valorDeNOPoner);
            }
        }
    }
    cout << "El maximo valor obtenido es: " << tab[cantObj][capacidad] << endl;

    int objetoIndex = cantObj;
    int capacidadIndex = capacidad;
    while(objetoIndex > 0){
        // Evaluo si se usa el objeto
        if(tab[objetoIndex][capacidadIndex] != tab[objetoIndex -1][capacidadIndex]){
            cout << "Use el objeto " << nombres[objetoIndex] << " p(" << pesos[objetoIndex] << ") v(" << valores[objetoIndex] << ")" << endl;
            capacidadIndex -= pesos[objetoIndex];
        }
        objetoIndex--;
    }
}

int main(){
    mochila();
    return 0;
}
