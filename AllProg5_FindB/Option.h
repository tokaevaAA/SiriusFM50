#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdexcept>
#include <cmath>
#include <utility>
#include <tuple>

#include "IRProvider.h"

using namespace std;

namespace SiriusFM{

template<typename AssetClassA, typename AssetClassB>
class Option{
  
  public:
	AssetClassA const m_assetA;
	AssetClassB const m_assetB;
	time_t const m_expirTime;
	bool const m_isAmerican;
	Option(AssetClassA a_assetA, AssetClassB a_assetB, time_t a_expirTime, bool a_isAmerican):m_assetA(a_assetA),m_assetB(a_assetB),m_expirTime(a_expirTime),m_isAmerican(a_isAmerican){}

	virtual double payoff(long a_L, double const* a_ts, double const* a_path) const=0;
	virtual ~Option(){}
	bool IsAmerican()const{return m_isAmerican;}
};
using OptionFX=Option<CcyE,CcyE>;

}