#include "sphere.h"

Sphere::Sphere(const Vector &c, Texture *t, double ya, double pi, double ro,
               double rad)
    : Shape(c, t, ya, pi, ro) {
  mapX = mapY = textureX = textureY = 1.;
  normalMap = NULL;
  radius = rad;
}
bool Sphere::getLightIntersection(Ray ray, double *fill) {
  const double A = ray.vector.mag2();
  const double B = 2 * ray.vector.dot(ray.point - center);
  const double C = (ray.point - center).mag2() - radius * radius;
  const double descriminant = B * B - 4 * A * C;
  if (descriminant < 0. || descriminant < B * ((B >= 0) ? B : -B))
    return false;
  const double desc = sqrt(descriminant);
  const double root1 = (-B - desc) / (2 * A);
  const double root2 = (-B + desc) / (2 * A);
  const double time = (root1 > 0) ? root1 : root2;
  if (time >= 1.)
    return false;

  Vector point = ray.point + ray.vector * time;

  // bugfix: these were swapped in original code!
  double data3 = (center.y - point.y + radius) / (2 * radius);
  double data2 = atan2(point.z - center.z, point.x - center.x);
  unsigned char temp[4];
  double amb, op, ref;

  double getx = (yaw + data2) / M_TWO_PI;
  if (getx < 0.0) getx += 1.0;

  double gety = fix(pitch / M_TWO_PI - data3);

  texture->getColor(temp, &amb, &op, &ref,
                    getx, gety);

  if (op > 1 - 1E-6)
    return true;

  fill[0] *= temp[0] / 255.;
  fill[1] *= temp[1] / 255.;
  fill[2] *= temp[2] / 255.;
  return false;
}
double Sphere::getIntersection(Ray ray) {
  const double A = ray.vector.mag2();
  const double B = 2 * ray.vector.dot(ray.point - center);
  const double C = (ray.point - center).mag2() - radius * radius;
  const double descriminant = B * B - 4 * A * C;
  if (descriminant < 0)
    return inf;
  else {
    const double desc = sqrt(descriminant);
    const double root1 = (-B - desc) / (2 * A);
    const double root2 = (-B + desc) / (2 * A);
    return (root1 > 0) ? (root1) : ((root2 > 0) ? root2 : inf);
  }
}
void Sphere::move() { return; }
unsigned char Sphere::reversible() { return 0; }

void Sphere::getColor(unsigned char *toFill, double *amb, double *op,
                      double *ref, [[maybe_unused]] Autonoma *r, Ray ray,
                      [[maybe_unused]] unsigned int depth) {
  double data3 = (center.y - ray.point.y + radius) / (2 * radius);
  double data2 = atan2(ray.point.z - center.z, ray.point.x - center.x);

  double getx = (yaw + data2) / M_TWO_PI;
  if (getx < 0.0) getx += 1.0;

  double gety = fix(pitch / M_TWO_PI - data3);

  texture->getColor(toFill, amb, op, ref,
                    getx, gety);
}

// only called on COLLIDED objects
Vector Sphere::getNormal(Vector point) {
  Vector vect = point - center;
  /*   A x B = <x, y, z>
  <ay bz- az by,  bz ax - az bx, ax by - bx ay>
  az = 0

  <ay bz,  bz ax, ax by - bx ay >

  bx = 0

  <ay bz,  bz ax, ax by >

  ax = 1

  <ay bz,  bz , by >

  B: <0, z, -y>
  A <-1,x/y,0>
  */
  if (normalMap == NULL)
    return vect;

  // point is on sphere surface, so difference is at max radius
  double data3 = (center.y - point.y + radius) / (2 * radius);
  double data2 = atan2(point.z - center.z, point.x - center.x);
  vect = vect.normalize();
  Vector right = Vector(vect.x, vect.z, -vect.y);
  Vector up = Vector(vect.z, vect.y, -vect.x);
  double am, ref, op;
  unsigned char norm[3];

  double getx = ((mapOffX + mapOffX) + data2) / M_TWO_PI / mapX;
  if (getx >= 1.0) getx -= 1;
  else if (getx < 0.0) getx += 1;

  double gety = ((mapOffY) / M_PI - data3) / mapY;
  if (gety < 0.0) gety += 1;

  normalMap->getColor(norm, &am, &op, &ref,
                      getx, gety);
  return ((norm[0] - 128) * right + (norm[1] - 128) * up + norm[2] * vect)
      .normalize();
}

void Sphere::setAngles(double a, double b, double c) {
  yaw = a;
  pitch = b;
  roll = c;
  xcos = cos(yaw);
  xsin = sin(yaw);
  ycos = cos(pitch);
  ysin = sin(pitch);
  zcos = cos(roll);
  zsin = sin(roll);
}

void Sphere::setYaw(double a) {
  yaw = a;
  xcos = cos(yaw);
  xsin = sin(yaw);
}

void Sphere::setPitch(double b) {
  pitch = b;
  ycos = cos(pitch);
  ysin = sin(pitch);
}

void Sphere::setRoll(double c) {
  roll = c;
  zcos = cos(roll);
  zsin = sin(roll);
}

std::array<Vector, 2> Sphere::getBoundingBox() {
  Vector extent = Vector(radius, radius, radius);
  return { center - extent, center + extent};
}
