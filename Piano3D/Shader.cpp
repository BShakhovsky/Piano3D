#include "stdafx.h"
#include "Shader.h"
#include "ResourceLoader.h"
#include "Resource.h"
#include "DxError.h"

using std::make_unique;
using namespace DirectX;

Shader::Shader(ID3D11Device* device)
	: effect_(make_unique<BasicEffect>(device))
{
	using namespace SimpleMath;

	effect_->SetVertexColorEnabled(true);
	effect_->EnableDefaultLighting();
	effect_->SetTextureEnabled(true);

	ResourceLoader resourceDesk(IDB_DESK, TEXT("PNG"));
	DxError::ThrowIfFailed(CreateWICTextureFromMemory(device,
		static_cast<const BYTE*>(resourceDesk.Data()), resourceDesk.Size(),
		nullptr, &textureDesk_), TEXT("Could not create DirectX texture-resource"));

	ResourceLoader resourceLeft(IDB_LEFT, TEXT("PNG"));
	DxError::ThrowIfFailed(CreateWICTextureFromMemory(device,
		static_cast<const BYTE*>(resourceLeft.Data()), resourceLeft.Size(),
		nullptr, &textureFingersLeft_), TEXT("Could not create DirectX texture-resource"));
	ResourceLoader resourceRight(IDB_RIGHT, TEXT("PNG"));
	DxError::ThrowIfFailed(CreateWICTextureFromMemory(device,
		static_cast<const BYTE*>(resourceRight.Data()), resourceRight.Size(),
		nullptr, &textureFingersRight_), TEXT("Could not create DirectX texture-resource"));

	sampler_ = CommonStates(device).LinearWrap();
}

void Shader::Apply(ID3D11Device* device, ID3D11DeviceContext* context)
{
	effect_->Apply(context);

	const void* shaderCode;
	size_t shaderLength;
	effect_->GetVertexShaderBytecode(&shaderCode, &shaderLength);
	DxError::ThrowIfFailed(device->CreateInputLayout(VertexPositionNormalColorTexture::InputElements,
			VertexPositionNormalColorTexture::InputElementCount, shaderCode, shaderLength, &layout_),
		TEXT("Could not create DirectX shaders"));
	context->IASetInputLayout(layout_.Get());

	context->PSSetSamplers(0, 1, sampler_.GetAddressOf());
}