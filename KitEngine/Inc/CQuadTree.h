#pragma once

#define USE_MATH_DEFINES

#include <math.h>
#include <vector>
#include <list>

#include "../Library/KitLib/include/Kitlib.h"

namespace kit {
	namespace Engine {

		class HitChackObject {
		public:
			HitChackObject(float _radius);
			~HitChackObject();

			inline Circle2D GetCircle() const {
				return m_circle;
			}
			inline bool Hit() const {
				return m_hit;
			}
			inline void Hit(bool _flag) {
				m_hit = _flag;
			}
		private:
			Circle2D		m_circle;
			bool			m_hit;
		};

		class Area {
		public:
			Area(int _mortonNumber, Quadrangle2D _quad);
			~Area();

			inline int MortonNumber() const {
				return m_mortonNumber;
			}
			inline std::list<HitChackObject*> GetListInstance() {
				return m_areaObjects;
			}
		private:
			Quadrangle2D				m_size;
			int							m_mortonNumber;
			std::list<HitChackObject*>	m_areaObjects;
		};

		class QuadTree :public Singleton< QuadTree > {
		public:
			bool Create(int _splitLevel, vec2 _mapSize);
			void QuadTreeHitCheck(int _deivdeLevel, int _mortonNumber);

		protected:
			friend class Singleton< QuadTree >;
			QuadTree();
			
			Area* AreaCreate(int _x, int _z, vec2 _mapSize);
			bool ClearObjectList();

			inline DWORD BitSeparate32(DWORD _value) {
				_value = (_value | (_value << 8)) & 0x00ff00ff;
				_value = (_value | (_value << 4)) & 0x0f0f0f0f;
				_value = (_value | (_value << 2)) & 0x33333333;
				_value = (_value | (_value << 1)) & 0x55555555;

				return _value;
			}

			inline DWORD Get2DMortonNumber(DWORD _x, DWORD _z) {
				return (BitSeparate32(_x) | (BitSeparate32(_z) << 1));
			}

			vec2							m_mapSize;
			unsigned int					m_splitLevel;
			std::vector<std::vector<Area>>	m_splitAreas;
			std::list<Area*>				m_inAreaList;
		};

	}
}