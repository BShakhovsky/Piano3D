#pragma once

class Geometry : boost::noncopyable
{
	Geometry() = delete;
public:
	explicit Geometry(ID3D11DeviceContext*);
	~Geometry();

	DirectX::SimpleMath::Matrix GetWorldMatrix(int note) const;
	DirectX::SimpleMath::Matrix GetRotatedMatrix(int note, float pressedKeyAngle) const;

	void DrawKey(int note, bool highlighted, const char* fingers);
	void DrawDesk(ID3D11Device* device, ID3D11DeviceContext* context,
		const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection,
		ID3D11ShaderResourceView* texture) const
	{
		pianoDesk_->Draw(DirectX::SimpleMath::Matrix::CreateTranslation(26, 5, -1), view, projection,
			DirectX::SimpleMath::Color(0.15f, 0.15f, 0.15f, 0.9f), texture , false, [&device, &context]()
		{
			context->OMSetBlendState(DirectX::CommonStates(device).Opaque(), nullptr, 0xFF'FF'FF'FF);
		});
	}
private:
	void DrawBlack(const std::string& fingerNumbers);
	void DrawWhiteLeft(const std::string& fingerNumbers);
	void DrawWhiteMiddle(const std::string& fingerNumbers);
	void DrawWhiteRight(const std::string& fingerNumbers);

	void UpdateFinger(DirectX::VertexPositionNormalColorTexture* destination1,
		DirectX::VertexPositionNormalColorTexture* destination2, const std::string& fingerNumbers) const;

	const std::unique_ptr<class Mesh> mesh_;
	const std::unique_ptr<DirectX::GeometricPrimitive> pianoDesk_;
	std::shared_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionNormalColorTexture>> batch_;
	std::vector<DirectX::VertexPositionNormalColorTexture> whiteMiddle_, whiteLeft_, whiteRight_, black_,
		greyMiddle_, greyLeft_, greyRight_, grey_;

	bool keyIsGrey_;
	const BYTE padding_[3] = { '\0' };
};