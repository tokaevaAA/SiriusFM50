#include <stdio.h>
#include <stdlib.h>
#include <stdexcept>
#include "IRProvider.h"
#include "IRProviderConst.h"

namespace SiriusFM{


IRProvider<IRModeE::Const>::IRProvider(const char* a_file){
  for (int i=0; i<int(CcyE::N); i=i+1) {m_IRs[i]=0;}
  if (a_file==nullptr || *a_file=='\0'){printf("Cannot open file\n");return;};
  FILE*f;
  f=fopen(a_file,"r");
  if (f==0){printf("Cannot open file in IRProvider<IRModeE::Const>::IRProvider\n");}
  char buf[128];
  
  char* k=fgets(buf,128,f);
  while (k != NULL) {
    //printf("%s\n",buf);
    char small_buf[4];
    small_buf[0]=buf[0];
    small_buf[1]=buf[1];
    small_buf[2]=buf[2];
    small_buf[3]='\0';
    
    CcyE tekcur=Str2CcyE(small_buf);
    double r=atof(buf+4);
    m_IRs[int(tekcur)]=r;
    k=fgets(buf,128,f);
   

  }
  //for (int i=0; i<int(CcyE::N); i=i+1) {printf("%f\n",m_IRs[i]);}



}


}