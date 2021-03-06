#pragma once

#pragma warning(push)
#pragma warning(disable:4514 4710) // Unreferenced inline function has been removed; Function not inlined
class RusError : public std::exception
{
public:
	RusError() : rusMsg_(TEXT("Unknown exception")) {}
	explicit RusError(const char* msg) : msg_(msg)
	{
		rusMsg_ = std::wstring(msg_.cbegin(), msg_.cend());
	}
	explicit RusError(const wchar_t* rusMsg) : rusMsg_(rusMsg) {}

#ifdef UNICODE
	const wchar_t* RusWhat() const
	{
		return rusMsg_.c_str();
	}
#else
	const char* RusWhat() const
	{
		if (msg_.empty()) msg_ = std::string(rusMsg_.cbegin(), rusMsg_.cend());
		return msg_.c_str();
	}
#endif
	virtual ~RusError() noexcept override = default;
private:
	virtual const char* what() const override final
	{
		assert(!"MidiError::RusWhat() should be called instead of what()");
		return nullptr;
	}

	std::string msg_;
	std::wstring rusMsg_;
};
#pragma warning(pop)