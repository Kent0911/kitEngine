#pragma once

#include <d3d11.h>
#include <wrl.h>

template<typename Ty>
class ConstantBuffer {
public:
	HRESULT Create(ID3D11Device* _pd3dDevice) {
		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufferDesc.ByteWidth = sizeof(Ty);

		return _pd3dDevice->CreateBuffer(&bufferDesc, nullptr, mcptr_buffer.GetAddressOf());
	}

	template<typename...Args>
	void Set(ID3D11DeviceContext* _pd3dImmidiateContext, Args&&... _args) {
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		if (SUCCEEDED(_pd3dImmidiateContext->Map(mcptr_buffer.Get(), 0,
			D3D11_MAP_WRITE_DISCARD, 0, &mappedResource))) {
			new(mappedResource.pData)Ty(std::forward<Args>(_args)...);
			_pd3dImmidiateContext->Unmap(mcptr_buffer.Get(), 0);
		}
	}

	void PsSet(ID3D11DeviceContext* _pd3dImmidiateContext, int _index) {
		_pd3dImmidiateContext->PSSetConstantBuffers(_index, 1, mcptr_buffer.GetAddressOf());

	}

	void VsSet(ID3D11DeviceContext* _pd3dImmidiateContext, int _index) {
		_pd3dImmidiateContext->VSSetConstantBuffers(_index, 1, mcptr_buffer.GetAddressOf());

	}
	void Destroy() {
		mcptr_buffer.Reset();
	}

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> mcptr_buffer;
};
