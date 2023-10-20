#include"setStyle.hh"
#include "../Algebra.hh"
#include "../DriftChamber.hh"
#include "../f.hh"
#include "../DriftWire.hh"
#include <vector>
#include <TCanvas.h>
#include <TFile.h>
#include <TGraph.h>
#include <TH1D.h>
#include <TH1F.h>
#include <TTree.h>
#include <TTreeReader.h>
#include <TTreeReaderArray.h>
#include <TTreeReaderValue.h>
#include <Eigen/Geometry>
#include <TPolyLine3D.h>
#include <TLine.h>
#include <cmath>
#include <random>

Circle getCircleInGeometrySpace(double zField, Vector3 vertex,
                                Vector3 initialMomentum, double charge) {
  double pt = std::hypot(initialMomentum.x(), initialMomentum.y());
  std::cout << " pt  = " << pt << std::endl;

  double radius = pt / 0.3 / zField * 1000; // in unit of mm

  Vector2 tangVec(initialMomentum.x() / pt, initialMomentum.y() / pt);

  Vector2 radialVec(tangVec.y(), -tangVec.x());

  // The sign needs to be checked.
  double sign = charge > 0 ? 1 : -1;
  Vector3 tangent = Vector3(0, 0, 1).cross(initialMomentum);
  double dotVec = tangent.dot(vertex);
  double sign2 = dotVec > 0 ? -1 : 1;
  sign = sign * sign2;

  Vector2 center(vertex.x() + sign * radialVec.x() * radius,
                 vertex.y() + sign * radialVec.y() * radius);
  // std::cout<<"vertex " << vertex << ", radius "<< radius << std::endl;

  return Circle(center.x(), center.y(), radius);
}

