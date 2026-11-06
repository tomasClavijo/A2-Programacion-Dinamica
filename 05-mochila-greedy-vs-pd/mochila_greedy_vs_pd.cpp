// Mochila 0/1: greedy vs programacion dinamica.
// Explicacion en Letra.md y linea por linea en CODIGO.md.
#include <iostream>
using namespace std;

int max(int a, int b) {
    return a > b ? a : b;
}

int ** initMatriz(int filas, int columnas) {
    int ** m = new int*[filas]();
    for (int i = 0; i < filas; i++) {
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

// Imprime los numeros de los objetos marcados en usados, de menor a mayor.
void imprimirUsados(bool* usados, int N) {
    cout << "[";
    bool primero = true;
    for (int i = 1; i <= N; i++) {
        if (usados[i]) {
            if (!primero) cout << " ";
            cout << i;
            primero = false;
        }
    }
    cout << "]";
}

// GREEDY: en cada vuelta mete el objeto de mejor ratio (valor/peso) que todavia entra.
// Nunca saca un objeto que ya metio.
int greedy(int* pesos, int* valores, int N, int capacidad, bool* usados) {
    int libre = capacidad;
    int total = 0;
    while (true) {
        int elegido = -1;                         // -1 = todavia no hay candidato
        for (int i = 1; i <= N; i++) {
            if (!usados[i] && pesos[i] <= libre) {
                double ratio = (double)valores[i] / pesos[i];
                if (elegido == -1 || ratio > (double)valores[elegido] / pesos[elegido]) {
                    elegido = i;
                }
            }
        }
        if (elegido == -1) {                      // no entra nada mas
            return total;
        }
        usados[elegido] = true;
        libre -= pesos[elegido];
        total += valores[elegido];
    }
}

// PROGRAMACION DINAMICA:
// tab[i][c] = maximo valor usando solo los objetos 1..i con capacidad c.
int pd(int* pesos, int* valores, int N, int capacidad, bool* usados) {
    int ** tab = initMatriz(N + 1, capacidad + 1);   // todo en 0: la fila 0 es el caso base

    for (int i = 1; i <= N; i++) {
        for (int c = 0; c <= capacidad; c++) {
            tab[i][c] = tab[i - 1][c];                                       // no lo pongo
            if (pesos[i] <= c) {
                tab[i][c] = max(tab[i][c], valores[i] + tab[i - 1][c - pesos[i]]);  // lo pongo
            }
        }
    }

    // Reconstruccion: si el valor cambia respecto de la fila de arriba, el objeto se uso.
    int c = capacidad;
    for (int i = N; i >= 1; i--) {
        if (tab[i][c] != tab[i - 1][c]) {
            usados[i] = true;
            c -= pesos[i];
        }
    }

    int resultado = tab[N][capacidad];
    deleteMatriz(tab, N + 1);
    return resultado;
}

int main() {
    int N, capacidad;
    cin >> N >> capacidad;

    // Indices de 1 a N (la posicion 0 no se usa), igual que la tabla.
    int* pesos = new int[N + 1];
    int* valores = new int[N + 1];
    for (int i = 1; i <= N; i++) {
        cin >> pesos[i] >> valores[i];
    }

    bool* usadosGreedy = new bool[N + 1]();
    bool* usadosPD = new bool[N + 1]();

    int valorGreedy = greedy(pesos, valores, N, capacidad, usadosGreedy);
    int valorPD = pd(pesos, valores, N, capacidad, usadosPD);

    cout << "greedy: " << valorGreedy << " ";
    imprimirUsados(usadosGreedy, N);
    cout << endl;

    cout << "pd: " << valorPD << " ";
    imprimirUsados(usadosPD, N);
    cout << endl;

    delete[] pesos;
    delete[] valores;
    delete[] usadosGreedy;
    delete[] usadosPD;
    return 0;
}
