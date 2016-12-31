#pragma once
#pragma comment(lib, "Piano3D.lib")

class Render
{
	Render() = delete;
public:
	explicit Render(HWND, UINT width, UINT height);
	~Render();

	void Resize(UINT width, UINT height) const;
	void Draw() const;

	void PressKey(int16_t note) const;
	void AssignFingerNums(int16_t note, const char* fingers, bool leftHand = false) const;
	void ReleaseAllKeys() const;
private:
	class Render_pimpl* const pimpl_;

	Render(const Render&) = delete;
	Render(const Render&&) = delete;
	Render& operator=(const Render&) = delete;
	Render&& operator=(const Render&&) = delete;
};