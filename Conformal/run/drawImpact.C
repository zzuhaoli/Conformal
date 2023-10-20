#include "../Algebra.hh"
#include "../DriftChamber.hh"
#include "../f.hh"
#include "setStyle.hh"
#include <Eigen/Geometry>
#include <TCanvas.h>
#include <TFile.h>
#include <TGraph.h>
#include <TH1D.h>
#include <TH1F.h>
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

void drawImpact() {
  gStyle->SetOptFit(0000);
  gStyle->SetOptStat(0000);
  gStyle->SetPadLeftMargin(0.20);
  gStyle->SetPadRightMargin(0.10);
  gStyle->SetPadTopMargin(0.1);
  gStyle->SetPadBottomMargin(0.18);
  gStyle->SetNdivisions(510, "y");
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);

  double xTitleSize = 0.05;
  double yTitleSize = 0.05;
  double xLabelSize = 0.05;
  double yLabelSize = 0.05;
  double xTitleOffset = 1.2;
  double yTitleOffset = 1.6;

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

  std::cout<<"Before creating driftChamber" << std::endl;
  // The map of particle index to particle circle
  std::map<int, Circle> particleCircles;
  // The map of particle index to drift circles
  std::map<int, std::vector<Circle>> driftCircles;

  DriftChamber driftChamber(
      "/mnt/c/Users/lhaza/Desktop/Conformal/run/STCF_tracker.root",
      "MultiLayerDriftChamber02_Gas_2");

  TH1F *hd0_pi = new TH1F("hd0_pi", "", 100, 0, 200);
  TH1F *hd0_p = new TH1F("hd0_p", "", 100, 0, 200);
  TH1F *hz0_pi = new TH1F("hz0_pi", "", 100, 0, 200);
  TH1F *hz0_p = new TH1F("hz0_p", "", 100, 0, 200);

  setThisHistStyle(hd0_pi, 796, 20, xTitleSize, yTitleSize, xLabelSize,
                   yLabelSize, xTitleOffset, yTitleOffset);
  setThisHistStyle(hd0_p, 814, 20, xTitleSize, yTitleSize, xLabelSize,
                   yLabelSize, xTitleOffset, yTitleOffset);
  setThisHistStyle(hz0_pi, 796, 20, xTitleSize, yTitleSize, xLabelSize,
                   yLabelSize, xTitleOffset, yTitleOffset);
  setThisHistStyle(hz0_p, 814, 20, xTitleSize, yTitleSize, xLabelSize,
                   yLabelSize, xTitleOffset, yTitleOffset);

  hd0_pi->SetLineColor(796);
  hd0_p->SetLineColor(814);

  while (treeReader->Next()) {
    std::cout << "Reading entry/event " << ientry << std::endl;

    // Since it's an array, we have to loop over it
    size_t nParticles = particleVertexX->GetSize();
    std::cout << "nParticles =  " << nParticles << std::endl;
    for (size_t i = 0; i < nParticles; ++i) {
      std::cout << "particle " << i << " has pdg " << (*particlePDG)[i]
                << std::endl;
      if (abs((*particlePDG)[i]) != 211 and abs((*particlePDG)[i]) != 2212)
        continue;

      Vector3 vertex((*particleVertexX)[i], (*particleVertexY)[i],
                     (*particleVertexZ)[i]);
      Vector3 mom((*particleMomentumX)[i], (*particleMomentumY)[i],
                  (*particleMomentumZ)[i]);
      std::cout << "mom " << mom << std::endl;
      double charge = (*particleCharge)[i];
      double vxy = std::hypot(vertex.x(), vertex.y());
      auto circle = getCircleInGeometrySpace(1, vertex, mom, charge);
      Vector2 center(circle.m_CircleXPosition, circle.m_CircleYPosition);
      double d0 = std::hypot(center.x(), center.y()) - circle.m_CircleRadius;

      Vector2 POCA_xy = center - center / std::hypot(center.x(), center.y()) *
                                     circle.m_CircleRadius;
      Vector2 vertex_xy((*particleVertexX)[i], (*particleVertexY)[i]);
      Vector2 POCA_line = POCA_xy - center;
      Vector2 vertex_line = vertex_xy - center;
      double cosphi = POCA_line.dot(vertex_line) /
                      (circle.m_CircleRadius * circle.m_CircleRadius);
      double sxy = circle.m_CircleRadius * std::acos(cosphi);
      if (sxy < 0) {
        throw std::runtime_error("sxy negative?");
      }
      Vector2 line = vertex_xy - POCA_xy;
      double sign = line.dot(Vector2(mom.x(), mom.y()));
      // sxy *cottheta
      double z0 =
          vertex.z() - sxy * mom.z() / std::hypot(mom.x(), mom.y()) * sign;

      if (abs((*particlePDG)[i]) == 211) {
        hd0_pi->Fill(d0);
        hz0_pi->Fill(z0);
      }
      if (abs((*particlePDG)[i]) == 2212) {
        hd0_p->Fill(d0);
        hz0_p->Fill(z0);
      }

    } // loop over particles in this event
  }

  TLine *line1 = new TLine(65.1125, 0, 65.1125, 30);
  TLine *line2 = new TLine(115.113, 0, 115.113, 30);
  TLine *line3 = new TLine(165.112, 0, 165.112, 30);
  line1->SetLineStyle(kDashed);
  line2->SetLineStyle(kDashed);
  line3->SetLineStyle(kDashed);

  line1->SetLineWidth(2);
  line2->SetLineWidth(2);
  line3->SetLineWidth(2);

  std::string myLabel = "J/#psi#rightarrow#Lambda(#rightarrow p#pi^{-}) "
                        "#bar{#Lambda}(#rightarrow #bar{p}#pi^{+})";

  auto leg0 = new TLegend(0.8, 0.7, 0.9, 0.8);
  leg0->SetLineStyle(0);
  leg0->SetBorderSize(0);
  leg0->SetFillStyle(0);
  leg0->AddEntry(hd0_pi, "#pi", "AL");
  leg0->AddEntry(hd0_p, "p", "AL");

  auto leg1 = new TLegend(0.8, 0.7, 0.9, 0.8);
  leg1->SetLineStyle(0);
  leg1->SetBorderSize(0);
  leg1->SetFillStyle(0);
  leg1->AddEntry(hd0_pi, "#pi", "AL");
  leg1->AddEntry(hd0_p, "p", "AL");

  hd0_p->GetXaxis()->SetTitle("d_{0} [mm]");
  hd0_p->GetYaxis()->SetTitle("Entries");
  hz0_pi->GetXaxis()->SetTitle("z_{0} [mm]");
  hz0_pi->GetYaxis()->SetTitle("Entries");

  hd0_pi->SetLineColor(796);
  hd0_p->SetLineColor(814);
  hz0_pi->SetLineColor(796);
  hz0_p->SetLineColor(814);

  hd0_pi->SetLineWidth(2);
  hd0_p->SetLineWidth(2);
  hz0_pi->SetLineWidth(2);
  hz0_p->SetLineWidth(2);

  TCanvas *c1 = new TCanvas("c1", "", 600, 500);

  hd0_pi->Draw("same");
  hd0_p->Draw("same");
  leg0->Draw("same");

  myText(0.52, 0.85, 1, 0.04, myLabel.c_str());

  line1->Draw();
  line2->Draw();
  line3->Draw();

  TCanvas *c2 = new TCanvas("c2", "", 600, 500);

  hz0_pi->Draw("same");
  hz0_p->Draw("same");
  leg1->Draw("same");

  myText(0.52, 0.85, 1, 0.04, myLabel.c_str());
}
