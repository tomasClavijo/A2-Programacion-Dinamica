#include <iostream>
using namespace std;

// Avanza el triangulo de Pascal desde la fila "filaActual" hasta la fila "n".
// "actual" siempre guarda la fila filaActual; "aux" es un buffer de trabajo.
// Cada fila nueva se calcula solo a partir de la anterior: C(n,k) = C(n-1,k-1) + C(n-1,k).
void avanzarHasta(long long n, long long N, long long& filaActual, long long*& actual, long long*& aux) {
    while (filaActual < n) {
        aux[0] = 1;
        for (long long i = 1; i <= N; i++)
            aux[i] = actual[i - 1] + actual[i];

        long long* tmp = actual;
        actual = aux;
        aux = tmp;
        filaActual++;
    }
}

int main() {
    long long N, P;
    cin >> N;
    cin >> P;

    long long* actual = new long long[N + 1]();
    long long* aux = new long long[N + 1]();
    actual[0] = 1;              // fila 0: C(0,0) = 1
    long long filaActual = 0;

    for (long long i = 0; i < P; i++) {
        long long ni, ki;
        cin >> ni;
        cin >> ki;

        // Las consultas vienen ordenadas por n (no decreciente),
        // asi que solo hace falta avanzar.
        avanzarHasta(ni, N, filaActual, actual, aux);
        cout << actual[ki] << endl;
    }

    delete[] actual;
    delete[] aux;
    return 0;
}
