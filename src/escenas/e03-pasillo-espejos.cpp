#include <memory>

#include <checker.h>
#include <escenas.h>
#include <plane.h>
#include <sphere.h>
#include <mirror.h>
#include <triangle.h>

void build_scene_03(Scene& scene, Camera& camera)
{

    camera.look_at(glm::vec3(-2.0f, 2.0f, 7.5f),     // ojo
    glm::vec3(3.0f, 1.4f, 0.0f),     // hacia dónde mira
    glm::vec3(0.0f, 1.0f, 0.0f),     // arriba
    45.0f);                          // fov vertical [grados]

    scene.set_background(glm::vec3(0.02f, 0.03f, 0.08f)); //fondo


    auto floor = std::make_shared<Checker>();
    floor -> size = 1.0f;
    floor->kd   = glm::vec3(0.7f, 0.7f, 0.7f);   
    floor->ka   = glm::vec3(0.07f, 0.07f, 0.07f);
    floor->kd2   = glm::vec3(0.1f, 0.1f, 0.1f);   
    floor->ka2   = glm::vec3(0.01f, 0.01f, 0.01f);
    scene.add_object(std::make_shared<Plane>(glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f), floor));

    auto red = std::make_shared<Diffuse>();
    red->ka = glm::vec3(0.08f, 0.01f, 0.01f);
    red->kd = glm::vec3(0.75f, 0.08f, 0.06f);
    red->ks = glm::vec3(0.5f,0.5f,0.5f);
    red->shininess = 60.0f;
    scene.add_object(std::make_shared<Sphere>(glm::vec3(0.2f, 1.0f, 0.0f), 1.0f, red));

    auto green = std::make_shared<Diffuse>();
    green->ka = glm::vec3(0.01f, 0.06f, 0.02f);
    green->kd = glm::vec3(0.1f, 0.6f, 0.2f);
    green->ks = glm::vec3(0.4f, 0.4f, 0.4f);
    green->shininess = 40.0f;
    scene.add_object(std::make_shared<Sphere>(glm::vec3(1.2f, 0.6f, 2.5f), 0.6f, green));

    auto mirror = std::make_shared<Mirror>();
    mirror->ka = glm::vec3(0.02f, 0.02f, 0.02f);
    mirror->kd = glm::vec3(0.1f, 0.1f,0.1f);
    mirror->ks = glm::vec3(1.0f, 1.0f, 1.0f);
    mirror->kr = 0.85f;
    mirror->shininess = 150.0f;
    scene.add_object(std::make_shared<Sphere>(glm::vec3(-1.3f, 0.9f, -2.5f), 0.9f, mirror));

    auto wall_mat = std::make_shared<Mirror>();
    wall_mat->ka = glm::vec3(0.0f, 0.0f, 0.0f);
    wall_mat->kd = glm::vec3(0.02f, 0.02f, 0.02f);
    wall_mat->ks = glm::vec3(0.0f, 0.0f, 0.0f);
    wall_mat->kr = 0.9f;
    
    //Vertices
    glm::vec3 a_izq(-3.0f, 0.0f, 14.0f);
    glm::vec3 a_der(3.0f, 0.0f, 14.0f);
    glm::vec3 b_izq(-3.0f, 0.0f, -12.0f);
    glm::vec3 b_der(3.0f, 0.0f, -12.0f);
    glm::vec3 c_izq(-3.0f, 5.0f, -12.0f);
    glm::vec3 c_der(3.0f, 5.0f, -12.0f);
    glm::vec3 d_izq(-3.0f, 5.0f, 14.0f);
    glm::vec3 d_der(3.0f, 5.0f, 14.0f);

    scene.add_object(std::make_shared<Triangle>(a_izq, b_izq, c_izq, wall_mat));
    scene.add_object(std::make_shared<Triangle>(a_der, c_der, d_der, wall_mat));
    scene.add_object(std::make_shared<Triangle>(a_der, b_der, c_der, wall_mat));
    scene.add_object(std::make_shared<Triangle>(a_izq, c_izq, d_izq, wall_mat));

    scene.add_light(Light(glm::vec3(0.0f, 9.0f, 5.0f),
    glm::vec3(4.0f, 4.0f, 4.0f),
    glm::vec3(1.0f, 0.12f, 0.02f)));








}