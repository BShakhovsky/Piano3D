#include "stdafx.h"
#include "RenderData.h"
#include "KeyStates.h"

#include "FramesPerSec.h"

#include "Device.h"
#include "Camera.h"
#include "Shader.h"
#include "Geometry.h"
#include "Text.h"
#include "BackBuffer2D.h"

using namespace std;
using namespace DirectX::SimpleMath;
using namespace DirectX::Colors;

RenderData::RenderData(const HWND hWnd, const UINT width, const UINT height, LPCTSTR path)
	: width_(width), height_(height),

	device_(make_shared<Device>(hWnd, width_, height_)),
	camera_(make_unique<Camera>()),
	shader_(make_unique<Shader>(device_->GetDevice())),
	geometry_(make_unique<Geometry>(device_->GetContext())),
	text_(make_unique<Text>(device_->GetDevice(), device_->GetContext(), path)),

	pianoDesk_(make_unique<BackBuffer2D>(device_->GetDevice(), device_->GetContext(),
		device_->GetDepthBuffer(), width_, height_)),
	keyboardReflection_(make_unique<BackBuffer2D>(device_->GetDevice(), device_->GetContext(),
		device_->GetDepthBuffer(), width_, height_, true)),

	keyStates_(make_unique<KeyStates>())
{}
RenderData::~RenderData() {}

const shared_ptr<Device>& RenderData::GetDevice() const
{
	return device_;
}
void RenderData::Resize(const UINT width, const UINT height)
{
	width_ = width;
	height_ = height;

	device_->Resize(width_, height_);
	keyboardReflection_ = std::make_unique<BackBuffer2D>(device_->GetDevice(), device_->GetContext(),
		device_->GetDepthBuffer(), width_, height_, true);
	pianoDesk_ = std::make_unique<BackBuffer2D>(device_->GetDevice(), device_->GetContext(),
		device_->GetDepthBuffer(), width_, height_);
}
void RenderData::Begin2D() const
{
	device_->Mode2D();
	device_->SetTargetBuffer();
	shader_->UpdateProjection(device_->GetOrthographic());
	shader_->UpdateWorldView(Matrix::Identity,
		Matrix::CreateLookAt(Vector3::Backward, Vector3::Zero, Vector3::Up));
}
void RenderData::SwitchTo3D() const
{
	device_->Mode3D();
	shader_->UpdateTexture();
	shader_->UpdateProjection(device_->GetProjection());
}

#pragma warning(push)
#pragma warning(disable: 4711) // Automatic inline expansion
bool RenderData::Restore3DPosition() const
{
	return camera_->RestorePosition();
}
bool RenderData::Zoom(const bool increase) const
{
	return camera_->Zoom(increase);
}
bool RenderData::FitToWindow() const
{
	return camera_->FitToWindow(device_->GetProjection());
}

void RenderData::MoveStart(const float screenX, const float screenY) const
{
	camera_->MoveStart(screenX, screenY);
}
void RenderData::MoveEnd(const float screenX, const float screenY) const
{
	camera_->MoveEnd(screenX, screenY, device_->GetProjection());
}
void RenderData::RotatePianoStart(const float screenX, const float screenY) const
{
	camera_->RotatePianoStart(screenX, screenY);
}
void RenderData::RotatePianoEnd(const float screenX, const float screenY) const
{
	camera_->RotatePianoEnd(screenX, screenY);
}
#pragma warning(pop)

void RenderData::PressKey(const int16_t note) const
{
	keyStates_->PressKey(note);
}
void RenderData::AssignFingerNums(const int16_t note, const char* fingers, const bool leftHand) const
{
	keyStates_->AssignFinger(note, fingers, leftHand);
}
void RenderData::ReleaseAllKeys() const
{
	keyStates_->ReleaseAllKeys();
}
void RenderData::ReturnPressedKeys() const
{
	keyStates_->ReturnPressedKeys();
}

void RenderData::BeginKeyboardReflectionBuffer() const
{
	keyboardReflection_->SetTargetBuffer(device_->GetContext(), device_->GetDepthView());
	keyboardReflection_->BeginFrame(device_->GetContext(), device_->GetDepthView(), Black.v);
	shader_->UpdateLights(false);
}
void RenderData::DrawPianoDeskBuffer() const
{
	BeginPianoDeskBuffer();
	ApplyShader();

	geometry_->DrawDesk(device_->GetDevice(), device_->GetContext(),
		camera_->GetViewMatrix(), device_->GetProjection(), shader_->GetDeskTexture());
}
void RenderData::DrawPianoDesk() const
{
	DrawKeyboardReflection();

	shader_->UpdateTexture(pianoDesk_->GetTexture(device_->GetContext()));
	ApplyShader();

	device_->AlfaBlendingOn();
	pianoDesk_->Draw();
	device_->AlfaBlendingOff();
}
void RenderData::DrawCotton(const bool mirrowed) const
{
	geometry_->DrawCotton(mirrowed ? camera_->GetMirrowedViewMatrix()
		: camera_->GetViewMatrix(), device_->GetProjection());
}
void RenderData::DrawPianoKey(const int16_t note, const bool mirrowed) const
{
	shader_->UpdateWorldView(geometry_->GetRotatedMatrix(note, keyStates_->GetAngle(note)),
		mirrowed ? camera_->GetMirrowedViewMatrix() : camera_->GetViewMatrix());
	shader_->UpdateTexture(nullptr, keyStates_->IsLeftHand(note));
	ApplyShader();

	geometry_->DrawKey(note, keyStates_->IsHighlighted(note), keyStates_->GetFingers(note));
}
void RenderData::DrawDebugInfo() const
{
	using boost::wformat;

	static FramesPerSec fps;
	fps.Calculate();

	const auto text((wformat
#ifdef _DEBUG
	{ TEXT("FPS = %u\nX = %.1f\nY = %.1f\nZ = %.1f") } %
		fps.Get() % camera_->GetPosition().x % camera_->GetPosition().y % camera_->GetPosition().z
#else
	{ TEXT("FPS = %1%") } % fps.Get()
#endif
		).str());
	text_->Draw(text.c_str(), static_cast<float>(width_ - 150), 5.f,
		Color(fps.Get() > 50 ? Lime.v : fps.Get() > 30 ? Gold.v : Red.v));
}

void RenderData::BeginPianoDeskBuffer() const
{
	pianoDesk_->SetTargetBuffer(device_->GetContext(), device_->GetDepthView());
	const auto darkCoeff(.65f);
	pianoDesk_->BeginFrame(device_->GetContext(), device_->GetDepthView(),
		{ SteelBlue[0] * darkCoeff, SteelBlue[1] * darkCoeff, SteelBlue[2] * darkCoeff });
	shader_->UpdateLights(true);
}
#pragma warning(push)
#pragma warning(disable: 4711) // Automatic inline expansion
void RenderData::DrawKeyboardReflection() const
{
	shader_->UpdateTexture(keyboardReflection_->GetTexture(device_->GetContext()));
	ApplyShader();

	keyboardReflection_->Draw();
}
#pragma warning(pop)

void RenderData::ApplyShader() const
{
	shader_->Apply(device_->GetDevice(), device_->GetContext());
}