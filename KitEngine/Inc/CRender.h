#pragma once

#include <stdio.h>
#include <iostream>
#include <assert.h>
#include <wrl.h>
#include <vector>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#pragma comment(lib,"d3dcompiler.lib")

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
		
		template<typename Ty>
		std::vector<Ty> LeadShaderFile(wchar_t const& _path) {
			std::vector<Ty> byteVector;
			BYTE byte = 0;

			FILE *fp = fopen(_path, "r");
			if (NULL == fp) { return false; }

			char buf[256];
			int n = 0;

			do {
				if (n > NULL) {
					static int array_ = 0;
					if (0 == strcmp(buf, "const")) {

						do {
							fscanf(fp, "%s", &buf);
						} while (0 != strcmp(buf, "{"));

						for (int f = 0, f = != strcmp(buf, "}"); f = fscanf(fp, "%s", &buf)) {
							fscanf(fp, "%d", &byte);
							byteVector.push_back(byte);
						}
					}
				}
				n = fscanf(fp, "%s", buf);
			} while (n > NULL);

			rewind(fp);
			fclose(fp);

			return byteVector;
		}


		template<typename Vertex>
		class Shaders {
		public:
			bool Create(ID3D11Device* _pd3dDevice, wchar_t const* _vsPath,wchar_t const* _psPath,bool _isCompiled) {
				if (false == _isCompiled) {
					HRESULT hr = S_OK;

					DWORD flags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
					flags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
					// Compile the vertex shader
					Microsoft::WRL::ComPtr<ID3DBlob> vsBlob;

					hr = D3DCompileFromFile(_vsPath, nullptr, nullptr, "vs_main", "vs_5_0", flags, 0, vsBlob.GetAddressOf());
					if (FAILED(hr)) { return false; }

					hr = _pd3dDevice->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(),
						nullptr, m_cptrVertexShader.GetAddressOf());
					if (FAILED(hr)) { return false; }

					hr = _pd3dDevice->CreateInputLayout(Vertex::VertexDesc, Vertex::InputElementCount,
						vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(),
						m_cptrInputLayout.GetAddressOf());
					if (FAILED(hr)) { return false; }

					// Conpile the pixel shader
					Microsoft::WRL::ComPtr<ID3DBlob> psBlob;
					hr = D3DCompileFromFile(_psPath, nullptr, nullptr, "ps_main", "ps_5_0", flags, 0, psBlob);
					if (FAILED(hr)) { return false; }

					hr = _pd3dDevice->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(),
						nullptr, m_cptrPixelShader.GetAddressOf());
					if (FAILED(hr)) { return false; }

					return true;
				}
				else if (true == _isCompiled) {
					HRESULT hr = S_OK;

					BYTE* vs_main = LeadShaderFile<BYTE>(_vsPath).data();
					
					size_t array_size = ARRAYSIZE(vs_main);

					hr = _pd3dDevice->CreateVertexShader(&vs_main, &array_size, NULL, m_cptrVertexShader.GetAddressOf());
					if (FAILED(hr)) { return false; }


					hr = _pd3dDevice->CreateInputLayout(Vertex::VertexDesc, Vertex::InputElementCount,
						&vs_main, array_size,
						m_cptrInputLayout.GetAddressOf());
					if (FAILED(hr)) { return false; }


					BYTE* ps_main = LeadShaderFile<BYTE>(_psPath).data();

					array_size = ARRAYSIZE(ps_main);

					hr = _pd3dDevice->CreatePixelShader(&ps_main, &array_size, NULL, m_cptrPixelShader.GetAddressOf());
					if (FAILED(hr)) { return false; }

					return true;
					}
			}
			void Apply(ID3D11DeviceContext* _pd3dImmediateContext) {
				_pd3dImmediateContext->IASetInputLayout(m_cptrInputLayout.Get());
				_pd3dImmediateContext->VSSetShader(m_cptrVertexShader.Get(), nullptr, 0);
				_pd3dImmediateContext->PSSetShader(m_cptrPixelShader.Get(), nullptr, 0);
			}
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

		template<typename Ty>
		class Buffers {
		public:
			HRESULT Create(ID3D11Device* _pd3dDevice) {
				D3D11_BUFFER_DESC bufferDesc;
				ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));
				bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
				bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
				bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
				bufferDesc.ByteWidth = sizeof(Ty);

				return _pd3dDevice->CreateBuffer(&bufferDesc, nullptr, m_cptrBuffer.GetAddressOf());
			}

			template<typename...Args>
			void Set(ID3D11DeviceContext* _pd3dImmidiateContext, Args&&... _args) {
				D3D11_MAPPED_SUBRESOURCE mappedResource;
				if (SUCCEEDED(_pd3dImmidiateContext->Map(m_cptrBuffer.Get(), 0
					D3D11_MAP_WRITE_DISCARD, 0, &mappedResource))) {
					new(mappedResource.pData)Ty(std::forward<Args>(_args)...);
					_pd3dImmidiateContext->Unmap(m_cptrBuffer.Get(), 0);
				}
			}

			void PsSet(ID3D11DeviceContext* _pd3dImmidiateContext, int _index) {
				_pd3dImmidiateContext->PSSetConstantBuffers(_index, 1, m_cptrBuffer.GetAddressOf());
			}

			void VsSet(ID3D11DeviceContext* _pd3dImmidiateContext, int _index) {
				_pd3dImmidiateContext->VSSetConstantBuffers(_index, 1, m_cptrBuffer.GetAddressOf());
			}

			void Destroy() {
				m_cptrBuffer.Reset();
			}

		private:
			Microsoft::WRL::ComPtr<ID3D11Buffer> m_cptrBuffer;
		};
	}
}