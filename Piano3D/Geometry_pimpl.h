#pragma once

class Geometry_pimpl : boost::noncopyable
{
	Geometry_pimpl() = delete;
public:
	explicit Geometry_pimpl(ID3D11DeviceContext*);
	~Geometry_pimpl();

	void KeyIsGrey(bool isGrey)
	{
		keyIsGrey_ = isGrey;
	}

	void DrawBlack(const std::string& fingerNumbers);
	void DrawWhiteLeft(const std::string& fingerNumbers);
	void DrawWhiteMiddle(const std::string& fingerNumbers);
	void DrawWhiteRight(const std::string& fingerNumbers);

	void UpdateFinger(DirectX::VertexPositionNormalColorTexture* destination1,
		DirectX::VertexPositionNormalColorTexture* destination2, const std::string& fingerNumbers) const;
private:
	const std::unique_ptr<class Mesh> mesh_;
	std::shared_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionNormalColorTexture>> batch_;
	std::vector<DirectX::VertexPositionNormalColorTexture> whiteMiddle_, whiteLeft_, whiteRight_, black_,
		greyMiddle_, greyLeft_, greyRight_, grey_;

	bool keyIsGrey_;
	const BYTE padding_[3] = { '\0' };
};