#pragma once

#include "color.h"
#include "hittable.h"
#include "material.h"
#include "vec3.h"
#include <array>
#include <cstdint>
#include <format>
#include <iostream>
#include <vector>

class Camera {
  public:
    double aspect_ratio = 1;
    int image_width = 100;
    int samples_per_pixel = 10;
    int max_depth = 10;

    std::vector<uint8_t> render(const Hittable& world) {
        initialize();

        std::vector<uint8_t> image_data(image_height * image_width * 3);

        int bar_length = 20;
        for (int j = 0; j < image_height; j++) {
            double progress = double(j + 1) / double(image_height);

            std::string thing(int(progress * bar_length), '#');
            std::string empty(int((1 - progress) * bar_length), ' ');
            std::clog << std::format("\r[{}{}] {}%            ", thing, empty, progress * 100)
                      << std::flush;

            for (int i = 0; i < image_width; i++) {
                Color pixel_color;
                for (int sample = 0; sample < samples_per_pixel; sample++) {
                    Ray r = get_ray(i, j);
                    pixel_color += ray_color(r, max_depth, world);
                }

                pixel_color *= pixel_samples_scale;
                auto r = pixel_color.x();
                auto g = pixel_color.y();
                auto b = pixel_color.z();

                r = linear_to_gamma(r);
                g = linear_to_gamma(g);
                b = linear_to_gamma(b);

                static const Interval intensity(0.000, 0.999);
                uint8_t rbyte = uint8_t(255.999 * intensity.clamp(r));
                uint8_t gbyte = uint8_t(255.999 * intensity.clamp(g));
                uint8_t bbyte = uint8_t(255.999 * intensity.clamp(b));
                image_data[3 * (j * image_width + i)] = rbyte;
                image_data[3 * (j * image_width + i) + 1] = gbyte;
                image_data[3 * (j * image_width + i) + 2] = bbyte;
            }
        }

        return image_data;
    }

    std::array<int, 2> dimensions() {
        return std::array<int, 2>{image_width, image_height};
    }

  private:
    int image_height;
    double pixel_samples_scale;
    Point3 center;
    Vec3 pixel00_loc;
    Vec3 pixel_delta_u;
    Vec3 pixel_delta_v;
    void initialize() {
        image_height = int(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;
        center = Point3(0, 0, 0);

        pixel_samples_scale = 1.0 / samples_per_pixel;

        double focal_length = 1.0;
        double viewport_height = 2.0;
        double viewport_width = viewport_height * (double(image_width) / image_height);

        auto viewport_u = Vec3(viewport_width, 0, 0);
        auto viewport_v = Vec3(0, -viewport_height, 0);

        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        Vec3 viewport_upper_left =
            center - Vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
    }

    Ray get_ray(int i, int j) const {
        auto offset = sample_square();
        auto pixel_center =
            pixel00_loc + ((i + offset.x()) * pixel_delta_u) + ((j + offset.y()) * pixel_delta_v);

        auto ray_direction = pixel_center - center;
        return Ray(center, ray_direction);
    }

    Vec3 sample_square() const {
        return Vec3(random_double() - 0.5, random_double() - 0.5, 0);
    }

    Color ray_color(const Ray& r, int depth, const Hittable& world) {
        if (depth <= 0) {
            return Color(0, 0, 0);
        }

        HitRecord rec;
        if (world.hit(r, Interval(0.001, infinity), rec)) {
            Ray scattered;
            Color attenuation;
            if (rec.mat->scatter(r, rec, attenuation, scattered)) {
                return attenuation * ray_color(scattered, depth - 1, world);
            }
            return Color(0, 0, 0);
        }

        Vec3 unit_dir = unit_vector(r.direction());
        auto a = 0.5 * (unit_dir.y() + 1.0);
        return (1.0 - a) * Color(1.0, 1.0, 1.0) + a * Color(0.5, 0.7, 1.0);
    }
};
