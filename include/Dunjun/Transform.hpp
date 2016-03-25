#ifndef DUNJUN_TRANSFORM_HPP 
#define DUNJUN_TRANSFORM_HPP 

#include <Dunjun/Math.hpp>

namespace Dunjun
{
	struct Transform
	{
		Vector3 position;
		Quaternion orientation;
		Vector3 scale;

		Transform()
			: position({0,0,0})
			, orientation(Quaternion(0, 0, 0, 1))
			, scale({1, 1, 1})
		{
		}

		// World Transform  = Parent * Local
		Transform operator*(const Transform& ls) const
		{
			const Transform& ps = *this;
			Transform ws;
			// correct order: scale >> rotate >> translate

			ws.position = ps.position + ps.orientation * (ps.scale * ls.position);
			ws.orientation = ps.orientation * ls.orientation;
			ws.scale = ps.scale * (ps.orientation * ls.scale);

			return ws;
		}


		Transform operator/(const Transform& ps) const
		{
			const Transform& ws = *this;
			Transform ls;

			const Quaternion psConjugate = conjugate(ps.orientation);

			ls.position = (psConjugate * (ws.position - ps.position)) / ps.scale;
			ls.orientation = psConjugate * ws.orientation;
			ls.scale = psConjugate * (ws.scale / ps.scale);

			return ls;
		}

	};
	inline Transform inverse(const Transform& t)
	{
		const Transform i;
		return i / t;
	}

} // end Dunjun

#endif
