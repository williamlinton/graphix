#include "Camera.h"

void Camera::SetPosition(float x, float y, float z)
{
	_positionX = x;
	_positionY = y;
	_positionZ = z;
}

// Travel the given distance in all directions relative to the camera.
void Camera::Travel(float x, float y, float z)
{
	DirectX::XMFLOAT3 travel;
	travel.x = x;
	travel.y = y;
	travel.z = z;
	DirectX::FXMVECTOR travelSimd = DirectX::XMLoadFloat3(&travel);

	// Figure out the current rotation matrix (from saved state)
	float pitch = radians(_rotationX);
	float yaw	= radians(_rotationY);
	float roll	= radians(_rotationZ);
	auto rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll);
	
	// Rotate the travel vector so it's pointing in the same direction as the camera
	auto travelTransformedSimd = DirectX::XMVector3TransformCoord(travelSimd, rotationMatrix);
	DirectX::XMFLOAT3 travelTransformed;
	DirectX::XMStoreFloat3(&travelTransformed, travelTransformedSimd);

	// Update the position with the rotated travel vector
	_positionX += travelTransformed.x;
	_positionY += travelTransformed.y;
	_positionZ += travelTransformed.z;
}

void Camera::SetRotation(float x, float y, float z)
{
	_rotationX = x;
	_rotationY = y;
	_rotationZ = z;
}

void Camera::Render()
{
	DirectX::XMFLOAT3 position;

	position.x = _positionX;
	position.y = _positionY;
	position.z = _positionZ;

	auto up		= DirectX::XMFLOAT3(0.0, 1.0, 0.0);
	auto lookAt = DirectX::XMFLOAT3(0.0, 0.0, 1.0);

	DirectX::FXMVECTOR upSimd = DirectX::XMLoadFloat3(&up);
	DirectX::FXMVECTOR lookAtSimd = DirectX::XMLoadFloat3(&lookAt);
	DirectX::FXMVECTOR positionSimd = DirectX::XMLoadFloat3(&position);

	float pitch = radians(_rotationX);
	float yaw	= radians(_rotationY);
	float roll	= radians(_rotationZ);
	auto rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	auto lookAtTransformed = DirectX::XMVector3TransformCoord(lookAtSimd, rotationMatrix);
	auto upTransformed = DirectX::XMVector3TransformCoord(upSimd, rotationMatrix);

	lookAtTransformed = DirectX::XMVectorAdd(positionSimd, lookAtTransformed);

	_viewMatrix = DirectX::XMMatrixLookAtLH(positionSimd, lookAtTransformed, upTransformed);
}

void Camera::GetViewMatrix(DirectX::XMMATRIX* viewMatrix)
{
	*viewMatrix = _viewMatrix;
}
