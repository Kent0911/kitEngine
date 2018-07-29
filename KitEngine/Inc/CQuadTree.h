#pragma once

#define USE_MATH_DEFINES

#include <math.h>
#include <vector>
#include <list>

#include "../Library/KitLib/include/Kitlib.h"

namespace kit {
	namespace Engine {

		template<typename Collision>
		class HitChackObject2D {
		public:
			HitChackObject2D(Collision _collision)
				: m_collision(_collision) {}
			~HitChackObject2D() {}

			inline Collision GetCollision() const {
				return m_collision;
			}
			inline bool Hit() const {
				return m_isHit;
			}
			inline void Hit(bool _flag) {
				m_isHit = _flag;
			}
		private:
			Collision		m_collision;
			bool			m_isHit;
		};

		template<typename Collision>
		class HitCheckObject {
		public:
			HitChackObject(Collision _collision)
				: m_collision(_collision) {}
			~HitChackObject() {}

			inline Collision GetCollision() const {
				return m_collision;
			}
			inline bool Hit() const {
				return m_isHit;
			}
			inline void Hit(bool _flag) {
				m_isHit = _flag;
			}
		private:
			Collision		m_collision;
			bool			m_isHit;
		};

		template<typename HitChackObject>
		class Area {
		public:
			Area(int _mortonNumber, Quadrangle2D _quad)
				: m_mortonNumber(_mortonNumber), m_size(_quad) {}
			~Area() {
				m_areaObjects.clear();
			}

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

		template<typename HitCheckObject>
		class QuadTree :public Singleton< QuadTree > {
		public:
			bool Create(int _splitLevel, vec2 _mapSize) {
				m_splitLevel = _splitLevel;

				// 分割数が偶数でなければ分割数を+1する
				if (!(0 == (m_splitLevel % 2))) {
					m_splitLevel++;
				}

				// Z軸の分割数分だけvector<Area>を追加
				for (int z = 0; z <= m_splitLevel; ++z) {
					std::vector<Area<HitCheckObject>> area_x;

					// X軸の分割数分だけAreaを追加
					for (int x = 0; x <= m_splitLevel; ++x) {
						area_x.push_back(*AreaCreate(x, z, m_mapSize));
					}

					m_splitAreas.push_back(area_x);
				}

				return true;
			}


			void QuadTreeHitCheck(int _divideLevel, int _mortonNumber) {
				// Area内の衝突判定
				if (m_splitAreas[_divideLevel][_mortonNumber].GetListInstance().size() > 1) {
					std::list<HitCheckObject*>::iterator thisIter = m_splitAreas[_divideLevel][_mortonNumber].GetListInstance().begin();
					std::list<HitCheckObject*>::iterator thisIterEnd = m_splitAreas[_divideLevel][_mortonNumber].GetListInstance().end();
					while (thisIter != thisIterEnd) {
						std::list<HitCheckObject*>::iterator anotherIter = thisIter;
						anotherIter++;
						while (anotherIter != thisIterEnd) {
							if (HitCheck((*thisIter)->GetCollision(), (*anotherIter)->GetCollision())) {
								(*thisIter)->Hit(true);
								(*anotherIter)->Hit(true);
							}
							anotherIter++;
						}
						thisIter++;
					}
				}

				// Areaリストとの衝突判定
				Area* area = nullptr;
				if (m_splitAreas[_divideLevel][_mortonNumber].GetListInstance().size() > 0) {
					area = &m_splitAreas[_divideLevel][_mortonNumber];
				}

				std::list<Area<HitCheckObject>*>::iterator areaListIter = m_inAreaList.begin();
				while (areaListIter != m_inAreaList.end()) {
					std::list<HitCheckObject*>::iterator thisIter = m_splitAreas[_divideLevel][_mortonNumber].GetListInstance().begin();
					std::list<HitCheckObject*>::iterator thisIterEnd = m_splitAreas[_divideLevel][_mortonNumber].GetListInstance().end();
					while (thisIter != thisIterEnd) {
						std::list<HitCheckObject*>::iterator anotherIter = (*areaListIter)->GetListInstance().begin();
						std::list<HitCheckObject*>::iterator anotherIterEnd = (*areaListIter)->GetListInstance().end();
						if (HitCheck((*thisIter)->GetCollision(), (*anotherIter)->GetCollision())) {
							(*thisIter)->Hit(true);
							(*anotherIter)->Hit(true);
						}
						anotherIter++;
					}
					thisIter++;
				}

				if (_divideLevel < m_splitLevel && area != nullptr) {
					m_inAreaList.push_back(area);
				}

				if (_divideLevel < m_splitLevel) {
					for (int i = 0; i < _divideLevel; ++i) {
						QuadTreeHitCheck(_divideLevel + 1, (_mortonNumber << 2) + i);
					}
				}

				if (area != nullptr) {
					m_inAreaList.remove(area);
				}
			}

		protected:
			friend class Singleton< QuadTree >;
			QuadTree() { }
			
			Area<HitChackObject>* AreaCreate(int _x, int _z, vec2 _mapSize) {
				// map全体の大きさから分割数分だけ割ったareaのサイズを決定
				int area_x = _areaSize.x / m_splitLevel;
				int area_z = _areaSize.y / m_splitLevel;

				// 分割されたareaの中心を取得
				float point_x = area_x / 2.0f;
				float point_z = area_z / 2.0f;


				Quadrangle2D areaSize;
				areaSize.mp_center.x = _x * point_x + point_x;
				areaSize.mp_center.y = _z * point_z + point_z;

				for (int i = 0; i < 4; ++i) {
					// 右側の点のx座標設定
					if (i < 2) {
						areaSize.m_point[i].x = areaSize.mp_center.x - point_x;
					}
					// 左側の点のx座標設定
					else {
						areaSize.m_point[i].x = areaSize.mp_center.x + point_x;
					}

					// 上側の点のy座標設定
					if (0 == (i % 3)) {
						areaSize.m_point[i].y = areaSize.mp_center.y - point_z;
					}
					// 下側の点のy座標設定
					else {
						areaSize.m_point[i].y = areaSize.mp_center.y - point_z;
					}
				}
				Area<HitCheckObject>* area = new Area(Get2DMortonNumber(_x, _z), areaSize);

				return area;
			}

			bool ClearObjectList() {
				for (int z = 0; z <= m_splitLevel; ++z) {
					for (int x = 0; x <= m_splitLevel; ++x) {
						m_splitAreas[z][x].GetListInstance().clear();
					}
				}
				return true;
			}

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

			vec2											m_mapSize;
			unsigned int									m_splitLevel;
			std::vector<std::vector<Area<HitCheckObject>>>	m_splitAreas;
			std::list<Area<HitCheckObject>*>				m_inAreaList;
		};

	}
}