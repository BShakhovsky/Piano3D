#pragma once
#include "Batch.h"

class BackBuffer2D : boost::noncopyable
{
	BackBuffer2D() = delete;
public:
	explicit BackBuffer2D(ID3D11Device*, ID3D11DeviceContext*, ID3D11Texture2D* depthBuffer, UINT width, UINT height);

	void SetTargetBuffer(ID3D11DeviceContext* context, ID3D11DepthStencilView* depthView)
	{
		context->OMSetRenderTargets(1, backBuffer_.GetAddressOf(), depthView);
	}
	void BeginFrame(ID3D11DeviceContext* context, ID3D11DepthStencilView* depthView, const DirectX::SimpleMath::Color& color)
	{
		context->ClearRenderTargetView(backBuffer_.Get(), color);
		context->ClearDepthStencilView(depthView, D3D11_CLEAR_DEPTH, 1, 0);
	}
	void Draw() const
	{
		Batch scene(batch_);
		batch_->DrawQuad(vertices_.front(), vertices_.at(1), vertices_.at(2), vertices_.back());
	}

	ID3D11ShaderResourceView* GetTexture(ID3D11DeviceContext* context) const
	{
		context->ResolveSubresource(textureSS_.Get(), D3D11CalcSubresource(0, 0, 1),
			textureMS_.Get(), D3D11CalcSubresource(0, 0, 1), BACK_BUFFER_FORMAT);
		return shaderResourceSS_.Get();
	}
private:
	static const DXGI_FORMAT BACK_BUFFER_FORMAT = DXGI_FORMAT_R32G32B32A32_FLOAT;

	const std::vector<DirectX::VertexPositionNormalColorTexture> vertices_;
	std::shared_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionNormalColorTexture>> batch_;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> textureMS_, textureSS_;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> backBuffer_;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shaderResourceMS_, shaderResourceSS_;
};