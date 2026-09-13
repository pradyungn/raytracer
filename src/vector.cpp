#include <limits>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <printf.h>
#include "vector.h"
#include <stddef.h>

Vector::Vector() : x(0), y(0), z(0) {}
Vector::Vector(double a, double b, double c) : x(a), y(b), z(c) {}
void Vector::operator-=(const Vector rhs) {
  x -= rhs.x;
  y -= rhs.y;
  z -= rhs.z;
}
void Vector::operator+=(const Vector rhs) {
  x += rhs.x;
  y += rhs.y;
  z += rhs.z;
}
void Vector::operator*=(const double rhs) {
  x *= rhs;
  y *= rhs;
  z *= rhs;
}
void Vector::operator*=(const float rhs) {
  x *= rhs;
  y *= rhs;
  z *= rhs;
}
void Vector::operator*=(const int rhs) {
  x *= rhs;
  y *= rhs;
  z *= rhs;
}
void Vector::operator/=(const double rhs) {
  x /= rhs;
  y /= rhs;
  z /= rhs;
}
void Vector::operator/=(const float rhs) {
  x /= rhs;
  y /= rhs;
  z /= rhs;
}
void Vector::operator/=(const int rhs) {
  x /= rhs;
  y /= rhs;
  z /= rhs;
}

Vector Vector::operator-(const Vector rhs) {
  return Vector(x - rhs.x, y - rhs.y, z - rhs.z);
}
Vector Vector::operator+(const Vector rhs) {
  return Vector(x + rhs.x, y + rhs.y, z + rhs.z);
}
/*
Vector Vector::operator * (const Vector a) {
   return Vector(y*a.z-z*a.y, z*a.x-x*a.z, x*a.y-y*a.x);
}*/
Vector Vector::operator*(const double rhs) {
  return Vector(x * rhs, y * rhs, z * rhs);
}
Vector Vector::operator*(const float rhs) {
  return Vector(x * rhs, y * rhs, z * rhs);
}
Vector Vector::operator*(const int rhs) {
  return Vector(x * rhs, y * rhs, z * rhs);
}
Vector Vector::operator/(const double rhs) {
  return Vector(x / rhs, y / rhs, z / rhs);
}
Vector Vector::operator/(const float rhs) {
  return Vector(x / rhs, y / rhs, z / rhs);
}
Vector Vector::operator/(const int rhs) {
  return Vector(x / rhs, y / rhs, z / rhs);
}
Vector Vector::cross(const Vector a) {
  return Vector(y * a.z - z * a.y, z * a.x - x * a.z, x * a.y - y * a.x);
}
double Vector::mag2() { return x * x + y * y + z * z; }
double Vector::mag() { return sqrt(x * x + y * y + z * z); }
double Vector::dot(const Vector a) { return x * a.x + y * a.y + z * a.z; }
Vector Vector::normalize() {
  double m = mag();
  return Vector(x / m, y / m, z / m);
}

// Claude says that this is Cramer's rule?
// Solves the system ax + by + cz = K, returning a vector of (a, b, c)
// Check assembly dump -- is this reusing internal products at all?
Vector solveScalers(Vector v1, Vector v2, Vector v3, Vector C) {
  double denom = v1.z * v2.y * v3.x - v1.y * v2.z * v3.x - v1.z * v2.x * v3.y +
                 v1.x * v2.z * v3.y + v1.y * v2.x * v3.z - v1.x * v2.y * v3.z;

  double a = C.z * v2.y * v3.x - C.y * v2.z * v3.x - C.z * v2.x * v3.y +
             C.x * v2.z * v3.y + C.y * v2.x * v3.z - C.x * v2.y * v3.z;
  double b = -C.z * v1.y * v3.x + C.y * v1.z * v3.x + C.z * v1.x * v3.y -
             C.x * v1.z * v3.y - C.y * v1.x * v3.z + C.x * v1.y * v3.z;
  double c = C.z * v1.y * v2.x - C.y * v1.z * v2.x - C.z * v1.x * v2.y +
             C.x * v1.z * v2.y + C.y * v1.x * v2.z - C.x * v1.y * v2.z;

  return Vector(a / denom, b / denom, c / denom);
}

Vector cached_cramers(double *quadrant_dets, double basis_det, Vector C) {
  double a1 =
      quadrant_dets[1] * C.z + quadrant_dets[4] * C.x + quadrant_dets[7] * C.y;
  double a2 =
      quadrant_dets[2] * C.z + quadrant_dets[5] * C.x + quadrant_dets[8] * C.y;
  double a3 =
      quadrant_dets[0] * C.z + quadrant_dets[3] * C.x + quadrant_dets[6] * C.y;
  return Vector(a1 / basis_det, a2 / basis_det, a3 / basis_det);
}

Ray::Ray(const Vector &po, const Vector &ve) : point(po), vector(ve) {}
