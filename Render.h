#pragma once
#pragma comment(lib, "Piano3D")
class Render
{
	Render() = delete;
public:
	explicit Render(HWND, UINT width, UINT height,
		float cameraX, float cameraY, float cameraZ, LPCTSTR path);
	~Render();

	void Resize(UINT width, UINT height) const;
	void Draw() const;

	void PressKey(int16_t note) const;
	void AssignFingerNums(int16_t note, const char* fingers, bool leftHand = false) const;
	void ReleaseAllKeys() const;

	bool Zoom(bool decrease = false) const;
	bool FitToWindow() const;
	void MoveStart(int screenX, int screenY) const;
	void MoveEnd(int screenX, int screenY) const;
	void RotateStart(int screenX, int screenY) const;
	void RotateEnd(int screenX, int screenY) const;
private:
	class Render_pimpl* const pimpl_;

	Render(const Render&) = delete;
	Render(const Render&&) = delete;
	Render& operator=(const Render&) = delete;
	Render&& operator=(const Render&&) = delete;
};