#pragma once

class Camera : boost::noncopyable
{
	Camera() = delete;
public:
	explicit Camera(float positionX, float positionY, float positionZ)
		: position_(positionX, positionY, positionZ),
		focus_(position_ + DirectX::SimpleMath::Vector3::Forward),
		up_(DirectX::SimpleMath::Vector3::Up)
	{}

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

	///////////////////////////////////////////////////////
	const DirectX::SimpleMath::Vector3& GetPosition() const
	{
		return position_;
	}
	///////////////////////////////////////////////////////

	void Rotate(float xPitch, float yYaw, float zRoll);
private:
	DirectX::SimpleMath::Vector3 position_, focus_, up_;
};