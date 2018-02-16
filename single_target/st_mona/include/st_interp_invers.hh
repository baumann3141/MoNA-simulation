/** \file
 * class for interpolating between points of a lookup table and
 * the inverse (root finding)
 * using the GSL!!!  (no base class without GSL)
 */

#ifndef ST_INTERP_INVERS_HH
#define ST_INTERP_INVERS_HH

#include <set>
#include <gsl/gsl_interp.h>
#include "deb_err.h"

using namespace std;

/// aux class for the data
class Data {
public:
  double x,y;
};

class ltData {
public:
  bool operator() (Data d1, Data d2) {return (d1.x - d2.x) < 0;}
};


/** interface for interpolation class 
 */
class StInterp {
public:
  StInterp() : fXarray(0), fYarray(0), fGSLInterp(0), fYisOrdered(false) {}

  void add(double x, double y);
  void print();

  virtual void   setup() = 0;               ///< setup
  virtual double operator()(double v) = 0;  ///< interp. value
  virtual double inv(double v) = 0;         ///< the inverse (if Y is also ordered) !!!

  double val(double v) {return operator()(v);}

  virtual ~StInterp() {}

protected:
  set<Data,ltData> fData;
  double* fXarray;
  double* fYarray;
  gsl_interp* fGSLInterp;
  gsl_interp* fGSLInterpInv;
  bool fYisOrdered;
};


/// linear interpolation
class StInterpLin : public StInterp {
public:
  void   setup();
  double operator()(double v);
  double inv(double v);
  void   reset();

  ~StInterpLin() { reset(); }
};


#endif  // ST_INTERP_INVERS_HH

