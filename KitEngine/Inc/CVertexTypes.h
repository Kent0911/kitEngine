#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d11.h>
#include <DirectXMath.h>

namespace kit {
	namespace Engine {
		enum class InputElements {
			Pos,
			PosCol,
			PosTex,
			PosNorTex,
			PosNorCol,
			PosNorColTex
		};

		typedef struct VertexPos {
			DirectX::XMFLOAT3 position;
			static const int InputElementCount = 1;
			static const D3D11_INPUT_ELEMENT_DESC VertexDesc[InputElementCount];
		}*LPVertexPos;

		typedef struct VertexPosCol {
			DirectX::XMFLOAT3 position;
			DirectX::XMFLOAT4 color;
			static const int InputElementCount = 2;
			static const D3D11_INPUT_ELEMENT_DESC VertexDesc[InputElementCount];
		}*LPVertexPosCol;

		typedef struct VertexPosTex {
			DirectX::XMFLOAT3 position;
			DirectX::XMFLOAT2 texture;
			static const int InputElementCount = 2;
			static const D3D11_INPUT_ELEMENT_DESC VertexDesc[InputElementCount];
		}*LPVertexPosTex;

		typedef struct VertexPosNorTex {
			DirectX::XMFLOAT3 position;
			DirectX::XMFLOAT3 normal;
			DirectX::XMFLOAT2 texture;
			static const int InputElementCount = 3;
			static const D3D11_INPUT_ELEMENT_DESC VertexDesc[InputElementCount];
		}*LPVertexPosNorTex;

		typedef struct VertexPosNorCol {
			DirectX::XMFLOAT3 position;
			DirectX::XMFLOAT3 normal;
			DirectX::XMFLOAT4 color;
			static const int InputElementCount = 3;
			static const D3D11_INPUT_ELEMENT_DESC VertexDesc[InputElementCount];
		}*LPVertexPosNorCol;

		typedef struct VertexPosNorColTex {
			DirectX::XMFLOAT3 position;
			DirectX::XMFLOAT3 normal;
			DirectX::XMFLOAT4 color;
			DirectX::XMFLOAT2 texture;
			static const int InputElementCount = 4;
			static const D3D11_INPUT_ELEMENT_DESC VertexDesc[InputElementCount];
		}*LPVertexPosNorColTex;
	}
}