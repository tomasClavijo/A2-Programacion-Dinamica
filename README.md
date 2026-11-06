# A2 — Programación Dinámica

Ejercicios de programación dinámica de los obligatorios de Estructuras de Datos y Algoritmos 2, tomados de:

- [A2-Obligatorio-2022](https://github.com/tomasClavijo/A2-Obligatorio-2022)
- [A2-Obligatorio-2023](https://github.com/tomasClavijo/A2-Obligatorio-2023)

Cada carpeta tiene:

- `Letra.md`: la letra y una **explicación paso a paso**, con la tabla de PD del ejemplo llenada y algunas celdas calculadas a mano.
- La solución en C++, comentada.
- `Pruebas/`: casos de entrada y salida esperada.

| # | Ejercicio | Origen | Tipo de PD | Orden |
|---|-----------|--------|------------|-------|
| 01 | [Mochila 0/1](01-mochila-01-ejemplo/Letra.md) | 2022, `Ejemplos/PD_Mochila.cpp` | Mochila 0/1 con reconstrucción | O(N·C) |
| 02 | [Subconjunto de suma M](02-subconjunto-suma-m/Letra.md) | 2022, Ejercicio 8 | Mochila no acotada (booleana) | O(N·M) |
| 03 | [Selección de archivos](03-archivos-mochila-2d/Letra.md) | 2023, Ejercicio 7 | Mochila 0/1 con dos restricciones | O(N·S·L) |
| 04 | [Coeficientes binomiales](04-coeficientes-binomiales/Letra.md) | 2023, Ejercicio 9 | Triángulo de Pascal, O(N) espacio | O(N²) |

## Orden de lectura sugerido

1. **01, Mochila 0/1.** El caso clásico: tabla `objetos × capacidad`, decisión "lo pongo o no", y cómo reconstruir la solución recorriendo la tabla hacia atrás.
2. **02, Suma M.** Igual que la mochila pero con repetición. El único cambio es que al usar un elemento se queda en la misma fila (`tab[i][j-Ki]` en lugar de `tab[i-1][...]`). También muestra cómo responder muchas consultas con una sola tabla.
3. **03, Archivos.** Una mochila con dos capacidades. Muestra cómo ahorrar una dimensión usando una sola matriz recorrida **de mayor a menor**, y por qué ese orden evita usar un archivo dos veces.
4. **04, Binomiales.** Una PD fuera de la familia de la mochila: la regla de Pascal, guardando solo la fila actual.

## Método general (los pasos que siguen todas las explicaciones)

Todas las `Letra.md` siguen el mismo esquema:

1. **¿Por qué PD?** El problema se descompone en subproblemas que **se repiten**, y la solución óptima se arma con soluciones óptimas de subproblemas (subestructura óptima). Si existe un greedy que funcione, no hace falta PD.
2. **Definir el subproblema.** Qué significa exactamente cada celda `tab[...]`, en una frase. Este es el paso más importante: si la definición no es clara, la recurrencia tampoco lo va a ser.
3. **Casos base.** Las celdas que se saben sin calcular nada: sin objetos, capacidad 0, fila 0, etc.
4. **Recurrencia.** Cómo se calcula una celda a partir de otras más chicas, considerando todas las decisiones posibles (por ejemplo, poner o no poner el objeto).
5. **Orden de llenado.** En qué orden recorrer la tabla para que cada celda que se lee ya esté calculada. Acá aparecen las diferencias finas: de izquierda a derecha en el 02, de derecha a izquierda en el 03.
6. **Dónde está la respuesta.** Qué celda (o fila) responde el problema original.
7. **Reconstrucción** (si se pide). Cómo recuperar *qué* decisiones llevaron al óptimo, además del valor.
8. **Órdenes de tiempo y espacio**, y si se puede ahorrar memoria guardando menos filas.

## Cómo correr las pruebas

```bash
./run_tests.sh        # compila y prueba todo
./run_tests.sh 02     # solo un ejercicio
```

Requiere `g++`. El 01 no tiene casos de prueba: los datos están en el código y solo imprime el resultado.

## Cambios respecto a las entregas originales

- **02 (suma M):** `initMatriz` reservaba `N` filas pero inicializaba `N+1`, así que escribía fuera del array. Además, las filas se liberaban con `delete` en lugar de `delete[]`. Las dos cosas están corregidas.
- **04 (binomiales):** la versión original solo daba bien cuando cada fila tenía al menos dos consultas (por ejemplo, `1 1`, `2 1`, `3 1` devolvía `1 1 2`). Se reescribió para que avance fila por fila hasta `n`. Se agregaron pruebas para ese caso (`una_consulta_por_fila`, `saltea_filas`).
- **01 (mochila):** se agregó un `main`, se ordenó el formato de la salida y se libera la tabla.
- **03 (archivos):** se libera la tabla de PD. Se agregó como prueba el ejemplo chico trazado a mano en la letra (`ejemplo_chico`).
- **03 y 04:** los repos de 2023 no tienen letras, así que se reconstruyeron a partir del código y las pruebas.
- Todo el código está comentado. Se sacaron los `.exe` y las salidas `*.mine.txt`.

## No incluidos

Estos ejercicios de los mismos obligatorios tienen relación pero no son de PD:

- 2022, Ejercicio 9: el mismo problema de suma M resuelto con **backtracking** (mínima cantidad de elementos). Sirve para comparar con el 02.
- 2023, Ejercicio 10: camino más corto en la facultad pasando por bedelía. Es **backtracking**.
- 2022, Ejercicio 10 (máximo número concatenado) y 2023, Ejercicio 8 (plataformas): son greedy y ordenamiento.
