/** \file
 * Things to initialize particles.  Inherited from StBLE
 */

#ifndef ST_PART_MISC_HH
#define ST_PART_MISC_HH

#include "st_ble.hh"
#include "st_particle.hh"
#include "st_rng.hh"
#include "st_part_init.hh"


/** Particle initializer (dummy class; not really needed)
 
class StPartInitializer : public StBLE {
public:
  StPartInitializer(string name) : StBLE(name) {}
};


/// when called first saves all particle(s) information
/// when called next time(s) restores particle(s) to that
class StPartInitSave : public StPartInitializer {
public:
  StPartInitSave() : StPartInitializer("StPartInitSave"), first_time(1) {}
  
  int act(vector <StParticle> *pp) {
    if (first_time) {
      for (size_t i = 0; i<pp->size(); i++) {
        pv.push_back(pp->at(i));
      }
      first_time = 0;
    } else {
      pp->clear();
      for (size_t i = 0; i<pv.size(); i++) {
        pp->push_back( pv.at(i) );
      }
    }    
    return 0;
  }

  int act(StParticle* p) {
    p=p; /// remove no-used-warning
    cout << "E: don't call this function!!!"<<endl; 
    return -1;
  } ///\todo fix this (ERR macro)
  
private:
  vector <StParticle> pv;
  int first_time;
};
*/

/** Generate random particle positions x,y and angles tx,ty and energies
 */
class StPartInitRandomMisc : public StPartInitializer {
public:
  /// constructor
  StPartInitRandomMisc(StRNG* rngx=0, 
                        StRNG* rngtx=0, 
                        StRNG* rngy=0, 
                        StRNG* rngty=0, 
                        StRNG* rnge1=0,
			StRNG* rnge2=0,
			double xtx_slope=0,
			double yty_slope=0);

  /// act
  /// \param p particle pointer
  int act(StParticle* p);

private:
  /// random generator for x,y, tx, ty, e
  StRNG *rx,*rtx,*ry,*rty,*re1,*re2;
  /// slopes of x vs. tx and y vs. ty
  double sl_xtx, sl_yty;
  inline double liseFit(double xx){ return (2.99389e-3)*xx*xx - (7.46751e-1)*xx + 46.576;}
};

#endif

