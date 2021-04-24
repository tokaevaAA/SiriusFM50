#include <stdio.h>
#include <stdlib.h>
#include <stdexcept>
#include <cmath>
#include <cstring>
#include <ctime>


#include "DiffusionGBM.h"

#include "IRProvider.h"
#include "IRProviderConst.h"

#include "VanillaOptions.h"

#include "GridNOP1D_S3_RKC1.hpp"


using namespace SiriusFM;
using namespace std;


int main(int argc, char** argv)
{
  printf("Hello!\n");
  if (argc!=8){printf("Usage: sigma, s_0, call/put, K, Tdays, NS, tauMins\n"); return -1;}
  //./a.out 0.25 100 call 100 90 1000 30 
  //Hello! B=155.844276 Goodbuy!

 
  double sigma=atof(argv[1]);
  double s0=atof(argv[2]);
  char const* optType=argv[3];
  double K=atof(argv[4]);
  long Tdays=atof(argv[5]);
  int NS=atol(argv[6]);
  int tauMins=atoi(argv[7]);
 
  
  if (sigma<=0 || s0<=0 ||K<=0 || Tdays<=0 ||NS<=0|| tauMins<=0 ){printf("Bad values in input!\n"); return -2;}
  
  CcyE ccyA=CcyE::USD;
  CcyE ccyB=CcyE::USD;

  char const* ratesFileA=nullptr;
  char const* ratesFileB=nullptr;
  
  DiffusionGBM diff(0.0, sigma,s0);

  time_t t0=time(nullptr);
  time_t T=t0+Tdays*86400;
    
  OptionFX const* option=nullptr;
  
  if (strcmp(optType,"call")==0) {option=new EurCallOptionFX(ccyA,ccyB,K,T);}
  if (strcmp(optType,"put")==0) {option=new EurPutOptionFX(ccyA,ccyB,K,T);}

  
  GridNOP1D_S3_RKC1<decltype(diff),IRPConst,IRPConst,CcyE,CcyE>
					grid(ratesFileA,ratesFileB);


  grid.RunBI(option,&diff,s0,t0,NS,tauMins);

  

  delete option;
   
  
  printf("Goodbuy!\n");
  return 0;

}
