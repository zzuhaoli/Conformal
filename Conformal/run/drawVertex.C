
void myText(Double_t x, Double_t y, Color_t color, float font,
            const char* text) {
  Double_t tsize = 0.05;
  TLatex l;  //
  l.SetTextAlign(12);
  l.SetTextSize(tsize);
  l.SetNDC();
  l.SetTextSize(font);
  l.SetTextColor(color);
  l.DrawLatex(x, y, text);
}


template <typename T>
void setThisHistStyle(T* hist, short color = 1, short marker = 20,
                      float xTitleSize = 0.05, float yTitleSize = 0.05,
                      float xLabelSize = 0.05, float yLabelSize = 0.05,
                      float xTitleOffset = 1.2, float yTitleOffset = 1.2,
                      int nDiv = 510) {
  hist->GetXaxis()->SetTitleSize(xTitleSize);
  hist->GetYaxis()->SetTitleSize(yTitleSize);
  hist->GetXaxis()->SetLabelSize(xLabelSize);
  hist->GetYaxis()->SetLabelSize(yLabelSize);
  hist->GetXaxis()->SetTitleOffset(xTitleOffset);
  hist->GetYaxis()->SetTitleOffset(yTitleOffset);
  hist->GetXaxis()->SetNdivisions(510);
  hist->GetYaxis()->SetNdivisions(nDiv);
  hist->SetMarkerStyle(marker);
  hist->SetMarkerSize(1.0);
  hist->SetLineWidth(2);
  hist->SetTitle("");
  hist->SetLineColor(color);
  //hist->SetLineColor(0);
  //hist->SetLineColorAlpha(0, 1); 
  //hist->SetLineStyle(0); 
  hist->SetMarkerColor(color);
}


void drawVertex(){
  gStyle->SetOptFit(0000);
  gStyle->SetOptStat(0000);
  //gStyle->SetPadLeftMargin(0.20);
  //gStyle->SetPadRightMargin(0.10);
  gStyle->SetPadLeftMargin(0.18);
  gStyle->SetPadRightMargin(0.08);
  gStyle->SetPadTopMargin(0.1);
  gStyle->SetPadBottomMargin(0.18);

  gStyle->SetNdivisions(510, "y");
  gStyle->SetPadTickX(1);

  double xTitleSize = 0.05;
  double yTitleSize = 0.05;
  double xLabelSize = 0.05;
  double yLabelSize = 0.05;
  double xTitleOffset = 1.2;
  double yTitleOffset = 1.6;



  TFile* file = TFile::Open("lambdalambdabar/fullsim.root","READ");
  
  TTree* tree = (TTree*)file->Get("events");
  
  TH1F* vxy = new TH1F("vxy", "", 100, 0, 400);
  TH1F* vz = new TH1F("vz", "", 100, 0, 400);
  
  tree->Draw("hypot(MCParticleCol.vertex.x,MCParticleCol.vertex.y)>>vxy", "abs(MCParticleCol.PDG)==2212||abs(MCParticleCol.PDG)==211");
  tree->Draw("MCParticleCol.vertex.z>>vz", "abs(MCParticleCol.PDG)==2212||abs(MCParticleCol.PDG)==211");
  
  TCanvas *c1 = new TCanvas("c1", "", 600, 500);
  
  auto leg = new TLegend(0.8, 0.7, 0.9, 0.8);
  leg->SetLineStyle(0);
  leg->SetBorderSize(0);
  leg->SetFillStyle(0);
  leg->AddEntry(vxy,"V_{xy}", "AL"); 
  leg->AddEntry(vz, "V_{z}", "AL"); 
  
  vz->GetXaxis()->SetTitle("Vertex displacement [mm]");
  vz->GetYaxis()->SetTitle("Entries");
  
  setThisHistStyle(vxy, 796, 20, xTitleSize, yTitleSize, xLabelSize, yLabelSize, xTitleOffset, yTitleOffset);
  setThisHistStyle(vz,  814, 20, xTitleSize, yTitleSize, xLabelSize, yLabelSize, xTitleOffset, yTitleOffset);
  
  
  vxy->SetLineColor(796);
  vz->SetLineColor(814);
  
  vxy->SetLineWidth(2);
  vz->SetLineWidth(2);
  
  vz->Draw("same");
  vxy->Draw("same");
  
  leg->Draw("same");
  
  
  std::string myLabel = "J/#psi#rightarrow#Lambda(#rightarrow p#pi^{-}) #bar{#Lambda}(#rightarrow #bar{p}#pi^{+})";
  
  myText(0.52, 0.85, 1, 0.04, myLabel.c_str());
  
  
  TLine* line1 = new TLine(65.1125, 0, 65.1125, 30000);
  TLine* line2 = new TLine(115.113, 0, 115.113, 30000);
  TLine* line3 = new TLine(165.112, 0, 165.112, 30000);
  line1->SetLineStyle(kDashed);
  line2->SetLineStyle(kDashed);
  line3->SetLineStyle(kDashed);
  
  line1->SetLineWidth(2);
  line2->SetLineWidth(2);
  line3->SetLineWidth(2);
  
  line1->Draw();
  line2->Draw();
  line3->Draw();

}
