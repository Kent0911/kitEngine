#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

#include "../Library/KitLib/include/Kitlib.h"

class CameraState {
public:
	inline void SetEye(DirectX::XMFLOAT4 _eye) {
		eye = DirectX::XMVectorSet(_eye.x, _eye.y, _eye.w, _eye.z);
	}
	inline void SetTarget(DirectX::XMFLOAT4 _target) {
		target = DirectX::XMVectorSet(_target.x, _target.y, _target.w, _target.z);
	}
	inline void SetUpperVec(DirectX::XMFLOAT4 _upperVec) {
		upperVec = DirectX::XMVectorSet(_upperVec.x, _upperVec.y, _upperVec.w, _upperVec.z);
	}
	inline DirectX::XMMATRIX GetMatrix() const {
		return DirectX::XMMatrixLookAtLH(eye, target, upperVec);
	}

private:
	DirectX::XMVECTOR eye;
	DirectX::XMVECTOR target;
	DirectX::XMVECTOR upperVec;
};

class Camera {
public:
	inline void SetInstance(const CameraState _state) {
		m_cameraState = _state;
	}
	inline CameraState GetInstance() {
		return m_cameraState;
	}
private:
	CameraState m_cameraState;
};