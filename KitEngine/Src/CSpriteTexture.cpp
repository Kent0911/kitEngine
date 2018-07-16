#include "stdafx.h"

#include "../Inc/CSpriteTexture.h"
#include "AlignedNew.h"

using namespace kit;
using namespace kit::Engine;

VSMatrix2D::VSMatrix2D(DirectX::XMVECTOR const& _scale, DirectX::XMVECTOR const& _rotationOrigin, float const& _rotation, DirectX::XMVECTOR const& _translation)
	:scale(_scale), rotationOrigin(_rotationOrigin), rotation(_rotation), translation(_translation) {

}


inline Microsoft::WRL::ComPtr<ID3D11Device> GetDevice(_In_ ID3D11DeviceContext* _deviceContext) {
	Microsoft::WRL::ComPtr<ID3D11Device> device;

	_deviceContext->GetDevice(&device);

	return device;
}

inline DirectX::XMVECTOR LoadRect(_In_ RECT const* _rect) {
	DirectX::XMVECTOR v = DirectX::XMLoadInt4(reinterpret_cast<uint32_t const*>(_rect));

	v = DirectX::XMConvertVectorIntToFloat(v, 0);

	v = DirectX::XMVectorSubtract(v, DirectX::XMVectorPermute<0, 1, 4, 5>(DirectX::g_XMZero, v));

	return v;
}

__declspec(align(16)) class SpriteTexture::Impl:public AlignedNew<SpriteTexture::Impl> {
public:
	Impl(_In_ ID3D11DeviceContext* _pd3dImmidiateContext, ID3D11Device* _pd3dDevice);

	void XM_CALLCONV Begin(
		_In_opt_ ID3D11BlendState* _blendState,
		_In_opt_ ID3D11SamplerState* _samplarState,
		_In_opt_ ID3D11DepthStencilState* _depthStencilState,
		_In_opt_ ID3D11RasterizerState* _rasterizerState,
		std::function<void()>& _setCustomShaders,
		DirectX::XMMATRIX _transform);
	
	void End();

	void Render(
		_In_ ID3D11ShaderResourceView* _texture,
		DirectX::XMVECTOR _destination,
		_In_opt_ RECT const* _sourceRectangle,
		DirectX::FXMVECTOR _color,
		DirectX::FXMVECTOR _originRotationDepth,
		int _flags);

	__declspec(align(16)) struct SpriteInfo :public AlignedNew <SpriteInfo> {
		DirectX::XMFLOAT4A source;
		DirectX::XMFLOAT4A destination;
		DirectX::XMFLOAT4A color;
		DirectX::XMFLOAT4A originRotationDepth;
		ID3D11ShaderResourceView* texture;
		int flags;

		static const int SourceInTexels = 4;
		static const int DesSizeInPixels = 8;
	};

	bool m_bSetViewport;
	D3D11_VIEWPORT m_d3dViewport;

private:
	// Constants
	static const size_t MaxTextureSize = 2048;
	static const size_t MinTextureSize = 128;
	static const size_t IntitalQueueSize = 64;
	static const size_t VerticesPerSprite = 4;
	static const size_t IndicesPerSprite = 6;

	std::unique_ptr<SpriteInfo[]> m_uptrSpriteQueue;

	size_t m_sizeSpriteQueueCount;
	size_t m_sizeSpriteQueueArraySize;

	void PrepareForRendering(ID3D11DeviceContext* _pd3dDeviceContext);
	void RenderTextrue(_In_ ID3D11ShaderResourceView* _texuture, _In_reads_(const) SpriteInfo const* const* _sprites, size_t count);

	static DirectX::XMVECTOR GetTextureSize(_In_ ID3D11ShaderResourceView* _texture);

	static void XM_CALLCONV RenderSprite(_In_ SpriteInfo const* _sprite,
		_Out_writes_(VerticesPerSprite)VertexPosColTex* vertices,
		DirectX::XMVECTOR _textureSize, DirectX::XMVECTOR _inverseTextureSize);

	bool m_bInBeginEndPair;
	Microsoft::WRL::ComPtr<ID3D11BlendState>		m_cptrBlendState;
	Microsoft::WRL::ComPtr<ID3D11SamplerState>		m_cptrSamplerState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_cptrDepthStencilState;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	m_cptrRasterizerState;
	std::function<void()>							m_funcSetCustomShaders;
	DirectX::XMMATRIX								m_mTransform;



	struct DeviceResources {
		DeviceResources(_In_ ID3D11Device* _pd3dDevice);

		inline DeviceResources GetInstance() const {
			return *this;
		}
	private:
		Shaders<VertexPosColTex> m_shaders;
		Buffers<VSMatrix2D> m_vsBuffers;
		Buffers<PSMaterial> m_psBuffers;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_cptrIndexBuffer;

		static std::vector<short> CreateIndexValues();
		bool CreateIndexBuffer(_In_ ID3D11Device* _pd3dDevice);
	};

	std::shared_ptr<DeviceResources> m_sptrDeviceResources;
};

SpriteTexture::Impl::DeviceResources::DeviceResources(_In_ ID3D11Device* _pd3dDevice) {
	m_shaders.Create(_pd3dDevice, L"../Shaders/Header/Vertex/SpriteTexture.h", L"../Shaders/Header/Pixel/SpriteTexture.h", true);
	m_vsBuffers.Create(_pd3dDevice);
	m_psBuffers.Create(_pd3dDevice);
}

