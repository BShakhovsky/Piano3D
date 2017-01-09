#include "stdafx.h"
#include "DxError.h"

using namespace std;

#define DEF_MSG "Unknown exception"
const string DxError::defMsg_ = DEF_MSG;
const wstring DxError::defRusMsg_ = TEXT(DEF_MSG);

DxError::DxError(const char* msg)
	: msg_(msg ? msg : defMsg_),
	rusMsg_(wstring(msg_.cbegin(), msg_.cend()))
{}
DxError::DxError(const wchar_t* rusMsg)
	: rusMsg_(rusMsg ? rusMsg : defRusMsg_)
{
	msg_ = string(rusMsg_.cbegin(), rusMsg_.cend());
}

void DxError::ThrowIfFailed(const HRESULT hResult, LPCTSTR msg)
{
	if (FAILED(hResult))
	{
		const auto error(
#ifdef UNICODE
			wstring
#else
			string
#endif
			(msg) + TEXT(".\n") + _com_error(hResult).ErrorMessage());

		throw DxError(error.c_str());
	}
}