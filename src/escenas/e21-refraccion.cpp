#include <memory>

#include <checker.h>
#include <escenas.h>
#include <plane.h>
#include <sphere.h>
#include <mirror.h>
#include <triangle.h>
#include <dielectric.h>

void build_scene_21(Scene& scene, Camera& camera)
{

    camera.look_at(glm::vec3(3.0f, 1.6f, 5.0f),     // ojo
    glm::vec3(0.0f, 1.0f, -2.0f),     // hacia dónde mira
    glm::vec3(0.0f, 1.0f, 0.0f),     // arriba
    45.0f);       
    scene.set_background(glm::vec3(0.03f, 0.03f, 0.05f));

    auto floor = std::make_shared<Checker>();
    floor -> size = 0.8f;
    floor->kd   = glm::vec3(0.8f, 0.8f, 0.8f);   
    floor->ka   = glm::vec3(0.05f,0.05f,0.05f);
    floor->kd2   = glm::vec3(0.1f, 0.1f, 0.15f);   
    floor->ka2   = glm::vec3(0.01f, 0.01f, 0.02f);
    scene.add_object(std::make_shared<Plane>(glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f), floor));


    // Material del panel de vidrio
    auto vidrio = std::make_shared<Dielectric>();
    vidrio->ior = 1.5f;
    vidrio->transparency = 0.8f;

    // Vertices cara delantera (z = -1.0): normal tiene que apuntar hacia +z (hacia la camara)
    glm::vec3 a(-1.5f, 0.0f, -1.0f);   // abajo-izq
    glm::vec3 b(-1.5f, 3.0f, -1.0f);   // arriba-izq
    glm::vec3 c(1.5f, 0.0f, -1.0f);    // abajo-der
    glm::vec3 d(1.5f, 3.0f, -1.0f);    // arriba-der

    // Vertices cara trasera (z = -1.3): normal tiene que apuntar hacia -z (lejos de la camara)
    glm::vec3 e(-1.5f, 0.0f, -1.3f);   // abajo-izq
    glm::vec3 f(-1.5f, 3.0f, -1.3f);   // arriba-izq
    glm::vec3 g(1.5f, 0.0f, -1.3f);    // abajo-der
    glm::vec3 h(1.5f, 3.0f, -1.3f);    // arriba-der

    scene.add_object(std::make_shared<Triangle>(a, c, d, vidrio));
    scene.add_object(std::make_shared<Triangle>(a, d, b, vidrio));

    scene.add_object(std::make_shared<Triangle>(e, h, g, vidrio));
    scene.add_object(std::make_shared<Triangle>(e, f, h, vidrio));

    // Esfera roja: se ve A TRAVES del panel de vidrio
    auto rojo = std::make_shared<Diffuse>();
    rojo->ka = glm::vec3(0.05f, 0.0f, 0.0f);
    rojo->kd = glm::vec3(0.75f, 0.1f, 0.08f);
    rojo->ks = glm::vec3(0.3f, 0.3f, 0.3f);
    rojo->shininess = 40.0f;
    scene.add_object(std::make_shared<Sphere>(glm::vec3(0.0f, 1.0f, -3.0f), 0.8f, rojo));

    // Esfera verde: al costado del panel, para comparar contra la vista directa
    auto verde = std::make_shared<Diffuse>();
    verde->ka = glm::vec3(0.0f, 0.05f, 0.0f);
    verde->kd = glm::vec3(0.1f, 0.7f, 0.2f);
    verde->ks = glm::vec3(0.3f, 0.3f, 0.3f);
    verde->shininess = 40.0f;
    scene.add_object(std::make_shared<Sphere>(glm::vec3(3.0f, 1.0f, -3.0f), 0.8f, verde));

    scene.add_light(Light(glm::vec3(0.0f, 6.0f, 3.0f),
                           glm::vec3(3.0f, 3.0f, 3.0f),
                           glm::vec3(1.0f, 0.12f, 0.02f)));

}