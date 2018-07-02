#pragma once

#include <d3d11.h>
#include <wrl.h>
#include <vector>

template<typename Vertex>
class Model {
public:
	template<class IndexTypes>
	bool Create(ID3D11Device* _pd3dDevice, std::vector<Vertex> const& _vertices, std::vector<IndexTypes> const& _indices) {
		HRESULT hr = S_OK;
		
		m_uIndexBuffer = static_cast<unsigned>(_indices.size());

		// Create vertex buffer
		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.ByteWidth = sizeof(Vertex) * static_cast<unsigned>(_vertices.size());
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = _vertices.data();

		hr = _pd3dDevice->CreateBuffer(&bufferDesc, &initData, m_cptrVertexBuffer.GetAddressOf());
		if (FAILED(hr)) { return false; }

		// Create index buffer
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bufferDesc.ByteWidth = sizeof(IndexTypes) * static_cast<unsigned>(_indices.size());
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0;

		initData.pSysMem = _indices.data();

		hr = _pd3dDevice->CreateBuffer(&bufferDesc, &initData, m_cptrIndexBuffer.GetAddressOf());
		if (FAILED(hr)) { return false; }

		return true;
	}

	void Render(ID3D11DeviceContext* _pd3dImmidiateContext) {
		// Set index buffer
		_pd3dImmidiateContext->IASetIndexBuffer(m_cptrIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

		// Set primitive topology
		_pd3dImmidiateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// Set vertex buffer
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		_pd3dImmidiateContext->IASetVertexBuffers(0, 1, m_cptrVertexBuffer.GetAddressOf(), &stride, &offset);

		// Render the meth
		_pd3dImmidiateContext->DrawIndexed(m_uIndexBuffer, 0, 0);
	}

	void Destroy() {
		m_cptrVertexBuffer.Reset();
		m_cptrIndexBuffer.Reset();
	}
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_cptrVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_cptrIndexBuffer;
	unsigned m_uIndexBuffer;
};