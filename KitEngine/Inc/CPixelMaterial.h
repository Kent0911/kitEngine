#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

namespace kit {
	namespace Engine {
		enum class Materials {
			Col,
		};

		union MaterialCol {
			DirectX::XMFLOAT4 color;
			float color[4];
		};
	}
}