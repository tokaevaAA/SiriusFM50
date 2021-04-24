#pragma once
#include "GridNOP1D_S3_RKC1.h"
#include "Time.h"
#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <cassert>
#include <tuple>


using namespace std;

namespace SiriusFM{

template<typename Diffusion1D, typename AProvider, typename BProvider, typename AssetClassA, typename AssetClassB>
template<bool IsFwd>
void GridNOP1D_S3_RKC1<Diffusion1D, AProvider,BProvider,AssetClassA, AssetClassB>::
Run(Option<AssetClassA,AssetClassB> const* a_option, Diffusion1D const* a_diff, double a_S0,time_t a_t0, long a_Nints, int a_tauMins,double a_BFactor){

	assert(a_option!=nullptr && a_diff!=nullptr && a_tauMins>0 && a_Nints >0 && a_BFactor>0);
	assert(a_S0>0);
	if (a_option->m_isAsian){throw std::invalid_argument("Asian options are not suppoted by 1D Grid");}
	if (IsFwd && a_option->m_isAmerican){throw std::invalid_argument("American options are not suppoted in Fwd");}
	m_isFwd=IsFwd;

	double TTE=YearFrac(a_option->m_expirTime-a_t0)-YearFrac(0);
	long tauSecs=a_tauMins*60;
	long Mintervals=(a_option->m_expirTime-a_t0)/tauSecs;
	if (TTE<=0||Mintervals<=0){printf("Option has already expired, or too close\n"); throw std::invalid_argument("Option has already expired, or too close\n");}
	m_M=Mintervals+1;
	if (m_M>=m_maxM){printf("Too many t points\n"); throw std::invalid_argument("Too many t points\n");}
	double tau=TTE/double(Mintervals);
	double integrAB=0;
	m_ES[0]=a_S0;
	m_VarS[0]=0;
	for (int j=0; j<m_M; j=j+1){
		double t=YearFrac(a_t0+j*tauSecs);
		m_ts[j]=t;
		double rA=m_irpA.r(a_option->m_assetA,t);
		double rB=m_irpB.r(a_option->m_assetB,t);
		double rateDiff=std::max<double>(rB-rA,0.0);
		if (j<m_M-1){
			  integrAB+=rateDiff*tau;
			  m_ES[j+1]=a_S0*exp(integrAB);
			  double sigma = a_diff->sigma(m_ES[j],t);
			  m_VarS[j+1]=m_VarS[j]+sigma*sigma*tau;

		}
		
	}
	double B=m_ES[m_M-1]+a_BFactor*sqrt(m_VarS[m_M-1]);
 	printf("B=%f\n",B);
	double h=B/double(a_Nints);

	m_i0=int(round(a_S0/h));
	h=a_S0/double(m_i0);
	if (!std::isfinite(h)){throw std::invalid_argument("S0 too small, try increasing Nints\n");}
	B=h*double(a_Nints);

	m_N=a_Nints+1;
	if (m_N>m_maxN){throw std::invalid_argument("Nints too large\n");}

	//The Grid is stored by-column (S-contiguous)

	double* payOff=!IsFwd ? (m_grid + (m_M-1)*m_N):nullptr; //It is last column

	for (int i=0; i<m_N; i=i+1){
				   m_S[i]=double(i)*h;
				   if (!IsFwd){payOff[i]=a_option->payoff(1,m_ts+(m_M-1),m_S+i);}
			           }
	if(IsFwd){m_grid[m_i0]=1/h; for(int i=0; i<m_N; i=i+1){m_grid[i]=0.0;}}

	//At low bound (a=0) we always have a constant boundary condition, continuous with payoff, or 0 in Fwd run

	double fa=IsFwd ? 0 : payOff[0];
	bool isNeumann = false;
	double UBC=0.0; //Upper Boundary Condition
	if (!IsFwd){
		    isNeumann=(payOff[m_N-1]!=0);
	            UBC=isNeumann ? (payOff[m_N-1]-payOff[m_N-2]):0;
		   }
	for (int j=0; j<m_M-1; j=j+1){m_grid[j*m_N]=fa;}

	//Time Marshalling
	double D2=2*h*h;
	for(int j=IsFwd?0:m_M-1; IsFwd?(j<=m_M-2):(j>=1);j+=(IsFwd?1:-1)){
			double const* fj=m_grid+j*m_N; //Prev time layer(j)
			double* fj1=const_cast<double*>(IsFwd?(fj+m_N):(fj-m_N)); //Curr time layer to be filled(j+-1)
			double tj=m_ts[j];
			double rateAj=m_irpA.r(a_option->m_assetA,tj);
			double rateBj=m_irpB.r(a_option->m_assetB,tj);
			double C1=(rateBj-rateAj)/(2*h);
			fj1[0]=fa;
			for (int i=1; i<=m_N-2; i=i+1){double Si=m_S[i];
						       double fjiMinus=fj[i-1];
						       double fji=fj[i];
						       double fjiPlus=fj[i+1];
						       double sigma=a_diff->sigma(Si,tj);
						       double DfDt=0;
						       if(IsFwd){//Fokker-Plank
								double SiMinus=m_S[i-1];
								double SiPlus=m_S[i+1];
								double sigmaMinus=a_diff->sigma(SiMinus,tj);
								double sigmaPlus=a_diff->sigma(SiPlus,tj);	
								DfDt=-C1*(SiPlus*fjiPlus - SiMinus*fjiMinus)+(sigmaPlus*sigmaPlus*fjiPlus-2*sigma*sigma*fji + sigmaMinus*sigmaMinus*fjiMinus)/D2;
								}
							else{//Backward: BSM
							     DfDt=rateBj*fji-C1*Si*(fjiPlus-fjiMinus)-sigma*sigma/D2*(fjiPlus-2*fji+fjiMinus);
							    }
							fj1[i]=fji-tau*DfDt;
			}
			
			fj1[m_N-1]=(!IsFwd && isNeumann) ? (fj1[m_N-2]+UBC):UBC;
			//Grid allows us to price American options as well (but only in Bwd)
			if (a_option->m_isAmerican){assert(!IsFwd);
						    for (int i=0; i<m_N; i=i+1){
									       double intrinsicValue=a_option->payoff(1,&tj,m_S+i);
										fj1[i]=std::max<double>(fj1[i],intrinsicValue);
									       }
						    }

			}

	}
	

	template<typename Diffusion1D, typename AProvider, typename BProvider, typename AssetClassA, typename AssetClassB>
	std::tuple<double,double,double> GridNOP1D_S3_RKC1<Diffusion1D, AProvider,BProvider,AssetClassA, AssetClassB>::
	GetPxDeltaGamma0()const{
				if (m_M==0||m_N==0){throw std::runtime_error("Do Run first!\n");}
				assert(0<= m_i0 && m_i0<m_N);
				double h=m_S[1]-m_S[0];
				double px=m_grid[m_i0]; //j=0
				double delta=0;
				double gamma=0;

								printf("m_i0=%d m_N=%d\n",m_i0,m_N);
				if(0<m_i0 && m_i0<=m_N-2){delta=(m_grid[m_i0+1]-m_grid[m_i0-1])/(2*h);
							  gamma=(m_grid[m_i0+1]-2*m_grid[m_i0]+m_grid[m_i0-1])/(h*h);
							 }
				else if(m_i0==0){delta=(m_grid[1]-m_grid[0])/h; gamma=0.0;}
				else {assert(m_i0==m_N-1); delta=(m_grid[m_N-1]-m_grid[m_N-2])/h; gamma=0.0;}
				return std::make_tuple(px, delta, gamma);

				}

}