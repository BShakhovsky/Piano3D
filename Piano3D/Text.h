#pragma once

class Text : boost::noncopyable
{
	Text() = delete;
public:
	explicit Text(ID3D11Device*, ID3D11DeviceContext*);
	void Draw(LPCTSTR text, float x, float y, const DirectX::SimpleMath::Color& color) const;
private:
	std::shared_ptr<DirectX::SpriteBatch> sprite_;
	std::unique_ptr<DirectX::SpriteFont> font_;
};