
#include "light.h"
#include "camera.h"
#include "shape.h"

Light::Light(const Vector &cente, unsigned char *colo) : center(cente) {
  color = colo;
}

unsigned char *Light::getColor(unsigned char a, unsigned char b,
                               unsigned char c) {
  unsigned char *r = (unsigned char *)malloc(sizeof(unsigned char) * 3);
  r[0] = a;
  r[1] = b;
  r[2] = c;
  return r;
}

Autonoma::Autonoma(const Camera &c) : camera(c) {
  listStart = NULL;
  listEnd = NULL;
  lightStart = NULL;
  lightEnd = NULL;
  depth = 10;
  skybox = BLACK;
}

Autonoma::Autonoma(const Camera &c, Texture *tex) : camera(c) {
  listStart = NULL;
  listEnd = NULL;
  lightStart = NULL;
  lightEnd = NULL;
  depth = 10;
  skybox = tex;
}

void Autonoma::addShape(Shape *r) {
  ShapeNode *hi = (ShapeNode *)malloc(sizeof(ShapeNode));
  hi->data = r;
  hi->next = hi->prev = NULL;
  if (listStart == NULL) {
    listStart = listEnd = hi;
  } else {
    listEnd->next = hi;
    hi->prev = listEnd;
    listEnd = hi;
  }
}

void Autonoma::removeShape(ShapeNode *s) {
  if (s == listStart) {
    if (s == listEnd) {
      listStart = NULL;
      listStart = NULL;
    } else {
      listStart = s->next;
      listStart->prev = NULL;
    }
  } else if (s == listEnd) {
    listEnd = s->prev;
    listEnd->next = NULL;
  } else {
    ShapeNode *b4 = s->prev, *aft = s->next;
    b4->next = aft;
    aft->prev = b4;
  }
  free(s);
}

void Autonoma::addLight(Light *r) {
  LightNode *hi = (LightNode *)malloc(sizeof(LightNode));
  hi->data = r;
  hi->next = hi->prev = NULL;
  if (lightStart == NULL) {
    lightStart = lightEnd = hi;
  } else {
    lightEnd->next = hi;
    hi->prev = lightEnd;
    lightEnd = hi;
  }
}

void Autonoma::removeLight(LightNode *s) {
  if (s == lightStart) {
    if (s == lightEnd) {
      lightStart = NULL;
      lightStart = NULL;
    } else {
      lightStart = s->next;
      lightStart->prev = NULL;
    }
  } else if (s == lightEnd) {
    lightEnd = s->prev;
    lightEnd->next = NULL;
  } else {
    LightNode *b4 = s->prev, *aft = s->next;
    b4->next = aft;
    aft->prev = b4;
  }
  free(s);
}

void getLight(double *tColor, Autonoma *aut, Vector point, Vector norm,
              unsigned char flip) {
  tColor[0] = tColor[1] = tColor[2] = 0.;
  LightNode *t = aut->lightStart;
  while (t != NULL) {
    double lightColor[3];
    lightColor[0] = t->data->color[0] / 255.;
    lightColor[1] = t->data->color[1] / 255.;
    lightColor[2] = t->data->color[2] / 255.;
    Vector ra = t->data->center - point;
    ShapeNode *shapeIter = aut->listStart;

    // keeps going until hit... ?
    // should just pas back object from the outer call. what are we doing.
    bool hit = false;
    while (!hit && shapeIter != NULL) {
      hit = shapeIter->data->getLightIntersection(Ray(point + ra * .01, ra),
                                                  lightColor);
      shapeIter = shapeIter->next;
    }
    double perc = (norm.dot(ra) / (ra.mag() * norm.mag()));
    if (!hit) {
      if (flip && perc < 0)
        perc = -perc;
      if (perc > 0) {
        tColor[0] += perc * (lightColor[0]);
        tColor[1] += perc * (lightColor[0]);
        tColor[2] += perc * (lightColor[0]);
        if (tColor[0] > 1.)
          tColor[0] = 1.;
        if (tColor[1] > 1.)
          tColor[1] = 1.;
        if (tColor[2] > 1.)
          tColor[2] = 1.;
      }
    }
    t = t->next;
  }
}


struct {
    bool operator()(SizedShape a, SizedShape b) const {
      return a.center.x > b.center.x;
    }
} x_extractor;

struct {
    bool operator()(SizedShape a, SizedShape b) const {
      return a.center.y > b.center.y;
    }
} y_extractor;

struct {
    bool operator()(SizedShape a, SizedShape b) const {
      return a.center.z > b.center.z;
    }
} z_extractor;

