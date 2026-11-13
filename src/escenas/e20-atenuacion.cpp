#include <memory>

#include <checker.h>
#include <escenas.h>
#include <plane.h>
#include <sphere.h>
#include <mirror.h>
#include <triangle.h>
#include <dielectric.h>

void build_scene_20(Scene& scene, Camera& camera)
{

    camera.look_at(glm::vec3(0.0f, 3.0f, 10.0f),     // ojo
    glm::vec3(0.0f, 1.0f, -2.0f),     // hacia dónde mira
    glm::vec3(0.0f, 1.0f, 0.0f),     // arriba
    40.0f);       

    scene.set_background(glm::vec3(0.05f, 0.07f, 0.12f),glm::vec3(0.01f, 0.01f, 0.03f));

    auto floor = std::make_shared<Checker>();
    floor -> size = 1.0f;
    floor->kd   = glm::vec3(0.7f, 0.7f, 0.7f);   
    floor->ka   = glm::vec3(0.05f,0.05f,0.05f);
    floor->kd2   = glm::vec3(0.1f, 0.1f, 0.1f);   
    floor->ka2   = glm::vec3(0.01f, 0.01f, 0.01f);
    scene.add_object(std::make_shared<Plane>(glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f), floor));


    auto sphere_model = std::make_shared<Diffuse>();
    sphere_model->kd   = glm::vec3(0.1f, 0.2f, 0.8f);   
    sphere_model->ka   = glm::vec3(0.02f,0.02f,0.05f);
    sphere_model->ks   = glm::vec3(0.3f,0.3f,0.3f);
    sphere_model-> shininess = 40.0f;
    scene.add_object(std::make_shared<Sphere>(glm::vec3(-4.0f, 0.8f, 2.0f), 0.8f, sphere_model));
    scene.add_object(std::make_shared<Sphere>(glm::vec3(0.0f, 0.8f, -4.0f), 0.8f, sphere_model));
    scene.add_object(std::make_shared<Sphere>(glm::vec3(4.0f, 0.8f, -10.0f), 0.8f, sphere_model));

    auto sphere_mirror = std::make_shared<Mirror>();
    sphere_mirror->ks   = glm::vec3(1.0f,1.0f,1.0f);
    sphere_mirror-> shininess = 100.0f;
    sphere_mirror->kr=0.8f;
    scene.add_object(std::make_shared<Sphere>(glm::vec3(-2.0f, 0.8f, -14.0f), 0.8f, sphere_mirror));


    scene.add_light(Light(glm::vec3(-4.0f, 4.0f, 2.0f),
    glm::vec3(3.0f, 3.0f, 3.0f),
    glm::vec3(1.0f, 0.12f, 0.02f)));


}