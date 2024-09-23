#ifndef CUBE_HPP_6874B39C_112D_4D34_BD85_AB81A730955B
#define CUBE_HPP_6874B39C_112D_4D34_BD85_AB81A730955B
#include <vector>

#include <cstdlib>

#include "simple_mesh.hpp"

#include "../vmlib/vec3.hpp"
#include "../vmlib/mat44.hpp"
// This defines the vertex data for a colored unit cube.
SimpleMeshData make_cube(
	Vec3f aColor = { 1.f, 1.f, 1.f },
	Mat44f aPreTransform = kIdentity44f
);
float const kCubePositions[] = {
	+1.f, +1.f, -1.f,
	-1.f, +1.f, -1.f,
	-1.f, +1.f, +1.f,
	+1.f, +1.f, -1.f,
	-1.f, +1.f, +1.f,
	+1.f, +1.f, +1.f,

	+1.f, -1.f, +1.f,
	+1.f, +1.f, +1.f,
	-1.f, +1.f, +1.f,
	+1.f, -1.f, +1.f,
	-1.f, +1.f, +1.f,
	-1.f, -1.f, +1.f,

	-1.f, -1.f, +1.f,
	-1.f, +1.f, +1.f,
	-1.f, +1.f, -1.f,
	-1.f, -1.f, +1.f,
	-1.f, +1.f, -1.f,
	-1.f, -1.f, -1.f,

	-1.f, -1.f, -1.f,
	+1.f, -1.f, -1.f,
	+1.f, -1.f, +1.f,
	-1.f, -1.f, -1.f,
	+1.f, -1.f, +1.f,
	-1.f, -1.f, +1.f,

	+1.f, -1.f, -1.f,
	+1.f, +1.f, -1.f,
	+1.f, +1.f, +1.f,
	+1.f, -1.f, -1.f,
	+1.f, +1.f, +1.f,
	+1.f, -1.f, +1.f,

	-1.f, -1.f, -1.f,
	-1.f, +1.f, -1.f,
	+1.f, +1.f, -1.f,
	-1.f, -1.f, -1.f,
	+1.f, +1.f, -1.f,
	+1.f, -1.f, -1.f,
};
float const kCubeNormals[] = {
	0.f, +1.f, 0.f,
	0.f, +1.f, 0.f,
	0.f, +1.f, 0.f,
	0.f, +1.f, 0.f,
	0.f, +1.f, 0.f,
	0.f, +1.f, 0.f,

	0.f, 0.f, +1.f,
	0.f, 0.f, +1.f,
	0.f, 0.f, +1.f,
	0.f, 0.f, +1.f,
	0.f, 0.f, +1.f,
	0.f, 0.f, +1.f,

	-1.f, 0.f, 0.f,
	-1.f, 0.f, 0.f,
	-1.f, 0.f, 0.f,
	-1.f, 0.f, 0.f,
	-1.f, 0.f, 0.f,
	-1.f, 0.f, 0.f,

	0.f, -1.f, 0.f,
	0.f, -1.f, 0.f,
	0.f, -1.f, 0.f,
	0.f, -1.f, 0.f,
	0.f, -1.f, 0.f,
	0.f, -1.f, 0.f,

	+1.f, 0.f, 0.f,
	+1.f, 0.f, 0.f,
	+1.f, 0.f, 0.f,
	+1.f, 0.f, 0.f,
	+1.f, 0.f, 0.f,
	+1.f, 0.f, 0.f,

	0.f, 0.f, -1.f,
	0.f, 0.f, -1.f,
	0.f, 0.f, -1.f,
	0.f, 0.f, -1.f,
	0.f, 0.f, -1.f,
	0.f, 0.f, -1.f,
};

float const kCubeTexCoords[] = {
	0.0f, 1.0f,   
	1.0f, 1.0f,   
	1.0f, 0.0f,   
	0.0f, 1.0f,  
	1.0f, 0.0f,   
	0.0f, 0.0f,   

	0.0f, 0.0f,   
	0.0f, 1.0f,   
	1.0f, 1.0f,   
	0.0f, 0.0f,   
	1.0f, 1.0f,   
	1.0f, 0.0f,   

	1.0f, 1.0f,   
	0.0f, 1.0f,   
	0.0f, 0.0f,  
	1.0f, 1.0f,   
	0.0f, 0.0f,   
	1.0f, 0.0f,   

	0.0f, 0.0f,   
	1.0f, 0.0f,   
	1.0f, 1.0f,   
	0.0f, 0.0f,   
	1.0f, 1.0f,   
	0.0f, 1.0f,   

	1.0f, 0.0f,   
	1.0f, 1.0f,   
	0.0f, 1.0f,   
	1.0f, 0.0f,   
	0.0f, 1.0f,   
	0.0f, 0.0f    
};


#endif // CUBE_HPP_6874B39C_112D_4D34_BD85_AB81A730955B