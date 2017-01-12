#include "stdafx.h"
#include "FramesPerSec.h"

FramesPerSec::FramesPerSec()
// Consider using 'GetTickCount64' : GetTickCount overflows every 49 days, and code can loop indefinitely
#pragma warning(suppress:28159)
	: startTime_(GetTickCount() - 1),
	frames_(0),
	fps_(0)
{}

void FramesPerSec::Calculate()
{
	++frames_;

// Consider using 'GetTickCount64' : GetTickCount overflows every 49 days, and code can loop indefinitely
#pragma warning(suppress:28159)
	const auto deltaTime(GetTickCount() - startTime_);
	if (deltaTime > 1'000)
	{
		fps_ = frames_ * 1'000 / deltaTime;
		startTime_ += 1'000;
		frames_ = 0;
	}
}