#ifndef DUNJUN_VERTEX_HPP
#define DUNJUN_VERTEX_HPP

#include <Dunjun/Color.hpp>

namespace Dunjun
{
	struct Vertex // must come before render
	{
		Vector3 position;
		Color color;
		Vector2 texCoord;
	};
} // end Dunjun

#endif
