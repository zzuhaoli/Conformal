#include "../Algebra.hh"
#include "../DriftChamber.hh"
#include "../f.hh"
#include <Eigen/Geometry>
#include <TCanvas.h>
#include <TFile.h>
#include <TGraph.h>
#include <TH1D.h>
#include <TTree.h>
#include <TTreeReader.h>
#include <TTreeReaderArray.h>
#include <TTreeReaderValue.h>

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

void drawDisplacedTrack() {

  TFile *file =
      TFile::Open("/mnt/c/Users/lhaza/Desktop/Conformal/run/fullsim_lambdalambdabar.root",
                  "READ");
  // TFile* file =
  // TFile::Open("/home/xiaocong/Software/Oscar/offline/Examples/JobSubmission/3RPC7PS/fullsim_pipijpsi.root","READ");

  TTreeReader *treeReader = new TTreeReader("events", file);

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

  // std::default_random_engine generator;
  // std::normal_distribution<double> stdNormal(0.0, 1.0);

  // Get a particle that has vxy> 150 mm
  int ientry = 0;
  bool findDisplacedTrack = false;

  // The map of particle index to particle circle
  std::map<int, Circle> particleCircles;
  // The map of particle index to drift circles
  std::map<int, std::vector<Circle>> driftCircles;

  DriftChamber driftChamber(
      "/mnt/c/Users/lhaza/Desktop/Conformal/run/STCF_tracker.root",
      "MultiLayerDriftChamber02_Gas_2");

  while (treeReader->Next()) {
    std::cout << "Reading entry/event " << ientry << std::endl;

    // Since it's an array, we have to loop over it
    size_t nParticles = particleVertexX->GetSize();
    std::cout << "nParticles =  " << nParticles << std::endl;
    for (size_t i = 0; i < nParticles; ++i) {
      std::cout << "particle " << i << " has pdg " << (*particlePDG)[i]
                << std::endl;
      // if(abs((*particlePDG)[i]) != 211  and abs((*particlePDG)[i]) != 2212)
      // continue;

      Vector3 vertex((*particleVertexX)[i], (*particleVertexY)[i],
                     (*particleVertexZ)[i]);
      Vector3 mom((*particleMomentumX)[i], (*particleMomentumY)[i],
                  (*particleMomentumZ)[i]);
      std::cout << "mom " << mom << std::endl;
      double charge = (*particleCharge)[i];
      double vxy = std::hypot(vertex.x(), vertex.y());
      if (vxy > 0) {
        std::cout << "vxy = " << vxy << std::endl;
        findDisplacedTrack = true;
        particleCircles[i] = getCircleInGeometrySpace(1, vertex, mom, charge);
      }
    } // loop over particles in this event

    if (findDisplacedTrack) {
      // Loop over all the dc hits for this particle
      size_t nDCHits = MDCcellID->GetSize();
      for (int i = 0; i < nDCHits; ++i) {
        double driftDist = (*MDCdriftDistance)[i];
        int layerID = (*MDClayerID)[i];
        int cellID = (*MDCcellID)[i];
        int index = (*MDCparticleId)[i];
        std::cout << "index =  " << index << std::endl;
        // Get wire center
        auto dw = driftChamber.getSignalWire(layerID, cellID);
        Vector3 center = dw->getCenter();
        driftCircles[index].push_back({center.x(), center.y(), driftDist});
      }

      // stop here
      break;
    }

    ientry++;
  }

  // Get the conformal circles
  std::map<int, ConformalCircle> particleCirclesConformal;
  std::map<int, std::vector<ConformalCircle>> driftCirclesConformal;
  // Initialize
  for (const auto &[index, circle] : particleCircles) {
    particleCirclesConformal[index] = ConformalCircle(1, 1, 1);
  }
  for (const auto &[index, circles] : driftCircles) {
    for (const auto &circle : circles) {
      driftCirclesConformal[index].push_back(ConformalCircle(1, 1, 1));
    }
  }

  // get conformal circles
  for (const auto &[index, circle] : particleCircles) {
    particleCirclesConformal[index].get_ConformalCircleInf(
        particleCircles[index]);
  }
  int ndd = 0;
  for (const auto &[index, circles] : driftCircles) {
    ndd += circles.size();
    for (int ic = 0; ic < circles.size(); ++ic) {
      driftCirclesConformal[index][ic].get_ConformalCircleInf(circles[ic]);
    }
  }

  // 定义圆上的点数
  int numPoints = 5000;
  // std::vector<int> colors = {814, 796, 854, 896, 2};
  std::vector<int> colors = {2, 4, 5, 6};

  auto mg1 = new TMultiGraph();
  TGraph *gp1[particleCirclesConformal.size()];
  std::map<int, std::vector<TGraph *>> gd1;
  TCanvas *c1 = new TCanvas("c1", "", 600, 600);

  int i = 0;
  for (const auto &[index, circle] : particleCircles) {
    // 设置坐标点
    gp1[i] = new TGraph(numPoints);
    double centerX = circle.m_CircleXPosition;
    double centerY = circle.m_CircleYPosition;
    double r = circle.m_CircleRadius;
    std::cout << "centerX = " << centerX << ", centerY  " << centerY
              << ", r = " << r << std::endl;
    for (int j = 0; j < numPoints; ++j) {
      double theta = 2 * TMath::Pi() * j / numPoints;
      double x = centerX + r * TMath::Cos(theta);
      double y = centerY + r * TMath::Sin(theta);
      gp1[i]->SetPoint(j, x, y);
    }
    gp1[i]->SetLineColor(colors[i]);
    gp1[i]->SetLineWidth(2);
    mg1->Add(gp1[i]);
    i++;
  }

  int ipp = 0;
  for (const auto &[index, circles] : driftCircles) {
    for (int i = 0; i < circles.size(); i++) {
      // 设置坐标点
      gd1[index].push_back(new TGraph(numPoints));
      double centerX = circles[i].m_CircleXPosition;
      double centerY = circles[i].m_CircleYPosition;
      double r = circles[i].m_CircleRadius;
      for (int j = 0; j < numPoints; ++j) {
        double theta = 2 * TMath::Pi() * j / numPoints;
        double x = centerX + r * TMath::Cos(theta);
        double y = centerY + r * TMath::Sin(theta);
        gd1[index][i]->SetPoint(j, x, y);
      }
      gd1[index][i]->SetLineColor(colors[ipp]);
      gd1[index][i]->SetLineWidth(2);
      mg1->Add(gd1[index][i]);
    }
    ipp++;
  }

  mg1->Draw("APL");
  c1->Update();

  auto mg2 = new TMultiGraph();
  TGraph *gp2[particleCirclesConformal.size()];
  std::map<int, std::vector<TGraph *>> gd2;

  TCanvas *c2 = new TCanvas("c2", "", 600, 600);

  i = 0;
  for (const auto &[index, circle] : particleCirclesConformal) {
    // 设置坐标点
    gp2[i] = new TGraph(numPoints);
    double centerX = circle.m_ConformalCircleXPosition;
    double centerY = circle.m_ConformalCircleYPosition;
    double r = circle.m_ConformalCircleRadius;
    std::cout << "centerX = " << centerX << ", centerY  " << centerY
              << ", r = " << r << std::endl;
    for (int j = 0; j < numPoints; ++j) {
      double theta = 2 * TMath::Pi() * j / numPoints;
      double x = centerX + r * TMath::Cos(theta);
      double y = centerY + r * TMath::Sin(theta);
      gp2[i]->SetPoint(j, x, y);
    }
    gp2[i]->SetLineColor(colors[i]);
    gp2[i]->SetLineWidth(2);
    mg2->Add(gp2[i]);
    i++;
  }

  ipp = 0;
  for (const auto &[index, circles] : driftCirclesConformal) {
    for (int i = 0; i < circles.size(); i++) {
      // 设置坐标点
      gd2[index].push_back(new TGraph(numPoints));
      double centerX = circles[i].m_ConformalCircleXPosition;
      double centerY = circles[i].m_ConformalCircleYPosition;
      double r = circles[i].m_ConformalCircleRadius;
      for (int j = 0; j < numPoints; ++j) {
        double theta = 2 * TMath::Pi() * j / numPoints;
        double x = centerX + r * TMath::Cos(theta);
        double y = centerY + r * TMath::Sin(theta);
        gd2[index][i]->SetPoint(j, x, y);
      }
      gd2[index][i]->SetLineColor(colors[ipp]);
      gd2[index][i]->SetLineWidth(2);
      mg2->Add(gd2[index][i]);
    }
    ipp++;
  }

  mg2->Draw("APL"); // 绘制线条
  c2->Update();
}
