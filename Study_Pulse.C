// To do:
// 1. find max of given pulse
// 2. find rise time (t at which f(t) = 0.9* max f)
// 3. repeat the process for different parameters rt, ft
// 4. plot all the graphs and save them in a root file

#include"Pulse.h"

void Good_g(TGraph* gg, TString, TString);
void Everything();
void ReverseFit();
void Stepwise();

void Study_Pulse()
{
  // Everything();
  // Stepwise();
  ReverseFit();
}

void ReverseFit()
{
  vector<float>rt,ft,RtFt,RiseTime,FallTime; // RtFt = rt*ft

  // TProfile* p_rt= new TProfile("p_rt","avg. Rise Time vs rt.ft;log(rt.ft);<log(Rise Time)>",1000,log(0.02),log(1.99*3.99+0.01));
  // TProfile* p_ft= new TProfile("p_ft","avg. Fall Time vs rt.ft;log(rt.ft);<log(Fall Time)>",1000,log(0.02),log(1.99*3.99+0.01));

  // TH2F* h_rt = new TH2F("h_rt","",300,0,3.01,300,2,5.01);
  // TH2F* h_ft = new TH2F("h_ft","",300,0,3.01,300,2,5.01);

  for(float i=1;i<400;i+=5){
    for(float j=400;j<800;j+=5 ){
      Pulse pl(0,i/100,j/100);
      float max = pl.MyMax();
      float val09 = 0.9*pl.eval(max);
      float val01 = 0.1*pl.eval(max);
      
      // finding rise time
      float rr,ff;
      for(float tt=0;pl.eval(tt)<=val09;tt+=0.001) rr=tt;
      for(float tt=25;pl.eval(tt)<=val01;tt-=0.001) ff=tt;

      // fill vectors
      rt.push_back(i/100);
      ft.push_back(j/100);
      // RtFt.push_back(i*j/10000);
      RtFt.push_back((rr+0.0005)*(ff-0.0005));
      RiseTime.push_back(rr+0.0005);
      FallTime.push_back(ff-0.0005);

      // p_rt->Fill(log(i*j/10000),log(rr+0.0005));
      // p_ft->Fill(log(i*j/10000),log(ff-0.0005));
    }
  }

  TFile* tf = new TFile("temp_3_Param_corr.root","RECREATE");
  TGraph* RT_1 = new TGraph(rt.size(),&RiseTime[0],&rt[0]); Good_g(RT_1,"RT_1","rt vs Rise Time;Rise Time;rt"); RT_1->Write();
  TGraph* RT_2 = new TGraph(rt.size(),&FallTime[0],&rt[0]); Good_g(RT_2,"RT_2","rt vs Fall Time;Fall Time;rt"); RT_2->Write();
  TGraph* RT_3 = new TGraph(rt.size(),&RtFt[0],&rt[0]); Good_g(RT_3,"RT_3","rt vs (Rise).(Fall);(Rise Time).(Fall Time);rt"); RT_3->Write();
  TGraph2D* RT_4= new TGraph2D(rt.size(),&FallTime[0],&RiseTime[0],&rt[0]); RT_4->SetName("RT_4"); RT_4->Write();

  TGraph* FT_1 = new TGraph(ft.size(),&RiseTime[0],&ft[0]); Good_g(FT_1,"FT_1","ft vs Rise Time;Rise Time;ft"); FT_1->Write();
  TGraph* FT_2 = new TGraph(ft.size(),&FallTime[0],&ft[0]); Good_g(FT_2,"FT_2","ft vs Fall Time;Fall Time;ft"); FT_2->Write();
  TGraph* FT_3 = new TGraph(ft.size(),&RtFt[0],&ft[0]); Good_g(FT_3,"FT_3","ft vs (Rise).(Fall);(Rise Time).(Fall Time);ft"); FT_3->Write();
  TGraph2D* FT_4= new TGraph2D(rt.size(),&FallTime[0],&RiseTime[0],&ft[0]); FT_4->SetName("FT_4"); FT_4->Write();

  tf->Close();
}


void Good_g(TGraph* gg,TString hname,TString ttl)
{
  // gg->SetMarkerStyle(21);
  gg->SetMarkerColor(1);
  gg->SetNameTitle(hname,ttl);
  gg->SetLineColor(0);
}

