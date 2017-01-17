#include "stdafx.h"
#include "Geometry_pimpl.h"
#include "Mesh.h"
#include "Batch.h"

using namespace std;
using namespace DirectX;
using namespace Colors;
using namespace SimpleMath;

Geometry_pimpl::Geometry_pimpl(ID3D11DeviceContext* context)
	: mesh_(make_unique<Mesh>()),
	batch_(make_shared<PrimitiveBatch<VertexPositionNormalColorTexture>>(context)),

	whiteMiddle_(mesh_->GetWhiteMiddlePositions().size()),
	whiteLeft_(mesh_->GetWhiteLeftPositions().size()),
	whiteRight_(mesh_->GetWhiteRightPositions().size()),
	black_(mesh_->GetBlackPositions().size()),

	greyMiddle_(mesh_->GetWhiteMiddlePositions().size()),
	greyLeft_(mesh_->GetWhiteLeftPositions().size()),
	greyRight_(mesh_->GetWhiteRightPositions().size()),
	grey_(mesh_->GetBlackPositions().size()),

	keyIsGrey_(false)
{
	for (size_t i(0); i < whiteMiddle_.size(); ++i)
	{
		whiteMiddle_.at(i).position = greyMiddle_.at(i).position = mesh_->GetWhiteMiddlePositions().at(i);
		whiteLeft_.at(i).position = greyLeft_.at(i).position = mesh_->GetWhiteLeftPositions().at(i);
		whiteRight_.at(i).position = greyRight_.at(i).position = mesh_->GetWhiteRightPositions().at(i);
		whiteLeft_.at(i).normal = whiteRight_.at(i).normal = whiteMiddle_.at(i).normal
			= greyLeft_.at(i).normal = greyRight_.at(i).normal = greyMiddle_.at(i).normal
			= mesh_->GetWhiteNormals().at(i);
		whiteLeft_.at(i).color = whiteRight_.at(i).color = whiteMiddle_.at(i).color = Color(AliceBlue.v);
		greyLeft_.at(i).color = greyRight_.at(i).color = greyMiddle_.at(i).color = Color(Silver.v);
	}
	for (size_t i(0); i < black_.size(); ++i)
	{
		black_.at(i).position = grey_.at(i).position = mesh_->GetBlackPositions().at(i);
		black_.at(i).normal = grey_.at(i).normal = mesh_->GetBlackNormals().at(i);
		black_.at(i).color = Color(0.15f, 0.15f, 0.15f);
		grey_.at(i).color = Color(SlateGray.v);
	}
}

Geometry_pimpl::~Geometry_pimpl() {}

void Geometry_pimpl::DrawBlack(const string& fingerNumbers)
{
	const auto key(keyIsGrey_ ? &grey_ : &black_);

	UpdateFinger(key->data(), key->data() + 4, fingerNumbers);

	Batch<PrimitiveBatch<VertexPositionNormalColorTexture>> scene(batch_);
	batch_->DrawIndexed(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, mesh_->GetBlackIndices().data(),
		mesh_->GetBlackIndices().size(), key->data(), key->size());
}
void Geometry_pimpl::DrawWhiteLeft(const string& fingerNumbers)
{
	const auto key(keyIsGrey_ ? &greyLeft_ : &whiteLeft_);

	UpdateFinger(key->data() + 4, key->data(), fingerNumbers);

	Batch<PrimitiveBatch<VertexPositionNormalColorTexture>> scene(batch_);
	batch_->DrawIndexed(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, mesh_->GetWhiteIndices().data(),
		mesh_->GetWhiteIndices().size(), key->data(), key->size());
}
void Geometry_pimpl::DrawWhiteMiddle(const string& fingerNumbers)
{
	const auto key(keyIsGrey_ ? &greyMiddle_ : &whiteMiddle_);

	UpdateFinger(key->data() + 4, key->data(), fingerNumbers);

	Batch<PrimitiveBatch<VertexPositionNormalColorTexture>> scene(batch_);
	batch_->DrawIndexed(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, mesh_->GetWhiteIndices().data(),
		mesh_->GetWhiteIndices().size(), key->data(), key->size());
}
void Geometry_pimpl::DrawWhiteRight(const string& fingerNumbers)
{
	const auto key(keyIsGrey_ ? &greyRight_ : &whiteRight_);

	UpdateFinger(key->data() + 4, key->data(), fingerNumbers);

	Batch<PrimitiveBatch<VertexPositionNormalColorTexture>> scene(batch_);
	batch_->DrawIndexed(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, mesh_->GetWhiteIndices().data(),
		mesh_->GetWhiteIndices().size(), key->data(), key->size());
}

void Geometry_pimpl::UpdateFinger(VertexPositionNormalColorTexture* destination1,
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