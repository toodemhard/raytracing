#pragma once

#include "color.h"
#include "ray.h"
#include "vec3.h"

#include "hittable.h"

class HitRecord;
class Material {
  public:
    virtual ~Material() = default;

    virtual bool
    scatter(const Ray& r_in, const HitRecord& rec, Color& attenuation, Ray& scattered) const = 0;
};

class Lambertian : public Material {
  public:
    Lambertian(const Color& albedo) : albedo(albedo) {}

    bool scatter(const Ray& r_in, const HitRecord& rec, Color& attenuation, Ray& scattered)
        const override {
        auto scattered_direction = rec.normal + random_unit_vector();

        if (scattered_direction.near_zero()) {
            scattered_direction = rec.normal;
        }

        scattered = Ray(rec.p, scattered_direction);
        attenuation = albedo;
        return true;
    }

  private:
    Color albedo;
};

class Metallic : public Material {
  public:
    Metallic(const Color& albedo) : albedo(albedo) {}

    bool scatter(const Ray& r_in, const HitRecord& rec, Color& attenuation, Ray& scattered)
        const override {
        auto reflected_dir = reflect(r_in.direction(), rec.normal);

        scattered = Ray(rec.p, reflected_dir);
        attenuation = albedo;
        return true;
    }

  private:
    Color albedo;
};
