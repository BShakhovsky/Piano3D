#include "stdafx.h"
#include "Render.h"
#include "Render_pimpl.h"

using std::make_unique;

Render::Render(const HWND hWnd, const UINT width, const UINT height)
	: pimpl_(make_unique<Render_pimpl>(hWnd, width, height))
{}

Render::~Render() {}

void Render::Resize(const UINT width, const UINT height)
{
	pimpl_->Resize(width, height);
}

void Render::Draw() const
{
	pimpl_->Draw();
}