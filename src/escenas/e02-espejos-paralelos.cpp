#include <memory>

#include <escenas.h>
#include <plane.h>
#include <sphere.h>
#include <mirror.h>
#include <checker.h>

void build_scene_02(Scene& scene, Camera& camera)
{
    camera.look_at(glm::vec3(2.0f, 1.6f, 2.5f),     // ojo
    glm::vec3(-6.0f, 0.7f, -1.5f),     // hacia dónde mira
    glm::vec3(0.0f, 1.0f, 0.0f),     // arriba
    55.0f);                          // fov vertical [grados]

    scene.set_background(glm::vec3(0.02f, 0.02f, 0.03f)); //fondo

    // Piso: tablero rojo y amarillo. ka también sigue al tablero, para que las
    // casillas se distingan dentro de las sombras
    auto floor = std::make_shared<Checker>();
    floor -> size = 0.5f;
    floor->kd   = glm::vec3(0.85f, 0.85f, 0.85f);   
    floor->ka   = glm::vec3(0.05f, 0.05f, 0.05f);
    floor->kd2   = glm::vec3(0.1f, 0.1f, 0.1f);   
    floor->ka2   = glm::vec3(0.006f, 0.006f, 0.006f);

    auto left_mirror = std::make_shared<Mirror>();
    left_mirror->ka = glm::vec3(0.0f);
    left_mirror->kd = glm::vec3(0.0f);
    left_mirror->ks = glm::vec3(0.0f);
    
    auto right_mirror = std::make_shared<Mirror>();
    right_mirror->ka = glm::vec3(0.0f);
    right_mirror->kd = glm::vec3(0.0f);
    right_mirror->ks = glm::vec3(0.0f);



     // Esfera nranja
     auto orange = std::make_shared<Diffuse>();
     orange->ka = glm::vec3(0.05f,0.02f,0.0f);
     orange->kd = glm::vec3(0.9f,0.45f,0.05f);
     orange->ks = glm::vec3(0.5f,0.5f,0.5f);
     orange->shininess = 50.0f;


     scene.add_object(std::make_shared<Plane>(glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f), floor));
     scene.add_object(std::make_shared<Plane>(glm::vec3(-2.5f,0.0f,0.0f), glm::vec3(1.0f, 0.0f, 0.0f), left_mirror));
     scene.add_object(std::make_shared<Plane>(glm::vec3(2.5f,0.0f,0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), right_mirror));
     scene.add_object(std::make_shared<Sphere>(glm::vec3(0.0f, 0.7f, -1.0f), 0.7f, orange));


       // Alta y un poco por delante de las esferas, para que los brillos queden
    // hacia el observador. Con atenuación 1 / (1 + 0.12 d + 0.02 d^2)
    scene.add_light(Light(glm::vec3(0.0f, 5.0f, 2.0f),
    glm::vec3(3.5f, 3.5f, 3.5f),
    glm::vec3(1.0f, 0.12f, 0.02f)));

}