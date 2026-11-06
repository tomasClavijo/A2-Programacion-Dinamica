## Coeficientes binomiales (triángulo de Pascal)

Nombre de archivo: `coeficientes.cpp` (originalmente `Ejercicio 9/ejercicio9.cpp`, A2 2023)

> El repo de 2023 no incluye la letra original. Esta letra se reconstruyó a partir del código y de los casos de prueba.

### Letra

Se desea calcular varios coeficientes binomiales `C(n, k)` ("n tomados de a k", la cantidad de formas de elegir `k` elementos de un conjunto de `n`).

### Input

La primera línea contiene `N`, una cota para todos los `n` consultados. La segunda línea contiene `P`, la cantidad de consultas. Las siguientes `P` líneas contienen `n k` (con `0 ≤ k ≤ n ≤ N`).

Las consultas vienen **ordenadas por `n` de forma no decreciente**.

### Salida

`P` líneas, cada una con el valor de `C(n, k)` correspondiente.

### Restricciones

Resolver con programación dinámica usando O(N) espacio (no guardar el triángulo completo).

### Ejemplos de entrada y salida

#### Entrada
4
8
0 0
1 0
1 1
2 1
2 2
3 1
3 2
4 1

#### Salida
1
1
1
2
1
3
3
4

---

## Explicación paso a paso

### Paso 1: por qué no usar la fórmula directa

`C(n, k) = n! / (k! · (n−k)!)`. El problema es que los factoriales crecen muy rápido: `21!` ya no entra en un `long long`, aunque `C(21, 10) = 352716` es chico. Con la fórmula habría overflow en los cálculos intermedios.

### Paso 2: definir el subproblema

> `C[n][k]` = cantidad de formas de elegir `k` elementos de un conjunto de `n`.

### Paso 3: casos base

- `C[n][0] = 1`: hay una sola forma de no elegir nada.
- `C[n][n] = 1`: hay una sola forma de elegir todo.
- `C[n][k] = 0` si `k > n`: no se pueden elegir más elementos de los que hay.

### Paso 4: recurrencia (regla de Pascal)

Tomo un elemento cualquiera `x` del conjunto. Cada forma de elegir `k` de los `n` elementos cae en uno de dos casos:

- **Incluye a `x`**: hay que elegir los `k−1` restantes de los otros `n−1`, o sea `C[n-1][k-1]`.
- **No incluye a `x`**: hay que elegir los `k` de los otros `n−1`, o sea `C[n-1][k]`.

`C[n][k] = C[n-1][k-1] + C[n-1][k]`

Esta es la PD. Cada valor sale de sumar otros dos más chicos, y nunca se calculan factoriales.

### Paso 5: orden de llenado y ahorro de memoria

La fila `n` depende solo de la fila `n−1`, así que se calcula fila por fila desde la 0. Para responder no hace falta todo el triángulo, solo **la fila actual**. El código usa dos arrays de tamaño `N+1`:

- `actual`: la fila `filaActual` del triángulo,
- `aux`: donde se arma la fila siguiente.

Para avanzar una fila:

```cpp
aux[0] = 1;
for (i = 1; i <= N; i++)
    aux[i] = actual[i-1] + actual[i];   // regla de Pascal
swap(actual, aux);                      // la nueva pasa a ser la actual
filaActual++;
```

Las posiciones con `k > n` quedan en 0 solas, porque `actual[i-1] + actual[i]` suma dos ceros. Eso cubre el caso base `C[n][k] = 0`. Además, `C[n][n] = 1` sale de `actual[n-1] + actual[n] = 1 + 0`.

Intercambiar los punteros (`swap`) es O(1), así que no se copian los arrays.

### Paso 6: el triángulo del ejemplo

Así evoluciona `actual` (N = 4):

| Fila n | actual[0] | [1] | [2] | [3] | [4] |
|---|---|---|---|---|---|
| 0 | 1 | 0 | 0 | 0 | 0 |
| 1 | 1 | 1 | 0 | 0 | 0 |
| 2 | 1 | 2 | 1 | 0 | 0 |
| 3 | 1 | 3 | 3 | 1 | 0 |
| 4 | 1 | 4 | 6 | 4 | 1 |

Por ejemplo, la fila 4 sale de la fila 3:

- `aux[1] = actual[0] + actual[1] = 1 + 3 = 4`
- `aux[2] = actual[1] + actual[2] = 3 + 3 = 6`
- `aux[3] = actual[2] + actual[3] = 3 + 1 = 4`
- `aux[4] = actual[3] + actual[4] = 1 + 0 = 1`

### Paso 7: responder las consultas en orden

Como las consultas vienen ordenadas por `n`, el triángulo **nunca hay que retroceder**. Para cada consulta `(n, k)`:

1. Mientras `filaActual < n`, se avanza una fila (paso 5).
2. Se responde `actual[k]`.

Siguiendo el ejemplo:

| Consulta | Filas que se calculan | Respuesta |
|---|---|---|
| `0 0` | ninguna (ya estoy en la 0) | 1 |
| `1 0` | 0 → 1 | 1 |
| `1 1` | ninguna | 1 |
| `2 1` | 1 → 2 | 2 |
| `2 2` | ninguna | 1 |
| `3 1` | 2 → 3 | 3 |
| `3 2` | ninguna | 3 |
| `4 1` | 3 → 4 | 4 |

Cada fila se calcula **una sola vez** en total, sin importar cuántas consultas haya.

### Paso 8: órdenes

- Tiempo: O(n_max · N + P), donde `n_max` es el mayor `n` consultado. Cada fila nueva cuesta O(N) y hay a lo sumo `n_max` filas. En el peor caso es O(N² + P).
- Espacio: O(N), con dos arrays de `N+1`. Guardar el triángulo entero ocuparía O(N²).

### Límite de `long long`

`C(n, k)` entra en un `long long` para todo `k` mientras `n ≤ 66`. En las pruebas el mayor `n` consultado es 45, así que no hay problema. Para `n` más grandes habría que trabajar módulo algún número o con enteros de precisión arbitraria.

### Observación sobre la versión original

La solución entregada en 2023 calculaba la fila siguiente a partir de un array `anterior` que solo quedaba sincronizado cuando la fila recibía al menos dos consultas. Pasaba las pruebas porque en ellas todas las filas tienen varias consultas, pero, por ejemplo, con las consultas `1 1`, `2 1`, `3 1` respondía `1 1 2` en lugar de `1 2 3`. La versión de este repo avanza fila por fila hasta `n` (paso 7) y no depende de eso.
