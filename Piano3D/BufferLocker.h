#pragma once

class BufferLocker : boost::noncopyable
{
	BufferLocker() = delete;
public:
	explicit BufferLocker(ID3D11Buffer* buffer, ID3D11DeviceContext* context)
		: mappedResource_({ 0 }),
		buffer_(buffer),
		context_(context)
	{
		const auto hResult(context_->Map(buffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource_));
		assert("Cannot lock the buffer" && SUCCEEDED(hResult));
	}
	~BufferLocker()
	{
		context_->Unmap(buffer_, 0);
	}
	void* GetData()
	{
		return mappedResource_.pData;
	}
private:
	D3D11_MAPPED_SUBRESOURCE mappedResource_;
	ID3D11Buffer* buffer_;
	ID3D11DeviceContext* context_;
};