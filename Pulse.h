// Class to store all the functions related to the input pulse to QIE
// Largely copied from generate_bimoid.py

class Pulse
{
public:
  Pulse(float start,float rise, float fall, float qq=1);
  Pulse(float start,float qq=1);
  float eval(float T);		// get the pulse value at time T
  float Integrate(float T1,float T2); // Integrate the pulse
  // float RandInit(float tlow,float thi);		// Initializae the pulse at a random time
  void RandInit(float tlow=0,float thi=100);		// Initializae the pulse at a random time
  float MyMax(); // To find maximum of the pulse
  float MyDer(float x);       // Derivative-like function for the pulse
private:
  float t0;			// starting time of the pulse
  float rt;			// rise time
  float ft;			// fall time
  float NC;			// Normalization constant
  float Q0;			// Net charge (integral I.dt)
};

class PulseGen
{
 public:
  PulseGen();			// Default constructor
  PulseGen(float t_start, float rise, float fall, float tgap, int npulses); // Define the parameters of the pulses
  vector<Pulse> pp0;	        // set of pulses
  float  PeriodicGen(float T);	// To generate final output of periodic pulses
  float Integrate(float T1,float T2); // Integrate the pulse train
  float eval(float time);		       // evaluate the pulse train at a given time
};

Pulse::Pulse(float start,float qq=1)
{
  t0 = start;
  rt = 1;			// default rise and fall tiems
  ft = 10;
  Q0 = qq;
  NC = (ft-rt)*log(2)/Q0;		// Remember, u hv to divide by it
}

Pulse::Pulse(float start,float rise, float fall, float qq=1)
{
  t0 = start;
  rt = rise;
  ft = fall;
  Q0 = qq;
  NC = (ft-rt)*log(2)/Q0;		// Remember, u hv to divide by it
}

// Pulse::Pulse(float rise, float fall, float TLow=0, float THi=100)
// {
//   t0 = RandInit(TLow,THi);
//   rt = rise;
//   ft = fall;
//   NC = (ft-rt)*log(2);		// Remember, u hv to divide by it
// }

float Pulse::eval(float T)
{
  if(T<t0) return(0);
  float y1 = 1/(1+exp((t0-T)/rt));
  float y2 = 1/(1+exp((t0-T)/ft));
  return((y1-y2)/NC);
}

float Pulse::Integrate(float T1, float T2)
{
  if(T2<t0) return(0);
  float t1 = T1;
  float t2 = T2;
  if(T1<0) t1 = 0;
  
  // float I0 = (ft-rt)*log(2);
  float I1 = rt*log(1+exp((t1-t0)/rt)) - ft*log(1+exp((t1-t0)/ft));
  float I2 = rt*log(1+exp((t2-t0)/rt)) - ft*log(1+exp((t2-t0)/ft));
  // return((I2-I1)/I0);
  return((I2-I1)/NC);
}

// float Pulse::RandInit(float tl, float th)
// {
//   TRandom tr;
//   float tt = tr.Uniform(tl,th);
//   float hh = tr.Uniform(0.,1.);
//   while(hh>eval(tt)){
//     tt = tr.Uniform(tl,th);
//     hh = tr.Uniform(0.,1.);
//   }
//   return(tt);
// }

void Pulse::RandInit(float tl=0, float th=100)
{
  TRandom tr;
  float tt = tr.Uniform(tl,th);
  float hh = tr.Uniform(0.,1.);
  while(hh>eval(tt)){
    tt = tr.Uniform(tl,th);
    hh = tr.Uniform(0.,1.);
  }
  t0=tt;
}

PulseGen::PulseGen()
{
  pp0.clear();
}

PulseGen::PulseGen(float t_start, float rise, float fall, float tgap, int npulses)
{
  pp0.clear();
  for(int i=0;i<npulses;i++){
    Pulse temp(t_start+i*tgap,rise,fall);
    pp0.push_back(temp);
  }
}

float PulseGen::PeriodicGen(float Time)
{
  int NN = pp0.size();
  float sig = 0;
  for(int i=0;i<NN;i++){
    sig+=pp0[i].eval(Time);
  }
  return(sig);
}

float PulseGen::eval(float time)
{
  if(pp0.size()==0){cout<<"Pulse train empty. returning -1\n"; return(-1);}
  int NN = pp0.size();
  float yy = 0;
  for(int i=0;i<pp0.size();i++) yy+=pp0[i].eval(time);
  return(yy);
}

float Pulse::MyMax()
{
  float a = t0;
  float b =t0+10;
  float mx=(a+b)/2;		// maximum

  while(abs(MyDer(mx))>=1e-5){
    if(MyDer(a)*MyDer(mx)>0) a=mx;
    else b = mx;
    mx = (a+b)/2;
  }
  return(mx);
}

float Pulse::MyDer(float x)
{
  float x_ = x-t0;
  float E1 = exp(-x_/rt);
  float E2 = exp(-x_/ft);

  float v1 = E1/(rt*pow(1+E1,2));
  float v2 = E2/(ft*pow(1+E2,2));

  return((v1-v2)/NC);		// Actual derivative
}

float PulseGen::Integrate(float T1, float T2)
{
  float QQ=0;
  for(int i=0;i<pp0.size();i++){
    QQ+=pp0[i].Integrate(T1,T2);
  }
  return(QQ);
}
