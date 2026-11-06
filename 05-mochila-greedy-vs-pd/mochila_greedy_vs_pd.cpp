// Mochila: greedy vs programacion dinamica.
// Explicacion completa paso a paso en Letra.md.
//
// Resuelve la misma instancia de tres formas:
//   1. Mochila 0/1 con greedy por ratio (valor/peso)  -> rapido, pero NO garantiza el optimo.
//   2. Mochila 0/1 con programacion dinamica           -> optimo, con reconstruccion.
//   3. Mochila fraccionaria con greedy por ratio       -> optimo (en esta variante greedy SI funciona).
#include <iostream>
#include <iomanip>
using namespace std;

struct Objeto {
    int numero;   // posicion en la entrada (1..N), para imprimir cuales se usan
    int peso;
    int valor;
};

// a tiene mejor ratio que b?  Quiere decir a.valor/a.peso > b.valor/b.peso.
// Se multiplica cruzado para no usar division entera (que trunca).
// Los pesos son positivos, asi que la desigualdad no cambia de sentido.
bool mejorRatio(Objeto a, Objeto b) {
    return (long long)a.valor * b.peso > (long long)b.valor * a.peso;
}

// ---------------------------------------------------------------------------
// Ordenamiento: mergeSort de mayor a menor ratio. O(N log N).
// Es estable: ante un empate de ratio toma primero el de la izquierda,
// asi gana el que aparece primero en la entrada.
// ---------------------------------------------------------------------------
void merge(Objeto* arr, int izq, int medio, int der) {
    int n1 = medio - izq + 1;
    int n2 = der - medio;
    Objeto* L = new Objeto[n1];
    Objeto* R = new Objeto[n2];
    for (int i = 0; i < n1; i++) L[i] = arr[izq + i];
    for (int j = 0; j < n2; j++) R[j] = arr[medio + 1 + j];

    int i = 0, j = 0, k = izq;
    while (i < n1 && j < n2) {
        // Solo paso el de la derecha si es ESTRICTAMENTE mejor (estabilidad).
        if (mejorRatio(R[j], L[i])) {
            arr[k] = R[j];
            j++;
        } else {
            arr[k] = L[i];
            i++;
        }
        k++;
    }
    while (i < n1) { arr[k] = L[i]; i++; k++; }
    while (j < n2) { arr[k] = R[j]; j++; k++; }

    delete[] L;
    delete[] R;
}

void mergeSort(Objeto* arr, int izq, int der) {
    if (izq < der) {
        int medio = izq + (der - izq) / 2;
        mergeSort(arr, izq, medio);
        mergeSort(arr, medio + 1, der);
        merge(arr, izq, medio, der);
    }
}

// Devuelve una copia de los objetos ordenada por ratio (el original no se toca).
Objeto* ordenarPorRatio(Objeto* objetos, int N) {
    Objeto* orden = new Objeto[N];
    for (int i = 0; i < N; i++) orden[i] = objetos[i];
    mergeSort(orden, 0, N - 1);
    return orden;
}

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

// usados[i] == true si el objeto numero i (1..N) se lleva.
// Se recorre en orden, asi que los numeros salen de menor a mayor.
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

// ---------------------------------------------------------------------------
// 1. GREEDY 0/1 POR RATIO
// Recorre los objetos de mejor a peor ratio. Si el objeto entra en lo que queda,
// lo mete entero; si no entra, lo saltea y SIGUE probando con los siguientes
// (uno peor puede entrar). Nunca vuelve atras sobre una decision.
// O(N log N) por el ordenamiento.
// ---------------------------------------------------------------------------
int greedy01(Objeto* objetos, int N, int capacidad, bool* usados) {
    Objeto* orden = ordenarPorRatio(objetos, N);
    int libre = capacidad;
    int total = 0;
    for (int i = 0; i < N; i++) {
        if (orden[i].peso <= libre) {          // entra: lo meto entero
            libre -= orden[i].peso;
            total += orden[i].valor;
            usados[orden[i].numero] = true;
        }
        // no entra: lo salteo. Los kilos que queden libres al final se desperdician.
    }
    delete[] orden;
    return total;
}

