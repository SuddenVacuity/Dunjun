#ifndef DUNJUN_GUI_GUI_HPP
#define DUNJUN_GUI_GUI_HPP

#include <Dunjun/Graphics/Material.hpp>

namespace Dunjun
{
	struct GUIobject
	{
		u32 flags;
		Rectangle region;
		Material* material;
		u32 returnValue;
	};



} // end Dunjun

#endif
