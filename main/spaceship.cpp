#include "spaceship.hpp"
#include "../vmlib/mat33.hpp"
#include "cone.hpp"
#include "cube.hpp"
#include "cylinder.hpp"

SimpleMeshData make_spaceship(Mat44f aPreTransform)
{
	auto cylinder1 = make_cylinder(true, 16, { 1.f, 1.f, 1.f },  make_rotation_y(3.141592f / 2.f)* make_translation({ 1.f,0.f,0.f })* make_scaling(1.f, 0.25f, 0.5f));
	auto cylinder2 = make_cylinder(true, 16, { 1.f, 1.f, 1.f }, make_rotation_y(3.141592f * 1.5f) * make_translation({ 0.2f,0.f,1.f }) * make_scaling(0.4f, 0.25f, 0.25));
	auto cylinder3 = make_cylinder(true, 16, { 1.f, 1.f, 1.f }, make_rotation_y(3.141592f * 1.5f) * make_translation({ 0.2f,0.f,-1.f }) * make_scaling(0.4f, 0.25f, 0.25f));
	auto cube = make_cube({ 0.f, 0.f, 0.f }, make_translation({ -1.f, 0.f, 0.f }) * make_scaling(0.5f, 0.25f, .25f));
	auto cube2 = make_cube({ 0.f, 0.f, 0.f }, make_translation({ 1.f, 0.f, 0.f }) * make_scaling(0.5f, 0.25f, .25f));
	auto cube3 = make_cube({ 0.f, 0.f, 0.f }, make_scaling(0.75f, 0.25f, 1.f));
	auto light1 = make_cube({ 1.f, 0.f, 0.f }, make_translation({ -0.5f, 0.f, -1.1f }) * make_scaling(0.03f, 0.03f, 0.03f));
	auto light2 = make_cube({ 0.f, 1.f, 0.f }, make_translation({ 0.5f, 0.f, -1.1f }) * make_scaling(0.03f, 0.03f, 0.03f));
	auto light3 = make_cube({ 0.f, 0.f, 1.f }, make_translation({ 0.f, 0.3f, 0.5f }) * make_scaling(0.03f, 0.03f, 0.03f));
	auto cone = make_cone(true, 16, { 0.f,0.f,0.f },make_rotation_y(3.141592f / 2.f)* make_translation({ 2.f,0.f,0.f }) * make_scaling(0.5f,0.25f,0.25f));

	auto xarrow = concatenate(std::move(cylinder1), cube);
	xarrow = concatenate(std::move(xarrow), cone);
	xarrow = concatenate(std::move(xarrow), cube2);
	xarrow = concatenate(std::move(xarrow), cube3);
	xarrow = concatenate(std::move(xarrow), cylinder2);
	xarrow = concatenate(std::move(xarrow), cylinder3);
	xarrow = concatenate(std::move(xarrow), light1);
	xarrow = concatenate(std::move(xarrow), light2);
	xarrow = concatenate(std::move(xarrow), light3);


	Mat33f const N = mat44_to_mat33(transpose(invert(aPreTransform)));
	for (auto& p : xarrow.positions)
	{
		Vec4f p4{ p.x, p.y, p.z, 1.f };
		Vec4f t = aPreTransform * p4;
		t /= t.w;

		p = Vec3f{ t.x, t.y, t.z };
	}
	for (auto& n : xarrow.normals)
	{
		Vec3f p4{ n.x, n.y, n.z };
		Vec3f t = N * p4;


		n = Vec3f{ t.x, t.y, t.z };
	}
	return xarrow;
}