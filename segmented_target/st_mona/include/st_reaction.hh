/** \file 
 * reaction ( i.e. change, creation, destruction of particle(s) )
 *
 */

#ifndef ST_TARGET_HH
#define ST_TARGET_HH

#include "st_material.hh"
#include "st_ble.hh"
#include "st_rng.hh"
#include "deb_err.h"

/// reaction: change one beam particle into another (can be more than one)
class StReaction {
public:
  /// constructor
  StReaction(string name) : fName(name) {}

  string getName() {return fName;}

  //@{
  /// set/get reaction probability
  void setProb(double p) {
    if (p>1) ERR("p>1, p=%f",p); 
    if (p<0) ERR("p<0, p=%f",p); 
    fProb = p;
  }
  double getProb() { return fProb; }
  //@}

  /// act and return pointer to vector of new particles
  virtual int act(StParticle* p, vector<StParticle>* newPart=0) = 0;

  virtual int  savedValue(size_t part_id, string name, double* v) = 0; ///< return a saved value
  virtual void savedReset() = 0;                                       ///< reset all saved values

  virtual ~StReaction() {}

private: 
  double fProb;  ///< reaction probability
  string fName;
  
};
/// Sequential 5body, strip first 2 then do 3body decay -- 5body decay
class StReacSequential_5bodydecay : public StReaction {
public:
  /// strip n neutrons and p protons and emit neutron 
  /// with const decay energy \todo allow non-const decay energy
  /// \param prob    Probability
  /// \param n       Number of neutrons to strip
  /// \param p       Number of protons  to strip
  /// \param exEnRng RNG for excitation energy
  StReacSequential_5bodydecay(double prob, int n, int p, StRNG* exEnRng, StRNG* exEn1Rng,StRNG* exEn2Rng, StRNG* exEn3Rng, StRNG* exEn4Rng, string WhichEnergyDoIReconstruct) : 
         StReaction("StReacSequential_5bodydecay"),
         fNp(p), fNn(n), fRngExEn(exEnRng), fRngExEn1(exEn1Rng), fRngExEn2(exEn2Rng), fRngExEn3(exEn3Rng), fRngExEn4(exEn4Rng), fRngUniform(0), fRngTwoNeutron_seq(0), 
	 fWhichEnergyDoIReconstruct(WhichEnergyDoIReconstruct), fGlauberKickScale(1), fVelocityScale(1), fAsymMomentum(1) {
    setProb(prob);   // inherited 'prob' must be set like this
  }
  
  /** act
   * \param  p       particle pointer
   * \param  newPart vector of new particles
   * \return ???
   */
  int act(StParticle* p, vector<StParticle>* newPart=0);

  /// return a saved value
  int savedValue(size_t part_id, string name, double* v);
  //int aDistSwitch; //used to switch between options for the 2body angular distribution

  /// reset all saved values
  void savedReset() {
    fExEnMap.clear();
    fExEnMap2.clear();
    fExEnMap3.clear();
    fExEnMap4.clear();
    fPhiMap.clear();
    fPhiMap2.clear();
    fPhiMap3.clear();
    fPhiMap4.clear();
    fThetaMap.clear();
    fThetaMap2.clear();
    fThetaMap3.clear();
    fThetaMap4.clear();
    fPzMap.clear();
  }

  void setRngExEn                 (StRNG*                r) { fRngExEn               = r; }
  void setRngExEn1                (StRNG*                r) { fRngExEn1              = r; }
  void setRngExEn2                (StRNG*                r) { fRngExEn2              = r; }
  void setRngExEn3		  (StRNG*		 r) { fRngExEn3	 	     = r; }
  void setRngExEn4		  (StRNG*		 r) { fRngExEn4		     = r; }
  void setRngTwoNeutron_seq       (StRNGTwoNeutron_seq*  r) { fRngTwoNeutron_seq     = r; }
  void setRngUniform      (StRNGUniform*   r) { fRngUniform      = r; }
  void setRngGaussGlauber (StRNGGauss* r)  { fRngGaussGlauber = r; }
  void setGlauberKickScale(double v) { fGlauberKickScale = v; }
  void setVelocityScale(double v) { fVelocityScale = v; }
  void setAsymMomentum(double v) { fAsymMomentum = v; }
  void setDebugSwitch(string dSwitch) {debugSwitch = dSwitch;}

private:
  int fNp;                   ///< number of protons to strip
  int fNn;                   ///< number of neutrons to strip

  StRNG*                fRngExEn;            ///< RNG excitation energy
  StRNG*                fRngExEn1;           ///< RNG excitation energy
  StRNG*                fRngExEn2;           ///< RNG excitation energy
  StRNG*		fRngExEn3;
  StRNG*		fRngExEn4;
  StRNGTwoNeutron_seq*  fRngTwoNeutron_seq;  ///< Two neutron RNG 
  StRNGUniform*         fRngUniform;         ///< uniform RNG (needed for emission angle etc.)
  StRNGGauss*           fRngGaussGlauber;    ///< gauss RNG (needed for Glauber momentum kick)

  StRNG*        fRngParaMom;
  bool fUseParaMom;

  double fGlauberKickScale;   ///< scale for Glauber momentum kick
  double fVelocityScale;      ///< scale for reduced velocity after fragmentation, zwk
  double fAsymMomentum;       ///< scale for asymmetric mom. distribution for fragmentation based on LISE++, zwk

  map<size_t, double> fPzMap; ///< saved Parallel momeumtum distribution
  map<size_t, double> fParaMomMap; ///< saved Parallel momeumtum distribution
  map<size_t, double> fExEnMap;  ///< saved excitation energy
  map<size_t, double> fExEnMap2;  ///< saved excitation energy
  map<size_t, double> fExEnMap3;
  map<size_t, double> fExEnMap4;
  map<size_t, double> fPhiMap;   ///< saved cm phi angle
  map<size_t, double> fPhiMap2;   ///< saved cm phi angle
  map<size_t, double> fPhiMap3;
  map<size_t, double> fPhiMap4;
  map<size_t, double> fThetaMap; ///< saved cm theta angle
  map<size_t, double> fThetaMap2; ///< saved cm theta angle
  map<size_t, double> fThetaMap3; 
  map<size_t, double> fThetaMap4;

  string debugSwitch;
  string fWhichEnergyDoIReconstruct;
};// End Sequential 5body

/// stripping and evaporation reaction -- 5body decay
class StReacStripEvap_5bodydecay : public StReaction {
public:
  /// strip n neutrons and p protons and emit neutron 
  /// with const decay energy \todo allow non-const decay energy
  /// \param prob    Probability
  /// \param n       Number of neutrons to strip
  /// \param p       Number of protons  to strip
  /// \param exEnRng RNG for excitation energy
  StReacStripEvap_5bodydecay(double prob, int n, int p, StRNG* exEnRng, StRNG* exEn1Rng,StRNG* exEn2Rng, StRNG* exEn3Rng, StRNG* exEn4Rng, string WhichEnergyDoIReconstruct) : 
         StReaction("StReacStripEvap_5bodydecay"),
         fNp(p), fNn(n), fRngExEn(exEnRng), fRngExEn1(exEn1Rng), fRngExEn2(exEn2Rng), fRngExEn3(exEn3Rng), fRngExEn4(exEn4Rng), fRngUniform(0), fRngTwoNeutron_seq(0), 
	 fWhichEnergyDoIReconstruct(WhichEnergyDoIReconstruct), fGlauberKickScale(1), fVelocityScale(1), fAsymMomentum(1) {
    setProb(prob);   // inherited 'prob' must be set like this
  }
  
  /** act
   * \param  p       particle pointer
   * \param  newPart vector of new particles
   * \return ???
   */
  int act(StParticle* p, vector<StParticle>* newPart=0);

  /// return a saved value
  int savedValue(size_t part_id, string name, double* v);
  //int aDistSwitch; //used to switch between options for the 2body angular distribution

