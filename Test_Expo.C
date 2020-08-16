// Program to test the QIE simulation class
#include"SimQIE.h"

void Plot_Pulse();		// Plot current vs time for a single pulse
void Integration();		// Integrate a single pulse
void I2Q();			// simulating current pulse to ADC
void TDC_Check();		// Plot TDC vs. pulse delay
void Plot_CapID();		// To plot capacitor ID

bool DoSave=false;		// whether to print graphs and histograms

void SaveMe(TCanvas*,TString);		// To save TCanvas as pdf & png
void Good_g(TGraph* gg,int mc, int mst=21, int msz=1);

void Test_Expo()
{
  Plot_Pulse();
  // Integration();
  // I2Q();
  // TDC_Check();
  // Plot_CapID();
}

void Plot_Pulse()
{
  Expo bm(1,3);
  vector<float>TT,PP;		// time, signal

  for(float i=0; i<100; i++){
    TT.push_back(i/5);
    PP.push_back(bm.eval(i/5));
  }

  TGraph* tg = new TGraph(TT.size(),&TT[0],&PP[0]);
  TCanvas* tc = new TCanvas("aa","bb",800,600);
  tg->Draw("apl");
  tg->SetTitle("PMT pulse;Time;Signal");
  tg->SetMarkerStyle(21);

  SaveMe(tc, "Pulse_shape");
  if(DoSave) delete tc;
}

void Integration()
{
  // Expo bm(1,3);
  Expo bm(1,3,0,7);		// integral = 7
  vector<float>TT,PP;		// time, signal

  for(float i=0; i<100; i++){
    TT.push_back(i/5);
    PP.push_back(bm.Integrate(0,i/5));
  }

  TGraph* tg = new TGraph(TT.size(),&TT[0],&PP[0]);
  TCanvas* tc = new TCanvas("aa","bb",800,600);
  tg->Draw("apl");
  tg->SetTitle("Integrating the pulse;Time;Charge collected");
  tg->SetMarkerStyle(21);
  tc->SetGrid();

  SaveMe(tc, "Integral");
  if(DoSave) delete tc;
}


void I2Q()
{
  SimQIE smq;			// QIE simulation instance
  float rt = 5.7;		// common rise time
  float ft = 7.3;		// common fall time
  float Q0 = 3250;		// Net charge in the pulse
  
  Bimoid bm(0,rt,ft,3250);
  vector<float>TT,PP;		// time, signal
  for(float i=0; i<100; i++){
    TT.push_back(i);
    PP.push_back(bm.eval(i));
  }

  TGraph* tg = new TGraph(TT.size(),&TT[0],&PP[0]);
  TCanvas* tc = new TCanvas("aa","bb",800,600);
  tg->Draw("apl");
  TLatex tll;
  tll.DrawLatex(65,160,"rise time = 5.7ns");
  tll.DrawLatex(65,150,"fall time = 7.3ns");
  tll.DrawLatex(65,140,"Total charge = 3250");
  tg->SetTitle("Input pulse;Time [ns];Signal");
  tg->SetMarkerStyle(21);

  // Integrate and find ADC
  cout<<"Actual charge = "<<Q0<<"\nintended ADC = "<<smq.Q2ADC(Q0)<<endl;
  cout<<"t1\tt2\tCharge\tADC\n";
  int* OP1 = smq.Out_ADC(&bm,10);
  for(int i=0;i<10;i++) cout<<OP1[i]<<endl;
  SaveMe(tc,"Random_Pulse");
  if(DoSave) delete tc;
}

void TDC_Check()
{
  vector<float> dly,tdc;
  SimQIE smq;
  float rt = 0.9;		// common rise time
  float ft = 2.1;		// common fall time
  float thr = 3.4;		// TDC threshold

  for(float i=-5;i<30;i+=0.1){
    // Bimoid bm(i,rt,ft);
    Bimoid bm(i,rt,ft,1000);
    dly.push_back(i);
    // tdc.push_back(smq.TDC((Bimoid)bm));
    tdc.push_back(smq.TDC(&bm));
  }

  TGraph* tg = new TGraph(dly.size(),&dly[0],&tdc[0]);
  TCanvas* tc = new TCanvas("aa","bb",800,600);
  tg->Draw("ap");
  TLatex tll;
  tll.DrawLatex(14,22,"rise time = 0.9ns");
  tll.DrawLatex(14,18,"fall time = 2.1ns");
  tll.DrawLatex(14,14,"TDC threshold = 3.4ns");
  tg->SetTitle("TDC closure test;Pulse delay [ns];TDC");
  tg->SetMarkerStyle(21);
  tg->SetMarkerSize(0.5);
  tc->SetGrid();

  SaveMe(tc,"TDC_Check");
  if(DoSave) delete tc;
}

void Plot_CapID()
{
  SimQIE smq;
  Bimoid* bm = new Bimoid(0,0.9,2.1);
  int* capid = smq.CapID(bm,10);
  TGraph* tg = new TGraph(10);
  for(int i=0;i<10;i++) tg->SetPoint(i,i,capid[i+1]);
  TCanvas* tc = new TCanvas("aa","bb",800,600);
  tg->Draw("apl");
  Good_g(tg,1);
  tg->SetTitle("capacitance ID;Time sample; CapID");
  SaveMe(tc,"CapID");
}

void SaveMe(TCanvas* cc, TString fname)
{
  if(DoSave){
    cc->SaveAs("PDF_OUT/Expo_"+fname+".pdf");
    cc->SaveAs("PNG_OUT/Expo_"+fname+".png");
  }
}

void Good_g(TGraph* gg,int mc, int mst=21, int msz=1)
{
  gg->SetMarkerStyle(mst);
  gg->SetMarkerColor(mc);
  gg->SetMarkerSize(msz);
}
