# A2 — Programación Dinámica

Ejercicios de programación dinámica de los obligatorios de Estructuras de Datos y Algoritmos 2, tomados de:

- [A2-Obligatorio-2022](https://github.com/tomasClavijo/A2-Obligatorio-2022)
- [A2-Obligatorio-2023](https://github.com/tomasClavijo/A2-Obligatorio-2023)

Cada carpeta tiene la letra (`Letra.md`, con la idea de la solución), la solución en C++ y, cuando existen, los casos de prueba.

| # | Ejercicio | Origen | Tipo de PD | Orden |
|---|-----------|--------|------------|-------|
| 01 | [Mochila 0/1](01-mochila-01-ejemplo/Letra.md) | 2022, `Ejemplos/PD_Mochila.cpp` | Mochila 0/1 con reconstrucción | O(N·C) |
| 02 | [Subconjunto de suma M](02-subconjunto-suma-m/Letra.md) | 2022, Ejercicio 8 | Mochila no acotada (booleana) | O(N·M) |
| 03 | [Selección de archivos](03-archivos-mochila-2d/Letra.md) | 2023, Ejercicio 7 | Mochila 0/1 con dos restricciones | O(N·S·L) |
| 04 | [Coeficientes binomiales](04-coeficientes-binomiales/Letra.md) | 2023, Ejercicio 9 | Triángulo de Pascal, O(N) espacio | O(N²) |

Un orden de lectura posible es 01 → 02 → 03 → 04: la mochila clásica, después la variante con repetición (se queda en la misma fila), después la de dos capacidades con una sola matriz recorrida de atrás para adelante, y por último una PD fuera de la familia de la mochila.

## Cómo correr las pruebas

```bash
./run_tests.sh        # compila y prueba todo
./run_tests.sh 02     # solo un ejercicio
```

Requiere `g++`. El 01 no tiene casos de prueba: los datos están en el código y solo imprime el resultado.

## Cambios respecto a las entregas originales

- **02 (suma M):** `initMatriz` reservaba `N` filas pero inicializaba `N+1`, así que escribía fuera del array. Además, las filas se liberaban con `delete` en lugar de `delete[]`. Las dos cosas están corregidas.
- **01 (mochila):** se agregó un `main` para poder ejecutarlo.
- **03 y 04:** los repos de 2023 no tienen letras, así que se reconstruyeron a partir del código y las pruebas.
- **04 (binomiales):** la versión original solo daba bien cuando cada fila tenía al menos dos consultas (por ejemplo, `1 1`, `2 1`, `3 1` devolvía `1 1 2`). Se reescribió para que avance fila por fila hasta `n`. Hay más detalle en su letra.
- Se sacaron los `.exe` y las salidas `*.mine.txt`.

## No incluidos

Estos ejercicios de los mismos obligatorios tienen relación pero no son de PD:

- 2022, Ejercicio 9: el mismo problema de suma M resuelto con **backtracking** (mínima cantidad de elementos). Sirve para comparar con el 02.
- 2023, Ejercicio 10: camino más corto en la facultad pasando por bedelía. Es **backtracking**.
- 2022, Ejercicio 10 (máximo número concatenado) y 2023, Ejercicio 8 (plataformas): son greedy y ordenamiento.
