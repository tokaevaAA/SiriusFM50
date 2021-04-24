#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "Option.h"
#include <stdexcept>
#include <cmath>
#include <utility>
#include <tuple>
#include <algorithm>


using namespace std;

namespace SiriusFM{

template<typename AssetClassA, typename AssetClassB>
class CallOption final: public Option<AssetClassA, AssetClassB>{
  private:
	double const m_K;
  public:
	CallOption(AssetClassA a_assetA,AssetClassB a_assetB,double a_K, time_t a_expirTime,bool a_isAmerican) : Option<AssetClassA, AssetClassB>(a_assetA,a_assetB,a_expirTime,a_isAmerican,false), m_K(a_K) 
	{
	if (m_K<=0){printf("K<=0 in CallOption Constructor!\n");}}

	~CallOption() override {}
	double payoff(long a_L, double const* a_ts, double const* a_path) const override {assert(a_L>0 && a_path!=nullptr);
	double ST=a_path[a_L-1];
	return std::max<double>(ST-m_K,0.0);
	}

};


template<typename AssetClassA, typename AssetClassB>
class PutOption final: public Option<AssetClassA, AssetClassB>{
  private:
	double const m_K;
  public:
	PutOption(AssetClassA a_assetA,AssetClassB a_assetB,double a_K, time_t a_expirTime, bool a_isAmerican) : Option<AssetClassA, AssetClassB>(a_assetA,a_assetB,a_expirTime,a_isAmerican,false), m_K(a_K) 
	{
	if (m_K<=0){printf("K<=0 in EurCallOption Constructor!\n");}}

	~PutOption() override {}
	double payoff(long a_L, double const* a_ts, double const* a_path) const override {assert(a_L>0 && a_path!=nullptr);
	double ST=a_path[a_L-1];
	return std::max<double>(m_K-ST,0.0);
	}

};
using CallOptionFX=CallOption<CcyE,CcyE>;
using PutOptionFX=PutOption<CcyE,CcyE>;


}