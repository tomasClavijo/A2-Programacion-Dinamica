## Mochila 0/1 (ejemplo de clase)

Nombre de archivo: `mochila01.cpp` (originalmente `Ejemplos/PD_Mochila.cpp`, A2 2022)

### Letra

Se tiene una mochila de capacidad `C` y `N` objetos, cada uno con un peso y un valor. Cada objeto se puede llevar **a lo sumo una vez**. Se desea saber el máximo valor que se puede cargar sin superar la capacidad, y qué objetos se usan.

El ejemplo trae los datos fijos en el código (no lee de la entrada):

| i | Objeto   | Peso | Valor |
|---|----------|------|-------|
| 1 | Lingote  | 5    | 10    |
| 2 | Diamante | 3    | 20    |
| 3 | Rubí     | 8    | 25    |
| 4 | Moneda   | 4    | 8     |

Capacidad: `C = 13`.

### Salida

```
El maximo valor obtenido es: 45
Use el objeto Rubi p(8) v(25)
Use el objeto Diamante p(3) v(20)
```

---

## Explicación paso a paso

### Paso 1: por qué PD y no greedy

Un greedy tentador es "tomar primero los más livianos". Acá eso sería Diamante (3), Moneda (4) y Lingote (5), que da 20 + 8 + 10 = 38 con peso 12, y Rubí ya no entra. El óptimo es Diamante + Rubí = 45.

"Tomar primero el de mejor valor/peso" justo funciona en este ejemplo, pero falla en general. Con capacidad 10 y objetos (peso 6, valor 30), (5, 20), (5, 20), el greedy toma el primero (relación 5) y obtiene 30, mientras que el óptimo es 20 + 20 = 40.

Como ningún greedy sirve siempre, hay que considerar las dos opciones para cada objeto (ponerlo o no). PD hace eso sin repetir cálculos.

### Paso 2: definir el subproblema

> `tab[i][c]` = máximo valor que se puede obtener **usando solo los objetos 1..i** con una mochila de capacidad **c**.

Son dos parámetros porque al decidir sobre un objeto cambian dos cosas: qué objetos quedan por decidir y cuánta capacidad queda.

La respuesta al problema original es `tab[N][C]`.

### Paso 3: casos base

- `tab[0][c] = 0` para toda `c`: sin objetos el valor es 0.
- `tab[i][0] = 0`: con capacidad 0 no entra nada. La recurrencia ya lo da solo, porque todos los pesos son > 0.

Por eso los arrays del código tienen un objeto "0" que no existe (peso 0, valor 0). La fila 0 de la tabla es el caso base.

### Paso 4: recurrencia

Para el objeto `i` con capacidad `c` hay dos casos:

1. **No entra** (`peso[i] > c`): no hay nada que elegir.
   `tab[i][c] = tab[i-1][c]`
2. **Entra**: me quedo con la mejor de las dos opciones.
   - No ponerlo: `tab[i-1][c]`
   - Ponerlo: `valor[i] + tab[i-1][c - peso[i]]`. Gano su valor y resuelvo el resto con la capacidad que sobra y **sin el objeto i** (fila `i-1`), porque no se puede repetir.

   `tab[i][c] = max(tab[i-1][c], valor[i] + tab[i-1][c - peso[i]])`

### Paso 5: orden de llenado

Cada celda de la fila `i` solo usa celdas de la fila `i-1`. Entonces se llena fila por fila de arriba hacia abajo, y dentro de cada fila en cualquier orden. El código va de izquierda a derecha.

### Paso 6: la tabla completa del ejemplo

| i \ c | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | 11 | 12 | 13 |
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
| 0 (nada)     | 0 | 0 | 0 | 0  | 0  | 0  | 0  | 0  | 0  | 0  | 0  | 0  | 0  | 0  |
| 1 Lingote (5, 10)  | 0 | 0 | 0 | 0  | 0  | 10 | 10 | 10 | 10 | 10 | 10 | 10 | 10 | 10 |
| 2 Diamante (3, 20) | 0 | 0 | 0 | 20 | 20 | 20 | 20 | 20 | 30 | 30 | 30 | 30 | 30 | 30 |
| 3 Rubí (8, 25)     | 0 | 0 | 0 | 20 | 20 | 20 | 20 | 20 | 30 | 30 | 30 | **45** | 45 | 45 |
| 4 Moneda (4, 8)    | 0 | 0 | 0 | 20 | 20 | 20 | 20 | 28 | 30 | 30 | 30 | 45 | 45 | **45** |

Algunas celdas calculadas a mano:

- `tab[1][4] = 0`: el Lingote pesa 5 y no entra en 4, así que se copia `tab[0][4] = 0`.
- `tab[1][5] = max(tab[0][5], 10 + tab[0][0]) = max(0, 10) = 10`.
- `tab[2][8] = max(tab[1][8], 20 + tab[1][5]) = max(10, 20 + 10) = 30`: Diamante + Lingote, peso 8.
- `tab[3][11] = max(tab[2][11], 25 + tab[2][3]) = max(30, 25 + 20) = 45`: Rubí + Diamante, peso 11.
- `tab[4][7] = max(tab[3][7], 8 + tab[3][3]) = max(20, 8 + 20) = 28`: Moneda + Diamante, peso 7.
- `tab[4][13] = max(tab[3][13], 8 + tab[3][9]) = max(45, 8 + 30) = 45`: la Moneda no mejora nada.

La respuesta es `tab[4][13] = 45`.

### Paso 7: reconstruir qué objetos se usaron

La tabla da el valor óptimo pero no la lista de objetos. Para obtenerla se recorre la tabla hacia atrás desde `tab[N][C]`:

- Si `tab[i][c] == tab[i-1][c]`, el objeto `i` **no** hizo falta: se baja a `i-1` con la misma `c`.
- Si `tab[i][c] != tab[i-1][c]`, el objeto `i` **sí** se usó: se baja a `i-1` y se resta su peso (`c -= peso[i]`).

En el ejemplo:

| Paso | (i, c) | tab[i][c] | tab[i-1][c] | ¿Se usó? | c queda |
|------|--------|-----------|-------------|----------|---------|
| 1 | (4, 13) | 45 | 45 | No (Moneda) | 13 |
| 2 | (3, 13) | 45 | 30 | **Sí (Rubí)** | 13 − 8 = 5 |
| 3 | (2, 5)  | 20 | 10 | **Sí (Diamante)** | 5 − 3 = 2 |
| 4 | (1, 2)  | 0  | 0  | No (Lingote) | 2 |

Se usan Rubí y Diamante: valor 45, peso 11.

### Paso 8: órdenes

- Tiempo: O(N·C), porque cada una de las (N+1)·(C+1) celdas se calcula en O(1).
- Espacio: O(N·C) para la tabla. Se podría bajar a O(C) con una sola fila recorrida de derecha a izquierda (como en el ejercicio 03), pero entonces no se puede reconstruir la solución.

> Ojo: el orden es **pseudo-polinomial**. Depende del *valor* de C, no de su tamaño en bits. Con C = 10⁹ la tabla no entra en memoria.
