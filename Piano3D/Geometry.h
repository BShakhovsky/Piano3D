#pragma once

class Geometry : boost::noncopyable
{
	Geometry() = delete;
public:
	static constexpr float deskLength = 58.0f, deskHeight = 10.0f, deskThickness = 2.0f,
		keyboardLength = 52.0f;
	static float GetKeyboardWidth();

	explicit Geometry(ID3D11DeviceContext*);
	~Geometry();

	DirectX::SimpleMath::Matrix GetWorldMatrix(int note) const;
	DirectX::SimpleMath::Matrix GetRotatedMatrix(int note, float pressedKeyAngle) const;

	void DrawKey(int note, bool highlighted, const char* fingers);
	void DrawDesk(ID3D11Device* device, ID3D11DeviceContext* context,
		const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection,
		ID3D11ShaderResourceView* texture) const
	{
		pianoDesk_->Draw(DirectX::SimpleMath::Matrix::CreateTranslation(
			keyboardLength / 2, deskHeight / 2, -deskThickness / 2), view, projection,
			DirectX::SimpleMath::Color(0.15f, 0.15f, 0.15f, 0.9f), texture , false, [&device, &context]()
		{
			context->OMSetBlendState(DirectX::CommonStates(device).Opaque(), nullptr, 0xFF'FF'FF'FF);
		});
	}
private:
	const std::unique_ptr<class Geometry_pimpl> pimpl_;
	const std::unique_ptr<DirectX::GeometricPrimitive> pianoDesk_;
};