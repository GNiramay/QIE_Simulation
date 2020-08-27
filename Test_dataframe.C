// Program to test the classes dataframe, digiCollection
#include"dataframe.h"

void MakeTree();
void PlotGraphs(TTree*);
void FixnPrint(TH1F* hh,TString fname);
void FixnPrint(TH2F* hh,TString fname,bool IsLogZ=false);

void Test_dataframe()
{
  MakeTree();
}

void MakeTree()
{
  gStyle->SetStatX(0.9);
  gStyle->SetStatY(0.9);
  
  ifstream ii1("TXT_IN/nd.txt");
  ifstream ii2("TXT_IN/pe.txt");
  TFile* tf = new TFile("ROOT_OUT/TempTrial.root","RECREATE");
  TTree* tr = new TTree("QIE","QIE output");
  int ND;			// no. of hits
  float PE;			// corresponding photo electrons

  int maxTS=5;			// no. of time samples stored
  int* ADC = new int[maxTS];
  int* TDC = new int[maxTS];
  int* CID = new int[maxTS];
  // //////////////// To write the tree
  tr->Branch("maxTS",&maxTS,"maxTS/I");
  tr->Branch("ADC",ADC,"ADC[maxTS]/I");
  tr->Branch("TDC",TDC,"TDC[maxTS]/I");
  tr->Branch("CID",CID,"CID[maxTS]/I");

  while(ii1>>ND){
    for(int i=0;i<ND;i++){
      ii2>>PE;
      // Expo* ex = new Expo(2,35,0,PE); // This is recommended
      Expo* ex = new Expo(0.1,35,0,PE);
      dataframe* dt = new dataframe(maxTS,ex);

      int* ADC_ = dt->GetADC();
      int* TDC_ = dt->GetTDC();
      int* CID_ = dt->GetCID();

      for(int j=0;j<maxTS;j++){
	ADC[j]=ADC_[j];
	TDC[j]=TDC_[j];
	CID[j]=CID_[j];
      }
      tr->Fill();
    }
  }
  tr->Write();
  PlotGraphs(tr);

  tf->Close();
}

void PlotGraphs(TTree* T)
{
  SimQIE sm;			// to use ADC2Q function
  sm.SetGain();
  
  int nn = T->GetEntries();
  ifstream aa("TXT_IN/pe.txt");
  TFile* ff = new TFile("ROOT_OUT/dataframe_Graphs.root","RECREATE");
  TH1F* h_pe = new TH1F("h_pe","raw no. of PEs",500,0,500);
  TH1F* h_adc = new TH1F("h_adc","raw ADC distribution",256,0,256);
  TH1F* h_tdc = new TH1F("h_tdc","TDC distribution",64,0,64);
  TH1F* h_lin = new TH1F("h_lin","Charge reconstructed from ADCs",500,0,500);

  TH2F* h2_adtd=new TH2F("h2_adtd","TDC vs. ADC",256,0,256,64,0,64);
  TH2F* h2_Q2Q = new TH2F("h2_Q2Q","reco PE vs. raw PE;raw PE;Reco PE",500,0,500,500,0,500);
  
  int maxTS=5;			// no. of time samples stored
  int* ADC = new int[maxTS];
  int* TDC = new int[maxTS];
  int* CID = new int[maxTS];

  T->SetBranchAddress("ADC",ADC);
  T->SetBranchAddress("TDC",TDC);
  T->SetBranchAddress("CID",CID);

  int PE;
  // while(aa>>PE) h_pe->Fill(PE);

  for(int i=0;i<nn && aa>>PE;i++){
    T->GetEntry(i);
    for(int j=0;j<maxTS;j++){
      h_adc->Fill(ADC[j]);
      h_tdc->Fill(TDC[j]);
      // h_lin->Fill(sm.ADC2Q(ADC[j]));
      h2_adtd->Fill(ADC[j],TDC[j]);
    }
    h_lin->Fill(sm.ADC2Q(ADC[0]));
    h_pe->Fill(PE);
    h2_Q2Q->Fill(PE,sm.ADC2Q(ADC[1]));
  }

  // FixnPrint(h_adc,"raw_ADC");
  // FixnPrint(h_tdc,"raw_TDC");
  // FixnPrint(h_pe,"raw_PE");
  // FixnPrint(h2_adtd,"raw_ad_vs_td",true);

  ff->Write();
  ff->Close();

}

void FixnPrint(TH1F* hh,TString fname)
{
  TCanvas* tc = new TCanvas("aa","bb",800,600);
  hh->SetFillColor(5);
  hh->SetLineColor(1);
  hh->SetFillStyle(1001);
  if(fname=="raw_PE") hh->Rebin();
  hh->Draw("hist");

  tc->SetLogy();

  tc->SaveAs("PNG_OUT/"+fname+".png");
  tc->SaveAs("PDF_OUT/"+fname+".pdf");
  delete tc;
}

void FixnPrint(TH2F* hh,TString fname,bool IsLogZ=false)
{
  TCanvas* tc = new TCanvas("aa","bb",800,600);
  hh->Draw("colz");

  if(IsLogZ) tc->SetLogz();
  tc->SaveAs("PNG_OUT/"+fname+".png");
  tc->SaveAs("PDF_OUT/"+fname+".pdf");
  delete tc;
}
