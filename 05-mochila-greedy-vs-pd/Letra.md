## Mochila: greedy vs programación dinámica

Nombre de archivo: `mochila_greedy_vs_pd.cpp`

> Une los dos materiales de mochila: [`01-mochila-01-ejemplo`](../01-mochila-01-ejemplo/Letra.md) de este repo (PD) y [`02-mochila`](https://github.com/tomasClavijo/greedy/tree/main/02-mochila) del repo de [greedy](https://github.com/tomasClavijo/greedy). Usa las mismas instancias que el de greedy para poder comparar.

### Letra

Se tiene una mochila de capacidad `C` y `N` objetos, cada uno con un peso y un valor. Se quiere maximizar el valor total sin pasarse de la capacidad.

Se pide:

a) **Greedy 0/1.** Resolver la mochila 0/1 (cada objeto se lleva entero o no se lleva) con un algoritmo greedy que priorice el mejor **ratio** valor/peso.

b) **PD 0/1.** Resolver la misma mochila 0/1 con **programación dinámica**, indicando además qué objetos se llevan.

c) **Greedy fraccionario.** Resolver la mochila **fraccionaria** (se puede llevar una parte de un objeto, y su valor es proporcional) con el mismo greedy por ratio.

d) Mostrar una instancia en la que (a) no da el óptimo, y explicar por qué (c) siempre lo da.

### Input

La primera línea tiene `N` y `C`. Las siguientes `N` líneas tienen el peso y el valor de cada objeto (enteros positivos).

### Salida

```
greedy 0/1: <valor> [<objetos usados>]
pd 0/1: <valor> [<objetos usados>]
greedy fraccionaria: <valor con 2 decimales>
```

Los objetos se numeran del 1 al `N` según el orden de la entrada y se listan de menor a mayor. Si hay empate de ratio, el greedy prefiere el que aparece primero en la entrada.

### Restricciones

