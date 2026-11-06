# El código línea por línea: `mochila_greedy_vs_pd.cpp`

> La letra y las ideas están en [Letra.md](Letra.md). Si la PD en general todavía no cierra, conviene leer primero [PD desde cero](../PD-DESDE-CERO.md).

Los números de línea son los del archivo [`mochila_greedy_vs_pd.cpp`](mochila_greedy_vs_pd.cpp). Todas las trazas usan la entrada del ejemplo (capacidad 7):

```
4 7
1 6     <- objeto 1 (A): peso 1, valor 6
2 6     <- objeto 2 (B): peso 2, valor 6
5 9     <- objeto 3 (C): peso 5, valor 9
7 13    <- objeto 4 (D): peso 7, valor 13
```

Mapa del archivo:

| Líneas | Qué hay |
|---|---|
| 6–37 | Funciones auxiliares: `max`, crear y borrar la matriz, imprimir |
| 41–61 | **Greedy** |
| 65–89 | **PD** |
| 91–121 | `main` |

---

## 1. `main`: cómo se guardan los datos (líneas 91–121)

Conviene empezar por acá para ver cómo se guardan los objetos.

```cpp
92      int N, capacidad;
93      cin >> N >> capacidad;
```

- **93**: lee la primera línea (`4 7`): 4 objetos y capacidad 7.

```cpp
96      int* pesos = new int[N + 1];
97      int* valores = new int[N + 1];
98      for (int i = 1; i <= N; i++) {
99          cin >> pesos[i] >> valores[i];
100     }
```

- **96–97**: dos arrays, uno para los pesos y otro para los valores. Tienen tamaño `N + 1` para usar las posiciones **1 a N**. La posición 0 no se usa.
- **98–99**: lee cada objeto. Quedan así:

| i | 0 | 1 (A) | 2 (B) | 3 (C) | 4 (D) |
|---|---|---|---|---|---|
| `pesos[i]` | — | 1 | 2 | 5 | 7 |
| `valores[i]` | — | 6 | 6 | 9 | 13 |

**Por qué de 1 a N:** así el objeto `i` es la fila `i` de la tabla de PD, y no hay que andar restando 1. Es lo mismo que hace el [ejercicio 01](../01-mochila-01-ejemplo/mochila01.cpp), con su "objeto 0" vacío.

```cpp
102     bool* usadosGreedy = new bool[N + 1]();
103     bool* usadosPD = new bool[N + 1]();
```

- **102–103**: una marca por objeto, para saber cuáles se llevan. Hay un array para cada algoritmo. Los `()` hacen que todas arranquen en `false`.

```cpp
105     int valorGreedy = greedy(pesos, valores, N, capacidad, usadosGreedy);
106     int valorPD = pd(pesos, valores, N, capacidad, usadosPD);
```

- **105–106**: corre los dos algoritmos con los mismos datos. Cada uno devuelve el valor total y marca en su array qué objetos usó.

```cpp
108     cout << "greedy: " << valorGreedy << " ";
109     imprimirUsados(usadosGreedy, N);
110     cout << endl;
112     cout << "pd: " << valorPD << " ";
113     imprimirUsados(usadosPD, N);
114     cout << endl;
```

- Imprime `greedy: 12 [1 2]` y `pd: 15 [1 3]`.

```cpp
116     delete[] pesos;
117     delete[] valores;
118     delete[] usadosGreedy;
119     delete[] usadosPD;
```

- Libera todo lo que se pidió con `new`.

---

## 2. Funciones auxiliares (líneas 6–37)

```cpp
6   int max(int a, int b) {
7       return a > b ? a : b;
8   }
```

- **7**: devuelve el mayor de los dos. `cond ? x : y` quiere decir "si `cond`, `x`; si no, `y`".

```cpp
10  int ** initMatriz(int filas, int columnas) {
11      int ** m = new int*[filas]();
12      for (int i = 0; i < filas; i++) {
13          m[i] = new int[columnas]();
14      }
15      return m;
16  }
```

