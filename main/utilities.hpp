#include <glad.h>
#include <GLFW/glfw3.h>

#include "../support/program.hpp"

#include "../vmlib/mat44.hpp"
#include "../vmlib/mat33.hpp"

#include "simple_mesh.hpp"

enum class CameraMode {
	Default,
	FixedDistance,
	GroundFixed
};

struct State_
{
	ShaderProgram* prog;
	bool animate = false;
	float accelerationX = 0.f;
	float speedX = 0.0f;
	float spaceshipLocation[3] = { 30.f, -0.3f, 50.f };

	struct CamCtrl_
	{
		bool cameraActive;
		bool actionZoomIn, actionZoomOut;
		bool actionMoveRight, actionMoveLeft;
		bool actionMoveUp, actionMoveDown;

		CameraMode cameraMode;

		float phi, theta;
		float radius;

		float lastX, lastY;

		float xPos;
		float yPos;

		float movementPerSecond;
	}   camControlUpper, camControlLower;
};

struct Projection {
	Mat44f projectionMatrixUpper;
	Mat44f projectionMatrixLower;
	Mat44f cameraDirection;
	Mat33f normalMatrix;
	Vec3f lightDir;
};

Projection compute_projection_matrices(State_* state, float fbwidth, float fbheight);

void camera_switching(State_* state, float dt);

void camera_update(State_* state, float dt, Mat44f cameraDirection);

void camera_controls(State_* state, int aKey, int aAction);

float trajectory(float x, float root, float steepness);

void clearTimer(std::vector<float>& timer);

void displayPerformanceTime(std::vector<float>& timer, std::string title);