#pragma once

class Shader : boost::noncopyable
{
	Shader() = delete;
public:
	explicit Shader(ID3D11Device*);

	void UpdateWorldView(const DirectX::SimpleMath::Matrix& world, const DirectX::SimpleMath::Matrix& view) const
	{
		effect_->SetWorld(world);
		effect_->SetView(view);
	}
	void UpdateProjection(const DirectX::SimpleMath::Matrix& projection) const
	{
		effect_->SetProjection(projection);
	}
	void UpdateLights(bool enableSpecular) const
	{
		if (enableSpecular)
		{
			effect_->SetSpecularColor(DirectX::Colors::White);
			effect_->SetSpecularPower(16);
		}
		else effect_->DisableSpecular();
	}
	void UpdateTexture(ID3D11ShaderResourceView* texture = nullptr, bool leftHand = false) const
	{
		effect_->SetTexture(texture ? texture :
			leftHand ? textureFingersLeft_.Get() : textureFingersRight_.Get());
	}
	ID3D11ShaderResourceView* GetDeskTexture()
	{
		return textureDesk_.Get();
	}

	void Apply(ID3D11Device*, ID3D11DeviceContext*);
private:
	const std::unique_ptr<DirectX::BasicEffect> effect_;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> layout_;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> textureDesk_, textureFingersLeft_, textureFingersRight_;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler_;
};