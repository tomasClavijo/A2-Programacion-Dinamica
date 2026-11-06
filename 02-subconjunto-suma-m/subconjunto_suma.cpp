// Subconjunto de suma M con repeticion (mochila no acotada booleana), A2 2022 - Ejercicio 8.
// Explicacion completa paso a paso en Letra.md.
#include <cassert>
#include <string>
#include <iostream>
#include <limits>
using namespace std;

// Crea una tabla de cantidad x (cota + 1), inicializada en false por los "()".
bool ** initMatriz(int cantidad, int cota)
{
    bool **m = new bool* [cantidad]();
    for (int i = 0; i < cantidad; i++){   // antes: i <= cantidad (escribia fuera del array)
        m[i] = new bool[cota+1]();
    }
    return m;
};

// tab[i][j] = se puede formar la suma exacta j usando (con repeticion)
//             solo los elementos objetos[0..i]?
bool ** resolucion(int cantidad, int cota, int* objetos)
{
    bool** tab = initMatriz(cantidad, cota);

    // Columna 0: se deja en false y el caso j == Ki se trata aparte mas abajo.
    // (Las consultas siempre son Mi >= 1, asi que esta columna nunca se imprime.)
    for (int i = 0; i < cantidad; i++){
        tab[i][0] = false;
    }

    // Caso base, fila 0: con solo objetos[0] repetido llego a j
    // si y solo si j es multiplo de objetos[0].
    for(int j = 1; j <= cota; j++){
        if(j % objetos[0] == 0){
            tab[0][j] = true;
        }else{
            tab[0][j] = false;
        }
    }

    // Resto de las filas. Se recorre j de izquierda a derecha porque
    // tab[i][j - Ki] (misma fila) tiene que estar calculado antes.
    for(int i = 1; i< cantidad; i++){
        for (int j = 1; j <= cota; j++){
            if (j < objetos[i]){
                // Ki no entra en j: solo puedo no usarlo.
                tab[i][j] = tab[i-1][j];
            } else if (j == objetos[i]){
                // Con Ki solo ya llego a j.
                tab[i][j] = true;
            } else
            {
                // No usar Ki (fila de arriba) o usarlo al menos una vez.
                // Al usarlo me quedo en la MISMA fila i: lo que falta (j - Ki)
                // se puede seguir armando con Ki. Eso permite la repeticion
                // (en la mochila 0/1 seria tab[i-1][j - Ki]).
                tab[i][j] = tab[i-1][j] || tab[i][j-objetos[i]];
            }
        }
    }
    return tab;
}

int main()
{
    // Lectura de los N elementos del conjunto.
    int N;
    cin >> N;
    int* objetos = new int [N];
    int elemento;
    for (int i = 0; i < N; i++){
        cin >> elemento;
        objetos[i] = elemento;
    }

    // Cota M: la tabla se arma una sola vez hasta M y sirve para todas las consultas.
    int cota;
    cin >> cota;

    // Lectura de las P consultas.
    int P;
    cin >> P;
    int* pruebas = new int [P];
    int prueba;
    for (int i = 0; i < P; i ++){
        cin >> prueba;
        pruebas[i] = prueba;
    }

    // O(N*M) para construir la tabla y O(1) por consulta:
    // la respuesta es la ultima fila (todos los elementos disponibles).
    bool ** res = resolucion(N, cota, objetos);
    for (int p = 0; p < P; p++){
        if (res[N-1][pruebas[p]] == true){
            cout << 1 << endl;
        } else {
            cout << 0 << endl;
        }
    }
    delete [] objetos;
    delete [] pruebas;
    for (int i = 0; i < N; i++){
        delete[] res[i];
    }
    delete [] res;
    return 0;
}
