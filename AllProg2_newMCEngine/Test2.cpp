#include <stdio.h>
#include <stdlib.h>
#include <stdexcept>
#include <cmath>

#include "DiffusionGBM.h"

#include "IRProvider.h"
#include "IRProviderConst.h"

#include "MCEngine1D.h"
#include "MCEngine1D.hpp"

#include "VanillaOptions.h"

namespace SiriusFM{
class OPPathEval{ //Path Evaluator for Option Pricing
  private:
	Option const* const m_option;
	long m_P; //Total paths evaluated
	double m_sum; //Sum of payoffs
	double m_sum2; //Sum of Payoff^2
	double m_minP0; //Min Payoff
	double m_maxP0; //Max Payoff
  public:
	OPPathEval(Option const* a_option):m_option(a_option),m_P(0),m_sum(0),m_sum2(0),m_minP0(INFINITY),m_maxP0(-INFINITY){assert(m_option!=nullptr;}
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
	std::pair<double,double> GetPxStats()const{
			if(m_P<2){printf("Empty OpPathEval in GetPxStats\n"); }
			double px=m_sum/double(m_P);
			double var=(m_sum2-double(m_P)*px*px)/double(m_P-1);
			assert(var>=0);
			double err=(px!=0)?sqrt(var)/fabs(px):sqrt(var);
			return std::make_pair(px, err);
	}


};


}

using namespace SiriusFM;
using namespace std;

int main(int argc, char** argv)
{
  printf("Hello!\n");
  if (argc!=9){printf("Usage: mu, sigma, S_0, call/put, K, T_days, tau_min,P\n"); return -1;}
  double mu=atof(argv[1]);
  double sigma=atof(argv[2]);
  double s0=atof(argv[3]);
  char const* optType=argv[4];
  double K=atof(argv[5]);
  long T_days=atof(argv[6]);
  int tau_min=atoi(argv[7]);
  long P=atoi(argv[8]);
  if (sigma<=0 || s0<=0 || T_days<=0 || tau_min<=0 || P<=0){printf("Bad values in input!\n"); return -2;}
  
  CcyE ccyA=CcyE::USD;
  CcyE ccyA=CcyE::USD;
  IRProvider<IRModeE::Const> irp(nullptr);
  DiffusionGBM diff(mu, sigma,s0);

  MCEngine1D<decltype(diff),decltype(irp), decltype(irp), decltype(ccyA), decltype(ccyA),OPPathEval> mce(20000,20000);

  Option const* option=nullptr;
  if (strcmp(optType,"call")==0) {option=new EurCallOption(K,T_days);}
  if (strcmp(optType,"put")==0) {option=new EurPutOption(K,T_days);}


  time_t t0=time(nullptr);
  time_t T=t0+T_days*86400;
  double T_years=(double)(T_days)/365.25;

  OPPathEval pathEval(option);
  
  mce.Simulate<true>(t0,T,tau_min,P,true,&diff,&irp,&irp,ccyA,ccyA,&pathEval);
  
  
  auto res=pathEval.GetPxStats;
  double px=res.first;
  double err=res.second;
  printf("Price=%f\n",px);
  printf("RelErr=%f\n",err);


  delete option;
   
  
  printf("Goodbuy!\n");
  return 0;

}