  /// reset all saved values
  void savedReset() {
    fExEnMap.clear();
    fExEnMap2.clear();
    fExEnMap3.clear();
    fExEnMap4.clear();
    fPhiMap.clear();
    fPhiMap2.clear();
    fPhiMap3.clear();
    fPhiMap4.clear();
    fThetaMap.clear();
    fThetaMap2.clear();
    fThetaMap3.clear();
    fThetaMap4.clear();
    fPzMap.clear();
  }

  void setRngExEn                 (StRNG*                r) { fRngExEn               = r; }
  void setRngExEn1                (StRNG*                r) { fRngExEn1              = r; }
  void setRngExEn2                (StRNG*                r) { fRngExEn2              = r; }
  void setRngExEn3		  (StRNG*		 r) { fRngExEn3	 	     = r; }
  void setRngExEn4		  (StRNG*		 r) { fRngExEn4		     = r; }
  void setRngTwoNeutron_seq       (StRNGTwoNeutron_seq*  r) { fRngTwoNeutron_seq     = r; }
  void setRngUniform      (StRNGUniform*   r) { fRngUniform      = r; }
  void setRngGaussGlauber (StRNGGauss* r)  { fRngGaussGlauber = r; }
  void setGlauberKickScale(double v) { fGlauberKickScale = v; }
  void setVelocityScale(double v) { fVelocityScale = v; }
  void setAsymMomentum(double v) { fAsymMomentum = v; }
  void setDebugSwitch(string dSwitch) {debugSwitch = dSwitch;}

private:
  int fNp;                   ///< number of protons to strip
  int fNn;                   ///< number of neutrons to strip

  StRNG*                fRngExEn;            ///< RNG excitation energy
  StRNG*                fRngExEn1;           ///< RNG excitation energy
  StRNG*                fRngExEn2;           ///< RNG excitation energy
  StRNG*		fRngExEn3;
  StRNG*		fRngExEn4;
  StRNGTwoNeutron_seq*  fRngTwoNeutron_seq;  ///< Two neutron RNG 
  StRNGUniform*         fRngUniform;         ///< uniform RNG (needed for emission angle etc.)
  StRNGGauss*           fRngGaussGlauber;    ///< gauss RNG (needed for Glauber momentum kick)

  StRNG*        fRngParaMom;
  bool fUseParaMom;

  double fGlauberKickScale;   ///< scale for Glauber momentum kick
  double fVelocityScale;      ///< scale for reduced velocity after fragmentation, zwk
  double fAsymMomentum;       ///< scale for asymmetric mom. distribution for fragmentation based on LISE++, zwk

  map<size_t, double> fPzMap; ///< saved Parallel momeumtum distribution
  map<size_t, double> fParaMomMap; ///< saved Parallel momeumtum distribution
  map<size_t, double> fExEnMap;  ///< saved excitation energy
  map<size_t, double> fExEnMap2;  ///< saved excitation energy
  map<size_t, double> fExEnMap3;
  map<size_t, double> fExEnMap4;
  map<size_t, double> fPhiMap;   ///< saved cm phi angle
  map<size_t, double> fPhiMap2;   ///< saved cm phi angle
  map<size_t, double> fPhiMap3;
  map<size_t, double> fPhiMap4;
  map<size_t, double> fThetaMap; ///< saved cm theta angle
  map<size_t, double> fThetaMap2; ///< saved cm theta angle
  map<size_t, double> fThetaMap3; 
  map<size_t, double> fThetaMap4;

  string debugSwitch;
  string fWhichEnergyDoIReconstruct;
};


/// stripping and evaporation reaction -- 5body decay
class StReacStripEvap_3bodyDineutron : public StReaction {
public:
  /// strip n neutrons and p protons and emit neutron 
  /// with const decay energy \todo allow non-const decay energy
  /// \param prob    Probability
  /// \param n       Number of neutrons to strip
  /// \param p       Number of protons  to strip
  /// \param exEnRng RNG for excitation energy
  StReacStripEvap_3bodyDineutron(double prob, int n, int p, StRNG* exEnRng, StRNG* exEn1Rng,StRNG* exEn2Rng, StRNG* exEn3Rng, StRNG* exEn4Rng, string WhichEnergyDoIReconstruct) : 
         StReaction("StReacStripEvap_3bodyDineutron"),
         fNp(p), fNn(n), fRngExEn(exEnRng), fRngExEn1(exEn1Rng), fRngExEn2(exEn2Rng), fRngExEn3(exEn3Rng), fRngExEn4(exEn4Rng), fRngUniform(0), fRngTwoNeutron_seq(0), 
	 fWhichEnergyDoIReconstruct(WhichEnergyDoIReconstruct), fGlauberKickScale(1), fVelocityScale(1), fAsymMomentum(1) {
    setProb(prob);   // inherited 'prob' must be set like this
  }
  
  /** act
   * \param  p       particle pointer
   * \param  newPart vector of new particles
   * \return ???
   */
  int act(StParticle* p, vector<StParticle>* newPart=0);

  /// return a saved value
  int savedValue(size_t part_id, string name, double* v);
  //int aDistSwitch; //used to switch between options for the 2body angular distribution

  /// reset all saved values
  void savedReset() {
    fExEnMap.clear();
    fExEnMap2.clear();
    fExEnMap3.clear();
    fExEnMap4.clear();
    fPhiMap.clear();
    fPhiMap2.clear();
    fPhiMap3.clear();
    fPhiMap4.clear();
    fThetaMap.clear();
    fThetaMap2.clear();
    fThetaMap3.clear();
    fThetaMap4.clear();
    fPzMap.clear();
  }

  void setRngExEn                 (StRNG*                r) { fRngExEn               = r; }
  void setRngExEn1                (StRNG*                r) { fRngExEn1              = r; }
  void setRngExEn2                (StRNG*                r) { fRngExEn2              = r; }
  void setRngExEn3		  (StRNG*		 r) { fRngExEn3	 	     = r; }
  void setRngExEn4		  (StRNG*		 r) { fRngExEn4		     = r; }
  void setRngTwoNeutron_seq       (StRNGTwoNeutron_seq*  r) { fRngTwoNeutron_seq     = r; }
  void setRngUniform      (StRNGUniform*   r) { fRngUniform      = r; }
  void setRngGaussGlauber (StRNGGauss* r)  { fRngGaussGlauber = r; }
  void setGlauberKickScale(double v) { fGlauberKickScale = v; }
  void setVelocityScale(double v) { fVelocityScale = v; }
  void setAsymMomentum(double v) { fAsymMomentum = v; }
  void setDebugSwitch(string dSwitch) {debugSwitch = dSwitch;}

private:
  int fNp;                   ///< number of protons to strip
  int fNn;                   ///< number of neutrons to strip

  StRNG*                fRngExEn;            ///< RNG excitation energy
  StRNG*                fRngExEn1;           ///< RNG excitation energy
  StRNG*                fRngExEn2;           ///< RNG excitation energy
  StRNG*		fRngExEn3;
  StRNG*		fRngExEn4;
  StRNGTwoNeutron_seq*  fRngTwoNeutron_seq;  ///< Two neutron RNG 
  StRNGUniform*         fRngUniform;         ///< uniform RNG (needed for emission angle etc.)
  StRNGGauss*           fRngGaussGlauber;    ///< gauss RNG (needed for Glauber momentum kick)

  StRNG*        fRngParaMom;
  bool fUseParaMom;

  double fGlauberKickScale;   ///< scale for Glauber momentum kick
  double fVelocityScale;      ///< scale for reduced velocity after fragmentation, zwk
  double fAsymMomentum;       ///< scale for asymmetric mom. distribution for fragmentation based on LISE++, zwk

  map<size_t, double> fPzMap; ///< saved Parallel momeumtum distribution
  map<size_t, double> fParaMomMap; ///< saved Parallel momeumtum distribution
  map<size_t, double> fExEnMap;  ///< saved excitation energy
  map<size_t, double> fExEnMap2;  ///< saved excitation energy
  map<size_t, double> fExEnMap3;
  map<size_t, double> fExEnMap4;
  map<size_t, double> fPhiMap;   ///< saved cm phi angle
  map<size_t, double> fPhiMap2;   ///< saved cm phi angle
  map<size_t, double> fPhiMap3;
  map<size_t, double> fPhiMap4;
  map<size_t, double> fThetaMap; ///< saved cm theta angle
  map<size_t, double> fThetaMap2; ///< saved cm theta angle
  map<size_t, double> fThetaMap3; 
  map<size_t, double> fThetaMap4;

