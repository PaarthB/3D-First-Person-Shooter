#include "ThirdPersonCamera.h"

ThirdPersonCamera::ThirdPersonCamera()
{
	m_catchupMode = false;
	m_catchupSpeed = 1.0f;
	m_objectToFollow = NULL;
	m_offset = Vector3(0.0f, 10.0f, -10.0f);

	SetPosition(m_objectToFollow->GetPosition() + TransformOffset());
}

ThirdPersonCamera::ThirdPersonCamera(GameObject* target, Vector3 offset, bool catchup, float catchupSpeed)
{
	m_catchupMode = catchup;
	m_catchupSpeed = catchupSpeed;
	m_objectToFollow = target;
	m_offset = offset;

	// Set starting position so catchup mode doesn't show camera animating from the origin
	SetPosition(m_objectToFollow->GetPosition() + TransformOffset());
}

void ThirdPersonCamera::Update(float timestep)
{
	if (m_objectToFollow != NULL)
	{
		// We'll look at the object we're following
		// Sometimes we'll even add an offset here too 
		SetLookAt(m_objectToFollow->GetPosition());

		// Not storing this directly in m_offset as it would accumulate over time and spin out of control
		Vector3 rotatedOffset = TransformOffset();

		if (m_catchupMode)
		{
			// Catchup to target position. A simple LERP will do.
			SetPosition(Vector3::Lerp(GetPosition(), m_objectToFollow->GetPosition() + rotatedOffset, timestep * m_catchupSpeed));
		}
		else
		{
			// Jump directly to target position (including offset)
			SetPosition(m_objectToFollow->GetPosition() + rotatedOffset);
		}
	}

	// Give our parent a chance to regenerate the view matrix
	Camera::Update(timestep);
}

Vector3 ThirdPersonCamera::TransformOffset()
{
	// Wrap up the Y rotation of the object we're following in a matrix
	Matrix rotation = Matrix::CreateRotationY(m_objectToFollow->GetYRotation());

	// Transform our offset vector by the rotation so the offset rotates with our follow object
	return Vector3::TransformNormal(m_offset, rotation);
}

