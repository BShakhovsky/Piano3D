#include "stdafx.h"
#include "Geometry.h"
#include "Mesh.h"
#include "Batch.h"

using namespace std;
using namespace DirectX;
using namespace Colors;
using namespace SimpleMath;

Geometry::Geometry(ID3D11DeviceContext* context)
	: mesh_(make_unique<Mesh>()),
	pianoDesk_(GeometricPrimitive::CreateBox(context, Vector3(58, 10, 2))),
	batch_(make_shared<PrimitiveBatch<VertexPositionNormalColorTexture>>(context)),

	whiteMiddle_(mesh_->GetWhiteMiddlePositions().size()),
	whiteLeft_(mesh_->GetWhiteLeftPositions().size()),
	whiteRight_(mesh_->GetWhiteRightPositions().size()),
	black_(mesh_->GetBlackPositions().size())
{
	for (size_t i(0); i < whiteMiddle_.size(); ++i)
	{
		whiteMiddle_.at(i).position = mesh_->GetWhiteMiddlePositions().at(i);
		whiteLeft_.at(i).position = mesh_->GetWhiteLeftPositions().at(i);
		whiteRight_.at(i).position = mesh_->GetWhiteRightPositions().at(i);
		whiteLeft_.at(i).normal = whiteRight_.at(i).normal = whiteMiddle_.at(i).normal = mesh_->GetWhiteNormals().at(i);
		whiteLeft_.at(i).color = whiteRight_.at(i).color = whiteMiddle_.at(i).color = Color(AliceBlue.v);
	}
	for (size_t i(0); i < black_.size(); ++i)
	{
		black_.at(i).position = mesh_->GetBlackPositions().at(i);
		black_.at(i).normal = mesh_->GetBlackNormals().at(i);
		black_.at(i).color = Color(0.15f, 0.15f, 0.15f);
	}
}

Geometry::~Geometry() {}

Matrix Geometry::GetWorldMatrix(const int note) const
{
	auto offset(0);

	if		(note == 0)		offset = 0;
	else if (note == 1)		offset = 0;
	else if (note == 2)		offset = 1;
	else if (note == 87)	offset = (87 - 3) / 12 * 7 + 2;
	else switch ((note - 3) % 12)
	{
	case 0:					offset = (note - 3) / 12 * 7 + 2; break;
	case 1:					offset = (note - 3) / 12 * 7 + 2; break;
	case 2:					offset = (note - 3) / 12 * 7 + 3; break;
	case 3:					offset = (note - 3) / 12 * 7 + 3; break;
	case 4:					offset = (note - 3) / 12 * 7 + 4; break;
	case 5:					offset = (note - 3) / 12 * 7 + 5; break;
	case 6:					offset = (note - 3) / 12 * 7 + 5; break;
	case 7:					offset = (note - 3) / 12 * 7 + 6; break;
	case 8:					offset = (note - 3) / 12 * 7 + 6; break;

	case 9:					offset = (note - 3) / 12 * 7 + 7; break;
	case 10:				offset = (note - 3) / 12 * 7 + 7; break;
	case 11:				offset = (note - 3) / 12 * 7 + 8; break;
	default: assert(!"Wrong note");
	}

	return move(Matrix::CreateTranslation(Vector3(static_cast<float>(offset), 0, 0)));
}

void Geometry::DrawKeyboard(const int note, const string& fingerNumbers)
{
	if		(note == 0)		DrawWhiteLeft	(fingerNumbers);
	else if (note == 1)		DrawBlack		(fingerNumbers);
	else if (note == 2)		DrawWhiteRight	(fingerNumbers);
	else if (note == 87)
	{
							DrawWhiteLeft	(fingerNumbers);
							DrawWhiteRight	(fingerNumbers);
	}
	else switch ((note - 3) % 12)
	{
	case 0:					DrawWhiteLeft	(fingerNumbers); break;
	case 1:					DrawBlack		(fingerNumbers); break;
	case 2:					DrawWhiteMiddle	(fingerNumbers); break;
	case 3:					DrawBlack		(fingerNumbers); break;
	case 4:					DrawWhiteRight	(fingerNumbers); break;
	case 5:					DrawWhiteLeft	(fingerNumbers); break;
	case 6:					DrawBlack		(fingerNumbers); break;
	case 7:					DrawWhiteMiddle	(fingerNumbers); break;
	case 8:					DrawBlack		(fingerNumbers); break;

	case 9:					DrawWhiteMiddle	(fingerNumbers); break;
	case 10:				DrawBlack		(fingerNumbers); break;
	case 11:				DrawWhiteRight	(fingerNumbers); break;
	default: assert(!"Wrong note");
	}
}

