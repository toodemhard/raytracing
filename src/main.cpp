#include <cmath>
#include <memory>
#include <vector>

#include "camera.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"
#include "vec3.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

int main() {
    HittableList world;
    auto material_ground = std::make_shared<Lambertian>(Color(0.8, 0.8, 0.0));
    auto material_center = std::make_shared<Lambertian>(Color(0.1, 0.2, 0.5));
    auto material_left = std::make_shared<Metallic>(Color(0.8, 0.8, 0.8));
    auto material_right = std::make_shared<Metallic>(Color(0.8, 0.6, 0.2));

    world.add(make_shared<Sphere>(Point3(0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<Sphere>(Point3(0.0, 0.0, -1.2), 0.5, material_center));
    world.add(make_shared<Sphere>(Point3(-1.0, 0.0, -1.0), 0.5, material_left));
    world.add(make_shared<Sphere>(Point3(1.0, 0.0, -1.0), 0.5, material_right));

    Camera cam;
    cam.aspect_ratio = 4.0 / 3.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 10;

    auto image = cam.render(world);

    auto [image_width, image_height] = cam.dimensions();

    stbi_write_png("image.png", image_width, image_height, 3, image.data(), image_width * 3);
    return 0;
}
