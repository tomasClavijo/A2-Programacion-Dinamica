## Mochila 0/1: greedy vs programación dinámica

Nombre de archivo: `mochila_greedy_vs_pd.cpp` · Código explicado línea por línea: [CODIGO.md](CODIGO.md)

> Une los dos materiales de mochila: [`01-mochila-01-ejemplo`](../01-mochila-01-ejemplo/Letra.md) de este repo (PD) y [`02-mochila`](https://github.com/tomasClavijo/greedy/tree/main/02-mochila) del repo de [greedy](https://github.com/tomasClavijo/greedy). Usa las mismas instancias que el de greedy para poder comparar.

### Letra

Se tiene una mochila de capacidad `C` y `N` objetos, cada uno con un peso y un valor. Cada objeto se lleva entero o no se lleva (mochila **0/1**). Se quiere maximizar el valor total sin pasarse de la capacidad.

Se pide:

a) Resolverlo con un algoritmo **greedy** que en cada paso meta el objeto de mejor **ratio** valor/peso que todavía entre.

b) Resolverlo con **programación dinámica**, indicando además qué objetos se llevan.

c) Mostrar una instancia en la que (a) no da el óptimo y explicar en qué se equivoca.

### Input

La primera línea tiene `N` y `C`. Las siguientes `N` líneas tienen el peso y el valor de cada objeto (enteros positivos).

### Salida

```
greedy: <valor> [<objetos usados>]
pd: <valor> [<objetos usados>]
```

Los objetos se numeran del 1 al `N` según el orden de la entrada y se listan de menor a mayor. Si dos objetos empatan en ratio, el greedy prefiere el que aparece primero.

### Ejemplo de entrada y salida

#### Entrada
```
4 7
1 6
2 6
5 9
7 13
```

#### Salida
```
greedy: 12 [1 2]
pd: 15 [1 3]
```

---

## Explicación paso a paso

Todo el ejercicio sigue una sola instancia, la del ejemplo (**capacidad 7**):

| Objeto | Peso | Valor | Ratio |
|---|---|---|---|
| A (1) | 1 | 6 | 6 |
| B (2) | 2 | 6 | 3 |
| C (3) | 5 | 9 | 1,8 |
| D (4) | 7 | 13 | 1,86 |

Primero se resuelve con greedy, después se ve por qué falla, después se resuelve con PD y al final se mira **en qué celda de la tabla la PD corrige el error del greedy**.

---

## Parte A: greedy

### Paso A1: la idea

Un greedy arma la solución de a un paso. En cada paso toma lo que *parece* mejor según un criterio y **nunca vuelve atrás**.

