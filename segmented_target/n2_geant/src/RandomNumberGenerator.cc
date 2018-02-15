/** \file
 * Random number generator stuff (implementation)
 */
#include <time.h>
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <gsl/gsl_sf_bessel.h>
#include <gsl/gsl_histogram.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "RandomNumberGenerator.hh"

using namespace std;

Geant4GSLrng::Geant4GSLrng() {
  gsl_rng_env_setup();
  fT = gsl_rng_default;
  fR = gsl_rng_alloc (fT);
 }

Geant4GSLrng::~Geant4GSLrng() {
  gsl_rng_free(fR);
 }


double Geant4RNG::value(double l, double u) {  // this makes a cut in the distribution
  //if (l>=u) ERR("l>=u: %f %f",l,u);
  double v;
  do v = value(); while (v < l || v > u);
  return v;
}

// ######################################################

Geant4RNGGaussGSL::Geant4RNGGaussGSL(Geant4GSLrng* rr, double c, double s) : fR(rr) {
  fCentroid=c; 
  fSigma=s;
}
double Geant4RNGGaussGSL::value () {
  if (fSigma == 0) return fCentroid;
  return fCentroid + gsl_ran_gaussian(fR->getr(),fSigma);
}

// ######################################################





