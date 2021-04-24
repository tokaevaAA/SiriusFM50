#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdexcept>
#include "IRProvider.h"

namespace SiriusFM{

template<>
class IRProvider<IRModeE::Const>{
  private:
	double m_IRs[int(CcyE::N)];
  public:
	IRProvider(const char* a_file);
   	double r (CcyE a_ccy, double a_t)const {return m_IRs[int(a_ccy)];}
	void pechat(){for (int i=0; i<int(CcyE::N); i=i+1) {printf("%f\n",m_IRs[i]);}}


};


}