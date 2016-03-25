#include "stdafx.h"
#include "Render.h"
#include "Frame.h"

using namespace DirectX::Colors;
using namespace DirectX::SimpleMath;

void Render::Resize(const UINT width, const UINT height)
{
	width_ = width;
	height_ = height;
	device_->Resize(width_, height_);
	pianoDesk_ = std::make_unique<BackBuffer2D>(device_->GetDevice(), device_->GetContext(),
		device_->GetDepthBuffer(), width_, height_);
	keyboardReflection_ = std::make_unique<BackBuffer2D>(device_->GetDevice(), device_->GetContext(),
		device_->GetDepthBuffer(), width_, height_);
}

void Render::Draw() const
{
	keyboardReflection_->SetTargetBuffer(device_->GetContext(), device_->GetDepthView());
	keyboardReflection_->BeginFrame(device_->GetContext(), device_->GetDepthView(), Black.v);
	shader_->UpdateLights(false);
	DrawScene(camera_->GetMirrowedViewMatrix());

	pianoDesk_->SetTargetBuffer(device_->GetContext(), device_->GetDepthView());
	pianoDesk_->BeginFrame(device_->GetContext(), device_->GetDepthView(), SteelBlue.v);
	shader_->UpdateLights(true);
	shader_->Apply(device_->GetDevice(), device_->GetContext());
	geometry_->DrawDesk(camera_->GetViewMatrix(), device_->GetProjection(), nullptr,
		device_->GetDevice(), device_->GetContext());

	device_->Mode2D();
	device_->SetTargetBuffer();
	shader_->UpdateProjection(device_->GetOrthographic());
	shader_->UpdateWorldView(Matrix::Identity,
		Matrix::CreateLookAt(Vector3::Backward, Vector3::Zero, Vector3::Up));

	Frame frame(device_, SteelBlue.v);

	shader_->UpdateTexture(keyboardReflection_->GetTexture(device_->GetContext()));
	shader_->Apply(device_->GetDevice(), device_->GetContext());
	keyboardReflection_->Draw();

	shader_->UpdateTexture(pianoDesk_->GetTexture(device_->GetContext()));
	shader_->Apply(device_->GetDevice(), device_->GetContext());
	device_->AlfaBlendingOn();
	pianoDesk_->Draw();
	device_->AlfaBlendingOff();

	DrawScene(camera_->GetViewMatrix());

	DrawDebugInfo();
}

void Render::DrawScene(const Matrix& view) const
{
	device_->Mode3D();
	shader_->UpdateTexture();
	shader_->UpdateProjection(device_->GetProjection());
	for (auto i(0); i < 88; ++i)
	{
		shader_->UpdateWorldView(geometry_->GetWorldMatrix(i), view);
		shader_->Apply(device_->GetDevice(), device_->GetContext());
		geometry_->DrawKeyboard(i, std::string(static_cast<size_t>(i % 3), static_cast<char>('0' + i % 6)));
	}
}

void Render::DrawDebugInfo() const
{
	using boost::wformat;

	static auto startTime(GetTickCount() - 1), frames(0ul), fps(0ul);
	++frames;

	const auto deltaTime(GetTickCount() - startTime);
	if (deltaTime > 1'000)
	{
		fps = frames * 1'000 / deltaTime;
		startTime += 1'000;
		frames = 0;
	}

	const auto text((wformat{ TEXT("FPS = %1%\nX = %2%\nY = %3%\nZ = %4%") }
		% fps % camera_->GetPosition().x % camera_->GetPosition().y % camera_->GetPosition().z).str());
	text_->Draw(text.c_str(), static_cast<float>(width_ - 150), 5.f, Color(fps > 50 ? Green.v : Red.v));
}