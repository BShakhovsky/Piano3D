#pragma once
#include "Device.h"
#include "Camera.h"
#include "Shader.h"
#include "Geometry.h"
#include "Text.h"
#include "BackBuffer2D.h"

class Render : boost::noncopyable
{
	Render() = delete;
public:
	explicit Render(HWND hWnd, UINT width, UINT height)
		: width_(width), height_(height),

		device_(std::make_shared<Device>(hWnd, width_, height_)),
		camera_(std::make_unique<Camera>(26.0f, 25.0f, 30.0f)),
		shader_(std::make_unique<Shader>(device_->GetDevice())),
		geometry_(std::make_unique<Geometry>(device_->GetContext())),
		text_(std::make_unique<Text>(device_->GetDevice(), device_->GetContext())),

		pianoDesk_(std::make_unique<BackBuffer2D>(device_->GetDevice(), device_->GetContext(),
			device_->GetDepthBuffer(), width_, height_)),
		keyboardReflection_(std::make_unique<BackBuffer2D>(device_->GetDevice(), device_->GetContext(),
			device_->GetDepthBuffer(), width_, height_, true))
	{
		Resize(width_, height_);
		camera_->Rotate(-40, 0, 0);
	}

	void Resize(UINT width, UINT height);
	void Draw() const;
private:
	void DrawScene(const DirectX::SimpleMath::Matrix& view) const;
	void DrawDebugInfo() const;

	UINT width_, height_;

	std::shared_ptr<Device> device_;
	const std::unique_ptr<Camera> camera_;
	const std::unique_ptr<Shader> shader_;
	const std::unique_ptr<Geometry> geometry_;
	const std::unique_ptr<Text> text_;

	std::unique_ptr<BackBuffer2D> pianoDesk_, keyboardReflection_;
};