#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdexcept>
#include <cmath>

namespace SiriusFM{

class DiffusionCIR{
private:
	double const  m_cappa;
	double const  m_teta;
	double const m_sigma;
public:
	DiffusionCIR(double a_cappa, double a_teta, double a_sigma):m_cappa(a_cappa),m_teta(a_teta), m_sigma(a_sigma) {if (m_sigma <= 0 or m_cappa <= 0 or m_teta<=0 ) {printf ("Invalid input in DiffusionCIR.\n");throw std::invalid_argument("Invalid syntax in DiffusionCIR.");}}	
	~DiffusionCIR(){}
	double mu(double s, double t)const{return m_cappa*(m_teta-s); }
	double sigma(double s, double t)const{return m_sigma*sqrt(s);}
	void pechat(){printf("cappa=%f, teta=%f,sigma=%f\n",m_cappa,m_teta,m_sigma);}
	



};

}