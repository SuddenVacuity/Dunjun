
#include <Dunjun/Scene/FaceCamera.hpp>

namespace Dunjun
{
	void FaceCamera::update(f32 dt)
	{
		Vector3 pos = parent->getGlobalTransform().position;
		Vector3 camPos = camera->transform.position;

		Matrix4 orientation = matrix4LookAt(camPos, pos, up);

		parent->transform.orientation = conjugate(matrix4ToQuaternion(orientation));
	}
} // end Dunjun
