#include "cube.hpp"
#include "../vmlib/mat33.hpp"

SimpleMeshData make_cube( Vec3f aColor, Mat44f aPreTransform)
{
	std::vector<Vec3f> pos;
	std::vector<Vec3f> nor;
	std::vector<Vec2f> tex;

	for (std::size_t i = 0; i < 108; i+=3)
	{
		pos.emplace_back(Vec3f{ kCubePositions[i],kCubePositions[i + 1] ,kCubePositions[i + 2] });
		nor.emplace_back(Vec3f{ kCubeNormals[i],kCubeNormals[i + 1] ,kCubeNormals[i + 2] });
	}
	for (std::size_t i = 0; i < 36; i += 2) {
		tex.emplace_back(Vec2f{ kCubeTexCoords[i],kCubeTexCoords[i + 1] });
	}
	Mat33f const N = mat44_to_mat33(transpose(invert(aPreTransform)));
	for (auto& p : pos)
	{
		Vec4f p4{ p.x, p.y, p.z, 1.f };
		Vec4f t = aPreTransform * p4;
		t /= t.w;

		p = Vec3f{ t.x, t.y, t.z };
	}
	for (auto& n : nor)
	{
		Vec3f p4{ n.x, n.y, n.z };
		Vec3f t = N * p4;


		n = Vec3f{ t.x, t.y, t.z };
	}
	std::vector col(pos.size(), aColor);
	return SimpleMeshData{ std::move(pos), std::move(col), std::move(nor),
	std::move(tex), Vec3f{0.5f, 0.45f, 0.45f}, Vec3f{1.f, 1.f, 1.f},
	Vec3f{0.f, 0.f, 0.f}, 100.f };
}