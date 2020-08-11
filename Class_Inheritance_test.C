// Small example to see how class inheritance works
class Pulse
{
public:
  Pulse();			// Default constructor
  Pulse(int, int);		// main constructor
  virtual int Sum();		// sum of two integers
  virtual void PrintInfo();	// Print X,Y
// private:
protected:
  int X;
  int Y;
};
/////////////////////////////////////////////////////////////////////////

class Bimoid:public Pulse
{
public:
  Bimoid(int, int);		// main constructor
  int Sum();			// actually product of two integers
  void PrintInfo();
};
/////////////////////////////////////////////////////////////////////////

Pulse::Pulse(int x, int y){
  X=x; Y=y;}
int Pulse::Sum(){
  cout<<"Inside Pulse::Sum()\n"; return(X+Y);}
void Pulse::PrintInfo(){
  cout<<"X="<<X<<"\tY="<<Y<<endl;}
/////////////////////////////////////////////////////////////////////////

Bimoid::Bimoid(int x, int y):Pulse(x,y){
  cout<<"Inside daughter's main constructor\n";}
int Bimoid::Sum(){
  cout<<"Inside daughter's Sum()\n"; return(X*Y);}
void Bimoid::PrintInfo(){
  cout<<"Inside daughter's PrintInfo()\nX="<<X<<"\tY="<<Y<<endl;}
/////////////////////////////////////////////////////////////////////////

void TestFun(Pulse*);

void Class_Inheritance_test()
{
  Bimoid bm(6,4);
  Pulse* pl = new Bimoid(7,3);
  TestFun(pl);
  TestFun(&bm);
}

void TestFun(Pulse* pp)
{
  pp->PrintInfo();
  cout<<"Sum() = "<<pp->Sum()<<endl;
}
