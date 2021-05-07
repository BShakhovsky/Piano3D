#include "stdafx.h"
#include "Render_pimpl.h"
#include "RenderData.h"
#include "FrameDx.h"

using std::make_unique;

Render_pimpl::Render_pimpl(const HWND hWnd, const UINT width, const UINT height, LPCTSTR path)
	: data_(make_unique<RenderData>(hWnd, width, height, path))
{
	Resize(width, height);
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
	FrameDx frame(data_->GetDevice(), SteelBlue.v);
	data_->DrawPianoDesk();

	DrawScene();

	data_->DrawDebugInfo();


	data_->ReturnPressedKeys();
}
#pragma warning(push)
#pragma warning(disable:5045) // Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified
void Render_pimpl::DrawScene(const bool mirrowed) const
{
	data_->SwitchTo3D();
	data_->DrawCotton(mirrowed);
	for (int16_t i(0); i < 88; ++i)
		data_->DrawPianoKey(i, mirrowed);
}
#pragma warning(pop)

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

bool Render_pimpl::Restore3DPosition() const
{
	return data_->Restore3DPosition();
}
bool Render_pimpl::Zoom(const bool increase) const
{
	return data_->Zoom(increase);
}
bool Render_pimpl::FitToWindow() const
{
	return data_->FitToWindow();
}

void Render_pimpl::MoveStart(const float screenX, const float screenY) const
{
	data_->MoveStart(screenX, screenY);
}
void Render_pimpl::MoveEnd(const float screenX, const float screenY) const
{
	data_->MoveEnd(screenX, screenY);
}
void Render_pimpl::RotateStart(const float screenX, const float screenY) const
{
	data_->RotatePianoStart(screenX, screenY);
}
void Render_pimpl::RotateEnd(const float screenX, const float screenY) const
{
	data_->RotatePianoEnd(screenX, screenY);
}