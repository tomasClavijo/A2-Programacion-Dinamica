# El código línea por línea: `mochila_greedy_vs_pd.cpp`

> La letra y la explicación de las ideas están en [Letra.md](Letra.md). Si la PD en general todavía no cierra, conviene leer primero [PD desde cero](../PD-DESDE-CERO.md).

Los números de línea son los del archivo [`mochila_greedy_vs_pd.cpp`](mochila_greedy_vs_pd.cpp). Para todas las trazas se usa la entrada del ejemplo (capacidad 7):

```
4 7
1 6     <- objeto 1 (A): peso 1, valor 6
2 6     <- objeto 2 (B): peso 2, valor 6
5 9     <- objeto 3 (C): peso 5, valor 9
7 13    <- objeto 4 (D): peso 7, valor 13
```

Mapa del archivo:

| Líneas | Qué hay | ¿Es PD? |
|---|---|---|
| 1–23 | Includes, `struct Objeto`, comparar ratios | No |
| 25–72 | `mergeSort` para ordenar por ratio | No (lo usa el greedy) |
| 74–106 | Funciones auxiliares: `max`, matriz, imprimir | No |
| 108–129 | **Greedy 0/1** | No |
| 131–169 | **PD 0/1** ← lo central | **Sí** |
| 171–193 | **Greedy fraccionario** | No |
| 195–227 | `main` | No |

Para explicar PD, lo único imprescindible son las líneas **141 a 169**. El resto es lectura, ordenamiento e impresión.

---

## 1. Lo básico (líneas 1–23)

```cpp
 8  #include <iostream>
 9  #include <iomanip>
10  using namespace std;
```

- **8**: `cin` y `cout`.
- **9**: `setprecision`, para imprimir el valor fraccionario con 2 decimales (línea 220).
- **10**: permite escribir `cout` en lugar de `std::cout`.

```cpp
12  struct Objeto {
13      int numero;
14      int peso;
15      int valor;
16  };
```

- **12–16**: cada objeto guarda su peso, su valor y su **número** (su posición en la entrada, 1..N). El número hace falta porque el greedy **reordena** los objetos, y al final queremos imprimir "usé el objeto 1 y el 2" según el orden original.

```cpp
21  bool mejorRatio(Objeto a, Objeto b) {
22      return (long long)a.valor * b.peso > (long long)b.valor * a.peso;
23  }
```

- **21**: responde "¿`a` rinde más por kilo que `b`?".
- **22**: lo natural sería `a.valor / a.peso > b.valor / b.peso`, pero con `int` la división **trunca**: 6/2 = 3 está bien, pero 9/5 da 1 en lugar de 1,8. Para evitarlo se multiplican los dos lados por `a.peso * b.peso` (que es positivo, así que la desigualdad no cambia):
  `a.valor * b.peso > b.valor * a.peso`.
  Ejemplo con C (5, 9) y D (7, 13): `9 * 7 = 63` y `13 * 5 = 65`. Como 63 > 65 es falso, C **no** tiene mejor ratio que D (1,8 < 1,86). El `(long long)` evita que la multiplicación se pase del rango de `int` si los números son grandes.

---

## 2. Ordenar por ratio (líneas 25–72)

Es el mismo `mergeSort` del ejercicio 8 de 2023 (plataformas). Lo único que cambia es la comparación. Lo usan los dos greedy, la PD no lo necesita.

```cpp
30  void merge(Objeto* arr, int izq, int medio, int der) {
31      int n1 = medio - izq + 1;
32      int n2 = der - medio;
33      Objeto* L = new Objeto[n1];
34      Objeto* R = new Objeto[n2];
35      for (int i = 0; i < n1; i++) L[i] = arr[izq + i];
36      for (int j = 0; j < n2; j++) R[j] = arr[medio + 1 + j];
```