// ---------------------------------------------------------------------------
// 2. PROGRAMACION DINAMICA 0/1 (tabulacion)
// tab[i][c] = maximo valor usando solo los objetos 1..i con capacidad c.
//   tab[0][c] = 0                                   (sin objetos)
//   tab[i][c] = tab[i-1][c]                         si peso_i > c   (no entra)
//   tab[i][c] = max(tab[i-1][c],                    no lo pongo
//                   valor_i + tab[i-1][c - peso_i]) lo pongo (y no lo puedo repetir)
// Se guarda la tabla entera (no una sola fila) para poder reconstruir.
// O(N * C) tiempo y espacio.
// ---------------------------------------------------------------------------
int pd01(Objeto* objetos, int N, int capacidad, bool* usados) {
    // Los "()" de initMatriz dejan todo en 0: la fila 0 ya es el caso base.
    int ** tab = initMatriz(N + 1, capacidad + 1);

    for (int i = 1; i <= N; i++) {
        int peso = objetos[i - 1].peso;
        int valor = objetos[i - 1].valor;
        for (int c = 0; c <= capacidad; c++) {
            tab[i][c] = tab[i - 1][c];                                  // no ponerlo
            if (peso <= c) {
                tab[i][c] = max(tab[i][c], valor + tab[i - 1][c - peso]);   // ponerlo
            }
        }
    }

    // Reconstruccion: desde tab[N][capacidad] hacia arriba.
    // Si el valor cambia respecto de la fila anterior, el objeto i se uso.
    int c = capacidad;
    for (int i = N; i >= 1; i--) {
        if (tab[i][c] != tab[i - 1][c]) {
            usados[objetos[i - 1].numero] = true;
            c -= objetos[i - 1].peso;
        }
    }

    int resultado = tab[N][capacidad];
    deleteMatriz(tab, N + 1);
    return resultado;
}

// ---------------------------------------------------------------------------
// 3. GREEDY FRACCIONARIO POR RATIO
// Igual que el greedy 0/1, pero cuando el siguiente objeto no entra entero
// se lleva el pedazo que entra y se termina: la mochila queda llena.
// En esta variante el greedy es optimo (ver demostracion en Letra.md).
// O(N log N).
// ---------------------------------------------------------------------------
double greedyFraccionario(Objeto* objetos, int N, int capacidad) {
    Objeto* orden = ordenarPorRatio(objetos, N);
    int libre = capacidad;
    double total = 0;
    for (int i = 0; i < N && libre > 0; i++) {
        if (orden[i].peso <= libre) {          // entra entero
            libre -= orden[i].peso;
            total += orden[i].valor;
        } else {                               // entra un pedazo: libre/peso del objeto
            total += (double)orden[i].valor * libre / orden[i].peso;
            libre = 0;
        }
    }
    delete[] orden;
    return total;
}

int main() {
    int N, capacidad;
    cin >> N >> capacidad;
    Objeto* objetos = new Objeto[N];
    for (int i = 0; i < N; i++) {
        objetos[i].numero = i + 1;
        cin >> objetos[i].peso >> objetos[i].valor;
    }

    // Arrays de N+1 para indexar por numero de objeto (1..N). Los "()" los dejan en false.
    bool* usadosGreedy = new bool[N + 1]();
    bool* usadosPD = new bool[N + 1]();

    int valorGreedy = greedy01(objetos, N, capacidad, usadosGreedy);
    int valorPD = pd01(objetos, N, capacidad, usadosPD);
    double valorFrac = greedyFraccionario(objetos, N, capacidad);

    cout << "greedy 0/1: " << valorGreedy << " ";
    imprimirUsados(usadosGreedy, N);
    cout << endl;

    cout << "pd 0/1: " << valorPD << " ";
    imprimirUsados(usadosPD, N);
    cout << endl;

    cout << fixed << setprecision(2);
    cout << "greedy fraccionaria: " << valorFrac << endl;

    delete[] objetos;
    delete[] usadosGreedy;
    delete[] usadosPD;
    return 0;
}
