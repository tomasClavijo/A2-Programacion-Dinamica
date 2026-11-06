// Mochila 0/1: greedy vs programacion dinamica.
// Explicacion en Letra.md y linea por linea en CODIGO.md.
#include <iostream>                                // para cin y cout
using namespace std;                               // para escribir cout en vez de std::cout

int max(int a, int b) {                            // devuelve el mayor de dos enteros
    return a > b ? a : b;                          // si a > b devuelve a, si no devuelve b
}                                                  // fin de max

int ** initMatriz(int filas, int columnas) {       // crea una matriz de filas x columnas
    int ** m = new int*[filas]();                  // array de punteros: uno por fila
    for (int i = 0; i < filas; i++) {              // para cada fila...
        m[i] = new int[columnas]();                // ...crea la fila; los () la dejan toda en 0
    }                                              // fin del for de filas
    return m;                                      // devuelve la matriz (se usa como m[fila][columna])
}                                                  // fin de initMatriz

void deleteMatriz(int** matriz, int filas) {       // libera la memoria de la matriz
    for (int i = 0; i < filas; i++) {              // para cada fila...
        delete[] matriz[i];                        // ...borra la fila
    }                                              // fin del for de filas
    delete[] matriz;                               // al final borra el array de punteros
}                                                  // fin de deleteMatriz

// Imprime los numeros de los objetos marcados en usados, de menor a mayor.
void imprimirUsados(bool* usados, int N) {         // usados[i] es true si el objeto i se lleva
    cout << "[";                                   // abre el corchete
    bool primero = true;                           // para no poner espacio antes del primer numero
    for (int i = 1; i <= N; i++) {                 // recorre los objetos del 1 al N (salen ordenados)
        if (usados[i]) {                           // si el objeto i se lleva...
            if (!primero) cout << " ";             // ...pone un espacio si no es el primero
            cout << i;                             // imprime el numero del objeto
            primero = false;                       // los siguientes ya no son el primero
        }                                          // fin del if usados
    }                                              // fin del for de objetos
    cout << "]";                                   // cierra el corchete
}                                                  // fin de imprimirUsados

// GREEDY: en cada vuelta mete el objeto de mejor ratio (valor/peso) que todavia entra.
// Nunca saca un objeto que ya metio.
int greedy(int* pesos, int* valores, int N, int capacidad, bool* usados) {   // devuelve el valor total
    int libre = capacidad;                         // kilos que quedan libres en la mochila
    int total = 0;                                 // valor acumulado de lo que se metio
    while (true) {                                 // cada vuelta mete un objeto (sale por el return)
        int elegido = -1;                          // mejor candidato de esta vuelta; -1 = ninguno todavia
        for (int i = 1; i <= N; i++) {             // revisa todos los objetos
            if (!usados[i] && pesos[i] <= libre) { // solo los que no se usaron y entran en lo libre
                double ratio = (double)valores[i] / pesos[i];   // cuanto rinde por kilo (double para no truncar)
                if (elegido == -1 || ratio > (double)valores[elegido] / pesos[elegido]) {   // es el primero o rinde mas?
                    elegido = i;                   // pasa a ser el mejor candidato (en empate queda el primero)
                }                                  // fin del if mejor ratio
            }                                      // fin del if no usado y entra
        }                                          // fin del for de objetos
        if (elegido == -1) {                       // si no hubo ningun candidato...
            return total;                          // ...no entra nada mas: termina (lo libre se desperdicia)
        }                                          // fin del if sin candidatos
        usados[elegido] = true;                    // marca el objeto como usado (no se puede elegir otra vez)
        libre -= pesos[elegido];                   // descuenta su peso de lo libre
        total += valores[elegido];                 // suma su valor; nunca se saca: por eso es greedy
    }                                              // fin del while: vuelve a buscar el siguiente
}                                                  // fin de greedy

// PROGRAMACION DINAMICA:
// tab[i][c] = maximo valor usando solo los objetos 1..i con capacidad c.
int pd(int* pesos, int* valores, int N, int capacidad, bool* usados) {   // devuelve el valor optimo
    int ** tab = initMatriz(N + 1, capacidad + 1); // tabla (N+1) x (C+1) en 0: la fila 0 es el caso base

    for (int i = 1; i <= N; i++) {                 // filas de arriba a abajo: agrega el objeto i
        for (int c = 0; c <= capacidad; c++) {     // columnas: cada capacidad de 0 a C
            tab[i][c] = tab[i - 1][c];             // opcion 1, NO lo pongo: lo mismo que la celda de arriba
            if (pesos[i] <= c) {                   // si el objeto i entra en capacidad c...
                tab[i][c] = max(tab[i][c], valores[i] + tab[i - 1][c - pesos[i]]);  // opcion 2, LO PONGO: su valor + lo mejor de los anteriores en lo que sobra; me quedo con la mejor
            }                                      // fin del if entra
        }                                          // fin del for de capacidades
    }                                              // fin del for de objetos

    // Reconstruccion: si el valor cambia respecto de la fila de arriba, el objeto se uso.
    int c = capacidad;                             // arranca con la mochila entera
    for (int i = N; i >= 1; i--) {                 // recorre los objetos de abajo hacia arriba
        if (tab[i][c] != tab[i - 1][c]) {          // si agregar el objeto i mejoro el valor...
            usados[i] = true;                      // ...entonces se uso: lo marca
            c -= pesos[i];                         // y descuenta su peso: sigue con lo que queda
        }                                          // fin del if se uso
    }                                              // fin del for de reconstruccion

    int resultado = tab[N][capacidad];             // la respuesta: esquina de abajo a la derecha
    deleteMatriz(tab, N + 1);                      // libera la tabla
    return resultado;                              // devuelve el valor optimo
}                                                  // fin de pd

int main() {                                       // programa principal
    int N, capacidad;                              // cantidad de objetos y capacidad de la mochila
    cin >> N >> capacidad;                         // lee la primera linea de la entrada

    // Indices de 1 a N (la posicion 0 no se usa), igual que la tabla.
    int* pesos = new int[N + 1];                   // pesos[i] = peso del objeto i
    int* valores = new int[N + 1];                 // valores[i] = valor del objeto i
    for (int i = 1; i <= N; i++) {                 // para cada objeto del 1 al N...
        cin >> pesos[i] >> valores[i];             // ...lee su peso y su valor
    }                                              // fin del for de lectura

    bool* usadosGreedy = new bool[N + 1]();        // que objetos usa el greedy (todo en false)
    bool* usadosPD = new bool[N + 1]();            // que objetos usa la PD (todo en false)

    int valorGreedy = greedy(pesos, valores, N, capacidad, usadosGreedy);   // corre el greedy
    int valorPD = pd(pesos, valores, N, capacidad, usadosPD);               // corre la PD

    cout << "greedy: " << valorGreedy << " ";     // imprime el valor del greedy
    imprimirUsados(usadosGreedy, N);               // imprime los objetos que uso, ej. [1 2]
    cout << endl;                                  // fin de linea

    cout << "pd: " << valorPD << " ";             // imprime el valor de la PD
    imprimirUsados(usadosPD, N);                   // imprime los objetos que uso, ej. [1 3]
    cout << endl;                                  // fin de linea

    delete[] pesos;                                // libera el array de pesos
    delete[] valores;                              // libera el array de valores
    delete[] usadosGreedy;                         // libera las marcas del greedy
    delete[] usadosPD;                             // libera las marcas de la PD
    return 0;                                      // termina el programa sin error
}                                                  // fin de main
