#include "stdafx.h"
#include "BackBuffer2D.h"
#include "DxError.h"

using std::make_shared;
using namespace DirectX;
using namespace Colors;
using namespace SimpleMath;

BackBuffer2D::BackBuffer2D(ID3D11Device* device, ID3D11DeviceContext* context, ID3D11Texture2D* depthBuffer,
	const UINT width, const UINT height)
	: vertices_
{
	{ Vector3(-static_cast<float>(width) / 2,  static_cast<float>(height) / 2, 0), Vector3::Backward, Color(White.v), Vector2(1, 0) },
	{ Vector3( static_cast<float>(width) / 2,  static_cast<float>(height) / 2, 0), Vector3::Backward, Color(White.v), Vector2(0, 0) },
	{ Vector3( static_cast<float>(width) / 2, -static_cast<float>(height) / 2, 0), Vector3::Backward, Color(White.v), Vector2(0, 1) },
	{ Vector3(-static_cast<float>(width) / 2, -static_cast<float>(height) / 2, 0), Vector3::Backward, Color(White.v), Vector2(1, 1) }
},
	batch_(make_shared<PrimitiveBatch<VertexPositionNormalColorTexture>>(context))
{
	D3D11_TEXTURE2D_DESC textureDesc;
	depthBuffer->GetDesc(&textureDesc);
	textureDesc.Format = BACK_BUFFER_FORMAT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	if (FAILED(device->CreateTexture2D(&textureDesc, nullptr, &textureMS_)))
		throw DxError("Could not create DirectX multi-sampled back-buffer texture");
	textureDesc = CD3D11_TEXTURE2D_DESC(BACK_BUFFER_FORMAT, max(width, 1), max(height, 1));
	textureDesc.BindFlags |= D3D11_BIND_RENDER_TARGET;
	if (FAILED(device->CreateTexture2D(&textureDesc, nullptr, &textureSS_)))
		throw DxError("Could not create DirectX single-sampled back-buffer texture");

	D3D11_RENDER_TARGET_VIEW_DESC backBufferDesc{ BACK_BUFFER_FORMAT, D3D11_RTV_DIMENSION_TEXTURE2DMS };
	if (FAILED(device->CreateRenderTargetView(textureMS_.Get(), &backBufferDesc, &backBuffer_)))
		throw DxError("Could not create DirectX multi-sampled back-buffer");

	CD3D11_SHADER_RESOURCE_VIEW_DESC resourceDesc(D3D11_SRV_DIMENSION_TEXTURE2DMS, BACK_BUFFER_FORMAT);
	if (FAILED(device->CreateShaderResourceView(textureMS_.Get(), &resourceDesc, &shaderResourceMS_)))
		throw DxError("Could not create DirectX multi-sampled shader resource view");
	resourceDesc = CD3D11_SHADER_RESOURCE_VIEW_DESC(D3D11_SRV_DIMENSION_TEXTURE2D, BACK_BUFFER_FORMAT);
	if (FAILED(device->CreateShaderResourceView(textureSS_.Get(), &resourceDesc, &shaderResourceSS_)))
		throw DxError("Could not create DirectX single-sampled shader resource view");
}