  string debugSwitch;
  string fWhichEnergyDoIReconstruct;
};






/// stripping and evaporation reaction 4neutron decay MDJ
class StReacStripEvap4neutron : public StReaction {
public:
  /// strip n neutrons and p protons and emit neutron 
  /// with const decay energy \todo allow non-const decay energy
  /// \param prob    Probability
  /// \param n       Number of neutrons to strip
  /// \param p       Number of protons  to strip
  /// \param exEnRng RNG for excitation energy
  StReacStripEvap4neutron(double prob, int n, int p, StRNG* exEnRng, StRNG* exEn1Rng,StRNG* exEn2Rng, StRNG* exEn3Rng, StRNG* exEn4Rng, string WhichEnergyDoIReconstruct) : 
         StReaction("StReacStripEvap4neutron"),
         fNp(p), fNn(n), fRngExEn(exEnRng), fRngExEn1(exEn1Rng), fRngExEn2(exEn2Rng), fRngExEn3(exEn3Rng), fRngExEn4(exEn4Rng), fRngUniform(0), fRngTwoNeutron_seq(0), 
	 fWhichEnergyDoIReconstruct(WhichEnergyDoIReconstruct), fGlauberKickScale(1), fVelocityScale(1), fAsymMomentum(1), fRngLegendre(0) {
    setProb(prob);   // inherited 'prob' must be set like this
  }
  
  /** act
   * \param  p       particle pointer
   * \param  newPart vector of new particles
   * \return ???
   */
  int act(StParticle* p, vector<StParticle>* newPart=0);

  /// return a saved value
  int savedValue(size_t part_id, string name, double* v);
  //int aDistSwitch; //used to switch between options for the 2body angular distribution

  /// reset all saved values
  void savedReset() {
    fExEnMap.clear();
    fExEnMap2.clear();
    fExEnMap3.clear();
    fExEnMap4.clear();
    fPhiMap.clear();
    fPhiMap2.clear();
    fPhiMap3.clear();
    fPhiMap4.clear();
    fThetaMap.clear();
    fThetaMap2.clear();
    fThetaMap3.clear();
    fThetaMap4.clear();
    fPzMap.clear();
  }

  void setRngExEn                 (StRNG*                r) { fRngExEn               = r; }
  void setRngExEn1                (StRNG*                r) { fRngExEn1              = r; }
  void setRngExEn2                (StRNG*                r) { fRngExEn2              = r; }
  void setRngExEn3		          (StRNG*		 r) { fRngExEn3 	     = r; }
  void setRngExEn4		          (StRNG*		 r) { fRngExEn4		     = r; }
  void setRngTwoNeutron_seq       (StRNGTwoNeutron_seq*  r) { fRngTwoNeutron_seq     = r; }
  void setRngUniform              (StRNGUniform*   r) { fRngUniform      = r; }
  void setRngLegendre             (StRNGLegendre*  r) { fRngLegendre     = r; }
  void setRngGaussGlauber          (StRNGGauss* r)  { fRngGaussGlauber = r; }
  void setGlauberKickScale(double v) { fGlauberKickScale = v; }
  void setVelocityScale(double v) { fVelocityScale = v; }
  void setAsymMomentum(double v) { fAsymMomentum = v; }
  void setDebugSwitch(string dSwitch) {debugSwitch = dSwitch;}
  

private:
  int fNp;                   ///< number of protons to strip
  int fNn;                   ///< number of neutrons to strip

  StRNG*                fRngExEn;            ///< RNG excitation energy
  StRNG*                fRngExEn1;           ///< RNG excitation energy
  StRNG*                fRngExEn2;           ///< RNG excitation energy
  StRNG*	        	fRngExEn3;	     ///< RNG excitation energy
  StRNG*	        	fRngExEn4;	
  StRNGTwoNeutron_seq*  fRngTwoNeutron_seq;  ///< Two neutron RNG 
  StRNGUniform*         fRngUniform;         ///< uniform RNG (needed for emission angle etc.)
  StRNGLegendre*        fRngLegendre;        // legendre RNG for angular correlations
  StRNGGauss*           fRngGaussGlauber;    ///< gauss RNG (needed for Glauber momentum kick)

  StRNG*        fRngParaMom;
  bool fUseParaMom;

  double fGlauberKickScale;   ///< scale for Glauber momentum kick
  double fVelocityScale;      ///< scale for reduced velocity after fragmentation, zwk
  double fAsymMomentum;       ///< scale for asymmetric mom. distribution for fragmentation based on LISE++, zwk

  map<size_t, double> fPzMap; ///< saved Parallel momeumtum distribution
  map<size_t, double> fParaMomMap; ///< saved Parallel momeumtum distribution
  map<size_t, double> fExEnMap;  ///< saved excitation energy
  map<size_t, double> fExEnMap2;  ///< saved excitation energy
  map<size_t, double> fExEnMap3;
  map<size_t, double> fExEnMap4;
  map<size_t, double> fPhiMap;   ///< saved cm phi angle
  map<size_t, double> fPhiMap2;   ///< saved cm phi angle
  map<size_t, double> fPhiMap3; 
  map<size_t, double> fPhiMap4;
  map<size_t, double> fThetaMap; ///< saved cm theta angle
  map<size_t, double> fThetaMap2; ///< saved cm theta angle
  map<size_t, double> fThetaMap3;
  map<size_t, double> fThetaMap4;

  string debugSwitch;
  string fWhichEnergyDoIReconstruct;
};// End 4neutron


/// stripping and evaporation reaction -- 4body decay
class StReacStripEvap_4bodydecay : public StReaction {
public:
  /// strip n neutrons and p protons and emit neutron 
  /// with const decay energy \todo allow non-const decay energy
  /// \param prob    Probability
  /// \param n       Number of neutrons to strip
  /// \param p       Number of protons  to strip
  /// \param exEnRng RNG for excitation energy
  StReacStripEvap_4bodydecay(double prob, int n, int p, StRNG* exEnRng, StRNG* exEn1Rng,StRNG* exEn2Rng, StRNG* exEn3Rng, string WhichEnergyDoIReconstruct) : 
         StReaction("StReacStripEvap_4bodydecay"),
         fNp(p), fNn(n), fRngExEn(exEnRng), fRngExEn1(exEn1Rng), fRngExEn2(exEn2Rng), fRngExEn3(exEn3Rng), fRngUniform(0), fRngTwoNeutron_seq(0), 
	 fWhichEnergyDoIReconstruct(WhichEnergyDoIReconstruct), fGlauberKickScale(1), fVelocityScale(1), fAsymMomentum(1) {
    setProb(prob);   // inherited 'prob' must be set like this
  }
  
  /** act
   * \param  p       particle pointer
   * \param  newPart vector of new particles
   * \return ???
   */
  int act(StParticle* p, vector<StParticle>* newPart=0);

  /// return a saved value
  int savedValue(size_t part_id, string name, double* v);
  //int aDistSwitch; //used to switch between options for the 2body angular distribution

  /// reset all saved values
  void savedReset() {
    fExEnMap.clear();
    fExEnMap2.clear();
    fExEnMap3.clear();
    fPhiMap.clear();
    fPhiMap2.clear();
    fPhiMap3.clear();
    fThetaMap.clear();
    fThetaMap2.clear();
    fThetaMap3.clear();
    fPzMap.clear();
  }

