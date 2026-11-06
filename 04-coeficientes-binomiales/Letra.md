## Coeficientes binomiales (triángulo de Pascal)

Nombre de archivo: coeficientes.cpp (originalmente `Ejercicio 9/ejercicio9.cpp`, A2 2023)

> El repo de 2023 no incluye la letra original. Esta letra se reconstruyó a partir del código y de los casos de prueba.

### Letra

Se desea calcular varios coeficientes binomiales `C(n, k)` ("n tomados de a k").

### Input

La primera línea contiene `N`, una cota para todos los `n` consultados. La segunda línea contiene `P`, la cantidad de consultas. Las siguientes `P` líneas contienen `n k` (con `0 ≤ k ≤ n ≤ N`).

En las pruebas las consultas vienen ordenadas por `n` de forma no decreciente y sin saltear filas.

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

### Idea de la solución (PD)

Recurrencia de Pascal: `C(n, 0) = 1`, `C(n, k) = C(n-1, k-1) + C(n-1, k)`.

Cada fila del triángulo depende solo de la anterior, así que alcanza con dos arrays de tamaño `N+1` (`actual` y un auxiliar). Como las consultas vienen ordenadas por `n`, antes de responder se avanzan filas hasta llegar a la `n` pedida y se lee `actual[k]`. En total se calculan a lo sumo `N` filas: O(N²) temporal, O(N) espacial.

### Observación sobre la versión original

La solución entregada en 2023 calculaba la fila siguiente a partir de un array `anterior` que solo quedaba sincronizado cuando la fila recibía al menos dos consultas. Pasaba las pruebas porque en ellas todas las filas tienen varias consultas, pero, por ejemplo, con las consultas `1 1`, `2 1`, `3 1` respondía `1 1 2` en lugar de `1 2 3`. La versión de este repo avanza fila por fila hasta `n` y no depende de eso.
