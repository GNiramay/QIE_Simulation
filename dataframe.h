

class dataframe{

 public: 
  dataframe(int maxTS_);

  vector<int> getADC;
  vector<int> getTDC;

  int maxTS;
  int chanID;
  vector<int> ADCs=vector<int>(0,maxTS);
  vector<int> TDCs;
  vector<int> capIDs;

}

class digiCollection{

 public:
  digiCollection();
  
  vector<dataframe> digis;

}
