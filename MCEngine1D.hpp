#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdexcept>
#include <cmath>
#include <cassert>
#include <random>

#include "MCEngine1D.h"
#include "IRProvider.h"
#include "Time.h"

namespace SiriusFM{

template <typename Diffusion1D, typename AProvider, typename BProvider, typename AssetClassA, typename AssetClassB>
template<bool IsRN>
inline
void MCEngine1D<Diffusion1D, AProvider,BProvider,AssetClassA,AssetClassB>::
Simulate(time_t a_t0, time_t a_T, int a_tau_min, double a_s0, long a_P, Diffusion1D const* a_diff, AProvider const* a_rateA, BProvider const* a_rateB, AssetClassA a_A, AssetClassB a_B){

  assert(a_diff !=nullptr && a_rateA != nullptr && a_rateB != nullptr && a_A != AssetClassA::UNDEFINED 
						&& a_B != AssetClassA::UNDEFINED && a_t0 <= a_T && a_tau_min > 0 && a_P >0);

  
  double y0=YearFrac(a_t0);
  double yT=YearFrac(a_T);
  //printf("y0=%f\n",y0);
  time_t Tsec =a_T-a_t0;
  time_t tau_sec=a_tau_min*60.0;
  double tau = YearFrac(tau_sec)-YearFrac(0.0);
  long L=(Tsec%tau_sec==0) ? (Tsec/tau_sec) : (Tsec/tau_sec+1);
  //printf("L=%f\n",L);
  double tlast=(Tsec%tau_sec==0) ? tau : YearFrac(Tsec-(L-1)*tau_sec);
  assert(0<tlast && tlast<=tau);
  L++;
  long P=2*a_P; //Real amount of paths; but we generate only a_p paths

  //m_MaxL and m_MaxP - are personal properties of class MCEngine1D
  if (L*P > m_MaxL*m_MaxP) {printf ("L*P > m_maxL*m_MaxP in Simulate\n");throw std::invalid_argument("L*P > m_maxL*m_MaxP in Simulate");}
	
  std::normal_distribution <> N01(0.0,1.0);
  std::mt19937_64 U;
  double stau=sqrt(tau);
  double slast=sqrt(tlast);
  assert(slast <= stau  && slast>0);


  //Main Simulation Loop
  for (long p=0; p<a_P; ++p){ //cycle on paths
	double* path0=m_paths + 2*p*L;
        double* path1=path0+L;
	path0[0]=a_s0;
	path1[0]=a_s0;
	double y=y0;
	double sp0=a_s0; //it is s_previous
	double sp1=a_s0;
	for (long l=1; l<L; ++l){
	    double mu0=0.0;
	    double mu1=0.0;
	    if (IsRN){double delta_r=a_rateB->r(a_B,y)-a_rateA->r(a_A,y); mu0=delta_r*sp0; mu1=delta_r*sp1;}
	    else {mu0=a_diff->mu(sp0,y); mu1=a_diff->mu(sp1,y);}
	    double sigma0=a_diff->sigma(sp0,y);
	    double sigma1=a_diff->sigma(sp1,y);
	
	    double Z=N01(U);
            double sn0=0.0;
            double sn1=0.0;
	    if (l==L-1) {sn0=sp0+mu0*tlast+sigma0*Z*slast; sn1=sp1+mu1*tlast-sigma1*Z*slast; y+=tlast; }
	    else {sn0=sp0+mu0*tau+sigma0*Z*stau; sn1=sp1+mu1*tau-sigma1*Z*stau; y+=tau;}

	    path0[l]=sn0;
	    path1[l]=sn1;
	    sp0=sn1;
	    sp1=sn1;
	    

	

	}//for l cycle
	


  }//for p cycle
  m_L=L;
  m_P=P;
  


}//for Simulate
  

}//for namespace


