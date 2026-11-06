## Mochila 0/1 (ejemplo de clase)

Nombre de archivo: mochila01.cpp (originalmente `Ejemplos/PD_Mochila.cpp`, A2 2022)

### Letra

Se tiene una mochila de capacidad `C` y `N` objetos, cada uno con un peso y un valor. Cada objeto se puede llevar a lo sumo una vez. Se desea saber el máximo valor que se puede cargar sin superar la capacidad, y qué objetos se usan.

El ejemplo trae los datos fijos en el código (no lee de la entrada):

| Objeto   | Peso | Valor |
|----------|------|-------|
| Lingote  | 5    | 10    |
| Diamante | 3    | 20    |
| Rubí     | 8    | 25    |
| Moneda   | 4    | 8     |

Capacidad: 13.

### Salida

El máximo valor obtenido y la lista de objetos usados (reconstruida recorriendo la tabla hacia atrás).

### Idea de la solución (PD)

`tab[i][c]` = máximo valor usando los primeros `i` objetos con capacidad `c`.

- `tab[0][c] = 0` (sin objetos).
- Si `peso[i] > c`: `tab[i][c] = tab[i-1][c]`.
- Si no: `tab[i][c] = max(tab[i-1][c], valor[i] + tab[i-1][c - peso[i]])`.

A diferencia del ejercicio de suma M, al usar el objeto se baja a la fila `i-1`, porque no se puede repetir.

Reconstrucción: desde `tab[N][C]`, si `tab[i][c] != tab[i-1][c]` el objeto `i` se usó y se resta su peso.

Orden: O(N·C) en tiempo y espacio.
