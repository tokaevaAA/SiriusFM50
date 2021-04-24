#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdexcept>
#include <cmath>
#include <utility>
#include <tuple>

using namespace std;

namespace SiriusFM{

class Option{
  protected:
	bool const m_isAmerican;
	int const m_Tdays;
  public:
	Option(bool a_isAmerican, int a_Tdays):m_isAmerican(a_isAmerican),m_Tdays(a_Tdays){if (m_Tdays <=0) {printf("m_Tdays <=0 in Option!\n");}}

	virtual double payoff(long a_L,double const* a_t, double const* a_S) const=0;
	virtual ~Option(){}
	bool IsAmerican()const{return m_isAmerican;}

};

}