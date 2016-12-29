#pragma once

class DxError : public std::exception
{
public:
	explicit DxError(const char* msg = "Unknown exception") : msg_(msg) {}
	virtual ~DxError() throw() override final = default;
	virtual const char* what() const override final
	{
		return msg_.c_str();
	}
private:
	std::string msg_;
};