#include <stdio.h>

#include "eri.h"
#include "Rengine.h"


int setup(){  
  int res =  r_setup();
  return res;
}

int terminate(){
  return 1;
}

long parse(const char *s){ 
  return r_parse(s);
}

int erl_eval_convert(long inexp, ei_x_buff *result){
  long exp;
  int er = 0;
  SEXP es;
  int type, i;
  double *vd;
  int *vi;  
  unsigned int len;

  exp = r_eval(inexp, &er);

  if (er == 0) { // all ok
    es = L2SEXP(exp);
    type = TYPEOF(es);
    len = LENGTH(es);
  } else {       // R error
    type = -1;
  }

  if(type == REALSXP){    
    if(len > 0){   
      if(ei_x_encode_atom(result,"ok") || ei_x_encode_atom(result,"REALSXP") || 
	 ei_x_encode_list_header(result,len)){
      }
      vd = REAL(es);
      i = 0;      
      while(i < len){
	ei_x_encode_double(result,*(vd+i));	
	i++;
      }
      if(ei_x_encode_empty_list(result)){
      }
    }    
  }else if(type == INTSXP){    
    if(len > 0){
      if(ei_x_encode_atom(result,"ok") || ei_x_encode_atom(result,"INTSXP") || 
	 ei_x_encode_list_header(result,len)){
      }
      vi = INTEGER(es);
      i = 0;      
      while(i < len){	
	ei_x_encode_long(result,*(vi+i));
	i++;
      }
      if(ei_x_encode_empty_list(result)){
      }
    }   
  }else if(type == VECSXP){
    if(ei_x_encode_atom(result,"ok") || ei_x_encode_atom(result,"VECSXP") ||
       ei_x_encode_long(result,exp)){
    }
  }else if(type == STRSXP){
    if(len>0){
      if(ei_x_encode_atom(result,"ok") || ei_x_encode_atom(result,"STRSXP") || 
	 ei_x_encode_list_header(result,len)){
      }
      i = 0;
      while(i < len){	
	ei_x_encode_string(result,CHAR(STRING_ELT(es,i)));
	i++;
      }
      if(ei_x_encode_empty_list(result)){
      }
    }
  } else if (type == -1) {
    if(ei_x_encode_atom(result,"ok") || ei_x_encode_atom(result,"ERROR") ||
       ei_x_encode_long(result,exp)){
    }    
  }else{
    if(ei_x_encode_atom(result,"ok") || ei_x_encode_atom(result,"OTHER") ||
       ei_x_encode_long(result,exp)){
    }
  }  
  return 0;
}
