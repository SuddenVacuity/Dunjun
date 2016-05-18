#ifndef DUNJUN_DRAWABLE_HPP
#define DUNJUN_DRAWABLE_HPP

#include <Dunjun/Renderer.hpp>

namespace Dunjun
{
	class Drawable
	{
	public:
	protected:
		virtual void draw(Renderer& renderer, Transform t) const = 0;

	private:
		friend class Renderer;
	};
} // end Dunjun

#endif
