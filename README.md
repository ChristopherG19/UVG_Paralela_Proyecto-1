# Proyecto: Screensaver de Ondas en Movimiento

Este proyecto es parte del curso de Computación Paralela y Distribuida de la Universidad del Valle de Guatemala. Consiste en un programa que simula ondas en movimiento en una ventana de visualización utilizando la biblioteca SDL (Simple DirectMedia Layer). El programa permite configurar y renderizar múltiples ondas en movimiento de manera simultánea.

## Integrantes del equipo

- Maria Isabel Solano (20504)
- Andrea de Lourdes Lam (20102)
- Christopher García (20541)

## Descripción del Programa

El programa crea un efecto de pantalla de descanso (screensaver) que muestra ondas en movimiento en una ventana. Cada onda se representa mediante una serie de puntos que forman una forma sinusoidal en constante cambio. Las ondas se crean con diferentes parámetros, como amplitud, frecuencia y velocidad, lo que da lugar a un efecto visual interesante.

## Características

- Configuración de la cantidad de ondas a renderizar.
- Generación aleatoria de parámetros para cada onda.
- Representación de las ondas en tiempo real.
- Cálculo y visualización de los FPS (cuadros por segundo).

## Requisitos

Para compilar y ejecutar este programa, necesitarás tener instalada la biblioteca SDL 2. Asegúrate de tenerla correctamente instalada en tu sistema antes de compilar el código.

## Compilación y Ejecución

Para compilar el programa, puedes utilizar el siguiente comando:

```bash
g++ screensaver.cpp -o screensaver -lSDL2

Instrucciones ejemplo para secuencial:
	g++ -o Secuencial2 SecuencialV2.cpp -lSDL2
	./Secuencialv2 <número de elementos>

Instrucciones ejemplo para paralelo:
g++ -o par ParalelaV1.cpp -lSDL2 -fopenmp
./par <num_elementos>



