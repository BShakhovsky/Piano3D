#pragma once

class Geometry : boost::noncopyable
{
	Geometry() = delete;
public:
	explicit Geometry(ID3D11DeviceContext*);
	~Geometry();

	DirectX::SimpleMath::Matrix GetWorldMatrix(int note) const;
	void Draw(int note, const std::string& fingerNumbers = "");
private:
	void DrawBlack(const std::string& fingerNumbers);
	void DrawWhiteLeft(const std::string& fingerNumbers);
	void DrawWhiteMiddle(const std::string& fingerNumbers);
	void DrawWhiteRight(const std::string& fingerNumbers);

	void UpdateFinger(DirectX::VertexPositionNormalColorTexture* destination1,
		DirectX::VertexPositionNormalColorTexture* destination2, const std::string& fingerNumbers) const;

	const std::unique_ptr<class Mesh> mesh_;
	std::shared_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionNormalColorTexture>> batch_;
	std::vector<DirectX::VertexPositionNormalColorTexture> whiteMiddle_, whiteLeft_, whiteRight_, black_;
};