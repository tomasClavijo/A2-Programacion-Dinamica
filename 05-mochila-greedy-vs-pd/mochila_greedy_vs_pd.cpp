// Mochila: greedy vs programacion dinamica.
// Explicacion completa paso a paso en Letra.md.
//
// Resuelve la misma instancia de tres formas:
//   1. Mochila 0/1 con greedy por ratio (valor/peso)  -> rapido, pero NO garantiza el optimo.
//   2. Mochila 0/1 con programacion dinamica           -> optimo, con reconstruccion.
//   3. Mochila fraccionaria con greedy por ratio       -> optimo (en esta variante greedy SI funciona).
#include <algorithm>
#include <cstdio>
#include <iostream>
#include <vector>
using namespace std;

struct Objeto {
    int numero;   // posicion en la entrada (1..N), para imprimir cuales se usan
    int peso;
    int valor;
};

// a tiene mejor ratio que b?  Quiere decir a.valor/a.peso > b.valor/b.peso.
// Se multiplica cruzado para no usar division entera (que trunca).
// Los pesos son positivos, asi que la desigualdad no cambia de sentido.
bool mejorRatio(const Objeto& a, const Objeto& b) {
    return (long long)a.valor * b.peso > (long long)b.valor * a.peso;
}

// Devuelve los objetos ordenados de mejor a peor ratio.
// stable_sort: ante un empate de ratio se respeta el orden de la entrada
// (gana el que aparece primero), asi el resultado es reproducible.
vector<Objeto> ordenarPorRatio(const vector<Objeto>& objetos) {
    vector<Objeto> orden = objetos;
    stable_sort(orden.begin(), orden.end(), mejorRatio);
    return orden;
}

void imprimirUsados(const vector<int>& usados) {
    cout << "[";
    for (size_t i = 0; i < usados.size(); i++) {
        if (i > 0) cout << " ";
        cout << usados[i];
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
int greedy01(const vector<Objeto>& objetos, int capacidad, vector<int>& usados) {
    int libre = capacidad;
    int total = 0;
    for (const Objeto& o : ordenarPorRatio(objetos)) {
        if (o.peso <= libre) {          // entra: lo meto entero
            libre -= o.peso;
            total += o.valor;
            usados.push_back(o.numero);
        }
        // no entra: lo salteo. Los kilos que queden libres al final se desperdician.
    }
    sort(usados.begin(), usados.end());
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
int pd01(const vector<Objeto>& objetos, int capacidad, vector<int>& usados) {
    int n = objetos.size();
    vector<vector<int>> tab(n + 1, vector<int>(capacidad + 1, 0));  // fila 0 = caso base

    for (int i = 1; i <= n; i++) {
        const Objeto& o = objetos[i - 1];
        for (int c = 0; c <= capacidad; c++) {
            tab[i][c] = tab[i - 1][c];                                     // no ponerlo
            if (o.peso <= c) {
                tab[i][c] = max(tab[i][c], o.valor + tab[i - 1][c - o.peso]);  // ponerlo
            }
        }
    }

    // Reconstruccion: desde tab[n][capacidad] hacia arriba.
    // Si el valor cambia respecto de la fila anterior, el objeto i se uso.
    int c = capacidad;
    for (int i = n; i >= 1; i--) {
        if (tab[i][c] != tab[i - 1][c]) {
            usados.push_back(objetos[i - 1].numero);
            c -= objetos[i - 1].peso;
        }
    }
    sort(usados.begin(), usados.end());
    return tab[n][capacidad];
}

// ---------------------------------------------------------------------------
// 3. GREEDY FRACCIONARIO POR RATIO
// Igual que el greedy 0/1, pero cuando el siguiente objeto no entra entero
// se lleva el pedazo que entra y se termina: la mochila queda llena.
// En esta variante el greedy es optimo (ver demostracion en Letra.md).
// O(N log N).
// ---------------------------------------------------------------------------
double greedyFraccionario(const vector<Objeto>& objetos, int capacidad) {
    int libre = capacidad;
    double total = 0;
    for (const Objeto& o : ordenarPorRatio(objetos)) {
        if (libre == 0) break;
        if (o.peso <= libre) {          // entra entero
            libre -= o.peso;
            total += o.valor;
        } else {                        // entra un pedazo: libre/peso del objeto
            total += (double)o.valor * libre / o.peso;
            libre = 0;
        }
    }
    return total;
}

int main() {
    int n, capacidad;
    cin >> n >> capacidad;
    vector<Objeto> objetos(n);
    for (int i = 0; i < n; i++) {
        objetos[i].numero = i + 1;
        cin >> objetos[i].peso >> objetos[i].valor;
    }

    vector<int> usadosGreedy, usadosPD;
    int valorGreedy = greedy01(objetos, capacidad, usadosGreedy);
    int valorPD = pd01(objetos, capacidad, usadosPD);
    double valorFrac = greedyFraccionario(objetos, capacidad);

    cout << "greedy 0/1: " << valorGreedy << " ";
    imprimirUsados(usadosGreedy);
    cout << endl;

    cout << "pd 0/1: " << valorPD << " ";
    imprimirUsados(usadosPD);
    cout << endl;

    printf("greedy fraccionaria: %.2f\n", valorFrac);
    return 0;
}
