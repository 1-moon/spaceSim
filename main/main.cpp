#include "utilities.hpp"

#include <typeinfo>
#include <stdexcept>

#include <cstdio>
#include <cstdlib>

#include "../support/error.hpp"
#include "../support/checkpoint.hpp"
#include "../support/debug_output.hpp"

#include "../vmlib/vec4.hpp"

#include "defaults.hpp"
#include "loadobj.hpp"
#include "texture.hpp"
#include "cylinder.hpp"
#include "cone.hpp"
#include "cube.hpp"
#include "spaceship.hpp"
#include "particles.hpp"

namespace
{
	constexpr char const* kWindowTitle = "COMP3811 - CW2";

	constexpr float kPi_ = 3.1415926f;
	constexpr float kMovementPerSecond_ = 5.f; // units per second
	constexpr float kMouseSensitivity_ = 0.01f; // radians per pixel

	bool splitScreenMode;
	unsigned int queryID[8];

	void glfw_callback_error_( int, char const* );

	void glfw_callback_key_( GLFWwindow*, int, int, int, int );

	void glfw_callback_motion_(GLFWwindow*, double, double);

	struct GLFWCleanupHelper
	{
		~GLFWCleanupHelper();
	};
	struct GLFWWindowDeleter
	{
		~GLFWWindowDeleter();
		GLFWwindow* window;
	};
}

// Utility function to cycle through camera modes 
CameraMode cycleCameraModes(CameraMode currentMode)
{
	switch (currentMode) {
		case CameraMode::Default:
			return CameraMode::FixedDistance;
		case CameraMode::FixedDistance:
			return CameraMode::GroundFixed;
		case CameraMode::GroundFixed:
			return CameraMode::Default;
		default:
			return CameraMode::Default;
	}
}

//Busy-waiting, avoid waiting longer than necessary for GPU
float getTimeResults(int id) {
	GLint64 startTime, stopTime;
	GLint resultAvailble = 0;
	while (!resultAvailble) {
		glGetQueryObjectiv(queryID[id + 1], GL_QUERY_RESULT_AVAILABLE, &resultAvailble);
	}

	glGetQueryObjecti64v(queryID[id], GL_QUERY_RESULT, &startTime);
	glGetQueryObjecti64v(queryID[id + 1], GL_QUERY_RESULT, &stopTime);

	return (stopTime - startTime) / 1000000.0;
}

int main() try
{
	//Performance timers
	std::vector<float> timeSection1_2;
	std::vector<float> timeSection1_4;
	std::vector<float> timeSection1_5;
	std::vector<float> timeFull;
	std::vector<float> timeFullCpuRender;
	std::vector<float> timeFullCpuTotal;

	// Initialize GLFW
	if( GLFW_TRUE != glfwInit() )
	{
		char const* msg = nullptr;
		int ecode = glfwGetError( &msg );
		throw Error( "glfwInit() failed with '%s' (%d)", msg, ecode );
	}

	// Ensure that we call glfwTerminate() at the end of the program.
	GLFWCleanupHelper cleanupHelper;

	// Configure GLFW and create window
	glfwSetErrorCallback( &glfw_callback_error_ );

	glfwWindowHint( GLFW_SRGB_CAPABLE, GLFW_TRUE );
	glfwWindowHint( GLFW_DOUBLEBUFFER, GLFW_TRUE );

	//glfwWindowHint( GLFW_RESIZABLE, GLFW_FALSE );

	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
	glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE );
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

	glfwWindowHint( GLFW_DEPTH_BITS, 24 );

#	if !defined(NDEBUG)
	// When building in debug mode, request an OpenGL debug context. This
	// enables additional debugging features. However, this can carry extra
	// overheads. We therefore do not do this for release builds.
	glfwWindowHint( GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE );
