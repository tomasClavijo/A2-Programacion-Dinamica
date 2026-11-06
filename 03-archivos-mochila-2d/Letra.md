## Selección de archivos (mochila con dos restricciones)

Nombre de archivo: `archivos.cpp` (originalmente `Ejercicio 7/ejercicio7.cpp`, A2 2023)

> El repo de 2023 no incluye la letra original. Esta letra se reconstruyó a partir del código y de los casos de prueba.

### Letra

Se tienen `N` archivos. Cada archivo tiene un **tamaño**, una cantidad de **líneas de código** y un **puntaje**. Se desea elegir un subconjunto de archivos (cada uno a lo sumo una vez) que **maximice el puntaje total**, sin superar un tamaño máximo `S` ni una cantidad máxima de líneas `L`.

### Input

La primera línea contiene `N`, la cantidad de archivos. La segunda contiene `S`, el tamaño máximo. La tercera contiene `L`, la cantidad máxima de líneas. Las siguientes `N` líneas contienen, para cada archivo, `tamaño líneas puntaje` separados por espacio.

### Salida

Una línea con el puntaje máximo alcanzable.

### Restricciones

Resolver con programación dinámica en O(N·S·L) temporal.

### Ejemplos de entrada y salida

#### Entrada
10
50
1000
16 665 1
6 501 8
25 904 7
18 591 5
7 683 6
22 88 1
24 497 9
22 565 7
31 414 9
49 389 2

#### Salida
17

---

## Explicación paso a paso

### Paso 1: relación con la mochila

Es la [mochila 0/1](../01-mochila-01-ejemplo/Letra.md) con **dos capacidades** en lugar de una:

| Mochila 0/1 | Este ejercicio |
|---|---|
| objeto | archivo |
| peso ≤ C | tamaño ≤ S **y** líneas ≤ L |
| valor | puntaje |

Hay dos recursos limitados a la vez, así que el estado necesita las dos capacidades restantes.

### Paso 2: definir el subproblema

Versión "de libro", con tres dimensiones:

> `T[i][l][s]` = máximo puntaje usando solo los archivos **1..i**, con a lo sumo **l** líneas y tamaño a lo sumo **s**.

La respuesta es `T[N][L][S]`.

### Paso 3: casos base

- `T[0][l][s] = 0`: sin archivos el puntaje es 0.

En el código esto sale gratis porque `initMatriz` crea la matriz con `new int[...]()`, y los `()` la inicializan en ceros.

### Paso 4: recurrencia

Para el archivo `i` con tamaño `t`, líneas `li` y puntaje `p`:

- Si no entra (`li > l` o `t > s`): `T[i][l][s] = T[i-1][l][s]`.
- Si entra:
  `T[i][l][s] = max( T[i-1][l][s],  p + T[i-1][l - li][s - t] )`
  La primera opción es no usarlo. La segunda es usarlo y gastar `li` líneas y `t` de tamaño.

Igual que en la mochila 0/1, al usar el archivo se baja a `i-1`, porque no se puede repetir.

### Paso 5: ahorrar una dimensión (lo que hace el código)

Una tabla de `N × L × S` con los datos de prueba (N = 10000) no entra en memoria. Pero la capa `i` solo usa la capa `i-1`, así que alcanza con **una sola matriz** `tab[l][s]` que se va sobrescribiendo archivo por archivo.

El problema es que, si se sobrescribe en el orden equivocado, se puede leer un valor de la capa `i` cuando se necesitaba el de la `i-1`. La solución es recorrer **las dos capacidades de mayor a menor**:

```cpp
for (int j = L; j >= l; j--)          // líneas, de L hacia abajo
    for (int k = S; k >= t; k--)      // tamaño, de S hacia abajo
        tab[j][k] = max(tab[j][k], tab[j-l][k-t] + p);
```

**Por qué funciona.** `tab[j][k]` lee `tab[j-l][k-t]`, que está más arriba y más a la izquierda. Como se recorre de abajo hacia arriba y de derecha a izquierda, esa celda **todavía no se tocó** en esta vuelta, así que conserva el valor de la capa `i-1` (sin el archivo actual). Por lo tanto cada archivo se usa a lo sumo una vez.

