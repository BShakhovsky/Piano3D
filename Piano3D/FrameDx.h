#pragma once
#include "Device.h"

class FrameDx : boost::noncopyable
{
	FrameDx() = delete;
public:
	explicit FrameDx(const std::shared_ptr<Device>& device, const DirectX::SimpleMath::Color& color)
		: device_(device)
	{
		device_->BeginFrame(color);
	}
	~FrameDx()
	{
		device_->EndFrame();
	}
private:
	std::shared_ptr<Device> device_;
};