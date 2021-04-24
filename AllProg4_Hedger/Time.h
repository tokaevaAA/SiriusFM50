#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdexcept>
#include <cmath>

namespace SiriusFM{

inline double YearFrac(time_t a_t){
  //Avg year in seconds = avg year in days * seconds in a day
  constexpr double SecY=365.25*86400.00;
  return 1970.0 + double(a_t)/SecY;
  }

}