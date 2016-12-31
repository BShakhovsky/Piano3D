#pragma once

class KeyStates : boost::noncopyable
{
	const static float DELTA_BLACK, DELTA_WHITE, MAX_BLACK, MAX_WHITE;
public:
	KeyStates();
	~KeyStates() = default;

	void PressKey(int16_t note) const;
	void AssignFinger(int16_t note, const char* fingers, bool leftHand) const;
	void ReleaseAllKeys() const;
	void ReturnPressedKeys() const;

	float GetAngle(int16_t note) const;
	const char* GetFingers(int16_t note) const;
	bool IsLeftHand(int16_t note) const;
	bool IsHighlighted(int16_t note) const;
private:
	std::vector<std::shared_ptr<struct KeyData>> keys_;
};