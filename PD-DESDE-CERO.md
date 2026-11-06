# Programación dinámica desde cero

Guía para entender el tema y para darlo en clase. No supone nada previo más que recursión. Al final hay un guion de clase y las dudas típicas de los alumnos.

---

## 1. La idea en una frase

> **Programación dinámica = resolver un problema recursivo, pero guardando la respuesta de cada subproblema para no calcularlo dos veces.**

Ahí está todo. Lo demás es cómo aplicarlo bien.

---

## 2. El ejemplo más simple: Fibonacci

`fib(0) = 0`, `fib(1) = 1`, `fib(n) = fib(n-1) + fib(n-2)`.

### 2.1 Recursión directa (sin PD)

```cpp
int fib(int n) {
    if (n <= 1) return n;                  // casos base
    return fib(n - 1) + fib(n - 2);        // recurrencia
}
```

Es correcta, pero lentísima. Este es el árbol de llamadas de `fib(5)`:

```
                    fib(5)
                 /          \
            fib(4)            fib(3)
           /      \           /     \
       fib(3)    fib(2)    fib(2)   fib(1)
       /    \    /    \    /    \
   fib(2) fib(1) fib(1) fib(0) fib(1) fib(0)
   /    \
fib(1) fib(0)
```

- `fib(3)` se calcula **2 veces**, `fib(2)` **3 veces** y `fib(1)` **5 veces**.
- Son 15 llamadas para `fib(5)`. Para `fib(40)` son **331.160.281 llamadas**, y crece exponencialmente.

El problema no es la recursión: es que **se resuelve muchas veces el mismo subproblema**. Eso se llama **subproblemas superpuestos**, y es la señal de que PD sirve.

### 2.2 Memoización (PD *top-down*)

La misma función recursiva, con una "libreta" donde se anota cada resultado:

```cpp
int* memo;   // memo[n] = fib(n), o -1 si todavía no se calculó

int fib(int n) {
    if (n <= 1) return n;
    if (memo[n] != -1) return memo[n];          // ¿ya lo calculé? lo devuelvo
    memo[n] = fib(n - 1) + fib(n - 2);          // si no, lo calculo y lo anoto
    return memo[n];
}

// en el main:
memo = new int[n + 1];
for (int i = 0; i <= n; i++) memo[i] = -1;
```

Ahora cada `fib(k)` se calcula **una sola vez**. Para `fib(40)` son unas 40 cuentas en lugar de 331 millones.

### 2.3 Tabulación (PD *bottom-up*)

En lugar de empezar por `fib(n)` y bajar, se arranca desde los casos base y se sube:

```cpp
int fib(int n) {
    int* tab = new int[n + 1];
    tab[0] = 0;                                 // casos base
    tab[1] = 1;
    for (int i = 2; i <= n; i++)                // de chico a grande
        tab[i] = tab[i - 1] + tab[i - 2];       // recurrencia
    int res = tab[n];
    delete[] tab;
    return res;
}
```

| i | 0 | 1 | 2 | 3 | 4 | 5 | 6 |
|---|---|---|---|---|---|---|---|
| tab[i] | 0 | 1 | 1 | 2 | 3 | 5 | 8 |

Cada celda se llena sumando las dos anteriores, que **ya están calculadas** porque se recorre de izquierda a derecha.

**Todos los ejercicios de este repo usan tabulación.**

### 2.4 Lo que hay que ver en Fibonacci

| | Recursión | Memoización | Tabulación |
|---|---|---|---|
| Recurrencia | `fib(n-1) + fib(n-2)` | la misma | la misma |
| ¿Guarda resultados? | No | Sí, en `memo` | Sí, en `tab` |
| Orden de cálculo | Desde n hacia abajo, repitiendo | Desde n hacia abajo, sin repetir | Desde 0 hacia arriba |
| Llamadas / cuentas para n = 40 | 331 millones | ~40 | ~40 |

**La recurrencia es la misma en las tres.** La PD no cambia *qué* se calcula, cambia *cuántas veces*.

---

## 3. ¿Cuándo sirve PD?

Hacen falta dos condiciones:

1. **Subestructura óptima:** la solución del problema grande se arma con soluciones de problemas más chicos del mismo tipo. Es lo que permite escribir una recurrencia.
2. **Subproblemas superpuestos:** esos problemas chicos se repiten muchas veces. Es lo que hace que guardarlos valga la pena.

Comparación con las otras técnicas del curso:

