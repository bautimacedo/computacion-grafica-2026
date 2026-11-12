#include <memory>

#include <checker.h>
#include <escenas.h>
#include <plane.h>
#include <sphere.h>
#include <mirror.h>
#include <triangle.h>
#include <dielectric.h>

void build_scene_05(Scene& scene, Camera& camera)
{
    camera.look_at(glm::vec3(0.0f, 1.8f, 5.0f),     // ojo
    glm::vec3(0.0f, 0.7f, 0.0f),     // hacia dónde mira
    glm::vec3(0.0f, 1.0f, 0.0f), 
    45.0f);     // arrib1. ¿Qué fracción de la luz deja pasar la esfera de vidrio a un rayo de sombra? ¿Y la de agua?
    
    auto floor = std::make_shared<Checker>();
    floor -> size = 0.4f;
    floor->kd   = glm::vec3(0.85f, 0.85f, 0.85f);   
    floor->ka   = glm::vec3(0.05f,0.05f, 0.05f);
    floor->kd2   = glm::vec3(0.08f, 0.08f, 0.3f);   
    floor->ka2   = glm::vec3(0.005f, 0.005f, 0.018f);
    scene.add_object(std::make_shared<Plane>(glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f), floor));
  
    scene.set_background(glm::vec3(0.05f, 0.07f, 0.1f)); //fondo

    auto glass = std::make_shared<Dielectric>();
    glass->ks = glm::vec3(1.0f, 1.0f, 1.0f);
    glass->transparency = 0.9f;
    glass->ior = 1.5f;
    glass->shininess = 150.0f;
    scene.add_object(std::make_shared<Sphere>(glm::vec3(0.8f, 0.8f, 0.0f), 0.7f, glass));


    auto air = std::make_shared<Dielectric>();
    air->ks = glm::vec3(1.0f, 1.0f, 1.0f);
    air->transparency = 0.9f;
    air->ior = 1.0f;
    air->shininess = 150.0f;
    scene.add_object(std::make_shared<Sphere>(glm::vec3(-2.4f, 0.8f, 0.0f), 0.7f, air));


    auto water = std::make_shared<Dielectric>();
    water->ks = glm::vec3(1.0f, 1.0f, 1.0f);
    water->transparency = 0.9f;
    water->ior = 1.33f;
    water->shininess = 150.0f;
    scene.add_object(std::make_shared<Sphere>(glm::vec3(-0.8f, 0.8f, 0.0f), 0.7f, water));


    auto diamond = std::make_shared<Dielectric>();
    diamond->ks = glm::vec3(1.0f, 1.0f, 1.0f);
    diamond->transparency = 0.9f;
    diamond->ior = 2.42f;
    diamond->shininess = 150.0f;
    scene.add_object(std::make_shared<Sphere>(glm::vec3(2.4f, 0.8f, 0.0f), 0.7f, diamond));

    scene.add_light(Light(glm::vec3(-3.0f, 6.0f, 4.0f),
    glm::vec3(2.6f, 2.6f, 2.6f),
    glm::vec3(1.0f, 0.12f, 0.02f)));

    scene.add_light(Light(glm::vec3(4.0f, 4.0f, 3.0f),
    glm::vec3(1.0f, 1.0f, 1.0f),
    glm::vec3(1.0f, 0.12f, 0.02f)));


}