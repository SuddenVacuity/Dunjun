
#include <Dunjun/Scene/MeshRenderer.hpp>
#ifdef DUNJUN_SCENE_FACE_CAMERA_HPP
#define DUNJUN_SCENE_FACE_CAMERA_HPP

namespace Dunjun
{
	struct FaceCamera : public NodeComponent
	{
	public:
		explicit FaceCamera(const Camera& camera, const Vector3& up = {0, 1, 0})
			: camera(&camera)
			, up(up)
		{
		}

		virtual void update(Time dt) override;

		const Camera* camera;
		Vector3 up;
	};
} // end Dunjun

#endif
