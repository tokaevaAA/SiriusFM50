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

#include "VanillaOptions.h"

using namespace SiriusFM;
using namespace std;

int main(int argc, char** argv)
{
  printf("Hello!\n");
  if (argc!=9){printf("Usage: mu, sigma, S_0, call/put, K, T_days, tau_min,P\n"); return -1;}
  double mu=atof(argv[1]);
  double sigma=atof(argv[2]);
  double s0=atof(argv[3]);
  char const* optType=argv[4];
  double K=atof(argv[5]);
  long T_days=atof(argv[6]);
  int tau_min=atoi(argv[7]);
  long P=atoi(argv[8]);
  if (sigma<=0 || s0<=0 || T_days<=0 || tau_min<=0 || P<=0){printf("Bad values in input!\n"); return -2;}
  
  CcyE ccyA=CcyE::USD;
  IRProvider<IRModeE::Const> irp(nullptr);
  DiffusionGBM diff(mu, sigma);
  MCEngine1D<decltype(diff),decltype(irp), decltype(irp), decltype(ccyA), decltype(ccyA)> mce(20000,20000);
  time_t t0=time(nullptr);
  time_t T=t0+T_days*86400;
  double T_years=(double)(T_days)/365.25;
  
  mce.Simulate<true>(t0,T,tau_min,s0,P,&diff,&irp,&irp,ccyA,ccyA);
  
  //Analyze the results
  auto res=mce.GetPaths();
  long L1=get<0>(res);
  long P1=get<1>(res);
  double const* paths=get<2>(res);
  double EST=0.0;
  
  Option const* option=nullptr;
  if (strcmp(optType,"call")==0) {option=new EurCallOption(K,T_days);}
  if (strcmp(optType,"put")==0) {option=new EurPutOption(K,T_days);}
  for (int p=0; p<P1; p=p+1){
	double const* path=paths+p*L1;
	double ST=path[L1-1];
	if(ST<=0.0){continue;}
	
	double RT=option->payoff(L1,nullptr,path);
	EST+=RT;
	
  }
  
 
  EST/=P1;
  printf("EST=%f \n",EST);
  
  delete option;
   
  
  printf("Goodbuy!\n");
  return 0;

}
