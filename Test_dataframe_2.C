// Program to test the classes dataframe, digiCollection
#include"dataframe.h"

void MakeTree();
void MakeHists(TString);
void FixnPrint(TH1F* hh,TString fname);
void FixnPrint(TH2F* hh,TString fname,bool IsLogZ=false);

void Test_dataframe_2()
{
  MakeTree();
  // MakeHists("ROOT_OUT/TempTrial_2.root");
}

void MakeTree()
{
  ifstream ii1("TXT_IN/nd.txt");
  ifstream ii2("TXT_IN/pe.txt");
  ifstream ii3("TXT_IN/barID.txt");
  TFile* tf = new TFile("ROOT_OUT/TempTrial_2.root","RECREATE");

  SimQIE sm;			// to use ADC2Q function
  sm.SetGain();

  Expo* temp = new Expo(0.1,5,30,1); // Trial 2
  vector<float>t,x;
  for(float i=0;i<100;i+=0.1){
    t.push_back(i);
    x.push_back(temp->eval(i));
  }

  /////////////////////// Defining Onjects //////////////////////////////
  TTree* tr = new TTree("QIE","QIE output");
  tf->mkdir("ADC_Hists");
  tf->mkdir("TDC_Hists");
  TGraph* g_pulse = new TGraph(t.size(),&t[0],&x[0]);
  g_pulse->SetTitle("Unit pulse shape;time[ns];current[micro A]");
  TH1F* h_adc[5];
  TH1F* h_tdc[5];
  TH1F* h_lin[5];

  TH1F* h_pe = new TH1F("h_pe","raw no. of PEs",500,0,500);
  TH1F* h_All_adc = new TH1F("h_All_adc","raw ADC distribution",256,0,256);
  TH1F* h_All_tdc = new TH1F("h_All_tdc","TDC distribution",64,0,64);
  TH1F* h_All_lin = new TH1F("h_All_lin","Total Reco. charge",500,0,500);

  TH2F* h2_adtd=new TH2F("h2_adtd","TDC vs. ADC",256,0,256,64,0,64);
  TH2F* h2_Q2Q[5];
  
  for(int i=0;i<5;i++){
    TString tt;tt.Form("%d",i);
    h_adc[i] = new TH1F("h_adc_"+tt,"raw ADC distribution - Time sample: "+tt,256,0,256);
    h_tdc[i] = new TH1F("h_tdc_"+tt,"TDC distribution - Time sample: "+tt,64,0,64);
    h_lin[i] = new TH1F("h_lin_"+tt,"Reconstructed charge - Time sample: "+tt,500,0,500);
    h2_Q2Q[i] = new TH2F("h2_Q2Q_"+tt,"reco PE vs. raw PE - Time sample: "+tt+";raw PE;Reco PE",500,0,500,500,0,500);
  }

  int ND;			// no. of hits
  float PE;			// corresponding photo electrons
  int barID;			// channel number

  const int N = 5;
  int maxTS=N;			// no. of time samples stored
  int ADC[50][N]={};
  int TDC[50][N]={};
  int CID[50][N]={};

  /////////////////////// Generating Branches ///////////////////////////
  tr->Branch("maxTS",&maxTS,"maxTS/I");
  for(int i=0;i<50;i++){
    TString id;id.Form("_%02d",i);
    tr->Branch("ADC"+id,ADC[i],"ADC"+id+"[maxTS]/I");
    tr->Branch("TDC"+id,TDC[i],"TDC"+id+"[maxTS]/I");
    tr->Branch("CID"+id,CID[i],"CID"+id+"[maxTS]/I");
  }

  /////////////////////// Fill all with only noise///////////////////////
  Expo* ex0 = new Expo(0,1,0,0);
  for(int i=0;i<50;i++){
    dataframe* dt = new dataframe(maxTS,ex0,6,1.5);
    int* ADC_ = dt->GetADC();
    int* TDC_ = dt->GetTDC();
    int* CID_ = dt->GetCID();

    for(int j=0;j<maxTS;j++){
      ADC[i][j]=ADC_[j];
      TDC[i][j]=TDC_[j];
      CID[i][j]=CID_[j];
    }
  }

  /////////////////////// Filling Branches //////////////////////////////
  while(ii1>>ND){
    for(int i=0;i<ND;i++){
      ii2>>PE;
      ii3>>barID;
      // Expo* ex = new Expo(0.1,35,0,PE); // Trial 1
      Expo* ex = new Expo(0.1,5,30,PE); // Trial 2
      // dataframe* dt = new dataframe(maxTS,ex); // without noise
      dataframe* dt = new dataframe(maxTS,ex,6,1.5); // with noise

      int* ADC_ = dt->GetADC();
      int* TDC_ = dt->GetTDC();
      int* CID_ = dt->GetCID();

      float RecoQ=0;
      for(int j=0;j<maxTS;j++){
  	ADC[barID][j]=ADC_[j];
  	TDC[barID][j]=TDC_[j];
  	CID[barID][j]=CID_[j];
	RecoQ+=sm.ADC2Q(ADC_[j]);
	
  	h_adc[j]->Fill(ADC_[j]);
  	h_tdc[j]->Fill(TDC_[j]);
  	h_lin[j]->Fill(sm.ADC2Q(ADC_[j]));

  	h_All_adc->Fill(ADC_[j]);
  	h_All_tdc->Fill(TDC_[j]);
  	// h_All_lin->Fill(sm.ADC2Q(ADC_[j]));

  	h2_adtd->Fill(ADC_[j],TDC_[j]);
  	h2_Q2Q[j]->Fill(PE,sm.ADC2Q(ADC_[j]));
      }
      h_All_lin->Fill(RecoQ);
      h_pe->Fill(PE);
    }
    tr->Fill();
  }
  for(int i=0;i<5;i++){
    tf->cd("ADC_Hists");
    h_adc[i]->Write();
    h_lin[i]->Write();
    h2_Q2Q[i]->Write();

    tf->cd("TDC_Hists");
    h_tdc[i]->Write();
  }
  tf->cd();

  h_pe->Write();
  h_All_adc->Write();
  h_All_tdc->Write();
  h_All_lin->Write();
  h2_adtd->Write();
  g_pulse->Write();
  tr->Write();
  tf->Close();
}

