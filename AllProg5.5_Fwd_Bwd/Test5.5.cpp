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
  if (argc!=10){printf("Usage: sigma, s_0, call/put, K, Tdays, isAmerican, ratesFile, NS, tauMins\n"); return -1;}
 
  double sigma=atof(argv[1]);
  double s0=atof(argv[2]);
  char const* optType=argv[3];
  double K=atof(argv[4]);
  long Tdays=atof(argv[5]);
  bool isAmerican = bool(atoi(argv[6]));
  char const* ratesFile=argv[7];
  int NS=atol(argv[8]);
  int tauMins=atoi(argv[9]);
 
  
  if (sigma<=0 || s0<=0 ||K<=0 || Tdays<=0 ||NS<=0|| tauMins<=0 ){printf("Bad values in input!\n"); return -2;}
  
  CcyE ccyA=CcyE::USD;
  CcyE ccyB=CcyE::RUB;

    
  DiffusionGBM diff(0.0, sigma,s0);

  time_t t0=time(nullptr);
  time_t T=t0+Tdays*86400;
    
  OptionFX const* option=nullptr;
  
  if (strcmp(optType,"call")==0) {option=new CallOptionFX(ccyA,ccyB,K,T,isAmerican);}
  if (strcmp(optType,"put")==0) {option=new PutOptionFX(ccyA,ccyB,K,T,isAmerican);}

  
  GridNOP1D_S3_RKC1<decltype(diff),IRPConst,IRPConst,CcyE,CcyE>
					grid(ratesFile,ratesFile);


  grid.Run<false>(option,&diff,s0,t0,NS,tauMins);

  auto res=grid.GetPxDeltaGamma0();
  double px = get<0>(res);
  double delta = get<1>(res);
  double gamma = get<2>(res);
  
  printf("Price=%f\n",px);
  printf("Delta=%f\n",delta);
  printf("Gamma=%f\n",gamma);

  delete option;
   
  
  printf("Goodbuy!\n");
  return 0;

}
