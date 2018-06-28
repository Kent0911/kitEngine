#pragma once

#include "CAssetsManager.h"

namespace kit {
	namespace Engine {
		class KitBehaviour {
		protected:
			vec3 mvec_position;
			axis m_axis;

		public:
			KitBehaviour();
			~KitBehaviour();

			virtual void Update() = 0;
			virtual void Render() = 0;
		};
	}
}