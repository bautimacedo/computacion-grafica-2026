#include <memory>

#include <checker.h>
#include <escenas.h>
#include <plane.h>
#include <sphere.h>
#include <mirror.h>
#include <triangle.h>
#include <dielectric.h>

void build_scene_08(Scene& scene, Camera& camera)
{

    camera.look_at(glm::vec3(0.0f, 2.0f, 6.0f),     // ojo
    glm::vec3(0.0f, 1.5f, 0.0f),     // hacia dónde mira
    glm::vec3(0.0f, 1.0f, 0.0f), 
    45.0f);     // arrib1. ¿Qué fracción de la luz deja pasar la esfera de vidrio a un rayo de sombra? ¿Y la de agua?
    
    scene.set_background(glm::vec3(0.05f, 0.07f, 0.1f)); //fondo


    auto floor = std::make_shared<Checker>();
    floor -> size = 0.4f;
    floor->kd   = glm::vec3(0.85f, 0.85f, 0.85f);   
    floor->ka   = glm::vec3(0.05f,0.05f, 0.05f);
    floor->kd2   = glm::vec3(0.08f, 0.08f, 0.3f);   
    floor->ka2   = glm::vec3(0.005f, 0.005f, 0.018f);
    scene.add_object(std::make_shared<Plane>(glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f), floor));


    auto sup_ext1 = std::make_shared<Dielectric>();
    sup_ext1->ks = glm::vec3(1.0f, 1.0f, 1.0f);
    sup_ext1->transparency = 0.9f;
    sup_ext1->ior = 1.5f;
    sup_ext1->shininess = 150.0f;
    scene.add_object(std::make_shared<Sphere>(glm::vec3(0.0f, 1.6f, 0.0f), 1.5f, sup_ext1));

    auto sup_ext2 = std::make_shared<Dielectric>();
    sup_ext2->ks = glm::vec3(1.0f, 1.0f, 1.0f);
    sup_ext2->transparency = 0.9f;
    sup_ext2->ior = 1.5f;
    sup_ext2->shininess = 150.0f;
    scene.add_object(std::make_shared<Sphere>(glm::vec3(0.0f, 1.6f, 0.0f), 1.0f, sup_ext2));


    auto sup_ext3 = std::make_shared<Dielectric>();
    sup_ext3->ks = glm::vec3(1.0f, 1.0f, 1.0f);
    sup_ext3->transparency = 0.9f;
    sup_ext3->ior = 1.5f;
    sup_ext3->shininess = 150.0f;
    scene.add_object(std::make_shared<Sphere>(glm::vec3(0.0f, 1.6f, 0.0f), 0.6f, sup_ext3));

    auto sup_int1 = std::make_shared<Dielectric>();
    sup_int1->ks = glm::vec3(0.0f, 0.0f, 0.0f);
    sup_int1->transparency = 0.9f;
    sup_int1->ior = 1.0f/1.5f;
    sup_int1->shininess = 150.0f;
    scene.add_object(std::make_shared<Sphere>(glm::vec3(0.0f, 1.6f, 0.0f), 1.44f, sup_int1));

    auto sup_int2 = std::make_shared<Dielectric>();
    sup_int2->ks = glm::vec3(0.0f, 0.0f, 0.0f);
    sup_int2->transparency = 0.9f;
    sup_int2->ior = 1.0f/1.5f;
    sup_int2->shininess = 150.0f;
    scene.add_object(std::make_shared<Sphere>(glm::vec3(0.0f, 1.6f, 0.0f), 0.96f, sup_int2));


    auto sup_int3 = std::make_shared<Dielectric>();
    sup_int3->ks = glm::vec3(0.0f, 0.0f, 0.0f);
    sup_int3->transparency = 0.9f;
    sup_int3->ior = 1.0f/1.5f;
    sup_int3->shininess = 150.0f;
    scene.add_object(std::make_shared<Sphere>(glm::vec3(0.0f, 1.6f, 0.0f), 0.576f, sup_int3));


    auto core = std::make_shared<Diffuse>();
    core->ks = glm::vec3(0.5f, 0.5f, 0.5f);
    core->ka = glm::vec3(0.05f, 0.0f, 0.0f);
    core->kd = glm::vec3(0.85f, 0.08f, 0.05f);
    core->shininess=40.0f;
    scene.add_object(std::make_shared<Sphere>(glm::vec3(0.0f, 1.6f, 0.0f), 0.3f, core));

    
    scene.add_light(Light(glm::vec3(-3.0f, 6.0f, 4.0f),
    glm::vec3(2.8f, 2.8f, 2.8f),
    glm::vec3(1.0f, 0.12f, 0.02f)));

    scene.add_light(Light(glm::vec3(4.0f, 4.0f, 3.0f),
    glm::vec3(1.0f, 1.0f, 1.0f),
    glm::vec3(1.0f, 0.12f, 0.02f)));
    scene.set_max_depth(3U);
}
