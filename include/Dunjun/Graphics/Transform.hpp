#ifndef DUNJUN_GRAPHICS_TRANSFORM_HPP 
#define DUNJUN_GRAPHICS_TRANSFORM_HPP 

#include <Dunjun/Math.hpp>

namespace Dunjun
{
	struct Transform
	{
		Vector3 position = {0, 0, 0};
		Quaternion orientation = Quaternion();
		Vector3 scale = {1, 1, 1};
	};

	// World Transform
	Transform operator*(const Transform& ps, const Transform& ls);
	Transform operator/(const Transform& ws, const Transform& ps);

	inline Transform& operator*=(Transform& ps, const Transform& ls)
	{
		ps = ps * ls;

		return ps;
	}

	inline Transform& operator/=(Transform& ws, const Transform& ps)
	{
		ws = ws / ps;

		return ws;
	}


	Transform inverse(const Transform& t);
	Matrix4 transformMatrix4(const Transform& t);
} // end Dunjun

#endif
