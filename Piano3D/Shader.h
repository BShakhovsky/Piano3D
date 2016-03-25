#pragma once

class Shader : boost::noncopyable
{
	Shader() = delete;
public:
	explicit Shader(ID3D11Device*, ID3D11DeviceContext*);

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
			effect_->EnableDefaultLighting();
		else
		{
			effect_->DisableSpecular();
			effect_->SetLightDiffuseColor(0, DirectX::Colors::Gray);
		}
	}
	void UpdateTexture(ID3D11ShaderResourceView* texture = nullptr) const
	{
		effect_->SetTexture(texture ? texture : texture_.Get());
	}

	void Apply(ID3D11DeviceContext* context) const
	{
		effect_->Apply(context);
		context->PSSetSamplers(0, 1, sampler_.GetAddressOf());
	}
private:
	const std::unique_ptr<DirectX::BasicEffect> effect_;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> layout_;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture_;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler_;
};