#pragma once

class Mesh : boost::noncopyable
{
public:
	static constexpr float WHITE_LENGTH = 8.0f;

	Mesh();

	const std::vector<DirectX::SimpleMath::Vector3>& GetWhiteMiddlePositions() const
	{
		return whiteMiddlePositions_;
	}
	const std::vector<DirectX::SimpleMath::Vector3>& GetWhiteLeftPositions() const
	{
		return whiteLeftPositions_;
	}
	const std::vector<DirectX::SimpleMath::Vector3>& GetWhiteRightPositions() const
	{
		return whiteRightPositions_;
	}
	const std::vector<DirectX::SimpleMath::Vector3>& GetWhiteNormals() const
	{
		return whiteNormals_;
	}
	const std::vector<WORD>& GetWhiteIndices() const
	{
		return whiteIndices_;
	}

	const std::vector<DirectX::SimpleMath::Vector3>& GetBlackPositions() const
	{
		return blackPositions_;
	}
	const std::vector<DirectX::SimpleMath::Vector3>& GetBlackNormals() const
	{
		return blackNormals_;
	}
	const std::vector<WORD>& GetBlackIndices() const
	{
		return blackIndices_;
	}
private:
	static constexpr float WHITE_GAP = 0.35f, WHITE_WIDTH = 0.94f, BLACK_FILLET = 0.15f;

	std::vector<DirectX::SimpleMath::Vector3>
		whiteMiddlePositions_, whiteLeftPositions_, whiteRightPositions_, blackPositions_,
		whiteNormals_, blackNormals_;
	std::vector<WORD> whiteIndices_, blackIndices_;
};