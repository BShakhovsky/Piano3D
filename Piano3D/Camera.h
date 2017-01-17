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
			DirectX::SimpleMath::Vector3(focus_.x, focus_.y, -focus_.z), up_));
	}

	bool Zoom(bool increase, bool precise = false);
	bool FitToWindow(const DirectX::SimpleMath::Matrix& projection);
	void MoveStart(int screenX, int screenY);
	void MoveEnd(int screenX, int screenY);
	void RotatePianoStart(int screenX, int screenY);
	void RotatePianoEnd(int screenX, int screenY);
	void RotateCamera(float xPitch, float yYaw, float zRoll);
private:
	void CalcFocus();
	bool IsInsideWindow(const DirectX::SimpleMath::Matrix& projection) const;

	static constexpr float deltaZoom_ = 0.1f, deltaMove_ = 1.0f, deltaRotate_ = 1.0f,
		minGap_ = 2.0f, maxDistance_ = 300.0f;

	int moveX_, moveY_, rotateX_, rotateY_;
	DirectX::SimpleMath::Vector3 position_, direction_, focus_, up_;
};