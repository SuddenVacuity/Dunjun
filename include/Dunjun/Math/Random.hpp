#ifndef DUNJUN_MATH_RANDOM_HPP
#define DUNJUN_MATH_RANDOM_HPP

#include <Dunjun/Math/Functions/Interpolation.hpp>

namespace Dunjun
{
	class Random
	{
	public:
		Random() = default;
		Random(std::mt19937::result_type seed) 
			: m_engine(seed)
		{
		}

		void setSeed(std::mt19937::result_type seed)
		{
			m_engine.seed(seed);
		}

		// inclusive
		s32 getInt(s32 min, s32 max)
		{
			assert(!(min > max));
			std::uniform_int_distribution<s32> dist(min, max);
			return dist(m_engine);
		}

		// inclusive
		f32 getFloat(f32 min, f32 max)
		{
			assert(!(min > max));
			std::uniform_real_distribution<f32> dist(min, max);
			return dist(m_engine);
		}

		b8 getBool()
		{
			return getInt(0, 1) == 1;
		}

	private:
		std::mt19937 m_engine{std::random_device{}()};
	};
} // end Dunjun

#endif
