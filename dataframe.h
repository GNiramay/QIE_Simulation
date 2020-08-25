// Class dataframe: to organise output of QIE per event
// Class digiCollection: to store collect digis per event
#include"SimQIE.h"

class dataframe
{
public:
  dataframe(int maxTS_,Pulse* pl);

  int* GetADC(){return(ADCs);}
  int* GetTDC(){return(TDCs);}
  int* GetCID(){return(CIDs);}
  
// private:
  int maxTS;			// no. of time samples stored
  int chanID;			// channel ID

  int* ADCs;			// analog to digital counts
  int* TDCs;			// Time to Digital counts
  int* CIDs;			// capacitor IDs
};

class digiCollection
{
public:
  digiCollection();
  
  vector<dataframe> digis;
};
/////////////////////////////////////////////////////////////////////////

dataframe::dataframe(int maxTS_,Pulse* pl)
{
  SimQIE* smq = new SimQIE();
  maxTS = maxTS_;
  ADCs = smq->Out_ADC(pl,maxTS);
  TDCs = smq->Out_TDC(pl,maxTS);
  CIDs = smq->CapID(pl,maxTS);
}

digiCollection::digiCollection(){}
