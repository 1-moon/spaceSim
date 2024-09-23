#include "particles.hpp"

//Constructor for particle system
ParticleSystem::ParticleSystem(int particleNum, Vec3f position) {
	particles1.resize(particleNum);
	this->particleNum = particleNum;
	this->position = position;
	
	//initalise random distributions of velocities, start positions and time to live
	std::default_random_engine randomGenerator;
	std::uniform_real_distribution<float> distribution(-.1f, .1f);
	std::uniform_real_distribution<float> velocity(0.f, 1.f);
	std::uniform_real_distribution<float> time(0.5f, 8.f);

	//Initailise particles with random values
	for (auto& particle : particles1) {
		particle.start = Vec3f{ distribution(randomGenerator), distribution(randomGenerator), distribution(randomGenerator) };
		particle.velocity = Vec3f{ velocity(randomGenerator), velocity(randomGenerator), velocity(randomGenerator) };
		particle.ttl = time(randomGenerator);
		particle.time = particle.ttl;
		particle.progress = particle.start;
	}
}

//Animate particles by updating their positions
void ParticleSystem::animateParticles(float dt, Vec4f shipLoc) {
	int i = 0;
	//for each particle, compute its new position, given its current position, velocity and time to live
	for (auto& particle : particles1) {
		Vec3f shipLoc1 = Vec3f{ shipLoc.x, shipLoc.y, shipLoc.z };
		Vec3f shipLocN = normalize(shipLoc1);

		position = Vec3f{ shipLoc.x, shipLoc.y - 0.2f, shipLoc.z + 1.f};
		Vec3f update = { particle.velocity.x * shipLocN.x, -particle.velocity.y * shipLocN.y, particle.velocity.z * shipLocN.z };
		particle.progress += update * dt;
		particle.time -= 3.f * dt;
		particle.position = position + particle.progress;

		if (particle.time <= 0.0f) {
			particle.progress = particle.start;
			particle.time = particle.ttl;
			particle.position = position;
		}

		positions[i++] = particle.position.x;
		positions[i++] = particle.position.y;
		positions[i++] = particle.position.z;
	}
}

//Return a cube mesh scaled to be a particle
SimpleMeshData make_particle() {
	return make_cube(Vec3f{ 1.f, 0.f, 0.f }, make_scaling(0.05f, 0.05f, 0.0001f));
}

