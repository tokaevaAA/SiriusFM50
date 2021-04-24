#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdexcept>

namespace SiriusFM{

class DiffusionGBM{
private:
	double const  m_mu;
	double const m_sigma;
	double m_s0;
public:
	DiffusionGBM(double a_mu, double a_sigma,double a_s0):m_mu(a_mu), m_sigma(a_sigma), m_s0(a_s0) {if (m_sigma <= 0) {printf ("Invalid input in DiffusionGBM.\n");throw std::invalid_argument("Invalid syntax in DiffusionGbm.");}}	
	~DiffusionGBM(){}
	double mu(double s, double t)const {return m_mu*s; }
	double sigma(double s, double t)const{return m_sigma*s;}
	double get_s0() const {return m_s0;}
	void pechat(){printf("mu=%f,sigma=%f\n",m_mu,m_sigma);}
	



};

}