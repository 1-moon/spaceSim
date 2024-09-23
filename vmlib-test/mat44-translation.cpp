#include <catch2/catch_amalgamated.hpp>


#include "../vmlib/mat44.hpp"

// See mat44-rotation.cpp first!
using namespace Catch::Matchers;

TEST_CASE("vector translation", "[mat44]")
{
	static constexpr float kEps_ = 1e-6f;
	Vec4f v1 = { 1,2,3,1};
	Mat44f m2 = make_translation({ 1.f, 2.f, 1.f });
	
	v1 = m2 * v1;

	REQUIRE_THAT(v1[0], WithinAbs(2.f, kEps_));
	REQUIRE_THAT(v1[1], WithinAbs(4.f, kEps_));
	REQUIRE_THAT(v1[2], WithinAbs(4.f, kEps_));
}
TEST_CASE("typical vector translation", "[mat44]")
{
	static constexpr float kEps_ = 1e-6f;
	Vec4f v1 = { 5,7,10,1 };
	Mat44f m2 = make_translation({ 20.f, 3.f, 60.f });

	v1 = m2 * v1;

	REQUIRE_THAT(v1[0], WithinAbs(25.f, kEps_));
	REQUIRE_THAT(v1[1], WithinAbs(10.f, kEps_));
	REQUIRE_THAT(v1[2], WithinAbs(70.f, kEps_));
}
TEST_CASE("boundary vector translation", "[mat44]")
{
	static constexpr float kEps_ = 1e-6f;
	Vec4f v1 = { 5,7,10,1 };
	Mat44f m2 = make_translation({ 99999.f, 99999.f, 99999.f });

	v1 = m2 * v1;

	REQUIRE_THAT(v1[0], WithinAbs(100004.f, kEps_));
	REQUIRE_THAT(v1[1], WithinAbs(100006.f, kEps_));
	REQUIRE_THAT(v1[2], WithinAbs(100009.f, kEps_));
}