#include "stdafx.h"
#include "Device.h"
#include "DxError.h"

using Microsoft::WRL::ComPtr;

Device::Device(const HWND hWnd, const UINT width, const UINT height)
{
	assert("Direct3D Debug Device is not available" && SUCCEEDED(D3D11CreateDevice(nullptr,
		D3D_DRIVER_TYPE_NULL,		// No need to create a real hardware device
		0,
		D3D11_CREATE_DEVICE_DEBUG,	// Check for the SDK layers
		nullptr,					// Any feature level will do
		0, D3D11_SDK_VERSION,
		nullptr, nullptr, nullptr	// No need to keep the device, know the feature level, keep the device context
		)));
	DXGI_SWAP_CHAIN_DESC chainDesc{ { width, height,{ 1 }, BACK_BUFFER_FORMAT },{ 4 },
		DXGI_USAGE_RENDER_TARGET_OUTPUT, 1, hWnd, TRUE, DXGI_SWAP_EFFECT_DISCARD, BACK_BUFFER_FLAGS };

	D3D_DRIVER_TYPE driverTypes[]
		= { D3D_DRIVER_TYPE_HARDWARE, D3D_DRIVER_TYPE_WARP, D3D_DRIVER_TYPE_REFERENCE };
	for (const auto& driver : driverTypes)
		if (SUCCEEDED(D3D11CreateDeviceAndSwapChain(nullptr, driver, nullptr, chainDesc.Flags,
			nullptr, 0, D3D11_SDK_VERSION, &chainDesc, &chain_, &device_, nullptr, &context_)))
				break;
	if (!device_) throw DxError("Could not initialize DirectX");

#ifdef _DEBUG
	ComPtr<ID3D11Debug> debug;
	if (SUCCEEDED(device_.As(&debug)))
	{
		ComPtr<ID3D11InfoQueue> infoQueue;
		if (SUCCEEDED(debug.As(&infoQueue)))
		{
			infoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
			infoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);
			D3D11_MESSAGE_ID hide[] = { D3D11_MESSAGE_ID_SETPRIVATEDATA_CHANGINGPARAMS };
			D3D11_INFO_QUEUE_FILTER filter{ { 0 }, { 0, nullptr, 0, nullptr, ARRAYSIZE(hide), hide } };
			infoQueue->AddStorageFilterEntries(&filter);
		}
	}
#endif

	Resize(width, height);
}
Device::~Device()
{
	if (context_) context_->ClearState();
}

#pragma warning(push)
#pragma warning(disable:4711)
HRESULT Device::CreateBackBuffer()
{
	ComPtr<ID3D11Texture2D> resourceBuffer;
	const auto hResult(chain_->GetBuffer(0, IID_PPV_ARGS(&resourceBuffer)));
	if (FAILED(hResult)) return hResult;
	return device_->CreateRenderTargetView(resourceBuffer.Get(), nullptr, &backBuffer_);
}
#pragma warning(pop)

HRESULT Device::CreateDepthBuffer(const UINT width, const UINT height)
{
	CD3D11_TEXTURE2D_DESC textDesc(DXGI_FORMAT_D24_UNORM_S8_UINT, width, height, 1, 1,
		D3D11_BIND_DEPTH_STENCIL, D3D11_USAGE_DEFAULT, 0, 4, 0);
	const auto hResult(device_->CreateTexture2D(&textDesc, nullptr, &depthBuffer_));
	if (FAILED(hResult)) return hResult;

	CD3D11_DEPTH_STENCIL_VIEW_DESC viewDesc(depthBuffer_.Get(), D3D11_DSV_DIMENSION_TEXTURE2DMS);
	return device_->CreateDepthStencilView(depthBuffer_.Get(), &viewDesc, &depthView_);
}

HRESULT Device::SetAntiAliasingMode()
{
	CD3D11_RASTERIZER_DESC rasterDesc(D3D11_FILL_SOLID, D3D11_CULL_NONE, FALSE, D3D11_DEFAULT_DEPTH_BIAS,
		D3D11_DEFAULT_DEPTH_BIAS_CLAMP, D3D11_DEFAULT_SLOPE_SCALED_DEPTH_BIAS, TRUE, FALSE, TRUE, TRUE);
	const auto hResult(device_->CreateRasterizerState(&rasterDesc, raster_.ReleaseAndGetAddressOf()));
	if (FAILED(hResult)) return hResult;
	
	context_->RSSetState(raster_.Get());
	return hResult;
}

void Device::Resize(UINT width, UINT height)
{
	using std::max;
	using DirectX::XMConvertToRadians;
	using DirectX::SimpleMath::Matrix;

	width = max(width, static_cast<UINT>(1));
	height = max(height, static_cast<UINT>(1));

	orthographic_ = Matrix::CreateOrthographic(
		static_cast<float>(width), static_cast<float>(height), SCREEN_NEAR, SCREEN_DEPTH);
	projection_ = Matrix::CreatePerspectiveFieldOfView(XMConvertToRadians(45),
		static_cast<float>(width) / height, SCREEN_NEAR, SCREEN_DEPTH);

	backBuffer_.Reset();
	depthBuffer_.Reset();
	depthView_.Reset();
	context_->Flush();

	THROW_IF_FAILED(DxError, chain_->ResizeBuffers(1, width, height, BACK_BUFFER_FORMAT, BACK_BUFFER_FLAGS),
															"Could not resize DirectX swap chain");
	THROW_IF_FAILED(DxError, CreateBackBuffer(),			"Could not create DirectX back-buffer");
	THROW_IF_FAILED(DxError, CreateDepthBuffer(width, height),"Could not create DirectX depth buffer");
	THROW_IF_FAILED(DxError, SetAntiAliasingMode(),			"Could not create DirectX anti-aliasing mode");

	SetTargetBuffer();
	SetViewPort(width, height);
}