#ifndef DUNJUN_CAMERA_HPP 
#define DUNJUN_CAMERA_HPP 

#include <Dunjun/Transform.hpp>

namespace Dunjun
{
	enum class ProjectionType
	{
		Perspective,
		InfinitePerspective,
		Orthographic,
	};


	struct Camera
	{
		Camera();
		Camera(const Camera& other) = default;
		//const Camera& operator=(const Camera& other) = default;

		void lookAt(const Vector3& position, const Vector3& up = {0, 1, 0});

		// positive values go counterclock-wise
		void offsetOrientation(const Radian& yaw, const Radian& pitch);

		Vector3 forward() const;
		Vector3 backward() const;

		Vector3 right() const;
		Vector3 left() const;

		Vector3 up() const;
		Vector3 down() const;

		Matrix4 getMatrix() const;
		Matrix4 getProjection() const;
		Matrix4 getView() const;

		Transform transform;
		Radian fieldOfView;
		f32 orthoScale;
		f32 nearPlane;
		f32 farPlane;

		Vector2 viewportSize;
		f32 viewportAspectRatio;

		ProjectionType projectionType;


	};
} // end Dunjun

#endif