- **30**: mezcla dos mitades **ya ordenadas** del array: `arr[izq..medio]` y `arr[medio+1..der]`.
- **31–32**: tamaño de cada mitad.
- **33–36**: copia cada mitad a un array auxiliar (`L` = izquierda, `R` = derecha).

```cpp
38      int i = 0, j = 0, k = izq;
39      while (i < n1 && j < n2) {
41          if (mejorRatio(R[j], L[i])) {
42              arr[k] = R[j];
43              j++;
44          } else {
45              arr[k] = L[i];
46              i++;
47          }
48          k++;
49      }
```

- **38**: `i` recorre `L`, `j` recorre `R`, y `k` es la posición de `arr` que se está llenando.
- **39**: mientras queden elementos en las dos mitades...
- **41–43**: ...si el primero de la derecha rinde **estrictamente** más, va ese.
- **44–46**: si no (incluido el empate), va el de la izquierda. Así, ante un empate gana el que estaba antes en la entrada. Esto se llama ordenamiento **estable**, y es lo que pide la letra.
- **48**: se avanza en `arr`.

```cpp
50      while (i < n1) { arr[k] = L[i]; i++; k++; }
51      while (j < n2) { arr[k] = R[j]; j++; k++; }
53      delete[] L;
54      delete[] R;
```

- **50–51**: cuando una mitad se termina, se copia lo que quede de la otra (ya está ordenado).
- **53–54**: se libera la memoria auxiliar.

```cpp
57  void mergeSort(Objeto* arr, int izq, int der) {
58      if (izq < der) {
59          int medio = izq + (der - izq) / 2;
60          mergeSort(arr, izq, medio);
61          mergeSort(arr, medio + 1, der);
62          merge(arr, izq, medio, der);
63      }
64  }
```

- **58**: si el tramo tiene un solo elemento (`izq == der`), ya está ordenado y no se hace nada.
- **59**: el punto medio. Se escribe así en lugar de `(izq + der) / 2` para evitar overflow con índices grandes.
- **60–62**: ordena cada mitad (recursión) y después las mezcla. Es dividir y conquistar, O(N log N).

```cpp
67  Objeto* ordenarPorRatio(Objeto* objetos, int N) {
68      Objeto* orden = new Objeto[N];
69      for (int i = 0; i < N; i++) orden[i] = objetos[i];
70      mergeSort(orden, 0, N - 1);
71      return orden;
72  }
```

- **68–69**: hace una **copia**, porque la PD necesita los objetos en el orden original.
- **70**: ordena la copia. Con el ejemplo queda A (6), B (3), D (1,86), C (1,8).
- **71**: devuelve la copia. Quien la pide tiene que liberarla con `delete[]` (líneas 127 y 191).

---

## 3. Funciones auxiliares (líneas 74–106)

```cpp
74  int max(int a, int b) {
75      return a > b ? a : b;
76  }
```

- **75**: devuelve el mayor de los dos. `cond ? x : y` significa "si `cond`, `x`; si no, `y`". La PD lo usa para elegir entre poner y no poner un objeto.

```cpp
78  int ** initMatriz(int filas, int columnas) {
79      int ** m = new int*[filas]();
80      for (int i = 0; i < filas; i++) {
81          m[i] = new int[columnas]();
82      }
83      return m;
84  }
```

- **79**: un array de `filas` punteros. Cada puntero va a ser una fila.
- **81**: cada fila es un array de `columnas` enteros. Los **`()`** del final hacen que arranque **todo en 0**, y eso es importante para la PD (ver línea 143).
- Se accede como `m[fila][columna]`.

```cpp
86  void deleteMatriz(int** matriz, int filas) {
87      for (int i = 0; i < filas; i++) {
88          delete[] matriz[i];
89      }
90      delete[] matriz;
91  }
```

- **87–89**: libera cada fila.
- **90**: libera el array de punteros. Va en ese orden: si se borrara primero el array de punteros, se perderían las direcciones de las filas.

