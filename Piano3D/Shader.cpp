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
	
	// https://github.com/microsoft/DirectXTK/wiki/IEffectLights#controlling-lights

	//effect_->SetLightingEnabled(true);
	//effect_->SetAmbientLightColor({ .05333332f, .09882354f, .1819608f });

	//effect_->SetLightEnabled(		0, true);
	//effect_->SetLightDirection(		0, { -.5265408f, -.5735765f, -.6275069f });
	//effect_->SetLightDiffuseColor(	0, { 1.f, .9607844f, .8078432f });
	//effect_->SetLightSpecularColor(	0, { 1.f, .9607844f, .8078432f });

	//effect_->SetLightEnabled(		1, true);
	//effect_->SetLightDirection(		1, { .7198464f, .3420201f, .6040227f });
	//effect_->SetLightDiffuseColor(	1, { .9647059f, .7607844f, .4078432f });
	//effect_->SetLightSpecularColor(	1, { .0f, .0f, .0f });

	// Z reversed, so we see it:
	effect_->SetLightDirection(		1, { .7198464f, .3420201f, -.6040227f });
	// R=G=B, otherwise steel blue background changes its shade
	// If too shiny, gaps between some white keys become less contrast:
	effect_->SetLightDiffuseColor(	1, { .7f, .7f, .7f });
	// Let it have specular too:
	effect_->SetLightSpecularColor(	1, { .9647059f, .7607844f, .4078432f });
	
	//effect_->SetLightEnabled(		2, true);
	//effect_->SetLightDirection(		2, { .4545195f, -.7660444f, .4545195f });
	//effect_->SetLightDiffuseColor(	2, { .3231373f,  .3607844f, .3937255f });
	//effect_->SetLightSpecularColor(	2, { .3231373f,  .3607844f, .3937255f });

	effect_->SetTextureEnabled(true);

	ResourceLoader resourceDesk(IDB_DESK, TEXT("PNG"));
	THROW_IF_FAILED(DxError, CreateWICTextureFromMemory(device,
		static_cast<const BYTE*>(resourceDesk.Data()), resourceDesk.Size(),
		nullptr, &textureDesk_), "Could not create DirectX texture-resource");

	ResourceLoader resourceLeft(IDB_LEFT, TEXT("PNG"));
	THROW_IF_FAILED(DxError, CreateWICTextureFromMemory(device,
		static_cast<const BYTE*>(resourceLeft.Data()), resourceLeft.Size(),
		nullptr, &textureFingersLeft_), "Could not create DirectX texture-resource");
	ResourceLoader resourceRight(IDB_RIGHT, TEXT("PNG"));
	THROW_IF_FAILED(DxError, CreateWICTextureFromMemory(device,
		static_cast<const BYTE*>(resourceRight.Data()), resourceRight.Size(),
		nullptr, &textureFingersRight_), "Could not create DirectX texture-resource");

	sampler_ = CommonStates(device).LinearWrap();
}

void Shader::Apply(ID3D11Device* device, ID3D11DeviceContext* context)
{
	effect_->Apply(context);

	const void* shaderCode;
	size_t shaderLength;
	effect_->GetVertexShaderBytecode(&shaderCode, &shaderLength);
	THROW_IF_FAILED(DxError, device->CreateInputLayout(VertexPositionNormalColorTexture::InputElements,
		VertexPositionNormalColorTexture::InputElementCount, shaderCode, shaderLength, &layout_),
		"Could not create DirectX shaders");
	context->IASetInputLayout(layout_.Get());

	context->PSSetSamplers(0, 1, sampler_.GetAddressOf());
}