#pragma once

#include <Windows.h>
#include <d3d11.h>

#include "../Library/KitLib/include/Kitlib.h"
#include "../Src/CWindow.h"
#include "../Src/CSceneManager.h"
#include "CAssetsManager.h"

#pragma comment (lib,"d3d11.lib")

#define DEFALUT_NUMERATOR (UINT)60
#define DEFALUT_DENOMINATOR (UINT)1

namespace kit {
	namespace Engine {

		class FrameRate {
		public:
			FrameRate();
			~FrameRate();
			FrameRate operator= (const FrameRate& _frameRate);
			inline void Numerator(const UINT& _num) {
				m_uiNumerator = _num;
			}
			inline UINT Numerator() const {
				return m_uiNumerator;
			}
			inline UINT Denominator() const {
				return m_uiDenominator;
			}
		private:
			UINT m_uiNumerator, m_uiDenominator;
		};

		typedef struct {
			LPSTR		m_lpstr;
			FrameRate	m_cFrameRate;
			vec2		m_vWindowSize;
		}CONFIG, *LPCONFIG;

		class Scene;
		class KitEngine :public kit::Singleton<KitEngine> {
		public:
			bool Create(HINSTANCE _hInstance, int _nCmdShow, LPCONFIG _config, std::shared_ptr<Scene> _startScene);
			void ChangeScene(std::shared_ptr<Scene> _changeScene);

			void Update();
			void Render();

		private:
			friend class Singleton<KitEngine>;
			KitEngine();
			// DirectX11で使用する変数
			HINSTANCE						m_hInst;
			HWND							m_hWnd;
			D3D_DRIVER_TYPE					m_driverType;
			D3D_FEATURE_LEVEL				m_featureLevel;
			ID3D11Device*					m_pd3dDevice;
			ID3D11RasterizerState*			m_pd3dRasterizerState;
			ID3D11DeviceContext*			m_pImmediateContext;
			IDXGISwapChain*					m_pSwapChain;
			ID3D11RenderTargetView*			m_pRenderTargetView;
			ID3D11Texture2D*				m_pDepthStencil;
			ID3D11DepthStencilView*			m_pDepthStencilView;

			// Engineに使用する変数及びクラス
			CONFIG							m_cConfig;
			std::unique_ptr<CWindow>		m_uptrWindow;
			std::unique_ptr<SceneManager>	m_uptrSceneManager;

			HRESULT InitDevice();
			HRESULT HCreateWindow(HINSTANCE _hInstance, int _nCmdShow);
		};
	}
}