void MakeHists(TString ifname)
{
  SimQIE sm;			// to use ADC2Q function
  sm.SetGain();
  TFile* f0 = new TFile(ifname);
  TTree* T = (TTree*)f0->Get("QIE");

  int nn = T->GetEntries();
  ifstream aa("TXT_IN/pe.txt");

  TFile* ff = new TFile("ROOT_OUT/dataframe_Graphs.root","RECREATE");
  TH1F* h_pe = new TH1F("h_pe","raw no. of PEs",500,0,500);
  TH1F* h_All_adc = new TH1F("h_All_adc","raw ADC distribution",256,0,256);
  TH1F* h_All_tdc = new TH1F("h_All_tdc","TDC distribution",64,0,64);
  TH1F* h_All_lin = new TH1F("h_All_lin","Charge reconstructed from ADCs",500,0,500);

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
      h_All_adc->Fill(ADC[j]);
      h_All_tdc->Fill(TDC[j]);
      // h_All_lin->Fill(sm.ADC2Q(ADC[j]));
      h2_adtd->Fill(ADC[j],TDC[j]);
    }
    h_All_lin->Fill(sm.ADC2Q(ADC[0]));
    h_pe->Fill(PE);
    h2_Q2Q->Fill(PE,sm.ADC2Q(ADC[1]));
  }

  // FixnPrint(h_All_adc,"raw_ADC");
  // FixnPrint(h_All_tdc,"raw_TDC");
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
