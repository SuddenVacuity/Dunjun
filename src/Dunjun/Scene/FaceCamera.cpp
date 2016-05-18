
#include <Dunjun/Scene/FaceCamera.hpp>

namespace Dunjun
{
	void FaceCamera::update(f32 dt)
	{
		Vector3 pos = parent->getGlobalTransform().position;
		Vector3 camPos = camera->transform.position;

		parent->transform.orientation = conjugate(lookAt<Quaternion>(camPos, pos, up));
	}
} // end Dunjun