El criterio acá es el ratio `valor / peso`, o sea, cuánto rinde cada kilo del objeto. En el [repo de greedy](https://github.com/tomasClavijo/greedy/blob/main/02-mochila/EXPLICACION.md) se ve que los otros criterios (mayor valor, menor peso) también fallan.

### Paso A2: el algoritmo

Repetir:

1. Entre los objetos que **no se usaron** y que **entran** en lo que queda, buscar el de mejor ratio.
2. Si no hay ninguno, terminar.
3. Si hay, meterlo: restar su peso de lo libre y sumar su valor.

### Paso A3: la traza con capacidad 7

| Vuelta | Candidatos (no usados y que entran) | Elige | Libre | Total |
|---|---|---|---|---|
| — | | | 7 | 0 |
| 1 | A (6), B (3), C (1,8), D (1,86) | **A** | 7 − 1 = 6 | 6 |
| 2 | B (3), C (1,8). D pesa 7 y no entra | **B** | 6 − 2 = 4 | 12 |
| 3 | ninguno: C pesa 5 y D pesa 7 | — | 4 | **12** |

Resultado: **12**, con A y B. Sobran 4 kilos que no se usan.

### Paso A4: ¿es el óptimo?

Con 4 objetos hay 2⁴ = 16 subconjuntos. Estos son los que entran:

| Lleva | Peso | Valor |
|---|---|---|
| nada | 0 | 0 |
| A | 1 | 6 |
| B | 2 | 6 |
| C | 5 | 9 |
| D | 7 | 13 |
| A + B | 3 | 12 ← greedy |
| A + C | 6 | **15** |
| B + C | 7 | **15** |

El óptimo es **15** y el greedy da 12. Con una sola instancia que falla alcanza para demostrar que el criterio **no** es correcto.

### Paso A5: dónde se equivoca

Tomar A no estaba mal, porque A + C es óptimo. El error está en la **vuelta 2**: con 6 kilos libres el greedy toma B porque rinde más por kilo que C. Pero B gasta justo los kilos que C necesitaba (después de B quedan 4, y C pesa 5).

El ratio no ve **qué objetos dejan de entrar** por elegir uno. En la 0/1, los kilos que sobran valen 0.

Otros dos casos para mostrar:

- **Capacidad 10** (prueba `03`): X (6, 60), Y (5, 45), Z (5, 40). El greedy toma X (ratio 10) y quedan 4 kilos donde no entra nada: total 60. El óptimo es Y + Z = 85, y **no usa el objeto de mejor ratio**.
- **Capacidad 100** (prueba `06`): objetos (1, 2) y (100, 100). El greedy toma el primero (ratio 2) y el segundo ya no entra: total 2. El óptimo es 100. El greedy puede quedar **todo lo lejos que se quiera** del óptimo.

---

## Parte B: programación dinámica

### Paso B1: qué tiene que hacer distinto

El greedy falló porque decidió B **sin mirar qué pasaba después**. Lo correcto es probar las dos opciones (poner o no poner) para cada objeto. Con fuerza bruta eso cuesta O(2ᴺ). Pero muchos caminos llegan a la misma situación ("ya decidí los primeros `i` objetos y me quedan `c` kilos"), y esa situación tiene siempre la misma mejor continuación. La PD guarda cada situación una sola vez en una tabla.

### Paso B2: definir el subproblema

> `tab[i][c]` = máximo valor que se puede obtener usando **solo los objetos 1..i** con una mochila de capacidad **c**.

La respuesta es `tab[N][C]`.

### Paso B3: caso base

`tab[0][c] = 0`, porque sin objetos el valor es 0.

### Paso B4: recurrencia

Para el objeto `i` con capacidad `c`:

- Si **no entra** (`peso_i > c`): `tab[i][c] = tab[i-1][c]`.
- Si **entra**: `tab[i][c] = max( tab[i-1][c],  valor_i + tab[i-1][c - peso_i] )`. La primera opción es no ponerlo y la segunda es ponerlo.

Al ponerlo se baja a la fila `i-1`, porque cada objeto se usa una sola vez.

**La diferencia con el greedy:** el greedy elige una de las dos opciones mirando solo el objeto. La PD calcula **las dos** y se queda con la mejor, para **todas** las capacidades, así que más adelante puede usar cualquiera de esos resultados.

### Paso B5: la tabla completa con capacidad 7

Se llena fila por fila, de arriba hacia abajo (cada fila solo usa la anterior).

| i \ c | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 |
|---|---|---|---|---|---|---|---|---|
| 0 (nada)      | 0 | 0 | 0 | 0  | 0  | 0  | 0  | 0  |
| 1 A (1, 6)    | 0 | 6 | 6 | 6  | 6  | 6  | 6  | 6  |
| 2 B (2, 6)    | 0 | 6 | 6 | 12 | 12 | 12 | 12 | 12 |
| 3 C (5, 9)    | 0 | 6 | 6 | 12 | 12 | 12 | 15 | **15** |
| 4 D (7, 13)   | 0 | 6 | 6 | 12 | 12 | 12 | 15 | **15** |

Celdas calculadas a mano:

- `tab[1][1] = max(tab[0][1], 6 + tab[0][0]) = max(0, 6) = 6`.
- `tab[2][1] = tab[1][1] = 6`, porque B pesa 2 y no entra en 1.
- `tab[2][3] = max(tab[1][3], 6 + tab[1][1]) = max(6, 12) = 12`: A + B.
- `tab[2][7] = max(tab[1][7], 6 + tab[1][5]) = max(6, 12) = 12`. Hasta acá coincide con el greedy.
- `tab[3][7] = max(tab[2][7], 9 + tab[2][2]) = max(12, 9 + 6) = 15`.
- `tab[4][7] = max(tab[3][7], 13 + tab[3][0]) = max(15, 13) = 15`.

La traza de **todas** las celdas, vuelta por vuelta de los `for`, está en [CODIGO.md](CODIGO.md#traza-completa-de-los-for).

### Paso B6: la celda donde la PD corrige al greedy

Hay que mirar `tab[3][7]`, el momento en que aparece C con la mochila de 7 kilos:

| Opción | Cuenta | Valor | Qué significa |
|---|---|---|---|
| No poner C | `tab[2][7]` | 12 | Lo mejor con A y B: A + B. **Es la solución del greedy.** |
| Poner C | `9 + tab[2][2]` | 9 + 6 = **15** | C, y en los 2 kilos que quedan, lo mejor de A y B: uno solo. |

El greedy ya había metido A **y** B cuando llegó a C, y no podía sacar nada. La PD tiene guardado `tab[2][2] = 6` ("lo mejor con A y B en solo 2 kilos"), y con eso puede evaluar "C + uno de los dos". En la práctica es como si **se arrepintiera de B**, algo que el greedy no puede hacer.

Con capacidad 10 pasa lo mismo: `tab[3][10] = max(60, 40 + tab[2][5]) = max(60, 40 + 45) = 85`. "No poner Z" vale 60, que es el greedy con X solo. "Poner Z" usa `tab[2][5] = 45`, que es Y, y deja afuera a X.

### Paso B7: reconstruir qué objetos se llevan

Se recorre la tabla desde `tab[4][7]` hacia arriba. Si `tab[i][c] != tab[i-1][c]`, el objeto `i` se usó y se descuenta su peso.

| (i, c) | tab[i][c] | tab[i-1][c] | ¿Se usó? | c queda |
|---|---|---|---|---|
| (4, 7) | 15 | 15 | No (D) | 7 |
| (3, 7) | 15 | 12 | **Sí (C)** | 7 − 5 = 2 |
| (2, 2) | 6  | 6  | No (B) | 2 |
| (1, 2) | 6  | 0  | **Sí (A)** | 2 − 1 = 1 |

Resultado: A + C = **15**, la salida `pd: 15 [1 3]`.

B + C también vale 15. La reconstrucción encuentra *una* de las soluciones óptimas.

### Paso B8: la PD no depende del orden de la entrada

Las pruebas `04` y `05` tienen los mismos objetos en distinto orden (A y D empatan en ratio):

| Prueba | Greedy | PD |
|---|---|---|
| `04` (A antes que D) | 30 | 30 |
| `05` (D antes que A) | **28** | 30 |

El greedy cambia según quién gane el empate. La PD considera todas las combinaciones, así que el valor óptimo es siempre el mismo.

---

## Resumen

| | Greedy | PD |
|---|---|---|
| Qué hace en cada paso | Elige **una** opción mirando solo el objeto | Prueba **las dos** opciones (poner o no) y guarda la mejor |
| ¿Vuelve atrás? | Nunca | No hace falta: tiene todas las alternativas en la tabla |
| ¿Da el óptimo? | No, y puede quedar muy lejos | Sí |
| Tiempo | O(N²) | O(N · C) |
| Espacio | O(N) | O(N · C) |
| Si se reordena la entrada | Puede cambiar el resultado | El valor no cambia |

Una advertencia: O(N · C) depende del **valor** de `C`. Con `C = 10⁹` la tabla no entra en memoria, aunque haya pocos objetos.

## Para pensar

1. ¿Por qué el greedy no puede corregir su error aunque "se dé cuenta" en la vuelta 3?
2. Si todos los objetos pesan lo mismo, ¿el greedy por ratio da el óptimo?
3. En la recurrencia, ¿qué pasaría si se escribiera `tab[i][c - peso_i]` en lugar de `tab[i-1][c - peso_i]`?
4. ¿Por qué la reconstrucción necesita la tabla completa y no le alcanza con la última fila?

<details>
<summary>Respuestas</summary>

1. Porque greedy significa no volver atrás: B ya está adentro y no hay ningún paso que lo saque. Si se permitiera sacarlo y probar otra cosa, ya sería backtracking.
2. Sí. Si todos pesan lo mismo, entra la misma cantidad de objetos con cualquier elección, así que no quedan huecos distintos. Conviene llevar los más valiosos, y en ese caso mejor ratio es lo mismo que mayor valor.
3. El objeto se podría usar varias veces: al ponerlo, uno se queda en la misma fila y lo puede volver a poner. Esa es la mochila *no acotada*, como el [ejercicio 02](../02-subconjunto-suma-m/Letra.md).
4. Para saber si el objeto `i` se usó hay que comparar `tab[i][c]` con `tab[i-1][c]`. Si se guarda una sola fila, ese valor anterior se pierde.

</details>
