#include <scene.h>


//Recibe un rayo y devuelve cual es el objeto mas cercanoc con el que choca
bool Scene::find_nearest(const Ray& r, Hit& closest) const
{
    bool any = false;
    Hit current;

    for( const auto& obj : objects_ ){
        if( obj->intersect(r, current) && current.t < closest.t ){
            closest = current;
            any = true;
        }
    }
    return any;
}

//Te devuelve el color del fondo en funcion de la direccion del rayo, porque no choca con ningun objeto
glm::vec3 Scene::background(const Ray& r) const
{
    const float up = glm::max(glm::normalize(r.d()).y, 0.0f);
    return glm::mix(bg_horizon_, bg_zenith_, up);
}

//Te devuelve el color de la normal del objeto que choca con el rayo
glm::vec3 Scene::normal_color(const Ray& r) const
{
    Hit h;
    if( !find_nearest(r, h) ){
        return background(r);
    }
    // De [-1,1] a [0,1], componente a componente
    return 0.5f * (h.n + glm::vec3(1.0f));
}

//Te devuelve los terminos de Phong con sombras (local_terms) y el
// material elige cuáles usa. La llamada es virtual: trace no necesita
// saber qué material tiene el objeto
//Te devuelve el resultado de iluminar un punto
//Dado un rayo y la interseccion de un objeto, devuelve...
LocalTerms Scene::local_terms(const Ray& r, const Hit& h) const
{
    LocalTerms terms; //Terminos de Phong
    const Material& m = *h.mat; //Material del objeto intersectado

    const glm::vec3 v = glm::normalize(-r.d());     // hacia el observador
    const glm::vec3& n = h.n;                       // normal del punto

    // Constante de ambiente
    terms.ambient = m.ka_at(h.p);

    //Por cada luz, calculamos la iluminacion que le llega al punto
    for( const auto& L : lights_ ){
        glm::vec3 s = L.pos() - h.p; //vector desde el punto hasta la luz
        const float dist = glm::length(s); //distancia entre el punto y la luz
        s /= dist; //normalizamos el vector

        // Llega luz o hay algo bloqueando? En ese caso pasamos a la siguiente luz
        const float visibility = light_transmission(h.p, n, L.pos());
        if( visibility <= 0.0f ){
            continue;
        }

        // Calculamos el termino difuso
        const float k_diff = glm::max(glm::dot(n, s), 0.0f);

        // Calculamos el termino especular
        const glm::vec3 refl = glm::reflect(-s, n);
        const float k_spec = (k_diff > 0.0f)
                ? glm::pow(glm::max(glm::dot(refl, v), 0.0f), m.shininess)
                : 0.0f;

        // Calculamos la intensidad que llega de la luz
        const glm::vec3 Li = L.color() * (L.attenuation(dist) * visibility);

        terms.diffuse  += m.kd_at(h.p) * k_diff * Li;
        terms.specular += m.ks * k_spec * Li;
    }

    return terms;
}

//Te devuelve la transmision de la luz a traves del vidrio, si hay un objeto en el camino hace sombra plena (devuelve 0).
//Cuanta luz de una luz puntual le llega a un punto?
float Scene::light_transmission(const glm::vec3& p, const glm::vec3& n,
                                const glm::vec3& light_pos) const
{
    // Punto de impacto corrido para afuera, para que el rayo de sombra no
    // choque con la propia superficie por redondeo
    glm::vec3 origin = p + n * EPSILON;

    float luz = 1.0f;  // fracción de luz acumulada que todavía puede llegar

    const int MAX_CRUCES = 16;  // tope por si el rayo queda atrapado dentro de un objeto
    for( int i = 0; i < MAX_CRUCES; ++i ){
        const glm::vec3 to_light = light_pos - origin;
        const float dist = glm::length(to_light);
        const glm::vec3 s = to_light / dist;  // dirección hacia la luz, normalizada

        Hit h;
        if( !find_nearest(Ray(origin, s), h) || h.t >= dist - EPSILON ){
            // No hay nada más en el camino (o lo que hay está más allá de la
            // luz, no cuenta): se terminó el recorrido, devolvemos lo acumulado
            return luz;
        }

        // Se multiplica la fracción acumulada por lo que deja pasar esta
        // superficie (0 para opaco, transparency para vidrio)
        luz *= h.mat->shadow_transmission();
        if( luz <= 0.0f ){
            return 0.0f;  // ya no queda luz, no tiene sentido seguir cruzando
        }

        // Seguimos el recorrido un poco más allá del punto que acabamos de cruzar
        origin = h.p + s * EPSILON;
    }

    return luz;  // caso raro: se agotaron los cruces permitidos
}

// TODO (paso 7): trazado recursivo.
//   - Agregar el parámetro de profundidad a la firma: trace(const Ray& r,
//     unsigned int depth) const (el valor por defecto va sólo en scene.h).
//   - Si depth supera max_depth_, devolver glm::vec3(0.0f): la rama del árbol
//     de rayos termina acá y no aporta luz.
//   - Después de la iluminación local, pedirle al material sus rayos
//     secundarios: Secondary sec[2]; int n = h.mat->secondary(r, h, sec);
//     y sumar al color, por cada uno, sec[i].weight * trace(sec[i].ray, depth + 1).
glm::vec3 Scene::trace(const Ray& r, unsigned int depth) const
{
    //Si la profundidad supera el maximo, devuelve 0 (la rama del arbol de rayos termina aca y no aporta luz)
    if( depth > max_depth_ ){
        return glm::vec3(0.0f);
    }

    Hit h; //Interseccion del rayo con un objeto
    if( !find_nearest(r, h) ){ //Si no hay interseccion, devuelve el color del fondo
        return background(r);
    }

    //Calculamos los terminos de Phong con sombras (local_terms) y el material elige cuáles usa
    glm::vec3 color = h.mat->local(*this, r, h);

    //Le pedimos al material sus rayos secundarios (reflejado/refractado) y sumamos su aporte, pesado, de forma recursiva
    Secondary sec[2]; //Arreglo de hasta 2 rayos secundarios
    const int n = h.mat->secondary(r, h, sec); //Numero de rayos secundarios
    for( int i = 0; i < n; ++i ){
        color += sec[i].weight * trace(sec[i].ray, depth + 1); //Sumamos el aporte de cada rayo secundario, pesado, de forma recursiva
    }

    return color;
}
