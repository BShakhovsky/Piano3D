#pragma once

class Render_pimpl : boost::noncopyable
{
	Render_pimpl() = delete;
public:
	explicit Render_pimpl(HWND, UINT width, UINT height);
	~Render_pimpl();

	void Resize(UINT width, UINT height);
	void Draw() const;
private:
	void DrawScene(const DirectX::SimpleMath::Matrix& view) const;
	void DrawDebugInfo() const;

	UINT width_, height_;

	std::shared_ptr<class Device> device_;
	const std::unique_ptr<class Camera> camera_;
	const std::unique_ptr<class Shader> shader_;
	const std::unique_ptr<class Geometry> geometry_;
	const std::unique_ptr<class Text> text_;

	std::unique_ptr<class BackBuffer2D> pianoDesk_, keyboardReflection_;
};