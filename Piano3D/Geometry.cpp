#include "stdafx.h"
#include "Geometry.h"
#include "Geometry_pimpl.h"
#include "Mesh.h"

using namespace std;
using namespace DirectX;
using namespace SimpleMath;

Geometry::Geometry(ID3D11DeviceContext* context)
	: pimpl_(make_unique<Geometry_pimpl>(context)),
	pianoDesk_(GeometricPrimitive::CreateBox(context, Vector3(deskLength, deskHeight, deskThickness)))
{}
Geometry::~Geometry() {}

float Geometry::GetKeyboardWidth()
{
	return Mesh::WHITE_LENGTH;
}

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
Matrix Geometry::GetRotatedMatrix(const int note, const float angle) const
{
	return move(Matrix::CreateRotationX(angle) * GetWorldMatrix(note));
}

void Geometry::DrawKey(const int note, const bool highlighted, const char* fingerNumbers)
{
	pimpl_->KeyIsGrey(highlighted);

	if		(note == 0)		pimpl_->DrawWhiteLeft	(fingerNumbers);
	else if (note == 1)		pimpl_->DrawBlack		(fingerNumbers);
	else if (note == 2)		pimpl_->DrawWhiteRight	(fingerNumbers);
	else if (note == 87)
	{
							pimpl_->DrawWhiteLeft	(fingerNumbers);
							pimpl_->DrawWhiteRight	(fingerNumbers);
	}
	else switch ((note - 3) % 12)
	{
	case 0:					pimpl_->DrawWhiteLeft	(fingerNumbers); break;
	case 1:					pimpl_->DrawBlack		(fingerNumbers); break;
	case 2:					pimpl_->DrawWhiteMiddle	(fingerNumbers); break;
	case 3:					pimpl_->DrawBlack		(fingerNumbers); break;
	case 4:					pimpl_->DrawWhiteRight	(fingerNumbers); break;
	case 5:					pimpl_->DrawWhiteLeft	(fingerNumbers); break;
	case 6:					pimpl_->DrawBlack		(fingerNumbers); break;
	case 7:					pimpl_->DrawWhiteMiddle	(fingerNumbers); break;
	case 8:					pimpl_->DrawBlack		(fingerNumbers); break;

	case 9:					pimpl_->DrawWhiteMiddle	(fingerNumbers); break;
	case 10:				pimpl_->DrawBlack		(fingerNumbers); break;
	case 11:				pimpl_->DrawWhiteRight	(fingerNumbers); break;
	default: assert(!"Wrong note");
	}
}