bool SpriteTexture::Impl::DeviceResources::CreateIndexBuffer(_In_ ID3D11Device* _pd3dDevice) {
	HRESULT hr = S_OK;

	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(D3D11_BUFFER_DESC));

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.ByteWidth = sizeof(short) * MaxTextureSize * IndicesPerSprite;

	auto indexValues = CreateIndexValues();

	D3D11_SUBRESOURCE_DATA indexDataDesc;
	indexDataDesc.pSysMem = indexValues.data();

	hr = _pd3dDevice->CreateBuffer(&indexBufferDesc, &indexDataDesc, m_cptrIndexBuffer.GetAddressOf());
	if (FAILED(hr)) { return false; }

	return true;
}

std::vector<short> SpriteTexture::Impl::DeviceResources::CreateIndexValues() {
	std::vector<short> indices;

	indices.reserve(MaxTextureSize * IndicesPerSprite);

	for (size_t t = 0; t < MaxTextureSize * VerticesPerSprite; t += VerticesPerSprite) {
		short s = static_cast<short>(t);

		indices.push_back(s);
		indices.push_back(s + 1);
		indices.push_back(s + 2);

		indices.push_back(s + 1);
		indices.push_back(s + 3);
		indices.push_back(s + 2);
	}

	return indices;
}

SpriteTexture::Impl::Impl(_In_ ID3D11DeviceContext* _pd3dImmidiateContext, ID3D11Device* _pd3dDevice)
	: m_bInBeginEndPair(false),
	  m_sizeSpriteQueueCount(0),
 	  m_sizeSpriteQueueArraySize(0),
	  m_sptrDeviceResources(std::make_shared<SpriteTexture::Impl::DeviceResources>(_pd3dDevice)){	}

void SpriteTexture::Impl::Begin(
	ID3D11BlendState* _blendState,
	ID3D11SamplerState* _samplarState,
	ID3D11DepthStencilState* _depthStencilState,
	ID3D11RasterizerState* _rasterizerState,
	std::function<void()>& _setCustomShaders,
	DirectX::XMMATRIX _transform) {
	
	if (m_bInBeginEndPair) { throw std::exception("Cannot nest Begin calls on a single Sprites"); }

	m_cptrBlendState = _blendState;
	m_cptrSamplerState = _samplarState;
	m_cptrDepthStencilState = _depthStencilState;
	m_cptrRasterizerState = _rasterizerState;
	m_funcSetCustomShaders = _setCustomShaders;
	m_mTransform = _transform;

	m_bInBeginEndPair = true;
}

void SpriteTexture::Impl::End() {
	if (m_bInBeginEndPair) { throw std::exception("Begin must be called before End"); }
	
	m_funcSetCustomShaders = nullptr;
	m_bInBeginEndPair = false;
}

_Use_decl_annotations_
void XM_CALLCONV SpriteTexture::Impl::Render(ID3D11ShaderResourceView* _texture,
	DirectX::XMVECTOR _destination,
	RECT const* _sourceRectangle,
	DirectX::FXMVECTOR _color,
	DirectX::FXMVECTOR _originRotationDepth,
	int _flags) {
	if (!_texture) { throw std::exception("Texture cannot be null"); }

	if (!m_bInBeginEndPair) { throw std::exception("Begin must be called before Render"); }

	SpriteInfo* sprite = &m_uptrSpriteQueue[m_sizeSpriteQueueCount];

	DirectX::XMVECTOR dest = _destination;

	if (_sourceRectangle) {
		DirectX::XMVECTOR source = LoadRect(_sourceRectangle);

		DirectX::XMStoreFloat4A(&sprite->source, source);

		if (!(_flags & SpriteInfo::DesSizeInPixels)) {
			dest = DirectX::XMVectorPermute<0, 1, 6, 7>(dest, DirectX::XMVectorMultiply(dest, source));
		}

		_flags |= SpriteInfo::SourceInTexels | SpriteInfo::DesSizeInPixels;
	}
	else {
		static const DirectX::XMVECTORF32 wholeTexture = { {{0,0,1,1}} };

		DirectX::XMStoreFloat4A(&sprite->source, wholeTexture);
	}

	DirectX::XMStoreFloat4A(&sprite->destination, dest);
	DirectX::XMStoreFloat4A(&sprite->color, _color);
	DirectX::XMStoreFloat4A(&sprite->originRotationDepth, _originRotationDepth);

	sprite->texture = _texture;
	sprite->flags = _flags;
}

void SpriteTexture::Impl::PrepareForRendering(ID3D11DeviceContext* _pd3dDeviceContext) {
	auto diviceContext = _pd3dDeviceContext;
	auto blendState = m_cptrBlendState.Get();
}

DirectX::XMVECTOR SpriteTexture::Impl::GetTextureSize(_In_ ID3D11ShaderResourceView* _texture) {
	Microsoft::WRL::ComPtr<ID3D11Resource> resource;

	_texture->GetResource(&resource);

	// Cast to texture
	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture2D;

	if (FAILED(resource.As(&texture2D))) { throw std::exception("SpriteTexture can only draw texture2D resources"); }

	D3D11_TEXTURE2D_DESC texture2DDesc;

	texture2D->GetDesc(&texture2DDesc);

	// Comvert to vector format
	DirectX::XMVECTOR size = DirectX::XMVectorMergeXY(DirectX::XMLoadInt(&texture2DDesc.Width),
													  DirectX::XMLoadInt(&texture2DDesc.Height));

	return DirectX::XMConvertVectorIntToFloat(size, 0);
}