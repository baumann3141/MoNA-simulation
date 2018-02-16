/** \file
 */

#include "deb_err.h"
#include "stdlib.h"

typedef struct {
  int n;
  int z;
  int a;
  char s[3];
  double bea;
  double bea_e;
  double me;
  double me_e;
  double miu;
  double miu_e;
} MassData;

int mass_data_setup(MassData** md);

