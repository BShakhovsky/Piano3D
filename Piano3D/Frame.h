#pragma once
#include "Device.h"

class Frame : boost::noncopyable
{
	Frame() = delete;
public:
	explicit Frame(const std::shared_ptr<Device>& device, const DirectX::SimpleMath::Color& color)
		: device_(device)
	{
		device_->BeginFrame(color);
	}
	~Frame()
	{
		device_->EndFrame();
	}
private:
	std::shared_ptr<Device> device_;
};