#include "cone.hpp"
#include "../vmlib/mat33.hpp"

SimpleMeshData make_cone(bool aCapped, std::size_t aSubdivs, Vec3f aColor, Mat44f aPreTransform)
{
	std::vector<Vec3f> pos;
	std::vector<Vec3f> nor;

	float prevY = std::cos(0.f);
	float prevZ = std::sin(0.f);

	for (std::size_t i = 0; i < aSubdivs; ++i)
	{
		float const angle = (i + 1) / float(aSubdivs) * 2.f * 3.1415926f;
		float y = std::cos(angle);
		float z = std::sin(angle);

		pos.emplace_back(Vec3f{ 0.f, prevY, prevZ });
		nor.emplace_back(Vec3f{ 0.f,prevY, prevZ });
		pos.emplace_back(Vec3f{ 0.f, y, z });
		nor.emplace_back(Vec3f{ 0.f,y, z });
		pos.emplace_back(Vec3f{ 1.f, 0, 0 });
		nor.emplace_back(Vec3f{ 0.f,prevY, prevZ });
		


		if (aCapped == true) {
			pos.emplace_back(Vec3f{ 0.f, y, z });
			nor.emplace_back(Vec3f{ -1,0, 0 });
			pos.emplace_back(Vec3f{ 0.f, prevY, prevZ });
			nor.emplace_back(Vec3f{ -1,0, 0 });
			pos.emplace_back(Vec3f{ 0.f, 0, 0 });
			nor.emplace_back(Vec3f{ -1,0, 0 });

		}


		prevY = y;
		prevZ = z;
	}
	Mat33f const N = mat44_to_mat33(transpose(invert(aPreTransform)));
	for(auto& p : pos)
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
	std::vector<Vec2f>{}, Vec3f{0.5f, 0.45f, 0.45f}, Vec3f{1.f, 1.f, 1.f},
	Vec3f{0.f, 0.f, 0.f}, 100.f };
}