- **11**: un array de `filas` punteros. Cada puntero va a ser una fila.
- **13**: cada fila es un array de `columnas` enteros. Los **`()`** hacen que arranque **todo en 0**, y eso importa para la PD (ver línea 66).
- Se usa como `m[fila][columna]`.

```cpp
18  void deleteMatriz(int** matriz, int filas) {
19      for (int i = 0; i < filas; i++) {
20          delete[] matriz[i];
21      }
22      delete[] matriz;
23  }
```

- **19–21**: borra cada fila.
- **22**: borra el array de punteros. Va al final, porque si se borra primero se pierden las direcciones de las filas.

```cpp
26  void imprimirUsados(bool* usados, int N) {
27      cout << "[";
28      bool primero = true;
29      for (int i = 1; i <= N; i++) {
30          if (usados[i]) {
31              if (!primero) cout << " ";
32              cout << i;
33              primero = false;
34          }
35      }
36      cout << "]";
37  }
```

- **29**: recorre del 1 al N, así que los números salen ordenados.
- **31–33**: pone un espacio **entre** números, pero no antes del primero. Con A y C imprime `[1 3]`.

---

## 3. Greedy (líneas 41–61)

La idea: **en cada vuelta, meter el objeto de mejor ratio que todavía entre, y nunca sacar nada.**

```cpp
41  int greedy(int* pesos, int* valores, int N, int capacidad, bool* usados) {
42      int libre = capacidad;
43      int total = 0;
```

- **42**: `libre` = cuántos kilos quedan en la mochila. Arranca en 7.
- **43**: `total` = valor acumulado. Arranca en 0.

```cpp
44      while (true) {
45          int elegido = -1;
```

- **44**: cada vuelta del `while` mete **un** objeto. Se sale con el `return` de la línea 55.
- **45**: `elegido` es el mejor candidato encontrado en esta vuelta. `-1` quiere decir "todavía ninguno".

```cpp
46          for (int i = 1; i <= N; i++) {
47              if (!usados[i] && pesos[i] <= libre) {
48                  double ratio = (double)valores[i] / pesos[i];
49                  if (elegido == -1 || ratio > (double)valores[elegido] / pesos[elegido]) {
50                      elegido = i;
51                  }
52              }
53          }
```

- **46**: revisa todos los objetos.
- **47**: solo considera los que **no se usaron** y que **entran** en lo que queda.
- **48**: el ratio del objeto `i`. El `(double)` es necesario porque, con `int`, `9 / 5` da `1` en lugar de `1,8`.
- **49**: se queda con `i` si es el primer candidato (`elegido == -1`) o si su ratio es **mayor** que el del mejor hasta ahora. Como es `>` y no `>=`, ante un empate se queda el que ya estaba, que es el que aparece primero.
- **50**: `i` pasa a ser el mejor candidato.

```cpp
54          if (elegido == -1) {
55              return total;
56          }
```

- **54–55**: si después de revisar todos no hubo ningún candidato, no entra nada más y se termina. Los kilos libres que queden se desperdician.

```cpp
57          usados[elegido] = true;
58          libre -= pesos[elegido];
59          total += valores[elegido];
60      }
```

- **57**: marca el objeto (no se puede volver a elegir, y además queda anotado para imprimirlo).
- **58**: descuenta su peso.
- **59**: suma su valor.
- Vuelve al `while` a buscar el siguiente. **No hay ninguna línea que saque un objeto**: por eso es greedy.

**Traza con capacidad 7:**

| Vuelta del `while` | Línea 47: candidatos (no usados y `peso <= libre`) | Líneas 48–50: ratios | `elegido` | `libre` | `total` |
|---|---|---|---|---|---|
| — | | | | 7 | 0 |
| 1 | A (1 ≤ 7), B (2 ≤ 7), C (5 ≤ 7), D (7 ≤ 7) | 6; 3; 1,8; 1,86 | 1 (A) | 6 | 6 |
| 2 | B (2 ≤ 6), C (5 ≤ 6). D no (7 > 6) | 3; 1,8 | 2 (B) | 4 | 12 |
| 3 | ninguno: C (5 > 4), D (7 > 4) | — | −1 | 4 | `return 12` |

