#pragma once

class FontGdi : boost::noncopyable
{
	FontGdi() = delete;
public:
	explicit FontGdi(HDC hDC, const LOGFONT* logFont, COLORREF color, int bkMode)
		: hDC_(hDC),
		hNewFont_(CreateFontIndirect(logFont)),
		hOldFont_(SelectFont(hDC_, hNewFont_)),
		oldColor_(SetTextColor(hDC_, color)),
		oldBkMode_(SetBkMode(hDC_, bkMode))
	{}
	~FontGdi()
	{
		SetBkMode(hDC_, oldBkMode_);
		SetTextColor(hDC_, oldColor_);
		SelectFont(hDC_, hOldFont_);
		DeleteFont(hNewFont_);
	}
private:
	const HDC hDC_;
	const HFONT hNewFont_, hOldFont_;
	const COLORREF oldColor_;
	const int oldBkMode_;
};