
#include <Dunjun/Scene/FaceCamera.hpp>

namespace Dunjun
{
	void FaceCamera::update(Time dt)
	{
		Vector3 pos = getParent()->getGlobalTransform().position;
		Vector3 camPos = camera->transform.position;

		getParent()->transform.orientation = conjugate(Math::lookAt<Quaternion>(camPos, pos, up));
	}
} // end Dunjun
