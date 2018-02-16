#ifndef MONA_ANALYSIS_HH
#define MONA_ANALYSIS_HH

#include <TTree.h>
#include <TLorentzVector.h>
#include <TObject.h>

#include <Rtypes.h>

#include "st_cosy_map.hh"
#include "st_par_mat_ia.hh"
#include "CycSrim.h"
//#include "st_rng.hh"

class CalMonaEvent;
class ParticleCoordinates;


/** Analysis class for mona data (simulated or measured).
 * It is assumed that there is only one neutron and one fragment
 */
class MonaAnalyzedEvent : public TObject {
public:
  /// initializes target element for targA targZ and thickness (mg/cm2)
  void initTarget();

  MonaAnalyzedEvent();

  void reset();
  
  int ana(CalMonaEvent* e);  ///< analyze event
  double manDE; //for manual control of the deltaE of Schiller map.
  int manDESwitch;
  int applyInvMap(ParticleCoordinates* c);
  //  int doChisqRecon(ParticleCoordinates* c);

  //void JacobiCorr(TVector3 FF, TVector3 N1, TVector3 N2, Double_t massF, Double_t massN1, Double_t massN2, double Tsystem[4], double Ysystem[4]);
  void JacobiCorr(TLorentzVector lv_F, TLorentzVector lv_N1, TLorentzVector lv_N2, Double_t massF, Double_t massN1, Double_t massN2, double Tsystem[4], double Ysystem[4], double &cosn1n2theta, double &nCM_ppar1, double &nCM_ppar2, double &nCM_pperp1, double &nCM_pperp2);