  void setRngExEn                 (StRNG*                r) { fRngExEn               = r; }
  void setRngExEn1                (StRNG*                r) { fRngExEn1              = r; }
  void setRngExEn2                (StRNG*                r) { fRngExEn2              = r; }
  void setRngExEn3		  (StRNG*		 r) { fRngExEn3	 	     = r; }
  void setRngTwoNeutron_seq       (StRNGTwoNeutron_seq*  r) { fRngTwoNeutron_seq     = r; }
  void setRngUniform      (StRNGUniform*   r) { fRngUniform      = r; }
  void setRngGaussGlauber (StRNGGauss* r)  { fRngGaussGlauber = r; }
  void setGlauberKickScale(double v) { fGlauberKickScale = v; }
  void setVelocityScale(double v) { fVelocityScale = v; }
  void setAsymMomentum(double v) { fAsymMomentum = v; }
  void setDebugSwitch(string dSwitch) {debugSwitch = dSwitch;}

private:
  int fNp;                   ///< number of protons to strip
  int fNn;                   ///< number of neutrons to strip

  StRNG*                fRngExEn;            ///< RNG excitation energy
  StRNG*                fRngExEn1;           ///< RNG excitation energy
  StRNG*                fRngExEn2;           ///< RNG excitation energy
  StRNG*		        fRngExEn3;
  StRNGTwoNeutron_seq*  fRngTwoNeutron_seq;  ///< Two neutron RNG 
  StRNGUniform*         fRngUniform;         ///< uniform RNG (needed for emission angle etc.)
  StRNGGauss*           fRngGaussGlauber;    ///< gauss RNG (needed for Glauber momentum kick)

  StRNG*        fRngParaMom;
  bool fUseParaMom;

  double fGlauberKickScale;   ///< scale for Glauber momentum kick
  double fVelocityScale;      ///< scale for reduced velocity after fragmentation, zwk
  double fAsymMomentum;       ///< scale for asymmetric mom. distribution for fragmentation based on LISE++, zwk

  map<size_t, double> fPzMap; ///< saved Parallel momeumtum distribution
  map<size_t, double> fParaMomMap; ///< saved Parallel momeumtum distribution
  map<size_t, double> fExEnMap;  ///< saved excitation energy
  map<size_t, double> fExEnMap2;  ///< saved excitation energy
  map<size_t, double> fExEnMap3;
  map<size_t, double> fPhiMap;   ///< saved cm phi angle
  map<size_t, double> fPhiMap2;   ///< saved cm phi angle
  map<size_t, double> fPhiMap3;
  map<size_t, double> fThetaMap; ///< saved cm theta angle
  map<size_t, double> fThetaMap2; ///< saved cm theta angle
  map<size_t, double> fThetaMap3; 
  
  string debugSwitch;
  string fWhichEnergyDoIReconstruct;
};




	
/// stripping and evaporation reaction 3neutron decay MDJ
class StReacStripEvap3neutron : public StReaction {
public:
  /// strip n neutrons and p protons and emit neutron 
  /// with const decay energy \todo allow non-const decay energy
  /// \param prob    Probability
  /// \param n       Number of neutrons to strip
  /// \param p       Number of protons  to strip
  /// \param exEnRng RNG for excitation energy
  StReacStripEvap3neutron(double prob, int n, int p, StRNG* exEnRng, StRNG* exEn1Rng,StRNG* exEn2Rng, StRNG* exEn3Rng, string WhichEnergyDoIReconstruct) : 
         StReaction("StReacStripEvap3neutron"),
         fNp(p), fNn(n), fRngExEn(exEnRng), fRngExEn1(exEn1Rng), fRngExEn2(exEn2Rng), fRngExEn3(exEn3Rng), fRngUniform(0), fRngTwoNeutron_seq(0), 
	 fWhichEnergyDoIReconstruct(WhichEnergyDoIReconstruct), fGlauberKickScale(1), fVelocityScale(1), fAsymMomentum(1), fRngLegendre(0) {
    setProb(prob);   // inherited 'prob' must be set like this
  }
  
  /** act
   * \param  p       particle pointer
   * \param  newPart vector of new particles
   * \return ???
   */
  int act(StParticle* p, vector<StParticle>* newPart=0);

  /// return a saved value
  int savedValue(size_t part_id, string name, double* v);
  //int aDistSwitch; //used to switch between options for the 2body angular distribution

  /// reset all saved values
  void savedReset() {
    fExEnMap.clear();
    fExEnMap2.clear();
    fExEnMap3.clear();
    fPhiMap.clear();
    fPhiMap2.clear();
    fPhiMap3.clear();
    fThetaMap.clear();
    fThetaMap2.clear();
    fThetaMap3.clear();
    fPzMap.clear();
  }

  void setRngExEn                 (StRNG*                r) { fRngExEn               = r; }
  void setRngExEn1                (StRNG*                r) { fRngExEn1              = r; }
  void setRngExEn2                (StRNG*                r) { fRngExEn2              = r; }
  void setRngExEn3		  (StRNG*		 r) { fRngExEn3 	     = r; }
  void setRngTwoNeutron_seq       (StRNGTwoNeutron_seq*  r) { fRngTwoNeutron_seq     = r; }
  void setRngUniform      (StRNGUniform*   r) { fRngUniform      = r; }
  void setRngLegendre     (StRNGLegendre*  r) { fRngLegendre     = r; }
  void setRngGaussGlauber (StRNGGauss* r)  { fRngGaussGlauber = r; }
  void setGlauberKickScale(double v) { fGlauberKickScale = v; }
  void setVelocityScale(double v) { fVelocityScale = v; }
  void setAsymMomentum(double v) { fAsymMomentum = v; }
  void setDebugSwitch(string dSwitch) {debugSwitch = dSwitch;}

private:
  int fNp;                   ///< number of protons to strip
  int fNn;                   ///< number of neutrons to strip

  StRNG*                fRngExEn;            ///< RNG excitation energy
  StRNG*                fRngExEn1;           ///< RNG excitation energy
  StRNG*                fRngExEn2;           ///< RNG excitation energy
  StRNG*		        fRngExEn3;	     ///< RNG excitation energy
  StRNGTwoNeutron_seq*  fRngTwoNeutron_seq;  ///< Two neutron RNG 
  StRNGUniform*         fRngUniform;         ///< uniform RNG (needed for emission angle etc.)
  StRNGLegendre*        fRngLegendre;        // Legendre polynomial
  StRNGGauss*           fRngGaussGlauber;    ///< gauss RNG (needed for Glauber momentum kick)

  StRNG*        fRngParaMom;
  bool fUseParaMom;

  double fGlauberKickScale;   ///< scale for Glauber momentum kick
  double fVelocityScale;      ///< scale for reduced velocity after fragmentation, zwk
  double fAsymMomentum;       ///< scale for asymmetric mom. distribution for fragmentation based on LISE++, zwk

  map<size_t, double> fPzMap; ///< saved Parallel momeumtum distribution
  map<size_t, double> fParaMomMap; ///< saved Parallel momeumtum distribution
  map<size_t, double> fExEnMap;  ///< saved excitation energy
  map<size_t, double> fExEnMap2;  ///< saved excitation energy
  map<size_t, double> fExEnMap3;
  map<size_t, double> fPhiMap;   ///< saved cm phi angle
  map<size_t, double> fPhiMap2;   ///< saved cm phi angle
  map<size_t, double> fPhiMap3; 
  map<size_t, double> fThetaMap; ///< saved cm theta angle
  map<size_t, double> fThetaMap2; ///< saved cm theta angle
  map<size_t, double> fThetaMap3;

  string debugSwitch;
  string fWhichEnergyDoIReconstruct;
};// End 3neutron







/// (d,p) reaction MDJ 8/22/12 

class StReacDP : public StReaction {
public:
  /// picks up a neutron from the target and then decays back down to the orginal isotope
  /// \param prob    Probability
  /// \param exEnRng RNG for excitation energy
  StReacDP(double prob, int n, int p, StRNG* exEnRng) : 
         StReaction("StReacDP"),
         fNp(p), fNn(n), fRngExEn(exEnRng), fRngUniform(0), fRngAngDist(0),
         fGlauberKickScale(1) {
    setProb(prob);   // inherited 'prob' must be set like this
  }
  
  /** act
   * \param  p       particle pointer
   * \param  newPart vector of new particles
   * \return ???
   */
  int act(StParticle* p, vector<StParticle>* newPart=0);

