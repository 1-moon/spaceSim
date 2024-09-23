#include <vector>
#include <random>
#include "../vmlib/mat44.hpp"
#include "../vmlib/mat33.hpp"
#include "../vmlib/vec4.hpp"
#include "simple_mesh.hpp"
#include "cube.hpp"

struct Particle {
	Vec3f position;
	Vec3f start;
	Vec3f velocity;
	Vec3f progress;
	float ttl;
	float time;
};

class ParticleSystem {
	int particleNum;

public:
	std::vector<Particle> particles1;
	float positions[900] = { 0 };
	Vec3f position;
	ParticleSystem(int particleNum, Vec3f position);
	void animateParticles(float dt, Vec4f shipLoc);
};

SimpleMeshData make_particle();