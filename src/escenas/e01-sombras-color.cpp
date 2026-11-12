#include <memory>

#include <escenas.h>
#include <plane.h>
#include <sphere.h>

void build_scene_01(Scene& scene, Camera& camera)
{
    camera.look_at(glm::vec3(0.0f, 6.0f, 7.0f),     // ojo
    glm::vec3(0.0f, 0.3f, 0.0f),     // hacia dónde mira
    glm::vec3(0.0f, 1.0f, 0.0f),     // arriba
    45.0f);                          // fov vertical [grados]

    scene.set_background(glm::vec3(0.02f, 0.02f, 0.02f)); //fondo

    // Piso: tablero rojo y amarillo. ka también sigue al tablero, para que las
    // casillas se distingan dentro de las sombras
    auto floor = std::make_shared<Diffuse>();
    floor->kd   = glm::vec3(0.8f, 0.8f, 0.8f);   
    floor->ka   = glm::vec3(0.02f, 0.02f, 0.02f);
    floor->ks   = glm::vec3(0.0f);

     // Esfera blanca
     auto white = std::make_shared<Diffuse>();
     white->ka = glm::vec3(0.02f);
     white->kd = glm::vec3(0.8f);
     white->ks = glm::vec3(0.3f);
     white->shininess = 30.0f;


     scene.add_object(std::make_shared<Plane>(glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f), floor));
     scene.add_object(std::make_shared<Sphere>(glm::vec3(0.0f, 1.0f, 0.0f), 1.0f, white));


       // Alta y un poco por delante de las esferas, para que los brillos queden
    // hacia el observador. Con atenuación 1 / (1 + 0.12 d + 0.02 d^2)
    scene.add_light(Light(glm::vec3(0.0f, 5.0f, -4.0f),
    glm::vec3(2.4f, 0.0f, 0.0f),
    glm::vec3(1.0f, 0.12f, 0.02f)));

    scene.add_light(Light(glm::vec3(-3.464f, 5.0f, 2.0f),
    glm::vec3(0.0f, 2.4f, 0.0f),
    glm::vec3(1.0f, 0.12f, 0.02f)));

    scene.add_light(Light(glm::vec3(3.464f, 5.0f, 2.0f),
    glm::vec3(0.0f, 0.0f, 2.4f),
    glm::vec3(1.0f, 0.12f, 0.02f)));
}