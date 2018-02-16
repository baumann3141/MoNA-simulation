/// \file

#ifndef ST_PROPAGATE_HH
#define ST_PROPAGATE_HH

#include <string>
#include "st_common_defs.hh"
#include "st_ble.hh"
#include "st_particle.hh"
#include "st_cosy_map.hh"

/** General propagator, i.e. z (distance along beam-axis) is changed
 *
 */
class StPropagator : public StBLE {
public:
  StPropagator(string name, double z = 0) : StBLE(name), distance(z) {}  ///< constructor, sets the distance
  virtual ~StPropagator() {}                 ///< destructor

  /// set the propagation distance
  /// \param z distance
  virtual void setDistance(double z = 0) {distance = z;}
  double getDistance() {return distance;}

  //@{
  /// propagate particle
  int act(StParticle* p) {return propagate(p);}
  int propagate (StParticle* p) {
    p->setz(p->getz() + distance);
    return prop(p);
  }
  //@}

private:
  virtual int prop(StParticle*) = 0;   ///< must be reimplented by daughter classes

protected:
  double distance;                     ///< propagation distance
};



/** Drift of particle in free space.
 *
 */
class StPropDrift : public StPropagator {
public:
  /// constructor
  /// \param z drift distance
  StPropDrift(double z = 0);
  
  /// set distance and update name
  void setDistance(double z = 0);

private:  
  /// do the propagation 
  int prop(StParticle* p);
};




/** Propagation of particle via a map (magnet (or anything) map of cosy)
 *
 */
class StPropMapCosy : public StPropagator {
public:
  StPropMapCosy(CosyMap m);

private:
  /// to the propagation trough the magnet
  int prop(StParticle* p);
  CosyMap map;
};



#endif
