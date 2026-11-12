#include <memory>

#include <checker.h>
#include <escenas.h>
#include <plane.h>
#include <sphere.h>
#include <mirror.h>
#include <triangle.h>
#include <dielectric.h>

void build_scene_06(Scene& scene, Camera& camera)
{

    camera.look_at(glm::vec3(0.0f, 2.2f, 6.0f),     // ojo
    glm::vec3(0.0f, 0.8f, 0.0f),     // hacia dónde mira
    glm::vec3(0.0f, 1.0f, 0.0f), 
    45.0f);     // arrib1. ¿Qué fracción de la luz deja pasar la esfera de vidrio a un rayo de sombra? ¿Y la de agua?
    
    scene.set_background(glm::vec3(0.05f, 0.07f, 0.1f)); //fondo


    auto floor = std::make_shared<Checker>();
    floor -> size = 0.4f;
    floor->kd   = glm::vec3(0.85f, 0.85f, 0.85f);   
    floor->ka   = glm::vec3(0.05f,0.05f, 0.05f);
    floor->kd2   = glm::vec3(0.06f, 0.3f, 0.1f);   
    floor->ka2   = glm::vec3(0.004f, 0.018f, 0.006f);
    scene.add_object(std::make_shared<Plane>(glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f), floor));
  
    auto maciza = std::make_shared<Dielectric>();
    maciza->ks = glm::vec3(1.0f, 1.0f, 1.0f);
    maciza->transparency = 0.9f;
    maciza->ior = 1.5f;
    maciza->shininess = 150.0f;
    scene.add_object(std::make_shared<Sphere>(glm::vec3(-2.0f, 0.9f, 0.0f), 0.8f, maciza));

    auto hueca_ext = std::make_shared<Dielectric>();
    hueca_ext->ks = glm::vec3(1.0f, 1.0f, 1.0f);
    hueca_ext->transparency = 0.9f;
    hueca_ext->ior = 1.5f;
    hueca_ext->shininess = 150.0f;
    scene.add_object(std::make_shared<Sphere>(glm::vec3(0.0f, 0.9f, 0.0f), 0.8f, hueca_ext));


    auto hueca_int = std::make_shared<Dielectric>();
    hueca_int->ks = glm::vec3(0.0f, 0.0f, 0.0f);
    hueca_int->transparency = 0.9f;
    hueca_int->ior = 1.0f/1.5f;
    hueca_int->shininess = 150.0f;
    scene.add_object(std::make_shared<Sphere>(glm::vec3(0.0f, 0.9f, 0.0f), 0.768f, hueca_int));

    auto bubble = std::make_shared<Dielectric>();
    bubble->ks = glm::vec3(1.0f, 1.0f, 1.0f);
    bubble->transparency = 0.9f;
    bubble->ior = 1.0f/1.33f;
    bubble->shininess = 150.0f;
    scene.add_object(std::make_shared<Sphere>(glm::vec3(2.0f, 0.9f, 0.0f), 0.8f, bubble));

    
    scene.add_light(Light(glm::vec3(-3.0f, 6.0f, 4.0f),
    glm::vec3(2.6f, 2.6f, 2.6f),
    glm::vec3(1.0f, 0.12f, 0.02f)));

    scene.add_light(Light(glm::vec3(4.0f, 4.0f, 3.0f),
    glm::vec3(1.0f, 1.0f, 1.0f),
    glm::vec3(1.0f, 0.12f, 0.02f)));

}
