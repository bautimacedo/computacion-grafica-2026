# Plantilla estándar para armar una escena (parcial)

Copiar el bloque de abajo en un `.cpp` nuevo dentro de `src/escenas/`, reemplazar
los valores y los comentarios `// <<< ...` por los datos que te den en el
enunciado. Las firmas (nombres de parámetros, orden) son las reales del
proyecto — compila tal cual, solo cambiando números.

---

## 1. Esqueleto completo de una escena

```cpp
#include <memory>

#include <escenas.h>
#include <plane.h>
#include <sphere.h>
#include <triangle.h>
#include <checker.h>
#include <mirror.h>
#include <dielectric.h>

void build_scene_NN(Scene& scene, Camera& camera)
{
    // ---------- CÁMARA ----------
    // look_at(ojo, hacia_dónde_mira, arriba, fov_vertical_en_grados)
    camera.look_at(glm::vec3(0.0f, 2.0f, 6.0f),   // <<< posición del ojo
                    glm::vec3(0.0f, 1.0f, 0.0f),   // <<< punto al que mira (target)
                    glm::vec3(0.0f, 1.0f, 0.0f),   // arriba: casi siempre (0,1,0), no cambiar salvo que pidan cámara "de costado"/inclinada
                    45.0f);                        // <<< fov vertical en grados (típico 45-60)

    // ---------- FONDO ----------
    scene.set_background(glm::vec3(0.02f, 0.02f, 0.02f));  // <<< color de fondo (RGB 0..1). Casi negro por defecto
    // Alternativa (fondo degradado horizonte/cenit):
    // scene.set_background(glm::vec3(horizonR,horizonG,horizonB), glm::vec3(cenitR,cenitG,cenitB));

    // ---------- MATERIALES ----------
    // Difuso (mate, sin brillo): ka ambiente, kd difuso, ks especular (0 = sin brillo)
    auto rojo = std::make_shared<Diffuse>();
    rojo->ka = glm::vec3(0.02f, 0.0f, 0.0f);   // <<< fracción de luz ambiente reflejada (suele ser ~kd/20..40)
    rojo->kd = glm::vec3(0.8f, 0.0f, 0.0f);    // <<< color/fracción difusa (esto es "el color" del objeto)
    rojo->ks = glm::vec3(0.0f);                 // sin brillo especular

    // Difuso CON brillo (plástico brillante): ks>0 y shininess (exponente de Phong)
    auto plastico = std::make_shared<Diffuse>();
    plastico->ka = glm::vec3(0.02f);
    plastico->kd = glm::vec3(0.1f, 0.3f, 0.8f); // <<< color del objeto
    plastico->ks = glm::vec3(0.3f);             // <<< intensidad del brillo (no la concentración)
    plastico->shininess = 30.0f;                // <<< qué tan chico/concentrado es el brillo (alto = puntito chico)

    // Tablero de ajedrez (piso típico): hereda de Diffuse, usa ka/kd para casillas
    // "pares" y ka2/kd2 para las "impares"
    auto tablero = std::make_shared<Checker>();
    tablero->ka  = glm::vec3(0.02f, 0.02f, 0.0f);   // <<< casilla color 1 (ambiente)
    tablero->kd  = glm::vec3(0.8f, 0.8f, 0.0f);     // <<< casilla color 1 (difuso) -- ej. amarillo
    tablero->ka2 = glm::vec3(0.02f, 0.0f, 0.0f);    // <<< casilla color 2 (ambiente)
    tablero->kd2 = glm::vec3(0.8f, 0.0f, 0.0f);     // <<< casilla color 2 (difuso) -- ej. rojo
    tablero->ks  = glm::vec3(0.0f);
    tablero->size = 1.0f;                            // <<< lado de cada casilla

    // Espejo: Phong (para el brillo de luces puntuales) + 1 rayo reflejado de peso kr
    auto espejo = std::make_shared<Mirror>();
    espejo->kr = 0.8f;                 // <<< cuánto pesa el reflejo (0..1, cercano a 1 = espejo casi perfecto)
    espejo->ks = glm::vec3(1.0f);      // brillo blanco típico en un espejo
    espejo->shininess = 100.0f;

    // Dieléctrico (vidrio/agua): refracción + reflexión según Fresnel/Schlick.
    // ka y kd no se usan (el constructor ya los pone en 0); sólo importan ior y transparency
    auto vidrio = std::make_shared<Dielectric>();
    vidrio->ior = 1.5f;              // <<< índice de refracción (vidrio 1.5, agua 1.33, aire 1.0)
    vidrio->transparency = 0.8f;     // <<< cuánta luz deja pasar un rayo de SOMBRA al atravesarlo (0=opaco,1=no hace sombra)

    // ---------- OBJETOS ----------
    // Plano(punto_cualquiera_del_plano, normal, material)
    scene.add_object(std::make_shared<Plane>(
        glm::vec3(0.0f, 0.0f, 0.0f),      // <<< punto del plano (piso: y=0 -> (0,0,0))
        glm::vec3(0.0f, 1.0f, 0.0f),      // <<< normal (piso: apunta para arriba)
        tablero));

    // Sphere(centro, radio, material)
    scene.add_object(std::make_shared<Sphere>(
        glm::vec3(0.0f, 1.0f, 0.0f),      // <<< centro
        1.0f,                              // <<< radio
        plastico));

    // Triangle(a, b, c, material) -- 3 vértices en sentido que dé la normal hacia afuera (b-a)x(c-a)
    scene.add_object(std::make_shared<Triangle>(
        glm::vec3(-1.0f, 0.0f, 0.0f),     // <<< vértice a
        glm::vec3( 1.0f, 0.0f, 0.0f),     // <<< vértice b
        glm::vec3( 0.0f, 2.0f, 0.0f),     // <<< vértice c
        rojo));

    // ---------- LUCES ----------
    // Light(posición, color/intensidad, atenuación=(c0,c1,c2))  -- atenuación es opcional
    scene.add_light(Light(
        glm::vec3(0.0f, 5.0f, 3.0f),       // <<< posición de la luz
        glm::vec3(2.4f, 2.4f, 2.4f),       // <<< color/intensidad (blanca; para color puro ej. (2.4,0,0) = roja)
        glm::vec3(1.0f, 0.12f, 0.02f)));   // <<< atenuación (c0,c1,c2) en f_att = 1/(c0+c1·d+c2·d²). Si no la dan: dejar (1,0,0) = sin atenuación

    // Más luces: repetir add_light las veces que haga falta
}
```

