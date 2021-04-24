#pragma once
#include "MCEngine1D.h"
#include "Option.h"

namespace SiriusFM{

template<typename Diffusion1D, typename AProvider, typename BProvider, typename AssetClassA, typename AssetClassB>
class MCOptionPricer1D{
  private:
	class OPPathEval{
	  private:
		Option<AssetClassA,AssetClassB> const* const m_option;
		long m_P;
		double m_sum;
		double m_sum2;
		double m_minP0;
		double m_maxP0;
	  public:
		OPPathEval(Option<AssetClassA,AssetClassB>const* a_option):m_option(a_option),m_P(0),m_sum(0),m_sum2(0),m_minP0(INFINITY),m_maxP0(-INFINITY){assert(m_option!=nullptr);}
		void operator()(long a_L, long a_PM, double const* a_paths, double const* a_ts){
		for(long p=0; p<a_PM; ++p){double const* path=a_paths+p*a_L;
					   double payOff=m_option->payoff(a_L,a_ts,path);
					   m_sum+=payOff;
					   m_sum2+=payOff*payOff;
					   m_minP0=std::min<double>(m_minP0,payOff);
					   m_maxP0=std::max<double>(m_maxP0,payOff);
					  }
		m_P+=a_PM;

        	}

		double GetPx()const{if (m_P<2){printf("m_P<2 in GetPx\n");} return m_sum/double(m_P);}
		std::pair<double,double> GetPxStats()const{
			if(m_P<2){printf("Empty OpPathEval in GetPxStats\n"); }
			double px=m_sum/double(m_P);
			double var=(m_sum2-double(m_P)*px*px)/double(m_P-1);
			assert(var>=0);
			double err=(px!=0)?sqrt(var)/fabs(px):sqrt(var);
			return std::make_pair(px, err);
		}





	};
	Diffusion1D const* const m_diff;
	AProvider m_irpA;
	BProvider m_irpB;
	MCEngine1D<Diffusion1D,AProvider,BProvider,AssetClassA,AssetClassB,OPPathEval> m_mce;
	bool m_useTimeSeed;
  public:
	MCOptionPricer1D(Diffusion1D const* a_diff,const char* a_irsFileA, const char* a_irsFileB, bool a_useTimeSeed):m_diff(a_diff),m_irpA(a_irsFileA),m_irpB(a_irsFileB),m_mce(102271,4096),m_useTimeSeed(a_useTimeSeed){}
	double Px(Option<AssetClassA,AssetClassB> const* a_option,time_t a_t0,int a_tauMins=15,long a_P=100000);



};




}