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
      encode_list_header(result, type, len);
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
      encode_list_header(result, type, len);
      vi = INTEGER(es);
      i = 0;      
      while(i < len){	
	ei_x_encode_long(result,*(vi+i));
	i++;
      }
      if(ei_x_encode_empty_list(result)){
      }
    }   
  }else if(type == STRSXP){
    if(len>0){
      encode_list_header(result, type, len);
      i = 0;
      while(i < len){	
	ei_x_encode_string(result,CHAR(STRING_ELT(es,i)));
	i++;
      }
      if(ei_x_encode_empty_list(result)){
      }
    }
  } else if (type == VECSXP) {
    encode_long(result, type, exp);
  } else {
    encode_long(result, type, exp);
  }
  return 0;
}

void encode_list_header(ei_x_buff* result, int type, unsigned int len) {
  char type_str[16];
  type_to_string(type, type_str, sizeof(type_str));

  if (ei_x_encode_atom(result, "ok") ||
      ei_x_encode_atom(result, type_str) ||
      ei_x_encode_list_header(result, len)) {
  }
}

void encode_long(ei_x_buff* result, int type, long exp) {
  char type_str[16];
  type_to_string(type, type_str, sizeof(type_str));

  if (ei_x_encode_atom(result, "ok") ||
      ei_x_encode_atom(result, type_str) ||
      ei_x_encode_long(result, exp)) {
  }
}

void type_to_string(int type, char* type_str, int buffersize) {
  // for SEXP types see:
  // https://cran.r-project.org/doc/manuals/r-release/R-ints.html#SEXPTYPEs
  const char* type_strings[] = {"NILSXP", "SYMSXP", "LISTSXP", "CLOSXP",
				"ENVSXP", "PROMSXP", "LANGSXP", "SPECIALSXP",
				"BUILTINSXP", "CHARSXP", "LGLSXP",
				"UNKNOWNSXP", "UNKNOWNSXP",
				"INTSXP", "REALSXP", "CPLXSXP", "STRSXP",
				"DOTSXP", "ANYSXP", "VECSXP", "EXPRSXP",
				"BCODESXP", "EXTPTRSXP", "WEAKREFSXP",
				"RAWSXP", "S4SXP"};
  if (!type_str || buffersize < 12)
    return;
  if (type == -1) {
    strncpy(type_str, "ERRORSXP", buffersize-1);
  } else if (type < 0 || type > 25) {
    strncpy(type_str, "UNKNOWNSXP", buffersize-1);
  } else {
    strncpy(type_str, type_strings[type], buffersize-1);
  }
  type_str[buffersize-1] = '\0';
}
