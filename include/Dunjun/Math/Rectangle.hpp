#ifndef DUNJUN_MATH_RECTANGLE_HPP
#define DUNJUN_MATH_RECTANGLE_HPP

#include <Dunjun/Math/Functions.hpp>

namespace Dunjun
{
	struct Rectangle
	{
		f32 x = 0;
		f32 y = 0;
		f32 width = 0;
		f32 height = 0;

		Rectangle() = default;
		Rectangle(const Rectangle& other) = default;

		Rectangle(f32 x, f32 y, f32 width, f32 height);
		Rectangle(const Vector2& position, const Vector2& size);

		bool contains(const Vector2& point) const;
		bool intersects(const Rectangle& rectangle) const;
		bool intersects(const Rectangle& rectangle, Rectangle& intersection) const;
	};

	bool operator==(const Rectangle& left, const Rectangle& right);
	bool operator!=(const Rectangle& left, const Rectangle& right);


} // end Dunjun

#endif
