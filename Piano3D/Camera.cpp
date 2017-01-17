#include "stdafx.h"
#include "Camera.h"
#include "Geometry.h"

using namespace std;
using namespace DirectX;
using namespace SimpleMath;

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

bool Camera::Zoom(const bool increase, const bool precise)
{
	const auto pos(position_ + (focus_ - position_) * deltaZoom_
		* (increase ? 1.0f : -1.0f) * (precise ? 0.1f : 1.0f));
	if (min({ pos.x, pos.y, pos.z }) < minGap_ || max({ pos.x, pos.y, pos.z }) > maxDistance_) return false;
	position_ = pos;
	return true;
}
bool Camera::FitToWindow(const Matrix& projection)
{
	if (IsInsideWindow(projection))
	{
		Zoom(true);
		if (IsInsideWindow(projection)) return false;
		else while (!IsInsideWindow(projection)) Zoom(false, true);
	}
	else
	{
		Zoom(false);
		if (!IsInsideWindow(projection)) return false;
		else
		{
			while (IsInsideWindow(projection)) Zoom(true, true);
			Zoom(false, true);
		}
	}
	return true;
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
		&& max({ abs(position_.x - Geometry::keyboardLength / 2), position_.y, position_.z }) <= maxDistance_
		&& min(position_.y, position_.z) >= minGap_ && max(direction_.y, direction_.z) <= 0);
	const auto
		deltaX(abs(position_.x) > numeric_limits<float>::epsilon() ? direction_.x / position_.x : 0),
		deltaY(position_.y > numeric_limits<float>::epsilon() ? direction_.y / position_.y : 0),
		deltaZ(position_.z > numeric_limits<float>::epsilon() ? direction_.z / position_.z : 0);
	assert("Wrong camera position, it may cause division by zero"
		&& abs(min({ deltaX, deltaY, deltaZ })) > numeric_limits<float>::epsilon());
	const auto delta(1.0f / abs(min({ deltaX, deltaY, deltaZ })));

	focus_ = position_ + direction_ * delta;
}
bool Camera::IsInsideWindow(const Matrix& projection) const
{
	static const Vector3 borderPoint1(0, 0, Geometry::GetKeyboardWidth());
	auto screenPoint(Vector3::Transform(Vector3::Transform(borderPoint1, GetViewMatrix()), projection));
	if (screenPoint.x < -1 || screenPoint.x > 1 || screenPoint.y < -1 || screenPoint.y > 1)
		return false;

	static const Vector3 borderPoint2(Geometry::keyboardLength, 0, Geometry::GetKeyboardWidth());
	screenPoint = Vector3::Transform(Vector3::Transform(borderPoint2, GetViewMatrix()), projection);
	if (screenPoint.x < -1 || screenPoint.x > 1 || screenPoint.y < -1 || screenPoint.y > 1)
		return false;

	static const Vector3 borderPoint3(-(Geometry::deskLength - Geometry::keyboardLength) / 2,
		Geometry::deskHeight, -Geometry::deskThickness);
	screenPoint = Vector3::Transform(Vector3::Transform(borderPoint3, GetViewMatrix()), projection);
	if (screenPoint.x < -1 || screenPoint.x > 1 || screenPoint.y < -1 || screenPoint.y > 1)
		return false;

	static const Vector3 borderPoint4(Geometry::deskLength - (Geometry::deskLength - Geometry::keyboardLength) / 2,
		Geometry::deskHeight, -Geometry::deskThickness);
	screenPoint = Vector3::Transform(Vector3::Transform(borderPoint4, GetViewMatrix()), projection);
	if (screenPoint.x < -1 || screenPoint.x > 1 || screenPoint.y < -1 || screenPoint.y > 1)
		return false;
	
	return true;
}