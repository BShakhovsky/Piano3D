#pragma once

class Render_pimpl : boost::noncopyable
{
	Render_pimpl() = delete;
public:
	explicit Render_pimpl(HWND, UINT width, UINT height,
		float cameraX, float cameraY, float cameraZ, LPCTSTR path);
	~Render_pimpl();

	void Resize(UINT width, UINT height);
	void Draw() const;

	void PressKey(int16_t) const;
	void AssignFingerNums(int16_t, const char*, bool leftHand = false) const;
	void ReleaseAllKeys() const;

	bool Zoom(bool increase) const;
	bool FitToWindow() const;
	void MoveStart(int screenX, int screenY) const;
	void MoveEnd(int screenX, int screenY) const;
	void RotateStart(int screenX, int screenY) const;
	void RotateEnd(int screenX, int screenY) const;
private:
	void DrawScene(bool mirrowed = false) const;

	const std::unique_ptr<class RenderData> data_;
};