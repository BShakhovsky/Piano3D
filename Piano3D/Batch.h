#pragma once

class Batch : boost::noncopyable
{
	Batch() = delete;
public:
	explicit Batch(const std::shared_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionNormalColorTexture>>& batch)
		: batch_(batch)
	{
		batch_->Begin();
	}
	~Batch()
	{
		batch_->End();
	}
private:
	std::shared_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionNormalColorTexture>> batch_;
};