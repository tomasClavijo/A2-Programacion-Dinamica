## SUBCONJ. DE SUMA M

Nombre de archivo: `subconjunto_suma.cpp` (originalmente `Ejercicio 8/ejercicio8.cpp`, A2 2022)

### Letra

Dados un conjunto C de N>0 enteros positivos estrictos Ki, es decir, Ki>0 para todo i con 1 ≤ i ≤ N, y un entero positivo Mi, se desea saber si existe un subconjunto S de C que sumados den como resultado Mi, pudiendo repetir tantas veces como se desee los elementos.

### Input

La primera línea N indica cuántos elementos tiene C. Las siguientes N líneas son los elementos. Luego, sigue un número M que es una cota de todos los casos Mi que finalizan a la entrada. La siguiente línea indica la cantidad P de casos de prueba. Los siguientes P números indican los casos Mi con 1≤i≤P para los cuales se desea conocer si existe S.

Por ejemplo:
Si C={3,34,4,12,5,2} y M=9 entonces sí existe S={4,5}.
Si C={3} y M=9 entonces sí existe S={3,3,3}.

### Salida

La salida contendrá P líneas indicando si existe S (1) o no (0).

### Restricciones

Se debe resolver en tiempo y espacio O(NM).

### Ejemplos de entrada y salida

C = {5, 6, 3}, M = 10, y se consultan todos los valores del 1 al 10.

#### Entrada
3
5
6
3
10
10
1
2
3
4
5
6
7
8
9
10

#### Salida
0
0
1
0
1
1
0
1
1
1

---

## Explicación paso a paso

### Paso 1: relación con la mochila

Es una mochila con dos diferencias respecto de la [mochila 0/1](../01-mochila-01-ejemplo/Letra.md):

1. No hay valores que maximizar. Solo importa si se puede llegar **exactamente** a la suma, así que la tabla es de `bool`.
2. Los elementos **se pueden repetir** (mochila *no acotada*). Esa diferencia cambia una sola fila en la recurrencia, como se ve en el paso 4.

### Paso 2: definir el subproblema

> `tab[i][j]` = ¿se puede formar la suma exacta **j** usando (con repetición) solo los elementos **K0..Ki**?

En el código los elementos se indexan desde 0, así que la fila `i` corresponde al elemento `objetos[i]`.

La respuesta para cada consulta `Mi` es `tab[N-1][Mi]`, es decir, con todos los elementos disponibles.

### Paso 3: casos base (fila 0, solo el elemento K0)

Usando únicamente `K0` (repetido las veces que quiera) llego a `j` si y solo si `j` es múltiplo de `K0`:

`tab[0][j] = (j % K0 == 0)` para `j ≥ 1`.

Columna 0: el código pone `tab[i][0] = false` y trata aparte el caso `j == Ki` (ver paso 4). Otra opción equivalente sería definir `tab[i][0] = true` (la suma 0 se logra con el subconjunto vacío). En ese caso `tab[i][j-Ki]` con `j == Ki` ya daría `true` y no haría falta el caso especial. Las consultas siempre son `Mi ≥ 1`, así que el valor de la columna 0 no se imprime nunca.

### Paso 4: recurrencia

Para el elemento `Ki` y la suma `j`:

1. **`j < Ki`**: `Ki` no entra, así que no lo puedo usar.
   `tab[i][j] = tab[i-1][j]`
2. **`j == Ki`**: con `Ki` solo ya llego.
   `tab[i][j] = true`
3. **`j > Ki`**: llego si alguna de las dos opciones funciona.
   - No usar `Ki`: `tab[i-1][j]`
   - Usar `Ki` **al menos una vez**: `tab[i][j - Ki]`

   `tab[i][j] = tab[i-1][j] || tab[i][j - Ki]`

**Esta es la diferencia clave con la mochila 0/1.** Al usar `Ki` me quedo en la **misma fila `i`** (no bajo a `i-1`). Lo que falta, `j - Ki`, se puede seguir armando con `Ki`, y así se permite repetirlo tantas veces como haga falta.

