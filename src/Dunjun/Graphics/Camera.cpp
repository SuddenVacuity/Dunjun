
#include <Dunjun/Graphics/Camera.hpp>

namespace Dunjun
{
	// Functions
	// TODO: FIX: changing lookAt position does not act as expected
	void Camera::lookAt(const Vector3& position, const Vector3& up)
	{
		transform.orientation = conjugate(Math::lookAt<Quaternion>(transform.position, position, up));
		//transform.orientation = conjugate(matrix4ToQuaternion(Dunjun::lookAt<Matrix4>(transform.position, position, up)));
	}

	void Camera::offsetOrientation(const Radian& yaw, const Radian& pitch)
	{
		const Quaternion yawRot = angleAxis(yaw, {0, 1, 0}); // absolute up
		const Quaternion pitchRot = angleAxis(pitch, right()); // relative right

		transform.orientation = yawRot * pitchRot * transform.orientation;
	}

	// move camera functions
	// must be normalized
	Vector3 Camera::forward() const
	{
		return transform.orientation * Vector3(0, 0, -1);
	}
	Vector3 Camera::backward() const
	{
		return transform.orientation * Vector3(0, 0, 1);
	}

	Vector3 Camera::right() const
	{
		return transform.orientation * Vector3(1, 0, 0);
	}
	Vector3 Camera::left() const
	{
		return transform.orientation * Vector3(-1, 0, 0);
	}

	Vector3 Camera::up() const
	{
		return transform.orientation * Vector3(0, 1, 0);
	}
	Vector3 Camera::down() const
	{
		return transform.orientation * Vector3(0, -1, 0);
	}
	
	//
	// helper functions
	Matrix4 Camera::getMatrix() const
	{
		return getProjection() * getView();
	}

	Matrix4 Camera::getProjection() const
	{
		Matrix4 proj;

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
			std::runtime_error("No ProjectionType [-getProjcetion()-]\n");

		return proj;
	}

	Matrix4 Camera::getView() const
	{
		Matrix4 view;

		view =	Math::scale(Vector3(1) / transform.scale) * 
				quaternionToMatrix4(conjugate(transform.orientation)) * Math::translate(-transform.position);

		return view;
	}
	
} // end Dunjun
