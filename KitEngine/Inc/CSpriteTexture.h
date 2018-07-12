#pragma once

#include <Windows.h>
#include <assert.h>
#include <wrl.h>
#include <vector>
#include <memory>
#include <d3d11.h>
#include <DirectXMath.h>
#include <functional>

#include "../Library/KitLib/include/Kitlib.h"
#include "CRender.h"
#include "CVertexTypes.h"
#include "CModel.h"
#include "../Shaders/Header/Vertex/SpriteTexture.h"
#include "../Shaders/Header/Pixel/SpriteTexture.h"

namespace kit {
	namespace Engine {
		typedef struct TextureBox {
			kit::vec2 position;
			kit::vec2 size;
		};

		typedef struct VSMatrix2D {
			DirectX::XMVECTOR scale;
			DirectX::XMVECTOR rotationOrigin;
			float rotation;
			DirectX::XMVECTOR translation;

			VSMatrix2D(DirectX::XMVECTOR const& _scale, DirectX::XMVECTOR const& _rotationOrigin,
				float const& _rotation, DirectX::XMVECTOR const& _translation);
		};

		typedef struct PSMaterial {
			DirectX::XMFLOAT4 color;

			PSMaterial(DirectX::XMFLOAT4 const& _color)
				:color(_color){}
		};


		class SpriteTexture {
			void Render(wchar_t const& _path,TextureBox const& _box);
			void Render(wchar_t const& _path, vec2 const& _pos, vec2 const& _size);

		private:
			class Impl;

			std::unique_ptr<Impl> m_uptrImpl;

			std::unique_ptr<ID3D11ShaderResourceView> m_uptrSprite;
			TextureBox m_box;
		};
	}
}