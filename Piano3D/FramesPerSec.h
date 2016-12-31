#pragma once

class FramesPerSec : boost::noncopyable
{
public:
	FramesPerSec();
	~FramesPerSec() = default;

	void Calculate();
	DWORD Get() const
	{
		return fps_;
	}
private:
	DWORD startTime_, frames_, fps_;
};