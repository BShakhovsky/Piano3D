#pragma once

class DxError : public std::exception
{
public:
#pragma warning(push)
#pragma warning(disable:4514)	// unreferenced inline function has been removed
	explicit DxError(const char* msg = "Unknown exception") : msg_(msg) {}
#pragma warning(pop)
	virtual ~DxError() throw() override final = default;
	virtual const char* what() const override final
	{
		return msg_.c_str();
	}
private:
	std::string msg_;
};