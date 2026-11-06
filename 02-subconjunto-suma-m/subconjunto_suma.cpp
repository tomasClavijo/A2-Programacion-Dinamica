#include <cassert>
#include <string>
#include <iostream>
#include <limits>
using namespace std;

bool ** initMatriz(int cantidad, int cota)
{
    bool **m = new bool* [cantidad]();
    for (int i = 0; i < cantidad; i++){   // antes: i <= cantidad (escribía fuera del array)
        m[i] = new bool[cota+1]();
    }
    return m;
};

bool ** resolucion(int cantidad, int cota, int* objetos)
{
    bool** tab = initMatriz(cantidad, cota);
    for (int i = 0; i < cantidad; i++){
        tab[i][0] = false;
    }
    for(int j = 1; j <= cota; j++){
        if(j % objetos[0] == 0){
            tab[0][j] = true;
        }else{
            tab[0][j] = false;
        }
    }
    for(int i = 1; i< cantidad; i++){
        for (int j = 1; j <= cota; j++){
            if (j < objetos[i]){
                tab[i][j] = tab[i-1][j];
            } else if (j == objetos[i]){
                tab[i][j] = true;
            } else
            {
                tab[i][j] = tab[i-1][j] || tab[i][j-objetos[i]];
            }
        }
    }
    return tab;
}

int main()
{
    int N;
    cin >> N;
    int* objetos = new int [N];
    int elemento;
    for (int i = 0; i < N; i++){
        cin >> elemento;
        objetos[i] = elemento;
    }

    int cota;
    cin >> cota;

    int P;
    cin >> P;
    int* pruebas = new int [P];
    int prueba;
    for (int i = 0; i < P; i ++){
        cin >> prueba;
        pruebas[i] = prueba;
    }

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