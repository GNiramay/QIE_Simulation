// Class to store all the functions related to the input pulse to QIE

////////////////////////// The base class ///////////////////////////////
class Pulse
{
public:
  virtual float eval(float T);		      // Evaluate pulse at time T
  virtual float Integrate(float T1,float T2); // Integrate pulse from T1 to T2
  virtual float Der(float T);		      // Differentiate pulse at time T
  virtual float Max();			      // Return the maximum of the pulse
};

float Pulse::eval(float T){return(0);}
float Pulse::Integrate(float T1,float T2){return(0);}
float Pulse::Der(float T){return(0);}
float Pulse::Max(){return(0);}
//////////////////////// Daughter class /////////////////////////////////

class Bimoid: public Pulse
{
public:
  Bimoid(float start,float rise, float fall, float qq=1);
  Bimoid(float start,float qq=1);
  float eval(float T);
  float Integrate(float T1,float T2);
  float Max();
  float Der(float T);
private:
  float t0;			// starting time of the pulse
  float rt;			// rise time
  float ft;			// fall time
  float NC;			// Normalization constant
  float Q0;			// Net charge (integral I.dt)
};

/////////////////////////////////////////////////////////////////////////

Bimoid::Bimoid(float start,float qq=1)
{
  t0 = start;
  rt = 1;			// default rise and fall tiems
  ft = 10;
  Q0 = qq;
  NC = (ft-rt)*log(2)/Q0;		// Remember, u hv to divide by it
}

Bimoid::Bimoid(float start,float rise, float fall, float qq=1)
{
  t0 = start;
  rt = rise;
  ft = fall;
  Q0 = qq;
  NC = (ft-rt)*log(2)/Q0;		// Remember, u hv to divide by it
}

float Bimoid::eval(float T)
{
  if(T<t0) return(0);
  float y1 = 1/(1+exp((t0-T)/rt));
  float y2 = 1/(1+exp((t0-T)/ft));
  return((y1-y2)/NC);
}

float Bimoid::Integrate(float T1, float T2)
{
  if(T2<t0) return(0);
  float t1 = T1;
  float t2 = T2;
  if(T1<0) t1 = 0;
  
  float I1 = rt*log(1+exp((t1-t0)/rt)) - ft*log(1+exp((t1-t0)/ft));
  float I2 = rt*log(1+exp((t2-t0)/rt)) - ft*log(1+exp((t2-t0)/ft));
  return((I2-I1)/NC);
}

float Bimoid::Max()
{
  float a = t0;
  float b =t0+10;
  float mx=(a+b)/2;		// maximum

  while(abs(Der(mx))>=1e-5){
    if(Der(a)*Der(mx)>0) a=mx;
    else b = mx;
    mx = (a+b)/2;
  }
  return(mx);
}

float Bimoid::Der(float T)
{
  float T_ = T-t0;
  float E1 = exp(-T_/rt);
  float E2 = exp(-T_/ft);

  float v1 = E1/(rt*pow(1+E1,2));
  float v2 = E2/(ft*pow(1+E2,2));

  return((v1-v2)/NC);		// Actual derivative
}
