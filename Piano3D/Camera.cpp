#include "stdafx.h"
#include "Camera.h"

using namespace std;
using namespace DirectX;
using namespace SimpleMath;

const float
Camera::deltaZoom_ = 0.1f, Camera::deltaMove_ = 1.0f, Camera::deltaRotate_ = 1.0f,
Camera::minGap_ = 2.0f, Camera::maxDistance_ = 300.0f;

Camera::Camera(const float positionX, const float positionY, const float positionZ)
	: position_(positionX, positionY, positionZ),
	direction_(Vector3::Forward),
	up_(Vector3::Up)
{
	if (position_.x < -maxDistance_) position_.x = -maxDistance_;
	else if (position_.x > maxDistance_) position_.x = maxDistance_;
	if (position_.y < -maxDistance_) position_.y = -maxDistance_;
	else if (position_.y > maxDistance_) position_.y = maxDistance_;
	if (position_.z < -maxDistance_) position_.z = -maxDistance_;
	else if (position_.z > maxDistance_) position_.z = maxDistance_;

	CalcFocus();
}

bool Camera::Zoom(const bool decrease)
{
	const auto pos(position_ + (focus_ - position_) * deltaZoom_ * (decrease ? -1.0f : 1.0f));
	if (min({ pos.x, pos.y, pos.z }) < minGap_ || max({ pos.x, pos.y, pos.z }) > maxDistance_) return false;
	position_ = pos;
	return true;
}
bool Camera::FitToWindow()
{
	return false;
}

void Camera::MoveStart(const int screenX, const int screenY)
{
	moveX_ = screenX;
	moveY_ = screenY;
}
void Camera::MoveEnd(const int screenX, const int screenY)
{
	UNREFERENCED_PARAMETER(screenX);
	UNREFERENCED_PARAMETER(screenY);
}
void Camera::RotatePianoStart(const int screenX, const int screenY)
{
	rotateX_ = screenX;
	rotateY_ = screenY;
}
void Camera::RotatePianoEnd(const int screenX, const int screenY)
{
	UNREFERENCED_PARAMETER(screenX);
	UNREFERENCED_PARAMETER(screenY);
}

void Camera::RotateCamera(const float xPitch, const float yYaw, const float zRoll)
{
	const auto rotationMatrix(Matrix::CreateFromYawPitchRoll(
		XMConvertToRadians(yYaw), XMConvertToRadians(xPitch), XMConvertToRadians(zRoll)));

	direction_ = Vector3::Transform(direction_, rotationMatrix);
	up_ = Vector3::Transform(up_, rotationMatrix);

	CalcFocus();
}

void Camera::CalcFocus()
{
	assert("Wrong camera position, it may cause division by zero"
		&& min(position_.y, position_.z) >= Camera::minGap_ && max(direction_.y, direction_.z) <= 0);
	const auto
		deltaX(abs(position_.x) > numeric_limits<float>::epsilon() ? direction_.x / position_.x : 0),
		deltaY(position_.y > numeric_limits<float>::epsilon() ? direction_.y / position_.y : 0),
		deltaZ(position_.z > numeric_limits<float>::epsilon() ? direction_.z / position_.z : 0);
	assert("Wrong camera position, it may cause division by zero"
		&& abs(min({ deltaX, deltaY, deltaZ })) > numeric_limits<float>::epsilon());
	const auto delta(1.0f / abs(min({ deltaX, deltaY, deltaZ })));
	focus_ = position_ + direction_ * delta;
}