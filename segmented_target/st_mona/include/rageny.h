/** \file
 * Things to use with the rageny energy loss routines.
 */

#define MATMAX 5  ///< do not increase this; the rest of rageny is hardwired to this value

typedef struct {
  double a[MATMAX];  // mass number
  double z[MATMAX];  // charge number
  double r[MATMAX];  // ratio
  int n;            // number of active target materials
  double t;          // thickness
} rageny_target_t;

enum rageny_flag { 
  rageny_CALC_DONE, ///< calculation performed
  rageny_RANGE,     ///< use initial range to do calculation
  rageny_ENERGY     ///< use initial energy to do calculation
};

typedef struct {
  double ri;  // initial range
  double rf;  // final range
  double ti;  // initial energy
  double tf;  // final energy
  enum rageny_flag f;  // indicates if calculation was performed 
} rageny_data_t;

#include <string.h>

/// do calculation, depending on what is given
/// \return energy loss
double rageny_calc(double ap, double zp, rageny_target_t* t, rageny_data_t* d);

