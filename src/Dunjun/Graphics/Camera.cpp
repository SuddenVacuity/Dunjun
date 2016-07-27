
#include <Dunjun/Graphics/Camera.hpp>

namespace Dunjun
{
	// Functions

	void Camera::cameraLookAt(const Vector3& target, const Vector3& up)
	{
		transform.orientation = conjugate(Math::lookAtQuaternion(transform.position, target, up));
	}

	// helper functions
	Matrix4 Camera::getMatrix() const
	{
		return getProjection() * getView();
	}

	Matrix4 Camera::getProjection() const
	{
		Matrix4 proj = Matrix4::Identity;

		if (projectionType == ProjectionType::Perspective)
		{
			proj = Math::perspective(fieldOfView, viewportAspectRatio, nearPlane, farPlane);
		}
		else if (projectionType == ProjectionType::InfinitePerspective)
		{
			proj = Math::infinitePerspective(fieldOfView, viewportAspectRatio, nearPlane);
		}
		else if (projectionType == ProjectionType::Orthographic)
		{
			f32 distance = 0.5f * (farPlane - nearPlane);

			proj = Math::ortho(-orthoScale * viewportAspectRatio, 
						  orthoScale * viewportAspectRatio, 
						 -orthoScale, orthoScale, 
						 -distance, distance);
		}
		else
			std::runtime_error("No ProjectionType Camera::getProjcetion()\n");

		return proj;
	}

	Matrix4 Camera::getView() const
	{
		Matrix4 view = Matrix4::Identity;

		view = Math::scale(Vector3{1, 1, 1} / transform.scale);

		view = view * quaternionToMatrix4(conjugate(transform.orientation));

		view = view * Math::translate(-transform.position);

		return view;
	}
	
} // end Dunjun
