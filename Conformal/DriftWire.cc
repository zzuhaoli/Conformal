#include "DriftWire.hh"

#include <iostream>

DriftWire::DriftWire(Vector3 center, Vector3 lineVec, double length){
  m_center = center;
  m_lineVec = lineVec;
  m_halfLength = length;
}

Vector3 DriftWire::getCenter() const {
  return m_center;
}

Vector3 DriftWire::getLineVec() const {
  return m_lineVec;
}

double DriftWire::getSinStereoAngle() const {
  double len = std::sqrt(m_lineVec.x()*m_lineVec.x() + m_lineVec.y()*m_lineVec.y()  + m_lineVec.z()*m_lineVec.z() );
  return std::hypot(m_lineVec.x(), m_lineVec.y())/len;
}


std::pair<Vector3, Vector3> DriftWire::getEnds() const {
  double len = std::sqrt(m_lineVec.x()*m_lineVec.x() + m_lineVec.y()*m_lineVec.y()  + m_lineVec.z()*m_lineVec.z() ); 
  Vector3 endLeft = m_center - m_lineVec/len*m_halfLength; 
  Vector3 endRight = m_center + m_lineVec/len*m_halfLength; 
  //std::cout<<"m_lineVec " << m_lineVec << "len " << len <<"m_halfLength "<< m_halfLength << std::endl; 
  return {endLeft, endRight};
}