#	endif // ~ !NDEBUG

	GLFWwindow* window = glfwCreateWindow(
		1280,
		720,
		kWindowTitle,
		nullptr, nullptr
	);

	if( !window )
	{
		char const* msg = nullptr;
		int ecode = glfwGetError( &msg );
		throw Error( "glfwCreateWindow() failed with '%s' (%d)", msg, ecode );
	}

	GLFWWindowDeleter windowDeleter{ window };

	// Set up event handling
	// TODO: Additional event handling setup
	State_ state{};

	glfwSetWindowUserPointer(window, &state);

	glfwSetKeyCallback(window, &glfw_callback_key_);

	glfwSetCursorPosCallback(window, &glfw_callback_motion_);

	glfwSetKeyCallback( window, &glfw_callback_key_ );


	// Set up drawing stuff
	glfwMakeContextCurrent( window );
	glfwSwapInterval( 1 ); // V-Sync is on.

	// Initialize GLAD
	// This will load the OpenGL API. We mustn't make any OpenGL calls before this!
	if( !gladLoadGLLoader( (GLADloadproc)&glfwGetProcAddress ) )
		throw Error( "gladLoaDGLLoader() failed - cannot load GL API!" );

	std::printf( "RENDERER %s\n", glGetString( GL_RENDERER ) );
	std::printf( "VENDOR %s\n", glGetString( GL_VENDOR ) );
	std::printf( "VERSION %s\n", glGetString( GL_VERSION ) );
	std::printf( "SHADING_LANGUAGE_VERSION %s\n", glGetString( GL_SHADING_LANGUAGE_VERSION ) );

	// Ddebug output
#	if !defined(NDEBUG)
	setup_gl_debug_output();
