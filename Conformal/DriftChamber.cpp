#include "DriftChamber.hh"
#include "TEveManager.h"
#include "TGeoTube.h"

#include <iostream>


DriftChamber::DriftChamber(std::string tgeoFileName, std::string dcVolName){
  TEveManager::Create();//新建TEveManager对象
  m_geo = gEve->GetGeometry(tgeoFileName.c_str());//获取TEveManager对象
  m_dcVolName = dcVolName; 
}


std::optional<DriftWire> DriftChamber::getSignalWire(int layerId, int cellId) const{

  std::string layerStr = std::to_string(layerId);
  std::string cellStr = std::to_string(cellId);//将层Id与Cell Id转换成字符串类型

  TGeoNode* node0 = m_geo->GetTopVolume()->FindNode(m_dcVolName.c_str());//利用 TGeoNode类获取体积指针
  if(not node0){
    return std::nullopt;
  }

  std::string layerName = "MDCLayer" + layerStr + "_layer" + layerStr + "_" + layerStr;
  TGeoNode* layer = node0->GetVolume()->FindNode(layerName.c_str());//获取某层的指针
  if(not layer){
    std::cout<<"find layer fail" << std::endl; 
    return std::nullopt;
  }
  // The relative position of layer w.r.t. chamber 
  const TGeoMatrix* lmatrix = layer->GetMatrix();//利用TGeoMatrix类获取层相对于探测器的矩阵信息

  std::string cellName_stereo = "layer_" + layerStr + "stereocell_" + cellStr;//立体cell信息
  std::string cellName_axial = "layer_" + layerStr + "axialcell_" + cellStr;//轴向cell信息
  std::string cellName = cellName_stereo;
  TGeoNode* cell = layer->GetVolume()->FindNode(cellName_stereo.c_str());//获取cell信息
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
  TGeoNode* wire = cell->GetVolume()->FindNode(signalWireName.c_str());//获取信号线信息
   
  TGeoTube* wireTube = dynamic_cast<TGeoTube*>(wire->GetVolume()->GetShape());//将线转换成柱体
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
  TGeoCombiTrans(*lmatrix) * TGeoCombiTrans(*cmatrix) * TGeoCombiTrans(*wmatrix);//将几何矩阵进行组合

  // Get the placement and orientation in respect to its mother
  const Double_t* rotation = transform.GetRotationMatrix();
  const Double_t* translation = transform.GetTranslation();
 
  Vector3 center = Vector3(translation[0] * unitScalar, translation[1] * unitScalar, translation[2] * unitScalar);
  Vector3 linex = Vector3(rotation[0], rotation[2], rotation[6]);
  Vector3 liney = Vector3(rotation[1], rotation[4], rotation[7]);
  Vector3 line = Vector3(rotation[2], rotation[5], rotation[8]);

  //std::cout<<"center " << center.transpose() <<", line "<< line.transpose() <<" length " << wireHalfZ << std::endl;

  DriftWire dw{center,linex,liney,line, wireHalfZ}; 
  return dw;
}
