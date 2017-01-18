#pragma once
class RenderData : boost::noncopyable
{
	RenderData() = delete;
public:
	explicit RenderData(HWND hWnd, UINT width, UINT height, LPCTSTR path);
	~RenderData();

	const std::shared_ptr<class Device>& GetDevice() const;
	void Resize(UINT width, UINT height);
	void Begin2D() const;
	void SwitchTo3D() const;

	bool Restore3DPosition() const;
	bool Zoom(bool increase) const;
	bool FitToWindow() const;

	void MoveStart(float screenX, float screenY) const;
	void MoveEnd(float screenX, float screenY) const;
	void RotatePianoStart(float screenX, float screenY) const;
	void RotatePianoEnd(float screenX, float screenY) const;

	void PressKey(int16_t note) const;
	void AssignFingerNums(int16_t note, const char* fingers, const bool leftHand) const;
	void ReleaseAllKeys() const;
	void ReturnPressedKeys() const;

	void BeginKeyboardReflectionBuffer() const;
	void DrawPianoDeskBuffer() const;
	void DrawPianoDesk() const;
	void DrawPianoKey(int16_t note, bool mirrowed) const;
	void DrawDebugInfo() const;
private:
	void BeginPianoDeskBuffer() const;
	void DrawKeyboardReflection() const;

	void ApplyShader() const;

	UINT width_, height_;

	std::shared_ptr<class Device> device_;
	const std::unique_ptr<class Camera> camera_;
	const std::unique_ptr<class Shader> shader_;
	const std::unique_ptr<class Geometry> geometry_;
	const std::unique_ptr<class Text> text_;
	std::unique_ptr<class BackBuffer2D> pianoDesk_, keyboardReflection_;

	const std::unique_ptr<class KeyStates> keyStates_;
};