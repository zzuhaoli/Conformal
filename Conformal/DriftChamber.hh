#include "Algebra.hh" 
#include "DriftWire.hh" 
#include "TGeoManager.h" 

#include <optional>


class DriftChamber{
public:
  DriftChamber(std::string tgeoFileName, std::string dcVolName);

  std::optional<DriftWire> getSignalWire(int layer, int cell) const; 

private:

  TGeoManager * m_geo;
  std::string m_dcVolName;

};