  /// return a saved value
  int savedValue(size_t part_id, string name, double* v);
  //int aDistSwitch; //used to switch between options for the 2body angular distribution

  /// reset all saved values
  void savedReset() {
    fExEnMap.clear();
    fPhiMap.clear();
    fThetaMap.clear();
  }

  void setRngExEn   (StRNG*           r) { fRngExEn         = r; }
  void setRngUniform(StRNGUniform*    r) { fRngUniform      = r; }
  void setRngAngDist(StRNGangular_distribution_dp*    r) { fRngAngDist      = r; }
  void setRngGaussGlauber(StRNGGauss* r) { fRngGaussGlauber = r; }
  void setGlauberKickScale(double v) { fGlauberKickScale = v; }
  void setDebugSwitch(string dSwitch) {debugSwitch = dSwitch;}

private:
  int fNp;                   ///< number of protons to strip
  int fNn;                   ///< number of neutrons to strip
  StRNG*        fRngExEn;    ///< RNG excitation energy
  StRNGUniform* fRngUniform; ///< uniform RNG (needed for emission angle etc.)
  StRNGangular_distribution_dp* fRngAngDist; ///< dp angular RNG (needed for emission angle etc.)
  StRNGGauss*   fRngGaussGlauber;   ///< gauss RNG (needed for Glauber momentum kick)
//  StRNGGauss*   fRngGauss2Body;   ///< gauss RNG (needed for 2body angular distribution)
//  StRNGUniform* fRngUniform2Body; ///< uniform RNG (needed for emission angle etc.)

  double fGlauberKickScale;   ///< scale for Glauber momentum kick

  map<size_t, double> fExEnMap;  ///< saved excitation energy
  map<size_t, double> fPhiMap;   ///< saved cm phi angle
  map<size_t, double> fThetaMap; ///< saved cm theta angle
  string debugSwitch; 
};
// End (d,p)
//
//


// Deuteron Breakup Kinematics
class StReacDeuteronBreakup : public StReaction {
public:
  /// explodes the deuteron emitting a neutron
  //  modelled as a three-body decay of the target + deuteron system
  //
  /// \param prob    Probability
  /// \param exEnRng RNG for excitation energy
  StReacDeuteronBreakup(double prob, int n, int p, StRNG* exEnRng) : 
         StReaction("StReacDeuteronBreakup"),
         fNp(p), fNn(n), fRngExEn(exEnRng), fRngUniform(0), fRngAngDist(0),
         fGlauberKickScale(1) {
    setProb(prob);   // inherited 'prob' must be set like this
  }
  
  /** act
   * \param  p       particle pointer
   * \param  newPart vector of new particles
   * \return ???
   */
  int act(StParticle* p, vector<StParticle>* newPart=0);

  /// return a saved value
  int savedValue(size_t part_id, string name, double* v);
  //int aDistSwitch; //used to switch between options for the 2body angular distribution

  /// reset all saved values
  void savedReset() {
    fExEnMap.clear();
    fPhiMap.clear();
    fThetaMap.clear();
  }

  void setRngExEn   (StRNG*           r) { fRngExEn         = r; }
  void setRngUniform(StRNGUniform*    r) { fRngUniform      = r; }
  void setRngAngDist(StRNGangular_distribution_dp*    r) { fRngAngDist      = r; }
  void setRngGaussGlauber(StRNGGauss* r) { fRngGaussGlauber = r; }
  void setGlauberKickScale(double v) { fGlauberKickScale = v; }
  void setDebugSwitch(string dSwitch) {debugSwitch = dSwitch;}

private:
  int fNp;                   ///< number of protons to strip
  int fNn;                   ///< number of neutrons to strip
  StRNG*        fRngExEn;    ///< RNG excitation energy
  StRNGUniform* fRngUniform; ///< uniform RNG (needed for emission angle etc.)
  StRNGangular_distribution_dp* fRngAngDist; ///< dp angular RNG (needed for emission angle etc.)
  StRNGGauss*   fRngGaussGlauber;   ///< gauss RNG (needed for Glauber momentum kick)
//  StRNGGauss*   fRngGauss2Body;   ///< gauss RNG (needed for 2body angular distribution)
//  StRNGUniform* fRngUniform2Body; ///< uniform RNG (needed for emission angle etc.)

  double fGlauberKickScale;   ///< scale for Glauber momentum kick

  map<size_t, double> fExEnMap;  ///< saved excitation energy
  map<size_t, double> fPhiMap;   ///< saved cm phi angle
  map<size_t, double> fThetaMap; ///< saved cm theta angle
  string debugSwitch; 
};
class StReacDeuteronBreakupTwoBody : public StReaction {
public:
  /// explodes the deuteron emitting a neutron
  //  modelled as a three-body decay of the target + deuteron system
  //
  /// \param prob    Probability
  /// \param exEnRng RNG for excitation energy
  StReacDeuteronBreakupTwoBody(double prob, int n, int p, StRNG* exEnRng) : 
         StReaction("StReacDeuteronBreakupTwoBody"),
         fNp(p), fNn(n), fRngExEn(exEnRng), fRngUniform(0), fRngAngDist(0),
         fGlauberKickScale(1) {
    setProb(prob);   // inherited 'prob' must be set like this
  }
  
  /** act
   * \param  p       particle pointer
   * \param  newPart vector of new particles
   * \return ???
   */
  int act(StParticle* p, vector<StParticle>* newPart=0);

  /// return a saved value
  int savedValue(size_t part_id, string name, double* v);
  //int aDistSwitch; //used to switch between options for the 2body angular distribution

  /// reset all saved values
  void savedReset() {
    fExEnMap.clear();
    fPhiMap.clear();
    fThetaMap.clear();
  }

  void setRngExEn   (StRNG*           r) { fRngExEn         = r; }
  void setRngUniform(StRNGUniform*    r) { fRngUniform      = r; }
  void setRngAngDist(StRNGangular_distribution_dp*    r) { fRngAngDist      = r; }
  void setRngGaussGlauber(StRNGGauss* r) { fRngGaussGlauber = r; }
  void setGlauberKickScale(double v) { fGlauberKickScale = v; }
  void setDebugSwitch(string dSwitch) {debugSwitch = dSwitch;}

private:
  int fNp;                   ///< number of protons to strip
  int fNn;                   ///< number of neutrons to strip
  StRNG*        fRngExEn;    ///< RNG excitation energy
  StRNGUniform* fRngUniform; ///< uniform RNG (needed for emission angle etc.)
  StRNGangular_distribution_dp* fRngAngDist; ///< dp angular RNG (needed for emission angle etc.)
  StRNGGauss*   fRngGaussGlauber;   ///< gauss RNG (needed for Glauber momentum kick)
//  StRNGGauss*   fRngGauss2Body;   ///< gauss RNG (needed for 2body angular distribution)
//  StRNGUniform* fRngUniform2Body; ///< uniform RNG (needed for emission angle etc.)

  double fGlauberKickScale;   ///< scale for Glauber momentum kick

  map<size_t, double> fExEnMap;  ///< saved excitation energy
  map<size_t, double> fPhiMap;   ///< saved cm phi angle
  map<size_t, double> fThetaMap; ///< saved cm theta angle
  string debugSwitch; 
};

// End deuteron breakup


/// stripping and evaporation reaction
class StReacStripEvap : public StReaction {
public:
  /// strip n neutrons and p protons and emit neutron 
  /// with const decay energy \todo allow non-const decay energy
  /// \param prob    Probability
  /// \param n       Number of neutrons to strip
  /// \param p       Number of protons  to strip
  /// \param exEnRng RNG for excitation energy
  StReacStripEvap(double prob, int n, int p, StRNG* exEnRng) : 
         StReaction("StReacStripEvap"),
         fNp(p), fNn(n), fRngExEn(exEnRng), fRngUniform(0),
         fGlauberKickScale(1), fUseParaMom(false), fVelocityScale(1), fAsymMomentum(1) {
    setProb(prob);   // inherited 'prob' must be set like this
  }
  
  /** act
   * \param  p       particle pointer
   * \param  newPart vector of new particles
   * \return ???
   */
  int act(StParticle* p, vector<StParticle>* newPart=0);