```cpp
95  void imprimirUsados(bool* usados, int N) {
96      cout << "[";
97      bool primero = true;
98      for (int i = 1; i <= N; i++) {
99          if (usados[i]) {
100             if (!primero) cout << " ";
101             cout << i;
102             primero = false;
103         }
104     }
105     cout << "]";
106 }
```

- **95**: `usados[i]` es `true` si el objeto número `i` se lleva.
- **98**: recorre del 1 al N, así que los números salen ordenados.
- **100–102**: pone un espacio **entre** números, pero no antes del primero. Con A y C imprime `[1 3]`.

---

## 4. Greedy 0/1 (líneas 115–129)

```cpp
115 int greedy01(Objeto* objetos, int N, int capacidad, bool* usados) {
116     Objeto* orden = ordenarPorRatio(objetos, N);
117     int libre = capacidad;
118     int total = 0;
```

- **115**: recibe los objetos y marca en `usados` cuáles lleva. Devuelve el valor total.
- **116**: copia de los objetos ordenada de mejor a peor ratio.
- **117**: `libre` = cuántos kilos quedan en la mochila. Arranca llena de espacio.
- **118**: `total` = valor acumulado.

```cpp
119     for (int i = 0; i < N; i++) {
120         if (orden[i].peso <= libre) {
121             libre -= orden[i].peso;
122             total += orden[i].valor;
123             usados[orden[i].numero] = true;
124         }
126     }
```

- **119**: recorre los objetos **una sola vez**, en orden de ratio.
- **120**: ¿el objeto entra en lo que queda?
- **121–123**: si entra, lo mete: descuenta su peso, suma su valor y lo marca. Se marca `orden[i].numero` (el número original), no `i` (la posición en el orden por ratio).
- Si no entra, no hace nada y **sigue** con el siguiente. **No hay `break`**, porque uno más liviano podría entrar. Tampoco hay forma de deshacer una elección: eso es lo que lo hace greedy.

Traza con el ejemplo:

| `i` | `orden[i]` | Línea 120: ¿`peso <= libre`? | `libre` | `total` |
|---|---|---|---|---|
| — | | | 7 | 0 |
| 0 | A (1, 6) | 1 ≤ 7, sí | 6 | 6 |
| 1 | B (2, 6) | 2 ≤ 6, sí | 4 | 12 |
| 2 | D (7, 13) | 7 ≤ 4, no | 4 | 12 |
| 3 | C (5, 9) | 5 ≤ 4, no | 4 | 12 |

```cpp
127     delete[] orden;
128     return total;
```

- **127**: libera la copia ordenada.
- **128**: devuelve 12.

---

## 5. PD 0/1 (líneas 141–169) ← lo central

Antes del código, la idea en una frase:

> Se arma una tabla donde **`tab[i][c]` = el mejor valor posible usando solo los primeros `i` objetos, con una mochila de capacidad `c`**. Se llena de a una celda, siempre a partir de celdas que ya se llenaron, hasta llegar a `tab[N][capacidad]`, que es la respuesta.

### 5.1 Crear la tabla (línea 143)

```cpp
143     int ** tab = initMatriz(N + 1, capacidad + 1);
```

- **Filas: `N + 1`** (de 0 a N). La fila `i` significa "puedo usar los objetos 1..i". La fila **0** es "no tengo ningún objeto".
- **Columnas: `capacidad + 1`** (de 0 a C). La columna `c` significa "la mochila aguanta `c` kilos".
- Por qué **+1** en las dos: para tener la fila 0 y la columna 0, que son los casos base.
- Gracias a los `()` de `initMatriz`, **todo arranca en 0**. Con eso la fila 0 ya queda bien, porque sin objetos el valor es 0. Es el **caso base**, y no hace falta escribir ninguna línea para cargarlo.

Así queda la tabla recién creada. Solo la fila 0 tiene significado; las demás tienen 0 porque todavía no se calcularon.