---

## 2. Dar de alta la escena (para que aparezca al correr el programa)

Dos archivos a tocar, siempre los mismos:

**`src/escenas/escenas.h`** — agregar la declaración (misma firma siempre):
```cpp
void build_scene_NN(Scene& scene, Camera& camera);
```

**`src/escenas/escenas.cpp`** — agregar una fila a la tabla `kScenes`:
```cpp
{ NN, "Descripción corta de la escena", build_scene_NN },
```

Después compilar con `make` y correr el binario pasándole el número de escena
(ver `README`/Makefile del proyecto si no lo recuerdo de memoria el comando
exacto de ejecución).

---

## 3. Cómo compilar y ejecutar

Parado en la carpeta `rrt-esqueleto/` (donde está el `Makefile`):

```sh
make                              # compila bin/rrt
./bin/rrt NN -r 1280 720 -ss 3    # renderiza la escena NN a resolución de entrega
./bin/rrt -l                      # lista las escenas disponibles con su número
```

`-r 1280 720` = resolución final (1280x720). `-ss 3` = supermuestreo 3x3 (sin
esto, usa 1 rayo por píxel, más rápido para probar mientras armás la escena).
La imagen sale como `escena-NN.png` en esa misma carpeta.

## 4. Cómo armar una pared/panel rectangular con triángulos (ubicarse en 3D)

El truco para no perderte en 3D: **una pared siempre tiene una de las 3
coordenadas FIJA** (la que define en qué plano vive) **y las otras 2 VARÍAN**
(ancho y alto). Si identificás cuál es la fija, el problema se reduce a
dibujar un rectángulo en 2D con las otras 2 coordenadas — exactamente como en
una hoja cuadriculada — y después le "pegás" la coordenada fija a los 4
puntos.

