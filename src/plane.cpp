#include "plane.h"
#include <iostream>

Plane::Plane(const Vector &c, Texture *t, double ya, double pi, double ro,
             double tx, double ty)
    : Shape(c, t, ya, pi, ro), vect(c), right(c), up(c) {
  textureX = tx;
  textureY = ty;
  setAngles(yaw, pitch, roll);
  normalMap = NULL;
  mapX = textureX;
  mapY = textureY;

  quadrant_dets[0] = right.x * up.y - up.x * right.y;
  quadrant_dets[1] = up.x * vect.y - vect.x * up.y;
  quadrant_dets[2] = vect.x * right.y - right.x * vect.y;

  quadrant_dets[3] = right.y * up.z - up.y * right.z;
  quadrant_dets[4] = up.y * vect.z - vect.y * up.z;
  quadrant_dets[5] = vect.y * right.z - right.y * vect.z;

  quadrant_dets[6] = right.z * up.x - up.z * right.x;
  quadrant_dets[7] = up.z * vect.x - vect.z * up.x;
  quadrant_dets[8] = vect.z * right.x - right.z * vect.x;

  basis_det = quadrant_dets[0] * vect.z + quadrant_dets[3] * vect.x +
              quadrant_dets[6] * vect.y;
}

void Plane::setAngles(double a, double b, double c) {
  yaw = a;
  pitch = b;
  roll = c;
  xcos = cos(yaw);
  xsin = sin(yaw);
  ycos = cos(pitch);
  ysin = sin(pitch);
  zcos = cos(roll);
  zsin = sin(roll);
  vect.x = xsin * ycos * zcos + ysin * zsin;
  vect.y = ysin * zcos - xsin * ycos * zsin;
  vect.z = xcos * ycos;
  up.x = -xsin * ysin * zcos + ycos * zsin;
  up.y = ycos * zcos + xsin * ysin * zsin;
  up.z = -xcos * ysin;
  right.x = xcos * zcos;
  right.y = -xcos * zsin;
  right.z = -xsin;
  d = -vect.dot(center);

  quadrant_dets[0] = right.x * up.y - up.x * right.y;
  quadrant_dets[1] = up.x * vect.y - vect.x * up.y;
  quadrant_dets[2] = vect.x * right.y - right.x * vect.y;

  quadrant_dets[3] = right.y * up.z - up.y * right.z;
  quadrant_dets[4] = up.y * vect.z - vect.y * up.z;
  quadrant_dets[5] = vect.y * right.z - right.y * vect.z;

  quadrant_dets[6] = right.z * up.x - up.z * right.x;
  quadrant_dets[7] = up.z * vect.x - vect.z * up.x;
  quadrant_dets[8] = vect.z * right.x - right.z * vect.x;

  basis_det = quadrant_dets[0] * vect.z + quadrant_dets[3] * vect.x +
              quadrant_dets[6] * vect.y;
}

void Plane::setYaw(double a) {
  yaw = a;
  xcos = cos(yaw);
  xsin = sin(yaw);

  vect.x = xsin * ycos * zcos + ysin * zsin;
  vect.y = ysin * zcos - xsin * ycos * zsin;
  vect.z = xcos * ycos;
  up.x = -xsin * ysin * zcos + ycos * zsin;
  up.y = ycos * zcos + xsin * ysin * zsin;
  up.z = -xcos * ysin;
  right.x = xcos * zcos;
  right.y = -xcos * zsin;
  right.z = -xsin;
  d = -vect.dot(center);

  quadrant_dets[0] = right.x * up.y - up.x * right.y;
  quadrant_dets[1] = up.x * vect.y - vect.x * up.y;
  quadrant_dets[2] = vect.x * right.y - right.x * vect.y;

  quadrant_dets[3] = right.y * up.z - up.y * right.z;
  quadrant_dets[4] = up.y * vect.z - vect.y * up.z;
  quadrant_dets[5] = vect.y * right.z - right.y * vect.z;

  quadrant_dets[6] = right.z * up.x - up.z * right.x;
  quadrant_dets[7] = up.z * vect.x - vect.z * up.x;
  quadrant_dets[8] = vect.z * right.x - right.z * vect.x;

  basis_det = quadrant_dets[0] * vect.z + quadrant_dets[3] * vect.x +
              quadrant_dets[6] * vect.y;
}

