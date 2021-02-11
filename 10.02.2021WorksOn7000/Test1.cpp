#include <stdio.h>
#include <stdlib.h>
#include <stdexcept>
#include <cmath>

#include "DiffusionGBM.h"
#include "DiffusionCEV.h"
#include "DiffusionOU.h"
#include "DiffusionLipton.h"
#include "DiffusionCIR.h"

#include "IRProvider.h"
#include "IRProviderConst.h"

#include "MCEngine1D.h"
#include "MCEngine1D.hpp"

using namespace SiriusFM;
using namespace std;

int main(int argc, char** argv)
{
  printf("Hello!\n");
  if (argc!=7){printf("Usage: mu,sigma,S_0,T_days,tau_min,P\n"); return -1;}
  double mu=atof(argv[1]);
  double sigma=atof(argv[2]);
  double s0=atof(argv[3]);
  long T_days=atof(argv[4]);
  int tau_min=atoi(argv[5]);
  long P=atoi(argv[6]);
  if (sigma<=0 || s0<=0 || T_days<=0 || tau_min<=0 || P<=0){printf("Bad values in input!\n"); return -2;}
  
  CcyE ccyA=CcyE::USD;
  IRProvider<IRModeE::Const> irp(nullptr);
  DiffusionGBM diff(mu, sigma);
  MCEngine1D<decltype(diff),decltype(irp), decltype(irp), decltype(ccyA), decltype(ccyA)> mce(20000,20000);
  time_t t0=time(nullptr);
  time_t T=t0+T_days*86400;
  double T_years=(double)(T_days)/365.25;
  
  mce.Simulate<false>(t0,T,tau_min,s0,P,&diff,&irp,&irp,ccyA,ccyA);
  
  //Analyze the results
  auto res=mce.GetPaths();
  long L1=get<0>(res);
  long P1=get<1>(res);
  double const* paths=get<2>(res);
  double EST=0.0;
  double EST2=0.0;
  int NVP=0;
  for (int p=0; p<P1; p=p+1){
	double const* path=paths+p*L1;
	double ST=path[L1-1];
	if(ST<=0.0){continue;}
	NVP+=1;
	double RT=log(ST/s0);
	EST+=RT;
	EST2+=RT*RT;
  }
  printf("EST=%f EST2=%f\n",EST,EST2);
  assert(NVP>1);
  EST/=double(NVP);

  double VarST=(EST2-(double)(NVP)*EST*EST)/(double)(NVP-1);
  double sigma2E=VarST/T_years;
  double muE=(EST+0.5*VarST)/T_years;
  printf("%f %f\n",mu,muE);
  printf("%f %f\n",sigma,sigma2E);
 
  
  printf("Goodbuy!\n");
  return 0;

}