  void setMaps(CosyMap a, CosyMap b) { 
    fMap=a; 
    fInvMap=b; 
  }
  void setCycSrim(CycSrim* fSrimTarget){srimTarget = fSrimTarget;}
  void setSrimFoil(CycSrim* fSrimFoil){srimFoil   = fSrimFoil;}
  //void setRngGaus(StRNGGaus* p){fRngGaus = p;}
  
private:  // these go to the tree
  TVector3 vTPos;           // target position
  TVector3 vNPos;           // neutron position
  TVector3 vN2Pos;          // second neutron position
  TVector3 vN3Pos;	    // third neutron
  TVector3 vN4Pos;	    // fourth neutron
  TVector3 vG1NPos;         // 1st Geant4 neutron position  
  TVector3 vG2NPos;         // 2nd Geant4 neutron position
  TVector3 vG3NPos;         // 3rd Geant4 neutron position
  TVector3 vG4NPos;         // 4th Geant4 neutron position
  TVector3 vG5NPos;         // 5th Geant4 neutron position
  TVector3 vG6NPos;         // 6th Geant4 neutron position
  TLorentzVector lvN;       // 4-vector of the neutron
  TLorentzVector lvN2;      // 4-vector for the second neutron
  TLorentzVector lvN3;
  TLorentzVector lvN4;
  TLorentzVector lvG1N;     // 4-vector for the 1st Geant4 neutron
  TLorentzVector lvG2N;     // 4-vector for the 2nd Geant4 neutron
  TLorentzVector lvG3N;     // 4-vector for the 3rd Geant4 neutron
  TLorentzVector lvG4N;     // 4-vector for the 4th Geant4 neutron
  TLorentzVector lvG5N;     // 4-vector for the 5th Geant4 neutron
  TLorentzVector lvG6N;     // 4-vector for the 6th Geant4 neutron
  TLorentzVector lvFCosy;   // 4-vector of fragment after target, i.e. w/o energy-loss correction (using cosy map)
  TLorentzVector lvFSchill; // 4-vector of fragment after target, i.e. w/o energy-loss correction (using Schiller map)
  TLorentzVector lvFFit;    // 4-vector of fragment after target, i.e. w/o energy-loss correction (using Chi^2 min)
  double deltaE[3];         // energy-loss correction [0]/[1]/[2] - using no/nominal/measured beam energy
  double EDecay1,EDecay2,EDecay3,EDecayTotal;                                 // Decay Energies for the 1st and 2nd neutron along with total decay energy
  double EDecayTotalG1and2, EDecayTotalG3and4, EDecayG1, EDecayG2, EDecayG3, EDecayG4, EDecayG5, EDecayG6;  // Decay Energy for the first 6 neutrons from Geant4
  double EDecayGHigh, EDecayGLow, EDecayG_2neutron;  // high and low decay energies from geant
  double EDecayG_3neutron, EDecayTotalG12and3, EDecay4body, EDecay4body_True;
  double EDecay5body, EDecay5body_True;
  double altExenCosy[3];    // alternative excitation energy using Cosy Map (index as for deltaE[3])
  double altExenSchill[3];  // alternative excitation energy using Schiller Map (index as for deltaE[3])
  double altExenFit[3];     // alternative excitation energy using Chi^2 reconstr.  (index as for deltaE[3])
  double tLab;              // theta lab between fragment and neutron
  double tLab2;             // theta lab between fragment and second neutron
  double vRel;              // velocity difference: v_neutr - v_frag
  double vRelG1;            // ANK:
  double vRelG2;            // ANK:
  double vRel2;             // velocity difference: v_neutr2 - v_frag
  double nVel;              // velocity of neutrons!!: nVel = lvN.Beta() * 30 (cm/ns)
  double nVelG1;            // ANK: velocity using GEANT neutron1
  double nVelG2;            // ANK: velocity using GEANT neutron2
  double nVel2;             // velocity of second neutrons!!: nVel = lvN.Beta() * 30 (cm/ns)
  double fVel;              // velocity of fragment
  double fragKinE;          // Frag kinetic energy
  double neutronKinE;       // Neutron kinetic energy
  double nTheta;            // Neutron theta ?
  double nPhi;              // Neutrons Phi
  double neutronKinE2;      // Second Neutron kinetic energy
  double nTheta2;           // Second Neutron theta 
  double nPhi2;             // Second Neutrons Phi  
  double fTheta;            // Frag theta
  double fPhi;              // Frag Phi 
  double fAngleX;           // Frag x-angle at target
  double fAngleY;           // Frag y-angle at target
  double nAngleX;
  double nAngleY;
  double nAngleX2;
  double nAngleY2;
  double fpATA;
  double fpBTA;
  double fpYTA;
  double delta;
  double fYfp;
  double fBfp;
  double fXfp;
  double fAfp;
  

  //Jacobi T Y system values
  double Tsys_CosTheta1;
  double Tsys_CosTheta2;
  double Ysys_CosTheta1;
  double Ysys_CosTheta2;
  double Tsys_Ex_Et1;
  double Tsys_Ex_Et2;
  double Ysys_Ex_Et1;
  double Ysys_Ex_Et2;
  double Pz_FragN;
  double Pz_Frag;
  double Ppar_Frag;
  double Pperp_Frag;
  double Ptot_Frag;
  double nnCM_ppar1;
  double nnCM_ppar2;
  double nnCM_pperp1;
  double nnCM_pperp2;
  double Theta_3body;
  double Phi_3body;


  double n1n2costhetaCM;


private:  // internal things  (these are not part of the tree; due to this comment '//!'
  CosyMap fMap;       //! forward map
  CosyMap fInvMap;    //! (Schiller) inverse map

public:
  int fragA;          //!
  int fragQ;          //!
  double dTarg;       //!
  double tflatfoil;
  double bulge;
  double test;
  int targA;          //!
  int targZ;          //!
  double monaDist;    //!
  double fragMassExcess; //! MeV

private:
  StRageny fRageny;   //! energy loss correction
 // StRNGUniform* fRngUniform; 
 // StRNGGuas* fRngGaus;
public:
  StMaterial* matTarget;  //! material
  //StMaterial* matTest;
  CycSrim* srimTarget;
  CycSrim* srimFoil;


public:
  ClassDef(MonaAnalyzedEvent,1);
};

#endif // MONA_ANALYSIS_HH

