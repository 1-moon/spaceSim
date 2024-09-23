#include <catch2/catch_amalgamated.hpp>


#include "../vmlib/mat44.hpp"

// See mat44-rotation.cpp first!

TEST_CASE("4X4 matrix by vector multiplication", "[mat44]")
{
	using namespace Catch::Matchers;
	static constexpr float kEps_ = 1e-6f;

	SECTION("STANDARD") {
		Mat44f m1 = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 };
		Vec4f v2 = { 1,2,3,4 };
		v2 = m1 * v2;
		Vec4f v3 = {30,70,110,150};

		REQUIRE_THAT(v2[0], WithinAbs(v3[0], kEps_));
		REQUIRE_THAT(v2[1], WithinAbs(v3[1], kEps_));
		REQUIRE_THAT(v2[2], WithinAbs(v3[2], kEps_));
		REQUIRE_THAT(v2[3], WithinAbs(v3[3], kEps_));
	}
	SECTION("IdentityMatrix") {
		Vec4f v1 = { 1,2,3,4 };
		Mat44f m2 = { 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 };
		v1 = m2 * v1;
		Vec4f v3 = { 1,2,3,4 };

		REQUIRE_THAT(v1[0], WithinAbs(v3[0], kEps_));
		REQUIRE_THAT(v1[1], WithinAbs(v3[1], kEps_));
		REQUIRE_THAT(v1[2], WithinAbs(v3[2], kEps_));
		REQUIRE_THAT(v1[3], WithinAbs(v3[3], kEps_));
	}
	SECTION("BOUNDARY") {
		Vec4f v1 = { 999,999,999,999 };
		Mat44f m2 = { 999,999,999,999,999,999,999,999,999,999,999,999,999,999,999,999 };
		v1 = m2 * v1;
		Vec4f v3 = { 3992004, 3992004, 3992004, 3992004 };

		REQUIRE_THAT(v1[0], WithinAbs(v3[0], kEps_));
		REQUIRE_THAT(v1[1], WithinAbs(v3[1], kEps_));
		REQUIRE_THAT(v1[2], WithinAbs(v3[2], kEps_));
		REQUIRE_THAT(v1[3], WithinAbs(v3[3], kEps_));
	}
}
