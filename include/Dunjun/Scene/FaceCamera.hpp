#ifndef DUNJUN_SCENE_FACE_CAMERA_HPP
#define DUNJUN_SCENE_FACE_CAMERA_HPP

#include <Dunjun/Scene/MeshRenderer.hpp>

namespace Dunjun
{
	class FaceCamera : public NodeComponent
	{
	public:
		explicit FaceCamera(const Camera& camera, const Vector3& up = {0, 1, 0})
			: camera(&camera)
			, up(up)
		{
		}

		virtual void update(f32 dt) override;

		const Camera* camera;
		Vector3 up;
	};
} // end Dunjun

#endif