| i \ c | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 |
|---|---|---|---|---|---|---|---|---|
| 0 (nada) | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
| 1 A | ? | ? | ? | ? | ? | ? | ? | ? |
| 2 B | ? | ? | ? | ? | ? | ? | ? | ? |
| 3 C | ? | ? | ? | ? | ? | ? | ? | ? |
| 4 D | ? | ? | ? | ? | ? | ? | ? | ? |

### 5.2 Los dos `for` (líneas 145–148)

```cpp
145     for (int i = 1; i <= N; i++) {
146         int peso = objetos[i - 1].peso;
147         int valor = objetos[i - 1].valor;
148         for (int c = 0; c <= capacidad; c++) {
```

- **145**: recorre las filas **de arriba hacia abajo**, desde la 1 (la 0 ya está). Cada vuelta agrega un objeto nuevo a los que se pueden usar.
- **146–147**: los datos del objeto `i`. Es `objetos[i - 1]` porque el array de objetos empieza en 0 y la tabla empieza en 1: la fila 1 es `objetos[0]` (A), la fila 2 es `objetos[1]` (B), y así.
- **148**: recorre todas las capacidades de esa fila, de 0 a C.

**Por qué este orden funciona:** cada celda de la fila `i` solo lee celdas de la fila `i - 1`, que ya se terminó de llenar en la vuelta anterior del `for` de afuera.

### 5.3 La decisión: ¿pongo el objeto o no? (líneas 149–152)

Estas cuatro líneas **son** la programación dinámica:

```cpp
149             tab[i][c] = tab[i - 1][c];
150             if (peso <= c) {
151                 tab[i][c] = max(tab[i][c], valor + tab[i - 1][c - peso]);
152             }
```

- **149**, **opción "no lo pongo"**. Si no pongo el objeto `i`, es como si no existiera: lo mejor que puedo hacer es lo mejor con los objetos anteriores y la misma capacidad, o sea `tab[i - 1][c]`, la celda **de arriba**. Se guarda como valor provisorio.
- **150**: ¿el objeto entra en una mochila de `c` kilos? Si no entra, la única opción es la de la línea 149 y queda esa.
- **151**, **opción "lo pongo"**. Gano `valor` y me quedan `c - peso` kilos para los objetos **anteriores**: lo mejor que puedo hacer con eso es `tab[i - 1][c - peso]`, en la fila de arriba y corrida `peso` columnas a la izquierda. `max` se queda con la mejor de las dos opciones.

Por qué en la 151 es `i - 1` y no `i`: porque es mochila **0/1**, y cada objeto se usa una sola vez. Si pongo el objeto `i`, para el resto solo puedo usar los anteriores. Si fuera `tab[i][c - peso]`, el objeto se podría repetir, que es lo que hace el [ejercicio 02](../02-subconjunto-suma-m/Letra.md).

Dibujo de qué celdas lee `tab[i][c]`:

```
fila i-1:   ... [c - peso] ........ [c]      <- las dos ya están calculadas
                     \               |
                      \  + valor     |  (tal cual)
                       \             |
fila i:                 ----------> [c]      <- max de las dos
```

### 5.4 Traza completa de los `for`

Cada tabla es una vuelta del `for` de la línea 145, y cada renglón es una vuelta del `for` de la línea 148.

**Fila 1 = A (peso 1, valor 6)**

| c | Línea 149: no ponerlo, `tab[0][c]` | Línea 150: ¿entra? | Línea 151: ponerlo, `6 + tab[0][c-1]` | `tab[1][c]` |
|---|---|---|---|---|
| 0 | 0 | no (1 > 0) | — | **0** |
| 1 | 0 | sí (1 ≤ 1) | 6 + tab[0][0] = 6 + 0 = 6 | max(0, 6) = **6** |
| 2 | 0 | sí (1 ≤ 2) | 6 + tab[0][1] = 6 + 0 = 6 | max(0, 6) = **6** |
| 3 | 0 | sí | 6 + 0 = 6 | **6** |
| 4 | 0 | sí | 6 + 0 = 6 | **6** |
| 5 | 0 | sí | 6 + 0 = 6 | **6** |
| 6 | 0 | sí | 6 + 0 = 6 | **6** |
| 7 | 0 | sí | 6 + 0 = 6 | **6** |

