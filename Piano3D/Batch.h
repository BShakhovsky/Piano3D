#pragma once

template<typename BatchType>
class Batch : boost::noncopyable
{
	Batch() = delete;
public:
	explicit Batch(const std::shared_ptr<BatchType>& batch)
		: batch_(batch)
	{
		batch_->Begin();
	}
	~Batch()
	{
		batch_->End();
	}
private:
	std::shared_ptr<BatchType> batch_;
};