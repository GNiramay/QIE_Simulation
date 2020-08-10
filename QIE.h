// Program, exact copy of QIE.py, but written in C++
class targetScint
{
public:
  float range0offsets[4] = {-16.,34.,158.,419.};
  float range0slope[4] = {3.1,6.2,12.4,24.8};
  float range1offsets[4] = {517.,915.,1910.,3990.};
  float range1slope[4] = {24.8,49.6,99.2,198.4};
  float range2offsets[4] = {4780.,7960.,15900.,32600.};
  float range2slope[4] = {198.4,396.8,793.6,1587.};
  float range3offsets[4] = {38900.,64300.,128000.,261000.};        
  float range3slope[4] = {1587.,3174.,6349.,12700.};
  int bins[4] = {0,16,36,57};

  int Q2ADC(float Q);
  float ADC2Q(float adc_);
  float Q_Err(float Q, bool DEBUG=false);

private:
  float Nt = 1600;   // number of pixels in SiPM
  float f = 46.e6;   // frequency of integration
  float beta = 200.; // product of photon detection efficiency and photons produced per MIP in scinitllator
  float G = 1.e4;    // gain of sipm

  float* offsets[4] = {range0offsets,range1offsets,range2offsets,range3offsets};
  float* slopes[4] = {range0slope,range1slope,range2slope,range3slope};

};

int targetScint::Q2ADC(float Q)
{
  if(Q >= offsets[3][3]+7*slopes[3][3]) return(255);
  if(Q < offsets[0][0]) return(0);
  for(int i=3;i>-1;i--){	// Loop over ranges
    if(Q >= offsets[i][0]){
      for(int j=3;j>-1;j--){	// Loop over subranges
	if(Q >= offsets[i][j]) return 64*(i)+bins[j]+floor((Q-offsets[i][j])/slopes[i][j]);
      }
    }
  }  
  return(999);
}

float targetScint::ADC2Q(float adc_)
{
  float ADC = adc_;
  if(ADC <= 0) return(-16.);
  if(ADC >= 255) return(350000.);
  int rangei = floor(ADC/64.);
  if(rangei > 0) ADC = ((int)ADC)%(rangei*64);
  for(int i=3;i>-1;i--){
    if(ADC > bins[i]){
      return(offsets[rangei][i]+(ADC-bins[i])*slopes[rangei][i]);
    }
    return(offsets[rangei][0]+ADC*slopes[rangei][0]);
  }
  return(-100);
}

float targetScint::Q_Err(float Q, bool DEBUG=false)
{
  if(Q >= offsets[3][3]+7*slopes[3][3]) return(0);
  if(Q < offsets[0][0]) return(0);
  if(Q >= offsets[3][3]){
    return((7*slopes[3][3])/(sqrt(3)*(2*offsets[3][3]+7*slopes[3][3])));
  }
  
  float All_Off[16];		// array of all the offsets of 16 small ranges
  float All_Slp[16];		// array of all the slopes of 16 small ranges

  if(DEBUG) cout<<"Inside Q_Err()\n";
  for(int i=0;i<4;i++){
    for(int j=0;j<4;j++){
      All_Off[4*i+j] = offsets[i][j];
      All_Slp[4*i+j] = slopes[i][j];
      if(DEBUG==true) cout<<"i= "<<i<<"\tj= "<<j<<"\t4i+j= "<<4*i+j<<endl;
    }
  }

  for(int i=14;i>=0;i--){
    if(Q>=All_Off[i]){
      // return((All_Off[i+1]-All_Off[i])/(sqrt(3)*(All_Off[i+1]+All_Off[i])));
      // return((All_Off[i+1]-Q)/(sqrt(3)*(All_Off[i+1]+All_Off[i])));
      return((All_Off[i+1]-All_Off[i])/(sqrt(12)*Q)); // This one is very close to the figure in the paper
      // return((Q-All_Off[i])/(Q*All_Slp[i]));
    }
  }
  return(999);
}
