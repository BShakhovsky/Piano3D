#include "stdafx.h"
#include "KeyStates.h"
#include "BlackWhiteKeys.h"

using DirectX::XMConvertToRadians;

const float
KeyStates::DELTA_BLACK = 0.0004f,
KeyStates::DELTA_WHITE = 0.0003f,
KeyStates::MAX_BLACK = XMConvertToRadians(7),
KeyStates::MAX_WHITE = XMConvertToRadians(5);

#pragma warning(push)
#pragma warning(disable:26495) // angle uninitialized
struct KeyData
{
	float angle, delta;

	std::string fingers;
	bool leftHand,
		highlighted;
private:
	const BYTE padding[2] = { '\0' };
};
#pragma warning(pop)

KeyStates::KeyStates()
	: keys_(88)
{
	using std::make_shared;

	for (auto& key : keys_) key = make_shared<KeyData>();
}

#pragma warning(push)
#pragma warning(disable: 5045) // Compiler will insert Spectre mitigation for memory load
float KeyStates::GetAngle(const int16_t note) const
{
	return keys_.at(static_cast<size_t>(note))->angle;
}
const char* KeyStates::GetFingers(const int16_t note) const
{
	return keys_.at(static_cast<size_t>(note))->fingers.c_str();
}
bool KeyStates::IsLeftHand(const int16_t note) const
{
	return keys_.at(static_cast<size_t>(note))->leftHand;
}
bool KeyStates::IsHighlighted(const int16_t note) const
{
	return keys_.at(static_cast<size_t>(note))->highlighted;
}
#pragma warning(pop)

bool IsBlackKey(int16_t note)
{
	return BlackWhiteKeys::IsBlack(note + 9);
}

void KeyStates::PressKey(const int16_t note) const
{
	assert("Relative key position should be in the range [0...87]" && note >= 0 && note < 88);
	const auto key(keys_.at(static_cast<size_t>(note)));

	key->highlighted = true;
	key->angle = IsBlackKey(note) ? MAX_BLACK : MAX_WHITE;
	key->delta = 0;
}
void KeyStates::AssignFinger(const int16_t note, const char* fingers, const bool leftHand) const
{
	assert("Relative key position should be in the range [0...87]" && note >= 0 && note < 88);
	const auto key(keys_.at(static_cast<size_t>(note)));

	key->fingers = fingers;
	key->leftHand = leftHand;
}
#pragma warning(push)
#pragma warning(disable:5045) // Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified
void KeyStates::ReleaseAllKeys() const
{
	for (size_t i(0); i < keys_.size(); ++i)
	{
		keys_.at(i)->highlighted = false;
		keys_.at(i)->fingers = "";
		keys_.at(i)->delta = IsBlackKey(static_cast<int16_t>(i)) ? DELTA_BLACK : DELTA_WHITE;
	}
}
#pragma warning(pop)

//#pragma warning(push)
#pragma warning(disable: 4711) // Automatic inline expansion
void KeyStates::ReturnPressedKeys() const
{
	static DWORD startTime(GetTickCount());

	for (const auto& key : keys_)
	{
// Consider using 'GetTickCount64' : GetTickCount overflows every 49 days, and code can loop indefinitely
#pragma warning(suppress:28159)
		const auto delta(key->delta * static_cast<float>(GetTickCount() - startTime));
#pragma warning(suppress:28159)
		if (key->angle > key->delta * static_cast<float>(GetTickCount() - startTime)) key->angle -= delta;
		else key->angle = key->delta = 0;
	}
#pragma warning(suppress:28159)
	startTime = GetTickCount();
}
//#pragma warning(pop)