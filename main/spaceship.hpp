#ifndef SPACESHIP_H
#define SPACESHIP_H
#include <vector>

#include <cstdlib>

#include "simple_mesh.hpp"

#include "../vmlib/vec3.hpp"
#include "../vmlib/mat44.hpp"
// This defines the vertex data for a colored unit cube.
SimpleMeshData make_spaceship(Mat44f aPreTransform = kIdentity44f);
#endif