#	endif // ~ !NDEBUG

	// Global GL state
	OGL_CHECKPOINT_ALWAYS();

	// TODO: global GL setup goes here
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_FRAMEBUFFER_SRGB);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	OGL_CHECKPOINT_ALWAYS();

	// Get actual framebuffer size.
	// This can be different from the window size, as standard window
	// decorations (title bar, borders, ...) may be included in the window size
	// but not be part of the drawable surface area.
	int iwidth, iheight;
	glfwGetFramebufferSize( window, &iwidth, &iheight );

	glGenQueries(8, queryID);

	glViewport( 0, 0, iwidth, iheight );

	// Other initialization & loading
	// Load default shader program
	ShaderProgram progDefault({
		{ GL_VERTEX_SHADER, "assets/default.vert" },
		{ GL_FRAGMENT_SHADER, "assets/default.frag" }
		});
	// Load shader program for non texture objects
	ShaderProgram progNoTexture({
		{GL_VERTEX_SHADER, "assets/noTexture.vert"},
		{ GL_FRAGMENT_SHADER, "assets/noTexture.frag"}
		});
	//Load shader program for particle system
	ShaderProgram progParticle({
		{GL_VERTEX_SHADER, "assets/particle.vert"},
		{ GL_FRAGMENT_SHADER, "assets/particle.frag"}
		});

	state.prog = &progDefault;
	state.camControlUpper.radius = 10.f;
	state.camControlUpper.xPos = 0.f;
	state.camControlUpper.yPos = 0.f;
	state.camControlUpper.movementPerSecond = 5.f;

	state.camControlLower.radius = 10.f;
	state.camControlLower.xPos = 0.f;
	state.camControlLower.yPos = 0.f;
	state.camControlLower.movementPerSecond = 5.f;

	//Animation state
	auto last = Clock::now();
	float angleX = 2 * kPi_;

	state.spaceshipLocation[0] = 30.f;
	state.spaceshipLocation[1] = 0.3f;
	state.spaceshipLocation[2] = 50.f;

	//Initialise particle system
	ParticleSystem *particles = new ParticleSystem(300, Vec3f{ state.spaceshipLocation[0], state.spaceshipLocation[1], state.spaceshipLocation[2] + 1.f });

	//Create vertex buffers and VAO
	auto island = load_wavefront_obj("assets/parlahti.obj");
	auto islandVao = create_vao(island);
	std::size_t islandVertexCount = island.positions.size();

	auto landingpad = load_wavefront_obj("assets/landingpad.obj");
	auto landingpadVao = create_vao(landingpad);
	std::size_t landingpadVertexCount = landingpad.positions.size();

	auto spaceship = make_spaceship(make_scaling(0.75f, 0.6f, 0.75f));
	auto spaceshipVao = create_vao(spaceship);
	std::size_t spaceshipVertexCount = spaceship.positions.size();

	auto particle = make_particle();
	auto particleVao = create_vao(particle);
	std::size_t particleVertexCount = particle.positions.size();

	//Load textures
	auto islandTexture = load_texture_2d("assets/L4343A-4k.jpeg");
	auto particleTexture = load_texture_2d("assets/fire.png");

	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		auto const startTimeFrame = Clock::now();

		// Let GLFW process events
		glfwPollEvents();


		// Check if window was resized.
		float fbwidth, fbheight;
		{
			int nwidth, nheight;
			glfwGetFramebufferSize(window, &nwidth, &nheight);

			fbwidth = float(nwidth);
			fbheight = float(nheight);

			if (0 == nwidth || 0 == nheight)
			{
				// Window minimized? Pause until it is unminimized.
				// This is a bit of a hack.
				do
				{
					glfwWaitEvents();
					glfwGetFramebufferSize(window, &nwidth, &nheight);

				} while (0 == nwidth || 0 == nheight);
			}

			glViewport(0, 0, nwidth, nheight);
		}

		// Update state
		auto const now = Clock::now();
		float dt = std::chrono::duration_cast<Secondsf>(now - last).count();
		last = now;

		//Update animation state
		state.spaceshipLocation[2] -= state.speedX * dt * 2.f;
		state.spaceshipLocation[1] = trajectory(state.spaceshipLocation[2], 50.f, 0.05f);
		state.spaceshipLocation[0] = trajectory(state.spaceshipLocation[1], 1.15f, 0.05f);
		angleX = static_cast<float>(atan2(0.1 * state.spaceshipLocation[2] - 5.f, 1.f) * .4f);
		state.speedX = (state.speedX * state.accelerationX) > 10 ? 10 : 
		(state.speedX * state.accelerationX);

		//Matrix computation
		Projection matrices = compute_projection_matrices(&state, fbwidth, fbheight);
		Mat44f projCameraWorld = matrices.projectionMatrixUpper;
		Mat44f projCameraWorld_d = matrices.projectionMatrixLower;

		// Update camera state 
		camera_update(&state, dt, matrices.cameraDirection);
		camera_switching(&state, dt);

		//Light computations
		Mat33f normalMatrix = matrices.normalMatrix;
		Vec3f lightDir = matrices.lightDir;

		//Animation matrix computation
		Mat44f uAnimationMatrix = make_translation({ state.spaceshipLocation[0], state.spaceshipLocation[1], state.spaceshipLocation[2] }) * 
		make_rotation_x(-angleX) * 
		make_translation({ -state.spaceshipLocation[0], -state.spaceshipLocation[1], -state.spaceshipLocation[2] });

		//Animate particles
		particles->animateParticles(dt,  
		Vec4f{ state.spaceshipLocation[0], state.spaceshipLocation[1], state.spaceshipLocation[2], 1.f });

		Mat44f uParticleBillboarding = make_translation({ particles->position.x, particles->position.y, particles->position.z })
			* invert(matrices.cameraDirection) *
			make_translation({ -particles->position.x, -particles->position.y, -particles->position.z });

		//Define coordinates for landing pads
		float locations[6] = { 0.f, -0.97f, 50.f, -20.f, -0.97f, 70.f };

		//Define point light positions, and colours
		float positions[9] = { 
		state.spaceshipLocation[0] - 0.525f, state.spaceshipLocation[1], state.spaceshipLocation[2] - 1.f,
		state.spaceshipLocation[0] + 0.525f, state.spaceshipLocation[1], state.spaceshipLocation[2] - 1.f,
		state.spaceshipLocation[0], state.spaceshipLocation[1] + 0.475f, state.spaceshipLocation[2] + 0.4f};
		float directions[9] = { 1, 1, 0, 0, 1, 1, 1, 1, 0 };
		float colours[9] = { 1.f, 0.1f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 1.f };

		//retrieve window features 
		int windowWidth, windowHeight;
		glfwGetFramebufferSize(window, &windowWidth, &windowHeight);

		// split screen mode rendering 
		if (splitScreenMode) {
			//Render upper screen
			glViewport(0, windowHeight / 2, windowWidth, windowHeight / 2);
		}
		else {
			glViewport(0, 0, windowWidth, windowHeight);
		}
		// Draw scene
		OGL_CHECKPOINT_DEBUG();

		auto const startTimeRender = Clock::now();

		glQueryCounter(queryID[0], GL_TIMESTAMP);

		//Draw frame
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glQueryCounter(queryID[2], GL_TIMESTAMP);

		//Use default shader program
		glUseProgram(progDefault.programId());

		glBindVertexArray(islandVao);

		glUniformMatrix4fv(
			0,
			1, GL_TRUE, projCameraWorld.v
		);

		glUniformMatrix3fv(
			1,
			1, GL_TRUE, normalMatrix.v
		);

		glUniform3fv(2, 1, &lightDir.x);

		//Non Blinn-Phong Diffuse lighting (white)
		glUniform3f(3, 0.9f, 0.9f, 0.9f);

		//Non Blinn-Phlong Ambient lighting (weak grey)
		glUniform3f(4, 0.05f, 0.05f, 0.05f);

		//View direction
		glUniform3f(5, state.camControlUpper.xPos, state.camControlUpper.yPos, state.camControlUpper.radius);

		//Light point directions, colours, and positions
		glUniform3fv(6, 3, directions);
		glUniform3fv(42, 3, colours);
		glUniform3fv(26, 3, positions);

		//Object material diffuse, specular, emission and shininess values
		glUniform3f(20, island.diffuse.x, island.diffuse.y, island.diffuse.z);
		glUniform3f(23, island.specular.x, island.specular.y, island.specular.z);
		glUniform3f(24, island.emission.x, island.emission.y, island.emission.z);
		glUniform1f(25, island.shininess);

		//Bind textures
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, islandTexture);

		glDrawArrays(GL_TRIANGLES, 0, islandVertexCount);

		//Unbind textures
		glBindTexture(GL_TEXTURE_2D, 0);

		//Section 1.2 times
		glQueryCounter(queryID[3], GL_TIMESTAMP);
		timeSection1_2.push_back(getTimeResults(2));

		glQueryCounter(queryID[4], GL_TIMESTAMP);

		//Use landing pad shader program
		glUseProgram(progNoTexture.programId());

		glBindVertexArray(landingpadVao);

		//Pass in landing pad locations
		glUniform3fv(88, 2, locations);

		glUniformMatrix4fv(
			50,
			1, GL_TRUE, kIdentity44f.v
		);

		glUniformMatrix4fv(
			0,
			1, GL_TRUE, projCameraWorld.v
		);

		glUniformMatrix3fv(
			1,
			1, GL_TRUE, normalMatrix.v
		);

		glUniform3fv(2, 1, &lightDir.x);

		//Non Blinn-Phong Diffuse lighting (white)
		glUniform3f(3, 0.9f, 0.9f, 0.9f);

		//Non Blinn-Phlong Ambient lighting (weak grey)
		glUniform3f(4, 0.05f, 0.05f, 0.05f);

		//View direction
		glUniform3f(5, state.camControlUpper.xPos, state.camControlUpper.yPos, state.camControlUpper.radius);

		//Light point directions, colours, and positions
		glUniform3fv(6, 3, directions);
		glUniform3fv(42, 3, colours);
		glUniform3fv(26, 3, positions);

		//Object material diffuse, specular, emission and shininess values
		glUniform3f(20, landingpad.diffuse.x, landingpad.diffuse.y, landingpad.diffuse.z);
		glUniform3f(23, landingpad.specular.x, landingpad.specular.y, landingpad.specular.z);
		glUniform3f(24, landingpad.emission.x, landingpad.emission.y, landingpad.emission.z);
		glUniform1f(25, landingpad.shininess);

		glDrawArraysInstanced(GL_TRIANGLES, 0, landingpadVertexCount, 2);

		//Section 1.4 times
		glQueryCounter(queryID[5], GL_TIMESTAMP);
		timeSection1_4.push_back(getTimeResults(4));

		glQueryCounter(queryID[6], GL_TIMESTAMP);

		//Bind spaceship VAO
		glBindVertexArray(spaceshipVao);

		//Pass in spaceship locations
		glUniform3fv(88, 2, state.spaceshipLocation);

		glUniformMatrix4fv(
			50,
			1, GL_TRUE, uAnimationMatrix.v
		);

		//Object material diffuse, specular, emission and shininess values
		glUniform3f(20, spaceship.diffuse.x, spaceship.diffuse.y, spaceship.diffuse.z);
		glUniform3f(23, spaceship.specular.x, spaceship.specular.y, spaceship.specular.z);
		glUniform3f(24, spaceship.emission.x, spaceship.emission.y, spaceship.emission.z);
		glUniform1f(25, spaceship.shininess);

		glDrawArraysInstanced(GL_TRIANGLES, 0, spaceshipVertexCount, 1);

		//Section 1.5 times
		glQueryCounter(queryID[7], GL_TIMESTAMP);
		timeSection1_5.push_back(getTimeResults(6));

		glUseProgram(progParticle.programId());

		//Pass in particle locations
		glUniform3fv(88, 300, particles->positions);

		//Bind particle VAO
		glBindVertexArray(particleVao);

		glUniformMatrix4fv(
			0,
			1, GL_TRUE, projCameraWorld.v
		);

		glUniformMatrix3fv(
			1,
			1, GL_TRUE, normalMatrix.v
		);

		//View direction
		glUniform3f(5, state.camControlUpper.xPos, state.camControlUpper.yPos, state.camControlUpper.radius);

		glUniformMatrix4fv(
			50,
			1, GL_TRUE, uParticleBillboarding.v
		);
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, particleTexture);

		//glDrawArraysInstanced(GL_TRIANGLES, 0, particleVertexCount, 300);

		//Unbind textures
		glBindTexture(GL_TEXTURE_2D, 0);

		//Full rendering times
		glQueryCounter(queryID[1], GL_TIMESTAMP);
		timeFull.push_back(getTimeResults(0));

		//CPU time benchmarks
		auto const stopTime = Clock::now();
		float elapsedRender = std::chrono::duration_cast<Secondsf>(stopTime - startTimeRender).count();
		float elapsedFull = std::chrono::duration_cast<Secondsf>(stopTime - startTimeFrame).count();
		timeFullCpuRender.push_back(elapsedRender);
		timeFullCpuTotal.push_back(elapsedFull);

		OGL_CHECKPOINT_DEBUG();
		if (splitScreenMode) {
			glViewport(0, 0, windowWidth, windowHeight / 2);
			// Draw scene
			OGL_CHECKPOINT_DEBUG();

			//Use default shader program
			glUseProgram(progDefault.programId());

			glBindVertexArray(islandVao);

			glUniformMatrix4fv(
				0,
				1, GL_TRUE, projCameraWorld_d.v
			);

			glUniformMatrix3fv(
				1,
				1, GL_TRUE, normalMatrix.v
			);

			glUniform3fv(2, 1, &lightDir.x);

			//Non Blinn-Phong Diffuse lighting (white)
			glUniform3f(3, 0.9f, 0.9f, 0.9f);

			//Non Blinn-Phlong Ambient lighting (weak grey)
			glUniform3f(4, 0.05f, 0.05f, 0.05f);

			//View direction
			glUniform3f(5, state.camControlLower.xPos, state.camControlLower.yPos, state.camControlLower.radius);

			//Light point directions, colours, and positions
			glUniform3fv(6, 3, directions);
			glUniform3fv(42, 3, colours);
			glUniform3fv(26, 3, positions);

			//Object material diffuse, specular, emission and shininess values
			glUniform3f(20, island.diffuse.x, island.diffuse.y, island.diffuse.z);
			glUniform3f(23, island.specular.x, island.specular.y, island.specular.z);
			glUniform3f(24, island.emission.x, island.emission.y, island.emission.z);
			glUniform1f(25, island.shininess);

			//Bind textures
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, islandTexture);

			glDrawArrays(GL_TRIANGLES, 0, islandVertexCount);

			//Unbind textures
			glBindTexture(GL_TEXTURE_2D, 0);

			//Use landing pad shader program
			glUseProgram(progNoTexture.programId());

			glBindVertexArray(landingpadVao);

			//Pass in landing pad locations
			glUniform3fv(88, 2, locations);

			glUniformMatrix4fv(
				50,
				1, GL_TRUE, kIdentity44f.v
			);

			glUniformMatrix4fv(
				0,
				1, GL_TRUE, projCameraWorld_d.v
			);

			glUniformMatrix3fv(
				1,
				1, GL_TRUE, normalMatrix.v
			);

			glUniform3fv(2, 1, &lightDir.x);

			//Non Blinn-Phong Diffuse lighting (white)
			glUniform3f(3, 0.9f, 0.9f, 0.9f);

			//Non Blinn-Phlong Ambient lighting (weak grey)
			glUniform3f(4, 0.05f, 0.05f, 0.05f);

			//View direction
			glUniform3f(5, state.camControlLower.xPos, state.camControlLower.yPos, state.camControlLower.radius);

			//Light point directions, colours, and positions
			glUniform3fv(6, 3, directions);
			glUniform3fv(42, 3, colours);
			glUniform3fv(26, 3, positions);

			//Object material diffuse, specular, emission and shininess values
			glUniform3f(20, landingpad.diffuse.x, landingpad.diffuse.y, landingpad.diffuse.z);
			glUniform3f(23, landingpad.specular.x, landingpad.specular.y, landingpad.specular.z);
			glUniform3f(24, landingpad.emission.x, landingpad.emission.y, landingpad.emission.z);
			glUniform1f(25, landingpad.shininess);

			glDrawArraysInstanced(GL_TRIANGLES, 0, landingpadVertexCount, 2);

			//Bind spaceship VAO
			glBindVertexArray(spaceshipVao);

			//Pass in spaceship locations
			glUniform3fv(88, 2, state.spaceshipLocation);

			glUniformMatrix4fv(
				50,
				1, GL_TRUE, uAnimationMatrix.v
			);

			glUniformMatrix4fv(
				0,
				1, GL_TRUE, projCameraWorld_d.v
			);

			glUniformMatrix3fv(
				1,
				1, GL_TRUE, normalMatrix.v
			);

			glUniform3fv(2, 1, &lightDir.x);

			//Non Blinn-Phong Diffuse lighting (white)
			glUniform3f(3, 0.9f, 0.9f, 0.9f);

			//Non Blinn-Phlong Ambient lighting (weak grey)
			glUniform3f(4, 0.05f, 0.05f, 0.05f);

			//View direction
			glUniform3f(5, state.camControlLower.xPos, state.camControlLower.yPos, state.camControlLower.radius);

			//Light point directions, colours, and positions
			glUniform3fv(6, 3, directions);
			glUniform3fv(42, 3, colours);
			glUniform3fv(26, 3, positions);

			//Object material diffuse, specular, emission and shininess values
			glUniform3f(20, spaceship.diffuse.x, spaceship.diffuse.y, spaceship.diffuse.z);
			glUniform3f(23, spaceship.specular.x, spaceship.specular.y, spaceship.specular.z);
			glUniform3f(24, spaceship.emission.x, spaceship.emission.y, spaceship.emission.z);
			glUniform1f(25, spaceship.shininess);

			glDrawArraysInstanced(GL_TRIANGLES, 0, spaceshipVertexCount, 1);

			glUseProgram(progParticle.programId());

			//Pass in particle locations
			glUniform3fv(88, 300, particles->positions);

			//Bind particle VAO
			glBindVertexArray(particleVao);

			glUniformMatrix4fv(
				0,
				1, GL_TRUE, projCameraWorld_d.v
			);

			glUniformMatrix3fv(
				1,
				1, GL_TRUE, normalMatrix.v
			);

			//View direction
			glUniform3f(5, state.camControlLower.xPos, state.camControlLower.yPos, state.camControlLower.radius);

			glUniformMatrix4fv(
				50,
				1, GL_TRUE, uParticleBillboarding.v
			);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, particleTexture);

			glDrawArraysInstanced(GL_TRIANGLES, 0, particleVertexCount, 300);

			//Unbind textures
			glBindTexture(GL_TEXTURE_2D, 0);

			OGL_CHECKPOINT_DEBUG();
		}
		//reset state
		glBindVertexArray(0);
		glUseProgram(0);
		glfwSwapBuffers(window);
		
		//Clear performance timers
		clearTimer(timeSection1_2);
		clearTimer(timeSection1_4);
		clearTimer(timeSection1_5);
		clearTimer(timeFull);
		clearTimer(timeFullCpuRender);
		clearTimer(timeFullCpuTotal);
	}

	//Print out performance timers for recent frames
	//displayPerformanceTime(timeSection1_2, "Section 1.2 Time");
	//displayPerformanceTime(timeSection1_4, "Section 1.4 Time");
	//displayPerformanceTime(timeSection1_5, "Section 1.5 Time");
	//displayPerformanceTime(timeFull, "Full Render Time");
	//displayPerformanceTime(timeFullCpuRender, "Render CPU Time (s)");
	//displayPerformanceTime(timeFullCpuTotal, "Frame-Frame CPU Time (s)");

	// Cleanup.
	state.prog = nullptr;
	delete particles;
	return 0;
}
catch( std::exception const& eErr )
{
	std::fprintf( stderr, "Top-level Exception (%s):\n", typeid(eErr).name() );
	std::fprintf( stderr, "%s\n", eErr.what() );
	std::fprintf( stderr, "Bye.\n" );
	return 1;
}


