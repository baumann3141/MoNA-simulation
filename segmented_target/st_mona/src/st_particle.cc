 /** \file
 *
 */

#include <stdio.h>
#include <TVector3.h>

#include "st_particle.hh"

StParticle::StParticle() {
  // set all values to zero
  memset(&fData,0,sizeof(fData));
}


void StParticle::print(int flag) {
  if (flag) printHeader();
  printf("%1d ",(int) getID());
  printf("%3d %3d "    ,fData.Z, fData.A);
  printf("%3d %4.2g "  ,fData.Q, fData.m - fData.A*931.494028);  /// \todo replace 931.5 with constant
  printf("%5.5f %5.5f %5.5f %5.5f %9.3g ", 
         fData.x*1000, fData.tx*1000, fData.y*1000, fData.ty*1000, fData.z);
  printf("%9.3g %9.3g %1.8f %1.8f %9.5f %9.4g ",
         getGammaMinOneU(), fData.Ekin,getBeta(), getGamma(), getBrho(), getP());
  printf("%9.3g %9.3g", fData.path, fData.t);
  printf("\n");
  fflush(0);

} 


void StParticle::printHeader () {
  printf("%1s %3s %3s %3s %4s %7s %7s %7s %7s %9s %9s %9s %9s %9s %9s %9s %9s %9s",
         "#","Z", "A","Q","ex","x","tx","y","ty","z","(g-1)u","Ekin","beta","gamma","Brho","P","path","t"); 
  printf("%1s %3s %3s %3s %4s %7s %7s %7s %7s %9s %9s %9s %9s %9s %9s %9s %9s %9s\n",
         "", "", "","" ,"MeV","mm","mrad","mm","mrad","m","MeV","MeV","","","Tm","MeV/c","m","ns"); 
}

double StParticle::getTheta() {
  double sx,sy; // slope x,y
  sx=getSlopeX();
  sy=getSlopeY();
  double t;
  t = acos(1./sqrt(sx*sx + sy*sy + 1.));
  DSV(t/M_PI*180.);
  return t;
}


double StParticle::getPhi()   {
  double v;
  v = atan2(getSlopeY(),getSlopeX());
  DSV(v/M_PI*180.);
  return v;
}


void StParticle::setThetaPhi(double t, double p) {
  double x,y,z; // x y z coordinate on unit sphere
  x = sin(t)*cos(p);
  y = sin(t)*sin(p);
  z = cos(t);
  /// \todo check for z==0
  setSlopeX(x/z);
  setSlopeY(y/z);
}


void StParticle::addThetaPhi(double theta, double phi) {
  TVector3 v(0,0,1);
  v.SetMagThetaPhi(1,theta,phi);
  v.RotateY(getTheta());
  v.RotateZ(getPhi());
  setThetaPhi(v.Theta(),v.Phi());
}

/// \test this still needs to be tested test test

