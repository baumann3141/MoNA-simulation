/** \file
 *   Detector response
 */

#ifndef ST_DET_RESP_HH
#define ST_DET_RESP_HH

#include "st_rng.hh"
#include <gsl/gsl_histogram.h>


/// pure virtual class for detector response
class StDetResp {
public:
  virtual double operator() (double v) = 0;
  virtual ~StDetResp() {}
};


/// add the result of the RNG to the measured (exact) value
class StDetRespRNG: public StDetResp {
public:
  StDetRespRNG(StRNG* r) : fRng(r) {}
  double operator()(double v) {return v + fRng->value();}
private:
  StRNG* fRng;
};


/// bin result into predefined bins; say positions of a scintillator bar
/// use a gsl-histo for the bin definition
class StDetRespBin : public StDetResp {
public:
  StDetRespBin(double min, double max, size_t nbins);

  double operator() (double v);

  ~StDetRespBin();

private:
  gsl_histogram* fHist;
};

#endif