void Plane::setPitch(double b) {
  pitch = b;
  ycos = cos(pitch);
  ysin = sin(pitch);
  vect.x = xsin * ycos * zcos + ysin * zsin;
  vect.y = ysin * zcos - xsin * ycos * zsin;
  vect.z = xcos * ycos;
  up.x = -xsin * ysin * zcos + ycos * zsin;
  up.y = ycos * zcos + xsin * ysin * zsin;
  up.z = -xcos * ysin;
  d = -vect.dot(center);

  quadrant_dets[0] = right.x * up.y - up.x * right.y;
  quadrant_dets[1] = up.x * vect.y - vect.x * up.y;
  quadrant_dets[2] = vect.x * right.y - right.x * vect.y;

  quadrant_dets[3] = right.y * up.z - up.y * right.z;
  quadrant_dets[4] = up.y * vect.z - vect.y * up.z;
  quadrant_dets[5] = vect.y * right.z - right.y * vect.z;

  quadrant_dets[6] = right.z * up.x - up.z * right.x;
  quadrant_dets[7] = up.z * vect.x - vect.z * up.x;
  quadrant_dets[8] = vect.z * right.x - right.z * vect.x;

  basis_det = quadrant_dets[0] * vect.z + quadrant_dets[3] * vect.x +
              quadrant_dets[6] * vect.y;
}

void Plane::setRoll(double c) {
  roll = c;
  zcos = cos(roll);
  zsin = sin(roll);
  vect.x = xsin * ycos * zcos + ysin * zsin;
  vect.y = ysin * zcos - xsin * ycos * zsin;
  //   vect.z = xcos*ycos;
  up.x = -xsin * ysin * zcos + ycos * zsin;
  up.y = ycos * zcos + xsin * ysin * zsin;
  // up.z = -xcos*ysin;
  right.x = xcos * zcos;
  right.y = -xcos * zsin;
  // right.z = -xsin;
  d = -vect.dot(center);

  quadrant_dets[0] = right.x * up.y - up.x * right.y;
  quadrant_dets[1] = up.x * vect.y - vect.x * up.y;
  quadrant_dets[2] = vect.x * right.y - right.x * vect.y;

  quadrant_dets[3] = right.y * up.z - up.y * right.z;
  quadrant_dets[4] = up.y * vect.z - vect.y * up.z;
  quadrant_dets[5] = vect.y * right.z - right.y * vect.z;

  quadrant_dets[6] = right.z * up.x - up.z * right.x;
  quadrant_dets[7] = up.z * vect.x - vect.z * up.x;
  quadrant_dets[8] = vect.z * right.x - right.z * vect.x;

  basis_det = quadrant_dets[0] * vect.z + quadrant_dets[3] * vect.x +
              quadrant_dets[6] * vect.y;
}

double Plane::getIntersection(Ray ray) {
  const double t = ray.vector.dot(vect);
  const double norm = vect.dot(ray.point) + d;

  const double r = -norm / t;
  return (r > 0) ? r : inf;
}

bool Plane::getLightIntersection(Ray ray, double *fill) {
  const double t = ray.vector.dot(vect);
  const double norm = vect.dot(ray.point) + d;
  const double r = -norm / t;
  if (r <= 0. || r >= 1.)
    return false;

  if (texture->opacity > 1 - 1E-6)
    return true;
  Vector dist = cached_cramers(quadrant_dets, basis_det, ray.point - center);
  unsigned char temp[4];
  double amb, op, ref;
  texture->getColor(temp, &amb, &op, &ref, fix(dist.x / textureX - .5),
                    fix(dist.y / textureY - .5));
  if (op > 1 - 1E-6)
    return true;
  fill[0] *= temp[0] / 255.;
  fill[1] *= temp[1] / 255.;
  fill[2] *= temp[2] / 255.;
  return false;
}

void Plane::move() { d = -vect.dot(center); }
void Plane::getColor(unsigned char *toFill, double *am, double *op, double *ref,
                     [[maybe_unused]] Autonoma *r, Ray ray,
                     [[maybe_unused]] unsigned int depth) {
  Vector dist = cached_cramers(quadrant_dets, basis_det, ray.point - center);
  texture->getColor(toFill, am, op, ref, fix(dist.x / textureX - .5),
                    fix(dist.y / textureY - .5));
}
unsigned char Plane::reversible() { return 1; }

Vector Plane::getNormal(Vector point) {
  if (normalMap == NULL)
    return vect;
  else {
    Vector dist = cached_cramers(quadrant_dets, basis_det, point - center);
    double am, ref, op;
    unsigned char norm[3];
    normalMap->getColor(norm, &am, &op, &ref, fix(dist.x / mapX - .5 + mapOffX),
                        fix(dist.y / mapY - .5 + mapOffY));
    Vector ret =
        ((norm[0] - 128) * right + (norm[1] - 128) * up + norm[2] * vect)
            .normalize();
    return ret;
  }
}

std::array<Vector, 2> Plane::getBoundingBox() {
  return {Vector(inf, inf, inf), Vector(inf, inf, inf)};
}