- Greedy (a) y (c): O(N log N).
- PD (b): O(N · C) en tiempo y espacio.

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
greedy 0/1: 12 [1 2]
pd 0/1: 15 [1 3]
greedy fraccionaria: 19.43
```

---

## Explicación paso a paso

El hilo de todo el ejercicio es una sola instancia, la del ejemplo. La llamamos **"capacidad 7"**:

| Objeto | Peso | Valor | Ratio |
|---|---|---|---|
| A (1) | 1 | 6 | 6 |
| B (2) | 2 | 6 | 3 |
| C (3) | 5 | 9 | 1,8 |
| D (4) | 7 | 13 | 1,86 |

Se resuelve primero con greedy (parte a), después se ve por qué falla, después se resuelve con PD (parte b) y se mira **en qué celda de la tabla la PD corrige el error del greedy**. Al final se ve la variante fraccionaria (parte c), donde el greedy sí es óptimo.

---

## Parte A: greedy 0/1 por ratio

### Paso A1: la idea

Un greedy arma la solución de a un paso: en cada paso toma lo que *parece* mejor según un criterio local y **nunca vuelve atrás**.

El criterio acá es el ratio `valor / peso`, o sea, cuánto rinde cada kilo del objeto. Es el criterio que se le ocurre a casi todo el mundo. En el [repo de greedy](https://github.com/tomasClavijo/greedy/blob/main/02-mochila/EXPLICACION.md) se ve que los otros dos criterios (mayor valor y menor peso) también fallan.

### Paso A2: el algoritmo

1. Ordenar los objetos de mayor a menor ratio.
2. Recorrerlos en ese orden. Si el objeto **entra** en lo que queda, meterlo entero. Si **no entra**, saltearlo y **seguir** con el siguiente, porque uno peor puede entrar.
3. Cuando se terminan los objetos, devolver el total.

Ordenar una vez y recorrer da exactamente las mismas elecciones que la versión del repo de greedy, que en cada vuelta busca "el mejor ratio entre los que todavía entran". La razón es que la capacidad libre solo baja: un objeto que no entra ahora no va a entrar nunca más, así que saltearlo es definitivo en las dos versiones. La diferencia está en el costo: O(N log N) contra O(N²).

### Paso A3: la traza con capacidad 7

Orden por ratio: A (6), B (3), D (1,86), C (1,8).

| Paso | Objeto | ¿Entra? | Capacidad libre | Valor acumulado |
|---|---|---|---|---|
| 0 | | | 7 | 0 |
| 1 | A (peso 1) | Sí | 7 − 1 = 6 | 6 |
| 2 | B (peso 2) | Sí | 6 − 2 = 4 | 12 |
| 3 | D (peso 7) | No (7 > 4) | 4 | 12 |
| 4 | C (peso 5) | No (5 > 4) | 4 | 12 |

Resultado: **12**, llevando A y B. Sobran 4 kilos que no se usan.

### Paso A4: ¿es el óptimo? Buscar un contraejemplo

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

El óptimo es **15** y el greedy da 12. Con una instancia alcanza para demostrar que el criterio **no** es correcto.

### Paso A5: dónde se equivoca

Tomar A no estaba mal, porque A + C es óptimo. El error es el **paso 2**: con 6 kilos libres el greedy toma B porque rinde más por kilo que C. Pero B gasta justo los kilos que C necesitaba (después de B quedan 4, y C pesa 5).

Lo que el ratio no ve es **qué objetos dejan de entrar** por elegir uno. En la 0/1 los kilos que sobran valen 0, así que un objeto que rinde mucho por kilo puede dejar una mochila peor.

El caso más claro para recordar es el de **capacidad 10** (prueba `03`): X (6, 60), Y (5, 45), Z (5, 40). El ratio toma X (rinde 10 por kilo) y quedan 4 kilos donde no entra nada, total 60. El óptimo es Y + Z = 85, que llena la mochila y **no usa el objeto de mejor ratio**.

### Paso A6: el greedy puede quedar arbitrariamente lejos

Prueba `06`, con capacidad 100: objetos (1, 2) y (100, 100).

- El ratio prefiere el primero (2 contra 1) y después el segundo ya no entra. Total **2**.
- El óptimo es el segundo solo. Total **100**.

Cambiando 100 por cualquier `C`, el greedy da 2 y el óptimo da `C`. No hay ninguna garantía de "estar cerca".

---

## Parte B: programación dinámica 0/1

### Paso B1: qué tiene que hacer distinto

El greedy falló porque decidió B **sin mirar qué pasaba después**. La alternativa honesta es probar las dos opciones (poner o no poner) para cada objeto. Con fuerza bruta eso es O(2ᴺ). Pero muchas combinaciones llegan a la misma situación ("ya decidí los primeros `i` objetos y me quedan `c` kilos libres"), y esa situación tiene siempre la misma mejor continuación. Esa repetición es lo que aprovecha la PD.

### Paso B2: definir el subproblema

> `tab[i][c]` = máximo valor que se puede obtener usando **solo los objetos 1..i** con una mochila de capacidad **c**.

La respuesta es `tab[N][C]`.

### Paso B3: casos base

`tab[0][c] = 0`, porque sin objetos el valor es 0.

### Paso B4: recurrencia

Para el objeto `i` con capacidad `c`:

- Si **no entra** (`peso_i > c`): `tab[i][c] = tab[i-1][c]`.
- Si **entra**: `tab[i][c] = max( tab[i-1][c],  valor_i + tab[i-1][c - peso_i] )`. La primera opción es no ponerlo y la segunda es ponerlo.

Al ponerlo se baja a la fila `i-1`, porque en la 0/1 no se puede repetir.

**Esta es la diferencia con el greedy:** el greedy elige una de las dos opciones mirando solo el objeto. La PD calcula **las dos** y se queda con la mejor, y lo hace para **todas** las capacidades posibles, así que más adelante puede usar cualquiera de esos resultados.

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
- `tab[2][7] = max(tab[1][7], 6 + tab[1][5]) = max(6, 12) = 12`. Hasta acá la PD coincide con el greedy: con A y B solamente, lo mejor es llevar los dos.
- `tab[3][6] = max(tab[2][6], 9 + tab[2][1]) = max(12, 9 + 6) = 15`: C + A.
- `tab[3][7] = max(tab[2][7], 9 + tab[2][2]) = max(12, 9 + 6) = 15`.
- `tab[4][7] = max(tab[3][7], 13 + tab[3][0]) = max(15, 13) = 15`: D solo vale menos.

Respuesta: `tab[4][7] = 15`.

### Paso B6: la celda donde la PD corrige al greedy

Hay que mirar `tab[3][7]`, el momento en que aparece C con la mochila vacía de 7 kilos:

| Opción | Cuenta | Valor | Qué significa |
|---|---|---|---|
| No poner C | `tab[2][7]` | 12 | Lo mejor con A y B: A + B. **Es la solución del greedy.** |
| Poner C | `9 + tab[2][2]` | 9 + 6 = **15** | C, y con los 2 kilos que quedan, lo mejor de A y B: uno solo. |

El greedy ya había metido A **y** B cuando llegó a C, y no podía sacar nada. La PD guarda `tab[2][2] = 6` ("lo mejor con A y B en solo 2 kilos"), y eso le permite evaluar "C + uno de los dos". En la práctica, es como si la PD **se arrepintiera de B**, algo que el greedy no puede hacer.

Lo mismo pasa con capacidad 10: `tab[3][10] = max(tab[2][10], 40 + tab[2][5]) = max(60, 40 + 45) = 85`. La opción "no poner Z" vale 60 (lo que hace el greedy, X solo). La opción "poner Z" usa `tab[2][5] = 45` (Y en 5 kilos) y descarta X.

### Paso B7: reconstruir qué objetos se llevan

Se recorre la tabla desde `tab[4][7]` hacia arriba. Si `tab[i][c] != tab[i-1][c]`, el objeto `i` se usó y se descuenta su peso.

| (i, c) | tab[i][c] | tab[i-1][c] | ¿Se usó? | c queda |
|---|---|---|---|---|
| (4, 7) | 15 | 15 | No (D) | 7 |
| (3, 7) | 15 | 12 | **Sí (C)** | 7 − 5 = 2 |
| (2, 2) | 6  | 6  | No (B) | 2 |
| (1, 2) | 6  | 0  | **Sí (A)** | 2 − 1 = 1 |

Resultado: A + C = **15**, la salida `pd 0/1: 15 [1 3]`.

B + C también vale 15. La reconstrucción encuentra *una* solución óptima. Cuál encuentra depende de cómo se resuelven los empates: con `!=`, ante igualdad prefiere "no usar el objeto".

### Paso B8: la PD no depende del orden de la entrada

Las pruebas `04` y `05` son los mismos objetos en distinto orden (A y D empatan en ratio):

| Prueba | Greedy 0/1 | PD 0/1 |
|---|---|---|
| `04` (A antes que D) | 30 | 30 |
| `05` (D antes que A) | **28** | 30 |

El greedy cambia según quién gane el empate. La PD considera todas las combinaciones, así que el **valor** óptimo es siempre el mismo (solo puede cambiar *cuál* de las soluciones óptimas reconstruye). Si un greedy cambia de resultado al reordenar la entrada, es una señal de que el criterio no captura todo lo que importa.

---

## Parte C: greedy fraccionario

### Paso C1: qué cambia

Ahora los objetos se pueden cortar (arena, harina, combustible). Si de un objeto de peso `p` y valor `v` se llevan `x` kilos, se suma `v · x / p`.

El algoritmo es el mismo greedy por ratio, con una diferencia: cuando el siguiente objeto no entra entero, **se lleva el pedazo que entra** y la mochila queda llena.

### Paso C2: la traza con capacidad 7

Orden por ratio: A (6), B (3), D (1,86), C (1,8).

| Paso | Objeto | Lleva | Capacidad libre | Valor acumulado |
|---|---|---|---|---|
| 1 | A | entero (1 kg) | 6 | 6 |
| 2 | B | entero (2 kg) | 4 | 12 |
| 3 | D | 4 de sus 7 kg: 13 · 4/7 ≈ 7,43 | 0 | **19,43** |

El hueco de 4 kilos que en la 0/1 quedaba vacío ahora se llena con un pedazo de D.

### Paso C3: por qué acá el greedy sí es óptimo (intercambio)

Supongamos una solución óptima que **no** sigue el orden de ratio. Entonces lleva algo de un objeto peor (ratio `r₂`) mientras queda sin llevar una parte de un objeto mejor (ratio `r₁ > r₂`).

Se cambia **un kilo** del peor por **un kilo** del mejor. El peso total no cambia y el valor sube `r₁ − r₂ > 0`. Eso contradice que fuera óptima.

Por lo tanto, en una solución óptima no puede pasar eso: los objetos se llenan en orden de ratio, que es lo que hace el greedy. Además, nunca sobra capacidad mientras queden objetos, porque siempre se puede cortar un pedazo.

### Paso C4: por qué el mismo intercambio falla en la 0/1

En la 0/1 no se puede cambiar "un kilo": solo se cambian objetos enteros, y los pesos no tienen por qué calzar. En capacidad 10, la solución óptima Y + Z lleva Z (ratio 8) mientras X (ratio 10) queda afuera. Pero:

- Cambiar Z por X da Y + X, que pesa 11 y no entra.
- Sacar Y y Z para meter X entra, pero vale 60 < 85.

No hay intercambio posible que mejore, y por eso el argumento que demuestra el greedy fraccionario no sirve para la 0/1.

| | Mochila 0/1 | Mochila fraccionaria |
|---|---|---|
| ¿Se puede cortar? | No | Sí |
| ¿Puede sobrar capacidad con objetos sin llevar? | Sí | No |
| ¿El intercambio de a un kilo siempre se puede hacer? | No | Sí |
| ¿Greedy por ratio es óptimo? | **No** | **Sí** |
| Algoritmo correcto | **PD**, O(N · C) | **Greedy**, O(N log N) |

### Paso C5: la fraccionaria como cota

La fraccionaria puede hacer todo lo que hace la 0/1 y además cortar, así que su óptimo es **mayor o igual** que el óptimo 0/1:

| Prueba | Greedy 0/1 | PD 0/1 (óptimo) | Fraccionaria (cota) |
|---|---|---|---|
| `01` capacidad 13 | 45 | 45 | 49,00 |
| `02` capacidad 7 | 12 | 15 | 19,43 |
| `03` capacidad 10 | 60 | 85 | 96,00 |
| `04` capacidad 9, A antes | 30 | 30 | 38,75 |
| `05` capacidad 9, D antes | 28 | 30 | 38,75 |
| `06` capacidad 100 | 2 | 100 | 101,00 |

En todas las filas se cumple `greedy 0/1 ≤ PD 0/1 ≤ fraccionaria`. El greedy 0/1 da una solución factible (cota inferior) y la fraccionaria da una cota superior. Las dos son baratas, y es justo lo que usa *branch and bound* para podar en backtracking.

En `04` y `05` la fraccionaria lleva un pedazo de C (6/8 de 25 = 18,75), que en la 0/1 no entraba: 20 + 18,75 = 38,75. Por eso el empate entre A y D no le importa.

---

## Parte D: el código

El programa resuelve las tres versiones sobre la misma entrada. Sigue el estilo de los obligatorios: arrays con `new`/`delete`, la tabla como `int**` armada con `initMatriz`, y el ordenamiento programado a mano con `mergeSort`.

### Comparar ratios sin dividir

```cpp
bool mejorRatio(Objeto a, Objeto b) {
    return (long long)a.valor * b.peso > (long long)b.valor * a.peso;
}
```

Se quiere comparar `a.valor / a.peso > b.valor / b.peso`. Con enteros la división trunca (5 / 2 = 2). Como los pesos son positivos, se pasan multiplicando y la cuenta queda exacta. El `long long` evita overflow si los valores son grandes.

### Ordenar por ratio con mergeSort

Es el mismo `mergeSort` del ejercicio 8 de 2023 (plataformas), pero ordena `Objeto` de **mayor a menor** ratio. En `merge`:

```cpp
// Solo paso el de la derecha si es ESTRICTAMENTE mejor (estabilidad).
if (mejorRatio(R[j], L[i])) {
    arr[k] = R[j];  j++;
} else {
    arr[k] = L[i];  i++;
}
```

Ante un empate pasa el de la izquierda, que es el que estaba antes en la entrada. Eso hace que el ordenamiento sea **estable**, y es lo que necesita la letra ("ante un empate gana el que aparece primero"). Con `>=` se perdería esa propiedad.

`ordenarPorRatio` ordena una **copia**, porque la PD necesita los objetos en el orden original.

### Qué objetos se usan

Se guardan en un array `bool* usados` de tamaño `N + 1`, indexado por el número de objeto (1..N). Se crea con `new bool[N + 1]()` para que arranque en `false`. Al imprimir se recorre del 1 al `N`, así que los números salen ordenados sin hacer nada más.

### Greedy 0/1

```cpp
for (int i = 0; i < N; i++) {
    if (orden[i].peso <= libre) {          // entra: lo meto entero
        libre -= orden[i].peso;
        total += orden[i].valor;
        usados[orden[i].numero] = true;
    }
    // no entra: lo salteo y sigo con el siguiente
}
```

Un solo recorrido y ninguna vuelta atrás. El error común es poner un `break` cuando un objeto no entra: hay que **seguir**, porque uno más liviano puede entrar.

### PD 0/1

```cpp
int ** tab = initMatriz(N + 1, capacidad + 1);   // todo en 0: fila 0 = caso base
for (int i = 1; i <= N; i++) {
    int peso = objetos[i - 1].peso;
    int valor = objetos[i - 1].valor;
    for (int c = 0; c <= capacidad; c++) {
        tab[i][c] = tab[i - 1][c];                                  // no ponerlo
        if (peso <= c)
            tab[i][c] = max(tab[i][c], valor + tab[i - 1][c - peso]);   // ponerlo
    }
}
```

Es la recurrencia del paso B4 tal cual, con tabulación (*bottom-up*), y tiene la misma forma que el [ejercicio 01](../01-mochila-01-ejemplo/mochila01.cpp). La fila `i` corresponde a `objetos[i - 1]` porque el array arranca en 0 y la tabla reserva la fila 0 para el caso base. Se guarda la tabla completa para poder reconstruir (paso B7). El `mochilaOptima` del repo de greedy usa una sola fila recorrida de mayor a menor (O(C) de espacio, como el [ejercicio 03](../03-archivos-mochila-2d/Letra.md)), y con eso da el valor pero no los objetos.

### Greedy fraccionario

```cpp
if (orden[i].peso <= libre) {                  // entra entero
    libre -= orden[i].peso;
    total += orden[i].valor;
} else {                                       // entra un pedazo
    total += (double)orden[i].valor * libre / orden[i].peso;
    libre = 0;
}
```

La única diferencia con el greedy 0/1 es la rama `else`: en lugar de saltear el objeto, se lleva la fracción `libre / peso`. El `(double)` va antes de multiplicar y dividir, porque si no la división sería entera.

---

## Resumen: cuándo usar cada uno

| | Greedy 0/1 | PD 0/1 | Greedy fraccionario |
|---|---|---|---|
| ¿Da el óptimo? | No, puede quedar arbitrariamente lejos | Sí | Sí (para la fraccionaria) |
| Tiempo | O(N log N) | O(N · C) | O(N log N) |
| Espacio | O(N) | O(N · C), u O(C) sin reconstrucción | O(N) |
| Depende del orden de la entrada | Sí, en los empates | No (el valor) | No (el valor) |
| Uso típico | Aproximación rápida, cota inferior | El problema 0/1 de verdad | El problema fraccionario, cota superior de la 0/1 |

Una advertencia sobre la PD: O(N · C) es **pseudo-polinomial**. Depende del *valor* de `C`, no de cuántos dígitos tiene. Con `C = 10⁹` la tabla no entra en memoria, y ahí se vuelve a usar backtracking con las cotas del paso C5.

## Para pensar

1. ¿En qué tipo de instancias el greedy por ratio sí da el óptimo de la 0/1?
2. Si todos los objetos pesan lo mismo, ¿qué criterio greedy es óptimo para la 0/1?
3. ¿Se puede resolver la mochila 0/1 con PD si los pesos son números reales?
4. Modificar la PD para que, ante un empate, prefiera la solución con **menos objetos**.
5. ¿Por qué la PD necesita la tabla completa para reconstruir, y no le alcanza con una fila?

<details>
<summary>Respuestas</summary>

1. Cuando el greedy toma los objetos en orden de ratio sin saltear ninguno y llena la mochila justo. En ese caso su valor coincide con la cota fraccionaria, y nada puede superarla. Por ejemplo, con los objetos de la prueba `01` y capacidad 11: el greedy toma B y C, que pesan exactamente 11, y obtiene 45, que es también la cota fraccionaria.
2. Mayor valor. Si todos pesan lo mismo, entra la misma cantidad de objetos con cualquier elección, así que no hay huecos distintos. Conviene llevar los más valiosos, y en ese caso el mayor ratio coincide con el mayor valor.
3. No directamente, porque la tabla se indexa por capacidad entera. Si los pesos tienen pocos decimales se pueden escalar a enteros (multiplicar por 10ᵏ). Si no, la alternativa es backtracking con poda, o una PD sobre valores en lugar de pesos cuando los valores son enteros chicos.
4. Guardar en cada celda un par (valor, cantidad de objetos) y comparar primero por valor (mayor) y después por cantidad (menor).
5. Para saber si el objeto `i` se usó hay que comparar `tab[i][c]` con `tab[i-1][c]`. Si se sobrescribe la fila, ese valor anterior se pierde. Con una sola fila se puede calcular el valor, pero para los objetos habría que guardar aparte las decisiones (una matriz de `bool` de N × C, que ocupa lo mismo).

</details>
