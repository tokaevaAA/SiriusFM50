#pragma once
#include "GridNOP1D_S3_RKC1.h"
#include "Time.h"
#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <cassert>
using namespace std;

namespace SiriusFM{

template<typename Diffusion1D, typename AProvider, typename BProvider, typename AssetClassA, typename AssetClassB>
void GridNOP1D_S3_RKC1<Diffusion1D, AProvider,BProvider,AssetClassA, AssetClassB>::RunBI(Option<AssetClassA,AssetClassB> const* a_option, Diffusion1D const* a_diff, double a_S0,time_t a_t0, long a_N, int a_tauMins,double a_BFactor){
	assert(a_option!=nullptr && a_diff!=nullptr && a_N>1 && a_BFactor>0);
	assert(a_S0>0);
	double TTE=YearFrac(a_option->m_expirTime)-YearFrac(a_t0);
	long tauSecs=a_tauMins*60;
	long Mintervals=(a_option->m_expirTime-a_t0)/tauSecs;
	if (TTE<=0||Mintervals<=0){printf("Option has already expired, or too close\n"); throw std::invalid_argument("Option has already expired, or too close\n");}
	long M=Mintervals+1;
	if (M>=m_maxM){printf("Too many t points\n"); throw std::invalid_argument("Too many t points\n");}
	double tau=TTE/double(Mintervals);
	double integrAB=0;
	m_ES[0]=a_S0;
	m_VarS[0]=0;
	for (int j=0; j<M; j=j+1){
		double t=YearFrac(a_t0+j*tauSecs);
		m_ts[j]=t;
		double rA=m_irpA.r(a_option->m_assetA,t);
		double rB=m_irpB.r(a_option->m_assetB,t);
		double rateDiff=std::max<double>(rB-rA,0.0);
		if (j<M-1){
			  integrAB+=rateDiff*tau;
			  m_ES[j+1]=a_S0*exp(integrAB);
			  double sigma = a_diff->sigma(m_ES[j],t);
			  m_VarS[j+1]=m_VarS[j]+sigma*sigma*tau;

		}
		
	}
	double B=m_ES[M-1]+a_BFactor*sqrt(m_VarS[M-1]);
 	printf("B=%f\n",B);
	double h=B/double(a_N-1);


}

}