En la vuelta 2, el greedy compara B contra C **por ratio** y gana B. No tiene forma de saber que C, sumado a A, daba más.

---

## 4. PD (líneas 65–89)

La idea en una frase:

> Se arma una tabla donde **`tab[i][c]` = el mejor valor posible usando solo los objetos 1..i, con una mochila de capacidad `c`**. Se llena celda por celda, siempre a partir de celdas que ya están llenas, hasta llegar a `tab[N][capacidad]`, que es la respuesta.

### Crear la tabla (línea 66)

```cpp
66      int ** tab = initMatriz(N + 1, capacidad + 1);
```

- **Filas: `N + 1`** (de 0 a N). La fila `i` significa "puedo usar los objetos 1..i". La fila **0** es "no tengo ningún objeto".
- **Columnas: `capacidad + 1`** (de 0 a C). La columna `c` significa "la mochila aguanta `c` kilos".
- Las dos tienen **+1** para que existan la fila 0 y la columna 0.
- Como `initMatriz` deja **todo en 0**, la fila 0 ya está bien: sin objetos, el valor es 0. Ese es el **caso base**, y no hace falta escribir nada más para cargarlo.

| i \ c | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 |
|---|---|---|---|---|---|---|---|---|
| 0 (nada) | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
| 1 A | ? | ? | ? | ? | ? | ? | ? | ? |
| 2 B | ? | ? | ? | ? | ? | ? | ? | ? |
| 3 C | ? | ? | ? | ? | ? | ? | ? | ? |
| 4 D | ? | ? | ? | ? | ? | ? | ? | ? |

### Los dos `for` (líneas 68–69)

```cpp
68      for (int i = 1; i <= N; i++) {
69          for (int c = 0; c <= capacidad; c++) {
```

- **68**: recorre las filas **de arriba hacia abajo**, desde la 1 (la 0 ya está). En cada vuelta se agrega un objeto más a los que se pueden usar.
- **69**: recorre todas las capacidades de esa fila, de 0 a C.

Este orden funciona porque cada celda de la fila `i` solo lee celdas de la fila `i - 1`, que se terminó de llenar en la vuelta anterior.

### La decisión: ¿lo pongo o no? (líneas 70–73)

Estas cuatro líneas **son** la programación dinámica:

```cpp
70              tab[i][c] = tab[i - 1][c];
71              if (pesos[i] <= c) {
72                  tab[i][c] = max(tab[i][c], valores[i] + tab[i - 1][c - pesos[i]]);
73              }
```

- **70**, **opción "no lo pongo"**. Si no pongo el objeto `i`, es como si no existiera, así que lo mejor es lo que ya se lograba con los objetos anteriores y la misma capacidad: `tab[i - 1][c]`, la celda **de arriba**. Se guarda como valor provisorio.
- **71**: ¿el objeto entra en `c` kilos? Si no entra, queda el valor de la línea 70.
- **72**, **opción "lo pongo"**. Gano `valores[i]` y me quedan `c - pesos[i]` kilos para los objetos **anteriores**. Lo mejor con eso es `tab[i - 1][c - pesos[i]]`: la fila de arriba, corrida `pesos[i]` columnas a la izquierda. El `max` se queda con la mejor de las dos opciones.

Por qué en la 72 es `i - 1` y no `i`: cada objeto se usa **una sola vez**. Si pongo el `i`, para el resto solo quedan los anteriores.

Qué celdas lee `tab[i][c]`:

```
fila i-1:   ... [c - pesos[i]] ........ [c]      <- las dos ya están calculadas
                      \                  |
                       \ + valores[i]    | (tal cual)
                        \                |
fila i:                  -------------> [c]      <- el max de las dos
```

