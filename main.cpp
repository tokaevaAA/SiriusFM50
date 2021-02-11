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



int main(void)
{
  printf("Hello!\n");
 
  SiriusFM::IRProvider<SiriusFM::IRModeE::Const>  my_irprovider("interest_rates.txt");
  my_irprovider.pechat();

  printf("Goodbuy!\n");

}