| Técnica | Cómo decide | ¿Repite subproblemas? | Ejemplo |
|---|---|---|---|
| **Dividir y conquistar** | Parte en subproblemas **independientes** | No | mergeSort: las dos mitades no comparten nada |
| **Greedy** | Elige **una** opción (la que parece mejor) y nunca vuelve atrás | — | Ordenar por ratio en la mochila |
| **Backtracking** | Prueba **todas** las opciones, recursivamente | Sí, y los recalcula | Suma M con BT (2022, ej. 9) |
| **PD** | Prueba **todas** las opciones, pero guarda cada subproblema | Sí, y los reutiliza | Mochila con tabla |

PD es "backtracking que no repite trabajo". Prueba todas las opciones como el backtracking, pero cada situación posible se resuelve una sola vez.

---

## 4. La receta (los 8 pasos)

Todos los ejercicios del repo siguen estos pasos. Para cada uno, la pregunta que hay que hacerse:

| Paso | Pregunta | En la mochila 0/1 |
|---|---|---|
| 1. ¿Por qué PD? | ¿Un greedy alcanza? ¿Se repiten subproblemas? | Greedy falla; los estados (objeto, capacidad) se repiten |
| 2. Subproblema | ¿Qué significa **una celda**, en una frase? | `tab[i][c]` = mejor valor con los objetos 1..i y capacidad c |
| 3. Casos base | ¿Qué celdas sé sin calcular? | Sin objetos: `tab[0][c] = 0` |
| 4. Recurrencia | ¿Qué decisión tomo en cada celda y qué opciones hay? | Poner o no el objeto i: `max(tab[i-1][c], v + tab[i-1][c-p])` |
| 5. Orden de llenado | ¿Qué celdas lee cada celda? ¿Ya están calculadas? | Solo lee la fila i-1, así que se llena de arriba hacia abajo |
| 6. Respuesta | ¿Qué celda responde el problema? | `tab[N][C]` |
| 7. Reconstrucción | ¿Cómo sé *qué* decisiones tomé? | Hacia atrás: si `tab[i][c] != tab[i-1][c]`, usé el i |
| 8. Órdenes | ¿Cuántas celdas hay? ¿Cuánto cuesta cada una? | (N+1)·(C+1) celdas, O(1) cada una: O(N·C) |

**El paso 2 es el más importante.** Si la definición de la celda está clara, la recurrencia suele salir sola: se piensa "¿cuál es la última decisión que tomo?" y se escribe una opción por cada respuesta posible.

---

## 5. La mochila 0/1 desde la recursión

Así se llega a la tabla de la mochila partiendo de una recursión, igual que con Fibonacci.

### 5.1 Pensarlo recursivo

`mejor(i, c)` = el mejor valor usando los objetos 1..i con capacidad c.

Para el objeto `i` hay **dos opciones**:

```cpp
int mejor(int i, int c) {
    if (i == 0) return 0;                                  // sin objetos
    int noPongo = mejor(i - 1, c);                         // opción 1
    if (peso[i] > c) return noPongo;                       // no entra
    int pongo = valor[i] + mejor(i - 1, c - peso[i]);      // opción 2
    return max(noPongo, pongo);
}
// respuesta: mejor(N, C)
```

Es fuerza bruta: cada objeto abre dos ramas, así que son O(2ᴺ) llamadas.

### 5.2 Ver que se repite

Una llamada depende solo de dos números: `i` y `c`. Hay como mucho `(N+1)·(C+1)` pares distintos, pero 2ᴺ llamadas. Si N = 30 y C = 100, son **1.073.741.824** llamadas para solo **3.131** pares distintos: casi todas las llamadas se repiten.

Ejemplo: si los objetos 1 y 2 pesan 3 cada uno, "pongo el 1 y no el 2" y "pongo el 2 y no el 1" llegan las dos a `mejor(0, c - 3)`. Dos caminos distintos llevan al mismo subproblema.

### 5.3 Guardarlo en una tabla

Se cambia cada llamada `mejor(i, c)` por una celda `tab[i][c]`, y se llenan en un orden en el que lo que se necesita ya esté:

| Recursión | Tabla |
|---|---|
| `if (i == 0) return 0;` | La fila 0 en ceros |
| `mejor(i - 1, c)` | `tab[i - 1][c]` |
| `mejor(i - 1, c - peso[i])` | `tab[i - 1][c - peso[i]]` |
| `return max(noPongo, pongo);` | `tab[i][c] = max(...)` |
| llamada inicial `mejor(N, C)` | respuesta `tab[N][C]` |
| (el orden lo da la recursión) | `for i` de 1 a N, `for c` de 0 a C |

