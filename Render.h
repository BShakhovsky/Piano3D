#pragma once

class Render : boost::noncopyable
{
	Render() = delete;
public:
	explicit Render(HWND, UINT width, UINT height);
	~Render();

	void Resize(UINT width, UINT height);
	void Draw() const;
private:
	const std::unique_ptr<class Render_pimpl> pimpl_;
};