Interpretación: con solo A, en cualquier mochila de 1 kilo o más llevo A y vale 6.

**Fila 2 = B (peso 2, valor 6)**

| c | Línea 149: no ponerlo, `tab[1][c]` | Línea 150: ¿entra? | Línea 151: ponerlo, `6 + tab[1][c-2]` | `tab[2][c]` |
|---|---|---|---|---|
| 0 | 0 | no (2 > 0) | — | **0** |
| 1 | 6 | no (2 > 1) | — | **6** |
| 2 | 6 | sí (2 ≤ 2) | 6 + tab[1][0] = 6 + 0 = 6 | max(6, 6) = **6** |
| 3 | 6 | sí (2 ≤ 3) | 6 + tab[1][1] = 6 + 6 = 12 | max(6, 12) = **12** |
| 4 | 6 | sí | 6 + tab[1][2] = 6 + 6 = 12 | **12** |
| 5 | 6 | sí | 6 + tab[1][3] = 6 + 6 = 12 | **12** |
| 6 | 6 | sí | 6 + tab[1][4] = 6 + 6 = 12 | **12** |
| 7 | 6 | sí | 6 + tab[1][5] = 6 + 6 = 12 | **12** |

Interpretación: en `c = 2` entra A o B, pero no los dos, así que vale 6. Desde `c = 3` entran los dos y vale 12.

**Fila 3 = C (peso 5, valor 9)** ← acá está la clave

| c | Línea 149: no ponerlo, `tab[2][c]` | Línea 150: ¿entra? | Línea 151: ponerlo, `9 + tab[2][c-5]` | `tab[3][c]` |
|---|---|---|---|---|
| 0 | 0 | no (5 > 0) | — | **0** |
| 1 | 6 | no | — | **6** |
| 2 | 6 | no | — | **6** |
| 3 | 12 | no | — | **12** |
| 4 | 12 | no (5 > 4) | — | **12** |
| 5 | 12 | sí (5 ≤ 5) | 9 + tab[2][0] = 9 + 0 = 9 | max(12, 9) = **12** |
| 6 | 12 | sí (5 ≤ 6) | 9 + tab[2][1] = 9 + 6 = 15 | max(12, 15) = **15** |
| 7 | 12 | sí (5 ≤ 7) | 9 + tab[2][2] = 9 + 6 = 15 | max(12, 15) = **15** |

En `c = 7`, la línea 149 deja 12 (A + B, lo mismo que hace el greedy). La línea 151 prueba "C + lo mejor con A y B en los 2 kilos que sobran" = 9 + 6 = 15, y el `max` se queda con 15. Esta comparación es la que el greedy nunca hace.

**Fila 4 = D (peso 7, valor 13)**

| c | Línea 149: no ponerlo, `tab[3][c]` | Línea 150: ¿entra? | Línea 151: ponerlo, `13 + tab[3][c-7]` | `tab[4][c]` |
|---|---|---|---|---|
| 0–5 | 0, 6, 6, 12, 12, 12 | no (7 > c) | — | **0, 6, 6, 12, 12, 12** |
| 6 | 15 | no (7 > 6) | — | **15** |
| 7 | 15 | sí (7 ≤ 7) | 13 + tab[3][0] = 13 + 0 = 13 | max(15, 13) = **15** |

Así queda la tabla al terminar los `for`:

| i \ c | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 |
|---|---|---|---|---|---|---|---|---|
| 0 (nada) | 0 | 0 | 0 | 0  | 0  | 0  | 0  | 0  |
| 1 A      | 0 | 6 | 6 | 6  | 6  | 6  | 6  | 6  |
| 2 B      | 0 | 6 | 6 | 12 | 12 | 12 | 12 | 12 |
| 3 C      | 0 | 6 | 6 | 12 | 12 | 12 | 15 | 15 |
| 4 D      | 0 | 6 | 6 | 12 | 12 | 12 | 15 | **15** |

