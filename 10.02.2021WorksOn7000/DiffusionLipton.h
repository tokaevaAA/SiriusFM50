#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdexcept>

namespace SiriusFM{

class DiffusionLipton{
private:
	double const  m_mu;
	double const m_sigma0;
	double const m_sigma1;
	double const m_sigma2;
public:
	DiffusionLipton(double a_mu, double a_sigma0, double a_sigma1, double a_sigma2):m_mu(a_mu), m_sigma0(a_sigma0),m_sigma1(a_sigma1),m_sigma2(a_sigma2) {if (m_sigma0 < 0 or m_sigma2<=0 or m_sigma1*m_sigma1 -4*m_sigma0*m_sigma2 >=0 ) {printf ("Invalid input in DiffusionLipton.\n");throw std::invalid_argument("Invalid syntax in DiffusionLipton.");}}	
	~DiffusionLipton(){}
	double mu(double s, double t)const {return m_mu*s; }
	double sigma(double s, double t)const{return m_sigma0+m_sigma1*s+m_sigma1*s*s;}
	void pechat(){printf("mu=%f,sigma0=%f,sigma1=%f,sigma2=%f\n",m_mu,m_sigma0,m_sigma1,m_sigma2);}
	



};

}