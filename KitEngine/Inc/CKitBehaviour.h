#pragma once

#define _USE_MATH_DEFINES

#include <math.h>

#include "CAssetsManager.h"
#include "CRender.h"

namespace kit {
	namespace Engine {

		class StandardState {
		public:
			inline void Position( vec3 _position ) {
				mvec_position = _position;
			}
			inline vec3 Position() const {
				return mvec_position;
			}
			inline void AddYaw( axis _yaw ) {
				m_yaw += _yaw;
			}
			inline axis Yaw() const {
				return m_yaw;
			}
			inline void CulYaw() {
				m_rad = m_yaw * M_PI / 180;
			}
			inline axis Rad() const {
				m_rad;
			}
			inline DirectX::XMMATRIX WorldMatirx() const {
				return m_worldMatrix;
			}
			inline DirectX::XMMATRIX MultiplicationWorldMatrix(DirectX::XMMATRIX _multi) {
				m_worldMatrix = _multi;
			}
		private:
			vec3 mvec_position;
			axis m_yaw;
			axis m_rad;
			DirectX::XMMATRIX m_worldMatrix;
		};

		class KitBehaviour {
		protected:
			StandardState m_state;
		public:
			KitBehaviour();
			~KitBehaviour();

			inline StandardState GetStandardState() {
				return m_state;
			}

			virtual void Update() = 0;
			virtual void Render() = 0;
		};
	}
}