### 5.5 Reconstruir qué objetos se usaron (líneas 158–164)

La tabla dice **cuánto** vale lo mejor (15), pero no **qué** objetos. Para saberlo se recorre la tabla hacia arriba, desde la esquina de abajo a la derecha.

```cpp
158     int c = capacidad;
159     for (int i = N; i >= 1; i--) {
160         if (tab[i][c] != tab[i - 1][c]) {
161             usados[objetos[i - 1].numero] = true;
162             c -= objetos[i - 1].peso;
163         }
164     }
```

- **158**: se arranca con la mochila entera (`c = 7`).
- **159**: se recorren las filas **de abajo hacia arriba**, del objeto N al 1.
- **160**: la pregunta es "¿el valor cambió al agregar el objeto `i`?".
  - Si `tab[i][c] == tab[i - 1][c]`, el objeto `i` no hizo falta: sin él se logra lo mismo. No se marca y `c` no cambia.
  - Si son distintos, el valor mejoró gracias al objeto `i`, así que **se usó**.
- **161**: se marca como usado.
- **162**: se descuenta su peso. Lo que queda por explicar son los otros objetos en `c - peso` kilos.

Traza:

| `i` | `c` | `tab[i][c]` | `tab[i-1][c]` | Línea 160: ¿distintos? | Qué pasa | `c` después |
|---|---|---|---|---|---|---|
| 4 (D) | 7 | 15 | 15 | no | D no se usó | 7 |
| 3 (C) | 7 | 15 | 12 | **sí** | C se usó | 7 − 5 = 2 |
| 2 (B) | 2 | 6 | 6 | no | B no se usó | 2 |
| 1 (A) | 2 | 6 | 0 | **sí** | A se usó | 2 − 1 = 1 |

Resultado: A y C, la salida `[1 3]`. Se puede comprobar: pesan 1 + 5 = 6 ≤ 7 y valen 6 + 9 = 15.

### 5.6 Devolver y liberar (líneas 166–168)

```cpp
166     int resultado = tab[N][capacidad];
167     deleteMatriz(tab, N + 1);
168     return resultado;
```

- **166**: la respuesta es la esquina de abajo a la derecha, `tab[4][7] = 15`. Se guarda **antes** de borrar la tabla.
- **167**: libera la tabla (`N + 1` filas, las mismas que se crearon).
- **168**: devuelve 15.

---

## 6. Greedy fraccionario (líneas 178–193)

```cpp
178 double greedyFraccionario(Objeto* objetos, int N, int capacidad) {
179     Objeto* orden = ordenarPorRatio(objetos, N);
180     int libre = capacidad;
181     double total = 0;
```

- **178**: devuelve `double` porque puede llevar pedazos de objetos, así que el valor puede no ser entero.
- **179–181**: igual que el greedy 0/1, pero `total` es `double`.

```cpp
182     for (int i = 0; i < N && libre > 0; i++) {
183         if (orden[i].peso <= libre) {
184             libre -= orden[i].peso;
185             total += orden[i].valor;
186         } else {
187             total += (double)orden[i].valor * libre / orden[i].peso;
188             libre = 0;
189         }
190     }
```

- **182**: recorre en orden de ratio, y además corta cuando la mochila se llena (`libre > 0`).
- **183–185**: si entra entero, igual que en el greedy 0/1.
- **186–188**: si no entra entero, se lleva **el pedazo que entra**: `libre` de sus `peso` kilos, que vale `valor * libre / peso`. La mochila queda llena (`libre = 0`) y el `for` termina.
- En **187**, el `(double)` va primero para que toda la cuenta se haga con decimales. Sin él, `13 * 4 / 7` daría `52 / 7 = 7` (entero) en lugar de 7,43.