void Everything()
{
  vector<float>rt,ft,RtFt,RiseTime,FallTime; // RtFt = rt*ft

  TH2F* h_rt = new TH2F("h_rt","",200,0,2.01,200,2,4.01);
  TH2F* h_ft = new TH2F("h_ft","",200,0,2.01,200,2,4.01);
  TH2F* h_corr= new TH2F("h2_r_f","Rise time , fall time correlation;Rise time;Fall time",100,0,10,100,0,10);

  // TProfile* p_rt= new TProfile("p_rt","avg. Rise Time vs rt.ft;log(rt.ft);<log(Rise Time)>",1000,log(0.02),log(1.99*3.99+0.01));
  // TProfile* p_ft= new TProfile("p_ft","avg. Fall Time vs rt.ft;log(rt.ft);<log(Fall Time)>",1000,log(0.02),log(1.99*3.99+0.01));

  // TH2F* h_rt = new TH2F("h_rt","",300,0,3.01,300,2,5.01);
  // TH2F* h_ft = new TH2F("h_ft","",300,0,3.01,300,2,5.01);

  for(float i=1;i<200;i++){
    for(float j=200;j<400;j++ ){
      Pulse pl(0,i/100,j/100);
      float max = pl.MyMax();
      float val09 = 0.9*pl.eval(max);
      
      // finding rise time
      float rr,ff;
      for(float tt=0;pl.eval(tt)<=val09;tt+=0.001) rr=tt;
      for(float tt=25;pl.eval(tt)<=val09;tt-=0.001) ff=tt;

      // fill vectors
      rt.push_back(i/100);
      ft.push_back(j/100);
      RtFt.push_back(i*j/10000);
      RiseTime.push_back(rr+0.0005);
      FallTime.push_back(ff-0.0005);

      h_rt->Fill(i/100,j/100,rr+0.0005);
      h_ft->Fill(i/100,j/100,ff-0.0005);
      h_corr->Fill(rr+0.0005,ff-0.0005);

      // p_rt->Fill(log(i*j/10000),log(rr+0.0005));
      // p_ft->Fill(log(i*j/10000),log(ff-0.0005));
    }
  }

  TFile* tf = new TFile("temp_3_Param_corr.root","RECREATE");
  TGraph* g_rt_rt = new TGraph(rt.size(),&rt[0],&RiseTime[0]); Good_g(g_rt_rt,"g_rt_rt","Rise time vs rt"); g_rt_rt->Write();
  TGraph* g_ft_rt = new TGraph(rt.size(),&rt[0],&FallTime[0]); Good_g(g_ft_rt,"g_ft_rt","Rise time vs ft"); g_ft_rt->Write();
  TGraph* g_rt_ft = new TGraph(rt.size(),&ft[0],&RiseTime[0]); Good_g(g_rt_ft,"g_rt_ft","Fall time vs rt"); g_rt_ft->Write();
  TGraph* g_ft_ft = new TGraph(rt.size(),&ft[0],&FallTime[0]); Good_g(g_ft_ft,"g_ft_ft","Fall time vs ft"); g_ft_ft->Write();
  TGraph2D* g2_rt = new TGraph2D(rt.size(),&rt[0],&ft[0],&RiseTime[0]);

  TGraph* g_rt_RtFt = new TGraph(rt.size(),&RtFt[0],&RiseTime[0]); Good_g(g_rt_RtFt,"g_rt_RtFt","Rise time vs rt*ft"); g_rt_RtFt->Write();
  TGraph* g_ft_RtFt = new TGraph(rt.size(),&RtFt[0],&FallTime[0]); Good_g(g_ft_RtFt,"g_ft_RtFt","Fall time vs rt*ft"); g_ft_RtFt->Write();

  h_rt->Write();
  h_ft->Write();
  h_corr->Write();

  tf->Close();
}


void Stepwise()
{
  vector<float>rt,ft,RtFt,RiseTime,FallTime; // RtFt = rt*ft

  for(float i=1;i<200;i++){
    Pulse pl(0,i/100,5);	      // define pulse
    float max = pl.MyMax();	// location of maximum
    float val09 = 0.9*pl.eval(max); // value of 0.9f(max)
    
    // finding rise time
    float rr,ff;
    for(float tt=0;pl.eval(tt)<=val09;tt+=0.001) rr=tt;
    for(float tt=25;pl.eval(tt)<=val09;tt-=0.001) ff=tt;

    // fill vectors
    rt.push_back(i/100);
    RiseTime.push_back(rr+0.0005);
    FallTime.push_back(ff+0.0005);
  }

  TFile* tf = new TFile("temp_2_Param_corr.root","RECREATE");
  TGraph* g_rt_rt = new TGraph(rt.size(),&rt[0],&RiseTime[0]); Good_g(g_rt_rt,"g_rt_rt","Rise time vs rt"); g_rt_rt->Write();
  TGraph* g_ft_rt = new TGraph(rt.size(),&rt[0],&FallTime[0]); Good_g(g_ft_rt,"g_ft_rt","Rise time vs ft"); g_ft_rt->Write();
  tf->Close();
}
