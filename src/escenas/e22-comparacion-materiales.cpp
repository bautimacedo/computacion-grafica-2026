#include <memory>

#include <escenas.h>
#include <plane.h>
#include <sphere.h>
#include <checker.h>
#include <mirror.h>
#include <dielectric.h>

void build_scene_22(Scene& scene, Camera& camera)
{
    camera.look_at(glm::vec3(0.0f, 2.5f, 8.0f),
                    glm::vec3(0.0f, 1.0f, 0.0f),
                    glm::vec3(0.0f, 1.0f, 0.0f),
                    45.0f);

    scene.set_background(glm::vec3(0.5f, 0.6f, 0.75f), glm::vec3(0.1f, 0.15f, 0.3f));

    auto floor = std::make_shared<Checker>();
    floor->ka  = glm::vec3(0.05f, 0.05f, 0.05f);
    floor->kd  = glm::vec3(0.75f, 0.75f, 0.75f);
    floor->ka2 = glm::vec3(0.01f, 0.01f, 0.01f);
    floor->kd2 = glm::vec3(0.1f, 0.1f, 0.1f);
    floor->ks  = glm::vec3(0.0f);
    floor->size = 1.0f;
    scene.add_object(std::make_shared<Plane>(glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f), floor));

    // Misma geometria (radio 1, misma altura), solo cambia el material
    auto difusa = std::make_shared<Diffuse>();
    difusa->ka = glm::vec3(0.02f, 0.01f, 0.08f);
    difusa->kd = glm::vec3(0.2f, 0.1f, 0.8f);
    difusa->ks = glm::vec3(0.4f, 0.4f, 0.4f);
    difusa->shininess = 50.0f;
    scene.add_object(std::make_shared<Sphere>(glm::vec3(-3.0f, 1.0f, 0.0f), 1.0f, difusa));

    auto espejo = std::make_shared<Mirror>();
    espejo->kr = 0.85f;
    espejo->ks = glm::vec3(1.0f);
    espejo->shininess = 120.0f;
    scene.add_object(std::make_shared<Sphere>(glm::vec3(0.0f, 1.0f, 0.0f), 1.0f, espejo));

    auto vidrio = std::make_shared<Dielectric>();
    vidrio->ior = 1.5f;
    vidrio->transparency = 0.85f;
    vidrio->ks = glm::vec3(1.0f);
    vidrio->shininess = 120.0f;
    scene.add_object(std::make_shared<Sphere>(glm::vec3(3.0f, 1.0f, 0.0f), 1.0f, vidrio));

    scene.add_light(Light(glm::vec3(0.0f, 6.0f, 4.0f),
                           glm::vec3(3.2f, 3.2f, 3.2f),
                           glm::vec3(1.0f, 0.12f, 0.02f)));
}
