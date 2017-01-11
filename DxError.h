#pragma once

class DxError : public std::exception
{
	static const std::string defMsg_;
	static const std::wstring defRusMsg_;
public:
	explicit DxError(const char* msg = nullptr);
	explicit DxError(const wchar_t* rusMsg = nullptr);
	
	virtual ~DxError() throw() override final = default;
	
#pragma warning(push)
#pragma warning(disable:4514)	// unreferenced inline function has been removed
	virtual const char* what() const override final
	{
		return msg_.c_str();
	}
#pragma warning(pop)

#ifdef UNICODE
	const wchar_t* RusWhat() const
	{
		return rusMsg_.c_str();
	}
#else
	const char* RusWhat() const
	{
		return what();
	}
#endif

	static void ThrowIfFailed(HRESULT, LPCTSTR msg = nullptr);
private:
	std::string msg_;
	std::wstring rusMsg_;
};