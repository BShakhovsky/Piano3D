#include "stdafx.h"
#include "Text.h"
#include "Batch.h"

using namespace std;
using namespace DirectX;
using namespace SimpleMath;

Text::Text(ID3D11Device* device, ID3D11DeviceContext* context)
	: sprite_(make_unique<SpriteBatch>(context))
{
	try
	{
		font_.reset(new SpriteFont(device, TEXT("Arial.SpriteFont")));
	}
	catch (const exception& e)
	{
		if (string(e.what()) == "BinaryReader") MessageBox(nullptr,
			TEXT("Could not find file 'Arial.SpriteFont'\n")
			TEXT("Some text will not be drawn unless you put the file back and restart the program"),
			TEXT("DirectX Error"), MB_OK | MB_ICONHAND);
		else throw;
	}
}

void Text::Draw(LPCTSTR text, const float x, const float y, const Color& color) const
{
	Batch<SpriteBatch> scene(sprite_);
	if (font_) font_->DrawString(sprite_.get(), text, Vector2(x, y), color);
}