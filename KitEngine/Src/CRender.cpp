#include "stdafx.h"

#include "../Inc/CRender.h"

using namespace kit::Engine;

Matrix::Matrix(Matrix const& _matrix) {
	m_mWorld = _matrix.m_mWorld;
	m_mView = _matrix.m_mView;
	m_mProjection = _matrix.m_mProjection;
}

Matrix Matrix::operator= (Matrix& _matrix) {
	this->m_mWorld = _matrix.m_mWorld;
	this->m_mView = _matrix.m_mView;
	this->m_mProjection = _matrix.m_mProjection;
	return *this;
}

Matrix2D::Matrix2D(Matrix2D const& _matrix) {
	m_xScale = _matrix.m_xScale;
	m_xRotationOrigin = _matrix.m_xRotationOrigin;
	m_fRotation = _matrix.m_fRotation;
	m_xTranslation = _matrix.m_xTranslation;
}

Matrix2D Matrix2D::operator= (Matrix2D& _matrix) {
	this->m_xScale = _matrix.m_xScale;
	this->m_xRotationOrigin = _matrix.m_xRotationOrigin;
	this->m_fRotation = _matrix.m_fRotation;
	this->m_xTranslation = _matrix.m_xTranslation;
	return *this;
}
