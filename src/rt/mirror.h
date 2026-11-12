#ifndef MIRROR_H
#define MIRROR_H

#include <material.h>

/// Espejo o metal opaco: Phong completo más un rayo reflejado de peso kr
/// constante.  I = I_local + kr * I(R)
/// El brillo de Phong sigue haciendo falta: una luz puntual no es geometría, el
/// rayo reflejado nunca la encuentra
class Mirror : public Material {
public:
    float kr = 0.8f;

    int secondary(const Ray& in, const Hit& h, Secondary out[2]) const override {
        // TODO (paso 8): rayo reflejado.
        // Escribir en out[0] el rayo reflejado y su peso, y devolver 1.
        //   - Dirección: la del rayo incidente (normalizada) reflejada
        //     respecto de la normal, con glm::reflect.
        //   - Origen: el punto de intersección corrido un poco hacia el lado
        //     de la normal, h.p + h.n * EPSILON, por la misma razón que en el
        //     rayo de sombra.
        //   - Peso: kr.

        //Direccion del rayo reflejado

        const glm::vec3 dir = glm::reflect(in.d() , h.n);
        const glm::vec3 origin = h.p + h.n * EPSILON; 
        out[0] = { Ray(origin, dir), kr };

        return 1;
    }
};

#endif // MIRROR_H
