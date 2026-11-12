#include <memory>

#include <checker.h>
#include <escenas.h>
#include <plane.h>
#include <sphere.h>
#include <mirror.h>
#include <triangle.h>
#include <dielectric.h>

void build_scene_07(Scene& scene, Camera& camera)
{

    camera.look_at(glm::vec3(0.0f, 0.9f, 6.0f),     // ojo
    glm::vec3(0.0f, 0.35f, -4.0f),     // hacia dónde mira
    glm::vec3(0.0f, 1.0f, 0.0f), 
    50.0f); 

    scene.set_background(glm::vec3(0.35f, 0.55f, 0.8f)); //fondo

    //Fondo Piscina
    auto pool = std::make_shared<Checker>();
    pool -> size = 0.5f;
    pool->kd   = glm::vec3(0.85f, 0.85f, 0.85f);   
    pool->ka   = glm::vec3(0.05f,0.05f, 0.05f);
    pool->kd2   = glm::vec3(0.08f, 0.3f, 0.55f);   
    pool->ka2   = glm::vec3(0.005f, 0.02f, 0.035f);
    scene.add_object(std::make_shared<Plane>(glm::vec3(0.0f,-1.0f,0.0f), glm::vec3(0.0f, 1.0f, 0.0f), pool));
  
    //Superficie Agua
    auto water_surface = std::make_shared<Dielectric>();
    water_surface->ior   = 1.33f;   
    water_surface->transparency = 0.9f;
    water_surface->ks   = glm::vec3(1.0f);
    water_surface->shininess = 300.0f;
    scene.add_object(std::make_shared<Plane>(glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f), water_surface));
  

    auto red = std::make_shared<Diffuse>();
    red->ka = glm::vec3(0.04f, 0.0f, 0.0f);
    red->kd = glm::vec3(0.8f, 0.1f, 0.1f);
    red->ks = glm::vec3(0.4f, 0.4f, 0.4f);
    red->shininess = 40.0f;
    scene.add_object(std::make_shared<Sphere>(glm::vec3(0.2f, 0.7f, 1.5f), 0.5f, red));

    auto yellow = std::make_shared<Diffuse>();
    yellow->ka = glm::vec3(0.04f, 0.04f, 0.0f);
    yellow->kd = glm::vec3(0.85f, 0.75f,0.1f);
    yellow->ks = glm::vec3(0.4f, 0.4f, 0.4f);
    yellow->shininess = 40.0f;
    scene.add_object(std::make_shared<Sphere>(glm::vec3(-4.5f, 2.2f, -16.0f), 2.0f, yellow));

    auto green = std::make_shared<Diffuse>();
    green->ka = glm::vec3(0.0f, 0.04f, 0.0f);
    green->kd = glm::vec3(0.1f, 0.7f, 0.2f);
    green->ks = glm::vec3(0.4f, 0.4f, 0.4f);
    green->shininess = 40.0f;
    scene.add_object(std::make_shared<Sphere>(glm::vec3(4.0f, 1.6f, -11.0f), 1.4f, green));

    scene.add_light(Light(glm::vec3(-6.0f, 10.0f, 4.0f),
    glm::vec3(13.5f, 13.5f, 12.6f),
    glm::vec3(1.0f, 0.12f, 0.02f)));

    scene.set_max_depth(3U);

}
