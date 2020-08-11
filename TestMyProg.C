// Program to test the QIE simulation class
#include"SimQIE.h"
#include"QIE.h"
// #include"Pulse.h"

void FADC_resp();		// Plot ADC vs charge
void Q_Error();			// Plot Quantization error vs charge
void Plot_Pulse();		// Plot current vs time for a single pulse
void Pulse_train();		// Plot superposition of multiple pulses
void Integration();		// Integrate a single pulse
void I2Q();			// simulating current pulse to ADC
void TDC_Check();		// Plot TDC vs. pulse delay
void Q2Q();			// closure test. do Q2ADC and then ADC to Q

bool DoSave=true;		// whether to print graphs and histograms

void SaveMe(TCanvas*,TString);		// To save TCanvas as pdf & png
void Good_g(TGraph* gg,int mc, int mst=21, int msz=1);

void TestMyProg()
{
  // FADC_resp();
  // Q_Error();
  // Plot_Pulse();
  // Pulse_train();
  // Integration();
  // I2Q();
  // TDC_Check();
  Q2Q();
}

void FADC_resp()
{
  vector<float> QQ,adc;
  targetScint tsc;
  // SimQIE smq;			// without noise
  // SimQIE smq(10,3);		// with noise. mu=10,sigma=3
  SimQIE smq(200,8);		// with noise. mu=200,sigma=8
  
  for(int i=10;i<350000;i+=10){
    float charge = i;
    QQ.push_back(charge);

    // adc.push_back(tsc.Q2ADC(charge));
    adc.push_back(smq.Q2ADC(charge));
  }

  TGraph* gg = new TGraph(QQ.size(),&QQ[0],&adc[0]);
  TCanvas* tc = new TCanvas("aa","bb",800,600);

  gg->Draw("ap");
  gg->SetMarkerStyle(20);
  gg->SetTitle("QIE simulation - closure test;Charge [fC];ADC");
  gg->SetMarkerSize(0.4);
  
  tc->SetLogx();
  // tc->SetLogy();
  tc->SetGrid();

  SaveMe(tc,"temp_test");
  delete tc;
}

void Q_Error()
{
  vector<float> QQ,Err;
  targetScint tsc;
  SimQIE smq;
  
  for(int i=10;i<350000;i+=10){
    float charge = i;
    QQ.push_back(charge);
    // Err.push_back(tsc.Q_Err(charge));
    Err.push_back(smq.QErr(charge));
  }
  
  TGraph* gg = new TGraph(QQ.size(),&QQ[0],&Err[0]);
  TCanvas* tc = new TCanvas("aa","bb",800,600);
  gg->Draw("apl");
  gg->SetTitle("QIE Quantization error;Charge [fC];Error");

  gg->SetLineWidth(2);
  Good_g(gg,2);

  tc->SetLogx();
  tc->SetLogy();
  tc->SetGrid();

  SaveMe(tc,"temp_test_QErr");
  delete tc;
}

void Plot_Pulse()
{
  Pulse pl(0,0.9,2.1);
  pl.RandInit();
  vector<float>TT,PP;		// time, signal

  for(float i=0; i<100; i++){
    TT.push_back(i/5);
    PP.push_back(pl.eval(i/5));
  }

  TGraph* tg = new TGraph(TT.size(),&TT[0],&PP[0]);
  TCanvas* tc = new TCanvas("aa","bb",800,600);
  TText* tt1 =  new TLatex(14,0.2,"Rise time = 0.9s");
  TText* tt2 =  new TLatex(14,0.18,"fall time = 2.1s");
  TText* tt3 =  new TLatex(8.,0.14,"#font[12]{f(t) = #frac{1}{(t_{2}-t_{1})log2}(#frac{1}{1+exp(#frac{t-t_{0}}{t_{1}})} - #frac{1}{1+exp(#frac{t-t_{0}}{t_{2}})})}");
  tg->Draw("apl");
  tt1->Draw("same");
  tt2->Draw("same");
  // tt3->Draw("same");
  tg->SetTitle("PMT pulse;Time;Signal");
  tg->SetMarkerStyle(21);

  SaveMe(tc, "Input_1_Pulse");
  delete tc;
}

void Pulse_train()
{
  PulseGen psg(0.,0.9,2.1,5,6);
  vector<float>TT,PP;		// time, signal

  for(float i=0; i<50; i+=0.1){
    TT.push_back(i);
    PP.push_back(psg.PeriodicGen(i));
  }

  TGraph* tg = new TGraph(TT.size(),&TT[0],&PP[0]);
  TCanvas* tc = new TCanvas("aa","bb",800,600);
  tg->Draw("apl");
  tg->SetTitle("Train of 6 pulses;Time;Signal");
  // tg->SetMarkerStyle(21);

  SaveMe(tc, "Pulse_6_Train");
  delete tc;
}