### Traza completa de los `for`

Cada tabla es una vuelta del `for` de la línea 68, y cada renglón es una vuelta del `for` de la línea 69.

**`i = 1`: A (peso 1, valor 6)**

| c | Línea 70: no lo pongo, `tab[0][c]` | Línea 71: ¿`1 <= c`? | Línea 72: lo pongo, `6 + tab[0][c-1]` | `tab[1][c]` |
|---|---|---|---|---|
| 0 | 0 | no | — | **0** |
| 1 | 0 | sí | 6 + tab[0][0] = 6 + 0 = 6 | max(0, 6) = **6** |
| 2 | 0 | sí | 6 + tab[0][1] = 6 + 0 = 6 | max(0, 6) = **6** |
| 3 a 7 | 0 | sí | 6 + 0 = 6 | **6** |

Con solo A, en cualquier mochila de 1 kilo o más se lleva A y vale 6.

**`i = 2`: B (peso 2, valor 6)**

| c | Línea 70: no lo pongo, `tab[1][c]` | Línea 71: ¿`2 <= c`? | Línea 72: lo pongo, `6 + tab[1][c-2]` | `tab[2][c]` |
|---|---|---|---|---|
| 0 | 0 | no | — | **0** |
| 1 | 6 | no | — | **6** |
| 2 | 6 | sí | 6 + tab[1][0] = 6 + 0 = 6 | max(6, 6) = **6** |
| 3 | 6 | sí | 6 + tab[1][1] = 6 + 6 = 12 | max(6, 12) = **12** |
| 4 | 6 | sí | 6 + tab[1][2] = 6 + 6 = 12 | **12** |
| 5 | 6 | sí | 6 + tab[1][3] = 6 + 6 = 12 | **12** |
| 6 | 6 | sí | 6 + tab[1][4] = 6 + 6 = 12 | **12** |
| 7 | 6 | sí | 6 + tab[1][5] = 6 + 6 = 12 | **12** |

En `c = 2` entra A o B, pero no los dos, así que vale 6. Desde `c = 3` entran los dos y vale 12.

**`i = 3`: C (peso 5, valor 9)** ← acá está la clave

| c | Línea 70: no lo pongo, `tab[2][c]` | Línea 71: ¿`5 <= c`? | Línea 72: lo pongo, `9 + tab[2][c-5]` | `tab[3][c]` |
|---|---|---|---|---|
| 0 | 0 | no | — | **0** |
| 1 | 6 | no | — | **6** |
| 2 | 6 | no | — | **6** |
| 3 | 12 | no | — | **12** |
| 4 | 12 | no | — | **12** |
| 5 | 12 | sí | 9 + tab[2][0] = 9 + 0 = 9 | max(12, 9) = **12** |
| 6 | 12 | sí | 9 + tab[2][1] = 9 + 6 = 15 | max(12, 15) = **15** |
| 7 | 12 | sí | 9 + tab[2][2] = 9 + 6 = 15 | max(12, 15) = **15** |

En `c = 7`, la línea 70 deja 12 (A + B, lo mismo que hace el greedy). La línea 72 prueba "C + lo mejor con A y B en los 2 kilos que sobran" = 9 + 6 = 15, y el `max` se queda con 15. **Esta comparación es la que el greedy nunca hace.**

**`i = 4`: D (peso 7, valor 13)**

| c | Línea 70: no lo pongo, `tab[3][c]` | Línea 71: ¿`7 <= c`? | Línea 72: lo pongo, `13 + tab[3][c-7]` | `tab[4][c]` |
|---|---|---|---|---|
| 0 a 5 | 0, 6, 6, 12, 12, 12 | no | — | **0, 6, 6, 12, 12, 12** |
| 6 | 15 | no | — | **15** |
| 7 | 15 | sí | 13 + tab[3][0] = 13 + 0 = 13 | max(15, 13) = **15** |

