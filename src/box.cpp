#include "box.h"

Box::Box(const Vector &c, Texture *t, double ya, double pi, double ro,
         double tx, double ty)
    : Plane(c, t, ya, pi, ro, tx, ty) {}
Box::Box(const Vector &c, Texture *t, double ya, double pi, double ro,
         double tx)
    : Plane(c, t, ya, pi, ro, tx, tx) {}

double Box::getIntersection(Ray ray) {
  double time = Plane::getIntersection(ray);
  if (time == inf)
    return time;

  Vector dist = projectBasis(ray.point + ray.vector * time - center);
  return (std::fabs(dist.x) > textureX / 2 || std::fabs(dist.y) > textureY / 2)
             ? inf
             : time;
}

bool Box::getLightIntersection(Ray ray, double *fill) {
  const double t = ray.vector.dot(vect);
  const double norm = vect.dot(ray.point) + d;
  const double r = -norm / t;
  if (r <= 0. || r >= 1.)
    return false;
  Vector dist = projectBasis(
                               ray.point + ray.vector * r - center);
  if (std::fabs(dist.x) > textureX / 2 || std::fabs(dist.y) > textureY / 2)
    return false;

  if (texture->opacity > 1 - 1E-6)
    return true;
  unsigned char temp[4];
  double amb, op, ref;

  texture->getColor(temp, &amb, &op, &ref, dist.x / textureX + .5,
                    dist.y / textureY + .5);
  if (op > 1 - 1E-6)
    return true;
  fill[0] *= temp[0] / 255.;
  fill[1] *= temp[1] / 255.;
  fill[2] *= temp[2] / 255.;
  return false;
}

std::array<Vector, 2> Box::getBoundingBox() {
  Vector extent = Vector(std::max(std::fabs(up.x), std::fabs(right.x)),
                         std::max(std::fabs(up.y), std::fabs(right.y)),
                         std::max(std::fabs(up.z), std::fabs(right.z)));
  return { center - extent, center + extent };
}
