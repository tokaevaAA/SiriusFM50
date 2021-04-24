#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdexcept>
#include <cmath>
#include "IRProvider.h"
#include "Time.h"

namespace SiriusFM{

template<>
class IRProvider<IRModeE::Const>{
  private:
	double m_IRs[int(CcyE::N)];
  public:
	IRProvider(const char* a_file);
   	double r (CcyE a_ccy, double a_t)const {return m_IRs[int(a_ccy)];}
	double DF(CcyE a_ccy, time_t a_t0, time_t a_t1) const{double y=YearFrac(a_t1-a_t0)-YearFrac(0.0); return exp(-m_IRs[int(a_ccy)]*y);}
	void pechat(){for (int i=0; i<int(CcyE::N); i=i+1) {printf("%f\n",m_IRs[i]);}}


};

using IRPConst = IRProvider<IRModeE::Const>;
}