#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdexcept>
#include <cmath>
#include <utility>
#include <tuple>

using namespace std;

namespace SiriusFM{

template <typename Diffusion1D, typename AProvider, typename BProvider, typename AssetClassA, typename AssetClassB>
class MCEngine1D{
  private:
    long m_L;
    long m_P;
    long const m_MaxL;
    long const m_MaxP;
    double* const m_paths;

  public:

    MCEngine1D(long a_MaxL, long a_MaxP):m_L(0),m_P(0),m_MaxL(a_MaxL),m_MaxP(a_MaxP),m_paths(new double[m_MaxL * m_MaxP]){if (m_MaxL <=0 || m_MaxP <=0) {printf("Bad input in MCEngine1D constructor!\n");throw std::invalid_argument("Bad ccyE in CcyE2Str."); }}
    
     MCEngine1D(MCEngine1D const&)=delete;
     MCEngine1D& operator=(MCEngine1D const &)=delete;
    ~MCEngine1D(){delete[] m_paths;}
    
    template<bool IsRN>
    void Simulate(time_t a_t0, time_t a_T, int a_tau_min, double a_s0, long a_P, Diffusion1D const* a_diff, AProvider const* a_rateA, BProvider const* a_rateB, AssetClassA a_A, AssetClassB a_B);

    std::tuple<long,long,double const*>GetPaths()const{
		return (m_L<=0 || m_P<=0)?
		std::make_tuple(0,0,nullptr):
		std::make_tuple(m_L,m_P,m_paths);

    }
  

};
}