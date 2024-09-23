#include <catch2/catch_amalgamated.hpp>

#include "../vmlib/mat44.hpp"

// See mat44-rotation.cpp first.

TEST_CASE("Perspective projection", "[mat44]")
{
	static constexpr float kEps_ = 1e-5f;

	using namespace Catch::Matchers;

	// "Standard" projection matrix presented in the exercises. Assumes
	// standard window size (e.g., 1280x720).
	//
	// Field of view (FOV) = 60 degrees
	// Window size is 1280x720 and we defined the aspect ratio as w/h
	// Near plane at 0.1 and far at 100
	SECTION("Standard")
	{
		auto const proj = make_perspective_projection(
			60.f * 3.1415926f / 180.f,
			1280 / float(720),
			0.1f, 100.f
		);

		REQUIRE_THAT(proj(0, 0), WithinAbs(0.974279, kEps_));
		REQUIRE_THAT(proj(0, 1), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(proj(0, 2), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(proj(0, 3), WithinAbs(0.f, kEps_));

		REQUIRE_THAT(proj(1, 0), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(proj(1, 1), WithinAbs(1.732051f, kEps_));
		REQUIRE_THAT(proj(1, 2), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(proj(1, 3), WithinAbs(0.f, kEps_));

		REQUIRE_THAT(proj(2, 0), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(proj(2, 1), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(proj(2, 2), WithinAbs(-1.002002f, kEps_));
		REQUIRE_THAT(proj(2, 3), WithinAbs(-0.200200f, kEps_));

		REQUIRE_THAT(proj(3, 0), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(proj(3, 1), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(proj(3, 2), WithinAbs(-1.f, kEps_));
		REQUIRE_THAT(proj(3, 3), WithinAbs(0.f, kEps_));
	}
	SECTION("Atypical Window Size") {
		// Alternative projection matrix. Assumes
		// Window size 1440x2560 (e.g., a phone in portrait mode)
		//
		// Field of view (FOV) = 70 degrees
		// Near plane at 0.1 and far at 100
		auto const proj = make_perspective_projection(
			70.0f * 3.1415926f / 180.0f,
			1440 / 2560.f,
			0.1f,
			100.0f
		);

		REQUIRE_THAT(proj(0, 0), WithinAbs(2.53893f, kEps_));
		REQUIRE_THAT(proj(0, 1), WithinAbs(0.0f, kEps_));
		REQUIRE_THAT(proj(0, 2), WithinAbs(0.0f, kEps_));
		REQUIRE_THAT(proj(0, 3), WithinAbs(0.0f, kEps_));

		REQUIRE_THAT(proj(1, 0), WithinAbs(0.0f, kEps_));
		REQUIRE_THAT(proj(1, 1), WithinAbs(1.42815f, kEps_));
		REQUIRE_THAT(proj(1, 2), WithinAbs(0.0f, kEps_));
		REQUIRE_THAT(proj(1, 3), WithinAbs(0.0f, kEps_));

		REQUIRE_THAT(proj(2, 0), WithinAbs(0.0f, kEps_));
		REQUIRE_THAT(proj(2, 1), WithinAbs(0.0f, kEps_));
		REQUIRE_THAT(proj(2, 2), WithinAbs(-1.002002f, kEps_));
		REQUIRE_THAT(proj(2, 3), WithinAbs(-0.200200f, kEps_));

		REQUIRE_THAT(proj(3, 0), WithinAbs(0.0f, kEps_));
		REQUIRE_THAT(proj(3, 1), WithinAbs(0.0f, kEps_));
		REQUIRE_THAT(proj(3, 2), WithinAbs(-1.0f, kEps_));
		REQUIRE_THAT(proj(3, 3), WithinAbs(0.0f, kEps_));
	}
	SECTION("Atypical Field of view") {
		// Alternative projection matrix. Assumes
		// Window size 1280x720 (e.g., a phone in portrait mode)
		//
		// Field of view (FOV) = 320 degrees
		// Near plane at 0.4 and far at 200
		auto const proj = make_perspective_projection(
			320.0f * 3.1415926f / 180.0f,
			1280 / 720.f,
			0.4f,
			200.0f
		);

		REQUIRE_THAT(proj(0, 0), WithinAbs(-1.54545f, kEps_));
		REQUIRE_THAT(proj(0, 1), WithinAbs(0.0f, kEps_));
		REQUIRE_THAT(proj(0, 2), WithinAbs(0.0f, kEps_));
		REQUIRE_THAT(proj(0, 3), WithinAbs(0.0f, kEps_));

		REQUIRE_THAT(proj(1, 0), WithinAbs(0.0f, kEps_));
		REQUIRE_THAT(proj(1, 1), WithinAbs(-2.74747f, kEps_));
		REQUIRE_THAT(proj(1, 2), WithinAbs(0.0f, kEps_));
		REQUIRE_THAT(proj(1, 3), WithinAbs(0.0f, kEps_));

		REQUIRE_THAT(proj(2, 0), WithinAbs(0.0f, kEps_));
		REQUIRE_THAT(proj(2, 1), WithinAbs(0.0f, kEps_));
		REQUIRE_THAT(proj(2, 2), WithinAbs(-1.00401f, kEps_));
		REQUIRE_THAT(proj(2, 3), WithinAbs(-0.8016f, kEps_));

		REQUIRE_THAT(proj(3, 0), WithinAbs(0.0f, kEps_));
		REQUIRE_THAT(proj(3, 1), WithinAbs(0.0f, kEps_));
		REQUIRE_THAT(proj(3, 2), WithinAbs(-1.0f, kEps_));
		REQUIRE_THAT(proj(3, 3), WithinAbs(0.0f, kEps_));
	}
}