#include <stdio.h>
#include <stdlib.h>
#include "DiffusionGBM.h"
#include "DiffusionCEV.h"
#include "DiffusionOU.h"
#include "DiffusionLipton.h"
#include "DiffusionCIR.h"



int main(){

  printf("Hello!\n");

  SiriusFM::DiffusionGBM Df1=SiriusFM::DiffusionGBM(1,2);
  Df1.pechat();

  SiriusFM::DiffusionCEV Df2=SiriusFM::DiffusionCEV(3,4,5);
  Df2.pechat();

  SiriusFM::DiffusionOU Df3=SiriusFM::DiffusionOU(6,7,8);
  Df3.pechat();
 
  SiriusFM::DiffusionLipton Df4=SiriusFM::DiffusionLipton(9,10,11,12);
  Df4.pechat();

  SiriusFM::DiffusionCIR Df5=SiriusFM::DiffusionCIR(13,14,15);
  Df5.pechat();

  printf("Goodbuy!\n");

  return 0;
}