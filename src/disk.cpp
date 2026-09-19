#include "disk.h"
Disk::Disk(const Vector &c, Texture *t, double ya, double pi, double ro,
           double tx, double ty)
    : Plane(c, t, ya, pi, ro, tx, ty) {}

double Disk::getIntersection(Ray ray) {
  double time = Plane::getIntersection(ray);
  if (time == inf)
    return time;
  Vector dist = projectBasis(ray.point + ray.vector * time - center);
  return (dist.x * dist.x / (textureX * textureX) +
              dist.y * dist.y / (textureY * textureY) >
          1)
             ? inf
             : time;
}

bool Disk::getLightIntersection(Ray ray, double *fill) {
  const double t = ray.vector.dot(vect);
  const double norm = vect.dot(ray.point) + d;
  const double r = -norm / t;
  if (r <= 0. || r >= 1.)
    return false;
  Vector dist = projectBasis(ray.point + ray.vector * r - center);
  if (texture->opacity > 1 - 1E-6)
    return true;
  unsigned char temp[4];
  double amb, op, ref;

  if (dist.x * dist.x / (textureX * textureX) +
          dist.y * dist.y / (textureY * textureY) >
      1)
    return false;

  // fix arguments, before offset by .5, are guaranteed to be [-1, 1]
  // conditional opts will be slower than just doing this normally
  texture->getColor(temp, &amb, &op, &ref, fix(dist.x / textureX - .5),
                    fix(dist.y / textureY - .5));
  if (op > 1 - 1E-6)
    return true;
  fill[0] *= temp[0] / 255.;
  fill[1] *= temp[1] / 255.;
  fill[2] *= temp[2] / 255.;
  return false;
}

// bounding box of an ellipse:
// https://web.archive.org/web/20120620045116/https://iquilezles.org/www/articles/ellipses/ellipses.htm
std::array<Vector, 2> Disk::getBoundingBox() {
  Vector up_s = up * textureY;
  Vector right_s = right * textureX;
  Vector extent = Vector(sqrt(up_s.x * up_s.x + right_s.x * right_s.x),
                         sqrt(up_s.y * up_s.y + right_s.y * right_s.y),
                         sqrt(up_s.z * up_s.z + right_s.z * right_s.z));

  return { center - extent, center + extent };
}
