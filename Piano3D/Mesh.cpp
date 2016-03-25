#include "stdafx.h"
#include "Mesh.h"
#include "DxError.h"

Mesh::Mesh()
	: whiteMiddlePositions_
{
	{ 0 * WHITE_WIDTH, 1, 5 },
	{ 1 * WHITE_WIDTH, 1, 5 },
	{ 0 * WHITE_WIDTH, 1, 6.5f },
	{ 1 * WHITE_WIDTH, 1, 6.5f },
	{ 0 * WHITE_WIDTH, 1, 6.5f },
	{ 1 * WHITE_WIDTH, 1, 6.5f },
	{ 0 * WHITE_WIDTH, 1, 8 },
	{ 1 * WHITE_WIDTH, 1, 8 },
	
	{ (0 + WHITE_GAP) * WHITE_WIDTH, 1, 0 },
	{ (1 - WHITE_GAP) * WHITE_WIDTH, 1, 0 },
	{ (0 + WHITE_GAP) * WHITE_WIDTH, 1, 5 },
	{ (1 - WHITE_GAP) * WHITE_WIDTH, 1, 5 },

	{ (0 + WHITE_GAP) * WHITE_WIDTH, 0, 0 },
	{ (1 - WHITE_GAP) * WHITE_WIDTH, 0, 0 },
	{ (0 + WHITE_GAP) * WHITE_WIDTH, 0, 5 },
	{ (1 - WHITE_GAP) * WHITE_WIDTH, 0, 5 },

	{ 0 * WHITE_WIDTH, 0, 5 },
	{ 1 * WHITE_WIDTH, 0, 5 },
	{ 0 * WHITE_WIDTH, 0, 8 },
	{ 1 * WHITE_WIDTH, 0, 8 }
},
whiteIndices_
{
	0, 1, 2,		2, 1, 3,		4, 5, 6,		6, 5, 7,		8, 9, 10,		10, 9, 11,
	13, 12, 15,		15, 12, 14,		17, 16, 19,		19, 16, 18
},

