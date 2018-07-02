#pragma once

#include <stdio.h>
#include <iostream>
#include <assert.h>
#include <wrl.h>
#include <d3d11.h>
#include <DirectXMath.h>

namespace kit {
	namespace Engine {
		class Matrix {
		public:
			Matrix(Matrix const& _matrix);
			Matrix operator= (Matrix& _matrix);
			inline Matrix GetMatrix() const {
				return *this;
			}
		private:
			DirectX::XMFLOAT4X4 m_mWorld;
			DirectX::XMFLOAT4X4 m_mView;
			DirectX::XMFLOAT4X4 m_mProjection;
		};
		
		class Matrix2D {
		public:
			Matrix2D(Matrix2D const& _matrix);
			Matrix2D operator= (Matrix2D& _matrix);
			inline Matrix2D GetMatirx2D() const {
				return *this;
			}
		private:
			DirectX::XMVECTOR m_xScale;
			DirectX::XMVECTOR m_xRotationOrigin;
			float m_fRotation;
			DirectX::XMVECTOR m_xTranslation;
		};
		

		class Shaders {
		public:
			bool Create(ID3D11Device* _pd3dDevice);
			void Apply(ID3D11DeviceContext* _pd3dImmediateContext);
		private:
			Microsoft::WRL::ComPtr<ID3D11VertexShader> m_cptrVertexShader;
			Microsoft::WRL::ComPtr<ID3D11PixelShader> m_cptrPixelShader;
			Microsoft::WRL::ComPtr<ID3D11InputLayout> m_cptrInputLayout;
		};

		enum class Buffer {
			Vertex,
			Index,
			Constant
		};

		class Buffers {
		public:
			bool Create(ID3D11Device* _pd3dDevice);
			void Apply(ID3D11DeviceContext* _pd3dImmediateContext);

		private:
			Microsoft::WRL::ComPtr<ID3D11Buffer> m_cptrVertexBuffer;
			Microsoft::WRL::ComPtr<ID3D11Buffer> m_cptrIndexBuffer;
			Microsoft::WRL::ComPtr<ID3D11Buffer> m_cptrConstantBuffer;
		};

	}
}