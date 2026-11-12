#include <dielectric.h>
#include <scene.h>

glm::vec3 Dielectric::local(const Scene& scene, const Ray& r, const Hit& h) const
{
    return h.front_face ? scene.local_terms(r, h).specular : glm::vec3(0.0f);
}

/// Aproximación de Schlick de la reflectancia de Fresnel
///     F(θ) = F0 + (1 - F0) (1 - cos θ)^5,   F0 = ((1 - η) / (1 + η))^2
///
/// cosine es el coseno del ángulo del lado del medio menos denso: el de
/// incidencia si el rayo entra a un medio más denso (η < 1), el de
/// transmisión si no
static float schlick(float cosine, float eta)
{
    float r0 = (1.0f - eta) / (1.0f + eta);
    r0 = r0 * r0;
    return r0 + (1.0f - r0) * glm::pow(1.0f - cosine, 5.0f);
}

int Dielectric::secondary(const Ray& in, const Hit& h, Secondary out[2]) const
{
    // TODO (paso 9): reflexión y refracción.
    //
    // a) η = η_i / η_t. Si el rayo entra al objeto (h.front_face) es 1 / ior;
    //    si sale, ior.
    //
    // b) Rayo reflejado, en out[0]: igual que en el espejo. Existe siempre.
    //
    // c) Snell: sin θt = η sin θi, de donde cos²θt = 1 - η² (1 - cos²θi),
    //    con cos θi = -d . n (d normalizada).
    //    Si cos²θt da negativo, no hay rayo refractado (reflexión interna
    //    total): toda la luz va al reflejado (peso 1) y se devuelve 1.
    //    No usar glm::refract para decidir esto: hace su propia cuenta, y
    //    cerca del ángulo límite el redondeo puede hacer que una prueba pase y
    //    la otra no. En ese caso glm::refract devuelve el vector nulo, al
    //    normalizarlo da NaN y aparecen píxeles negros sueltos sobre el vidrio.
    //
    // d) Rayo refractado, en out[1]:  T = η d + (η cos θi - cos θt) n,
    //    normalizado. El origen se corre hacia el otro lado de la superficie,
    //    h.p - h.n * EPSILON.
    //
    // e) Pesos: F = schlick(coseno, η) para el reflejado y 1 - F para el
    //    refractado. El coseno es cos θi si η <= 1 y cos θt si no (ver el
    //    comentario de schlick). Se devuelve 2.

    const float eta = h.front_face ? (1.0f / ior) : ior;

    const glm::vec3 d = glm::normalize(in.d());  // dirección del rayo entrante, normalizada

    // (b) Rayo reflejado: existe siempre, misma cuenta que en el espejo
    const glm::vec3 dir = glm::reflect(d, h.n);
    const glm::vec3 origen = h.p + h.n * EPSILON;

    // (c) Ley de Snell: coseno de incidencia, y cos² del ángulo refractado
    const float cos_i  = -glm::dot(d, h.n);
    const float cos2_t = 1.0f - eta * eta * (1.0f - cos_i * cos_i);

    if( cos2_t < 0.0f ){
        // Reflexión interna total: toda la luz va al reflejado
        out[0] = { Ray(origen, dir), 1.0f };
        return 1;
    }

    const float cos_t = glm::sqrt(cos2_t);

    // (d) Rayo refractado, al otro lado de la superficie
    const glm::vec3 dir_refractado    = glm::normalize(eta * d + (eta * cos_i - cos_t) * h.n);
    const glm::vec3 origen_refractado = h.p - h.n * EPSILON;

    // (e) Pesos con Schlick: F para el reflejado, 1-F para el refractado
    const float cosine = (eta <= 1.0f) ? cos_i : cos_t;
    const float F = schlick(cosine, eta);

    out[0] = { Ray(origen, dir), F };
    out[1] = { Ray(origen_refractado, dir_refractado), 1.0f - F };
    return 2;
}