Traza:

| `i` | `orden[i]` | ¿Entra entero? | Qué suma | `libre` | `total` |
|---|---|---|---|---|---|
| — | | | | 7 | 0 |
| 0 | A (1, 6) | sí | 6 | 6 | 6 |
| 1 | B (2, 6) | sí | 6 | 4 | 12 |
| 2 | D (7, 13) | no, entran 4 de 7 kg | 13 · 4 / 7 = 7,43 | 0 | 19,43 |
| 3 | — | el `for` ya cortó por `libre > 0` | | | |

```cpp
191     delete[] orden;
192     return total;
```

---

## 7. `main` (líneas 195–227)

```cpp
196     int N, capacidad;
197     cin >> N >> capacidad;
198     Objeto* objetos = new Objeto[N];
199     for (int i = 0; i < N; i++) {
200         objetos[i].numero = i + 1;
201         cin >> objetos[i].peso >> objetos[i].valor;
202     }
```

- **197**: lee la primera línea (`4 7`).
- **198**: array para los N objetos.
- **200**: el número de cada objeto es su posición **contando desde 1** (`i` empieza en 0).
- **201**: lee el peso y el valor de cada uno.

```cpp
205     bool* usadosGreedy = new bool[N + 1]();
206     bool* usadosPD = new bool[N + 1]();
```

- **205–206**: dos arrays de marcas, uno por algoritmo. Tienen tamaño `N + 1` para poder usar `usados[1]` a `usados[N]` directamente con el número del objeto (`usados[0]` no se usa). Los `()` los dejan en `false`.

```cpp
208     int valorGreedy = greedy01(objetos, N, capacidad, usadosGreedy);
209     int valorPD = pd01(objetos, N, capacidad, usadosPD);
210     double valorFrac = greedyFraccionario(objetos, N, capacidad);
```

- **208–210**: se corren los tres algoritmos sobre los mismos objetos. Los greedy ordenan una copia, así que `objetos` llega a la PD en el orden original.

```cpp
212     cout << "greedy 0/1: " << valorGreedy << " ";
213     imprimirUsados(usadosGreedy, N);
214     cout << endl;
216     cout << "pd 0/1: " << valorPD << " ";
217     imprimirUsados(usadosPD, N);
218     cout << endl;
220     cout << fixed << setprecision(2);
221     cout << "greedy fraccionaria: " << valorFrac << endl;
```

- **212–218**: imprime `greedy 0/1: 12 [1 2]` y `pd 0/1: 15 [1 3]`.
- **220**: a partir de acá los `double` se imprimen con exactamente 2 decimales.
- **221**: imprime `greedy fraccionaria: 19.43`.

```cpp
223     delete[] objetos;
224     delete[] usadosGreedy;
225     delete[] usadosPD;
226     return 0;
```

- **223–225**: libera todo lo que se pidió con `new` en el `main`.

---

## Resumen para el pizarrón

Si en la clase hay que reducir la PD a lo mínimo, son estas líneas:

```cpp
int ** tab = initMatriz(N + 1, capacidad + 1);        // fila 0 en ceros = caso base
for (int i = 1; i <= N; i++)                          // agrego objetos de a uno
    for (int c = 0; c <= capacidad; c++) {            // para cada capacidad
        tab[i][c] = tab[i - 1][c];                    // opción 1: no lo pongo
        if (peso_i <= c)                              // si entra...
            tab[i][c] = max(tab[i][c],                // ...me quedo con la mejor entre
                            valor_i + tab[i - 1][c - peso_i]);  // opción 2: lo pongo
    }
// respuesta: tab[N][capacidad]
```

Y la frase que lo resume: **para cada objeto y cada capacidad, se prueban las dos opciones (ponerlo o no) usando resultados ya calculados, y se guarda la mejor.** El greedy elige una sola opción mirando únicamente el objeto. La PD prueba las dos, y como las guarda todas en la tabla, no repite ninguna cuenta.