  /// return a saved value
  int savedValue(size_t part_id, string name, double* v);
  //int aDistSwitch; //used to switch between options for the 2body angular distribution

  /// reset all saved values
  void savedReset() {
    fExEnMap.clear();
    fPhiMap.clear();
    fThetaMap.clear();
    fPzMap.clear();
  }

  void setRngExEn   (StRNG*           r) { fRngExEn         = r; }
  void setRngUniform(StRNGUniform*    r) { fRngUniform      = r; }
  void setRngGaussGlauber(StRNGGauss* r) { fRngGaussGlauber = r; }
  void setRngParaMom(StRNG*           r) { fRngParaMom      = r;  fUseParaMom = true; }

  void setGlauberKickScale(double v) { fGlauberKickScale = v; }
  void setVelocityScale(double v) { fVelocityScale = v; }
  void setAsymMomentum(double v) { fAsymMomentum = v; }
  void setDebugSwitch(string dSwitch) {debugSwitch = dSwitch;}

private:
  int fNp;                   ///< number of protons to strip
  int fNn;                   ///< number of neutrons to strip
  StRNG*        fRngExEn;    ///< RNG excitation energy
  StRNGUniform* fRngUniform; ///< uniform RNG (needed for emission angle etc.)
  StRNGGauss*   fRngGaussGlauber;   ///< gauss RNG (needed for Glauber momentum kick)

  StRNG*        fRngParaMom;
  bool fUseParaMom;

//  StRNGGauss*   fRngGauss2Body;   ///< gauss RNG (needed for 2body angular distribution)
//  StRNGUniform* fRngUniform2Body; ///< uniform RNG (needed for emission angle etc.)

  double fGlauberKickScale;   ///< scale for Glauber momentum kick
  double fVelocityScale;      ///< scale for reduced velocity after fragmentation, zwk
  double fAsymMomentum;       ///< scale for asymmetric mom. distribution for fragmentation based on LISE++, zwk

  map<size_t, double> fPzMap; ///< saved Parallel momeumtum distribution
  map<size_t, double> fExEnMap;  ///< saved excitation energy
  map<size_t, double> fPhiMap;   ///< saved cm phi angle
  map<size_t, double> fThetaMap; ///< saved cm theta angle
  string debugSwitch; 
};




/// stripping/knockout and no Decay  (glaub_noDecay)
class StReacGlaub_noDecay : public StReaction {
public:
  /// strip/knockout n/p protons and no neutron decay
  /// \param prob    Probability
  /// \param n       Number of neutrons to strip
  /// \param p       Number of protons  to strip
  StReacGlaub_noDecay(double prob, int n, int p) : 
         StReaction("StReacGlaub_noDecay"),
         fNp(p), fNn(n), fRngUniform(0),
         fGlauberKickScale(1), fUseParaMom(false), fVelocityScale(1), fAsymMomentum(1) {
    setProb(prob);   // inherited 'prob' must be set like this
  }
  
  /** act
   * \param  p       particle pointer
   * \param  newPart vector of new particles
   * \return ???
   */
  int act(StParticle* p, vector<StParticle>* newPart=0);

  /// return a saved value
  int savedValue(size_t part_id, string name, double* v);
  //int aDistSwitch; //used to switch between options for the 2body angular distribution

  /// reset all saved values
  void savedReset() {
    fExEnMap.clear();
    fPhiMap.clear();
    fThetaMap.clear();
    fPzMap.clear();
  }

  //void setRngExEn   (StRNG*           r) { fRngExEn         = r; }
  void setRngUniform(StRNGUniform*    r) { fRngUniform      = r; }
  void setRngGaussGlauber(StRNGGauss* r) { fRngGaussGlauber = r; }
  void setRngParaMom(StRNGGauss*      r) { fRngParaMom      = r;  fUseParaMom = true; }

  void setGlauberKickScale(double v) { fGlauberKickScale = v; }
  void setVelocityScale(double v) { fVelocityScale = v; }
  void setAsymMomentum(double v) { fAsymMomentum = v; }
  // ** HL 01/10/2017
  void setGlauberTransScale1(double v) { fGlauberTransScale1 = v; } // JKS
  void setGlauberTransScale2(double v) { fGlauberTransScale2 = v; } // JKS
  void setDebugSwitch(string dSwitch) {debugSwitch = dSwitch;}

private:
  int fNp;                   ///< number of protons to strip
  int fNn;                   ///< number of neutrons to strip
  //StRNG*        fRngExEn;    ///< RNG excitation energy
  StRNGUniform* fRngUniform; ///< uniform RNG (needed for emission angle etc.)
  StRNGGauss*   fRngGaussGlauber;   ///< gauss RNG (needed for Glauber momentum kick)

  StRNGGauss*        fRngParaMom;
  bool fUseParaMom;

//  StRNGGauss*   fRngGauss2Body;   ///< gauss RNG (needed for 2body angular distribution)
//  StRNGUniform* fRngUniform2Body; ///< uniform RNG (needed for emission angle etc.)

  double fGlauberKickScale;   ///< scale for Glauber momentum kick
  double fVelocityScale;      ///< scale for reduced velocity after fragmentation, zwk
  double fAsymMomentum;       ///< scale for asymmetric mom. distribution for fragmentation based on LISE++, zwk
  double fGlauberTransScale1;
  double fGlauberTransScale2;

  map<size_t, double> fPzMap; ///< saved Parallel momeumtum distribution
  map<size_t, double> fExEnMap;  ///< saved excitation energy
  map<size_t, double> fPhiMap;   ///< saved cm phi angle
  map<size_t, double> fThetaMap; ///< saved cm theta angle
  string debugSwitch; 
};


/// stripping and evaporation reaction
class StReacStripEvap2neutron : public StReaction {
public:
  /// strip n neutrons and p protons and emit neutron 
  /// with const decay energy \todo allow non-const decay energy
  /// \param prob    Probability
  /// \param n       Number of neutrons to strip
  /// \param p       Number of protons  to strip
  /// \param exEnRng RNG for excitation energy
  StReacStripEvap2neutron(double prob, int n, int p, StRNG* exEnRng, StRNG* exEn1Rng,StRNG* exEn2Rng, string WhichEnergyDoIReconstruct) : 
         StReaction("StReacStripEvap2neutron"),
         fNp(p), fNn(n), fRngExEn(exEnRng), fRngExEn1(exEn1Rng), fRngExEn2(exEn2Rng), fRngUniform(0), fRngTwoNeutron_seq(0), 
	 fWhichEnergyDoIReconstruct(WhichEnergyDoIReconstruct), fGlauberKickScale(1), fVelocityScale(1), fAsymMomentum(1), fRngLegendre(0) {
    setProb(prob);   // inherited 'prob' must be set like this
  }
  
  /** act
   * \param  p       particle pointer
   * \param  newPart vector of new particles
   * \return ???
   */
  int act(StParticle* p, vector<StParticle>* newPart=0);

  /// return a saved value
  int savedValue(size_t part_id, string name, double* v);
  //int aDistSwitch; //used to switch between options for the 2body angular distribution

  /// reset all saved values
  void savedReset() {
    fExEnMap.clear();
    fExEnMap2.clear();
    fPhiMap.clear();
    fPhiMap2.clear();
    fThetaMap.clear();
    fThetaMap2.clear();
    fPzMap.clear();
  }

  void setRngExEn                 (StRNG*                r) { fRngExEn               = r; }
  void setRngExEn1                (StRNG*                r) { fRngExEn1              = r; }
  void setRngExEn2                (StRNG*                r) { fRngExEn2              = r; }
  void setRngTwoNeutron_seq       (StRNGTwoNeutron_seq*  r) { fRngTwoNeutron_seq     = r; }
  void setRngUniform              (StRNGUniform*         r) { fRngUniform      = r; }
  void setRngLegendre             (StRNGLegendre*        r) { fRngLegendre     =r; }
  void setRngGaussGlauber (StRNGGauss* r)  { fRngGaussGlauber = r; }
  void setGlauberKickScale(double v) { fGlauberKickScale = v; }
  void setVelocityScale(double v) { fVelocityScale = v; }
  void setAsymMomentum(double v) { fAsymMomentum = v; }
  void setDebugSwitch(string dSwitch) {debugSwitch = dSwitch;}

private:
  int fNp;                   ///< number of protons to strip
  int fNn;                   ///< number of neutrons to strip

