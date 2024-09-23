#include "utilities.hpp"

Projection compute_projection_matrices(State_ *state, float fbwidth, float fbheight) {
	Mat44f model2world = kIdentity44f;

	//compute matrices for upper screen
	Mat44f Rx = make_rotation_x(state->camControlUpper.theta);
	Mat44f Ry = make_rotation_y(state->camControlUpper.phi);
	Mat44f T = make_translation({state->camControlUpper.xPos, state->camControlUpper.yPos, -state->camControlUpper.radius });
	Mat44f cameraDirection = Rx * Ry;
	Mat44f world2camera = Rx * Ry * T;

	//compute matrices for lower screen 
	Mat44f Rx_d = make_rotation_x(state->camControlLower.theta);
	Mat44f Ry_d = make_rotation_y(state->camControlLower.phi);
	Mat44f T_d = make_translation({ state->camControlLower.xPos, state->camControlLower.yPos, -state->camControlLower.radius });
	Mat44f world2camera_d = Rx_d * Ry_d * T_d;

	//Projection matrix computation
	Mat44f projection = make_perspective_projection(
		60.f * 3.1415926f / 180.f,
		fbwidth / (float)fbheight,
		0.1f, 100.f);

	Projection projectionMatrices;
	projectionMatrices.projectionMatrixUpper =  projection * world2camera * model2world;
	projectionMatrices.projectionMatrixLower = projection * world2camera_d * model2world;
	projectionMatrices.normalMatrix = mat44_to_mat33(transpose(invert(model2world)));
	projectionMatrices.lightDir = normalize(Vec3f{ 0.f, 1.f, -1.f });
	projectionMatrices.cameraDirection = cameraDirection;

	return projectionMatrices;
}

void camera_switching(State_* state, float dt) {

	//Ground position for fixed camera;
	Vec3f groundPosition = { -11.f, -0.3f, 0.3f };

	//===================== Upper screen =====================
	// camera viewport change if camera mode is switched 
	if (state->camControlUpper.cameraMode == CameraMode::FixedDistance) {
		state->camControlUpper.xPos = -state->spaceshipLocation[0];
		state->camControlUpper.yPos = -state->spaceshipLocation[1];
		state->camControlUpper.radius = state->spaceshipLocation[2] + 5.f; // Fixed distance from the space vehicle

		// Adjust camera orientation to always look at the space vehicle
		state->camControlUpper.theta = atan2(0,
			length(Vec2f{ 0.f , 5.f }));
		state->camControlUpper.phi = atan2(0, 0);

	}
	else if (state->camControlUpper.cameraMode == CameraMode::GroundFixed) {
		// Set camera position to a fixed ground position
		state->camControlUpper.xPos = groundPosition.x;
		state->camControlUpper.yPos = groundPosition.y - 2.0f; // Adjust the height as needed
		state->camControlUpper.radius = 5.0f; // Fied distance from the ground

		// Adjust camera orientation to always look at the space vehicle
		state->camControlUpper.theta = atan2(-state->spaceshipLocation[1] - groundPosition.y,
			length(Vec2f{ -state->spaceshipLocation[0] - groundPosition.x, -state->spaceshipLocation[2] - groundPosition.z }));
		state->camControlUpper.phi = -atan2(-state->spaceshipLocation[0] - groundPosition.x, -state->spaceshipLocation[2] - groundPosition.z);
	}
	// ============== lower screen ==========================
	// camera viewport change if camera mode is switched 
	if (state->camControlLower.cameraMode == CameraMode::FixedDistance) {
		state->camControlLower.xPos = -state->spaceshipLocation[0];
		state->camControlLower.yPos = -state->spaceshipLocation[1];
		state->camControlLower.radius = state->spaceshipLocation[2] + 5.f; // Fixed distance from the space vehicle

		// Adjust camera orientation to always look at the space vehicle
		state->camControlLower.theta = atan2(0,
			length(Vec2f{ 0.f , 5.f }));
		state->camControlLower.phi = atan2(0, 0);
	}
	else if (state->camControlLower.cameraMode == CameraMode::GroundFixed) {
		// Set camera position to a fixed ground position
		state->camControlLower.xPos = groundPosition.x;
		state->camControlLower.yPos = groundPosition.y - 2.0f; // Adjust the height as needed
		state->camControlLower.radius = 5.0f; // Fied distance from the ground

		// Adjust camera orientation to always look at the space vehicle
		state->camControlLower.theta = atan2(-state->spaceshipLocation[1] - groundPosition.y,
			length(Vec2f{ -state->spaceshipLocation[0] - groundPosition.x, -state->spaceshipLocation[2] - groundPosition.z }));
		state->camControlLower.phi = -atan2(-state->spaceshipLocation[0] - groundPosition.x, -state->spaceshipLocation[2] - groundPosition.z);
	}
}

