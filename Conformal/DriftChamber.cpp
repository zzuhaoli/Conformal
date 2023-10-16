#include "DriftChamber.hh"
#include "TEveManager.h"
#include "TGeoTube.h"

#include <iostream>


DriftChamber::DriftChamber(std::string tgeoFileName, std::string dcVolName){
  TEveManager::Create();
  m_geo = gEve->GetGeometry(tgeoFileName.c_str());
  m_dcVolName = dcVolName; 
}


std::optional<DriftWire> DriftChamber::getSignalWire(int layerId, int cellId) const{

  std::string layerStr = std::to_string(layerId);
  std::string cellStr = std::to_string(cellId);

  TGeoNode* node0 = m_geo->GetTopVolume()->FindNode(m_dcVolName.c_str());
  if(not node0){
    return std::nullopt;
  }

  std::string layerName = "MDCLayer" + layerStr + "_layer" + layerStr + "_" + layerStr;
  TGeoNode* layer = node0->GetVolume()->FindNode(layerName.c_str());
  if(not layer){
    std::cout<<"find layer fail" << std::endl; 
    return std::nullopt;
  }
  // The relative position of layer w.r.t. chamber 
  const TGeoMatrix* lmatrix = layer->GetMatrix();

  std::string cellName_stereo = "layer_" + layerStr + "stereocell_" + cellStr;
  std::string cellName_axial = "layer_" + layerStr + "axialcell_" + cellStr;
  std::string cellName = cellName_stereo;
  TGeoNode* cell = layer->GetVolume()->FindNode(cellName_stereo.c_str());
  // The relative position of cell w.r.t. layer
  if(cell==nullptr){
     cell = layer->GetVolume()->FindNode(cellName_axial.c_str());
     if(cell ==nullptr){
       std::cout<<"can't find cell" << std::endl; 
       return std::nullopt;
     } 
     //std::cout<<"axial layer " << layerId <<" cell " << cellId << std::endl; 
     cellName = cellName_axial;
  } else{
     //std::cout<<"stereo layer " << layerId <<" cell " << cellId << std::endl; 
  }
  const TGeoMatrix* cmatrix = cell->GetMatrix();

  double unitScalar = 10;

  std::string signalWireName = "sense_wire_0";
  TGeoNode* wire = cell->GetVolume()->FindNode(signalWireName.c_str());
   
  TGeoTube* wireTube = dynamic_cast<TGeoTube*>(wire->GetVolume()->GetShape());
  double wireHalfZ = 0; 
  if (wireTube) {
    wireHalfZ = wireTube->GetDz() * unitScalar;
    //std::cout<<"cast to tube good " << std::endl; 
  } else {
    std::cout<<"cast to tube fail " << std::endl; 
    return std::nullopt;
  }

  // The relative position of wire w.r.t. cell 
  const TGeoMatrix* wmatrix = wire->GetMatrix();

  TGeoHMatrix transform =
  TGeoCombiTrans(*lmatrix) * TGeoCombiTrans(*cmatrix) * TGeoCombiTrans(*wmatrix);

  // Get the placement and orientation in respect to its mother
  const Double_t* rotation = transform.GetRotationMatrix();
  const Double_t* translation = transform.GetTranslation();
 
  Vector3 center = Vector3(translation[0] * unitScalar, translation[1] * unitScalar, translation[2] * unitScalar);
  Vector3 line = Vector3(rotation[2], rotation[5], rotation[8]);

  //std::cout<<"center " << center.transpose() <<", line "<< line.transpose() <<" length " << wireHalfZ << std::endl;

  DriftWire dw{center, line, wireHalfZ}; 
  return dw;
}
