#include <cmath>
#include <vector>

void  Vertex_p_phi()
{
  gStyle->SetOptFit(0000);
  gStyle->SetOptStat(0000);
  gStyle->SetPadLeftMargin(0.15);
  gStyle->SetPadRightMargin(0.15);

  TFile *file = TFile::Open("/mnt/c/Users/lhaza/Desktop/fullsim_lambdalambdabar.root","READ");
  TTreeReader *treeReader = new TTreeReader("events",file);

  TTreeReaderArray<double> *particleVertexX =new TTreeReaderArray<double>(*treeReader, "MCParticleCol.vertex.x");
  TTreeReaderArray<double> *particleVertexY =new TTreeReaderArray<double>(*treeReader, "MCParticleCol.vertex.y");
  TTreeReaderArray<double> *particleVertexZ =new TTreeReaderArray<double>(*treeReader, "MCParticleCol.vertex.z");
  TTreeReaderArray<float> *particleMomentumX =new TTreeReaderArray<float>(*treeReader, "MCParticleCol.momentum.x");
  TTreeReaderArray<float> *particleMomentumY =new TTreeReaderArray<float>(*treeReader, "MCParticleCol.momentum.y");
  TTreeReaderArray<float> *particleMomentumZ =new TTreeReaderArray<float>(*treeReader, "MCParticleCol.momentum.z"); 
  TTreeReaderArray<int> *particlePDG =new TTreeReaderArray<int>(*treeReader, "MCParticleCol.PDG");

  size_t nhisnames=12;
  std::string hisname[]={"Vxy_P(p)","Vxy_Pt(p)","Vxy_P(#pi)","Vxy_Pt(#pi)",
	  "Vz_P(p)","Vz_Pt(p)","Vz_P(#pi)","Vz_Pt(#pi)","Vxy_Phi(p)","Vz_Phi(p)","Vxy_Phi(#pi)","Vz_Phi(#pi)"};
  std::string XaxisTitle[]={"Vertex displacement_xy[mm]","Vertex displacement_xy[mm]","Vertex displacement_xy[mm]","Vertex displacement_xy[mm]",
	  "Vertex displacement_z[mm]","Vertex displacement_z[mm]","Vertex displacement_z[mm]","Vertex displacement_z[mm]",
	  "Vertex displacement_xy[mm]","Vertex displacement_xy[mm]","Vertex displacement_z[mm]","Vertex displacement_z[mm]", };
  std::string YaxisTitle[]={"P [GeV]","P_{T} [GeV]","P [GeV]","P_{T} [GeV]","P [GeV]","P_{T} [GeV]","P [GeV]","P_{T} [GeV]",
	  "Phi [degree]","Phi [degree]","Phi [degree]","Phi [degree]"};

  vector<TCanvas*> v_canvas;
  for(size_t i=0;i<nhisnames;i++)
  {
    v_canvas.push_back(new TCanvas(Form("c_%zu",i),"",800,800));
  }
  
  vector<TH2F*> v_TH2F;
  v_TH2F.push_back(new TH2F(hisname[0].c_str(),hisname[0].c_str(),100,-200,400,100,-3,3));//"Vxy_P(p)"
  v_TH2F.push_back(new TH2F(hisname[1].c_str(),hisname[1].c_str(),100,-200,400,100,-3,3));//"Vxy_Pt(p)"
  v_TH2F.push_back(new TH2F(hisname[2].c_str(),hisname[2].c_str(),100,-200,400,100,-3,3));//"Vxy_P(#pi)"
  v_TH2F.push_back(new TH2F(hisname[3].c_str(),hisname[3].c_str(),100,-200,400,100,-3,3));//"Vxy_Pt(#pi)"
  v_TH2F.push_back(new TH2F(hisname[4].c_str(),hisname[4].c_str(),100,-200,400,100,-10,10));//"Vz_P(p)"
  v_TH2F.push_back(new TH2F(hisname[5].c_str(),hisname[5].c_str(),100,-200,400,100,-3,3));//"Vz_Pt(p)"
  v_TH2F.push_back(new TH2F(hisname[6].c_str(),hisname[6].c_str(),100,-200,400,100,-3,3));//"Vz_P(#pi)"
  v_TH2F.push_back(new TH2F(hisname[7].c_str(),hisname[7].c_str(),100,-200,400,100,-10,10));//"Vz_Pt(#pi)"
  v_TH2F.push_back(new TH2F(hisname[8].c_str(),hisname[8].c_str(),100,-200,400,100,-10,10));//"Vxy_Phi(p)"
  v_TH2F.push_back(new TH2F(hisname[9].c_str(),hisname[9].c_str(),100,-200,400,100,-10,10));//"Vxy_Phi(p)"
  v_TH2F.push_back(new TH2F(hisname[10].c_str(),hisname[10].c_str(),100,-200,400,100,-10,10));//"Vxy_Phi(#pi)"
  v_TH2F.push_back(new TH2F(hisname[11].c_str(),hisname[11].c_str(),100,-200,400,100,-10,10));//"Vz_Phi(#pi)"

  size_t ientry=0;
  while(treeReader->Next())
  {
     std::cout<<"Reading entry:"<<ientry<<std::endl;
     size_t nparticles=particleVertexX->GetSize();
     std::cout<<"The number of the event is:"<<nparticles<<std::endl;
     std::cout<<"*******************************"<<std::endl;
     for(size_t i=0;i<nparticles;i++)
     {
       auto Vxy = hypot((*particleVertexX)[i],(*particleVertexY)[i]);
       auto Pt = hypot((*particleMomentumX)[i],(*particleMomentumY)[i]);
       auto P = sqrt((*particleMomentumX)[i]*(*particleMomentumX)[i]+(*particleMomentumY)[i]*(*particleMomentumY)[i]+
		       (*particleMomentumZ)[i]*(*particleMomentumZ)[i]);
       auto PDG = (*particlePDG)[i];

       auto r=std::sqrt((*particleVertexX)[i]*(*particleVertexX)[i]+(*particleVertexY)[i]*(*particleVertexY)[i]+
		       (*particleVertexZ)[i]*(*particleVertexZ)[i]);
       auto phi = std::acos((*particleVertexZ)[i]/r); 
       //std::cout<<"PDG is"<<PDG<<std::endl;
       //std::cout<<"particleVertexX is"<<(*particleVertexX)[i]<<std::endl;
       //std::cout<<"particleVertexY is"<<(*particleVertexY)[i]<<std::endl;
       //std::cout<<"particleVertexZ is"<<(*particleVertexZ)[i]<<std::endl;
       if(abs(PDG)==2212)//2212为质子，211为pi介子
       {
         v_TH2F[0]->Fill(Vxy,P);
         v_TH2F[1]->Fill(Vxy,Pt);
	 v_TH2F[4]->Fill((*particleVertexZ)[i],P);
         v_TH2F[5]->Fill((*particleVertexZ)[i],Pt);
	 v_TH2F[8]->Fill(Vxy,phi);
	 v_TH2F[9]->Fill((*particleVertexZ)[i],phi);
       } 
       else if(abs(PDG)==211)
       {
	 v_TH2F[2]->Fill(Vxy,P);
         v_TH2F[3]->Fill(Vxy,Pt);
         v_TH2F[6]->Fill((*particleVertexZ)[i],P);
         v_TH2F[7]->Fill((*particleVertexZ)[i],Pt);
	 v_TH2F[10]->Fill(Vxy,phi);
	 v_TH2F[11]->Fill((*particleVertexZ)[i],phi);
       }
       else
       {continue;}
     }     
     ientry++;
  }
  
  for(size_t i=0;i<nhisnames;i++)
  {
    v_canvas[i]->cd();
    v_TH2F[i]->GetXaxis()->SetTitle(XaxisTitle[i].c_str());
    v_TH2F[i]->GetYaxis()->SetTitle(YaxisTitle[i].c_str());
    v_TH2F[i]->Draw("COLZ");
  }
}

