/** \file
 * Classes dealing with materials as beam line elements (BLE).
 * As BLE they just do mainly energy-loss and energy and angular straggling.
 */

#ifndef ST_MATERIAL_HH
#define ST_MATERIAL_HH


#include <vector>
#include "st_particle.hh"
#include "st_ble.hh"
#include "st_par_mat_ia.hh"
#include "st_rng.hh"
//#include "CycSrim.h"

using namespace std;

class StParMatInteraction;
class StReaction;
class CycSrim;

/** An element is defined by the charge number and an average atomic mass
 * 
 */
class StElement {
public:
  StElement() : Z(0), m(0), a(1) {}

  void setZ(int z) {Z=z;}      ///< set charge number Z
  int getZ() {return Z;}       ///< get charge number Z

  void setm(double mm) {m=mm;} ///< set (molar) mass m
  double getm() {return m;}    ///< get (molar) mass m

  void seta(double aa) {a=aa;} ///< set abundance
  double geta() {return a;}    ///< get abundance

  void set(string name);       ///< set element by name, e.g.: 9Be, 1H, 2H

  //private:
public:
  int Z;     ///< charge number 
  double m;  ///< average atomic mass
  double a;  ///< abundance
};






/** Class for materials consisting of one or more elements
 * 
 */
class StMaterial : public StBLE { 
public:
  StMaterial() : StBLE("Material"), fTotalAb(0), fThickness(0), 
                 fTotReacProb(0), fStragAngScale(1), 
                 fRngUniform(0), fRngThick(0), fRngbSpotx(0), bSpotx(0), bSpoty(0), bulge(0), flatfoil(0) {}
	        // friend CycSrim act(StParticle &p, CycSrim &srimTarget);

  /// add an element to the material
  void addElement(StElement e);
  
  /// load material if known by name
  void set(string mat);

  /// action (energy loss; straggling)
  int act(StParticle* p);

  //@{
  /// set/get thickness of material
  void   setThickness(double t) {        fThickness=t; }
  double getThickness()         { return fThickness;}
  //@}
  void   setBspotx(double bx)   {        bSpotx = bx;}
  double getBspotx()	    	{ return bSpotx;}

  void   setBspoty(double by)   {        bSpoty = by;}
  double getBspoty()	        { return bSpoty;}

  void   setBulge(double b)     {	 bulge = b;}
  double getBulge()		        { return bulge;}
  //@{
  /// set/get density of material
  void   setDensity(double d)   {        fDensity=d;}
  double getDenstiy()           { return fDensity;}
  //@{
  void   setTfoil(double t)     {         flatfoil=t;}
  double getTfoil()	            { return  flatfoil;}

  //@{
  /// set/get scale for straggling
  void   setStragAngScale(double v) {        fStragAngScale = v;}
  double getStragAngScale()         { return fStragAngScale;}
  //@}

  void setInteraction(StParMatInteraction* ii) { fPMInt = ii; }

  int fillSpecialNtupFields(string name, int n_part);
  //  double getTotalAb() {return total_ab;}

  int cleanUp();

  void SetLifeTime(double t);

  void SetActive(bool active){fActive = active;};

  void SetSilicon(bool silicon){fIsSi = silicon;};

  // private methods
private:

  /// aaa \todo energy straggling
  void do_staggle_energy();

  /// aaa \todo angular straggling
  void do_staggle_angle();

public:
  /// \todo good idea???
  vector <StElement>* getElemP() { return &fElem; }
  
public:
  /// set RNG for selection of thickness (usually between 0 and fThickness)
  /// \todo need to have a different one for each particle???
  void setRngThickness(StRNG* r)      { fRngThick   = r; }
  void setRngUniform(StRNGUniform* r) { fRngUniform = r; }
  void setRngbSpotx(StRNG* r) 	      { fRngbSpotx  = r; }//StRNG*
  void setRngbSpoty(StRNG* r) 	      { fRngbSpoty  = r; }//StRNG*

public: 
  void addReaction(StReaction* r);  ///< add reaction

private:  // data needed for energy loss
  vector <StElement> fElem;      ///< vector of elements
  double fTotalAb;               ///< total abundance; must be updated by addElement
  double fThickness;             ///< thickness (in mg/cm^2)
  double fDensity;               ///< density (\todo which units???)
  double bSpotx;
  double bSpoty;
  double bulge;
  double flatfoil;
  StParMatInteraction* fPMInt;   ///< interaction "model" to use

  TF1 *fdecay;
  // THR 17 March 2017: for uniform lifetime distribution
  double lt;
  // THR 29 June 2017: set whether reaction happens in this material
  bool fActive;
  // THR 3 July 2017: flag Be or Si
  bool fIsSi;

  vector <StReaction*> fReactions;  ///< reactions that can take place
  double fTotReacProb;              ///< total reaction probability

  // store things for later histo and n-tuple filling
  vector <double>      fEnergyLoss; ///< store the energy-loss of all particles
  vector <double>      fStragAng;   ///< store the strag angle
  vector <double>      fTargetPos;  ///< store the interaction position of all particles

  //
  double fStragAngScale;  ///< scale factor for straggling

  // needed RNGs
  StRNGUniform* fRngUniform;
  StRNG*        fRngThick; 
  StRNG*	fRngbSpotx;
  StRNG* 	fRngbSpoty;
 };

#endif
