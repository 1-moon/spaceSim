#include <catch2/catch_amalgamated.hpp>


#include "../vmlib/mat44.hpp"

// See mat44-rotation.cpp first!

TEST_CASE( "4x4 matrix by matrix multiplication", "[mat44]" )
{
	using namespace Catch::Matchers;
	static constexpr float kEps_ = 1e-6f;

	SECTION("STANDARD") {
		Mat44f m1 = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 };
		Mat44f m2 = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 };
		m1 = m1 * m2;
		m2 = { 90, 100, 110, 120, 202, 228, 254, 280, 314, 356, 398, 440, 426, 484, 542, 600 };
	
		REQUIRE_THAT(m1.v[0], WithinAbs(m2.v[0],kEps_));
		REQUIRE_THAT(m1.v[1], WithinAbs(m2.v[1], kEps_));
		REQUIRE_THAT(m1.v[2], WithinAbs(m2.v[2], kEps_));
		REQUIRE_THAT(m1.v[3], WithinAbs(m2.v[3], kEps_));
		REQUIRE_THAT(m1.v[4], WithinAbs(m2.v[4], kEps_));
		REQUIRE_THAT(m1.v[5], WithinAbs(m2.v[5], kEps_));
		REQUIRE_THAT(m1.v[6], WithinAbs(m2.v[6], kEps_));
		REQUIRE_THAT(m1.v[7], WithinAbs(m2.v[7], kEps_));
		REQUIRE_THAT(m1.v[8], WithinAbs(m2.v[8], kEps_));
		REQUIRE_THAT(m1.v[9], WithinAbs(m2.v[9], kEps_));
		REQUIRE_THAT(m1.v[10], WithinAbs(m2.v[10], kEps_));
		REQUIRE_THAT(m1.v[11], WithinAbs(m2.v[11], kEps_));
		REQUIRE_THAT(m1.v[12], WithinAbs(m2.v[12], kEps_));
		REQUIRE_THAT(m1.v[13], WithinAbs(m2.v[13], kEps_));
		REQUIRE_THAT(m1.v[14], WithinAbs(m2.v[14], kEps_));
		REQUIRE_THAT(m1.v[15], WithinAbs(m2.v[15], kEps_));
	}
	SECTION("IdentityMatrix") {
		Mat44f m1 = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 };
		Mat44f m2 = { 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 };
		m1 = m1 * m2;
		m2 = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 };

		REQUIRE_THAT(m1.v[0], WithinAbs(m2.v[0], kEps_));
		REQUIRE_THAT(m1.v[1], WithinAbs(m2.v[1], kEps_));
		REQUIRE_THAT(m1.v[2], WithinAbs(m2.v[2], kEps_));
		REQUIRE_THAT(m1.v[3], WithinAbs(m2.v[3], kEps_));
		REQUIRE_THAT(m1.v[4], WithinAbs(m2.v[4], kEps_));
		REQUIRE_THAT(m1.v[5], WithinAbs(m2.v[5], kEps_));
		REQUIRE_THAT(m1.v[6], WithinAbs(m2.v[6], kEps_));
		REQUIRE_THAT(m1.v[7], WithinAbs(m2.v[7], kEps_));
		REQUIRE_THAT(m1.v[8], WithinAbs(m2.v[8], kEps_));
		REQUIRE_THAT(m1.v[9], WithinAbs(m2.v[9], kEps_));
		REQUIRE_THAT(m1.v[10], WithinAbs(m2.v[10], kEps_));
		REQUIRE_THAT(m1.v[11], WithinAbs(m2.v[11], kEps_));
		REQUIRE_THAT(m1.v[12], WithinAbs(m2.v[12], kEps_));
		REQUIRE_THAT(m1.v[13], WithinAbs(m2.v[13], kEps_));
		REQUIRE_THAT(m1.v[14], WithinAbs(m2.v[14], kEps_));
		REQUIRE_THAT(m1.v[15], WithinAbs(m2.v[15], kEps_));
	}
	SECTION("BOUNDARY") {
		Mat44f m1 = { 999,999,999,999,999,999,999,999,999,999,999,999,999,999,999,999 };
		Mat44f m2 = { 3,15,12,18,12,1,4,9,9,10,88,2,3,12,6,10 };
		m1 = m1 * m2;
		m2 = { 26973, 37962, 109890, 38961, 26973, 37962, 109890, 38961, 26973, 37962, 109890, 38961, 
		26973, 37962, 109890, 38961 };

		REQUIRE_THAT(m1.v[0], WithinAbs(m2.v[0], kEps_));
		REQUIRE_THAT(m1.v[1], WithinAbs(m2.v[1], kEps_));
		REQUIRE_THAT(m1.v[2], WithinAbs(m2.v[2], kEps_));
		REQUIRE_THAT(m1.v[3], WithinAbs(m2.v[3], kEps_));
		REQUIRE_THAT(m1.v[4], WithinAbs(m2.v[4], kEps_));
		REQUIRE_THAT(m1.v[5], WithinAbs(m2.v[5], kEps_));
		REQUIRE_THAT(m1.v[6], WithinAbs(m2.v[6], kEps_));
		REQUIRE_THAT(m1.v[7], WithinAbs(m2.v[7], kEps_));
		REQUIRE_THAT(m1.v[8], WithinAbs(m2.v[8], kEps_));
		REQUIRE_THAT(m1.v[9], WithinAbs(m2.v[9], kEps_));
		REQUIRE_THAT(m1.v[10], WithinAbs(m2.v[10], kEps_));
		REQUIRE_THAT(m1.v[11], WithinAbs(m2.v[11], kEps_));
		REQUIRE_THAT(m1.v[12], WithinAbs(m2.v[12], kEps_));
		REQUIRE_THAT(m1.v[13], WithinAbs(m2.v[13], kEps_));
		REQUIRE_THAT(m1.v[14], WithinAbs(m2.v[14], kEps_));
		REQUIRE_THAT(m1.v[15], WithinAbs(m2.v[15], kEps_));
	}
}