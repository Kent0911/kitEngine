#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

namespace kit {
	namespace Engine {
		enum class Materials {
			Col,
		};

		struct MaterialCol {
			DirectX::XMFLOAT4 color;
		};
	}
}