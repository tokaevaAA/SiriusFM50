#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdexcept>
#include <cmath>
#include <utility>
#include <tuple>
#include "Time.h"

using namespace std;

namespace SiriusFM{

template 
<typename Diffusion1D, typename AProvider, typename BProvider, typename AssetClassA, typename AssetClassB,typename PathEvaluator>
class MCEngine1D{
  private:
    long const m_MaxL; //Max Path Length
    long const m_MaxPM; //Max Number of Paths Stored in Memory
    double* const m_paths; //Stored Paths
    double* const m_ts; //TimeLine
 
  public:

    MCEngine1D(long a_MaxL, long a_MaxPM):m_MaxL(a_MaxL),m_MaxPM(a_MaxPM),m_paths(new double[m_MaxL * m_MaxPM]),m_ts(new double[m_MaxL]){
			if (m_MaxL <=0 || m_MaxPM <=0) {printf("Bad input in MCEngine1D constructor!\n");
			throw std::invalid_argument("Bad input in MCEngine1D constructor"); }
			for (long l=0; l<m_MaxL; l=l+1){
						m_ts[l]=0;
						long lp=l*m_MaxPM;
						for (long p=0; p<m_MaxPM; p=p+1){m_paths[lp+p]=0;}
						}
			
			}
     ~MCEngine1D(){delete[] m_paths; delete[] m_ts;}

    
     MCEngine1D(MCEngine1D const&)=delete;
     MCEngine1D& operator=(MCEngine1D const &)=delete;
        
    template<bool IsRN>
    void Simulate(time_t a_t0, time_t a_T, int a_tauMins, long a_P, bool a_useTimeSeed, Diffusion1D const* a_diff, AProvider const* a_rateA, BProvider const* a_rateB, AssetClassA a_A, AssetClassB a_B, PathEvaluator* a_pathEval);

    
};
}