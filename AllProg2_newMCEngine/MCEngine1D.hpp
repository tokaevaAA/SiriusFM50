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

template <typename Diffusion1D, typename AProvider, typename BProvider, typename AssetClassA, typename AssetClassB, typename PathEvaluator>
template<bool IsRN>
inline
void MCEngine1D<Diffusion1D, AProvider, BProvider,AssetClassA,AssetClassB,PathEvaluator>::
Simulate(time_t a_t0, time_t a_T, int a_tauMins, long a_P, bool a_useTimeSeed, Diffusion1D const* a_diff, AProvider const* a_rateA, BProvider const* a_rateB, AssetClassA a_assetA, AssetClassB a_assetB, PathEvaluator* a_pathEval){

  assert(a_diff !=nullptr && a_rateA != nullptr && a_rateB != nullptr && a_P>0 && a_assetA != AssetClassA::UNDEFINED 
						&& a_assetB != AssetClassA::UNDEFINED && a_t0 <= a_T && a_tauMins > 0 && a_pathEval!=nullptr);

  
  double y0=YearFrac(a_t0);
  double yT=YearFrac(a_T);
  
  time_t T_sec =a_T-a_t0;
  time_t tau_sec=a_tauMins*60.0;
  long L_segm=(T_sec%tau_sec==0) ? (Tsec/tau_sec) : (Tsec/tau_sec+1);
  double tau = YearFrac(tau_sec)-YearFrac(0.0);
  long L=L_segm+1;
  long P=2*a_P; 

  if (L> m_MaxL) {printf ("L> m_maxL in Simulate\n");throw std::invalid_argument("L > m_maxL in Simulate");}

  double stau=sqrt(tau);
  double tlast=(Tsec%tau_sec==0) ? tau : YearFrac(Tsec-(L-1)*tau_sec);
  assert(tlast<=tau && tlast>0);
  double slast=sqrt(tlast);
  assert(slast <= stau  && slast>0);
    	
  //Construct the Timeline
  for (long l=0; l<L-1; l=l+1){m_ts[l]=y0+double(l)*tau;}
  m_ts[L-1]=m_ts[L-2]+tlast;

  std::normal_distribution <> N01(0.0,1.0);
  std::mt19937_64 U(a_useTimeSeed ? time(nullptr) : 0);
  

  
  long PM=(m_maxL*m_MaxPM)/L; //PM: how many paths we can store in-memory
  if(PM%2!=0){PM=PM-1}
  assert(PM>0 && PM%2==0);

  long PMh=PM/2;

  
  long PI=(P%PM==0)?(P/PM):(P/PM+1); //PI: Number of outer P iterations

  //Now actual P=PI*PM;


  //Main Simulation Loop
  for (long i=0; i<PI; i=i+1){
      for (long p=0; p<PMh; ++p){ 
          double* path0=m_paths + 2*p*L;
          double* path1=path0+L;
	  path0[0]=a_diff->get_s0();
	  path1[0]=a_diff->get_s0();
	
	  double sp0=a_diff->get_s0(); //it is s_previous
	  double sp1=a_diff->get_s0();
	  for (long l=1; l<L; ++l){
	      double mu0=0.0;
	      double mu1=0.0;
              double y=m_ts[l-1];
	      if (IsRN){double delta_r=a_rateB->r(a_assetB,y)-a_rateA->r(a_assetA,y); mu0=delta_r*sp0; mu1=delta_r*sp1;}
	      else {mu0=a_diff->mu(sp0,y); mu1=a_diff->mu(sp1,y);}
	      double sigma0=a_diff->sigma(sp0,y);
	      double sigma1=a_diff->sigma(sp1,y);
	
	      double Z=N01(U);
              double sn0=0.0;
              double sn1=0.0;
	      if (l==L-1) {sn0=sp0+mu0*tlast+sigma0*Z*slast; sn1=sp1+mu1*tlast-sigma1*Z*slast;}
	      else {sn0=sp0+mu0*tau+sigma0*Z*stau; sn1=sp1+mu1*tau-sigma1*Z*stau; }

	      path0[l]=sn0;
	      path1[l]=sn1;
	      sp0=sn0;
	      sp1=sn1;
	    }//for l cycle
	


     }//for p cycle
    (*a_pathEval)(L,PM,m_paths,m_ts);
  
  }//for i cycle


}//for Simulate
  

}//for namespace