BVHNode* buildTree(std::vector<SizedShape> list) {
  Vector superbox[2] = { list[0].box[0], list[0].box[1] };
  for (auto shape: list) {
    superbox[0].x = std::min(shape.box[0].x, superbox[0].x);
    superbox[0].y = std::min(shape.box[0].y, superbox[0].y);
    superbox[0].z = std::min(shape.box[0].z, superbox[0].z);

    superbox[1].x = std::max(shape.box[1].x, superbox[1].x);
    superbox[1].y = std::max(shape.box[1].y, superbox[1].y);
    superbox[1].z = std::max(shape.box[1].z, superbox[1].z);
  }

  const unsigned int NODE_MAX_OBJ = 3;
  if (list.size() > NODE_MAX_OBJ) {
    // decide which extent
    Vector extent = superbox[1] - superbox[0];
    int splaxis = ((extent.z > extent.x) && (extent.z > extent.y))? 2: ((extent.x > extent.y) ?  0 : 1);

    // auto partition = list.begin();
    if (splaxis == 0) {
      std::sort(list.begin(), list.begin() + list.size(), x_extractor);
      // partition = std::lower_bound(list.begin(), list.begin()+list.size(),
      //                              SizedShape{ NULL, superbox[0] + (extent/2),
      //                                          { Vector(), Vector()} }, x_extractor);
    } else if (splaxis == 1) {
      std::sort(list.begin(), list.begin() + list.size(), y_extractor);
      // partition = std::lower_bound(list.begin(), list.begin()+list.size(),
      //                              SizedShape{ NULL, superbox[0] + (extent/2),
      //                                          { Vector(), Vector()} }, y_extractor);
    } else {
      std::sort(list.begin(), list.begin() + list.size(), z_extractor);
      // partition = std::lower_bound(list.begin(), list.begin()+list.size(),
      //                              SizedShape{ NULL, superbox[0] + (extent/2),
      //                                          { Vector(), Vector()} }, z_extractor);
    }

    // avoid weird recursive edgecase
    // if (partition == list.end()) {
    //   partition--;
    // }
    // if (partition == list.begin()) {
    //   partition++;
    // }

    auto partition = list.begin() + (list.size()/2);

    std::vector<SizedShape> before(list.begin(), partition), after(partition, list.end());

    BVHNode *l, *r;

    l = buildTree(before);
    r = buildTree(after);

    return new BVHNode{
      { superbox[0], superbox[1] }, false, std::vector<Shape*>(), l, r
    };
  } else {
    std::vector<Shape*> shapes;
    for (const auto& node: list) shapes.push_back(node.shape);
    // early exit -- just construct a node with superbox and dipe
    return new BVHNode{
      { superbox[0], superbox[1] }, true, shapes, NULL, NULL
    };
  }
}

void freeTree(BVHNode* node) {
  if (node == NULL) return;
  freeTree(node->left);
  freeTree(node->right);

  delete node;
}

inline double slabIsect(const Ray &r, Vector box[2]) {
  double xt0 = (box[0].x - r.point.x)/r.vector.x, xt1 = (box[1].x - r.point.x)/r.vector.x;
  double yt0 = (box[0].y - r.point.y)/r.vector.y, yt1 = (box[1].y - r.point.y)/r.vector.y;
  double zt0 = (box[0].z - r.point.z)/r.vector.z, zt1 = (box[1].z - r.point.z)/r.vector.z;

  if (xt0 > xt1) std::swap(xt0, xt1);
  if (yt0 > yt1) std::swap(yt0, yt1);
  if (zt0 > zt1) std::swap(zt0, zt1);

  double lower = std::max(xt0, std::max(yt0, zt0));
  double upper = std::min(xt1, std::min(yt1, zt1));

  if (lower > upper) return inf;
  if (upper < 0) return inf;

  return std::max(lower, 0.0);
}

TimeAndShape isectTree(BVHNode* node, Ray &r) {
  if (node->is_shape) {
    // return min time over isection with objects
    TimeAndShape mintime = { inf, NULL };
    for (auto shape: node->shapes) {
      double time = shape->getIntersection(r);
      if (time < mintime.time) {
        mintime = { time, shape };
      }
    }
    return mintime;
  } else {
    // dispatch into left or right (or return inf)
    double left_time = slabIsect(r, node->left->box);
    double right_time = slabIsect(r, node->right->box);

    if (left_time == inf && right_time == inf) return { inf, NULL };

    BVHNode* near = left_time < right_time ? node->left : node->right;
    BVHNode* far  = left_time < right_time ? node->right : node->left;
    double othertime = left_time < right_time ? right_time : left_time;

    TimeAndShape time = isectTree(near, r);
    if (time.time <= othertime) return time;

    TimeAndShape otherhit = isectTree(far, r);
    return time.time <= otherhit.time ? time : otherhit;
  }
}

bool isectLightTree(BVHNode* node, Ray &r, double *fill) {
  if (node->is_shape) {
    // return min time over isection with objects
    for (auto shape: node->shapes) {
      if(shape->getLightIntersection(r, fill))
        return true;
    }
    return false;
  } else {
    // dispatch into left or right (or return inf)
    double left_time = slabIsect(r, node->left->box);
    double right_time = slabIsect(r, node->right->box);

    if (left_time == inf && right_time == inf) return false;

    BVHNode* near = left_time < right_time ? node->left : node->right;
    BVHNode* far  = left_time < right_time ? node->right : node->left;
    double othertime = left_time < right_time ? right_time : left_time;

    return (isectLightTree(near, r, fill) || (othertime != inf && isectLightTree(far, r, fill)));
  }
}
