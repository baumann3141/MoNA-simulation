/** \file
 * Random number generator stuff...
 */
#ifndef GEANt4_RNG_HH
#define Geant4_RNG_HH

#include <fstream>
#include <iostream>
#include <string>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_histogram.h>

/// pure virutal random number generator
class Geant4RNG {
public:
  virtual double value() = 0; ///< return random value
  virtual double value(double l, double u); ///< return random value between l and u
  virtual ~Geant4RNG() {}  ///< destructor
};
//-----##########################################################-----//

/// Gauss RNG -----------///
class Geant4RNGGauss {
public:

  void setSigma(double v)    { fSigma    = v; }   /// set sigma
  void setCentroid(double v) { fCentroid = v; }   /// set centroid

protected:

  double fCentroid;   /// centroid of gauss
  double fSigma;   /// sigma of gauss

};
//-----##########################################################-----//

//--------------############################################----------//
///----------------------- aux class for GSL rng --------------------///
//--------------############################################----------//

class Geant4GSLrng {
public:
  /// constructor, takes everything from the environment
  /// \todo rng with given seed...
  Geant4GSLrng();

  /// destructor 
  ~Geant4GSLrng();

  /// get gsl_rng pointer
  gsl_rng* getr() {return fR;}

private:
  /// rng type
  const gsl_rng_type* fT;
  /// rng
  gsl_rng* fR;
};
//-----##########################################################-----//

/// Gauss RNG using GSL
class Geant4RNGGaussGSL : public Geant4RNGGauss {
public:
  /// constructor
  Geant4RNGGaussGSL(Geant4GSLrng* rr, double c=0, double s=1);
  
  /// destructor
  ~Geant4RNGGaussGSL() {}

  /// return value
  double value ();

private:
  /// GSL random number generator
  Geant4GSLrng* fR;
};
//-----##########################################################-----//


#endif
