#include <stdio.h>
#include <stdlib.h>
#include <stdexcept>
#include <cmath>
#include<ctime>

#include "DiffusionGBM.h"

#include "IRProvider.h"
#include "IRProviderConst.h"

#include "MCEngine1D.h"
#include "MCEngine1D.hpp"

#include "VanillaOptions.h"
#include "MCOptionPricer1D.hpp"



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
  double t_full;
  if (sigma<=0 || s0<=0 || T_days<=0 || tau_min<=0 || P<=0){printf("Bad values in input!\n"); return -2;}
  
  CcyE ccyA=CcyE::USD;
  CcyE ccyB=CcyE::USD;

  char const* ratesFileA=nullptr;
  char const* ratesFileB=nullptr;
  bool useTimerSeed=true; 

  
  DiffusionGBM diff(mu, sigma,s0);

  MCOptionPricer1D<decltype(diff),IRPConst,IRPConst,CcyE,CcyE> pricer(&diff,ratesFileA,ratesFileB,useTimerSeed);

  time_t t0=time(nullptr);
  time_t T=t0+T_days*86400;
  
  OptionFX const* option=nullptr;
  if (strcmp(optType,"call")==0) {option=new EurCallOptionFX(ccyA,ccyB,K,T);}
  if (strcmp(optType,"put")==0) {option=new EurPutOptionFX(ccyA,ccyB,K,T);}


  
  t_full=clock();
  double px=pricer.Px(option,t0,tau_min,P);

  t_full=(clock()-t_full)/CLOCKS_PER_SEC;
  
  printf("Price=%f\n",px);
  printf("t_full=%f\n",t_full);


  delete option;
   
  
  printf("Goodbuy!\n");
  return 0;

}