namespace
{
	bool keyC ;
	bool keyShift ;
	void glfw_callback_error_( int aErrNum, char const* aErrDesc )
	{
		std::fprintf( stderr, "GLFW error: %s (%d)\n", aErrDesc, aErrNum );
	}

	void glfw_callback_key_( GLFWwindow* aWindow, int aKey, int, int aAction, int )
	{
		if (GLFW_KEY_ESCAPE == aKey && GLFW_PRESS == aAction)
		{
			glfwSetWindowShouldClose(aWindow, GLFW_TRUE);
			return;
		}

		if (auto* state = static_cast<State_*>(glfwGetWindowUserPointer(aWindow)))
		{
			// R-key reloads shaders.
			if (GLFW_KEY_R == aKey && GLFW_PRESS == aAction)
			{
				if (state->prog)
				{
					try
					{
						state->prog->reload();
						std::fprintf(stderr, "Shaders reloaded and recompiled.\n");
					}
					catch (std::exception const& eErr)
					{
						std::fprintf(stderr, "Error when reloading shader:\n");
						std::fprintf(stderr, "%s\n", eErr.what());
						std::fprintf(stderr, "Keeping old shader.\n");
					}
				}
			}
			// utility key control in split screen mode 
			if (GLFW_KEY_C == aKey && GLFW_PRESS == aAction) {
				keyC = true;
			}
			else if (GLFW_KEY_C == aKey && GLFW_RELEASE == aAction) {
				keyC = false;
			}
			if (GLFW_KEY_LEFT_SHIFT == aKey && GLFW_PRESS == aAction) {
				keyShift = true;
			}
			else if (GLFW_KEY_LEFT_SHIFT == aKey && GLFW_RELEASE == aAction) {
				keyShift = false;
			}

			//Handle 'V' key to toggle split screen mode 
			if (GLFW_KEY_V == aKey && GLFW_PRESS == aAction) {
				// split mode on 
				splitScreenMode = !splitScreenMode;
				std::fprintf(stderr, "Split Screen Mode: %s\n",
					splitScreenMode ? "Enabled" : "Disabled");
			}
			
			if (splitScreenMode && keyShift && keyC) {
				// Your code for left_shift + 'C' key press in split-screen mode
				// change camera mode for lower split screen 
				state->camControlLower.cameraMode = cycleCameraModes(state->camControlLower.cameraMode);
				std::fprintf(stderr, "Camera Mode Changed on lower screen: %d\n", static_cast<int>(state->camControlLower.cameraMode));
			}
			else if (keyC) {
				// Your code for 'C' key press in split-screen mode
				// change camera mode for upper split screen 
				state->camControlUpper.cameraMode = cycleCameraModes(state->camControlUpper.cameraMode);
				std::fprintf(stderr, "Camera Mode Changed on upper screen: %d\n", static_cast<int>(state->camControlUpper.cameraMode));
				
			}
			// Space toggles camera
			if (GLFW_KEY_SPACE == aKey && GLFW_PRESS == aAction)
			{
				state->camControlUpper.cameraActive = !state->camControlUpper.cameraActive;
				state->camControlLower.cameraActive = !state->camControlLower.cameraActive;
				if (state->camControlUpper.cameraActive)
					glfwSetInputMode(aWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
				else
					glfwSetInputMode(aWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				if (state->camControlLower.cameraActive)
					glfwSetInputMode(aWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
				else
					glfwSetInputMode(aWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			}
			// Camera controls if camera is active
			if (state->camControlUpper.cameraActive)
			{
				camera_controls(state, aKey, aAction);
			}
		}
	}

	void glfw_callback_motion_(GLFWwindow* aWindow, double aX, double aY)
	{
		if (auto* state = static_cast<State_*>(glfwGetWindowUserPointer(aWindow)))
		{
			if (state->camControlUpper.cameraActive)
			{
				auto const dx = float(aX - state->camControlUpper.lastX);
				auto const dy = float(aY - state->camControlUpper.lastY);

				state->camControlUpper.phi += dx * kMouseSensitivity_;

				state->camControlUpper.theta += dy * kMouseSensitivity_;
				if (state->camControlUpper.theta > kPi_ / 2.f)
					state->camControlUpper.theta = kPi_ / 2.f;
				else if (state->camControlUpper.theta < -kPi_ / 2.f)
					state->camControlUpper.theta = -kPi_ / 2.f;
			}

			state->camControlUpper.lastX = float(aX);
			state->camControlUpper.lastY = float(aY);
		}
	}



}

namespace
{
	GLFWCleanupHelper::~GLFWCleanupHelper()
	{
		glfwTerminate();
	}

	GLFWWindowDeleter::~GLFWWindowDeleter()
	{
		if( window )
			glfwDestroyWindow( window );
	}
}

//Use Graphics Card
#if defined(_MSC_VER)
extern "C" {
	__declspec(dllexport) unsigned long NvOptimusEnablement = 1;
	__declspec(dllexport) unsigned long AmdPowerXpressRequestHighPerformance = 1;
}
#endif

