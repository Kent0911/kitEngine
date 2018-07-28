#include "stdafx.h"

#include "../Inc/CQuadTree.h"

using namespace kit;
using namespace kit::Engine;

HitChackObject::HitChackObject(float _radius) {
	m_circle.mf_radius = _radius;
}

HitChackObject::~HitChackObject(){}

Area::Area(int _mortonNumber, Quadrangle2D _quad) {
	m_mortonNumber = _mortonNumber;
	m_size = _quad;
}

Area::~Area() {
	m_areaObjects.clear();
}

QuadTree::QuadTree() {
	ClearObjectList();
}

bool QuadTree::Create(int _splitLevel, vec2 _mapSize) {
	m_splitLevel = _splitLevel;

	// �������������łȂ���Ε�������+1����
	if (! (0 == (m_splitLevel % 2))) {
		m_splitLevel++;
	}

	// Z���̕�����������vector<Area>��ǉ�
	for (int z = 0; z <= m_splitLevel; ++z) {
		std::vector<Area> area_x;

		// X���̕�����������Area��ǉ�
		for (int x = 0; x <= m_splitLevel; ++x) {
			area_x.push_back(*AreaCreate(x, z, m_mapSize));
		}

		m_splitAreas.push_back(area_x);
	}
	
	return true;
}

Area* QuadTree::AreaCreate(int _x, int _z, vec2 _areaSize) {
	// map�S�̂̑傫�����番����������������area�̃T�C�Y������
	int area_x = _areaSize.x / m_splitLevel;
	int area_z = _areaSize.y / m_splitLevel;
	
	// �������ꂽarea�̒��S���擾
	float point_x = area_x / 2.0f;
	float point_z = area_z / 2.0f;


	Quadrangle2D areaSize;
	areaSize.mp_center.x = _x * point_x + point_x;
	areaSize.mp_center.y = _z * point_z + point_z;

	for (int i = 0; i < 4; ++i) {
		// �E���̓_��x���W�ݒ�
		if (i < 2) {
			areaSize.m_point[i].x = areaSize.mp_center.x - point_x;
		}
		// �����̓_��x���W�ݒ�
		else {
			areaSize.m_point[i].x = areaSize.mp_center.x + point_x;
		}

		// �㑤�̓_��y���W�ݒ�
		if (0 == (i % 3)) {
			areaSize.m_point[i].y = areaSize.mp_center.y - point_z;
		}
		// �����̓_��y���W�ݒ�
		else {
			areaSize.m_point[i].y = areaSize.mp_center.y - point_z;
		}
	}
	Area* area = new Area(Get2DMortonNumber(_x, _z), areaSize);

	return area;
}

void QuadTree::QuadTreeHitCheck(int _divideLevel, int _mortonNumber) {

	// Area���̏Փ˔���
	if (m_splitAreas[_divideLevel][_mortonNumber].GetListInstance().size() > 1) {
		std::list<HitChackObject*>::iterator thisIter = m_splitAreas[_divideLevel][_mortonNumber].GetListInstance().begin();
		std::list<HitChackObject*>::iterator thisIterEnd = m_splitAreas[_divideLevel][_mortonNumber].GetListInstance().end();
		while (thisIter != thisIterEnd) {
			std::list<HitChackObject*>::iterator anotherIter = thisIter;
			anotherIter++;
			while (anotherIter != thisIterEnd) {
				if (HitCheck((*thisIter)->GetCircle(), (*anotherIter)->GetCircle())) {
					(*thisIter)->Hit(true);
					(*anotherIter)->Hit(true);
				}
				anotherIter++;
			}
			thisIter++;
		}
	}

	// Area���X�g�Ƃ̏Փ˔���
	Area* area = nullptr;
	if (m_splitAreas[_divideLevel][_mortonNumber].GetListInstance().size() > 0) {
		area = &m_splitAreas[_divideLevel][_mortonNumber];
	}

	std::list<Area*>::iterator areaListIter = m_inAreaList.begin();
	while (areaListIter != m_inAreaList.end()) {
		std::list<HitChackObject*>::iterator thisIter = m_splitAreas[_divideLevel][_mortonNumber].GetListInstance().begin();
		std::list<HitChackObject*>::iterator thisIterEnd = m_splitAreas[_divideLevel][_mortonNumber].GetListInstance().end();
		while (thisIter != thisIterEnd) {
			std::list<HitChackObject*>::iterator anotherIter = (*areaListIter)->GetListInstance().begin();
			std::list<HitChackObject*>::iterator anotherIterEnd = (*areaListIter)->GetListInstance().end();
			if (HitCheck((*thisIter)->GetCircle(), (*anotherIter)->GetCircle())) {
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

bool QuadTree::ClearObjectList() {
	for (int z = 0; z <= m_splitLevel; ++z) {
		for (int x = 0; x <= m_splitLevel; ++x) {
			m_splitAreas[z][x].GetListInstance().clear();
		}
	}
	return true;
}


