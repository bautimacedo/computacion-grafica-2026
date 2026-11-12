# Glosario técnico — parámetros y conceptos del trazador

Referencia rápida para interpretar enunciados de escenas y preguntas del
parcial (Práctico 10 y modelo de Whitted). Explicaciones simples y técnicas.

---

## Material `Diffuse` (Phong)

- **ka**: fracción de luz AMBIENTE (indirecta, constante, no depende de
  ninguna luz puntual) que refleja la superficie. Es el color mínimo que
  tiene el objeto aunque ninguna luz le llegue directo.
- **kd**: fracción de luz DIFUSA (directa) que refleja. Es "el color"
  visible del objeto bajo luz directa; depende de `N·L` (normal vs.
  dirección a la luz).
- **ks**: intensidad del brillo ESPECULAR (destello puntual de la luz).
  Depende de `R·V` (reflejo ideal de la luz vs. dirección a la cámara).
- **shininess**: qué tan concentrado es ese brillo. Alto = punto chico y
  duro (metal pulido); bajo = mancha ancha y difusa (plástico mate).

## Material `Checker`

Igual a `Diffuse` pero con un segundo par `ka2`/`kd2` que alterna con
`ka`/`kd` en casillas de lado `size`, sobre el plano xz.

## Material `Mirror`

Todo lo de `Diffuse` (ka, kd, ks, shininess) más:

- **kr**: peso (0 a 1) del rayo reflejado en el color final. Es la fracción
  de luz que llega por la dirección de reflexión (modelo de Whitted) que se
  suma al resultado. Cercano a 1 = espejo casi perfecto.

## Material `Dielectric` (vidrio, agua, etc.)

No usa `ka` ni `kd` (no tiene color propio difuso/ambiente). Usa:

- **ior**: índice de refracción RELATIVO (medio de adentro / medio de
  afuera). Rige la desviación del rayo refractado (Snell) y también `F0`
  de Schlick.
- **transparency**: fracción de luz que deja pasar CADA superficie que
  cruza un RAYO DE SOMBRA (no el rayo de cámara). En una esfera maciza el
  rayo de sombra cruza 2 superficies → fracción total = `transparency²`.
- **ks, shininess**: igual que en `Diffuse`, término especular de Phong.
- El reparto entre rayo reflejado y refractado del RAYO DE CÁMARA se
  calcula con `F` de Schlick, no con `transparency`.

## Aproximación de Schlick (reflectancia `F`)

- `F0 = ((n1-n2)/(n1+n2))²` — reflectancia a incidencia normal (`θ=0°`,
  rayo perpendicular a la superficie). `n1`, `n2` son los índices de los
  dos medios (o el `ior` relativo si ya viene como cociente).
- `F(θ) = F0 + (1-F0)·(1-cosθ)⁵` — reflectancia para cualquier ángulo de
  incidencia `θ`. `cosθ` sale del producto escalar entre la normal `N` y
  la dirección del rayo.
- Comportamiento clave (efecto Fresnel, universal, no exclusivo de
  dieléctricos): a medida que `θ` crece hacia 90° (rayo rasante), `F`
  siempre tiende a 1, sin importar cuánto valga `F0`. Toda superficie
  refleja más mirada de costado que de frente.
- Peso del rayo reflejado (cámara): `F`. Peso del refractado: `1-F`.

## Recursión y `max_depth`

Cada intersección con `Mirror` o `Dielectric` genera rayos secundarios.
`max_depth` limita los niveles de rebote; una rama que lo supera no aporta
luz (se corta, aporta 0).

- `Mirror`: 1 rayo secundario por intersección (reflejado).
- `Dielectric`: 2 rayos secundarios por intersección (reflejado +
  refractado), si ninguno se corta antes. Árbol binario → hasta
  `2^(n+1)-1` rayos por píxel con profundidad `n`, sin contar sombra.
- Cada nivel multiplica la contribución por un coeficiente `<1` (`kr`, o
  `F`/`1-F`), así que niveles profundos aportan cada vez menos color,
  aunque el COSTO de cómputo siga creciendo (lineal en `Mirror`,
  exponencial en `Dielectric`).

## Luces

`Light(posición, color/intensidad, atenuación=(c0,c1,c2))`.

- Atenuación: `f_att = 1/(c0 + c1·d + c2·d²)`, con `d` = distancia al
  punto. Si no la dan: `(1,0,0)` = sin atenuación.
- Rayo de sombra: va del punto hacia la luz. Si pega en un `Dielectric` se
  atenúa por `transparency` (no bloquea del todo); si pega en cualquier
  otro material opaco, bloquea el 100%.

## Lentes (refracción, no reflejo)

- **Convergente**: rayos paralelos que entran se desvían HACIA el eje
  óptico y se cruzan después del objeto. Imagen vista a través: invertida.
  Pasa al ir de un medio MENOS denso a uno MÁS denso (ej.: esfera maciza
  de vidrio en aire).
- **Divergente**: rayos paralelos que entran se desvían ALEJÁNDOSE del eje
  óptico, no se cruzan. Imagen vista a través: derecha, más chica. Pasa al
  ir de un medio MÁS denso a uno MENOS denso (ej.: burbuja de aire dentro
  de agua).