**Qué pasaría en orden creciente.** `tab[j-l][k-t]` ya estaría actualizado con el archivo actual, y se podría sumar el mismo archivo dos veces. Eso sería la mochila *no acotada* (como en el [ejercicio 02](../02-subconjunto-suma-m/Letra.md)), que acá no corresponde.

Además, los `for` cortan en `j >= l` y `k >= t`. Para capacidades menores el archivo no entra y la celda queda como estaba, que es justamente `T[i-1]`.

### Paso 6: ejemplo chico trazado a mano

El ejemplo de la letra tiene L = 1000 y no se puede seguir a mano. Este es más chico:

`S = 5`, `L = 4`, y tres archivos:

| Archivo | Tamaño t | Líneas l | Puntaje p |
|---|---|---|---|
| A | 2 | 1 | 3 |
| B | 3 | 2 | 4 |
| C | 2 | 3 | 5 |

Matriz `tab[l][s]` (filas = líneas, columnas = tamaño) **después de procesar cada archivo**:

**Después de A (2, 1, 3).** A entra en toda celda con `l ≥ 1` y `s ≥ 2`, así que vale 3.

| l \ s | 0 | 1 | 2 | 3 | 4 | 5 |
|---|---|---|---|---|---|---|
| 0 | 0 | 0 | 0 | 0 | 0 | 0 |
| 1 | 0 | 0 | 3 | 3 | 3 | 3 |
| 2 | 0 | 0 | 3 | 3 | 3 | 3 |
| 3 | 0 | 0 | 3 | 3 | 3 | 3 |
| 4 | 0 | 0 | 3 | 3 | 3 | 3 |

Si se hubiera recorrido en orden creciente, `tab[2][4]` habría quedado en `3 + tab[1][2] = 6`, que corresponde a usar A dos veces. Con el orden descendente queda en 3.

**Después de B (3, 2, 4).**

| l \ s | 0 | 1 | 2 | 3 | 4 | 5 |
|---|---|---|---|---|---|---|
| 0 | 0 | 0 | 0 | 0 | 0 | 0 |
| 1 | 0 | 0 | 3 | 3 | 3 | 3 |
| 2 | 0 | 0 | 3 | 4 | 4 | 4 |
| 3 | 0 | 0 | 3 | 4 | 4 | **7** |
| 4 | 0 | 0 | 3 | 4 | 4 | 7 |

- `tab[2][3] = max(3, 4 + tab[0][0]) = 4`: con 2 líneas y tamaño 3 conviene B solo.
- `tab[3][5] = max(3, 4 + tab[1][2]) = max(3, 4 + 3) = 7`: A + B usa 3 líneas y tamaño 5.

**Después de C (2, 3, 5).**

| l \ s | 0 | 1 | 2 | 3 | 4 | 5 |
|---|---|---|---|---|---|---|
| 0 | 0 | 0 | 0 | 0 | 0 | 0 |
| 1 | 0 | 0 | 3 | 3 | 3 | 3 |
| 2 | 0 | 0 | 3 | 4 | 4 | 4 |
| 3 | 0 | 0 | 5 | 5 | 5 | 7 |
| 4 | 0 | 0 | 5 | 5 | **8** | **8** |

- `tab[3][2] = max(3, 5 + tab[0][0]) = 5`: C solo.
- `tab[4][4] = max(4, 5 + tab[1][2]) = max(4, 5 + 3) = 8`: A + C usa 4 líneas y tamaño 4.
- `tab[4][5] = max(7, 5 + tab[1][3]) = max(7, 8) = 8`.

La respuesta es `tab[L][S] = tab[4][5] = 8`, con A + C. B + C daría 9 puntos, pero son 5 líneas y se pasa de L = 4.

### Paso 7: órdenes

- Tiempo: O(N·L·S), porque cada archivo recorre toda la matriz en el peor caso.
- Espacio: O(L·S) para `tab`, gracias a la reducción del paso 5 (en lugar de O(N·L·S)), más O(N) para guardar los archivos.

Con la reducción a una sola matriz se pierde la posibilidad de reconstruir qué archivos se eligieron. La letra solo pide el puntaje, así que alcanza.
