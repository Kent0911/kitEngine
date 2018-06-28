#include "stdafx.h"

#include "CWindow.h"

using namespace kit::Engine;

LRESULT CWindow::WndProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam) {
	PAINTSTRUCT ps;
	HDC hdc;

	switch (_message) {
	case WM_PAINT:
		hdc = BeginPaint(_hWnd, &ps);
		EndPaint(_hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_INPUT:
	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MOUSEWHEEL:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_MOUSEHOVER:
		DirectX::Mouse::ProcessMessage(_message, _wParam, _lParam);
		break;

	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
		DirectX::Keyboard::ProcessMessage(_message, _wParam, _lParam);

	default:
		return DefWindowProc(_hWnd, _message, _wParam, _lParam);
	}

	return 0;
}

HRESULT CWindow::InitWindow(HINSTANCE _hInstance, int _nCmdShow) {
	m_wcex.cbSize = sizeof(WNDCLASSEX);
	m_wcex.style = CS_HREDRAW | CS_VREDRAW;
	m_wcex.lpfnWndProc = WndProc;
	m_wcex.cbClsExtra = 0;
	m_wcex.cbWndExtra = 0;
	m_wcex.hInstance = _hInstance;
	m_wcex.hIcon = LoadIcon(_hInstance, (LPCTSTR)IDI_SAMPLE1);
	m_wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	m_wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	m_wcex.lpszMenuName = nullptr;
	m_wcex.lpszClassName = "WindowClass";
	m_wcex.hIconSm = LoadIcon(m_wcex.hInstance, (LPCTSTR)IDI_SAMPLE1);

	if (!RegisterClassEx(&m_wcex)) { return E_FAIL; }
}

CWindow::CWindow(HINSTANCE _hInstance, int _nCmdShow) {
	InitWindow(_hInstance, _nCmdShow);
	m_recWindowSize = { 0,0,640,480 };
}

CWindow::CWindow(HINSTANCE _hInstace, int _nCmdShow, RECT _createWindowSize) {
	InitWindow(_hInstace, _nCmdShow);
	m_recWindowSize = _createWindowSize;
}

CWindow::CWindow(HINSTANCE _hInstance, int _nCmdShow, kit::vec2 _createWindowSize) {
	InitWindow(_hInstance, _nCmdShow);
	m_recWindowSize.left = 0;
	m_recWindowSize.top = 0;
	m_recWindowSize.right = _createWindowSize.x;
	m_recWindowSize.bottom = _createWindowSize.y;
}

CWindow::~CWindow() { }