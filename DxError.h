#pragma once
#pragma warning(push, 2)
#include <comdef.h>
#pragma warning(pop)
#include "RusError.h"

class DxError : public RusError
{
public:
#pragma warning(push)
#pragma warning(disable:4514)	// unreferenced inline function has been removed
	DxError() = default;
	explicit DxError(const char* msg) : RusError(msg) {};
	explicit DxError(const wchar_t* rusMsg) : RusError(rusMsg) {};
#pragma warning(pop)
#pragma warning(suppress:26439) // This kind of function may not throw, declare it noexcept
	virtual ~DxError() throw() override /* not final */ = default;
	
#pragma warning(push)
#pragma warning(disable:5045) // Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified
	template<class ErrorClass>
	static void ThrowIfFailed(HRESULT hResult, LPCTSTR msg)
	{
		if (FAILED(hResult)) throw ErrorClass((std::wstring(msg) + TEXT(".\n")
			+ _com_error(hResult).ErrorMessage()).c_str());
	}
#pragma warning(pop)
};

#define THROW_IF_FAILED(ERROR_CLASS, H_RES, MESG) \
	ERROR_CLASS::ThrowIfFailed<ERROR_CLASS>((H_RES), TEXT(MESG))