#include "stdafx.h"
#include "Text.h"
#include "Batch.h"

using namespace std;
using namespace DirectX;
using namespace SimpleMath;

Text::Text(ID3D11Device* device, ID3D11DeviceContext* context, LPCTSTR path)
	: sprite_(make_unique<SpriteBatch>(context))
{
	try
	{
		font_.reset(new SpriteFont(device, (wstring(path) + TEXT("\\Arial.SpriteFont")).c_str()));
	}
	catch (const exception& e)
	{
		if (string(e.what()) == "BinaryReader") MessageBox(GetActiveWindow(),
			(TEXT("Could not find the following file:\n'") + wstring(path) + TEXT("\\Arial.SpriteFont'\n")
			TEXT("Some text will not be drawn unless you put the file back and restart the program")).c_str(),
			TEXT("DirectX Error"), MB_OK | MB_ICONHAND);
		else throw;
	}
}

void Text::Draw(LPCTSTR text, const float x, const float y, const Color& color) const
{
	Batch<SpriteBatch> scene(sprite_);
	if (font_) font_->DrawString(sprite_.get(), text, Vector2(x, y), color);
}