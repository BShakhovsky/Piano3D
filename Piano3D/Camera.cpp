#include "stdafx.h"
#include "Camera.h"

void Camera::Rotate(const float xPitch, const float yYaw, const float zRoll)
{
	using namespace DirectX;
	using namespace SimpleMath;

	const auto rotationMatrix(Matrix::CreateFromYawPitchRoll(
		XMConvertToRadians(yYaw), XMConvertToRadians(xPitch), XMConvertToRadians(zRoll)));

	focus_ = position_ + Vector3::Transform(focus_ - position_, rotationMatrix);
	up_ = Vector3::Transform(up_, rotationMatrix);
}