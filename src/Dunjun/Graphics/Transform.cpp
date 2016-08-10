
#include <Dunjun/Graphics/Transform.hpp>

namespace Dunjun
{
	const Transform Transform::Identity = Transform{Vector3::Zero, Quaternion::Identity, Vector3{1.0f, 1.0f, 1.0f}};

	// World Transform  = Parent * Local
	Transform operator*(const Transform& ps, const Transform& ls)
	{
		Transform ws = Transform::Identity;
		// correct order: scale >> rotate >> translate

		Vector3 absScale = ps.orientation * ls.scale;

		absScale.x = Math::abs(absScale.x);
		absScale.y = Math::abs(absScale.y);
		absScale.z = Math::abs(absScale.z);

		ws.position = ps.position + ps.orientation * (ps.scale * ls.position); // <-- This is correct
		ws.orientation = ps.orientation * ls.orientation;
		ws.scale = ps.scale * absScale;
		//ws.scale = ps.scale * (conjugate(ps.orientation) * ls.scale);

		return ws;
	}

	// Local Transform  = World / Parent
	Transform operator/(const Transform& ws, const Transform& ps)
	{
		Transform ls = Transform::Identity;

		const Quaternion psConjugate = conjugate(ps.orientation);

		ls.position = (psConjugate * (ws.position - ps.position)) / ps.scale;
		ls.orientation = psConjugate * ws.orientation;
		ls.scale = psConjugate * (ws.scale / ps.scale);
		//ls.scale = ls.orientation * (ws.scale / ps.scale);

		return ls;
	}

	Transform inverse(const Transform& t)
	{
		const Quaternion invOrientation = conjugate(t.orientation);

		Transform invTransform = Transform::Identity;

		invTransform.position = (invOrientation * -t.position) / t.scale;
		invTransform.orientation = invOrientation;
		//invTransform.scale = invOrientation * (Vector3(1) / t.scale);
		invTransform.scale = invOrientation * (Vector3{1, 1, 1} / t.scale);

		return invTransform;
	}

	Matrix4 transformMatrix4(const Transform& t)
	{
		return Math::translate(t.position) * 
			   quaternionToMatrix4(t.orientation) * 
			   Math::scale(t.scale);
	}

}