blackPositions_
{
	{ (1 - WHITE_GAP + BLACK_FILLET) * WHITE_WIDTH, 1.5f, 2.7f },
	{ (1 + WHITE_GAP - BLACK_FILLET) * WHITE_WIDTH, 1.5f, 2.7f },
	{ (1 - WHITE_GAP + BLACK_FILLET) * WHITE_WIDTH, 1.5f, 3.7f },
	{ (1 + WHITE_GAP - BLACK_FILLET) * WHITE_WIDTH, 1.5f, 3.7f },
	{ (1 - WHITE_GAP + BLACK_FILLET) * WHITE_WIDTH, 1.5f, 3.7f },
	{ (1 + WHITE_GAP - BLACK_FILLET) * WHITE_WIDTH, 1.5f, 3.7f },
	{ (1 - WHITE_GAP + BLACK_FILLET) * WHITE_WIDTH, 1.5f, 4.7f },
	{ (1 + WHITE_GAP - BLACK_FILLET) * WHITE_WIDTH, 1.5f, 4.7f },
	{ (1 - WHITE_GAP + BLACK_FILLET) * WHITE_WIDTH, 1.5f, 0 },
	{ (1 + WHITE_GAP - BLACK_FILLET) * WHITE_WIDTH, 1.5f, 0 },

	{ (1 - WHITE_GAP) * WHITE_WIDTH, 0.5f, 0 },
	{ (1 + WHITE_GAP) * WHITE_WIDTH, 0.5f, 0 },
	{ (1 - WHITE_GAP) * WHITE_WIDTH, 0.5f, 5 },
	{ (1 + WHITE_GAP) * WHITE_WIDTH, 0.5f, 5 }
},
blackIndices_
{
	0, 1, 2,		2, 1, 3,		4, 5, 6,		6, 5, 7,		8, 9, 0,		0, 9, 1,
	11, 10, 13,		13, 10, 12
}
{
	using std::vector;
	using namespace DirectX;
	using SimpleMath::Vector3;

	whiteMiddlePositions_.reserve(50);
	whiteMiddlePositions_.push_back(whiteMiddlePositions_.at(8));
	whiteMiddlePositions_.push_back(whiteMiddlePositions_.at(10));
	whiteMiddlePositions_.push_back(whiteMiddlePositions_.at(12));
	whiteMiddlePositions_.push_back(whiteMiddlePositions_.at(14));
	whiteMiddlePositions_.push_back(whiteMiddlePositions_.at(0));
	whiteMiddlePositions_.push_back(whiteMiddlePositions_.at(6));
	whiteMiddlePositions_.push_back(whiteMiddlePositions_.at(16));
	whiteMiddlePositions_.push_back(whiteMiddlePositions_.at(18));

	whiteMiddlePositions_.push_back(whiteMiddlePositions_.at(7));
	whiteMiddlePositions_.push_back(whiteMiddlePositions_.at(1));
	whiteMiddlePositions_.push_back(whiteMiddlePositions_.at(19));
	whiteMiddlePositions_.push_back(whiteMiddlePositions_.at(17));
	whiteMiddlePositions_.push_back(whiteMiddlePositions_.at(11));
	whiteMiddlePositions_.push_back(whiteMiddlePositions_.at(9));
	whiteMiddlePositions_.push_back(whiteMiddlePositions_.at(15));
	whiteMiddlePositions_.push_back(whiteMiddlePositions_.at(13));

	whiteMiddlePositions_.push_back(whiteMiddlePositions_.at(1));
	whiteMiddlePositions_.push_back(whiteMiddlePositions_.at(11));
	whiteMiddlePositions_.push_back(whiteMiddlePositions_.at(17));
	whiteMiddlePositions_.push_back(whiteMiddlePositions_.at(15));
	whiteMiddlePositions_.push_back(whiteMiddlePositions_.at(9));
	whiteMiddlePositions_.push_back(whiteMiddlePositions_.at(8));
	whiteMiddlePositions_.push_back(whiteMiddlePositions_.at(13));
	whiteMiddlePositions_.push_back(whiteMiddlePositions_.at(12));
	whiteMiddlePositions_.push_back(whiteMiddlePositions_.at(10));
	whiteMiddlePositions_.push_back(whiteMiddlePositions_.at(0));
	whiteMiddlePositions_.push_back(whiteMiddlePositions_.at(14));
	whiteMiddlePositions_.push_back(whiteMiddlePositions_.at(16));

	whiteMiddlePositions_.push_back(whiteMiddlePositions_.at(6));
	whiteMiddlePositions_.push_back(whiteMiddlePositions_.at(7));
	whiteMiddlePositions_.push_back(whiteMiddlePositions_.at(18));
	whiteMiddlePositions_.push_back(whiteMiddlePositions_.at(19));

	whiteLeftPositions_ = whiteRightPositions_ = whiteMiddlePositions_;
	for (auto& pos : whiteLeftPositions_) if (pos.x == WHITE_GAP * WHITE_WIDTH) pos.x = 0.01f;
	for (auto& pos : whiteRightPositions_) if (pos.x == (1 - WHITE_GAP) * WHITE_WIDTH) pos.x = WHITE_WIDTH - 0.01f;

	blackPositions_.reserve(28);
	blackPositions_.push_back(blackPositions_.at(8));
	blackPositions_.push_back(blackPositions_.at(6));
	blackPositions_.push_back(blackPositions_.at(10));
	blackPositions_.push_back(blackPositions_.at(12));

	blackPositions_.push_back(blackPositions_.at(7));
	blackPositions_.push_back(blackPositions_.at(9));
	blackPositions_.push_back(blackPositions_.at(13));
	blackPositions_.push_back(blackPositions_.at(11));

	blackPositions_.push_back(blackPositions_.at(9));
	blackPositions_.push_back(blackPositions_.at(8));
	blackPositions_.push_back(blackPositions_.at(11));
	blackPositions_.push_back(blackPositions_.at(10));

	blackPositions_.push_back(blackPositions_.at(6));
	blackPositions_.push_back(blackPositions_.at(7));
	blackPositions_.push_back(blackPositions_.at(12));
	blackPositions_.push_back(blackPositions_.at(13));

	const auto AppendIndices([](vector<WORD>* indices, size_t indexCount, size_t startIndex, size_t startPosIndex)
	{
		indices->resize(indexCount);
		for (size_t i(0); i < (indexCount - startIndex) / 6; ++i)
		{
			indices->at(startIndex + i * 6 + 0) = static_cast<WORD>(startPosIndex + i * 4 + 0);
			indices->at(startIndex + i * 6 + 1) = static_cast<WORD>(startPosIndex + i * 4 + 1);
			indices->at(startIndex + i * 6 + 2) = static_cast<WORD>(startPosIndex + i * 4 + 2);

			indices->at(startIndex + i * 6 + 3) = static_cast<WORD>(startPosIndex + i * 4 + 2);
			indices->at(startIndex + i * 6 + 4) = static_cast<WORD>(startPosIndex + i * 4 + 1);
			indices->at(startIndex + i * 6 + 5) = static_cast<WORD>(startPosIndex + i * 4 + 3);
		}
	});
	AppendIndices(&whiteIndices_, 78, 30, 20);
	AppendIndices(&blackIndices_, 48, 24, 14);

#ifdef _DEBUG
	vector<unsigned> adjacency(whiteIndices_.size());
	assert("Could not generate mesh adjacent indices" && SUCCEEDED(GenerateAdjacencyAndPointReps(
		whiteIndices_.data(), whiteIndices_.size() / 3, whiteMiddlePositions_.data(), whiteMiddlePositions_.size(),
		0, nullptr, adjacency.data())));
	assert("Mesh indices are not correct" && SUCCEEDED(Validate(
		whiteIndices_.data(), whiteIndices_.size() / 3, whiteMiddlePositions_.size(), adjacency.data(),
		VALIDATE_BACKFACING | VALIDATE_BOWTIES | VALIDATE_ASYMMETRIC_ADJ | VALIDATE_DEGENERATE | VALIDATE_UNUSED, nullptr)));

	assert("Could not generate mesh adjacent indices" && SUCCEEDED(GenerateAdjacencyAndPointReps(
		whiteIndices_.data(), whiteIndices_.size() / 3, whiteLeftPositions_.data(), whiteLeftPositions_.size(),
		0, nullptr, adjacency.data())));
	assert("Mesh indices are not correct" && SUCCEEDED(Validate(
		whiteIndices_.data(), whiteIndices_.size() / 3, whiteLeftPositions_.size(), adjacency.data(),
		VALIDATE_BACKFACING | VALIDATE_BOWTIES | VALIDATE_ASYMMETRIC_ADJ | VALIDATE_DEGENERATE | VALIDATE_UNUSED, nullptr)));

	assert("Could not generate mesh adjacent indices" && SUCCEEDED(GenerateAdjacencyAndPointReps(
		whiteIndices_.data(), whiteIndices_.size() / 3, whiteRightPositions_.data(), whiteRightPositions_.size(),
		0, nullptr, adjacency.data())));
	assert("Mesh indices are not correct" && SUCCEEDED(Validate(
		whiteIndices_.data(), whiteIndices_.size() / 3, whiteRightPositions_.size(), adjacency.data(),
		VALIDATE_BACKFACING | VALIDATE_BOWTIES | VALIDATE_ASYMMETRIC_ADJ | VALIDATE_DEGENERATE | VALIDATE_UNUSED, nullptr)));

	adjacency.resize(blackIndices_.size());
	assert("Could not generate mesh adjacent indices" && SUCCEEDED(GenerateAdjacencyAndPointReps(
		blackIndices_.data(), blackIndices_.size() / 3, blackPositions_.data(), blackPositions_.size(),
		0, nullptr, adjacency.data())));
	assert("Mesh indices are not correct" && SUCCEEDED(Validate(
		blackIndices_.data(), blackIndices_.size() / 3, blackPositions_.size(), adjacency.data(),
		VALIDATE_BACKFACING | VALIDATE_BOWTIES | VALIDATE_ASYMMETRIC_ADJ | VALIDATE_DEGENERATE | VALIDATE_UNUSED, nullptr)));
#endif
	const auto CalculateNormals = [](const vector<Vector3>& positions, vector<Vector3>* normals,
		size_t index1, size_t index2, size_t index3)
	{
		normals->at(index1) = normals->at(index2) = normals->at(index3)
			= (positions.at(index3) - positions.at(index1)).Cross(positions.at(index2) - positions.at(index1));
	};
	whiteNormals_.resize(whiteMiddlePositions_.size());
	blackNormals_.resize(blackPositions_.size());
	for (size_t i(0); i < whiteIndices_.size() / 3; ++i)
		CalculateNormals(whiteMiddlePositions_, &whiteNormals_,
			whiteIndices_.at(i * 3), whiteIndices_.at(i * 3 + 1), whiteIndices_.at(i * 3 + 2));
	for (size_t i(0); i < blackIndices_.size() / 3; ++i)
		CalculateNormals(blackPositions_, &blackNormals_,
			blackIndices_.at(i * 3), blackIndices_.at(i * 3 + 1), blackIndices_.at(i * 3 + 2));
}