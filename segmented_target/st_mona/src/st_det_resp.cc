/** \file
 *   Detector response (implementation)
 */


//#include <gsl/gsl_errno.h>
//#include <gsl/gsl_histogram.h>

#include "st_det_resp.hh"
#include "deb_err.h"

StDetRespBin::StDetRespBin(double min, double max, size_t nbins) {
  fHist = gsl_histogram_alloc(nbins);
  DSV(min);
  DSV(max);
  gsl_histogram_set_ranges_uniform(fHist, min, max);
  DSV(gsl_histogram_min(fHist));
  DSV(gsl_histogram_max(fHist));
}
  
double StDetRespBin::operator() (double v) {
  int i;
  size_t bin;
  double lower, upper, avg;
  
  gsl_error_handler_t* eh = gsl_set_error_handler_off();
  
  DSV(v);
  i = gsl_histogram_find (fHist, v, &bin);
  DSV(i);
  DSV(gsl_histogram_min(fHist));
  DSV(gsl_histogram_max(fHist));
  if (i != GSL_SUCCESS) {
    /*
      WAR("Value %g not in range of histogram (%g,%g).",v,
      gsl_histogram_min(fHist),
      gsl_histogram_max(fHist));
    */
    return v;
  }
  DSV(bin);
  i = gsl_histogram_get_range(fHist, bin, &lower, &upper);
  DSV(i);
  if (i != GSL_SUCCESS) ERR("Some error here...");
  avg = (lower+upper)/2;
  DSV(lower);
  DSV(upper);
  DSV(avg);
  gsl_set_error_handler(eh);
  return avg;
}

StDetRespBin::~StDetRespBin() {
  gsl_histogram_free(fHist);
}

