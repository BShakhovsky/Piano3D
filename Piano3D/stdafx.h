#pragma once
#pragma warning(disable:4514 4625 4626 4710 5026 5027)
#pragma warning(push)
#pragma warning(disable:4061 4619 4265 4365 4548 4571 4668 4711 4820 4917 5029 5031)

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <WindowsX.h>

#include <wrl\client.h>
#include <DirectXColors.h>
#include <DirectXTK\SimpleMath.h>
#include <DirectXTK\CommonStates.h>
#include <DirectXTK\Effects.h>
#include <DirectXTK\SpriteFont.h>
#include <DirectXTK\WICTextureLoader.h>
#include <DirectXTK\VertexTypes.h>
#include <DirectXTK\PrimitiveBatch.h>
#include <DirectXTK\GeometricPrimitive.h>
#pragma comment(lib, "DirectXTK.lib")
#pragma comment(lib, "d3d11.lib")

#include <DirectXMesh\DirectXMesh.h>
#pragma comment(lib, "DirectXMesh.lib")

#include <boost\noncopyable.hpp>
#include <boost\format.hpp>

#pragma warning(pop)