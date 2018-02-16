/// \file 

#include <math.h>
#include <gsl/gsl_pow_int.h>

#include "st_propagate.hh"


StPropDrift::StPropDrift(double z) : StPropagator("StPropDrift",z) {}

void StPropDrift::setDistance(double z) {
    distance=z;  
    char c[BLEN];
    snprintf(c,BLEN,"Free drift of %g m.",z);
    setName(c);
  }


/// Drift
int StPropDrift::prop(StParticle* p) {
  if(distance == 0) return 0;
  double x, y, tx, ty;
  x  = p->getx();
  y  = p->gety();
  tx = p->gettx();
  ty = p->getty();

  // x and y
  p->setx(x + distance * tan(tx));
  p->sety(y + distance * tan(ty));

  // path
  double path;
  path = distance * sqrt(1 + tan(tx)*tan(tx) + tan(ty)*tan(ty));
  p->setpath(p->getpath() + path);

  // time
  /// \todo replace 0.3 by properly defined constant
  p->sett(p->gett() + path / p->getVelo() );
  
  return 0;
}


/// CosyMap
StPropMapCosy::StPropMapCosy(CosyMap m) : StPropagator("StPropMapCosy",m.getLen()), map(m) {}


/// CosyMap
int StPropMapCosy::prop(StParticle* p) {
  if (p->getQ() == 0) {  // do free drift for not charged particles 
    // setup drift
    StPropDrift d(this->map.getLen());
    // we need to remember the z of the particle, 
    // as it will be increased twice otherwise
    double z = p->getz();
    d(p);      
    p->setz(z);
    return 0;  
  }

  // now apply the map, but first check that the map is for the right particle
  int wrongMap;
  wrongMap = 0;
  if ( p->getA() != map.getA() ) {
    WAR("Map and particle do not have same A.  Map: %d, P: %d",map.getA(), p->getA());
    wrongMap = 1;
  }
  if ( p->getQ() != map.getQ() ) {
    WAR("Map and particle do not have same Q.  Map: %d, P: %d",map.getQ(), p->getQ());
    wrongMap = 1;
  }

  double x, y, tx, ty;       ///< coordinates before mapping

  x  = p->getx();
  tx = p->gettx();
  y  = p->gety();
  ty = p->getty();

  //printf("tx %f.8 and ty %f.8\n",tx,ty);

  // path
  double path;
  path = distance * 1.0;  /// \todo need some work here!!!

  double len = 0.0;           /// \todo implement len
  double delta;
  
  double m, p0, e0; // nominal momentum and energy of map
  p0 = map.getBrho() * map.getQ() * 299.792458;
  m  = map.getA()*931.494028;
  e0 = sqrt(m*m + p0*p0) - m;
  /// \todo implement map.getEkin()...
  /// maybe make a StParticle be part of the map to store this info

  double eKin;    // kinetic energy of particle
  eKin = p->getEkin();  

  DSV(e0);
  DSV(eKin);

  if (wrongMap) { // recalculate Ekin to correspond to same Brho
    DEB("Recalculating Ekin for A=%d, Q=%d", map.getA(), map.getQ());
    DSV(p->getBrho()/map.getBrho()-1);
    StParticle dp;  // dummy particle
    dp = *p;
    double dbrho;   // dummy brho
    dbrho = dp.getBrho();
    dp.setA(map.getA());
    dp.setQ(map.getQ());
    dp.setm(dp.getA()*931.494028);
    dp.setBrho(dbrho);
    eKin = dp.getEkin();
    DEB("recalculated Ekin %g",eKin);
  }

  DSV(eKin);

  delta = eKin/e0 - 1.0;
  DSV(delta);

  double co[6]  = { x, tx, y, ty, len, delta};  // coordinate array
  double cop[6] = { 0,  0, 0,  0,   0,     0};  // coordinate array after mapping

  for (size_t i(0); i < map.size(); i++) {  // loop over map entries

    double f = 1;  // factor
    for (size_t j(0); j<6 ; j++) {
      f *= gsl_pow_int(co[j],map.getLine(i).power[j]);
    }
    
    // calculate new coordinates
    for (size_t k(0); k<5; k++) {
      cop[k] += map.getLine(i).coef[k] * f;
    }
  }
  p->setx (cop[0]);
  p->settx(cop[1]);
  p->sety (cop[2]);
  p->setty(cop[3]);

  path += cop[4];
  
  p->setpath(p->getpath() + path);
  p->sett   (p->gett()    + path / p->getVelo());

  return 0;  /// \todo check return values
}