La tabla al terminar los `for`:

| i \ c | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 |
|---|---|---|---|---|---|---|---|---|
| 0 (nada) | 0 | 0 | 0 | 0  | 0  | 0  | 0  | 0  |
| 1 A      | 0 | 6 | 6 | 6  | 6  | 6  | 6  | 6  |
| 2 B      | 0 | 6 | 6 | 12 | 12 | 12 | 12 | 12 |
| 3 C      | 0 | 6 | 6 | 12 | 12 | 12 | 15 | 15 |
| 4 D      | 0 | 6 | 6 | 12 | 12 | 12 | 15 | **15** |

### Reconstruir qué objetos se usaron (líneas 78–84)

La tabla dice **cuánto** vale lo mejor (15), pero no **qué** objetos. Para saberlo se recorre hacia arriba desde la esquina de abajo a la derecha.

```cpp
78      int c = capacidad;
79      for (int i = N; i >= 1; i--) {
80          if (tab[i][c] != tab[i - 1][c]) {
81              usados[i] = true;
82              c -= pesos[i];
83          }
84      }
```

- **78**: arranca con la mochila entera (`c = 7`).
- **79**: recorre las filas **de abajo hacia arriba**, del objeto N al 1.
- **80**: la pregunta es "¿el valor cambió al agregar el objeto `i`?".
  - Si es igual al de arriba, el objeto `i` no hizo falta, porque sin él se logra lo mismo.
  - Si es distinto, el valor mejoró gracias a `i`, así que **se usó**.
- **81**: lo marca.
- **82**: descuenta su peso. Lo que queda por explicar son los otros objetos en `c - pesos[i]` kilos.

| `i` | `c` | `tab[i][c]` | `tab[i-1][c]` | Línea 80: ¿distintos? | Qué pasa | `c` después |
|---|---|---|---|---|---|---|
| 4 (D) | 7 | 15 | 15 | no | D no se usó | 7 |
| 3 (C) | 7 | 15 | 12 | **sí** | C se usó | 7 − 5 = 2 |
| 2 (B) | 2 | 6 | 6 | no | B no se usó | 2 |
| 1 (A) | 2 | 6 | 0 | **sí** | A se usó | 2 − 1 = 1 |

Resultado: A y C, la salida `[1 3]`. Para comprobarlo: pesan 1 + 5 = 6 ≤ 7 y valen 6 + 9 = 15.

### Devolver y liberar (líneas 86–88)

```cpp
86      int resultado = tab[N][capacidad];
87      deleteMatriz(tab, N + 1);
88      return resultado;
```

- **86**: la respuesta es la esquina de abajo a la derecha, `tab[4][7] = 15`. Se guarda **antes** de borrar la tabla.
- **87**: libera la tabla.
- **88**: devuelve 15.

---

## Resumen para el pizarrón

**Greedy:**

```cpp
while (true) {
    elegido = el de mejor ratio entre los no usados que entran;
    if (no hay ninguno) return total;
    meterlo;                                   // y nunca sacarlo
}
```

**PD:**

```cpp
int ** tab = initMatriz(N + 1, capacidad + 1);           // fila 0 en ceros = caso base
for (int i = 1; i <= N; i++)                             // agrego objetos de a uno
    for (int c = 0; c <= capacidad; c++) {               // para cada capacidad
        tab[i][c] = tab[i - 1][c];                       // opción 1: no lo pongo
        if (pesos[i] <= c)                               // si entra...
            tab[i][c] = max(tab[i][c],                   // ...me quedo con la mejor entre
                            valores[i] + tab[i - 1][c - pesos[i]]);  // opción 2: lo pongo
    }
// respuesta: tab[N][capacidad]
```

La diferencia en una frase: **el greedy elige una sola opción en cada paso, mirando solo el objeto. La PD prueba las dos opciones (ponerlo o no) en cada celda, usando resultados ya calculados, y se queda con la mejor.**
