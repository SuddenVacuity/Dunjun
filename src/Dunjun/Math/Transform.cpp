
#include <Dunjun/Transform.hpp>

namespace Dunjun
{
	// World Transform  = Parent * Local
	Transform operator*(const Transform& ps, const Transform& ls)
	{
		Transform ws;
		// correct order: scale >> rotate >> translate

		ws.position = ps.position + ps.orientation * (ps.scale * ls.position); // <-- This is correct
		//ws.position = ps.position + (ps.scale * ls.position); // removed orientation for easier testing
		ws.orientation = ps.orientation * ls.orientation;
		//ws.scale = ps.scale * (ps.orientation * ls.scale); // the orientation doesn't multiply with the scale correctly
		ws.scale = ps.scale * ls.scale;

		return ws;
	}

	// World Transform  = World / Parent
	Transform operator/(const Transform& ws, const Transform& ps)
	{
		Transform ls;

		const Quaternion psConjugate = conjugate(ps.orientation);

		ls.position = (psConjugate * (ws.position - ps.position)) / ps.scale;
		ls.orientation = psConjugate * ws.orientation;
		ls.scale = psConjugate * (ws.scale / ps.scale);

		return ls;
	}

	Transform inverse(const Transform& t)
	{
		const Transform i;
		return i / t;
	}

	Matrix4 transformMatrix4(const Transform& t)
	{
		return Math::translate(t.position) * quaternionToMatrix4(t.orientation) * Math::scale(t.scale);
	}

}