Pasos:
1. Mirá el enunciado: ¿qué coordenada es la misma para los 4 vértices?
   (ej. "pared en x=3" -> x fija, y/z varían. "panel en z=-1" -> z fija, x/y varían).
2. Dibujá en una hoja un rectángulo 2D con esas 2 coordenadas que varían (como
   si fuera una ventana vista de frente), y anotá las 4 esquinas en orden,
   dando la vuelta al perímetro (nunca salteando a la diagonal opuesta).
3. Armá los triángulos `(a,b,c)` y `(a,c,d)` con esas 4 esquinas, agregándoles
   la coordenada fija en cada una.
4. Si la normal te queda para el lado que no querías, invertís el 2º y 3er
   vértice de cada triángulo: `(a,c,b)` y `(a,d,c)`.

### Ejemplo resuelto 1 (del Práctico 10, Escena 3 "Pasillo de espejos")

Enunciado real: *"Paredes: rectángulos en x=-3 y en x=3, con y de 0 a 5 y z de
-12 a 14. Con a=(x,0,14), b=(x,0,-12), c=(x,5,-12), d=(x,5,14), los triángulos
son a-b-c y a-c-d."*

Acá **x es la coordenada fija** (define el plano vertical de la pared); **y**
(altura, 0 a 5) y **z** (profundidad, -12 a 14) son las que varían. En 2D
(ejes y-z) el rectángulo tiene estas 4 esquinas, recorridas en orden:

```
    z=-12         z=14
y=5  c ----------- d
     |             |
y=0  b ----------- a
```

(a arriba a la derecha en z, dando la vuelta: a→b baja, b→c cruza en z=-12,
c→d sube, d→a cierra en z=14 — perímetro válido, sin diagonales).

```cpp
const float x = -3.0f;   // o 3.0f, para la otra pared
glm::vec3 a(x, 0.0f, 14.0f);
glm::vec3 b(x, 0.0f, -12.0f);
glm::vec3 c(x, 5.0f, -12.0f);
glm::vec3 d(x, 5.0f, 14.0f);
scene.add_object(std::make_shared<Triangle>(a, b, c, espejo));
scene.add_object(std::make_shared<Triangle>(a, c, d, espejo));
```

### Ejemplo resuelto 2 (panel de vidrio de 2 caras, Escena 21)

Acá hay DOS paredes paralelas (el grosor del vidrio): cada una tiene **z
fija** (una z por cara) y **x/y variables**. Para que el dieléctrico detecte
bien si el rayo entra o sale del vidrio, cada cara necesita la normal
apuntando **hacia afuera del vidrio** (ver sección de Recordatorios). Por eso
la cara trasera usa el orden de vértices invertido respecto de la delantera:

```cpp
// Cara delantera, z=-1: normal tiene que apuntar hacia +z (afuera, hacia la cámara)
glm::vec3 a(-1.5f, 0.0f, -1.0f), b(1.5f, 0.0f, -1.0f), c(1.5f, 3.0f, -1.0f), d(-1.5f, 3.0f, -1.0f);
scene.add_object(std::make_shared<Triangle>(a, b, c, vidrio));
scene.add_object(std::make_shared<Triangle>(a, c, d, vidrio));

// Cara trasera, z=-1.3: normal tiene que apuntar hacia -z (afuera, lejos de la cámara)
// Mismas 4 esquinas (misma forma), pero 2º y 3er vértice invertidos en cada triángulo
glm::vec3 e(-1.5f, 0.0f, -1.3f), f(1.5f, 0.0f, -1.3f), g(1.5f, 3.0f, -1.3f), h(-1.5f, 3.0f, -1.3f);
scene.add_object(std::make_shared<Triangle>(e, g, f, vidrio));   // (e,f,g) invertido
scene.add_object(std::make_shared<Triangle>(e, h, g, vidrio));   // (e,g,h) invertido
```

### Error típico a evitar

