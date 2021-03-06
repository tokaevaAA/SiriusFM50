#pragma once
#include "MCOptionHedger1D.h"
#include "MCEngine1D.h"
#include <ctime>

namespace SiriusFM{

template<typename Diffusion1D, typename AProvider, typename BProvider, typename AssetClassA, typename AssetClassB>
inline
std::tuple<double,double,double,double> MCOptionHedger1D<Diffusion1D,AProvider,BProvider,AssetClassA,AssetClassB>::
SimulateHedging(Option<AssetClassA,AssetClassB> const* a_option, time_t a_t0, double a_C0,DeltaFunc const* a_DeltaFunc, double a_DeltaAcc, int a_tauMins, long a_P){
        printf("Hello from SimulateHedging\n");
	assert(a_option!=nullptr && a_tauMins>0 && a_P>0 && a_DeltaFunc!=nullptr && a_DeltaAcc>0);
	OHPathEval pathEval(a_option,&m_irpA,&m_irpB,a_C0,a_DeltaFunc,a_DeltaAcc);
        double t_sim=clock();
	m_mce.template Simulate<false>(a_t0,a_option->m_expirTime, a_tauMins, a_P, m_useTimerSeed,m_diff,&m_irpA,&m_irpB,a_option->m_assetA,a_option->m_assetB,&pathEval);
        t_sim=(clock()-t_sim)/CLOCKS_PER_SEC;
        printf("t_simulate=%f\n",t_sim);
	return pathEval.GetStats();



}

}