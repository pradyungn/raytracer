#include "triangle.h"

Triangle::Triangle(Vector c, Vector b, Vector a, Texture *t)
    : Plane(Vector(0, 0, 0), t, 0., 0., 0., 0., 0.) {
  center = c;
  Vector righta = (b - c);
  textureX = righta.mag();
  right = righta / textureX;
  vect = right.cross(b - a).normalize();

  bmc = righta;
  amc = a - c;

  xsin = -right.z;
  if (xsin < -1.)
    xsin = -1;
  else if (xsin > 1.)
    xsin = 1.;
  yaw = asin(xsin);
  xcos = sqrt(1. - xsin * xsin);

  zcos = right.x / xcos;
  zsin = -right.y / xcos;
  if (zsin < -1.)
    zsin = -1;
  else if (zsin > 1.)
    zsin = 1.;
  if (zcos < -1.)
    zcos = -1;
  else if (zcos > 1.)
    zcos = 1.;
  roll = asin(zsin);

  ycos = vect.z / xcos;
  if (ycos < -1.)
    ycos = -1;
  else if (ycos > 1.)
    ycos = 1.;
  pitch = acos(ycos);
  ysin = sqrt(1 - ycos * ycos);

  up.x = -xsin * ysin * zcos + ycos * zsin;
  up.y = ycos * zcos + xsin * ysin * zsin;
  up.z = -xcos * ysin;
  Vector np = solveScalers(right, up, vect, amc);
  textureY = np.y;
  thirdX = np.x;

  d = -vect.dot(center);

  // need to rerun this, because Plane constructor was dummy'd
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

// replaced w/ the Moller-Trumbore intersection algo
// some C++ code pulled out of the wikipedia page
double Triangle::getIntersection(Ray ray) {
  const double eps = 1e-9;
  Vector ray_xamc = ray.vector.cross(amc);
  double det = bmc.dot(ray_xamc);

  // dropped backface culling, make output "patchy"

  // check if parallel
  if (det < eps && det > -eps)
    return inf;

  double idet = 1.0 / det;
  Vector s = ray.point - center;
  double u = idet * s.dot(ray_xamc);

  if (u < -eps || u - 1 > eps)
    return inf;

  Vector s_xbmc = s.cross(bmc);
  double v = idet * ray.vector.dot(s_xbmc);

  if (v < -eps || u + v - 1 > eps)
    return inf;

  double t = idet * amc.dot(s_xbmc);
  return (t > eps) ? t : inf;
}

bool Triangle::getLightIntersection(Ray ray, double *fill) {
  const double t = ray.vector.dot(vect);
  const double norm = vect.dot(ray.point) + d;
  const double r = -norm / t;
  if (r <= 0. || r >= 1.)
    return false;
  Vector dist = cached_cramers(quadrant_dets, basis_det,
                               ray.point + ray.vector * r - center);

  unsigned char tmp =
      (thirdX - dist.x) * textureY + (thirdX - textureX) * (dist.y - textureY) <
      0.0;
  if ((tmp != (textureX * dist.y < 0.0)) ||
      (tmp != (dist.x * textureY - thirdX * dist.y < 0.0)))
    return false;

  if (texture->opacity > 1 - 1E-6)
    return true;
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
