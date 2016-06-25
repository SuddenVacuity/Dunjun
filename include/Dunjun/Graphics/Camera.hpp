#ifndef DUNJUN_GRAPHICS_CAMERA_HPP 
#define DUNJUN_GRAPHICS_CAMERA_HPP 

#include <Dunjun/Graphics/Transform.hpp>

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
		Transform transform = Transform();

		Radian fieldOfView = Degree(50.0f);
		f32 orthoScale = 4.0f;
		f32 nearPlane = 0.01f;
		f32 farPlane = 256.0f;
		Vector2 viewportSize = {0, 0};
		f32 viewportAspectRatio = 4.0f / 3.0f;
		ProjectionType projectionType = ProjectionType::Perspective;


		Camera() = default;
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
	};
} // end Dunjun

#endif