void Geometry::DrawBlack(const string& fingerNumbers)
{
	UpdateFinger(black_.data(), black_.data() + 4, fingerNumbers);

	Batch<PrimitiveBatch<VertexPositionNormalColorTexture>> scene(batch_);
	batch_->DrawIndexed(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, mesh_->GetBlackIndices().data(),
		mesh_->GetBlackIndices().size(), black_.data(), black_.size());
}

void Geometry::DrawWhiteLeft(const string& fingerNumbers)
{
	UpdateFinger(whiteLeft_.data() + 4, whiteLeft_.data(), fingerNumbers);

	Batch<PrimitiveBatch<VertexPositionNormalColorTexture>> scene(batch_);
	batch_->DrawIndexed(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, mesh_->GetWhiteIndices().data(),
		mesh_->GetWhiteIndices().size(), whiteLeft_.data(), whiteLeft_.size());
}

void Geometry::DrawWhiteMiddle(const string& fingerNumbers)
{
	UpdateFinger(whiteMiddle_.data() + 4, whiteMiddle_.data(), fingerNumbers);

	Batch<PrimitiveBatch<VertexPositionNormalColorTexture>> scene(batch_);
	batch_->DrawIndexed(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, mesh_->GetWhiteIndices().data(),
		mesh_->GetWhiteIndices().size(), whiteMiddle_.data(), whiteMiddle_.size());
}

void Geometry::DrawWhiteRight(const string& fingerNumbers)
{
	UpdateFinger(whiteRight_.data() + 4, whiteRight_.data(), fingerNumbers);

	Batch<PrimitiveBatch<VertexPositionNormalColorTexture>> scene(batch_);
	batch_->DrawIndexed(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, mesh_->GetWhiteIndices().data(),
		mesh_->GetWhiteIndices().size(), whiteRight_.data(), whiteRight_.size());
}

void Geometry::UpdateFinger(VertexPositionNormalColorTexture* destination1,
	VertexPositionNormalColorTexture* destination2, const string& fingerNumbers) const
{
	static constexpr auto numSymbols(7);

	if (!fingerNumbers.empty())
	{
		const float symbolCoord(fingerNumbers.front() == '?' ? 0.0f : fingerNumbers.front() - '0');
		destination1[0].textureCoordinate = Vector2(symbolCoord / numSymbols, 0);
		destination1[1].textureCoordinate = Vector2((symbolCoord + 1) / numSymbols, 0);
		destination1[2].textureCoordinate = Vector2(symbolCoord / numSymbols, 1);
		destination1[3].textureCoordinate = Vector2((symbolCoord + 1) / numSymbols, 1);
	}
	else destination1[0].textureCoordinate = destination1[1].textureCoordinate
		= destination1[2].textureCoordinate = destination1[3].textureCoordinate = Vector2(0, 0);

	if (fingerNumbers.size() > 1)
	{
		const float symbolCoord(fingerNumbers.back() == '?' ? 0.0f : fingerNumbers.back() - '0');
		destination2[0].textureCoordinate = Vector2(symbolCoord / numSymbols, 0);
		destination2[1].textureCoordinate = Vector2((symbolCoord + 1) / numSymbols, 0);
		destination2[2].textureCoordinate = Vector2(symbolCoord / numSymbols, 1);
		destination2[3].textureCoordinate = Vector2((symbolCoord + 1) / numSymbols, 1);
	}
	else destination2[0].textureCoordinate = destination2[1].textureCoordinate
		= destination2[2].textureCoordinate = destination2[3].textureCoordinate = Vector2(0, 0);
}