No le pongas el mismo nombre de variable a dos vértices distintos (ej. usar
`a_izq` tanto para la esquina de abajo como la de arriba) — es redefinición y
no compila. Cada una de las 4 (u 8, si son 2 caras) esquinas necesita su
propio nombre: `a`, `b`, `c`, `d` (y `e`, `f`, `g`, `h` para la segunda cara)
es la convención más simple, o nombres descriptivos tipo `abajo_izq`,
`arriba_izq`, `abajo_der`, `arriba_der`.

## 5. Aspectos que el Práctico 10 dejó fijos para las 8 escenas, y dónde
   cambiarlos si el parcial da otros valores

Estos NO son valores por defecto del motor — son la convención que usó el
Práctico 10 en sus 8 escenas. Si el enunciado del parcial da otros números,
hay que tocar estos lugares puntuales:

- **Resolución (1280x720) y supermuestreo (3x3)**: NO están en el código de
  la escena. Son argumentos de línea de comando al ejecutar
  (`-r W H -ss N`). Si el parcial pide otra resolución/supermuestreo, se
  cambia en el comando, no hace falta tocar ningún `.cpp`.
- **Corrección gamma (2.2)**: está *hardcodeada* en el lugar donde se llama
  a escribir la imagen, en `src/main.cpp` (línea con `img.write(out_path,
  normals ? 1.0f : 2.2f)`). Si el parcial pidiera otro gamma, se cambia ese
  literal `2.2f` ahí (no es un parámetro de la escena).
- **Atenuación de luces (1, 0.12, 0.02)**: esto NO es un default del motor
  — es el 3er argumento de `Light(pos, color, atenuación)`, que hay que
  escribir explícitamente en cada luz de cada escena. Si el parcial no dice
  nada de atenuación, el default real del motor es `(1, 0, 0)` (sin
  atenuación) — se puede omitir el 3er argumento. Si el parcial da otros
  coeficientes, van ahí, por luz.
- **`max_depth` (8 por defecto)**: el valor por defecto está en el motor
  (`scene.h`, `max_depth_ = 8U`), pero se puede pisar por escena con
  `scene.set_max_depth(N);` dentro de `build_scene_NN`. Si el parcial pide
  otra profundidad máxima, se agrega/cambia esa línea en la función de la
  escena.

## 6. Recordatorios rápidos (para no perder tiempo el día del parcial)

- **Vidrio dentro de agua / vidrio hueco**: el `ior` de la superficie interior
  es el índice relativo *de adentro con respecto a afuera*. Si el material de
  "afuera" es distinto del aire, hay que pensarlo como `ior = n_adentro / n_afuera`.
- **Transparencia y color**: si además de transparente el material tiene un
  color (vidrio verdoso), ese efecto de absorción de color se nota como el
  **cuadrado** de la transparencia/color por unidad de paso (atraviesa 2 veces
  el espesor real al entrar y salir).
- **ka vs kd**: `ka` es la fracción de luz ambiente (indirecta, siempre
  presente) reflejada; `kd` es la fracción de luz difusa (directa) reflejada.
  Un valor típico es `ka` bastante más chico que `kd` (ambiente aporta poco).
- **ks / shininess**: `ks` = intensidad del brillo; `shininess` = qué tan
  concentrado/chico es (alto = puntito chico y duro, tipo metal pulido; bajo =
  mancha ancha y difusa, tipo plástico mate).
- **Normal del triángulo**: sale de `(b-a) x (c-a)` — si la escena se ve "al
  revés" (cara trasera visible, cara de adelante no), probablemente hay que
  invertir el orden de 2 vértices.
- **EPSILON**: ya lo maneja el motor en las intersecciones; no hace falta
  tocarlo al escribir una escena, solo recordarlo si preguntan por qué existe
  (evita auto-sombreado / auto-intersección por error de punto flotante).
- **Atenuación por defecto**: si el enunciado no menciona atenuación, usar
  `glm::vec3(1.0f, 0.0f, 0.0f)` (o simplemente omitir el 3er argumento de
  `Light`, tiene ese valor por defecto).
