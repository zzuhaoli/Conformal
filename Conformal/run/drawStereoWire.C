#include <circle.hh>
#include <Algebra.hh>
#include <DriftChamber.hh>

#include <TCanvas.h>
#include <TGraph.h>
#include <TH1D.h>
#include <TFile.h>
#include <TTree.h>
#include <TTreeReader.h>
#include <TTreeReaderArray.h>
#include <TTreeReaderValue.h>

#include <cmath>
#include <random>


void drawStereoWire(){


  DriftChamber driftChamber("/home/xiaocong/Software/Oscar/acts/build/bin/STCF_tracker.root", "MultiLayerDriftChamber02_Gas_2");

  int layer=29;
  int nCells = 6000;

  std::vector<TLine*> lines;
  int i=0; 
  
  TCanvas *c1 = new TCanvas("c1", "", 600, 500);
 
  TH1F* h1 = new TH1F("h1", "", 100, -1000, 1000); 
  h1->Draw(); 
  h1->GetYaxis()->SetRangeUser(-1000, 1000); 

  //for(int i=5504; i<5759; i=i+20){
  for(int i=5504; i<5759; i=i+10){
    auto wire = driftChamber.getSignalWire(layer, i);
    if(wire.has_value()){ 
    auto ends = wire->getEnds();
    auto end0xy = Vector2(ends.first.x(), ends.first.y());
    auto end1xy = Vector2(ends.second.x(), ends.second.y());
    std::cout<< i << ": end0 " << end0xy.x() << ", "<< end0xy.y() << std::endl; 
    std::cout<< i << ": end1 " << end1xy.x() << ", "<< end1xy.y() << std::endl;
    std::cout<< "line on xy length "<< std::hypot(end0xy.x()- end1xy.x(), end0xy.y()- end1xy.y()) << std::endl;

    TLine* line = new TLine(end0xy.x(), end0xy.y(), end1xy.x(), end1xy.y());
      line->SetLineWidth(2);
      line->SetLineColor(2);
      //lines.push_back(line);
      line->Draw("same"); 
    }
  }
   

//  for(auto line : lines){
//   line->Draw("same");
//  }

}
