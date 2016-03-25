#include "stdafx.h"
#include "Shader.h"
#include "ResourceLoader.h"
#include "Resource.h"
#include "DxError.h"

using std::make_unique;
using namespace DirectX;

Shader::Shader(ID3D11Device* device, ID3D11DeviceContext* context)
	: effect_(make_unique<BasicEffect>(device))
{
	using namespace SimpleMath;

	effect_->SetVertexColorEnabled(true);
	effect_->EnableDefaultLighting();
	effect_->SetTextureEnabled(true);

	const void* shaderCode;
	size_t shaderLength;
	effect_->GetVertexShaderBytecode(&shaderCode, &shaderLength);
	if (FAILED(device->CreateInputLayout(VertexPositionNormalColorTexture::InputElements,
		VertexPositionNormalColorTexture::InputElementCount, shaderCode, shaderLength, &layout_)))
			throw DxError("Could not create DirectX shaders");
	context->IASetInputLayout(layout_.Get());

	ResourceLoader resource(IDB_FONT, TEXT("PNG"));
	if (FAILED(CreateWICTextureFromMemory(device, static_cast<const BYTE*>(resource.Data()), resource.Size(),
		nullptr, &texture_))) throw DxError("Could not create DirectX texture-resource");

	sampler_ = CommonStates(device).LinearWrap();
}