void Integration()
{
  // Pulse pl(0,0.9,2.1);
  Pulse pl(0,0.9,2.1,7);	// charge = 7
  vector<float>TT,PP;		// time, signal

  for(float i=0; i<100; i++){
    TT.push_back(i/5);
    PP.push_back(pl.Integrate(0,i/5));
  }

  TGraph* tg = new TGraph(TT.size(),&TT[0],&PP[0]);
  TCanvas* tc = new TCanvas("aa","bb",800,600);
  TText* tt1 =  new TLatex(10,0.6,"Rise time=0.9s");
  TText* tt2 =  new TLatex(10,0.5,"fall time=2.1s");
  tg->Draw("apl");
  tt1->Draw("same");
  tt2->Draw("same");
  tg->SetTitle("Integrating the pulse;Time;Charge collected");
  tg->SetMarkerStyle(21);
  tc->SetGrid();

  SaveMe(tc, "Charge_Gathered");
  delete tc;
}


void I2Q()
{
  // 1. Plot the pulse that Imma integrate
  // 2. Integrate it for every 25 ns
  // 3. use the integrated charge to give ADCs
  
  SimQIE smq;			// QIE simulation instance
  float rt = 5.7;		// common rise time
  float ft = 7.3;		// common fall time
  float Q0 = 3250;		// Net charge in the pulse

  // Plot the pulse
  Pulse pl(0,rt,ft,Q0);
  // pl.RandInit();		// start it at random time
  vector<float>TT,PP;		// time, signal
  for(float i=0; i<100; i++){
    TT.push_back(i);
    PP.push_back(pl.eval(i));
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
  for(float time=25;pl.eval(time)>0.01;time+=25){
    cout<<time-25<<"\t"<<time<<"\t"<<pl.Integrate(time-25,time)<<"\t"<<smq.Q2ADC(pl.Integrate(time-25,time))<<endl;
  }

  SaveMe(tc,"Random_Pulse");
  delete tc;
}

void TDC_Check()
{
  vector<float> dly,tdc;
  SimQIE smq;
  float rt = 0.9;		// common rise time
  float ft = 2.1;		// common fall time
  float thr = 3.4;		// TDC threshold

  for(float i=-5;i<30;i+=0.1){
    // Pulse pls(i,rt,ft);
    Pulse pls(i,rt,ft,1000);
    dly.push_back(i);
    tdc.push_back(smq.TDC(pls));
    // tdc.push_back(smq.TDC(pls,thr));
    // // if(smq.TDC(pls,thr)>0) cout<<"i= "<<i<<"TDC = "<<smq.TDC(pls,thr)<<endl;
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
  // delete tc;
}

void Q2Q()
{
  TCanvas* tc = new TCanvas("aa","bb",600,600);
  tc->GetPad(0)->SetMargin(0.12,0.1,0.1,0.1);
  vector<float> Q0,Q1,adc;
  SimQIE smq;

  for(int i=10;i<350000;i+=10){
    float charge = i;
    Q0.push_back(charge);
    adc.push_back(smq.Q2ADC(charge));
    Q1.push_back(smq.ADC2Q((int)smq.Q2ADC(charge)));
  }
  TGraph* gg = new TGraph(Q0.size(),&Q0[0],&Q1[0]);
  TGraph* g2 = new TGraph(Q0.size(),&adc[0],&Q1[0]);

  // g2->Draw("ap");
  // g2->SetMarkerStyle(20);
  // g2->SetTitle("QIE simulation - closure test;ADC;charge [fC]");
  // g2->SetMarkerSize(0.4);

  gg->Draw("ap");
  // gg->SetMarkerStyle(20);
  gg->SetTitle("QIE simulation - closure test;Input Charge [fC];Output Charge [fC]");
  gg->SetMarkerSize(0.4);
  
  tc->SetGrid();
  // SaveMe(tc,"Q2Q");
  // delete tc;
}

void SaveMe(TCanvas* cc, TString fname)
{
  if(DoSave){
    cc->SaveAs("PDF_OUT/"+fname+".pdf");
    cc->SaveAs("PNG_OUT/"+fname+".png");
  }
}

void Good_g(TGraph* gg,int mc, int mst=21, int msz=1)
{
  gg->SetMarkerStyle(mst);
  gg->SetMarkerColor(mc);
  gg->SetMarkerSize(msz);
}
