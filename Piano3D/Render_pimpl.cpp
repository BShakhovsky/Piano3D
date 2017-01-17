#include "stdafx.h"
#include "Render_pimpl.h"
#include "RenderData.h"
#include "Frame.h"

using std::make_unique;

Render_pimpl::Render_pimpl(const HWND hWnd, const UINT width, const UINT height,
	const float cameraX, const float cameraY, const float cameraZ, LPCTSTR path)
	: data_(make_unique<RenderData>(hWnd, width, height, cameraX, cameraY, cameraZ, path))
{
	Resize(width, height);
	data_->RotateCamera(-40, 0, 0);
}
Render_pimpl::~Render_pimpl() {}

void Render_pimpl::Resize(const UINT width, const UINT height)
{
	data_->Resize(width, height);
}
void Render_pimpl::Draw() const
{
	using DirectX::Colors::SteelBlue;

	data_->BeginKeyboardReflectionBuffer();
	DrawScene(true);

	data_->DrawPianoDeskBuffer();

	data_->Begin2D();
	Frame frame(data_->GetDevice(), SteelBlue.v);
	data_->DrawPianoDesk();

	DrawScene();

	data_->DrawDebugInfo();


	data_->ReturnPressedKeys();
}
void Render_pimpl::DrawScene(const bool mirrowed) const
{
	data_->SwitchTo3D();
	for (int16_t i(0); i < 88; ++i)
		data_->DrawPianoKey(i, mirrowed);
}

void Render_pimpl::PressKey(const int16_t note) const
{
	data_->PressKey(note);
}
void Render_pimpl::AssignFingerNums(const int16_t note, const char* fingers, const bool leftHand) const
{
	data_->AssignFingerNums(note, fingers, leftHand);
}
void Render_pimpl::ReleaseAllKeys() const
{
	data_->ReleaseAllKeys();
}

bool Render_pimpl::Zoom(const bool decrease) const
{
	return data_->Zoom(decrease);
}
bool Render_pimpl::FitToWindow() const
{
	return data_->FitToWindow();
}
void Render_pimpl::MoveStart(const int screenX, const int screenY) const
{
	data_->MoveStart(screenX, screenY);
}
void Render_pimpl::MoveEnd(const int screenX, const int screenY) const
{
	data_->MoveEnd(screenX, screenY);
}
void Render_pimpl::RotateStart(const int screenX, const int screenY) const
{
	data_->RotatePianoStart(screenX, screenY);
}
void Render_pimpl::RotateEnd(const int screenX, const int screenY) const
{
	data_->RotatePianoEnd(screenX, screenY);
}