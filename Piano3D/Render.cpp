#include "stdafx.h"
#include "Render.h"
#include "Frame.h"

using namespace DirectX::SimpleMath;

void Render::Resize(const UINT width, const UINT height)
{
	device_->Resize(width, height);
	imageBuffer_ = std::make_unique<BackBuffer2D>(device_->GetDevice(), device_->GetContext(), device_->GetDepthBuffer(),
		width, height);
}

void Render::Draw() const
{
	using namespace DirectX::Colors;

	imageBuffer_->SetTargetBuffer(device_->GetContext(), device_->GetDepthView());
	imageBuffer_->BeginFrame(device_->GetContext(), device_->GetDepthView(), SteelBlue.v);
	shader_->UpdateLights(false);
	DrawScene(camera_->GetMirrowedViewMatrix());

	device_->Mode2D();
	device_->SetTargetBuffer();

	shader_->UpdateTexture(imageBuffer_->GetTexture(device_->GetContext()));
	shader_->UpdateProjection(device_->GetOrthographic());
	shader_->UpdateWorldView(Matrix::Identity, Matrix::CreateLookAt(Vector3::Backward, Vector3::Zero, Vector3::Up));
	shader_->Apply(device_->GetContext());

	Frame frame(device_, SteelBlue.v);
	imageBuffer_->Draw();
	shader_->UpdateLights(true);
	DrawScene(camera_->GetViewMatrix());
}

void Render::DrawScene(const Matrix& view) const
{
	device_->Mode3D();
	shader_->UpdateTexture();
	shader_->UpdateProjection(device_->GetProjection());
	for (auto i(0); i < 88; ++i)
	{
		shader_->UpdateWorldView(geometry_->GetWorldMatrix(i), view);
		shader_->Apply(device_->GetContext());
		geometry_->Draw(i, std::string(static_cast<size_t>(i % 3), static_cast<char>('0' + i % 6)));
	}
}