Así se obtiene exactamente el código del [ejercicio 01](01-mochila-01-ejemplo/mochila01.cpp) y de la PD del [ejercicio 05](05-mochila-greedy-vs-pd/CODIGO.md#5-pd-01-líneas-141169--lo-central). **Escribir la tabla no es inventar algo nuevo: es la recursión, guardada.**

### 5.4 Leer una celda en voz alta

Este es el ejercicio mental que más ayuda. Tomemos `tab[3][7] = 15` del ejemplo de capacidad 7 (A, B, C, D):

> "Si solo puedo usar A, B y C, y la mochila aguanta 7 kilos, lo mejor que puedo llevar vale 15."

Y la cuenta que lo produjo:

> "Si **no** pongo C, lo mejor con A y B en 7 kilos es `tab[2][7] = 12`. Si **pongo** C (5 kilos, vale 9), me quedan 2 kilos para A y B, y lo mejor ahí es `tab[2][2] = 6`. Entonces 9 + 6 = 15. Gana poner C."

Si se puede leer cualquier celda así, la PD está entendida.

---

## 6. Variantes que aparecen en el repo

Todas son la misma receta. Lo que cambia es qué significa la celda y qué opciones hay:

| Ejercicio | La celda significa | Opciones en cada celda | Detalle clave |
|---|---|---|---|
| [01 Mochila 0/1](01-mochila-01-ejemplo/Letra.md) | Mejor valor con objetos 1..i y capacidad c | Poner / no poner | Al poner se baja a `i-1` (no se repite) |
| [02 Suma M](02-subconjunto-suma-m/Letra.md) | ¿Se llega a la suma j con elementos 0..i? (`bool`) | Usar / no usar | Al usar se queda en la fila `i` (se puede repetir) |
| [03 Archivos](03-archivos-mochila-2d/Letra.md) | Mejor puntaje con l líneas y tamaño s | Poner / no poner | Una sola matriz recorrida de mayor a menor |
| [04 Binomiales](04-coeficientes-binomiales/Letra.md) | C(n, k) | Incluir / no incluir un elemento | Solo se guarda la fila anterior |
| [05 Greedy vs PD](05-mochila-greedy-vs-pd/Letra.md) | Igual que 01 | Igual que 01 | Se compara contra el greedy que elige una sola opción |

---

## 7. Errores típicos

1. **No poder decir qué significa una celda.** Si no se puede decir en una frase, la recurrencia va a estar mal. Hay que escribir la definición antes de escribir código.
2. **Olvidar los casos base**, o inicializar mal. En C++, `new int[n]` sin `()` deja basura en la memoria; con `()` deja ceros.
3. **Llenar en un orden en el que se lee una celda que todavía no se calculó.** Antes de escribir los `for`, hay que mirar qué celdas usa la recurrencia.
4. **Confundir `i - 1` con `i`** en la mochila: con `i - 1` cada objeto se usa una vez, con `i` se puede repetir.
5. **Índices corridos.** Si la tabla tiene la fila 0 para el caso base, el objeto de la fila `i` es `objetos[i - 1]`.
6. **Pensar que O(N·C) es polinomial.** Depende del *valor* de C: con C = 10⁹ la tabla no entra en memoria.

---

## 8. Guion para dar la clase (unos 75 minutos)

| Min | Tema | Qué hacer | Material |
|---|---|---|---|
| 0–15 | Fibonacci | Escribir la recursión. Dibujar el árbol de `fib(5)` y marcar los repetidos con colores. Preguntar: "¿cuántas veces calculamos fib(2)?". Agregar la libreta (memo), y después darla vuelta (tabla). | Sección 2 |
| 15–20 | Cuándo sirve PD | Las dos condiciones. Tabla comparando con DyC, greedy y BT. | Sección 3 |
| 20–30 | Mochila: el problema | Plantear la capacidad 7 y hacer votar un criterio greedy. Mostrar que el ratio da 12 y que A + C da 15. | [05, parte A](05-mochila-greedy-vs-pd/Letra.md#parte-a-greedy-01-por-ratio) |
| 30–40 | Mochila: la recursión | "¿Qué decisión tomo con el último objeto?" → dos opciones → `mejor(i, c)`. Mostrar que solo depende de (i, c), así que se repite. | Sección 5.1–5.2 |
| 40–60 | Mochila: la tabla | Llenar la tabla de capacidad 7 **en el pizarrón con ellos**, fila por fila. Frenar en `tab[3][7]` y leerla en voz alta. | [05, CODIGO.md 5.4](05-mochila-greedy-vs-pd/CODIGO.md#54-traza-completa-de-los-for) |
| 60–65 | Reconstrucción | Recorrer la tabla hacia atrás y encontrar A + C. | [05, CODIGO.md 5.5](05-mochila-greedy-vs-pd/CODIGO.md#55-reconstruir-qué-objetos-se-usaron-líneas-158164) |
| 65–70 | El código | Proyectar las ~10 líneas del "resumen para el pizarrón" y mostrar que son la tabla que llenaron. | [05, CODIGO.md resumen](05-mochila-greedy-vs-pd/CODIGO.md#resumen-para-el-pizarrón) |
| 70–75 | Cierre | La receta de 8 pasos. Dejar el 02 (suma M) como tarea: es la misma tabla con un solo cambio. | Sección 4 |

**Consejo:** la tabla se llena en el pizarrón **con los alumnos**, pidiendo que alguien calcule cada celda. Las primeras dos filas son fáciles y generan confianza. La fila de C es donde aparece la idea.

---

## 9. Dudas típicas de los alumnos

**"¿Cómo se me ocurre qué poner en la celda?"**
Lo que se pide en la letra, pero "restringido" a un problema más chico. Si piden "el mejor valor con N objetos y capacidad C", la celda es "el mejor valor con **i** objetos y capacidad **c**". Los parámetros que cambian al tomar una decisión (cuántos objetos quedan, cuánta capacidad queda) son las dimensiones de la tabla.

**"¿Por qué la tabla tiene una fila y una columna de más?"**
Para el caso base: la fila 0 es "sin objetos" y la columna 0 es "capacidad 0". Por eso es `N + 1` por `C + 1`.

**"¿Por qué se calculan celdas que no se usan?"**
En tabulación se calculan todas, porque no se sabe de antemano cuáles va a necesitar la respuesta. La memoización calcula solo las necesarias, pero usa recursión. Las dos tienen el mismo orden en el peor caso.

**"¿Memoización o tabulación?"**
Es la misma PD. La tabulación evita recursión (no hay riesgo de desbordar la pila) y permite ahorrar memoria quedándose con una fila (ejercicios 03 y 04). La memoización suele ser más fácil de escribir partiendo de la recursión.

**"¿Por qué no sirve el greedy?"**
Porque elige una sola opción mirando solo el objeto actual, y no puede deshacerla. La PD prueba las dos opciones en cada celda. Ver el [ejercicio 05](05-mochila-greedy-vs-pd/Letra.md#paso-b6-la-celda-donde-la-pd-corrige-al-greedy).

**"¿PD siempre es mejor que greedy?"**
No. Cuando el greedy es correcto (mochila fraccionaria, plataformas, máximo número), es más simple y más rápido. PD es para cuando hay que comparar opciones que el greedy no puede evaluar.

**"¿Cómo sé qué objetos usé, si la tabla solo tiene números?"**
Se reconstruye hacia atrás: si una celda es distinta de la de arriba, el objeto de esa fila se usó. Ver el [paso 7 del 01](01-mochila-01-ejemplo/Letra.md#paso-7-reconstruir-qué-objetos-se-usaron).

---

## 10. Glosario

| Término | Significado |
|---|---|
| Subproblema / estado | Una versión más chica del problema, identificada por sus parámetros, como (i, c) |
| Recurrencia | La fórmula que calcula un subproblema a partir de otros más chicos |
| Caso base | Un subproblema que se resuelve sin recurrencia |
| Memoización (*top-down*) | Recursión que guarda cada resultado la primera vez que lo calcula |
| Tabulación (*bottom-up*) | Llenar una tabla iterativamente desde los casos base |
| Subestructura óptima | El óptimo grande se arma con óptimos de subproblemas |
| Subproblemas superpuestos | Los mismos subproblemas aparecen muchas veces |
| Reconstrucción | Recuperar las decisiones (qué objetos) a partir de la tabla |
| Pseudo-polinomial | Polinomial en el *valor* de un número de la entrada (como C), no en su tamaño |
