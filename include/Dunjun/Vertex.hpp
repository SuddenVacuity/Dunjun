#ifndef DUNJUN_VERTEX_HPP
#define DUNJUN_VERTEX_HPP

#include <Dunjun/Color.hpp>

namespace Dunjun
{
	struct Vertex // must come before render
	{
		Vector3 position;
		Vector2 texCoord;
		Color color;
	};
} // end Dunjun

#endif
