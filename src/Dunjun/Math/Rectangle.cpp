#include <Dunjun/Math/Rectangle.hpp>

namespace Dunjun
{

	Rectangle::Rectangle(f32 x, f32 y, f32 width, f32 height)
		: x(x)
		, y(y)
		, width(width)
		, height(height)
	{
	}

	Rectangle::Rectangle(const Vector2& position, const Vector2& size)
		: x(position.x)
		, y(position.y)
		, width(size.x)
		, height(size.y)
	{
	}

	bool Rectangle::contains(const Vector2& point) const
	{
		f32 minX = Math::min(x, x + width);
		f32 maxX = Math::max(x, x + width);
		f32 minY = Math::min(y, y + height);
		f32 maxY = Math::max(y, y + height);

		return (point.x >= minX) && (point.x < maxX) && (point.y >= minY) && (point.y < maxY);
	}

	bool Rectangle::intersects(const Rectangle& rectangle) const
	{
		Rectangle intersection;
		return intersects(rectangle, intersection);
	}

	bool Rectangle::intersects(const Rectangle& rectangle, Rectangle& intersection) const
	{
		f32 r1MinX = Math::min(x, x + width);
		f32 r1MaxX = Math::max(x, x + width);
		f32 r1MinY = Math::min(y, y + height);
		f32 r1MaxY = Math::max(y, y + height);
					
		f32 r2MinX = Math::min(rectangle.x, rectangle.x + rectangle.width);
		f32 r2MaxX = Math::max(rectangle.x, rectangle.x + rectangle.width);
		f32 r2MinY = Math::min(rectangle.y, rectangle.y + rectangle.height);
		f32 r2MaxY = Math::max(rectangle.y, rectangle.y + rectangle.height);

		f32 x1 = Math::max(r1MinX, r2MinX);
		f32 x2 = Math::min(r1MaxX, r2MaxX);
				 
		f32 y1 = Math::max(r1MinY, r2MinY);
		f32 y2 = Math::min(r1MaxY, r2MaxY);

		// if intersection is valid(positive non zero area)
		if((x1 < x2) && (y1 < y2))
		{
			intersection = Rectangle(x1, y1, x2-x1, y2-y1);
			return true;
		}
		else
		{
			intersection = Rectangle(0, 0, 0, 0);
			return false;
		}
			
	}

	bool operator==(const Rectangle& left, const Rectangle& right)
	{
		return (left.x == right.x) && (left.y == right.y) && (left.width == right.width) && (left.height == right.height);
	}

	bool operator!=(const Rectangle& left, const Rectangle& right)
	{
		return !(left == right);
	}


} // end Dunjun