// Mochila 0/1 recursiva: sin guardar resultados y con memoizacion.
// Explicacion en Letra.md. Misma estructura que Ejemplos/Dac_Mochila01.cpp (A2 2022).
#include <iostream>                                // para cin y cout
using namespace std;                               // para escribir cout en vez de std::cout

int* pesos;                                        // pesos[i] = peso del objeto i (de 1 a N)
int* valores;                                      // valores[i] = valor del objeto i (de 1 a N)
long long llamadas = 0;                            // cuenta cuantas veces se llama a la funcion

int max(int a, int b) {                            // devuelve el mayor de dos enteros
    return a > b ? a : b;                          // si a > b devuelve a, si no devuelve b
}                                                  // fin de max

// RECURSIVA SIN GUARDAR: mejor valor usando los objetos 1..objetoHasta con capacidadRestante kilos.
int mochila01(int objetoHasta, int capacidadRestante) {
    llamadas++;                                    // una llamada mas
    if (objetoHasta == 0 || capacidadRestante == 0) {   // caso base: sin objetos o sin lugar...
        return 0;                                  // ...no puedo llevar nada
    }                                              // fin del caso base

    int peso = pesos[objetoHasta];                 // peso del objeto que estoy decidiendo
    int valor = valores[objetoHasta];              // valor del objeto que estoy decidiendo

    if (peso > capacidadRestante) {                // si no entra...
        return mochila01(objetoHasta - 1, capacidadRestante);   // ...lo ignoro y sigo con los anteriores
    }                                              // fin del if no entra

    int valorDePoner = valor + mochila01(objetoHasta - 1, capacidadRestante - peso);   // lo pongo: su valor + lo mejor de los anteriores en lo que sobra
    int valorDeNoPoner = mochila01(objetoHasta - 1, capacidadRestante);                // no lo pongo: lo mejor de los anteriores con la misma capacidad
    return max(valorDePoner, valorDeNoPoner);      // me quedo con la mejor de las dos opciones
}                                                  // fin de mochila01

// RECURSIVA CON MEMOIZACION: igual que la de arriba, pero anota cada resultado en memo.
// memo[i][c] == -1 quiere decir "todavia no lo calcule".
int mochila01Memo(int objetoHasta, int capacidadRestante, int** memo) {
    llamadas++;                                    // una llamada mas
    if (objetoHasta == 0 || capacidadRestante == 0) {   // caso base: sin objetos o sin lugar...
        return 0;                                  // ...no puedo llevar nada
    }                                              // fin del caso base
    if (memo[objetoHasta][capacidadRestante] != -1) {   // si este subproblema ya lo resolvi...
        return memo[objetoHasta][capacidadRestante];    // ...devuelvo lo anotado, sin recalcular
    }                                              // fin del if ya calculado

    int peso = pesos[objetoHasta];                 // peso del objeto que estoy decidiendo
    int valor = valores[objetoHasta];              // valor del objeto que estoy decidiendo
    int resultado;                                 // aca va la respuesta de este subproblema

    if (peso > capacidadRestante) {                // si no entra...
        resultado = mochila01Memo(objetoHasta - 1, capacidadRestante, memo);   // ...lo ignoro
    } else {                                       // si entra, pruebo las dos opciones
        int valorDePoner = valor + mochila01Memo(objetoHasta - 1, capacidadRestante - peso, memo);   // lo pongo
        int valorDeNoPoner = mochila01Memo(objetoHasta - 1, capacidadRestante, memo);                // no lo pongo
        resultado = max(valorDePoner, valorDeNoPoner);   // la mejor de las dos
    }                                              // fin del if entra

    memo[objetoHasta][capacidadRestante] = resultado;    // lo anoto para la proxima vez
    return resultado;                              // y lo devuelvo
}                                                  // fin de mochila01Memo

int main() {                                       // programa principal
    int N, capacidad;                              // cantidad de objetos y capacidad de la mochila
    cin >> N >> capacidad;                         // lee la primera linea de la entrada

    pesos = new int[N + 1];                        // indices de 1 a N (la posicion 0 no se usa)
    valores = new int[N + 1];                      // indices de 1 a N (la posicion 0 no se usa)
    for (int i = 1; i <= N; i++) {                 // para cada objeto del 1 al N...
        cin >> pesos[i] >> valores[i];             // ...lee su peso y su valor
    }                                              // fin del for de lectura

    llamadas = 0;                                  // reinicia el contador
    int valorRec = mochila01(N, capacidad);        // recursiva sin guardar, empezando por el ultimo objeto
    long long llamadasRec = llamadas;              // guarda cuantas llamadas hizo

    int** memo = new int*[N + 1];                  // memo de (N+1) x (capacidad+1)
    for (int i = 0; i <= N; i++) {                 // para cada fila...
        memo[i] = new int[capacidad + 1];          // ...la crea
        for (int c = 0; c <= capacidad; c++) {     // y para cada columna...
            memo[i][c] = -1;                       // ...marca "todavia no calculado"
        }                                          // fin del for de columnas
    }                                              // fin del for de filas

    llamadas = 0;                                  // reinicia el contador
    int valorMemo = mochila01Memo(N, capacidad, memo);   // recursiva con memoizacion
    long long llamadasMemo = llamadas;             // guarda cuantas llamadas hizo

    cout << "recursiva: " << valorRec << " (" << llamadasRec << " llamadas)" << endl;      // resultado sin guardar
    cout << "memoizada: " << valorMemo << " (" << llamadasMemo << " llamadas)" << endl;   // resultado con memo

    for (int i = 0; i <= N; i++) {                 // libera cada fila del memo
        delete[] memo[i];                          // borra la fila
    }                                              // fin del for
    delete[] memo;                                 // borra el array de punteros
    delete[] pesos;                                // libera los pesos
    delete[] valores;                              // libera los valores
    return 0;                                      // termina sin error
}                                                  // fin de main
