/** \file
 * Beam line element.
 */

#include <cstdio>
#include <cassert>
#include <cctype>  // tolower
#include <vector>
#include <algorithm>
#include <string>

#include "st_ble.hh"
#include "st_system.hh"
#include "st_common_defs.hh"

#include "deb_err.h"
#include "CycSrim.h"

using namespace std;


int StBLE::act(vector<StParticle>* pp) {

  /// number of particles
  size_t np = pp->size(); /// need to introduce this variable, so that possibly added particles
                          /// are not immediately looped over also

  for (size_t i = 0; i < np; i++) {
    act(&pp->at(i));
     //double zpos = pp->at(i).getz();
     //cout << "particle z-coordinate: " << zpos  << endl;

  }

  // fill n-tuple AFTER doing the action for all particles
  fillNtupFields(pp); //cout << "fillNtupFields(pp)" << endl;

  // do some clean up if there is the need
  cleanUp();

  // some consistency checks
  np =  pp->size();
  if (np > 0) {
    // all particle MUST have the same z-coordinate
    double z = pp->at(0).getz();
    for (size_t i = 1; i < pp->size(); i++) {
      double z1 = pp->at(i).getz();
       // cout << "particle z-coordinate: " << z1  << endl;
      if (z != z1 ) {
        ERR("particle %d has different z-cooridate z=%g z1=%g",i,z,z1);
      }
    }
  }

  return 0;
}


void StBLE::addNtupFields(string nn, int particle, StDetResp* detResp) {
  string n = nn;
  //cout << "addNtupFields: " << n << endl;

  char buf[BLEN];
  snprintf(buf,BLEN,"%d:%d-%s",(int) getID(),particle,n.c_str());
  
  assert(getSystemP());       ///\todo replace assert with own better assert
  if ( ! getSystemP()->getTupleP()) {
    WAR("No n-tuple pointer set.  Cannot add field %6s for BLE %2d and particle %2d.",
        n.c_str(), getID(), particle);
    return;
  }
  
  getSystemP()->getTupleP()->addField(buf,detResp);
  fNtupFields.push_back(buf);
  fNtupParticles.push_back(particle);
}


void StBLE::fillNtupFields(vector <StParticle>* pp) {
  for (size_t i=0; i < fNtupFields.size(); i++) { //cout << "fNtupFields.size(): " << fNtupFields.size() << endl;
    string n = fNtupFields.at(i);
    int   pn = fNtupParticles.at(i);  // particle number 
//	cout << "particle number: " << pn << endl;
    if (pn >= (int) pp->size()) {
      //WAR("This particle does not exist.");  
      getSystemP()->getTupleP()->fillField(n,-777); /// \todo fix this; define error codes
      return;
    }
	//cout << "fillNtupFields!: " << n << endl;
    string     nn = n.substr(n.find("-")+1);
    StTuple*    t = getSystemP()->getTupleP();
    StParticle  p = pp->at(pn);

 /** \def STD_FIELD(AAA)
     Macro to define a standard field, i.e. a
     field which can be set by calling StParticle::getAAA()
  */
#define STD_FIELD(AAA) if (nn == #AAA) t->fillField(n,p.get ## AAA ()); else

    if (0) {} else
    STD_FIELD(Z)
    STD_FIELD(A)
    STD_FIELD(Q)
    STD_FIELD(m)
    STD_FIELD(x)
    STD_FIELD(tx)
    STD_FIELD(y)
    STD_FIELD(ty)
    STD_FIELD(z)
    STD_FIELD(Ekin)
    STD_FIELD(path)
    STD_FIELD(t)
    STD_FIELD(EperA)
    STD_FIELD(GammaMinOneU)
    STD_FIELD(Gamma)
    STD_FIELD(Beta)
    STD_FIELD(Velo)
    STD_FIELD(Etotal)
    STD_FIELD(P)
    STD_FIELD(Brho)
    {
      if ( fillSpecialNtupFields(n,pn) ) {
        WAR("Could not find anything for name %s.", n.c_str());
      }
    }
  }
}

