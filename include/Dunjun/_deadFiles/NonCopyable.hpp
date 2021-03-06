#ifndef DUNJUN_SYSTEM_NONCOPYABLE_HPP
#define DUNJUN_SYSTEM_NONCOPYABLE_HPP

#include <Dunjun/ResourceHolder.hpp>

// this keeps shaders and texture from being copied
namespace Dunjun
{
	class NonCopyable
	{
	protected:
		NonCopyable() {}
	private:
		NonCopyable(const NonCopyable&) = delete;
		NonCopyable& operator=(const NonCopyable&) = delete;

		//NonCopyable(NonCopyable&&) = delete;
		//NonCopyable& operator=(NonCopyable&&) = delete;

	};
}

#endif
