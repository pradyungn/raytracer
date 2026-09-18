#ifndef __LIGHT_H__
#define __LIGHT_H__
#include "vector.h"
#include "camera.h"
#include "Textures/texture.h"
#include "Textures/colortexture.h"
#include <array>
#include <vector>

class Light{
  public:
   unsigned char* color;
   unsigned char* getColor(unsigned char a, unsigned char b, unsigned char c);
   Vector center;
   Light(const Vector & cente, unsigned char* colo);
};

struct LightNode{
   Light* data;
   LightNode* prev, *next;
};

class Shape;
struct ShapeNode{
   Shape* data;
   ShapeNode* prev, *next;
};

struct SizedShape {
  Shape* shape;
  Vector center;
  std::array<Vector, 2> box;
};

struct BVHNode {
  // bounding box
  Vector box[2];

  // is this a literal shape?
  bool is_shape;
  std::vector<Shape*> shapes;

  // if not a shape, we branch again
  BVHNode *left, *right;
};

class Autonoma{
public:
   Camera camera;
   Texture* skybox;
   unsigned int depth;
   ShapeNode *listStart, *listEnd;
   LightNode *lightStart, *lightEnd;
   std::vector<Shape*> planes;
   BVHNode* shapeTree;
   Autonoma(const Camera &c);
   Autonoma(const Camera &c, Texture* tex);
   void addShape(Shape* s);
   void removeShape(ShapeNode* s);
   void addLight(Light* s);
   void removeLight(LightNode* s);
};

BVHNode* buildTree(std::vector<SizedShape> list);
void freeTree(BVHNode* node);
void getLight(double* toFill, Autonoma* aut, Vector point, Vector norm, unsigned char r);

#endif
