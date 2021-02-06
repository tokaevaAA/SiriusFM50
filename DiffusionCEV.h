#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdexcept>
#include <cmath>

namespace SiriusFM{

class DiffusionCEV{
private:
	double const  m_mu;
	double const m_sigma;
	double const m_beta;
public:
	DiffusionCEV(double a_mu, double a_sigma, double a_beta):m_mu(a_mu), m_sigma(a_sigma),m_beta(a_beta) {if (m_sigma <= 0 or m_beta <0) {printf("Invalid input in DiffusionCEV.\n"); throw std::invalid_argument("Invalid input in DiffusionCEV.");}}	
	~DiffusionCEV(){}
	double mu(double s, double t)const {return m_mu*s; }
	double sigma(double s, double t)const{return m_sigma*pow(s,m_beta);}
	void pechat(){printf("mu=%f,sigma=%f,beta=%f\n",m_mu,m_sigma,m_beta);}
	



};

}