#pragma once
#pragma comment(lib, "Piano3D")
class Render
{
	Render() = delete;
public:
	explicit Render(HWND, UINT width, UINT height, LPCTSTR path);
	~Render();

	void Resize(UINT width, UINT height) const;
	void Draw() const;

	void PressKey(int16_t note) const;
	void AssignFingerNums(int16_t note, const char* fingers, bool leftHand = false) const;
	void ReleaseAllKeys() const;

	bool Restore3DPosition() const;
	bool Zoom(bool increase = true) const;
	bool FitToWindow() const;

	void MoveStart(float screenX, float screenY) const;
	void MoveEnd(float screenX, float screenY) const;
	void RotateStart(float screenX, float screenY) const;
	void RotateEnd(float screenX, float screenY) const;
private:
	class Render_pimpl* const pimpl_;

	Render(const Render&) = delete;
	Render(const Render&&) = delete;
	Render& operator=(const Render&) = delete;
	Render&& operator=(const Render&&) = delete;
};