# Proyecto 1 - Computación Paralela y Distribuida

Universidad del Valle de Guatemala

Diego Andrés Morales Aquino - 21762
Pablo Andrés Zamora Vasquez - 21780

El proyecto consiste en un screen saver con temática de múltiples pelotas oscilando en un péndulo simple, simulando la físcia y el movimiento de un péndulo simple como de proyectiles al realizarse el corte aleatorio de algunos péndulos.

## Compilación del proyecto

### Compilación en Windows

```
.\run.bat
```

### Compilación en Linux
```
g++ ./src/main.cpp -o ./build/Proyecto_1.exe -lSDL2 -fopenmp
```

### Ejecución del programa

## Valores default 
```
./build/Proyecto_1.exe 
```

## Añadir parámetros
```
./build/Proyecto_1.exe < N > < RADIUS > < PROB_INV_CUT > < INITIAL_LEN > < MAX_SAME_ROW > < LENGTH_INCREMENT >
```

* < N >: Número de péndulos a renderizar.
* < RADIUS >: Radio inicial de los péndulos.

* < PROB_INV_CUT >: Probabilidad inversa de corte de cuerda. (prob_corte = 1/prob_inv_cut)
* < INITIAL_LEN >: Longitud inicial de la cuerda.
* < MAX_SAME_ROW >: Máximo número de bolas en una misma fila.
* < LENGTH_INCREMENT >: Incremento de cuerda al cambiar de fila.