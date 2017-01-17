#pragma once

class Camera : boost::noncopyable
{
	Camera() = delete;
public:
	explicit Camera(float positionX, float positionY, float positionZ);

	const DirectX::SimpleMath::Vector3& GetPosition() const
	{
		return position_;
	}
	DirectX::SimpleMath::Matrix GetViewMatrix() const
	{
		return std::move(DirectX::SimpleMath::Matrix::CreateLookAt(position_, focus_, up_));
	}
	DirectX::SimpleMath::Matrix GetMirrowedViewMatrix() const
	{
		return std::move(DirectX::SimpleMath::Matrix::CreateLookAt(
			DirectX::SimpleMath::Vector3(position_.x, position_.y, -position_.z),
			DirectX::SimpleMath::Vector3(focus_.x, focus_.y, -focus_.z),
			DirectX::SimpleMath::Vector3(up_.x, up_.y, -up_.z)));
	}

	bool Zoom(bool increase, bool precise = false);
	bool FitToWindow(const DirectX::SimpleMath::Matrix& projection);

	void MoveStart(float screenX, float screenY)
	{
		assert("Wrong screen coordinates"
			&& std::min(screenX, screenY) >= 0 && std::max(screenX, screenY) <= 1);
		moveX_ = screenX;
		moveY_ = screenY;
	}
	void RotatePianoStart(float screenX, float screenY)
	{
		assert("Wrong screen coordinates"
			&& std::min(screenX, screenY) >= 0 && std::max(screenX, screenY) <= 1);
		rotateX_ = screenX;
		rotateY_ = screenY;
	}
	void MoveEnd(float screenX, float screenY);
	void RotatePianoEnd(float screenX, float screenY);

	void RotateCamera(float xPitch, float yYaw, float zRoll);
private:
	bool IsCorrectPosition(const DirectX::SimpleMath::Vector3& position,
		const DirectX::SimpleMath::Vector3& direction, const DirectX::SimpleMath::Vector3& up) const;
	bool IsCorrectRotation(const DirectX::SimpleMath::Matrix& rotation) const;
	bool IsInsideWindow(const DirectX::SimpleMath::Matrix& projection) const;
	void CalcFocus();

	static constexpr float deltaZoom_ = 0.1f, deltaMove_ = 1.0f, deltaRotate_ = 2.0f,
		minGap_ = 2.0f, maxDistance_ = 300.0f, minAngle = 1.0f;
	static const DirectX::SimpleMath::Vector3 xMove_;

	float moveX_, moveY_, rotateX_, rotateY_;
	DirectX::SimpleMath::Vector3 position_, direction_, focus_, up_;
};