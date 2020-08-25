// Program to test the classes dataframe, digiCollection
#include"dataframe.h"

void Test_dataframe()
{
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
      Expo* ex = new Expo(2,10,0,PE);
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
  tf->Close();
}
