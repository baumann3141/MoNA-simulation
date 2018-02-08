/** \file 
    routines to return certain things from the 2003 mass evaluation file  
       mass.mas03
*/

#include "mass.ic"
#include "mass.h"
#include <string.h>

// function needed
//-----------------
// Z for min BE for given A
// N for min BE for given A
// N for min BE for given Z
// Z for min BE for given N
// Z for string Mg
// Z,N for string 32Mg, Mg32, (case insensitive)

/** The following functions return the requested quantity from one line
    given in the mass file.  The following two lines were copied from the 
    mass file:

format    :  a1,i3,i5,i5,i5,1x,a3,a4,1x,f13.5,f11.5,f11.3,f9.3,1x,a2,f11.3,f9.3,1x,i3,1x,f12.5,f11.3,1x
             cc NZ  N  Z  A    el  o     mass  unc binding unc     B  beta  unc    atomic_mass   unc

Therefore the following column start values result:
             cc NZ  N  Z  A    el  o     mass  unc binding unc     B  beta  unc    atomic_mass   unc
col-num-sta: 0  1   4  9 14    20 23    28    41    52    63         75     86     96    100    112      
*/
#define STL(s,l) strtol(s,l,10)
#define STD(s,l) strtod(s,l)
#define FFF(t,n,f,s,l) static t mass_line_##n(const char* line) {\
   char* a = strdup(line); \
   if (!a) ERR("could not allocate a"); \
   a[s+l]='\0'; \
   t value; \
   errno = 0; \
   value = f(&a[s],0); \
   if (errno) ERR("Error"); \
   free(a); \
   return value; \
 }

//things with underscore are errors
//  type  quant, conversion, col-start, len
FFF(   int,   n  , STL,  4, 5); //N 
FFF(   int,   z  , STL,  9, 5); //Z
FFF(   int,   a  , STL, 14, 5); //A
static char* mass_line_sym(const char* line) {
  static char s[4];
  unsigned int i,j;
  const char* a = line;
  memset(s, '\0', sizeof(s));
  i=0;
  j=20;  // index from mass table format
  while (a[j] == ' ') j++;  // skip white space
  while (i<4 && a[j] && a[j] != ' ') s[i++] = a[j++];
  return s;
}
FFF(double, me   , STD, 28,13); // mass excess (keV)
FFF(double, me_e , STD, 41,11); //    error
FFF(double, bea  , STD, 52,11); // binding energy (keV)
FFF(double, bea_e, STD, 63, 9); //    error 
FFF(   int, miur , STL, 96, 3); // (rough) value  (in u)
FFF(double, miuf , STD,100,12); // (fine) value   (in micro-u)
static double mass_line_miu(const char* line) {   // mass in micro-u
  double v;
  v  = mass_line_miur(line);
  v *= 1000000;
  v += mass_line_miuf(line);
  return v;
}
FFF(double, miu_e, STD,112,11); //   error
#undef FFF

int mass_data_setup(MassData** md) {
  unsigned int i,j, numIso;
  MassData* m;
  m = 0;
  numIso = (massIdxLast - massIdxFirst + 1);
  m = (MassData*) malloc(sizeof(MassData) * numIso);
  if (!m) ERR("no mass data pointer");                 
  j = 0;
  for (i = massIdxFirst; i <= massIdxLast; i++, j++ ) {
    const char* l = massTab[i];
    m[j].n     = mass_line_n    (l);
    m[j].z     = mass_line_z    (l);
    m[j].a     = mass_line_a    (l);
    m[j].bea   = mass_line_bea  (l);
    m[j].bea_e = mass_line_bea_e(l);
    m[j].me    = mass_line_me   (l);
    m[j].me_e  = mass_line_me_e (l);
    m[j].miu   = mass_line_miu  (l);
    m[j].miu_e = mass_line_miu_e(l);
    strcpy(m[j].s,mass_line_sym (l));
  }
  *md = m;
  return numIso;
}


#ifdef TEST

int main (int argc, char* argv[]) {
  int i;
  MassData* m=0;

  i = argc;
  if (0) {argv[0] = '\0';}
  i = 1;
  printf("line 87 (12C): %s\n",massTab[86]);
  printf("line 88 (12N): %s\n",massTab[87]);
  printf("line 87 n    : %d\n",mass_line_n    (massTab[86]));
  printf("line 87 z    : %d\n",mass_line_z    (massTab[86]));
  printf("line 87 a    : %d\n",mass_line_a    (massTab[86]));
  printf("line 87 bea  : %f\n",mass_line_bea  (massTab[86]));
  printf("line 87 bea_e: %f\n",mass_line_bea_e(massTab[86]));
  printf("line 87 me   : %f\n",mass_line_me   (massTab[86]));
  printf("line 87 me_e : %f\n",mass_line_me_e (massTab[86]));
  printf("line 87 me_e : %d\n",mass_line_miur (massTab[86]));
  printf("line 87 me_e : %f\n",mass_line_miuf (massTab[86]));
  printf("line 87 me_e : %f\n",mass_line_miu  (massTab[86]));
  printf("line 87 me_e : %f\n",mass_line_miu_e(massTab[86]));
  //                                                     1         2         3         4         5         6
  printf("test    me   : %30.15f\n",mass_line_me   ("123456789012345678901234567890123456.89012345678901234567890"));
  printf("                     90123456.8901 (this should be the test result)\n");
  //  printf("N Z mass: %d %d %30.15f",12,6,mass_mass(6,6)
  printf("first line (%4d): %s\n", massIdxFirst, massTab[massIdxFirst]);
  printf(" last line (%4d): %s\n", massIdxLast,  massTab[massIdxLast]);
  /*
  for(i=massIdxFirst; i<=massIdxLast; i++) {
    printf("%3d %3d %.3f\n",mass_line_n(massTab[i]),mass_line_z(massTab[i]),mass_line_bea(massTab[i]));
  }
  */

  i = 525;
  printf("line (%4d): %s\n", i,  massTab[i]);
  printf("line (%4d): %f\n", i,  mass_line_bea_e(massTab[i]));
  int numIso;
  numIso = mass_data_setup(&m);
  i = 1;//1771;
  for(i=0; i< numIso; i++) {
    printf("m[%4d].a n z : %3d%-3s %3d %3d %11.3f (%9.3f) %17.5f (%11.5f) \n",
           i, m[i].a, m[i].s, m[i].n, m[i].z, m[i].bea, m[i].bea_e, m[i].miu, m[i].miu_e);
  }
  return 0;
}

#endif /* TEST */
