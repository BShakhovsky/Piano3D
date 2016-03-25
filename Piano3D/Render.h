#pragma once
#include "Device.h"
#include "Camera.h"
#include "Shader.h"
#include "Geometry.h"
#include "BackBuffer2D.h"

class Render : boost::noncopyable
{
	Render() = delete;
public:
	explicit Render(HWND hWnd, UINT width, UINT height)
		: device_(std::make_shared<Device>(hWnd, width, height)),
		camera_(std::make_unique<Camera>(26.0f, 25.0f, 30.0f)),
		shader_(std::make_unique<Shader>(device_->GetDevice(), device_->GetContext())),
		geometry_(std::make_unique<Geometry>(device_->GetContext())),
		imageBuffer_(std::make_unique<BackBuffer2D>(device_->GetDevice(), device_->GetContext(), device_->GetDepthBuffer(),
			width, height))
	{
		Resize(width, height);
		camera_->Rotate(-40, 0, 0);
	}

	void Resize(UINT width, UINT height);

	///////////////////////////////////////////////////////////
	const DirectX::SimpleMath::Vector3& GetPosition() const
	{
		return camera_->GetPosition();
	}
	///////////////////////////////////////////////////////////

	void Draw() const;
private:
	void DrawScene(const DirectX::SimpleMath::Matrix& view) const;

	std::shared_ptr<Device> device_;
	const std::unique_ptr<Camera> camera_;
	const std::unique_ptr<Shader> shader_;
	const std::unique_ptr<Geometry> geometry_;
	std::unique_ptr<BackBuffer2D> imageBuffer_;
};