void drawStereo_circle()
{
  DriftChamber driftChamber( "/mnt/c/Users/lhaza/Desktop/Conformal/run/STCF_tracker.root","MultiLayerDriftChamber02_Gas_2");
  TFile *file =TFile::Open("/mnt/c/Users/lhaza/Desktop/Conformal/run/fullsim_lambdalambdabar.root","READ");
  TTreeReader *treeReader =new TTreeReader ("events",file);
  
  TTreeReaderArray<double> *particleVertexX =
      new TTreeReaderArray<double>(*treeReader, "MCParticleCol.vertex.x");
  TTreeReaderArray<double> *particleVertexY =
      new TTreeReaderArray<double>(*treeReader, "MCParticleCol.vertex.y");
  TTreeReaderArray<double> *particleVertexZ =
      new TTreeReaderArray<double>(*treeReader, "MCParticleCol.vertex.z");
  TTreeReaderArray<float> *particleMomentumX =
      new TTreeReaderArray<float>(*treeReader, "MCParticleCol.momentum.x");
  TTreeReaderArray<float> *particleMomentumY =
      new TTreeReaderArray<float>(*treeReader, "MCParticleCol.momentum.y");
  TTreeReaderArray<float> *particleMomentumZ =
      new TTreeReaderArray<float>(*treeReader, "MCParticleCol.momentum.z");
  TTreeReaderArray<int> *particlePDG =
      new TTreeReaderArray<int>(*treeReader, "MCParticleCol.PDG");
  TTreeReaderArray<float> *particleCharge =
      new TTreeReaderArray<float>(*treeReader, "MCParticleCol.charge");
  TTreeReaderArray<int> *particleTrackID =
      new TTreeReaderArray<int>(*treeReader, "MCParticleCol.trackID");
  ;
  TTreeReaderArray<int> *MDCcellID =
      new TTreeReaderArray<int>(*treeReader, "MDCHitCol.cellID");
  TTreeReaderArray<int> *MDClayerID =
      new TTreeReaderArray<int>(*treeReader, "MDCHitCol.layerID");
  TTreeReaderArray<int> *MDCparticleId =
      new TTreeReaderArray<int>(*treeReader, "MDCHitCol.mcParticleIndex");
  TTreeReaderArray<double> *MDCpositionX =
      new TTreeReaderArray<double>(*treeReader, "MDCHitCol.position.x");
  TTreeReaderArray<double> *MDCpositionY =
      new TTreeReaderArray<double>(*treeReader, "MDCHitCol.position.y");
  TTreeReaderArray<double> *MDCpositionZ =
      new TTreeReaderArray<double>(*treeReader, "MDCHitCol.position.z");
  TTreeReaderArray<double> *MDCmomentumX =
      new TTreeReaderArray<double>(*treeReader, "MDCHitCol.momentum.x");
  TTreeReaderArray<double> *MDCmomentumY =
      new TTreeReaderArray<double>(*treeReader, "MDCHitCol.momentum.y");
  TTreeReaderArray<double> *MDCmomentumZ =
      new TTreeReaderArray<double>(*treeReader, "MDCHitCol.momentum.z");
  TTreeReaderArray<double> *MDCdriftDistance =
      new TTreeReaderArray<double>(*treeReader, "MDCHitCol.driftDistance");
  //**********定义一些量*****************
  Vector3 v_mdchit=Vector3(0,0,0);
  int mdccellid=0;
  int mdclayerid=0;
  double mdcdriftdistance=0;
  Vector3 v_linecenter;
  Vector3 v_linex;
  Vector3 v_liney;
  Vector3 v_line;
  std::pair<Vector3,Vector3> endpoint;
  double single=0;
  int index_particle=0;
  Vector3 vertex;
  Vector3 mom;
  double charge =0;
  std::vector<Vector3> v_driftpoints;//定义储存漂移圆上点的坐标的容器
  //*************************************
  while(treeReader->Next())
  {
      int n_particles = particleVertexX->GetSize();
      std::cout<<"该事例的粒子数为："<<n_particles<<std::endl;
      int n_mdchits = MDCpositionX->GetSize();
      std::cout<<"该事例的击中为："<<n_mdchits<<std::endl;
      for(int i=0;i<n_mdchits;i++)
      {
         mdccellid=(*MDCcellID)[i];
	 mdclayerid=(*MDClayerID)[i];
	 std::cout << "MDCcellID is " << mdccellid << std::endl;
	 std::cout << "MDClayerID is " << mdclayerid << std::endl;
	 v_mdchit=Vector3((*MDCpositionX)[i],(*MDCpositionY)[i],(*MDCpositionZ)[i]);
	 mdcdriftdistance=(*MDCdriftDistance)[i];
	 std::cout << "R is: " <<  mdcdriftdistance << std::endl;
	 std::cout << "i is: " << i << std::endl;
	 //******获取丝的信息******
         auto signalwire = driftChamber.getSignalWire(mdclayerid,mdccellid);
         v_linecenter=signalwire->getCenter();
         v_linex=signalwire->getLinexVec();
         v_liney=signalwire->getLineyVec();
         v_line=signalwire->getLineVec();
         endpoint=signalwire->getEnds();
	 single=signalwire->getSinStereoAngle();
	 //************************
	 //if(mdcdriftdistance>0&&single>30)
         if(i==10)		 
	 {
	    index_particle=(*MDCparticleId)[i];
	    std::cout<<"粒子索引值为:"<<index_particle<<std::endl;
	    vertex=Vector3((*particleVertexX)[index_particle], (*particleVertexY)[index_particle],(*particleVertexZ)[index_particle]);
            mom=Vector3((*particleMomentumX)[index_particle], (*particleMomentumY)[index_particle],(*particleMomentumZ)[index_particle]);
            std::cout << "mom " << mom << std::endl;
            charge = (*particleCharge)[i];
	    break;
         }    	 
	 else
	 {
            continue;
	 }         
      }
      break;
  }
  Circle trackcircle = getCircleInGeometrySpace(1, vertex, mom, charge);
  std::cout<<"x="<<v_line.x()<<"y="<<v_line.y()<<"z="<<v_line.z()<<std::endl;
  std::cout << "MDCcellID is " << mdccellid << std::endl;
  std::cout << "MDClayerID is " << mdclayerid << std::endl;
  Vector3 linecenter_mdchit=Vector3(v_mdchit.x()-v_linecenter.x(),v_mdchit.x()-v_linecenter.x(),v_mdchit.x()-v_linecenter.x());
  double mo_line=std::sqrt(v_line.x()*v_line.x()+v_line.y()*v_line.y()+v_line.z()*v_line.z());
  double mo_linex=std::sqrt(v_linex.x()*v_linex.x()+v_linex.y()*v_linex.y()+v_linex.z()*v_linex.z());
  double mo_liney=std::sqrt(v_liney.x()*v_liney.x()+v_liney.y()*v_liney.y()+v_liney.z()*v_liney.z());
  Vector3 unitv_line = Vector3(v_line.x()/mo_line,v_line.y()/mo_line,v_line.z()/mo_line);
  Vector3 unitv_linex = Vector3(v_linex.x()/mo_linex,v_linex.y()/mo_linex,v_linex.z()/mo_linex);
  Vector3 unitv_liney = Vector3(v_liney.x()/mo_liney,v_liney.y()/mo_liney,v_liney.z()/mo_liney);

  double len_linecenter_driftcenter = 0;
  double dot = linecenter_mdchit.dot(v_line);
  if(dot>0)
  {
      len_linecenter_driftcenter = linecenter_mdchit.dot(v_line)/mo_line;
  }
  else
  {
      len_linecenter_driftcenter = -(linecenter_mdchit.dot(v_line)/mo_line);
  }
  std::cout<<"点乘为"<<dot<<std::endl;

  Vector3 linecenter_driftcenter = Vector3(len_linecenter_driftcenter*unitv_line.x(),len_linecenter_driftcenter*unitv_line.y(),len_linecenter_driftcenter*unitv_line.z());
  Vector3 v_oring_driftcenter = v_linecenter + v_line/mo_line*len_linecenter_driftcenter;
  
  int numPoints=5000;
  for (int j = 0; j < numPoints; ++j)
  {
    double theta1 = 2 * TMath::Pi() * j / numPoints;
    double touyingx = mdcdriftdistance * TMath::Cos(theta1);
    double touyingy = mdcdriftdistance * TMath::Sin(theta1);
    Vector3 oring_driftpoint;
    oring_driftpoint.x()=v_oring_driftcenter.x()+touyingx*v_linex.x()+touyingy*v_liney.x();
    oring_driftpoint.y()=v_oring_driftcenter.y()+touyingx*v_linex.y()+touyingy*v_liney.y();
    oring_driftpoint.z()=v_oring_driftcenter.z()+touyingx*v_linex.z()+touyingy*v_liney.z();
    v_driftpoints.push_back(oring_driftpoint);
  }
  cout<<"容器数量"<< v_driftpoints.size()<<std::endl;
  // 画图
  TCanvas *c1 = new TCanvas("c1", "", 600, 600);
  TCanvas *c2 = new TCanvas("c2", "", 600, 600);
  TGraph2D* graph2D = new TGraph2D(numPoints);
  auto mg = new TMultiGraph();
  //iTGraph* graph = new TGraph2D(numPoints);
  TGraph* graph[2];
  graph[0] = new TGraph(numPoints); // 初始化
  graph[1] = new TGraph(numPoints);
  for(int n=0;n<numPoints;n++)
  {
     double xvalues = v_driftpoints[n].x();
     double yvalues = v_driftpoints[n].y();
     double zvalues = v_driftpoints[n].z();
     graph2D->SetPoint(n,xvalues,yvalues,zvalues);
     graph[0]->SetPoint(n,xvalues,yvalues);
  }
  //auto endpoint=signalwire.getEnds();
  Vector3 end1 = endpoint.first;
  Vector3 end2 = endpoint.second;
  TPolyLine3D *line1 = new TPolyLine3D(2);//新建三维直线
  line1->SetPoint(0,end1.x(),end1.y(),end1.z());
  line1->SetPoint(1,end2.x(),end2.y(),end2.z());
  std::cout<<"端点1x坐标: "<<end1.x()<<"端点1y坐标"<<end1.y()<<"端点1z坐标"<<end1.z()<<std::endl;
  std::cout<<"端点2x坐标: "<<end2.x()<<"端点2y坐标"<<end2.y()<<"端点2z坐标"<<end2.z()<<std::endl;
  line1->SetLineWidth(2);
  line1->SetLineColor(2);

  c1->cd();
  graph2D->Draw("APL");
  line1->Draw();
  c1->Update();
  
  //circle trackcircle = getCircleInGeometrySpace(1, vertex, mom, charge);
  //v_driftpoints
  double track_centerX = trackcircle.m_CircleYPosition;  
  double track_centerY = trackcircle.m_CircleYPosition;
  double track_radius = trackcircle.m_CircleRadius;
  std::cout<<"径迹x为："<<track_centerX<<std::endl;
  std::cout<<"径迹y为："<<track_centerY<<std::endl;
  std::cout<<"径迹半径为："<<track_radius<<std::endl;
  for(int ni=0;ni<numPoints;ni++)
  {
      double theta2 = 2 * TMath::Pi() * ni/numPoints;
      double x = track_centerX + track_radius * TMath::Cos(theta2);
      double y = track_centerY + track_radius * TMath::Sin(theta2);
      graph[1]->SetPoint(ni,x,y);
  }
  TLine *line2 = new TLine (end1.x(),end1.y(),end2.x(),end2.y());
  c2->cd();
  mg->Add(graph[0]);
  //mg->Add(graph[1]);
  mg->Draw("APL");
  line2->Draw();
  c2->Update();
}
