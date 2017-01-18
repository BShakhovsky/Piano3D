#pragma once
class Device : boost::noncopyable
{
	Device() = delete;
public:
	explicit Device(HWND, UINT width, UINT height);
	~Device();

	void Resize(UINT width, UINT height);
	void SetTargetBuffer()
	{
		context_->OMSetRenderTargets(1, backBuffer_.GetAddressOf(), depthView_.Get());
	}

	void BeginFrame(const DirectX::SimpleMath::Color& color)
	{
		context_->ClearRenderTargetView(backBuffer_.Get(), color);
		context_->ClearDepthStencilView(depthView_.Get(), D3D11_CLEAR_DEPTH, 1, 0);
	}
	void EndFrame()
	{
#ifdef _DEBUG
		chain_->Present(0, 0);
#else
		chain_->Present(1, 0);
#endif
	}

	void Mode2D() const
	{
		context_->OMSetDepthStencilState(DirectX::CommonStates(device_.Get()).DepthNone(), 1);
	}
	void Mode3D() const
	{
		context_->OMSetDepthStencilState(DirectX::CommonStates(device_.Get()).DepthDefault(), 1);
	}

	void AlfaBlendingOn() const
	{
		context_->OMSetBlendState(DirectX::CommonStates(device_.Get()).AlphaBlend(), nullptr, 0xFF'FF'FF'FF);
	}
	void AlfaBlendingOff() const
	{
		context_->OMSetBlendState(DirectX::CommonStates(device_.Get()).Opaque(), nullptr, 0xFF'FF'FF'FF);
	}

	ID3D11Device* GetDevice()
	{
		return device_.Get();
	}
	ID3D11DeviceContext* GetContext()
	{
		return context_.Get();
	}

	ID3D11Texture2D* GetDepthBuffer()
	{
		return depthBuffer_.Get();
	}
	ID3D11DepthStencilView* GetDepthView()
	{
		return depthView_.Get();
	}
	
	const DirectX::SimpleMath::Matrix& GetProjection() const
	{
		return projection_;
	}
	const DirectX::SimpleMath::Matrix& GetOrthographic() const
	{
		return orthographic_;
	}
private:
	HRESULT CreateBackBuffer();
	HRESULT CreateDepthBuffer(UINT width, UINT height);
	HRESULT SetAntiAliasingMode();
	void SetViewPort(UINT width, UINT height)
	{
		CD3D11_VIEWPORT viewport(0.0f, 0.0f, static_cast<float>(width), static_cast<float>(height));
		context_->RSSetViewports(1, &viewport);
	}

	static const DXGI_FORMAT BACK_BUFFER_FORMAT = DXGI_FORMAT_R8G8B8A8_UNORM;
	static const UINT BACK_BUFFER_FLAGS =
#ifdef _DEBUG
		D3D11_CREATE_DEVICE_DEBUG;
#else
		0;
#endif
	static constexpr float SCREEN_NEAR = 0.001f, SCREEN_DEPTH = 1000.0f;
	DirectX::SimpleMath::Matrix orthographic_, projection_;

	Microsoft::WRL::ComPtr<ID3D11Device> device_;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context_;
	Microsoft::WRL::ComPtr<IDXGISwapChain> chain_;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> backBuffer_;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> depthBuffer_;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthView_;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> raster_;
};