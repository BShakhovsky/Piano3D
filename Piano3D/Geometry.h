#pragma once

class Geometry : boost::noncopyable
{
	Geometry() = delete;
public:
	static constexpr float deskLength = 58.0f, deskHeight = 9.0f, deskThickness = 1.5f,
		keyboardLength = 52.0f, cottonThickness = 0.25f;
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
			DirectX::SimpleMath::Color(1.f, 1.f, 1.f, 0.85f), texture, false, [&device, &context]()
		{
			context->OMSetBlendState(DirectX::CommonStates(device).Opaque(), nullptr, 0xFF'FF'FF'FF);
		});
	}
	void DrawCotton(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection) const
	{
		cotton_->Draw(DirectX::SimpleMath::Matrix::CreateTranslation(
			52 / 2, 1 + cottonThickness / 2, cottonThickness / 2),
			view, projection, DirectX::SimpleMath::Color(1.f, .0f, .0f));
	}
private:
	const std::unique_ptr<class Geometry_pimpl> pimpl_;
	const std::unique_ptr<DirectX::GeometricPrimitive> pianoDesk_, cotton_;
};