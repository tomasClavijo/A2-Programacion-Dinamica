## Selección de archivos (mochila con dos restricciones)

Nombre de archivo: archivos.cpp (originalmente `Ejercicio 7/ejercicio7.cpp`, A2 2023)

> El repo de 2023 no incluye la letra original. Esta letra se reconstruyó a partir del código y de los casos de prueba.

### Letra

Se tienen `N` archivos. Cada archivo tiene un tamaño, una cantidad de líneas de código y un puntaje. Se desea elegir un subconjunto de archivos (cada uno a lo sumo una vez) que maximice el puntaje total, sin superar un tamaño máximo `S` ni una cantidad máxima de líneas `L`.

### Input

La primera línea contiene `N`, la cantidad de archivos. La segunda contiene `S`, el tamaño máximo. La tercera contiene `L`, la cantidad máxima de líneas. Las siguientes `N` líneas contienen, para cada archivo, `tamaño líneas puntaje` separados por espacio.

### Salida

Una línea con el puntaje máximo alcanzable.

### Restricciones

Resolver con programación dinámica en O(N·S·L) temporal.

### Ejemplos de entrada y salida

#### Entrada
10  
50  
1000  
16 665 1  
6 501 8  
25 904 7  
18 591 5  
7 683 6  
22 88 1  
24 497 9  
22 565 7  
31 414 9  
49 389 2  

#### Salida
17  

### Idea de la solución (PD)

Es la mochila 0/1 con **dos** capacidades. `tab[l][s]` = máximo puntaje con a lo sumo `l` líneas y tamaño `s`, usando los archivos procesados hasta el momento.

Por cada archivo `(t, l_i, p)`:

```
para l desde L hasta l_i (descendente):
  para s desde S hasta t (descendente):
    tab[l][s] = max(tab[l][s], tab[l - l_i][s - t] + p)
```

Recorrer las capacidades **de mayor a menor** es lo que permite usar una única matriz (en lugar de una por archivo) sin que un archivo se use dos veces.

Orden: O(N·S·L) temporal, O(S·L) espacial.
