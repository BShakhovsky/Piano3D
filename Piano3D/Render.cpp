#include "stdafx.h"
#include "Render.h"
#include "Render_pimpl.h"

using std::make_unique;

#pragma warning(push)
#pragma warning(disable:4711)

Render::Render(const HWND hWnd, const UINT width, const UINT height,
	const float cameraX, const float cameraY, const float cameraZ, LPCTSTR path)
	: pimpl_(new Render_pimpl(hWnd, width, height, cameraX, cameraY, cameraZ, path))
{}
Render::~Render()
{
	delete pimpl_;
}

void Render::Resize(const UINT width, const UINT height) const
{
	pimpl_->Resize(width, height);
}
void Render::Draw() const
{
	pimpl_->Draw();
}

void Render::PressKey(const int16_t note) const
{
	pimpl_->PressKey(note);
}
void Render::AssignFingerNums(const int16_t note, const char* fingers, const bool leftHand) const
{
	pimpl_->AssignFingerNums(note, fingers, leftHand);
}
void Render::ReleaseAllKeys() const
{
	pimpl_->ReleaseAllKeys();
}

bool Render::Zoom(const bool increase) const
{
	return pimpl_->Zoom(increase);
}
bool Render::FitToWindow() const
{
	return pimpl_->FitToWindow();
}
void Render::MoveStart(const float screenX, const float screenY) const
{
	pimpl_->MoveStart(screenX, screenY);
}
void Render::MoveEnd(const float screenX, const float screenY) const
{
	pimpl_->MoveEnd(screenX, screenY);
}
void Render::RotateStart(const float screenX, const float screenY) const
{
	pimpl_->RotateStart(screenX, screenY);
}
void Render::RotateEnd(const float screenX, const float screenY) const
{
	pimpl_->RotateEnd(screenX, screenY);
}

#pragma warning(pop)