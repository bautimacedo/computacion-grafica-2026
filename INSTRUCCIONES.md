# Cómo compilar y ejecutar

## 1. Pararse en esta carpeta (la que contiene este archivo y el `Makefile`)

```sh
cd rrt-esqueleto
```

## 2. Compilar

```sh
make
```

Esto genera el binario `bin/rrt`. Requiere `g++`, `make` y GLM (en Windows,
terminal MinGW64 de MSYS2). No hace falta OpenGL ni GLFW.

## 3. Ejecutar una escena

```sh
./bin/rrt <número_de_escena> -r 1280 720 -ss 3
```

Por ejemplo, para la Escena 4 (esferas de vidrio y de agua):

```sh
./bin/rrt 4 -r 1280 720 -ss 3
```

- `-r 1280 720`: resolución final de entrega (1280x720 píxeles).
- `-ss 3`: supermuestreo 3x3 rayos por píxel (antialiasing). Sin esta opción
  usa 1 (más rápido pero con bordes dentados), útil para pruebas rápidas.
- La imagen se escribe como `escena-NN.png` en esta misma carpeta (usar `-o`
  para elegir otro nombre).
- `./bin/rrt -l` lista todas las escenas disponibles con su número y nombre.

## Escenas disponibles

| Nº | Escena |
|---|---|
| 0 | Taller 1: plano con tablero, dos esferas difusas, luz puntual |
| 11 | Spheres and Checkerboard (Whitted, 1980) — ejemplo del taller 2 |
| 1 | Luces y sombras de colores |
| 2 | Espejos paralelos |
| 3 | Pasillo de espejos |
| 4 | Esferas de vidrio y de agua |
| 5 | Banco de índices de refracción |
| 6 | Esfera maciza, hueca y burbuja |
| 7 | La piscina |
| 8 | Matrioshka de vidrio |

Las escenas 1 a 8 corresponden al Práctico 10 (una por cada concepto del
modelo de iluminación de Whitted: sombra por color, pérdida de energía en
reflejos, triángulos y espejos, refracción, índices de refracción, lentes
convergente/divergente, reflectancia de Fresnel según ángulo, y profundidad
de recursión).

Más detalle de la arquitectura del código y los pasos del taller en `LEEME.md`.