void camera_update(State_* state, float dt, Mat44f cameraDirection) {
	if (state->camControlUpper.actionZoomIn) {
		Vec4f update = cameraDirection * Vec4f{ 0.f, 0.f, -(1.f * state->camControlUpper.movementPerSecond * dt), 1.f };
		int inverse = update.z > 0 ? -1 : 1;
		state->camControlUpper.radius += update.z; 
		state->camControlUpper.xPos += update.x; 
		state->camControlUpper.yPos += update.y * inverse; 

	}
	else if (state->camControlUpper.actionZoomOut) {
		Vec4f update = cameraDirection * Vec4f{ 0.f, 0.f, (1.f * state->camControlUpper.movementPerSecond * dt), 1.f };
		int inverse = update.z > 0 ? 1 : -1;
		state->camControlUpper.radius += update.z;
		state->camControlUpper.xPos += update.x;
		state->camControlUpper.yPos += update.y * inverse;
	}
		
	if (state->camControlUpper.actionMoveLeft) {
		Vec4f update = cameraDirection * Vec4f{ (1.f * state->camControlUpper.movementPerSecond * dt), 0.f, 0.f, 1.f };
		state->camControlUpper.radius += update.z;
		state->camControlUpper.xPos += update.x;
		state->camControlUpper.yPos += update.y;
	}
		
	else if (state->camControlUpper.actionMoveRight) {
		Vec4f update = cameraDirection * Vec4f{ -(1.f * state->camControlUpper.movementPerSecond * dt), 0.f, 0.f, 1.f };
		state->camControlUpper.radius += update.z;
		state->camControlUpper.xPos += update.x;
		state->camControlUpper.yPos += update.y;
	}
	if (state->camControlUpper.actionMoveUp)
		state->camControlUpper.yPos += state->camControlUpper.movementPerSecond * dt;
	else if (state->camControlUpper.actionMoveDown)
		state->camControlUpper.yPos -= state->camControlUpper.movementPerSecond * dt;
}

void camera_controls(State_* state, int aKey, int aAction) {
	if (GLFW_KEY_W == aKey)
	{
		if (GLFW_PRESS == aAction)
			state->camControlUpper.actionZoomIn = true;
		else if (GLFW_RELEASE == aAction)
			state->camControlUpper.actionZoomIn = false;
	}
	else if (GLFW_KEY_S == aKey)
	{
		if (GLFW_PRESS == aAction)
			state->camControlUpper.actionZoomOut = true;
		else if (GLFW_RELEASE == aAction)
			state->camControlUpper.actionZoomOut = false;
	}
	if (GLFW_KEY_A == aKey) {
		if (GLFW_PRESS == aAction)
			state->camControlUpper.actionMoveLeft = true;
		else if (GLFW_RELEASE == aAction)
			state->camControlUpper.actionMoveLeft = false;
	}
	else if (GLFW_KEY_D == aKey)
	{
		if (GLFW_PRESS == aAction)
			state->camControlUpper.actionMoveRight = true;
		else if (GLFW_RELEASE == aAction)
			state->camControlUpper.actionMoveRight = false;
	}
	if (GLFW_KEY_E == aKey) {
		if (GLFW_PRESS == aAction)
			state->camControlUpper.actionMoveUp = true;
		else if (GLFW_RELEASE == aAction)
			state->camControlUpper.actionMoveUp = false;
	}
	else if (GLFW_KEY_Q == aKey)
	{
		if (GLFW_PRESS == aAction)
			state->camControlUpper.actionMoveDown = true;
		else if (GLFW_RELEASE == aAction)
			state->camControlUpper.actionMoveDown = false;
	}
	if (GLFW_KEY_LEFT_SHIFT == aKey) {
		if (GLFW_PRESS == aAction)
			state->camControlUpper.movementPerSecond *= 1.5f;
	}
	else if (GLFW_KEY_LEFT_CONTROL == aKey)
	{
		if (GLFW_PRESS == aAction)
			state->camControlUpper.movementPerSecond /= 1.5f;
	}
	else if (GLFW_KEY_F == aKey) {
		if (GLFW_PRESS == aAction) {
			state->accelerationX = 1.01f;
			state->speedX = 1.f;
			state->animate = true;
		}
	}
	else if (GLFW_KEY_R == aKey) {
		if (GLFW_PRESS == aAction) {
			state->accelerationX = 0.f;
			state->speedX = 0.f;
			state->animate = false;
			state->spaceshipLocation[0] = 30.f;
			state->spaceshipLocation[1] = -0.3f;
			state->spaceshipLocation[2] = 50.f;
		}
	}
}

//Display performance time for each frame, for each timer
void displayPerformanceTime(std::vector<float>& timer, std::string title) {
	float sum = 0;
	const char *msg = title.c_str();
	for (int i = 0; i < (int)timer.size(); i++) {
		std::fprintf(stderr, "%s, frame %d: %f\n", msg, i, timer[i]);
		sum += timer[i];
	}
	std::fprintf(stderr, "Average %s: %f\n", msg, sum / timer.size());
}	

//Quadratic trajectory function for animation
float trajectory(float x, float root, float steepness) {
	return steepness*(x - root) * steepness * (x - root);
}

//Clear timers to avoid memory overflow
void clearTimer(std::vector<float>& timer) {
	if (timer.size() > 1000) {
		timer.clear();
	}
}