  StRNG*                fRngExEn;            ///< RNG excitation energy
  StRNG*                fRngExEn1;           ///< RNG excitation energy
  StRNG*                fRngExEn2;           ///< RNG excitation energy
  StRNGTwoNeutron_seq*  fRngTwoNeutron_seq;  ///< Two neutron RNG 
  StRNGUniform*         fRngUniform;         ///< uniform RNG (needed for emission angle etc.)
  StRNGLegendre*        fRngLegendre;
  StRNGGauss*           fRngGaussGlauber;    ///< gauss RNG (needed for Glauber momentum kick)

  StRNG*        fRngParaMom;
  bool fUseParaMom;

  double fGlauberKickScale;   ///< scale for Glauber momentum kick
  double fVelocityScale;      ///< scale for reduced velocity after fragmentation, zwk
  double fAsymMomentum;       ///< scale for asymmetric mom. distribution for fragmentation based on LISE++, zwk

  map<size_t, double> fPzMap; ///< saved Parallel momeumtum distribution
  map<size_t, double> fParaMomMap; ///< saved Parallel momeumtum distribution
  map<size_t, double> fExEnMap;  ///< saved excitation energy
  map<size_t, double> fExEnMap2;  ///< saved excitation energy
  map<size_t, double> fPhiMap;   ///< saved cm phi angle
  map<size_t, double> fPhiMap2;   ///< saved cm phi angle
  map<size_t, double> fThetaMap; ///< saved cm theta angle
  map<size_t, double> fThetaMap2; ///< saved cm theta angle
  string debugSwitch;
  string fWhichEnergyDoIReconstruct;
};


/// stripping and evaporation reaction -- dineutron
class StReacStripEvap_dineutron : public StReaction {
public:
  /// strip n neutrons and p protons and emit neutron 
  /// with const decay energy \todo allow non-const decay energy
  /// \param prob    Probability
  /// \param n       Number of neutrons to strip
  /// \param p       Number of protons  to strip
  /// \param exEnRng RNG for excitation energy
  StReacStripEvap_dineutron(double prob, int n, int p, StRNG* exEnRng, StRNG* exEn1Rng,StRNG* exEn2Rng, string WhichEnergyDoIReconstruct) : 
         StReaction("StReacStripEvap_dineutron"),
         fNp(p), fNn(n), fRngExEn(exEnRng), fRngExEn1(exEn1Rng), fRngExEn2(exEn2Rng), fRngUniform(0), fRngTwoNeutron_seq(0), 
	 fWhichEnergyDoIReconstruct(WhichEnergyDoIReconstruct), fGlauberKickScale(1), fVelocityScale(1), fAsymMomentum(1),
	 // ** HL 01/10/17
	 fUseTransKick(false), fGlauberTransScale1(1), fGlauberTransScale2(1){
    setProb(prob);   // inherited 'prob' must be set like this
  }
  
  /** act
   * \param  p       particle pointer
   * \param  newPart vector of new particles
   * \return ???
   */
  int act(StParticle* p, vector<StParticle>* newPart=0);

  /// return a saved value
  int savedValue(size_t part_id, string name, double* v);
  //int aDistSwitch; //used to switch between options for the 2body angular distribution

  /// reset all saved values
  void savedReset() {
    fExEnMap.clear();
    fExEnMap2.clear();
    fPhiMap.clear();
    fPhiMap2.clear();
    fThetaMap.clear();
    fThetaMap2.clear();
    fPzMap.clear();
  }

  void setRngExEn                 (StRNG*                r) { fRngExEn               = r; }
  void setRngExEn1                (StRNG*                r) { fRngExEn1              = r; }
  void setRngExEn2                (StRNG*                r) { fRngExEn2              = r; }
  void setRngTwoNeutron_seq       (StRNGTwoNeutron_seq*  r) { fRngTwoNeutron_seq     = r; }
  void setRngUniform      (StRNGUniform*   r) { fRngUniform      = r; }
  void setRngGaussGlauber (StRNGGauss* r)  { fRngGaussGlauber = r; }
  // ** HL 01/10/2017
  void setRngGaussGlauberTrans(StRNGGauss* r) { fRngGaussGlauberTrans  = r; fUseTransKick = true; }
  void setGlauberKickScale(double v) { fGlauberKickScale = v; }
  // ** HL 01/10/2017
  void setGlauberTransScale1(double v) { fGlauberTransScale1 = v; }
  void setGlauberTransScale2(double v) { fGlauberTransScale2 = v; }
  //
  void setVelocityScale(double v) { fVelocityScale = v; }
  void setAsymMomentum(double v) { fAsymMomentum = v; }
  void setDebugSwitch(string dSwitch) {debugSwitch = dSwitch;}

private:
  int fNp;                   ///< number of protons to strip
  int fNn;                   ///< number of neutrons to strip

  StRNG*                fRngExEn;            ///< RNG excitation energy
  StRNG*                fRngExEn1;           ///< RNG excitation energy
  StRNG*                fRngExEn2;           ///< RNG excitation energy
  StRNGTwoNeutron_seq*  fRngTwoNeutron_seq;  ///< Two neutron RNG 
  StRNGUniform*         fRngUniform;         ///< uniform RNG (needed for emission angle etc.)
  StRNGGauss*           fRngGaussGlauber;    ///< gauss RNG (needed for Glauber momentum kick)
  // ** HL 01/10/2017
  StRNGGauss*           fRngGaussGlauberTrans;    ///< gauss RNG (needed for Glauber transverse momentum kick) - JKS
  //

  StRNG*        fRngParaMom;
  bool fUseParaMom;
  // ** HL 01/10/2017
  bool fUseTransKick;// JKS

  double fGlauberKickScale;   ///< scale for Glauber momentum kick
  // ** HL 01/10/2017
  double fGlauberTransScale1;// scale for Glauber transverse momentum kick, component 1 - JKS
  double fGlauberTransScale2;// scale for Glauber transverse momentum kcik, component 2 - JKS
  //
  double fVelocityScale;      ///< scale for reduced velocity after fragmentation, zwk
  double fAsymMomentum;       ///< scale for asymmetric mom. distribution for fragmentation based on LISE++, zwk

  map<size_t, double> fPzMap; ///< saved Parallel momeumtum distribution
  map<size_t, double> fParaMomMap; ///< saved Parallel momeumtum distribution
  map<size_t, double> fExEnMap;  ///< saved excitation energy
  map<size_t, double> fExEnMap2;  ///< saved excitation energy
  map<size_t, double> fPhiMap;   ///< saved cm phi angle
  map<size_t, double> fPhiMap2;   ///< saved cm phi angle
  map<size_t, double> fThetaMap; ///< saved cm theta angle
  map<size_t, double> fThetaMap2; ///< saved cm theta angle
  string debugSwitch;
  string fWhichEnergyDoIReconstruct;
};

/// stripping and evaporation reaction -- 3body decay
class StReacStripEvap_3bodydecay : public StReaction {
public:
  /// strip n neutrons and p protons and emit neutron 
  /// with const decay energy \todo allow non-const decay energy
  /// \param prob    Probability
  /// \param n       Number of neutrons to strip
  /// \param p       Number of protons  to strip
  /// \param exEnRng RNG for excitation energy
  StReacStripEvap_3bodydecay(double prob, int n, int p, StRNG* exEnRng, StRNG* exEn1Rng,StRNG* exEn2Rng, string WhichEnergyDoIReconstruct, int nbody_weight) : 
         StReaction("StReacStripEvap_dineutron"),
         fNp(p), fNn(n), fRngExEn(exEnRng), fRngExEn1(exEn1Rng), fRngExEn2(exEn2Rng), fRngUniform(0), fRngTwoNeutron_seq(0), 
	 fWhichEnergyDoIReconstruct(WhichEnergyDoIReconstruct), fGlauberKickScale(1), fVelocityScale(1), fAsymMomentum(1), fWeight(nbody_weight),  fUseTransKick(false), fGlauberTransScale1(1), fGlauberTransScale2(1) {// HL 01/10/2017
    setProb(prob);   // inherited 'prob' must be set like this
  }
  
