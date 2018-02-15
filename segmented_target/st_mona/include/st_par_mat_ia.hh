/** \file
 * Description of interaction of particles and materials
 *
 */

#ifndef ST_PAR_MAT_IA_HH
#define ST_PAR_MAT_IA_HH

#include <map>
#include "st_material.hh"
#include "st_particle.hh"
#include "st_interp_invers.hh"

extern "C" {
#include "rageny.h"
#include "CycSrim.h"
}

/** Class containing target and beam information.
 *  Used to make a binary memcmp() to see if this target-beam combination
 *  already has a interpolation table generated
 */
class RagenyTargetBeam {
public:
  rageny_target_t t;  ///< rageny target type
  double ap;          ///< A of projectile
  double zp;          ///< Z of projectile
};

/** Class for comparing two RagenyTargetBeam instances
 *  Only operator() is needed.
 */
class ltRagenyTB {
  public:
  /// comparison operator
  bool operator()(RagenyTargetBeam a, RagenyTargetBeam b){
    ////////
    //    return memcmp(&a, &b, sizeof(a)) < 0;
    //  Formerly memcmp was used here, but it led to unpredictable
    //  behavior (for example, on occasion not evaluating correctly
    //  on certain platforms with certain compilers, etc). For reasons
    //  why do a web search on "memcmp compare structs" and read some
    //  of the results. It has been replaced with an element-by-element
    //  comparison.
    //  -GC
    ////////
    for(int i=0; i< MATMAX; ++i) {
      if(a.t.a[i] < b.t.a[i]) return 1;
      if(a.t.z[i] < b.t.z[i]) return 1;
      if(a.t.r[i] < b.t.r[i]) return 1;
    }
    if(a.t.n < b.t.n) return 1;
    if(a.t.t < b.t.t) return 1;

    if(a.ap < b.ap) return 1;
    if(a.zp < b.zp) return 1;

    else return 0;
  }
};

class StMaterial;

 /** Interface class for interaction between particles and matter.
 *
 */
class StParMatInteraction {
public:
//void setCycSrim(CycSrim* test) {srimTarget = test;}

  /// do calculation
  virtual double do_de(StMaterial* m, double thickness, StParticle* p, int range=0) = 0;
  /// do calculation
  virtual double do_de(StMaterial* m, StParticle* p, int range=0);
  virtual double do_de_foil(StMaterial* m, double thickness, StParticle* p) = 0;
// Rageny Definitions 
   // NOTE: These definitions appear AGAIN below in the StRageny Class

//  virtual double range(StMaterial* m, StParticle* p) = 0;
//  virtual double energy(StMaterial* m, double thickness, StParticle* p) = 0;

// CycSrim Definitons
  virtual double range(CycSrim* fSrimTarget, StParticle* p) = 0;
  virtual double range_foil(CycSrim* fSrimTarget, StParticle *p) = 0;
  virtual double energy(CycSrim* fSrimTarget, double thickness, StParticle* p) = 0;
  virtual double energy_foil(CycSrim* fSrimTarget, double thickness, StParticle* p) = 0;

  //@{
  /** angular stragling
   *  Calculates the angular straggling, particle p will be updated.
   *  \return sigma_theta
   */
  virtual double stragAng(StMaterial* m, StParticle* p);
  virtual double stragAng(StMaterial* m, double thickness, StParticle* p, double scale=1) = 0;
  //@}
  //CycSrim *srimTarget;
  //void setCycSrim(CycSrim* test) {srimTarget = test;}
  /// destructor
  virtual ~StParMatInteraction() {}
};


/// Energy loss using rageny
class StRageny : public StParMatInteraction {
public:

  /// constructor.  If flag is set to one then use lookup table
  StRageny(int lookup=1, double stragFactor = 1) : 
    fLookUp(lookup), fRngUniform(0), fRngGauss(0), 
    fStragFactor(stragFactor),fSrimTarget(0) {
    if (fLookUp) INFO("Setting up energy loss with interpolator");
  }


  /** do calculation
   * \param m          Material pointer
   * \param thickness  Thickness in mg/cm^2
   * \param p          Particle pointer (particle energy will be updated)
   * \return Energy loss in MeV
   */
  double do_de  (StMaterial* m, double thickness, StParticle* p, int range=0);
  double do_de_foil (StMaterial* m, double thickness, StParticle* p);
  // CycSrim Definitions
  double range(CycSrim* fSrimTarget, StParticle* p);
  double range_foil(CycSrim* fSrimTarget, StParticle* p);
  double energy(CycSrim* fSrimTarget, double thickness, StParticle* p);
  double energy_foil(CycSrim* fSrimTarget, double thickness, StParticle* p);

//  Rageny Definitions

//  double range(StMaterial* m, StParticle* p);
//  double energy(StMaterial* m, double thickness, StParticle* p);


  /** angular stragling
   *  Calculates the angular straggling, particle p will be updated.
   *  \return   \f$\theta\f$ in (rad)
   */
  double stragAng(StMaterial* m, double thickness, StParticle* p, double scale=1);

  
private:
#ifdef TEST
public:
 
#endif
  /** Calculation of \f$\sigma_\alpha\f$.
   *  \param z1    Projectile charge number
   *  \param z2    Target charge number
   *  \param t     Target thickness (mg/cm^2)
   *  \param A2    Target mass number
   *  \param e     Kinetic energy (NOT per nucleon!!!)
   *  \return \f$\sigma_\alpha\f$ in (rad)
   */
  double stragAng(double z1, double z2, double t, double A2, double e);

public:
  void setRngUniform(StRNGUniform* p) {fRngUniform = p;}
  void setRngGauss  (StRNGGauss*   p) {fRngGauss   = p;}
  void setCycSrim (CycSrim* p) {fSrimTarget = p;}
  void setSrimFoil (CycSrim* p) {fSrimFoil = p;}
private:
  /// look up table
  map<RagenyTargetBeam,StInterp*,ltRagenyTB> fTargetBeamTable;

  /// use lookup table if this flag is set
  int fLookUp;

  /// store rageny data
  rageny_data_t fData;

  /// RNGs needed for straggling
  StRNGUniform* fRngUniform;
  StRNGGauss*   fRngGauss;
  CycSrim*      fSrimTarget;
  CycSrim*      fSrimFoil;

  /// strag parameter
  double fStragFactor;
};

#endif
