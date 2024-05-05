#pragma once

#include "interval.h"
#include "ray.h"
#include <memory>

class Material;

class HitRecord {
  public:
    Point3 p;
    Vec3 normal;
    std::shared_ptr<Material> mat;
    double t;
    bool front_face;

    void set_face_normal(const Ray& r, const Vec3& outward_normal) {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class Hittable {
  public:
    virtual bool hit(const Ray& r, const Interval ray_t, HitRecord& rec) const = 0;
};