  /** act
   * \param  p       particle pointer
   * \param  newPart vector of new particles
   * \return ???
   */
  int act(StParticle* p, vector<StParticle>* newPart=0);

  /// return a saved value
  int savedValue(size_t part_id, string name, double* v);
  //int aDistSwitch; //used to switch between options for the 2body angular distribution

  /// reset all saved values
  void savedReset() {
    fExEnMap.clear();
    fExEnMap2.clear();
    fPhiMap.clear();
    fPhiMap2.clear();
    fThetaMap.clear();
    fThetaMap2.clear();
    fPzMap.clear();
  }

  void setRngExEn                 (StRNG*                r) { fRngExEn               = r; }
  void setRngExEn1                (StRNG*                r) { fRngExEn1              = r; }
  void setRngExEn2                (StRNG*                r) { fRngExEn2              = r; }
  void setRngTwoNeutron_seq       (StRNGTwoNeutron_seq*  r) { fRngTwoNeutron_seq     = r; }
  void setRngUniform      (StRNGUniform*   r) { fRngUniform      = r; }
  void setRngGaussGlauber (StRNGGauss* r)  { fRngGaussGlauber = r; }
  // ** HL 01/10/2017
  void setRngGaussGlauberTrans(StRNGGauss* r) { fRngGaussGlauberTrans  = r; fUseTransKick = true; }// JKS
  //
  void setGlauberKickScale(double v) { fGlauberKickScale = v; }
  // ** HL 01/10/2017
  void setGlauberTransScale1(double v) { fGlauberTransScale1 = v; } // JKS
  void setGlauberTransScale2(double v) { fGlauberTransScale2 = v; } // JKS
  //
  void setVelocityScale(double v) { fVelocityScale = v; }
  void setAsymMomentum(double v) { fAsymMomentum = v; }
  void setDebugSwitch(string dSwitch) {debugSwitch = dSwitch;}

private:
  int fNp;                   ///< number of protons to strip
  int fNn;                   ///< number of neutrons to strip
  int fWeight;               // phase_space weighting flag

  StRNG*                fRngExEn;            ///< RNG excitation energy
  StRNG*                fRngExEn1;           ///< RNG excitation energy
  StRNG*                fRngExEn2;           ///< RNG excitation energy
  StRNGTwoNeutron_seq*  fRngTwoNeutron_seq;  ///< Two neutron RNG 
  StRNGUniform*         fRngUniform;         ///< uniform RNG (needed for emission angle etc.)
  StRNGGauss*           fRngGaussGlauber;    ///< gauss RNG (needed for Glauber momentum kick)
  // ** HL 01/10/2017
  StRNGGauss*           fRngGaussGlauberTrans;
  //

  StRNG*        fRngParaMom;
  bool fUseParaMom;
  // ** HL 01/10/2017
  bool fUseTransKick;//JKS
  //

  double fGlauberKickScale;   ///< scale for Glauber momentum kick
  // ** HL 01/10/2017
  double fGlauberTransScale1;
  double fGlauberTransScale2;
  //
  double fVelocityScale;      ///< scale for reduced velocity after fragmentation, zwk
  double fAsymMomentum;       ///< scale for asymmetric mom. distribution for fragmentation based on LISE++, zwk

  map<size_t, double> fPzMap; ///< saved Parallel momeumtum distribution
  map<size_t, double> fParaMomMap; ///< saved Parallel momeumtum distribution
  map<size_t, double> fExEnMap;  ///< saved excitation energy
  map<size_t, double> fExEnMap2;  ///< saved excitation energy
  map<size_t, double> fPhiMap;   ///< saved cm phi angle
  map<size_t, double> fPhiMap2;   ///< saved cm phi angle
  map<size_t, double> fThetaMap; ///< saved cm theta angle
  map<size_t, double> fThetaMap2; ///< saved cm theta angle
  string debugSwitch;
  string fWhichEnergyDoIReconstruct;
};


/// stripping and evaporation reaction
class StReacStripEvapTwoBody : public StReaction {
public:
  /// strip n neutrons and p protons and emit neutron 
  /// with const decay energy \todo allow non-const decay energy
  /// \param prob    Probability
  /// \param n       Number of neutrons to strip
  /// \param p       Number of protons  to strip
  /// \param exEnRng RNG for excitation energy
  StReacStripEvapTwoBody(double prob, int n, int p, StRNG* exEnRng) : 
         StReaction("StReacStripEvapTwoBody"),
         fNp(p), fNn(n), fRngExEn(exEnRng), fRngUniform(0),
         fGlauberKickScale(1), Qval(0), Qspread(0) , fVelocityScale(1), fAsymMomentum(1){
    setProb(prob);   // inherited 'prob' must be set like this
  }
  
  /** act
   * \param  p       particle pointer
   * \param  newPart vector of new particles
   * \return ???
   */
  int act(StParticle* p, vector<StParticle>* newPart=0);

  /// return a saved value
  int savedValue(size_t part_id, string name, double* v);

  /// reset all saved values
  void savedReset() {
    fExEnMap.clear();
    fPhiMap.clear();
    fThetaMap.clear();
    fPzMap.clear();
  }

  void setRngExEn   (StRNG*           r) { fRngExEn          = r; }
  void setRngUniform(StRNGUniform*    r) { fRngUniform       = r; }
  void setRngGaussGlauber(StRNGGauss* r) { fRngGaussGlauber  = r; }
  void setRngGaussQvalue(StRNGGauss*  r) { fRngGaussQvalue   = r; }

  void setGlauberKickScale(double v)     { fGlauberKickScale = v; }
  void setVelocityScale(double v) { fVelocityScale = v; }
  void setAsymMomentum(double v) { fAsymMomentum = v; }
  void setQval(double v)                 { Qval              = v; }
  void setQspread(double v)              { Qspread           = v; }
  void setAspread(double v)              { Aspread           = v; }
  void setDistSwitch(int swVar) {distSwitchBool = swVar;}

  void setLowLim(double lowerLimit) {aLowLim = lowerLimit;}
  void setUpLim(double upperLimit) {aUpLim = upperLimit;}
  void setDebugSwitch(string dSwitch) {debugSwitch = dSwitch;}
  double getLowLim() {return aLowLim;}
  double getUpLim() {return aUpLim;}
  string getDebugSwitch() {return debugSwitch;}
private:
  int fNp;                   ///< number of protons to strip
  int fNn;                   ///< number of neutrons to strip
  StRNG*        fRngExEn;    ///< RNG excitation energy
  StRNGUniform* fRngUniform; ///< uniform RNG (needed for emission angle etc.)
  StRNGGauss*   fRngGaussGlauber;   ///< gauss RNG (needed for Glauber momentum kick)
  StRNGGauss*   fRngGaussQvalue;    ///< gauss RNG (needed for two-body reaction Q-value)

  double fGlauberKickScale;   ///< scale for Glauber momentum kick
  double fVelocityScale;      ///< scale for reduced velocity after fragmentation, zwk
  double fAsymMomentum;       ///< scale for asymmetric mom. distribution for fragmentation based on LISE++, zwk
  double Qval;                ///< Centroid of reaction Q-value
  double Qspread;             ///< % spread in reaction Q-value
  double Aspread;             ///< spread in reaction angle (rads) (only for 2body)
  double aUpLim;
  double aLowLim;
  int distSwitchBool;
  
  map<size_t, double> fPzMap; ///< saved Parallel momeumtum distribution
  map<size_t, double> fExEnMap;  ///< saved excitation energy
  map<size_t, double> fPhiMap;   ///< saved cm phi angle
  map<size_t, double> fThetaMap; ///< saved cm theta angle
  string debugSwitch; 
};

#endif

