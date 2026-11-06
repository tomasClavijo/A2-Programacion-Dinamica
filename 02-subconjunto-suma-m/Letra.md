## SUBCONJ. DE SUMA M	

Nombre de archivo: subconjunto_suma.cpp (originalmente ejercicio8.cpp, A2 2022)

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

### Idea de la solución (PD)

`tab[i][j]` = ¿se puede formar la suma `j` usando (con repetición) los primeros `i+1` elementos?

- Caso base: `tab[0][j] = (j % K0 == 0)`.
- `tab[i][j] = tab[i-1][j]` si `j < Ki` (no puedo usar `Ki`).
- `tab[i][j] = tab[i-1][j] || tab[i][j-Ki]` si no (no lo uso, o lo uso y **me quedo en la misma fila** porque se puede repetir).

Se arma la tabla una sola vez hasta la cota `M` y cada caso `Mi` se responde en O(1) leyendo `tab[N-1][Mi]`. Total O(N·M) en tiempo y espacio.
