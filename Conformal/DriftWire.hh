#pragma once

#include "Algebra.hh"


class DriftWire{
public:
  DriftWire(Vector3 center,Vector3 linexVec,Vector3 lineyVec,Vector3 lineVec, double halfLength);	
  Vector3 getCenter() const ;
  Vector3 getLinexVec() const;
  Vector3 getLineyVec() const;
  Vector3 getLineVec() const;
  double getSinStereoAngle() const;
  std::pair<Vector3, Vector3> getEnds() const;

private:
  Vector3 m_center;
  Vector3 m_linexVec;
  Vector3 m_lineyVec;
  Vector3 m_lineVec;
  double m_halfLength;
};