| | Mochila 0/1 | Mochila no acotada (este ejercicio) |
|---|---|---|
| Uso el elemento i | `tab[i-1][c - peso]` | `tab[i][j - Ki]` |
| Significado | después ya no lo puedo usar | lo puedo volver a usar |

### Paso 5: orden de llenado

`tab[i][j]` usa:
- `tab[i-1][j]`, de la fila anterior,
- `tab[i][j - Ki]`, de la **misma fila** pero en una columna más a la izquierda.

Por eso se llena fila por fila y, dentro de cada fila, **de izquierda a derecha** (`j` creciente). Así, cuando se necesita `tab[i][j - Ki]`, ya está calculado *con* el elemento `i` incluido.

### Paso 6: la tabla completa del ejemplo (C = {5, 6, 3}, M = 10)

| i (Ki) \ j | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 |
|---|---|---|---|---|---|---|---|---|---|---|---|
| 0 (5) | 0 | 0 | 0 | 0 | 0 | 1 | 0 | 0 | 0 | 0 | 1 |
| 1 (6) | 0 | 0 | 0 | 0 | 0 | 1 | 1 | 0 | 0 | 0 | 1 |
| 2 (3) | 0 | 0 | 0 | 1 | 0 | 1 | 1 | 0 | 1 | 1 | 1 |

Cómo se calculan algunas celdas:

- Fila 0: solo hay múltiplos de 5, así que hay `1` en `j = 5` y `j = 10`.
- `tab[1][6]`: `j == K1 = 6`, así que es `1` (caso 2).
- `tab[1][10] = tab[0][10] || tab[1][4] = 1 || 0 = 1`, con {5, 5}.
- `tab[2][3]`: `j == K2 = 3`, así que es `1`.
- `tab[2][6] = tab[1][6] || tab[2][3] = 1`, con {6} o con {3, 3}.
- `tab[2][8] = tab[1][8] || tab[2][5] = 0 || 1 = 1`, con {5, 3}.
- `tab[2][9] = tab[1][9] || tab[2][6] = 0 || 1 = 1`, con {3, 3, 3}. Acá se ve la repetición: `tab[2][6]` ya usaba el 3 y se le suma otro.
- `tab[2][7] = tab[1][7] || tab[2][4] = 0 || 0 = 0`. El 7 no se puede formar con 5, 6 y 3.

La última fila es exactamente la salida esperada: `0 0 1 0 1 1 0 1 1 1`.

### Paso 7: muchas consultas con una sola tabla

Las `P` consultas comparten la misma tabla. Por eso la entrada da la cota `M`: se construye `tab` **una sola vez** hasta `M`, en O(N·M), y después cada consulta se responde en O(1) leyendo `tab[N-1][Mi]`.

Si en cambio se recalculara la tabla para cada consulta, el costo sería O(P·N·M).

### Paso 8: órdenes

- Tiempo: O(N·M) para construir la tabla, más O(P) para las consultas.
- Espacio: O(N·M).

Como solo hace falta la última fila, se podría usar un único array `puedo[0..M]` que se va actualizando de izquierda a derecha para cada elemento. Eso deja O(M) espacial. La letra pide O(N·M), así que la tabla completa también cumple.

### Corrección respecto de la entrega original

`initMatriz` reservaba `cantidad` punteros pero el `for` iba hasta `i <= cantidad` y escribía uno de más (*heap buffer overflow*, detectado con `-fsanitize=address`). Además, las filas se liberaban con `delete` en lugar de `delete[]`. Las dos cosas están corregidas.

### Para comparar

En el Ejercicio 9 de 2022 se pide el mismo problema con **backtracking**, devolviendo la *mínima cantidad* de elementos. Ese resultado también se puede obtener con PD cambiando `bool` por `int`: `tab[i][j] = min(tab[i-1][j], 1 + tab[i][j-Ki])`.
