#include "stdafx.h"
#include "Camera.h"
#include "Geometry.h"

using namespace std;
using namespace DirectX;
using namespace SimpleMath;

const Vector3 Camera::xMove_ = Vector3(Geometry::keyboardLength / 2, 0, 0);

Camera::Camera()
	: position_(Vector3(Geometry::keyboardLength / 2, 25.0f, 30.0f)),
	direction_(Vector3::Forward),
	up_(Vector3::Up)
{
	const auto rotationMatrix(Matrix::CreateFromYawPitchRoll(0, XMConvertToRadians(-40), 0));
	direction_ = Vector3::Transform(direction_, rotationMatrix);
	up_ = Vector3::Transform(up_, rotationMatrix);
	
	CalcFocus();

	defPos_ = position_;
	defFocus_ = focus_;
	defDir_ = direction_;
	defUp_ = up_;
}

bool IsCorrectPosition(const Vector3& position, const Vector3& direction, const Vector3& up)
{
	static constexpr float minGap(2.0f), maxDistance(300.0f), minAngle(0.01f);

	return max({ abs(position.x - Geometry::keyboardLength / 2), position.y, position.z }) <= maxDistance
		&& min(position.y, position.z) >= minGap && abs(direction.x) < 1 - minAngle && abs(up.x) < 1 - minAngle
		&& min(direction.y, direction.z) > minAngle - 1 && max(direction.y, direction.z) < -minAngle
		&& up.y > minAngle && up.y < 1 - minAngle && up.z > minAngle - 1 && up.z < -minAngle;
}
bool Camera::Zoom(const bool increase, const bool precise)
{
	static constexpr float deltaZoom(0.1f);

	const auto pos(position_ + (focus_ - position_) * deltaZoom
		* (increase ? 1.0f : -1.0f) * (precise ? 0.1f : 1.0f));
	if (!IsCorrectPosition(pos, direction_, up_)) return false;

	position_ = pos;
	return true;
}
bool Camera::FitToWindow(const Matrix& projection)
{
	if (IsInsideWindow(projection))
		if (!Zoom(true)) return true;
		else if (IsInsideWindow(projection)) return false;
		else while (!IsInsideWindow(projection)) Zoom(false, true);
	else
		if (!Zoom(false)) return true;
		else if (!IsInsideWindow(projection)) return false;
		else
		{
			while (IsInsideWindow(projection)) Zoom(true, true);
			Zoom(false, true);
		}
	return true;
}

void Camera::MoveEnd(const float screenX, const float screenY)
{
	assert("Wrong screen coordinates" && min(screenX, screenY) >= 0 && max(screenX, screenY) <= 1);

//	static constexpr float deltaMove(1.0f);

	MoveStart(screenX, screenY);
}
void Camera::RotatePianoEnd(const float screenX, const float screenY)
{
	assert("Wrong screen coordinates" && min(screenX, screenY) >= 0 && max(screenX, screenY) <= 1);

	static constexpr float deltaRotate(2.0f);

	auto xAxis(up_.Cross(position_ - focus_));
	xAxis.Normalize();
	const auto deltaX(rotateX_ - screenX), deltaY(rotateY_ - screenY),
		xPitch((xAxis.x * deltaY + up_.x * deltaX) * deltaRotate),
		yYaw((xAxis.y * deltaY + up_.y * deltaX) * deltaRotate),
		zRoll((xAxis.z * deltaY + up_.z * deltaX) * deltaRotate);

	auto rotationMatrix(Matrix::CreateFromYawPitchRoll(yYaw, xPitch, zRoll));
	if (!IsCorrectRotation(rotationMatrix))
	{
		rotationMatrix = Matrix::CreateFromYawPitchRoll(0, xPitch, zRoll);
		if (!IsCorrectRotation(rotationMatrix))
		{
			rotationMatrix = Matrix::CreateFromYawPitchRoll(yYaw, 0, zRoll);
			if (!IsCorrectRotation(rotationMatrix))
			{
				rotationMatrix = Matrix::CreateFromYawPitchRoll(0, 0, zRoll);
				if (!IsCorrectRotation(rotationMatrix))
					rotationMatrix = Matrix::CreateFromYawPitchRoll(0, 0, 0);
			}
		}
	}

	position_ -= xMove_;
	position_ = Vector3::Transform(position_, rotationMatrix);
	direction_ = Vector3::Transform(direction_, rotationMatrix);
	up_ = Vector3::Transform(up_, rotationMatrix);
	position_ += xMove_;

	CalcFocus();
	
	RotatePianoStart(screenX, screenY);
}

bool RestoreOneVector(Vector3& vec, const Vector3& target, float delta, float deltaMin)
{
	auto dir(target - vec);
	if (dir.Length() > deltaMin)
	{
		dir *= delta;
		if (dir.Length() < deltaMin)
		{
			dir.Normalize();
			dir *= deltaMin;
		}
		vec += dir;
		return false;
	}
	else
	{
		vec = target;
		return true;
	}
}
bool Camera::RestorePosition()
{
	const auto subResult(RestoreOneVector(position_, defPos_, 0.1f, 1.0f));
	auto result(RestoreOneVector(focus_, defFocus_, 0.1f, 1.0f) && subResult);

	result = RestoreOneVector(direction_, defDir_, 0.01f, 0.1f) && result;
	return RestoreOneVector(up_, defUp_, 0.01f, 0.1f) && result;
}

bool Camera::IsCorrectRotation(const Matrix& rotation) const
{
	const auto testDir(Vector3::Transform(direction_, rotation)), testUp(Vector3::Transform(up_, rotation));
	auto testPos(position_ - xMove_);
	testPos = Vector3::Transform(testPos, rotation);
	testPos += xMove_;
	return IsCorrectPosition(testPos, testDir, testUp);
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
void Camera::CalcFocus()
{
	assert("Wrong camera position, it may cause division by zero"
		&& IsCorrectPosition(position_, direction_, up_));
	const auto
		deltaY(position_.y > numeric_limits<float>::epsilon() ? direction_.y / position_.y : 0),
		deltaZ(position_.z > numeric_limits<float>::epsilon() ? direction_.z / position_.z : 0);
	assert("Wrong camera position, it may cause division by zero"
		&& deltaY < -numeric_limits<float>::epsilon() && deltaZ < -numeric_limits<float>::epsilon());
	const auto delta(1.0f / min(deltaY, deltaZ));

	focus_ = position_ - direction_ * delta;
}