/** \file 
    implementation of target stuff
*/

#include <gsl/gsl_math.h>
#include "st_reaction.hh"
#include "st_system.hh"
#include <TLorentzVector.h>
#include <TGenPhaseSpace.h>
#include <stdio.h>
#include "TRandom3.h"
// C++ headers
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <functional>

// FILE *opf1 = fopen("test.out","w");

// For debugging

//FILE *pFile = fopen("/user/jonesm/tetra_neutrons/He10/pre_sequential.txt","w");
//FILE *pFile1 = fopen("/user/jonesm/tetra_neutrons/He10/post_sequential.txt","w");
//FILE *breakup = fopen("/user/jonesm/thesis_work/oxygen/break.txt","w");
/*FILE *pFile2 = fopen("/user/jonesm/tetra_neutrons/He10/post_dineutron.txt","w");
*/


//////////////////////// (d,p) reaction //////////////////////////////
int StReacDP::act(StParticle* p, vector<StParticle>* newPart) {
	/// check for all functions are initialized ///
  if (!p) ERR("Particle pointer not set.");
  if (!fRngUniform) ERR("Need uniform random number generator");
  if (!fRngAngDist) ERR("Need angular distribution random number generator");
  if (!fRngExEn) ERR("Need fRngExEn");
  if (!newPart) ERR("Pointer newPart not set; Cannot add new particles");

	/// intialize all variables ///
  double theta;			// angle theta
  double cosTheta;		// cos(theta)
  double phi;                 	// angle phi
  double eN,eF;   		// total neutron and fragment energy
  double pN,pF; 		// fragment and neutron momentum
  double eCM;   		// total CM energy
  double exen;  		// excitation energy
  TLorentzVector beam;
  TLorentzVector target;
  TLorentzVector total;
  TLorentzVector fragment;
  TLorentzVector neutron;
  TVector3 Boost_cm_dp;
  TVector3 Boost_cm_neutron_evaporation;

  double beta=p->getBeta();        	// initial beta
  p->setA(24.0);  //14                 	// remove protons and neutrons
  p->setm(p->getA()*931.494028);        // set new mass
  p->setZ(p->getZ());        		// set new Z
  p->setQ(p->getZ());			// set new Q
  p->setBeta(beta);			// set velocity

  // create the neutron, the particle p is already created
  StParticle pp;   
  pp.setA(1);
  pp.setm(1.0*939.565346);
  pp.setZ(0);
  pp.setQ(0);
  pp.setx(p->getx());     // same position
  pp.sety(p->gety()); 
  pp.setz(p->getz());     
  pp.sett(p->gett());     // same time
  pp.setEkin(0);

  // Setting up the tlorentz vectors for the beam/target
  beam.SetPxPyPzE(p->getPx(), p->getPy(), p->getPz(), p->getEtotal());
  target.SetPxPyPzE(0,0,0,2.0*931.494028);
  total.SetPxPyPzE(beam.Px(),beam.Py(),beam.Pz(),beam.E() + target.E());

  // Boosting the target/beam into the Center of Mass frame
  Boost_cm_dp = total.BoostVector();
  beam.Boost(-Boost_cm_dp);
  target.Boost(-Boost_cm_dp);

  eCM = beam.E()+target.E();  // total CM energy

  beam.SetPxPyPzE(0,0,0,25.0*931.494028);//15
  target.SetPxPyPzE(0,0,0,1.0*931.494028);

  eF = gsl_pow_2(eCM) - gsl_pow_2(target.M()) + gsl_pow_2(beam.M());
  eF = eF/(2.*eCM);                    // total energy of fragment

  pF = eF*eF - gsl_pow_2(beam.M());
  pF = sqrt(pF);

  theta    = fRngAngDist->value(0,M_PI);       // cos(theta)
  phi      = fRngUniform->value(0,2.*M_PI);  // phi
  fThetaMap[0] = theta;
  fPhiMap[0]   = phi;

  beam.SetPxPyPzE(0,0,pF,eF);
  beam.SetTheta(theta); // Set fragment angle
  beam.SetPhi(phi); // Set fragment phi angle

  // go to the lab
  beam.Boost(Boost_cm_dp);
//cout << beta << " " << beam.Beta() << endl;
  // start of neutron evaporation
  Boost_cm_neutron_evaporation = beam.BoostVector(); //create the boost 

  exen = 0;
  do {
    exen = fRngExEn->value();
    // printf("Here ########## fRngExEn %f",exen); // Test CRH
  } while (exen < 0);
  DSV(exen);
  fExEnMap[0]=exen;

  eCM = pp.getm() + p->getm() + exen;  // total CM energy
  eN = gsl_pow_2(eCM) + gsl_pow_2(pp.getm()) - gsl_pow_2(p->getm());
  eN = eN/(2.*eCM);                    // total energy neutron
  eF = gsl_pow_2(eCM) - gsl_pow_2(pp.getm()) + gsl_pow_2(p->getm());
  eF = eF/(2.*eCM);                    // total energy of fragment

  /// \todo check is arg of sqrt is positive
  pN = eN*eN - gsl_pow_2(pp.getm());
  pN = sqrt(pN);
  pF = eF*eF - gsl_pow_2(p->getm());
  pF = sqrt(pF);
  pF = -pF;  // fragment goes in opposite direction

  fragment.SetPxPyPzE(0,0,pF,eF);
  neutron.SetPxPyPzE(0,0,pN,eN);
  
  // set theta and phi to some random value  
  cosTheta = fRngUniform->value(-1,1);       // cos(theta)
  theta    = acos(cosTheta);
  phi      = fRngUniform->value(0,2.*M_PI);  // phi

  fragment.SetTheta(theta); // Set fragment angle
  neutron.SetTheta(M_PI - theta); // Set neutron angle 180-theta
  fragment.SetPhi(phi); // Set fragment phi angle
  neutron.SetPhi(phi + M_PI); // set neutron phi angle
  
  // go to the lab
  fragment.Boost(Boost_cm_neutron_evaporation);
  neutron.Boost(Boost_cm_neutron_evaporation);

  // set fragment energy and direction
  p->setEtotal(fragment.E()); /// \todo implement setLVec() to set particle data from 4-vector
  p->setThetaPhi(fragment.Theta(),fragment.Phi()); 
  
  // set neutron energy and direction
  pp.setEtotal(neutron.E());
  pp.setThetaPhi(neutron.Theta(),neutron.Phi()); 

  newPart->push_back(pp); 

  DSV(p->getA());
  DSV(p->getZ());

  return 0;
}

int StReacDP::savedValue(size_t part_id, string name, double* v) {
  DSV(part_id);
  DSV(name);
  if (name == "exen" &&  (fExEnMap.find(part_id) != fExEnMap.end()) ) {
    *v = fExEnMap[part_id];
    DSV(*v);
    return 0;
  }
  if (name == "ph" &&  (fPhiMap.find(part_id) != fPhiMap.end()) ) {
    *v = fPhiMap[part_id];
    DSV(*v);
    return 0;
  }
  if (name == "th" &&  (fThetaMap.find(part_id) != fThetaMap.end()) ) {
    *v = fThetaMap[part_id];
    DSV(*v);
    return 0;
  }
  return 1;
}
// End (d,p)

//////////////////////// Deuteron Breakup //////////////////////////////
int StReacDeuteronBreakup::act(StParticle* p, vector<StParticle>* newPart) {
    // borrowed code from D meson decay, the math is the same
	/// check for all functions are initialized ///
  if (!p) ERR("Particle pointer not set.");
  if (!fRngUniform) ERR("Need uniform random number generator");
  if (!fRngAngDist) ERR("Need angular distribution random number generator");
  if (!fRngExEn) ERR("Need fRngExEn");
  if (!newPart) ERR("Pointer newPart not set; Cannot add new particles");

	/// intialize all variables ///

  TRandom *random = new TRandom3();

  double A = p->getA();
  double beta=p->getBeta();     // initial beta
  p->setA(A);
  p->setm(p->getA()*931.494028);        // set new mass
  p->setZ(p->getZ());        		// set new Z
  p->setQ(p->getZ());			// set new Q
  p->setBeta(beta);			// set velocity

  // create the neutron, the particle p is already created

  StParticle neutron;   
  neutron.setA(1);
  neutron.setm(1.0*939.565346);
  neutron.setZ(0);
  neutron.setQ(0);
  neutron.setx(p->getx());     // same position
  neutron.sety(p->gety()); 
  neutron.setz(p->getz());     
  neutron.sett(p->gett());     // same time
  neutron.setEkin(0);

  // define constants, here I assume neutron and proton have same mass
  double K_M = p->getm(); //mass of beam
  double PI_M = neutron.getm(); // neutron proton mass
  double PI = 3.1415926535897932384626;
  double D_M;

  TLorentzVector beam;
  TLorentzVector target;
  TLorentzVector total;
  TVector3 boost_cm;
  TVector3 lab_boost;

 
  double targetA = 2*931.494061;// energy of target MeV
  double sum = p->getEtotal() + targetA;

  double exen = -1;

  fExEnMap[0]=exen;


  // set beam 4-vector
  beam.SetPxPyPzE(p->getPx(),p->getPy(),p->getPz(),p->getEtotal());
  // set target 4-vector
  target.SetPxPyPzE(0,0,0,targetA);

  // beam + target system
  total.SetPxPyPzE(p->getPx(),p->getPy(),p->getPz(), sum);

  TVector3 vP = total.Vect();

  // boost into target/beam CoM frame
  boost_cm = total.BoostVector();
  beam.Boost(-boost_cm);
  target.Boost(-boost_cm);

  lab_boost = target.BoostVector();
  total.Boost(-boost_cm);
  // set total energy in CoM frame
  D_M = total.E();

  
  // 4-vector of CoM frame
  TLorentzVector PD(0, 0, 0, D_M);


  // These are the quantities that you will create and make plots from

 // TLorentzVector KStar;
  TLorentzVector PK; // residual beam A
  TLorentzVector PPi1; // neutron
  TLorentzVector PPi2; // proton

  // 3-body mass pairs, sample uniformly
  double m12Sq,m23Sq,m13Sq;
      do {
          // (m_K+m_Pi)^2 < m12^2 < (D_M-m_Pi)^2
          m12Sq = fRngUniform->value(0,1)*(pow(D_M-PI_M,2)-pow(K_M+PI_M,2))+pow(K_M+PI_M,2);
          // (2m_Pi)^2 < m23^2 < (D_M-m_K)^2
          m23Sq = fRngUniform->value(0,1)*(pow(D_M-K_M,2)-pow(2*PI_M,2))+pow(2*PI_M,2);
          // m13^2 is now fixed
          m13Sq = D_M*D_M+K_M*K_M+2*PI_M*PI_M-m12Sq-m23Sq;
          // Calculate min and max of m23Sq
          // We need the energies of particles 2 and 3 in the m12 rest frame
          double E2Star = (m12Sq-K_M*K_M+PI_M*PI_M)/(2*sqrt(m12Sq));
          double E3Star = (D_M*D_M-m12Sq-PI_M*PI_M)/(2*sqrt(m12Sq));
          double m23SqMin = pow(E2Star+E3Star,2) - pow( sqrt(E2Star*E2Star-PI_M*PI_M) + sqrt(E3Star*E3Star-PI_M*PI_M) ,2);
          double m23SqMax = pow(E2Star+E3Star,2) - pow( sqrt(E2Star*E2Star-PI_M*PI_M) - sqrt(E3Star*E3Star-PI_M*PI_M) ,2);
          
          // This seems equivalent to declaring m23SqMin,m23SqMax outside the loop at first glance,
          // but this would allow the compiler to recognize the temporary nature of the variables,
          // and *might* lead to better optimization, but probably not.
          if ( m23Sq >= m23SqMin && m23Sq <= m23SqMax )
            break;
      } while (1);

      // This, after identifying 1,2,3 as K,Pi1,Pi2, respectively, sets the energy and magnitude of momentum.
      // beam
      double E1 = (D_M*D_M+K_M*K_M-m23Sq)/(2*D_M);
      double p1 = sqrt(E1*E1-K_M*K_M);
      // neutron
      double E2 = (D_M*D_M+PI_M*PI_M-m13Sq)/(2*D_M);
      double p2 = sqrt(E2*E2-PI_M*PI_M);
      // proton
      double E3 = (D_M*D_M+PI_M*PI_M-m12Sq)/(2*D_M);
      double p3 = sqrt(E3*E3-PI_M*PI_M);

   
      // This also sets the direction of 3-momentum since
      // m12^2 = (P1 + P2)^2 = m_K^2 + m_Pi^2 + 2P1.P2
      //                     = m_K^2 + m_Pi^2 + 2E1*E2 - 2|p1||p2|cos(theta12)
      double cosTheta12 = (K_M*K_M+PI_M*PI_M+2*E1*E2-m12Sq)/(2*sqrt((E1*E1-K_M*K_M)*(E2*E2-PI_M*PI_M)));
      // and similarly for theta13
      double cosTheta13 = (K_M*K_M+PI_M*PI_M+2*E1*E3-m13Sq)/(2*sqrt((E1*E1-K_M*K_M)*(E3*E3-PI_M*PI_M)));
      
      // We'll put the kaon along the x axis, and set the two pions on either side in the x-y plane.
      // Then, we'll rotate all of them using the axis-angle method, with an appropriate c.d.f. for the
      // angle as described in http://en.wikipedia.org/wiki/Rotation_matrix#Uniform_random_rotation_matrices
      // It is important to note that just picking three random euler angles will _not_ give a uniform
      // rotation, as several sets of angles describe equivalent rotations.
      //

    // use TGenPhaseSpace
   /*   double set_mass = D_M;
      TLorentzVector t1gev;
      t1gev.SetVectM(vP*(1/1000.0), (set_mass)/1000);
      double Mass3body[3] = {K_M/1000.0, PI_M/1000.0, PI_M/1000.0};
      TGenPhaseSpace tPSevent;
      Bool_t bevent = tPSevent.SetDecay(t1gev, 3, Mass3body);
      if(!bevent) cerr << "Decay is Kinematically FORBIDDEN" << endl;
       
      tPSevent.Generate();


      PK = tPSevent.GetDecay(0);
      PPi1 = tPSevent.GetDecay(1);
      PPi2 = tPSevent.GetDecay(2);*/


     
      PK.SetPxPyPzE(p1,0,0,E1);
      // Set Pi1 in positive y
      PPi1.SetPxPyPzE(p2*cosTheta12,p2*sqrt(1-cosTheta12*cosTheta12),0,E2);
      // and Pi2 in negative y
      PPi2.SetPxPyPzE(p3*cosTheta13,-p3*sqrt(1-cosTheta13*cosTheta13),0,E3);
            
      // Now, rotate
      double cosTheta = fRngUniform->value(0,2)-1.;
      double phi = fRngUniform->value(0,1)*2*PI;
      TVector3 axis(cos(phi)*sqrt(1-cosTheta*cosTheta), 
                    sin(phi)*sqrt(1-cosTheta*cosTheta),
                    cosTheta);
      // Monte-Carlo cdf generation for the rotation angle
      // cdf should be (a-sin(a))/pi
      // so pdf is (1-cos(a))/pi, max would be 2/pi
      // hence 1-cos(a) < u*2 is our test
      double angle;
      do {
        angle = random->Rndm()*PI;
      } while ( 1-cos(angle) < random->Rndm()*2 );
      
      PK.Rotate(angle, axis);
      PPi1.Rotate(angle, axis);
      PPi2.Rotate(angle, axis);

      fThetaMap[0] = cosTheta;
      fPhiMap[0]   = phi;

 
      // now boost products back
      // no boost with TGenPhaseSpace
      PD.Boost(boost_cm);
      PK.Boost(boost_cm);
      PPi1.Boost(boost_cm);
      PPi2.Boost(boost_cm);
    

  // set fragment energy and direction
  p->setEtotal(PK.E()); /// \todo implement setLVec() to set particle data from 4-vector
  p->setThetaPhi(PK.Theta(),PK.Phi()); 
  
  // set neutron energy and direction
  neutron.setEtotal(PPi1.E());
  neutron.setThetaPhi(PPi1.Theta(),PPi1.Phi()); 

  // we could care about the proton but we dont

  newPart->push_back(neutron); 

  DSV(p->getA());
  DSV(p->getZ());

  return 0;
}

int StReacDeuteronBreakup::savedValue(size_t part_id, string name, double* v) {
  DSV(part_id);
  DSV(name);
  if (name == "exen" &&  (fExEnMap.find(part_id) != fExEnMap.end()) ) {
    *v = fExEnMap[part_id];
    DSV(*v);
    return 0;
  }
  if (name == "ph" &&  (fPhiMap.find(part_id) != fPhiMap.end()) ) {
    *v = fPhiMap[part_id];
    DSV(*v);
    return 0;
  }
  if (name == "th" &&  (fThetaMap.find(part_id) != fThetaMap.end()) ) {
    *v = fThetaMap[part_id];
    DSV(*v);
    return 0;
  }
  return 1;
}

// Two-Body Kinematics for breakup
int StReacDeuteronBreakupTwoBody::act(StParticle* p, vector<StParticle>* newPart) {
    // borrowed code from D meson decay, the math is the same
	/// check for all functions are initialized ///
  if (!p) ERR("Particle pointer not set.");
  if (!fRngUniform) ERR("Need uniform random number generator");
  if (!fRngAngDist) ERR("Need angular distribution random number generator");
  if (!fRngExEn) ERR("Need fRngExEn");
  if (!newPart) ERR("Pointer newPart not set; Cannot add new particles");

	/// intialize all variables ///

  TRandom *random = new TRandom3();

  double A = p->getA();
  double beta=p->getBeta();     // initial beta
  p->setA(A);
  p->setm(p->getA()*931.494028);        // set new mass
  p->setZ(p->getZ());        		// set new Z
  p->setQ(p->getZ());			// set new Q
  p->setBeta(beta);			// set velocity

  // create the neutron, the particle p is already created

  StParticle StNeutron;   
  StNeutron.setA(1);
  StNeutron.setm(1.0*939.565346);
  StNeutron.setZ(0);
  StNeutron.setQ(0);
  StNeutron.setx(p->getx());     // same position
  StNeutron.sety(p->gety()); 
  StNeutron.setz(p->getz());     
  StNeutron.sett(p->gett());     // same time
  StNeutron.setEkin(0);

  // define constants, here I assume neutron and proton have same mass
  double K_M = p->getm(); //mass of beam
  double PI_M = StNeutron.getm(); // neutron proton mass
  double PI = 3.1415926535897932384626;
  double D_M;

  TLorentzVector beam;
  TLorentzVector target;
  TLorentzVector total;
  TVector3 boost_cm;
  TVector3 lab_boost;
  TVector3 deuteron_rest_frame;


  TLorentzVector fragment;
  TLorentzVector deuteron;
  TLorentzVector neutron;
  TLorentzVector proton;


  double beamA = 24*931.494061;
  double targetA = 2*931.494061;// energy of target MeV
  double sum = p->getEtotal() + targetA;
  double beamE = sqrt(9152*9152+beamA*beamA);

  double exen = -1;

  fExEnMap[0]=exen;


  // set beam 4-vector
  beam.SetPxPyPzE(p->getPx(),p->getPy(),p->getPz(),p->getEtotal());
 //   beam.SetPxPyPzE(0,0,p->getP(),p->getEtotal()); 
 // set target 4-vector
  target.SetPxPyPzE(0,0,0,targetA);

  // beam + target system
  total.SetPxPyPzE(p->getPx(),p->getPy(),p->getPz(), sum);

  
  TVector3 vP = total.Vect();

  // boost into target/beam CoM frame
  boost_cm = total.BoostVector();
 // cout << "boost_cm: " << endl;
 // boost_cm.Print();
  beam.Boost(-boost_cm);
  target.Boost(-boost_cm);

  lab_boost = target.BoostVector();
  total.Boost(-boost_cm);
  // set total energy in CoM frame
  D_M = total.E();

  double eTarget;
  double eBeam;

  double pTarget;
  double pBeam;

  double eN, pN;
  double eP, pP;
 
  eTarget = D_M*D_M + target.M()*target.M() - beam.M()*beam.M();
  eTarget = eTarget/(2*D_M);

  eBeam = D_M*D_M - target.M()*target.M() + beam.M()*beam.M();
  eBeam = eBeam/(2*D_M);


  pTarget = eTarget*eTarget - target.M()*target.M();
  pTarget = sqrt(pTarget);

  pBeam = eBeam*eBeam - beam.M()*beam.M();
  pBeam = -sqrt(pBeam);

  double cosTheta = fRngUniform->value(-1,1);
  double theta = acos(cosTheta);
  double phi = fRngUniform->value(0,2*PI);

  fragment.SetPxPyPzE(0,0,pBeam,eBeam);
  fragment.SetTheta(theta);
  fragment.SetPhi(phi);

  deuteron.SetPxPyPzE(0,0,pTarget, eTarget);
  deuteron.SetTheta(PI - theta);
  deuteron.SetPhi(PI + phi);
  //fragment.Print();
  //deuteron.Print();

  // deuteron rest frame to target + D2 frame
  deuteron_rest_frame = deuteron.BoostVector();
 // boost_cm.Print();
  //deuteron_rest_frame.Print();

  // now do breakup of deuteron into n + p with Q = 2.2 MeV
  neutron.SetPxPyPzE(0,0,0,939.565);
  proton.SetPxPyPzE(0,0,0,939.565);
  double Q = 2.2;
  double eCM;

  eCM = neutron.M() + proton.M() + Q;
  eN = eCM*eCM + neutron.M()*neutron.M() - proton.M()*proton.M();
  eN = eN/(2*eCM);

  eP = eCM*eCM - neutron.M()*neutron.M() + proton.M()*proton.M();
  eP = eP/(2*eCM);

  pN = eN*eN - neutron.M()*neutron.M();
  pN = sqrt(pN);

  pP = eP*eP - proton.M()*proton.M();
  pP = -sqrt(pP);

  neutron.SetPxPyPzE(0,0,pN, eN);
  proton.SetPxPyPzE(0,0,pP,eP);

  double cosThetaN = fRngUniform->value(-1,1);
  double thetaN = acos(cosThetaN);
  double phiN = fRngUniform->value(0,2*PI);

  neutron.SetTheta(thetaN);
  neutron.SetPhi(phiN);

  proton.SetTheta(PI-theta);
  proton.SetPhi(phiN + PI);
  
  fThetaMap[0]=thetaN;
  fPhiMap[0]=phiN;

  // boost deuteron + fragment back to lab
  fragment.Boost(boost_cm);
  deuteron.Boost(boost_cm);

  proton.Boost(deuteron_rest_frame);
  neutron.Boost(deuteron_rest_frame);

  proton.Boost(boost_cm);
  neutron.Boost(boost_cm);
  
  //boost_cm.Print();
 // deuteron_rest_frame.Print();

//  fprintf(breakup, "%f \t %f \n", neutron.E() - neutron.M(), neutron.Beta());

  //cout << neutron.E() - neutron.M() << endl;
      

  // set fragment energy and direction
  p->setEtotal(fragment.E()); /// \todo implement setLVec() to set particle data from 4-vector
  p->setThetaPhi(fragment.Theta(),fragment.Phi()); 
  
  // set neutron energy and direction
  StNeutron.setEtotal(neutron.E());
  StNeutron.setThetaPhi(neutron.Theta(),neutron.Phi()); 

  // we could care about the proton but we dont

  newPart->push_back(StNeutron); 

  DSV(p->getA());
  DSV(p->getZ());

  return 0;
}

int StReacDeuteronBreakupTwoBody::savedValue(size_t part_id, string name, double* v) {
  DSV(part_id);
  DSV(name);
  if (name == "exen" &&  (fExEnMap.find(part_id) != fExEnMap.end()) ) {
    *v = fExEnMap[part_id];
    DSV(*v);
    return 0;
  }
  if (name == "ph" &&  (fPhiMap.find(part_id) != fPhiMap.end()) ) {
    *v = fPhiMap[part_id];
    DSV(*v);
    return 0;
  }
  if (name == "th" &&  (fThetaMap.find(part_id) != fThetaMap.end()) ) {
    *v = fThetaMap[part_id];
    DSV(*v);
    return 0;
  }
  return 1;
}

// End Deuteron Breakup




//////////////////////////////////////////////////////////////////////////
// ==========================================================
//          FOUR NEUTRON REACTION CLASSES:
//     5bodydecay, StripEvap_5bodydecay, StripEvap4Neutron
// ==========================================================
////////////////////////////4 Neutron Decay///////////////////////////////

//---------------------5-body Sequential-------------------------//
int StReacSequential_5bodydecay::act(StParticle* p, vector<StParticle>* newPart) { //cout << "5body Sequential!" << endl;


  //Setting up the two decay energies
  double exen     = 0.0; //Total Decay Energy
  double exen1    = 0.0; //1st Decay Energy
  double exen2    = 0.0; //2nd Decay Energy
  double exen3    = 0.0;
  double exen4    = 0.0;
  //Create Varibles to give give directions to particles and Glauber Kick
  double cosTheta;     //Used to set the cos(Theta)
  double theta;        //Used to set the Theta
  double phi;          //Used to set the Phi

  //for volya 2n
  double erel = 0; //rel energy between 2 neutrons
  double di_ei=0, di_ek=0;

  //Check that all necassery random number generators are found
  if (!fRngExEn)           ERR("Need fRngExEn");
  if (!fRngExEn1)          ERR("Need fRngExEn1");
  if (!fRngExEn2)          ERR("Need fRngExEn2");
  if (!fRngExEn3)	       ERR("Need fRngExEn3");
  if (!fRngExEn4) 	       ERR("Need fRngExEn4");
  if (!fRngUniform)        ERR("Need uniform random number generator");
  if (!fRngGaussGlauber)   ERR("Need gauss random number generator");

  //Removed the fWhichEnergyDoIReconstruct (since it isn't needed for 3body decay)

  double exenTotal = -1;
  while(1){
    exenTotal = fRngExEn->value(); //MeV 
    //cout << "exenTotal: " << exenTotal << endl;
    if(exenTotal>0) break;
  }



    // Here we determine excitation energies
    //double exenTotal = -1;
    double exenDiNeutron = -1;
	

    if(fWhichEnergyDoIReconstruct=="volyadineutron"){ //cout << "Coreectly entered dinuetron" << endl;
	    		//for volya + bw
    		fRngExEn3->value2d(di_ei,di_ek); 
    
   		    exenTotal = di_ei + di_ek;
    		exenDiNeutron = di_ei;
		//cout << "exenTotal: " << exenTotal << " exenDiNeutron :" << di_ei << endl;


	do{
		exen1 = fRngExEn1->value();// cout << "exen1: " << exen1 << endl;
		//exen1=0.01;
  	} 
	 while (exen1 < 0);
	 do{
		exen2 = fRngExEn2->value(); //cout << "exen2: " << exen2 << endl;
		//exen2 = 10;
  	 }  
	 while (exen2 < 0);
	 /*do{
		exen3 = fRngExEn3->value();
		//exen3 = 0.01;
  	 }  
	 while (exen3 < 0);
	 do{
		exen4 = fRngExEn4->value();
		//exen3 = 0.01;
  	 }  
	 while (exen4 < 0);*/
   
	DSV(exenTotal);
 	DSV(exenDiNeutron);
 	fExEnMap[0]=exen1; // thermal 1
  	fExEnMap2[0]=exen2; // thermal 2
  	fExEnMap3[0]=exenTotal;
  	fExEnMap4[0]=exenDiNeutron;  


   } // end volya


    else if(fWhichEnergyDoIReconstruct=="grigorenko"){


     // First a total 5body energy
     do{
         exen = fRngExEn1->value();
     }
      while(exen < 0);
     
          
 
     // Now sample a total 3body energy LESS than the 12He
     
     //while(exen < exenTotal){
      do{ 
        //do{
             fRngExEn3->value2d(di_ei,di_ek); 
             exenTotal = di_ei + di_ek;
             exenDiNeutron = di_ei;
         //}
         //while( exenTotal < 0);
      }
      while( exen < exenTotal);
     //}

         
     double r;
     //r = ((double) rand() / (RAND_MAX));
     r = 0.5;
     // distribute the remaining energy based on a random number between zero and 1
     exen1 = (exen - exenTotal)*r;
     exen2 = (exen - exenTotal)*(1-r);
    
     if(exen1 < 0 || exen2 < 0 || exenTotal < 0 || exen < 0 || exen3 < 0 || exen4 < 0) cout << "SOMETHING WENT NEGATIVEEEEEEE" << endl;

     DSV(exenTotal);
 	 DSV(exenDiNeutron);
 	 fExEnMap[0]=exen1; // thermal 1
  	 fExEnMap2[0]=exen2; // thermal 2
  	 fExEnMap3[0]=exenTotal;
  	 fExEnMap4[0]=exenDiNeutron;  



    } // end grigorenko


   else{ 
       do{
           exen1 = fRngExEn1->value();
       }
       while(exen1 < 0);
       do{
           exen2 = fRngExEn2->value();
       }
       while(exen2 < 0);
       do{
           exen3 = fRngExEn3->value();
       }
       while(exen3 < 0);

       
       /*// Sequential to 3 body
         while(1){ //cout << "Went anti-dineutron" << endl;
		exen1 = fRngExEn1->value(); //cout << "exen1: " << exen1 << endl;
		//exen1=0.01;
		if(exen1 > 0 && exen1 < exenTotal) break;
  	 }  
	 do{
		exen2 = exenTotal-exen1; //cout << "exen2: " << exen2 << endl;
					 //cout << "exenTotal: " << exenTotal << endl;
					 //cout << "EXEN1:  " << exen1 << endl;
		//exen2 = 10;
  	 }  
	 while (exen2 < 0);
	 do{
		exen3 = fRngExEn3->value(); //cout << "exen3: " << exen3 << endl;
		//exen3 = 0.01;
  	 }  
	 while (exen3 < 0);
	 //do{
	 //	exen4 = fRngExEn4->value();
		//exen3 = 0.01;
  	 //}  
	 //while (exen4 < 0);*/



 	DSV(exenTotal);
  	DSV(exen2);
  	fExEnMap[0]=exen1; 
  	fExEnMap2[0]=exen2;  
  	fExEnMap3[0]=exen3;
  	fExEnMap4[0]=exen4;  

   }
 
  //debugging stuff -> Decay Energies
  if (debugSwitch == "exen1")
	  cout << exen1 << endl;
  if (debugSwitch == "exen2")
	  cout << exen2 << endl;
  if (debugSwitch == "exen3")
	  cout << exen3 << endl;
  if (debugSwitch == "exen4")
	  cout << exen4 << endl;

  if (!p) ERR("Particle pointer not set.");


  //------------------------------------------------------------------------------------------------------
  // Begin 4n decay, starting with removing 2n from distributions exen1 and exen2
  //cout << "Begin 4n decay!" << endl;

  // change particle, keep velocity the same
  //**zwk reduce beta after reaction following LISE++
  double beta = p->getBeta() * fVelocityScale;        // initial beta
  double A = p->getA();            // initial mass number
  //cout << "Mass: " << p->getA() << " | " << fNn << " | " << fNp << endl;
  p->setA(p->getA() - fNn - fNp);  // remove protons and neutrons
  p->setm(p->getA()*931.494028);        // set new mass
  p->setZ(p->getZ() - fNp);        // set new Z
  p->setQ(p->getZ());              // set new Q
  p->setBeta(beta);                // set velocity

  //cout << "Incoming Etotal: " << p->getEtotal() << endl;
  //cout << "EDecay: " << exenTotal << endl;

  TLorentzVector tl;
  tl.SetPxPyPzE(p->getPx(), p->getPy(), p->getPz(), p->getEtotal());

  DSV(tl.Theta());
  DSV(tl.Phi());
  DSV(tl.E() - tl.M());

  fPzMap[0]=-1;
  //-------------------------------------------------------------------------------------------------------
  // Glauber momentum kick 
  double sig, sig0;
  sig0 = 90;  // MeV/c
  sig = p->getA() * ( fNn + fNp ) / ( A - 1);
  if (sig < 0) ERR("sig < 0; sig = %g", sig);
  sig = fGlauberKickScale * sig0 * sqrt(sig);
  DSV(sig);
  
  cosTheta = fRngUniform->value(-1,1);// cos(theta)
  theta    = acos(cosTheta);
  phi      = fRngUniform->value(0,2.*M_PI);  // phi
  DSV(theta);
  DSV(phi);

  fRngGaussGlauber->setCentroid(0);
  fRngGaussGlauber->setSigma(sig);

  double glaubP = fRngGaussGlauber->value();
  //-=-=-==--=-=-=-==
  //idea to create tail in mom. distribution following LISE++, zwk
  if(glaubP < 0) glaubP = glaubP * fAsymMomentum;
  //--------------------

  DSV(glaubP);
  TVector3 vGlaubP(0,0,glaubP);
  vGlaubP.SetTheta(theta);
  vGlaubP.SetPhi(phi);
  
  TVector3 vP;                  // total momentum
  vP = tl.Vect() + vGlaubP;
  tl.SetVectM(vP,p->getm());
  DSV(tl.Theta());
  DSV(tl.Phi());
  DSV(tl.E() - tl.M());
  
  //debugging stuff
  if (debugSwitch == "thetaLABP")
    cout << tl.Theta()<< endl;
  if (debugSwitch == "phiLABP")
    cout << tl.Phi()<< endl;
  
  // End Glauber Kick, Record Boost
  TVector3 t3Boost;
  t3Boost = tl.BoostVector();  // boost vector 
  //cout << pFile << endl;
  //fprintf(pFile, "test \n");
  //fprintf(pFile, "%f \t %f \t %f \t %f \t %i \n" ,tl.Px() ,tl.Py() ,tl.Pz() ,tl.E(), p->getA() );

  //-------------------------------------------------------------------------------------------------------
  // Remove the first 2 neutrons, then do a 3body or di-neutron decay
  // start of neutron evaporation
  p->setA(p->getA() - 1);
  p->setm(p->getA()*931.494088);
  p->setEkin(0);             

  TLorentzVector tl1;  // particle 1 (heavy fragment)
  tl1.SetPxPyPzE(p->getPx(), p->getPy(), p->getPz(), p->getEtotal());

  // create neutron
  StParticle pp;   
  pp.setA(1);
  pp.setm(pp.getA()*939.565346);//939.565346
  pp.setZ(0);
  pp.setQ(0);
  pp.setx(p->getx());     // same position
  pp.sety(p->gety()); 
  pp.setz(p->getz());     
  pp.sett(p->gett());     // same time
  pp.setEkin(0);
  
  TLorentzVector tl2;  // particle 2 (neutron)
  tl2.SetPxPyPzE(pp.getPx(), pp.getPy(), pp.getPz(), pp.getEtotal());
  
  double eN, eF;   // total neutron and fragment energy
  double eCM;   // total CM energy

  eCM = pp.getm() + p->getm() + exen1;  // total CM energy
  eN = gsl_pow_2(eCM) + gsl_pow_2(pp.getm()) - gsl_pow_2(p->getm());
  eN = eN/(2.*eCM);                    // total energy neutron
  eF = gsl_pow_2(eCM) - gsl_pow_2(pp.getm()) + gsl_pow_2(p->getm());
  eF = eF/(2.*eCM);                    // total energy of fragment

  /// \todo check is arg of sqrt is positive
  double pN,pF; // fragment and neutron momentum
  pN = eN*eN - gsl_pow_2(pp.getm());
  pN = sqrt(pN);
  pF = eF*eF - gsl_pow_2(p->getm());
  if (pF < 0)
    pF = 0;
  else 
    pF = sqrt(pF);
  pF = -pF;  // fragment goes in opposit direction

  tl1.SetPxPyPzE(0,0,pF,eF);
  tl2.SetPxPyPzE(0,0,pN,eN);
  
  // set theta and phi for the first decay to some random value
  cosTheta = fRngUniform->value(-1,1);       // cos(theta)
  theta    = acos(cosTheta);
  phi      = fRngUniform->value(0,2.*M_PI);  // phi
  fThetaMap[0] = theta;
  fPhiMap[0]   = phi;

  tl1.SetTheta(theta); // Set fragment angle
  tl1.SetPhi(phi); // Set fragment phi angle
  tl2.SetTheta(M_PI - theta); // Set neutron angle 180-theta
  tl2.SetPhi(phi + M_PI); // set neutron phi angle

  TVector3 t4Boost;
  t4Boost = tl1.BoostVector();  // boost vector for 24O


  //-------------------------------------------------------------------------------------------------------
  // start of the second neutron evaporation


  p->setA(p->getA()-1);
  p->setm(p->getA()*931.494088);//931.494028
  p->setEkin(0);               // 23O at rest

  // create second neutron
  StParticle ppp;   
  ppp.setA(1);
  ppp.setm(ppp.getA()*939.565346);//939.565346//939.565378
  ppp.setZ(0);
  ppp.setQ(0);
  ppp.setx(p->getx());     // same position
  ppp.sety(p->gety()); 
  ppp.setz(p->getz());     
  ppp.sett(p->gett());     // same time
  ppp.setEkin(0);
  
  TLorentzVector tl3;  // particle 3 (neutron)
  tl3.SetPxPyPzE(ppp.getPx(), ppp.getPy(), ppp.getPz(), ppp.getEtotal());

  double eN2, eF2;   // total neutron and fragment energy
  double eCM2;       // total CM energy

  eCM2 = ppp.getm() + p->getm() + exen2;  // total CM energy
  eN2 = gsl_pow_2(eCM2) + gsl_pow_2(ppp.getm()) - gsl_pow_2(p->getm());
  eN2 = eN2/(2.*eCM2);                    // total energy neutron
  eF2 = gsl_pow_2(eCM2) - gsl_pow_2(ppp.getm()) + gsl_pow_2(p->getm());
  eF2 = eF2/(2.*eCM2);                    // total energy of fragment

  /// \todo check is arg of sqrt is positive
  double pN2, pF2; // fragment and neutron momentum
  pN2 = eN2*eN2 - gsl_pow_2(ppp.getm());
  pN2 = sqrt(pN2);
  pF2 = eF2*eF2 - gsl_pow_2(p->getm());
  if (pF2 < 0)
    pF2 = 0;
  else 
    pF2 = sqrt(pF2);
  pF2 = -pF2;  // fragment goes in opposite direction

  tl1.SetPxPyPzE(0,0,pF2,eF2);
  tl3.SetPxPyPzE(0,0,pN2,eN2);
  
  // set theta and phi to some random value
  cosTheta = fRngUniform->value(-1,1);       // cos(theta)
  theta    = acos(cosTheta);
  phi      = fRngUniform->value(0,2.*M_PI);  // phi
  fThetaMap2[0] = theta;
  fPhiMap2[0]   = phi;
  tl1.SetTheta(theta);        // Set fragment angle
  tl1.SetPhi(phi);            // Set fragment phi angle
  tl3.SetTheta(M_PI - theta); // Set neutron angle 180-theta
  tl3.SetPhi(phi + M_PI);     // Set neutron phi angle

  TVector3 t5Boost;
  t5Boost = tl1.BoostVector(); 
 
  /*
  // This is the correct way to boost back from 2n evap to lab frame
  // Up to here, P and E are conserved exactly if exen1 = exen2 = 0, glaub = 0, and 1 amu = neutron mass.

  //Boosting into the frame where the fragment is at rest after the first 1n decay
  tl1.Boost(t4Boost); 
  tl3.Boost(t4Boost);//Must boost each neutron back to the frame where fragment is at rest after 1st decay

 
  //Boosting all 5 particles to the lab frame
  tl1.Boost(t3Boost);
  tl2.Boost(t3Boost);
  tl3.Boost(t3Boost);

  //cout << "================ AFTER 2n EVAPORTATION =======================" << endl;
  //printf("Fragment:  %f | %f | %f | %f  | %i \n" ,tl1.Px() ,tl1.Py() ,tl1.Pz() ,tl1.E(), p->getA() );
  //printf("1n:        %f | %f | %f | %f  | %i \n" ,tl2.Px() ,tl2.Py() ,tl2.Pz() ,tl2.E(), pp.getA() );
  //printf("2n:        %f | %f | %f | %f  | %i \n" ,tl3.Px() ,tl3.Py() ,tl3.Pz() ,tl3.E(), ppp.getA() );
  //printf("Total:     %f | %f | %f | %f  \n" ,tl1.Px() +tl2.Px() + tl3.Px() , tl1.Py() + tl2.Py() + tl3.Py() , tl1.Pz() + tl2.Pz() + tl3.Pz() ,tl1.E() + tl2.E() + tl3.E());

  //fprintf(pFile1," %f \t %f \t %f \t %f \n", tl1.Px() +tl2.Px() + tl3.Px() , tl1.Py() + tl2.Py() + tl3.Py() , tl1.Pz() + tl2.Pz() + tl3.Pz() , tl1.E() + tl2.E() + tl3.E());


  // now anti-boost after check
  tl1.Boost(-t3Boost);
  tl2.Boost(-t3Boost);
  tl3.Boost(-t3Boost);

  //Boosting into the frame where the fragment is at rest after the first 1n decay
  tl1.Boost(-t4Boost); //tl1.Boost(-t5Boost); 
  tl3.Boost(-t4Boost);//Must boost each neutron back to the frame where fragment is at rest after 1st decay
*/

  //-------------------------------------------------------------------------------------------------------
  // Begin di neutron decay
  // 
 
  if(fWhichEnergyDoIReconstruct=="volyadineutron" || fWhichEnergyDoIReconstruct == "grigorenko"){

   // start of di-neutron evaporation
    p->setA(p->getA() - 2);
    p->setm(p->getA()*931.494028);
    p->setEkin(0);             

   // create di-neutron
   StParticle dineutron;   
   dineutron.setA(2);
   dineutron.setm(dineutron.getA()*939.565346);//939.565346
   dineutron.setZ(0);
   dineutron.setQ(0);
   dineutron.setx(p->getx());     // same position
   dineutron.sety(p->gety()); 
   dineutron.setz(p->getz());     
   dineutron.sett(p->gett());     // same time
   dineutron.setEkin(0);
  
   TLorentzVector tl4;  // di-neutron
   tl4.SetPxPyPzE(dineutron.getPx(), dineutron.getPy(), dineutron.getPz(), dineutron.getEtotal());

 
  
  double e2N_d, eF_d;   // total neutron and fragment energy
  double eCM_d;   // total CM energy

  //Make sure don't have negative decay energies.
  //Need to develop how we choose dineutron and frag-2n energies.
  double exen12_left = exenTotal - exenDiNeutron;
  if(exen12_left <= 0){
    cerr << "ExEn Left is less than ZERO!!" << endl;
  }
  eCM_d = dineutron.getm() + p->getm() + (exen12_left);  // total CM energy 
  e2N_d = gsl_pow_2(eCM_d) + gsl_pow_2(dineutron.getm()) - gsl_pow_2(p->getm());
  e2N_d = e2N_d/(2.*eCM_d);                    // total energy neutron
  eF_d = gsl_pow_2(eCM_d) - gsl_pow_2(dineutron.getm()) + gsl_pow_2(p->getm());
  eF_d = eF_d/(2.*eCM_d);                    // total energy of fragment

  /// \todo check is arg of sqrt is positive
  double p2N_d,pF_d; // fragment and neutron momentum
  p2N_d = e2N_d*e2N_d - gsl_pow_2(dineutron.getm());
  p2N_d = sqrt(p2N_d);
  pF_d = eF_d*eF_d - gsl_pow_2(p->getm());
  if (pF_d < 0)
    pF_d = 0;
  else 
    pF_d = sqrt(pF_d);
  pF_d = -pF_d;  // fragment goes in opposit direction

  tl1.SetPxPyPzE(0,0,pF_d,eF_d); //cout << "pF_d " << pF_d << endl;
  tl4.SetPxPyPzE(0,0,p2N_d,e2N_d);
  
  // set theta and phi for the first decay to some random value
  cosTheta = fRngUniform->value(-1,1);       // cos(theta)
  theta    = acos(cosTheta);
  phi      = fRngUniform->value(0,2.*M_PI);  // phi
  //fThetaMap[0] = theta;
  //fPhiMap[0]   = phi;  
  fThetaMap3[0] = theta;
  fPhiMap3[0]   = phi;

  tl1.SetTheta(theta); // Set fragment angle
  tl1.SetPhi(phi); // Set fragment phi angle
  tl4.SetTheta(M_PI - theta); // Set di-neutron angle 180-theta
  tl4.SetPhi(phi + M_PI); // set di-neutron phi angle

  TVector3 t6Boost;
  t6Boost = tl4.BoostVector();  // boost vector for di-neutron

  // start of the di-neutron decay
  dineutron.setA(dineutron.getA()-1);
  dineutron.setm(939.565346);
  dineutron.setEkin(0);               


  // create 4th neutron
  StParticle p4;   
  p4.setA(1);
  p4.setm(p4.getA()*939.565346);//938.565346
  p4.setZ(0);
  p4.setQ(0);
  p4.setx(p4.getx());     // same position
  p4.sety(p4.gety()); 
  p4.setz(p4.getz());     
  p4.sett(p4.gett());     // same time
  p4.setEkin(0);
  
  TLorentzVector tl5;  // neutron from dineutron
  tl5.SetPxPyPzE(p4.getPx(), p4.getPy(), p4.getPz(), p4.getEtotal());

  double eN_1, eN_2;   // total neutron and fragment energy
  double eCM2;       // total CM energy

  eCM2 = p4.getm() + dineutron.getm() + exenDiNeutron;  // total CM energy
  eN_2 = gsl_pow_2(eCM2) + gsl_pow_2(p4.getm()) - gsl_pow_2(dineutron.getm());
  eN_2 = eN_2/(2.*eCM2);                    // total energy neutron
  eN_1 = gsl_pow_2(eCM2) - gsl_pow_2(p4.getm()) + gsl_pow_2(dineutron.getm());
  eN_1 = eN_1/(2.*eCM2);                    // total energy of fragment


  /// \todo check is arg of sqrt is positive
  double pN_2, pN_1; // fragment and neutron momentum
  pN_2 = eN_2*eN_2 - gsl_pow_2(p4.getm()); //cout << "pN_2" << pN_2 << endl;
  pN_2 = sqrt(pN_2); //cout << "sqrt(pN_2)" << sqrt(pN_2) << endl;
  pN_1 = eN_1*eN_1 - gsl_pow_2(dineutron.getm());
  if (pN_1 < 0)
    pN_1 = 0;
  else 
    pN_1 = sqrt(pN_1);
  pN_1 = -pN_1;  // fragment goes in opposite direction

  tl4.SetPxPyPzE(0,0,pN_1,eN_1);
  tl5.SetPxPyPzE(0,0,pN_2,eN_2);
  
  // set theta and phi to some random value
  cosTheta = fRngUniform->value(-1,1);       // cos(theta)
  theta    = acos(cosTheta);
  phi      = fRngUniform->value(0,2.*M_PI);  // phi
  //fThetaMap2[0] = theta;
  //fPhiMap2[0]   = phi;
  fThetaMap4[0] = theta;
  fPhiMap4[0]   = phi;


  tl4.SetTheta(theta);        // Set neutron3 angle
  tl4.SetPhi(phi);            // Set neutron3 phi angle
  tl5.SetTheta(M_PI - theta); // Set neutron4 angle 180-theta
  tl5.SetPhi(phi + M_PI);     // Set neutron4 phi angle

  //------------------------------------BOOSTS---------------------------------
  // Sanity list, thanks to perpetuating this god damn stupid notation
  // 	    	|  StParticle	|	Boost		| Lorentz Vector
  //  FRAGMENT	|	p       	|	t3 (to lab)	|  	tl1
  //	1n  	|	pp      	|	t4 (to 1n)	|	tl2
  // 	2n  	|	ppp     	|	t5 (to 2n)	|	tl3
  //	di-n	|   dineutron   | 	t6 (to di-n)|	tl4
  //    4n	    |  	p4	        |		    	|	tl5

  // This is the correct order of boosts, t6 goes to rest frame of deuteron NOT fragment (unlike sequential)
  // E and P are conserved when exen12_left->0, glaub =0, 1 amu = 1 neutron mass

  //Boosting into the frame where the fragment is at rest after the first 1n decay
  tl1.Boost(t5Boost); tl1.Boost(t4Boost); 
  tl3.Boost(t4Boost);//Must boost each neutron back to the frame where fragment is at rest after 1st decay
  //tl4.Boost(t6Boost); 
  tl4.Boost(t6Boost); tl4.Boost(t5Boost); tl4.Boost(t4Boost); // Here we need 2 boosts. 
  tl5.Boost(t6Boost); tl5.Boost(t5Boost); tl5.Boost(t4Boost);

 
  //Boosting all 5 particles to the lab frame
  tl1.Boost(t3Boost);
  tl2.Boost(t3Boost);
  tl3.Boost(t3Boost);
  tl4.Boost(t3Boost);
  tl5.Boost(t3Boost);


 /* cout << "================ AFTER DINEUTRON EVAPORTATION =======================" << endl;
  printf("Fragment:  %f | %f | %f | %f  | %i \n" ,tl1.Px() ,tl1.Py() ,tl1.Pz() ,tl1.E(), p->getA() );
  printf("1n:        %f | %f | %f | %f  | %i \n" ,tl2.Px() ,tl2.Py() ,tl2.Pz() ,tl2.E(), pp.getA() );
  printf("2n:        %f | %f | %f | %f  | %i \n" ,tl3.Px() ,tl3.Py() ,tl3.Pz() ,tl3.E(), ppp.getA() );
  printf("Dineutron  %f | %f | %f | %f  | %i \n" ,tl4.Px() ,tl4.Py() ,tl4.Pz() ,tl4.E(), dineutron.getA() );
  printf("4n:        %f | %f | %f | %f  | %i \n" ,tl5.Px() ,tl5.Py() ,tl5.Pz() ,tl5.E(), p4.getA() );

  printf("Total:     %f | %f | %f | %f  \n" ,tl1.Px() +tl2.Px() + tl3.Px() + tl4.Px() + tl5.Px(), tl1.Py() + tl2.Py() + tl3.Py() + tl4.Py() + tl5.Py() , tl1.Pz() + tl2.Pz() + tl3.Pz() + tl4.Pz() + tl5.Pz()  ,tl1.E() + tl2.E() + tl3.E() + tl4.E() + tl5.E() );*/

  //fprintf(pFile2," %f \t %f \t %f \t %f \n", tl1.Px() +tl2.Px() + tl3.Px() + tl4.Px() + tl5.Px(), tl1.Py() + tl2.Py() + tl3.Py() + tl4.Py() + tl5.Py() , tl1.Pz() + tl2.Pz() + tl3.Pz() + tl4.Pz() + tl5.Pz()  ,tl1.E() + tl2.E() + tl3.E() + tl4.E() + tl5.E() );


  // set fragment energy and direction
  p->setEtotal(tl1.E()); /// \todo implement setLVec() to set particle data from 4-vector
  p->setThetaPhi(tl1.Theta(),tl1.Phi()); 

  // set 1n energy and direction
  pp.setEtotal(tl2.E());
  pp.setThetaPhi(tl2.Theta(),tl2.Phi());
  if(! newPart) ERR("Pointer newPart not set; Cannot add new particles");
  newPart->push_back(pp); 


  // set 2n energy and direction
  ppp.setEtotal(tl3.E());
  ppp.setThetaPhi(tl3.Theta(),tl3.Phi());
  if(! newPart) ERR("Pointer newPart not set; Cannot add new particles");
  newPart->push_back(ppp); 

   
  // set neutron energy and direction
  dineutron.setEtotal(tl4.E());
  dineutron.setThetaPhi(tl4.Theta(),tl4.Phi()); 

  if(! newPart) ERR("Pointer newPart not set; Cannot add new particles");
  newPart->push_back(dineutron); 

  // set second neutron energy and direction
  p4.setEtotal(tl5.E());
  p4.setThetaPhi(tl5.Theta(),tl5.Phi());

  if(! newPart) ERR("Pointer newPart not set; Cannot add new particles");
  newPart->push_back(p4); 

  }
  //-------------------------------------------------------------------------------------------------------
  // End di neutron decay

  else{
      //cout << "3 body!" << endl;


  //-------------------------------------------------------------------------------------------------------
  // Begin 3body phase space decay
  // Now boost everything back to the lab frame, since TGenPhase space takes arguments in the lab frame
  
  //Boosting into the frame where the fragment is at rest after the first decay
  tl1.Boost(t5Boost); tl1.Boost(t4Boost); 
  tl3.Boost(t4Boost);//Must boost each neutron back to the frame where fragment is at rest after 1st decay
  //Boosting all three particles to the lab frame
  tl1.Boost(t3Boost);
  tl2.Boost(t3Boost);
  tl3.Boost(t3Boost);


  //printf("AFTER 2n SEQUENTIAL: ------------------------------\n");
  //printf("FRAGMENT: %f | %f | %f | %f \n", tl1.Px(), tl1.Py(),  tl1.Pz(), tl1.E() );
  //printf("NEUTRON1: %f | %f | %f | %f \n", tl2.Px(), tl2.Py(),  tl2.Pz(), tl2.E());
  //printf("NEUTRON2: %f | %f | %f | %f \n", tl3.Px(), tl3.Py(),  tl3.Pz(), tl3.E());
  //printf("TOTAL:    %f | %f | %f | %f \n", tl1.Px() + tl2.Px() + tl3.Px(), tl1.Py() + tl2.Py() + tl3.Py(), tl1.Pz() + tl2.Pz() + tl3.Pz(), tl1.E() + tl2.E() + tl3.E());


  // set fragment energy and direction
  p->setEtotal(tl1.E()); /// \todo implement setLVec() to set particle data from 4-vector
  p->setThetaPhi(tl1.Theta(),tl1.Phi()); 
  
  // set neutron energy and direction
  pp.setEtotal(tl2.E());
  pp.setThetaPhi(tl2.Theta(),tl2.Phi()); 

  if(! newPart) ERR("Pointer newPart not set; Cannot add new particles");
  newPart->push_back(pp); 

  // set second neutron energy and direction
  ppp.setEtotal(tl3.E());
  ppp.setThetaPhi(tl3.Theta(),tl3.Phi());
  if(! newPart) ERR("Pointer newPart not set; Cannot add new particles");
  newPart->push_back(ppp); 

  vP = tl1.Vect();// CHECK THIS tl1 is the lorentz vector of the fragment
  
  //Now have the excited fragment in TLorentzVector tl1
  //Use 3-body decay from ROOT to decay Frag* -> frag + n +n
  //Units are GeV and GeV/c!!
  
  //set lorentz vector of decaying system (GeV unit)
  //need to set_mass to the same as the frag+2n since we don't know the mass excess of the 
  //excited fragment, otherwise can get E of excited frag < then E of the frag-n-n system
  //and therefore the 3-body decay is forbidden.

  double set_mass = (p->getA()-2)*931.494 + 1.0*931.494 + 1.0*931.494;
  TLorentzVector t1gev;
  t1gev.SetVectM(vP*(1/1000.), (set_mass+exen3)/1000.);// Set decay energy here
   
  //set the masses of the 3 products.
  Double_t Mass3body[3] = { (p->getA()-2)*931.494/1000.,  1.0*931.494/1000., 1.0*931.494/1000.};
  TGenPhaseSpace tPSevent;
  Bool_t bevent = tPSevent.SetDecay(t1gev, 3, Mass3body);
  if(!bevent) cerr << "5-Body Decay is Kinematically FORBIDDEN !!!!!" << endl;

  TLorentzVector *pt1, *pt4, *pt5;

  double wtmax = tPSevent.GetWtMax();
  while(1){    
    double weight = tPSevent.Generate();
    double rel_weight = weight / wtmax;
    double ran = fRngUniform->value(0,1);

    //events are weighted (rel_weight) between 0 and 1
    //therefore only select event when ran is less then rel_weight
    //if(ran < rel_weight) break;
    break; 
  }
  pt1 = tPSevent.GetDecay(0); //frag - 2n
  pt4 = tPSevent.GetDecay(1); //neutron3
  pt5 = tPSevent.GetDecay(2); //neutron4

  //The returned Lorentz vectors are automatically boosted into the frame
  //of the originally decaying fragment
  //NEED TO CONVERT BACK FROM GEV TO MEV

  //Set fragment info
  p->setEkin(0);
  p->setA(p->getA()-2);
  p->setm(pt1->M()*1000);
 // set fragment energy and direction
  p->setEtotal(pt1->E()*1000); /// \todo implement setLVec() to set particle data from 4-vector
  p->setThetaPhi(pt1->Theta(),pt1->Phi()); 
  //fThetaMap[0]  = pt1->Theta(); // These values are set in the stripping of the first 2 neutrons
  //fPhiMap[0]    = pt1->Phi();	  // "
  //fThetaMap2[0] = pt1->Theta(); // "
  //fPhiMap2[0]   = pt1->Phi();   // "
  fThetaMap3[0] = pt1->Theta();
  fPhiMap3[0]   = pt1->Phi();
  fThetaMap4[0] = pt1->Theta();
  fPhiMap4[0]   = pt1->Phi();

 // set third neutron energy and direction
  StParticle p4;   
  p4.setA(1);
  p4.setm(pt4->M()*1000);
  p4.setZ(0);
  p4.setQ(0);
  p4.setx(p->getx());     // same position
  p4.sety(p->gety()); 
  p4.setz(p->getz());     
  p4.sett(p->gett());     // same time
  p4.setEkin(0);
  p4.setEtotal(pt4->E()*1000);
  p4.setThetaPhi(pt4->Theta(),pt4->Phi());

  if(! newPart) ERR("Pointer newPart not set; Cannot add new particles");
  newPart->push_back(p4); 

 // set fourth neutron energy and direction
  StParticle p5;   
  p5.setA(1);
  p5.setm(pt5->M()*1000);
  p5.setZ(0);
  p5.setQ(0);
  p5.setx(p->getx());     // same position
  p5.sety(p->gety()); 
  p5.setz(p->getz());     
  p5.sett(p->gett());     // same time
  p5.setEkin(0);
  p5.setEtotal(pt5->E()*1000);
  p5.setThetaPhi(pt5->Theta(),pt5->Phi());


  if(! newPart) ERR("Pointer newPart not set; Cannot add new particles");
  newPart->push_back(p5); 

  }
  //-------------------------------------------------------------------------------------------------------
  // End 3body phase space decay


  // Check E and P conservation
  //printf("AFTER 3 BODY DECAY ----------------------------------------\n");
  //printf("FRAGMENT: %f | %f | %f | %f \n", pt1->Px()*1000 , pt1->Py()*1000, pt1->Pz()*1000, pt1->E()*1000 );
  //printf("NEUTRON1: %f | %f | %f | %f \n", tl2.Px(), tl2.Py(),  tl2.Pz(), tl2.E());
  //printf("NEUTRON2: %f | %f | %f | %f \n", tl3.Px(), tl3.Py(),  tl3.Pz(), tl3.E());
  //printf("NEUTRON3: %f | %f | %f | %f \n", pt4->Px()*1000, pt4->Py()*1000, pt4->Pz()*1000, pt4->E()*1000);
  //printf("NEUTRON4: %f | %f | %f | %f \n", pt5->Px()*1000, pt5->Py()*1000, pt5->Pz()*1000, pt5->E()*1000);
  //printf("TOTAL:    %f | %f | %f | %f \n", pt1->Px()*1000 + tl2.Px() +tl3.Px() + pt4->Px()*1000 + pt5->Px()*1000, pt1->Py()*1000 + tl2.Py() + tl3.Py() + pt4->Py()*1000 + pt5->Py()*1000, pt1->Pz()*1000 + tl2.Pz() + tl3.Pz() + pt4->Pz()*1000 + pt5->Pz()*1000, pt1->E()*1000 + tl2.E() + tl3.E() + pt4->E()*1000 + pt5->E()*1000 );




  DSV(p->getA());
  DSV(p->getZ());
  return 0;

}

int StReacSequential_5bodydecay::savedValue(size_t part_id, string name, double* v) {
  DSV(part_id);
  DSV(name);

  if (name == "exen" &&  (fExEnMap.find(part_id) != fExEnMap.end()) ) {
    *v = fExEnMap[part_id];
    DSV(*v);
    return 0;
  }
  if (name == "exen2" &&  (fExEnMap2.find(part_id) != fExEnMap2.end()) ) {
    *v = fExEnMap2[part_id];
    DSV(*v);
    return 0;
  }
  if (name == "exen3" &&  (fExEnMap3.find(part_id) != fExEnMap3.end()) ) {
    *v = fExEnMap3[part_id];
    DSV(*v);
    return 0;
  }
  if (name == "exen4" &&  (fExEnMap4.find(part_id) != fExEnMap4.end()) ) {
    *v = fExEnMap4[part_id];
    DSV(*v);
    return 0;
  }
  if (name == "ph" &&  (fPhiMap.find(part_id) != fPhiMap.end()) ) {
    *v = fPhiMap[part_id];
    DSV(*v);
    return 0;
  }
  if (name == "ph2" &&  (fPhiMap2.find(part_id) != fPhiMap2.end()) ) {
    *v = fPhiMap2[part_id];
    DSV(*v);
    return 0;
  }
  if (name == "ph3" &&  (fPhiMap3.find(part_id) != fPhiMap3.end()) ) {
    *v = fPhiMap3[part_id];
    DSV(*v);
    return 0;
  }
  if (name == "ph4" &&  (fPhiMap4.find(part_id) != fPhiMap4.end()) ) {
    *v = fPhiMap4[part_id];
    DSV(*v);
    return 0;
  }
  if (name == "th" &&  (fThetaMap.find(part_id) != fThetaMap.end()) ) {
    *v = fThetaMap[part_id];
    DSV(*v);
    return 0;
  }
  if (name == "th2" &&  (fThetaMap2.find(part_id) != fThetaMap2.end()) ) {
    *v = fThetaMap2[part_id];
    DSV(*v);
    return 0;
  }
  if (name == "th3" &&  (fThetaMap3.find(part_id) != fThetaMap3.end()) ) {
    *v = fThetaMap3[part_id];
    DSV(*v);
    return 0;
  }
  if (name == "th4" &&  (fThetaMap4.find(part_id) != fThetaMap4.end()) ) {
    *v = fThetaMap4[part_id];
    DSV(*v);
    return 0;
  }


  if (name == "pz" &&  (fPzMap.find(part_id) != fPzMap.end()) ) {
    *v = fPzMap[part_id];
    DSV(*v);
    return 0;
  }





  return 1;
}
////////////////////////////5body Sequential///////////////////////////////

//---------------------3-body decay leading to dineutron/sequential-------------------------//
int StReacStripEvap_3bodyDineutron::act(StParticle* p, vector<StParticle>* newPart) {

  //Setting up the two decay energies
  double exen     = 0.1; //Total Decay Energy
  double exen1    = 0.1; //1st Decay Energy
  double exen2    = 0.1; //2nd Decay Energy
  double exen3    = 0.1;
  double exen4    = 0.1;
  double exenTotal = -1;
  double exenTotal_Dineutron = 0;
  double exenDiNeutron = 0.1;
  double delta_E = 0;
  //Create Varibles to give give directions to particles and Glauber Kick
  double cosTheta;     //Used to set the cos(Theta)
  double theta;        //Used to set the Theta
  double theta3;
  double phi;          //Used to set the Phi

  //for volya 2n
  double erel = 0; //rel energy between 2 neutrons
  double di_ei=0, di_ek=0;

  //Check that all necassery random number generators are found
  if (!fRngExEn)           ERR("Need fRngExEn");
  if (!fRngExEn1)          ERR("Need fRngExEn1");
  if (!fRngExEn2)          ERR("Need fRngExEn2");
  if (!fRngExEn3)	       ERR("Need fRngExEn3");
  if (!fRngExEn4) 	       ERR("Need fRngExEn4");
  if (!fRngUniform)        ERR("Need uniform random number generator");
  if (!fRngGaussGlauber)   ERR("Need gauss random number generator");

  //Removed the fWhichEnergyDoIReconstruct (since it isn't needed for 3body decay)

  // Total 5body energy
  do{
      exenTotal = fRngExEn->value();
  }
  while(exenTotal < 0);


    if(fWhichEnergyDoIReconstruct=="volya_dineutron"){ //cout << "Coreectly entered dinuetron" << endl;

    //do{
        //for volya + bw
        do{
    		fRngExEn3->value2d(di_ei,di_ek); 
    
   		    exenTotal_Dineutron = di_ei + di_ek;
    		exenDiNeutron = di_ei;
        }
        while(exenTotal < exenTotal_Dineutron);
        //while(exenTotal_Dineutron < 0);

	//cout << "exenTotal: " << exenTotal << " exenDiNeutron :" << di_ei << endl;
    //}
    //while(exenTotal - exenTotal_Dineutron < 0);
    delta_E = exenTotal - exenTotal_Dineutron;

    //if(delta_E < 0) delta_E = exenTotal_Dineutron - exenTotal;

	do{
		exen1 = fRngExEn1->value();// cout << "exen1: " << exen1 << endl;
		//exen1=0.01;
  	} 
	 while (exen1 < 0);
	 do{
		exen2 = fRngExEn2->value(); //cout << "exen2: " << exen2 << endl;
		//exen2 = 10;
  	 }  
	 while (exen2 < 0);

        DSV(exenTotal);
        DSV(exen2);
        fExEnMap[0]=exenTotal; //cout << "ETOTAL: " << exenTotal << endl;
        fExEnMap2[0]=delta_E;  
        fExEnMap3[0]=exenTotal_Dineutron;
        fExEnMap4[0]=exenDiNeutron;

   } // end volya dineutron

    else{// Do a sequential decay

        do{
            fRngExEn3->value2d(exen,erel);
        }
        while(exenTotal < exen);

            delta_E = exenTotal - exen;

            exen3 = 0.5* (exen + erel);
            exen4 = 0.5* (exen - erel);

        fExEnMap[0]=exenTotal; //cout << "ETOTAL: " << exenTotal << endl;
        fExEnMap2[0]=delta_E;  
        fExEnMap3[0]=exen3;
        fExEnMap4[0]=exen4;


    }
  
  //debugging stuff -> Decay Energies
  if (debugSwitch == "exen1")
	  cout << exenTotal << endl;
  if (debugSwitch == "exen2")
	  cout << delta_E << endl;
  if (debugSwitch == "exen3")
	  cout << exen3 << endl;
  if (debugSwitch == "exen4")
	  cout << exen4 << endl;


  if (!p) ERR("Particle pointer not set.");


  // change particle, keep velocity the same
  //**zwk reduce beta after reaction following LISE++
  double beta = p->getBeta() * fVelocityScale;        // initial beta
  double A = p->getA();            // initial mass number
  //cout << "Mass: " << p->getA() << " | " << fNn << " | " << fNp << endl;
  p->setA(p->getA() - fNn - fNp);  // remove protons and neutrons
  p->setm(p->getA()*931.494028);        // set new mass
  p->setZ(p->getZ() - fNp);        // set new Z
  p->setQ(p->getZ());              // set new Q
  p->setBeta(beta);                // set velocity

  //cout << "Incoming Etotal: " << p->getEtotal() << endl;
  //cout << "EDecay: " << exenTotal << endl;

  TLorentzVector tl;
  tl.SetPxPyPzE(p->getPx(), p->getPy(), p->getPz(), p->getEtotal());
  DSV(tl.Theta());
  DSV(tl.Phi());
  DSV(tl.E() - tl.M());
  //printf("Pre Glauber (Frag): \t %f \t %f \t %f \t %f \t %i \n" ,tl.Px() ,tl.Py() ,tl.Pz() ,tl.E(), p->getA() );


  fPzMap[0]=-1;
  // Glauber momentum kick 
  double sig, sig0;
  sig0 = 90;  // MeV/c
  sig = p->getA() * ( fNn + fNp ) / ( A - 1);
  if (sig < 0) ERR("sig < 0; sig = %g", sig);
  sig = fGlauberKickScale * sig0 * sqrt(sig);
  DSV(sig);
  
  cosTheta = fRngUniform->value(-1,1);// cos(theta)
  theta    = acos(cosTheta);
  phi      = fRngUniform->value(0,2.*M_PI);  // phi
  DSV(theta);
  DSV(phi);

  fRngGaussGlauber->setCentroid(0);
  fRngGaussGlauber->setSigma(sig);

  double glaubP = fRngGaussGlauber->value();
  //-=-=-==--=-=-=-==
  //idea to create tail in mom. distribution following LISE++, zwk
  if(glaubP < 0) glaubP = glaubP * fAsymMomentum;
  //--------------------

  DSV(glaubP);
  TVector3 vGlaubP(0,0,glaubP);
  vGlaubP.SetTheta(theta);
  vGlaubP.SetPhi(phi);
  
  TVector3 vP;                  // total momentum
  vP = tl.Vect() + vGlaubP;
  tl.SetVectM(vP,p->getm());
  DSV(tl.Theta());
  DSV(tl.Phi());
  DSV(tl.E() - tl.M());
  
  //debugging stuff
  if (debugSwitch == "thetaLABP")
    cout << tl.Theta()<< endl;
  if (debugSwitch == "phiLABP")
    cout << tl.Phi()<< endl;
  
  // printf("Post Glauber (Frag): \t %f \t %f \t %f \t %f \t %i \n" ,tl.Px() ,tl.Py() ,tl.Pz() ,tl.E(), p->getA() );

  //______________________________________________________________________________
  //Now have the excited fragment in TLorentzVector tl
  //Use 3-body decay from ROOT to decay Frag* -> frag + n +n
  //Units are GeV and GeV/c!!
  
  //set lorentz vector of decaying system (GeV unit)
  //need to set_mass to the same as the frag+2n since we don't know the mass excess of the 
  //excited fragment, otherwise can get E of excited frag < then E of the frag-n-n system
  //and therefore the 3-body decay is forbidden.
  //double set_mass = (p->getA()-4)*931.494028 +  2*(939.565346 + 939.565346);
  //cout << "Total mass: " << set_mass + exenTotal << endl;
  //cout << "exenTotal: " << exenTotal << endl;
  //exenTotal = 0.1;
  // mass of 2n + frag
  
  double set_mass = (p->getA()-2)*931.494028 + 1.0*931.494028 + 1.0*931.494028;
  TLorentzVector t1gev;
  t1gev.SetVectM(vP*(1/1000.), (set_mass+delta_E)/1000.);
  //t1gev.SetVectM(vP*(1/1000.), (set_mass)/1000.);

  //cout << vP.x() << " " << vP.y() << " " << vP.z() << endl;
  //cout << p->getm() << " " << p->getA() << endl;
  //cout << exen1 << " " << exen2 << endl;
  //cout << t1gev.M() << "  " << t1gev.Mag() << endl;
  //cout << ( (p->getA()-2)*931.494028/1000. +  939.565346/1000. + 939.565346/1000.) << endl;
  
  //set the masses of the 5 products.
  Double_t Mass3body[3] = { (p->getA()-2)*931.494028/1000.,  1.0*931.494028/1000., 1.0*931.494028/1000.};
  TGenPhaseSpace tPSevent;
  Bool_t bevent = tPSevent.SetDecay(t1gev, 3, Mass3body);
  if(!bevent) cerr << "3-Body Decay is Kinematically FORBIDDEN !!!!!" << endl;

  TLorentzVector *pt1, *pt2, *pt3;

  double wtmax = tPSevent.GetWtMax();
  while(1){    
    double weight = tPSevent.Generate();
    double rel_weight = weight / wtmax;
    double ran = fRngUniform->value(0,1);

    //events are weighted (rel_weight) between 0 and 1
    //therefore only select event when ran is less then rel_weight
    //if(ran < rel_weight) break;
    break; 
  }
  pt1 = tPSevent.GetDecay(0); //frag - 2n
  pt2 = tPSevent.GetDecay(1); //neutron1
  pt3 = tPSevent.GetDecay(2); //neutron2


  //The returned Lorentz vectors are automatically boosted into the frame
  // of the originally decaying fragment (Lab frame)
  // 
  //NEED TO CONVERT BACK FROM GEV TO MEV

  //Set fragment info
  p->setEkin(0);
  p->setA(p->getA()-2);
  p->setm(pt1->M()*1000);
  // set fragment energy and direction
  p->setEtotal(pt1->E()*1000); /// \todo implement setLVec() to set particle data from 4-vector
  p->setThetaPhi(pt1->Theta(),pt1->Phi()); 
  
  // set fragment lorentz vector after 3body decay
  //
  TLorentzVector tl1;  // 
  tl1.SetPxPyPzE(p->getPx(), p->getPy(), p->getPz(), p->getEtotal()); // back to MeV


  TVector3 tFragBoost;
  tFragBoost = tl1.BoostVector();  // boost vector for fragment after 3-body breakup (lab)


  
  
  
  
  fThetaMap[0]  = pt1->Theta();
  fPhiMap[0]    = pt1->Phi();
  fThetaMap2[0] = pt1->Theta();
  fPhiMap2[0]   = pt1->Phi();
  fThetaMap3[0] = pt1->Theta();
  fPhiMap3[0]   = pt1->Phi();
  fThetaMap4[0] = pt1->Theta();
  fPhiMap4[0]   = pt1->Phi();

  
  // set neutron energy and direction
  StParticle pp;
  pp.setZ(0);
  pp.setQ(0);
  pp.setx(p->getx());     // same position
  pp.sety(p->gety()); 
  pp.setz(p->getz());     
  pp.sett(p->gett());     // same time
  pp.setEkin(0);
  pp.setA(1);
  pp.setm(pt2->M()*1000);
  pp.setEtotal(pt2->E()*1000);
  pp.setThetaPhi(pt2->Theta(),pt2->Phi()); 

  if(! newPart) ERR("Pointer newPart not set; Cannot add new particles");
  newPart->push_back(pp); 

  // set second neutron energy and direction
  StParticle ppp;   
  ppp.setA(1);
  ppp.setm(pt3->M()*1000);
  ppp.setZ(0);
  ppp.setQ(0);
  ppp.setx(p->getx());     // same position
  ppp.sety(p->gety()); 
  ppp.setz(p->getz());     
  ppp.sett(p->gett());     // same time
  ppp.setEkin(0);
  ppp.setEtotal(pt3->E()*1000);
  ppp.setThetaPhi(pt3->Theta(),pt3->Phi());

  if(! newPart) ERR("Pointer newPart not set; Cannot add new particles");
  newPart->push_back(ppp); 

  //cout << "1n mass: " << pp.getm() << endl;

  //printf("Post 3body (Frag): \t %f \t %f \t %f \t %f \t %i \n" ,tl1.Px() ,tl1.Py() ,tl1.Pz() ,tl1.E(), p->getA() );
 // printf("Post 3body (1n)  : \t %f \t %f \t %f \t %f \t %i \n" ,1000*pt2->Px() ,1000*pt2->Py() ,1000*pt2->Pz() ,1000*pt2->E(), pp.getA() );
 // printf("Post 3body (2n)  : \t %f \t %f \t %f \t %f \t %i \n" ,1000*pt3->Px() ,1000*pt3->Py() ,1000*pt3->Pz() ,1000*pt3->E(), ppp.getA() );
 // printf("SUM              : \t %f \t %f \t %f \t %f \t %i \n" ,tl1.Px() + 1000*(pt3->Px() + pt2->Px()) ,tl1.Py() + 1000*(pt2->Py() + pt3->Py()) ,tl1.Pz() + 1000*(pt2->Pz() + pt3->Pz()),tl1.E() + 1000*(pt3->E() + pt2->E()), p->getA() + pp.getA() + ppp.getA() );



  //_____________________________________END 3BODY DECAY___________________________________
  //
  //
  // Begin Dineutron Decay, THis is done in the rest frame of the compound nucleus
 if (fWhichEnergyDoIReconstruct == "volya_dineutron"){
  // start of di-neutron evaporation
    p->setA(p->getA() - 2);
    p->setm(p->getA()*931.494028);
    p->setEkin(0);             

   // create di-neutron
   StParticle dineutron;   
   dineutron.setA(2);
   dineutron.setm(dineutron.getA()*939.565346);//939.565346
   dineutron.setZ(0);
   dineutron.setQ(0);
   dineutron.setx(p->getx());     // same position
   dineutron.sety(p->gety()); 
   dineutron.setz(p->getz());     
   dineutron.sett(p->gett());     // same time
   dineutron.setEkin(0);
  
  TLorentzVector tl4;  // di-neutron
  tl4.SetPxPyPzE(dineutron.getPx(), dineutron.getPy(), dineutron.getPz(), dineutron.getEtotal()); // Rest frame of dineutron

 
  
  double e2N_d, eF_d;   // total neutron and fragment energy
  double eCM_d;   // total CM energy

  //Make sure don't have negative decay energies.
  //Need to develop how we choose dineutron and frag-2n energies.
  double exen12_left = exenTotal_Dineutron - exenDiNeutron;
  //double exen12_left = 0.1;
  if(exen12_left <= 0){
    cerr << "ExEn Left is less than ZERO!!" << endl;
  }
  
  eCM_d = dineutron.getm() + p->getm() + (exen12_left);  // total CM energy 
  e2N_d = gsl_pow_2(eCM_d) + gsl_pow_2(dineutron.getm()) - gsl_pow_2(p->getm());
  e2N_d = e2N_d/(2.*eCM_d);                    // total energy neutron
  eF_d = gsl_pow_2(eCM_d) - gsl_pow_2(dineutron.getm()) + gsl_pow_2(p->getm());
  eF_d = eF_d/(2.*eCM_d);                    // total energy of fragment

  /// \todo check is arg of sqrt is positive
  double p2N_d,pF_d; // fragment and neutron momentum
  p2N_d = e2N_d*e2N_d - gsl_pow_2(dineutron.getm());
  p2N_d = sqrt(p2N_d);
  pF_d = eF_d*eF_d - gsl_pow_2(p->getm());
  if (pF_d < 0)
    pF_d = 0;
  else 
    pF_d = sqrt(pF_d);
  
  pF_d = -pF_d;  // fragment goes in opposite direction

  tl1.SetPxPyPzE(0,0,pF_d,eF_d); //cout << "pF_d " << pF_d << endl;
  tl4.SetPxPyPzE(0,0,p2N_d,e2N_d);
  
  // set theta and phi for the first decay to some random value
  cosTheta = fRngUniform->value(-1,1);       // cos(theta)
  theta    = acos(cosTheta);
  phi      = fRngUniform->value(0,2.*M_PI);  // phi
  //fThetaMap[0] = theta;
  //fPhiMap[0]   = phi;  
  fThetaMap3[0] = theta;
  fPhiMap3[0]   = phi;

  tl1.SetTheta(theta); // Set fragment angle
  tl1.SetPhi(phi); // Set fragment phi angle
  tl4.SetTheta(M_PI - theta); // Set di-neutron angle 180-theta
  tl4.SetPhi(phi + M_PI); // set di-neutron phi angle

  TVector3 t6Boost;
  t6Boost = tl4.BoostVector();  // boost vector for di-neutron

  // start of the di-neutron decay
  dineutron.setA(dineutron.getA()-1);
  dineutron.setm(939.565346);
  dineutron.setEkin(0);               


  // create 4th neutron
  StParticle p4;   
  p4.setA(1);
  p4.setm(p4.getA()*939.565346);//938.565346
  p4.setZ(0);
  p4.setQ(0);
  p4.setx(p4.getx());     // same position
  p4.sety(p4.gety()); 
  p4.setz(p4.getz());     
  p4.sett(p4.gett());     // same time
  p4.setEkin(0);
  
  TLorentzVector tl5;  // neutron from dineutron
  tl5.SetPxPyPzE(p4.getPx(), p4.getPy(), p4.getPz(), p4.getEtotal());

  double eN_1, eN_2;   // total neutron and fragment energy
  double eCM2;       // total CM energy

  eCM2 = p4.getm() + dineutron.getm() + exenDiNeutron;  // total CM energy
  eN_2 = gsl_pow_2(eCM2) + gsl_pow_2(p4.getm()) - gsl_pow_2(dineutron.getm());
  eN_2 = eN_2/(2.*eCM2);                    // total energy neutron
  eN_1 = gsl_pow_2(eCM2) - gsl_pow_2(p4.getm()) + gsl_pow_2(dineutron.getm());
  eN_1 = eN_1/(2.*eCM2);                    // total energy of fragment


  /// \todo check is arg of sqrt is positive
  double pN_2, pN_1; // fragment and neutron momentum
  pN_2 = eN_2*eN_2 - gsl_pow_2(p4.getm()); //cout << "pN_2" << pN_2 << endl;
  pN_2 = sqrt(pN_2); //cout << "sqrt(pN_2)" << sqrt(pN_2) << endl;
  pN_1 = eN_1*eN_1 - gsl_pow_2(dineutron.getm());
  if (pN_1 < 0)
    pN_1 = 0;
  else 
    pN_1 = sqrt(pN_1);
  pN_1 = -pN_1;  // fragment goes in opposite direction

  tl4.SetPxPyPzE(0,0,pN_1,eN_1);
  tl5.SetPxPyPzE(0,0,pN_2,eN_2);
  
  // set theta and phi to some random value
  cosTheta = fRngUniform->value(-1,1);       // cos(theta)
  theta    = acos(cosTheta);
  phi      = fRngUniform->value(0,2.*M_PI);  // phi
  //fThetaMap2[0] = theta;
  //fPhiMap2[0]   = phi;
  fThetaMap4[0] = theta;
  fPhiMap4[0]   = phi;


  tl4.SetTheta(theta);        // Set neutron3 angle
  tl4.SetPhi(phi);            // Set neutron3 phi angle
  tl5.SetTheta(M_PI - theta); // Set neutron4 angle 180-theta
  tl5.SetPhi(phi + M_PI);     // Set neutron4 phi angle

 //////////////////////////////////
 // boost back to lab
 // n1 and n2 are already in lab frame
 // fFragBoost = fragment frame before dineutron decay
 // t6Boost = rest frame of frag + dineutron
 // 
 //
 
  tl1.Boost(tFragBoost);
  tl4.Boost(t6Boost); tl4.Boost(tFragBoost); 
  tl5.Boost(t6Boost); tl5.Boost(tFragBoost);

  
  //printf("Post dineutron (Frag): \t %f \t %f \t %f \t %f \t %i \n" ,tl1.Px() ,tl1.Py() ,tl1.Pz() ,tl1.E(), p->getA() );
  //printf("Post dineutron (3n)  : \t %f \t %f \t %f \t %f \t %i \n" ,tl4.Px() ,tl4.Py() ,tl4.Pz() ,tl4.E(), dineutron.getA() );
  //printf("Post dineutron (4n)  : \t %f \t %f \t %f \t %f \t %i \n" ,tl5.Px() ,tl5.Py() ,tl5.Pz() ,tl5.E(), p4.getA() );
  //printf("SUB SUM   2n+8He     : \t %f \t %f \t %f \t %f \t %i \n" ,tl1.Px() + tl4.Px() + tl5.Px(),tl1.Py() + tl4.Py() + tl5.Py(), tl1.Pz() + tl4.Pz() + tl5.Pz(),tl1.E() + tl4.E() + tl5.E(), p->getA() + dineutron.getA() + p4.getA() );
  //printf("FULL SUM  4n+8He     : \t %f \t %f \t %f \t %f \t %i \n" ,tl1.Px() + tl4.Px() + tl5.Px() + 1000*(pt2->Px() + pt3->Px()) ,tl1.Py() + tl4.Py() + tl5.Py() + 1000*(pt2->Py() + pt3->Py()), tl1.Pz() + tl4.Pz() + tl5.Pz() + 1000*(pt2->Pz() + pt3->Pz()),tl1.E() + tl4.E() + tl5.E() + 1000*(pt2->E() + pt3->E()), p->getA() + dineutron.getA() + p4.getA() + pp.getA() + ppp.getA() );

  
  // set fragment energy and direction
  p->setEtotal(tl1.E()); /// \todo implement setLVec() to set particle data from 4-vector
  p->setThetaPhi(tl1.Theta(),tl1.Phi()); 

  // set energy and direction for frag and remaining neutrons
  dineutron.setEtotal(tl4.E());
  dineutron.setThetaPhi(tl4.Theta(),tl4.Phi()); 

  if(! newPart) ERR("Pointer newPart not set; Cannot add new particles");
  newPart->push_back(dineutron); 

  // set second neutron energy and direction
  p4.setEtotal(tl5.E());
  p4.setThetaPhi(tl5.Theta(),tl5.Phi());

  if(! newPart) ERR("Pointer newPart not set; Cannot add new particles");
  newPart->push_back(p4); 

  


  DSV(p->getA());
  DSV(p->getZ());
  
  return 0; 
 } // End di-neutron

 else{ // strip/evaporate the third and fourth neutron 
 // start of the third neutron evaporation
  p->setA(p->getA()-1);
  p->setm(p->getA()*931.494088);
  p->setEkin(0);               // 23O at rest

// create third neutron
// this notation is stupid@!
  StParticle pppp;   
  pppp.setA(1);
  pppp.setm(pppp.getA()*939.565378);
  pppp.setZ(0);
  pppp.setQ(0);
  pppp.setx(p->getx());     // same position
  pppp.sety(p->gety()); 
  pppp.setz(p->getz());     
  pppp.sett(p->gett());     // same time
  pppp.setEkin(0);
  
  TLorentzVector tl4;  // particle 4 (neutron)
  tl4.SetPxPyPzE(pppp.getPx(), pppp.getPy(), pppp.getPz(), pppp.getEtotal());

  double eN3, eF3;   // total neutron and fragment energy
  double eCM3;       // total CM energy

  eCM3 = pppp.getm() + p->getm() + exen3;  // total CM energy
  eN3 = gsl_pow_2(eCM3) + gsl_pow_2(pppp.getm()) - gsl_pow_2(p->getm());
  eN3 = eN3/(2.*eCM3);                    // total energy neutron
  eF3 = gsl_pow_2(eCM3) - gsl_pow_2(pppp.getm()) + gsl_pow_2(p->getm());
  eF3 = eF3/(2.*eCM3);                    // total energy of fragment

  /// \todo check is arg of sqrt is positive
  double pN3, pF3; // fragment and neutron momentum
  pN3 = eN3*eN3 - gsl_pow_2(pppp.getm());
  pN3 = sqrt(pN3);
  pF3 = eF3*eF3 - gsl_pow_2(p->getm());
  if (pF3 < 0)
    pF3 = 0;
  else 
    pF3 = sqrt(pF3);
  pF3 = -pF3;  // fragment goes in opposit direction

  tl1.SetPxPyPzE(0,0,pF3,eF3);
  tl4.SetPxPyPzE(0,0,pN3,eN3);
  
  // set theta and phi to some random value
  //
  cosTheta = fRngUniform->value(-1,1);       // cos(theta)
  theta3    = acos(cosTheta);
  phi      = fRngUniform->value(0,2.*M_PI);  // phi
  fThetaMap3[0] = theta3;
  fPhiMap3[0]   = phi;

  tl1.SetTheta(theta3);        // Set fragment angle
  tl1.SetPhi(phi);            // Set fragment phi angle
  tl4.SetTheta(M_PI - theta3); // Set neutron angle 180-theta
  tl4.SetPhi(phi + M_PI);     // Set neutron phi angle

  TVector3 t6Boost;
  t6Boost = tl1.BoostVector();  // boost vector for 24O


 // start of the fourth neutron evaporation
  p->setA(p->getA()-1);
  p->setm(p->getA()*931.494088);
  p->setEkin(0);               // 23O at rest


// create fourth neutron
// this notation is stupid@!
  StParticle p5;   
  p5.setA(1);
  p5.setm(p5.getA()*939.565378);
  p5.setZ(0);
  p5.setQ(0);
  p5.setx(p->getx());     // same position
  p5.sety(p->gety()); 
  p5.setz(p->getz());     
  p5.sett(p->gett());     // same time
  p5.setEkin(0);
  
  TLorentzVector tl5;  // particle 4 (neutron)
  tl5.SetPxPyPzE(p5.getPx(), p5.getPy(), p5.getPz(), p5.getEtotal());

  double eN4, eF4;   // total neutron and fragment energy
  double eCM4;       // total CM energy

  eCM4 = p5.getm() + p->getm() + exen4;  // total CM energy
  eN4 = gsl_pow_2(eCM4) + gsl_pow_2(p5.getm()) - gsl_pow_2(p->getm());
  eN4 = eN4/(2.*eCM4);                    // total energy neutron
  eF4 = gsl_pow_2(eCM4) - gsl_pow_2(p5.getm()) + gsl_pow_2(p->getm());
  eF4 = eF4/(2.*eCM4);                    // total energy of fragment

  /// \todo check is arg of sqrt is positive
  double pN4, pF4; // fragment and neutron momentum
  pN4 = eN4*eN4 - gsl_pow_2(p5.getm());
  pN4 = sqrt(pN4);
  pF4 = eF4*eF4 - gsl_pow_2(p->getm());
  if (pF4 < 0)
    pF4 = 0;
  else 
    pF4 = sqrt(pF4);
    pF4 = -pF4;  // fragment goes in opposit direction

  tl1.SetPxPyPzE(0,0,pF4,eF4);
  tl5.SetPxPyPzE(0,0,pN4,eN4);
  
  // set theta and phi to some random value
  //cosTheta = fRngUniform->value(-1,1);       // cos(theta)
  
  
 
   cosTheta = fRngUniform->value(-1,1);
   theta    = acos(cosTheta);
  
       
   phi      = fRngUniform->value(0,2.*M_PI);  // phi
 
  fThetaMap4[0] = theta;
  fPhiMap4[0]   = phi;
  tl1.SetTheta(theta);        // Set fragment angle
  tl1.SetPhi(phi);            // Set fragment phi angle
  tl5.SetTheta(M_PI - theta); // Set neutron angle 180-theta
  tl5.SetPhi(phi + M_PI);     // Set neutron phi angle





  //Boosting into the frame where the fragment is at rest after the first decay
  tl1.Boost(t6Boost);  
  tl5.Boost(t6Boost); 
  //Boosting all four particles to the lab frame
  tl1.Boost(tFragBoost);
  tl4.Boost(tFragBoost);
  tl5.Boost(tFragBoost);

 // printf("Post seq. (Frag): \t %f \t %f \t %f \t %f \t %i \n" ,tl1.Px() ,tl1.Py() ,tl1.Pz() ,tl1.E(), p->getA() );
 // printf("Post seq. (3n)  : \t %f \t %f \t %f \t %f \t %i \n" ,tl4.Px() ,tl4.Py() ,tl4.Pz() ,tl4.E(), pppp.getA() );
 // printf("Post seq (4n)  : \t %f \t %f \t %f \t %f \t %i \n" ,tl5.Px() ,tl5.Py() ,tl5.Pz() ,tl5.E(), p5.getA() );
 // printf("SUB SUM   2n+8He     : \t %f \t %f \t %f \t %f \t %i \n" ,tl1.Px() + tl4.Px() + tl5.Px(),tl1.Py() + tl4.Py() + tl5.Py(), tl1.Pz() + tl4.Pz() + tl5.Pz(),tl1.E() + tl4.E() + tl5.E(), p->getA() + pppp.getA() + p5.getA() );
 // printf("FULL SUM  4n+8He     : \t %f \t %f \t %f \t %f \t %i \n" ,tl1.Px() + tl4.Px() + tl5.Px() + 1000*(pt2->Px() + pt3->Px()) ,tl1.Py() + tl4.Py() + tl5.Py() + 1000*(pt2->Py() + pt3->Py()), tl1.Pz() + tl4.Pz() + tl5.Pz() + 1000*(pt2->Pz() + pt3->Pz()),tl1.E() + tl4.E() + tl5.E() + 1000*(pt2->E() + pt3->E()), p->getA() + pppp.getA() + p5.getA() + pp.getA() + ppp.getA() );


  // set fragment energy and direction
  p->setEtotal(tl1.E()); /// \todo implement setLVec() to set particle data from 4-vector
  p->setThetaPhi(tl1.Theta(),tl1.Phi()); 
  
  // set third neutron energy and direction
  pppp.setEtotal(tl4.E());
  pppp.setThetaPhi(tl4.Theta(),tl4.Phi());

  if(! newPart) ERR("Pointer newPart not set; Cannot add new particles");
  newPart->push_back(pppp); 

  // set fourth neutron energy and direction
  p5.setEtotal(tl5.E());
  p5.setThetaPhi(tl5.Theta(),tl5.Phi());

  if(! newPart) ERR("Pointer newPart not set; Cannot add new particles");
  newPart->push_back(p5); 

  DSV(p->getA());
  DSV(p->getZ());

  
  return 0;



 }

}

int StReacStripEvap_3bodyDineutron::savedValue(size_t part_id, string name, double* v) {
  DSV(part_id);
  DSV(name);

  if (name == "exen" &&  (fExEnMap.find(part_id) != fExEnMap.end()) ) {
    *v = fExEnMap[part_id];
    DSV(*v);
    return 0;
  }
  if (name == "exen2" &&  (fExEnMap2.find(part_id) != fExEnMap2.end()) ) {
    *v = fExEnMap2[part_id];
    DSV(*v);
    return 0;
  }
  if (name == "exen3" &&  (fExEnMap3.find(part_id) != fExEnMap3.end()) ) {
    *v = fExEnMap3[part_id];
    DSV(*v);
    return 0;
  }
  if (name == "exen4" &&  (fExEnMap4.find(part_id) != fExEnMap4.end()) ) {
    *v = fExEnMap4[part_id];
    DSV(*v);
    return 0;
  }
  if (name == "ph" &&  (fPhiMap.find(part_id) != fPhiMap.end()) ) {
    *v = fPhiMap[part_id];
    DSV(*v);
    return 0;
  }
  if (name == "ph2" &&  (fPhiMap2.find(part_id) != fPhiMap2.end()) ) {
    *v = fPhiMap2[part_id];
    DSV(*v);
    return 0;
  }
  if (name == "ph3" &&  (fPhiMap3.find(part_id) != fPhiMap3.end()) ) {
    *v = fPhiMap3[part_id];
    DSV(*v);
    return 0;
  }
  if (name == "ph4" &&  (fPhiMap4.find(part_id) != fPhiMap4.end()) ) {
    *v = fPhiMap4[part_id];
    DSV(*v);
    return 0;
  }
  if (name == "th" &&  (fThetaMap.find(part_id) != fThetaMap.end()) ) {
    *v = fThetaMap[part_id];
    DSV(*v);
    return 0;
  }
  if (name == "th2" &&  (fThetaMap2.find(part_id) != fThetaMap2.end()) ) {
    *v = fThetaMap2[part_id];
    DSV(*v);
    return 0;
  }
  if (name == "th3" &&  (fThetaMap3.find(part_id) != fThetaMap3.end()) ) {
    *v = fThetaMap3[part_id];
    DSV(*v);
    return 0;
  }
  if (name == "th4" &&  (fThetaMap4.find(part_id) != fThetaMap4.end()) ) {
    *v = fThetaMap4[part_id];
    DSV(*v);
    return 0;
  }


  if (name == "pz" &&  (fPzMap.find(part_id) != fPzMap.end()) ) {
    *v = fPzMap[part_id];
    DSV(*v);
    return 0;
  }





  return 1;
}


//---------------------5-body decay Phase space-------------------------//
int StReacStripEvap_5bodydecay::act(StParticle* p, vector<StParticle>* newPart) {

  //Setting up the two decay energies
  double exen     = 0; //Total Decay Energy
  double exen1    = 0; //1st Decay Energy
  double exen2    = 0; //2nd Decay Energy
  double exen3    = 0;
  double exen4    = 0;
  //Create Varibles to give give directions to particles and Glauber Kick
  double cosTheta;     //Used to set the cos(Theta)
  double theta;        //Used to set the Theta
  double phi;          //Used to set the Phi

  //for volya 2n
  double erel = 0; //rel energy between 2 neutrons

  //Check that all necassery random number generators are found
  if (!fRngExEn)           ERR("Need fRngExEn");
  if (!fRngExEn1)          ERR("Need fRngExEn1");
  if (!fRngExEn2)          ERR("Need fRngExEn2");
  if (!fRngExEn3)	       ERR("Need fRngExEn3");
  if (!fRngExEn4) 	       ERR("Need fRngExEn4");
  if (!fRngUniform)        ERR("Need uniform random number generator");
  if (!fRngGaussGlauber)   ERR("Need gauss random number generator");

  //Removed the fWhichEnergyDoIReconstruct (since it isn't needed for 3body decay)

  double exenTotal = -1;
  //while(1){
    exenTotal = fRngExEn->value(); //MeV
    //if(exenTotal>0) break;
  //}*/

  DSV(exenTotal);
  DSV(exen2);
  fExEnMap[0]=exenTotal; //cout << "ETOTAL: " << exenTotal << endl;
  fExEnMap2[0]=exen2;  
  fExEnMap3[0]=exen3;
  fExEnMap4[0]=exen4;  
  
  //debugging stuff -> Decay Energies
  if (debugSwitch == "exen1")
	  cout << exen1 << endl;
  if (debugSwitch == "exen2")
	  cout << exen2 << endl;
  if (debugSwitch == "exen3")
	  cout << exen3 << endl;
  if (debugSwitch == "exen4")
	  cout << exen4 << endl;


  if (!p) ERR("Particle pointer not set.");


  // change particle, keep velocity the same
  //**zwk reduce beta after reaction following LISE++
  double beta = p->getBeta() * fVelocityScale;        // initial beta
  double A = p->getA();            // initial mass number
  //cout << "Mass: " << p->getA() << " | " << fNn << " | " << fNp << endl;
  p->setA(p->getA() - fNn - fNp);  // remove protons and neutrons
  p->setm(p->getA()*931.494028);        // set new mass
  p->setZ(p->getZ() - fNp);        // set new Z
  p->setQ(p->getZ());              // set new Q
  p->setBeta(beta);                // set velocity

  //cout << "Incoming Etotal: " << p->getEtotal() << endl;
  //cout << "EDecay: " << exenTotal << endl;

  TLorentzVector tl;
  tl.SetPxPyPzE(p->getPx(), p->getPy(), p->getPz(), p->getEtotal());
  DSV(tl.Theta());
  DSV(tl.Phi());
  DSV(tl.E() - tl.M());

  fPzMap[0]=-1;
  // Glauber momentum kick 
  double sig, sig0;
  sig0 = 90;  // MeV/c
  sig = p->getA() * ( fNn + fNp ) / ( A - 1);
  if (sig < 0) ERR("sig < 0; sig = %g", sig);
  sig = fGlauberKickScale * sig0 * sqrt(sig);
  DSV(sig);
  
  cosTheta = fRngUniform->value(-1,1);// cos(theta)
  theta    = acos(cosTheta);
  phi      = fRngUniform->value(0,2.*M_PI);  // phi
  DSV(theta);
  DSV(phi);

  fRngGaussGlauber->setCentroid(0);
  fRngGaussGlauber->setSigma(sig);

  double glaubP = fRngGaussGlauber->value();
  //-=-=-==--=-=-=-==
  //idea to create tail in mom. distribution following LISE++, zwk
  if(glaubP < 0) glaubP = glaubP * fAsymMomentum;
  //--------------------

  DSV(glaubP);
  TVector3 vGlaubP(0,0,glaubP);
  vGlaubP.SetTheta(theta);
  vGlaubP.SetPhi(phi);
  
  TVector3 vP;                  // total momentum
  vP = tl.Vect() + vGlaubP;
  tl.SetVectM(vP,p->getm());
  DSV(tl.Theta());
  DSV(tl.Phi());
  DSV(tl.E() - tl.M());
  
  //debugging stuff
  if (debugSwitch == "thetaLABP")
    cout << tl.Theta()<< endl;
  if (debugSwitch == "phiLABP")
    cout << tl.Phi()<< endl;
  
  
  
  //Now have the excited fragment in TLorentzVector tl
  //Use 3-body decay from ROOT to decay Frag* -> frag + n +n
  //Units are GeV and GeV/c!!
  
  //set lorentz vector of decaying system (GeV unit)
  //need to set_mass to the same as the frag+2n since we don't know the mass excess of the 
  //excited fragment, otherwise can get E of excited frag < then E of the frag-n-n system
  //and therefore the 3-body decay is forbidden.
  //double set_mass = (p->getA()-4)*931.494028 +  2*(939.565346 + 939.565346);
  //cout << "Total mass: " << set_mass + exenTotal << endl;
  double set_mass = (p->getA()-4)*931.494 + 1.0*931.494 + 1.0*931.494 + 1.0*931.494 + 1.0*931.494;
  TLorentzVector t1gev;
  t1gev.SetVectM(vP*(1/1000.), (set_mass+exenTotal)/1000.);

  //cout << vP.x() << " " << vP.y() << " " << vP.z() << endl;
  //cout << p->getm() << " " << p->getA() << endl;
  //cout << exen1 << " " << exen2 << endl;
  //cout << t1gev.M() << "  " << t1gev.Mag() << endl;
  //cout << ( (p->getA()-2)*931.494028/1000. +  939.565346/1000. + 939.565346/1000.) << endl;
  
  //set the masses of the 5 products.
  Double_t Mass5body[5] = { (p->getA()-4)*931.494/1000.,  1.0*931.494/1000., 1.0*931.494/1000., 1.0*931.494/1000., 1.0*931.494/1000. };
  TGenPhaseSpace tPSevent;
  Bool_t bevent = tPSevent.SetDecay(t1gev, 5, Mass5body);
  if(!bevent) cerr << "5-Body Decay is Kinematically FORBIDDEN !!!!!" << endl;

  TLorentzVector *pt1, *pt2, *pt3, *pt4, *pt5;

  double wtmax = tPSevent.GetWtMax();
  while(1){    
    double weight = tPSevent.Generate();
    double rel_weight = weight / wtmax;
    double ran = fRngUniform->value(0,1);

    //events are weighted (rel_weight) between 0 and 1
    //therefore only select event when ran is less then rel_weight
    //if(ran < rel_weight) break;
    break; 
  }
  pt1 = tPSevent.GetDecay(0); //frag - 2n
  pt2 = tPSevent.GetDecay(1); //neutron1
  pt3 = tPSevent.GetDecay(2); //neutron2
  pt4 = tPSevent.GetDecay(3);
  pt5 = tPSevent.GetDecay(4);

  //The returned Lorentz vectors are automatically boosted into the frame
  //of the originally decaying fragment
  //NEED TO CONVERT BACK FROM GEV TO MEV

  //Set fragment info
  p->setEkin(0);
  p->setA(p->getA()-4);
  p->setm(pt1->M()*1000);
 // set fragment energy and direction
  p->setEtotal(pt1->E()*1000); /// \todo implement setLVec() to set particle data from 4-vector
  p->setThetaPhi(pt1->Theta(),pt1->Phi()); 
  fThetaMap[0]  = pt1->Theta();
  fPhiMap[0]    = pt1->Phi();
  fThetaMap2[0] = pt1->Theta();
  fPhiMap2[0]   = pt1->Phi();
  fThetaMap3[0] = pt1->Theta();
  fPhiMap3[0]   = pt1->Phi();
  fThetaMap4[0] = pt1->Theta();
  fPhiMap4[0]   = pt1->Phi();

  
  // set neutron energy and direction
  StParticle pp;
  pp.setZ(0);
  pp.setQ(0);
  pp.setx(p->getx());     // same position
  pp.sety(p->gety()); 
  pp.setz(p->getz());     
  pp.sett(p->gett());     // same time
  pp.setEkin(0);
  pp.setA(1);
  pp.setm(pt2->M()*1000);
  pp.setEtotal(pt2->E()*1000);
  pp.setThetaPhi(pt2->Theta(),pt2->Phi()); 

  if(! newPart) ERR("Pointer newPart not set; Cannot add new particles");
  newPart->push_back(pp); 

  // set second neutron energy and direction
  StParticle ppp;   
  ppp.setA(1);
  ppp.setm(pt3->M()*1000);
  ppp.setZ(0);
  ppp.setQ(0);
  ppp.setx(p->getx());     // same position
  ppp.sety(p->gety()); 
  ppp.setz(p->getz());     
  ppp.sett(p->gett());     // same time
  ppp.setEkin(0);
  ppp.setEtotal(pt3->E()*1000);
  ppp.setThetaPhi(pt3->Theta(),pt3->Phi());

  if(! newPart) ERR("Pointer newPart not set; Cannot add new particles");
  newPart->push_back(ppp); 

 // set third neutron energy and direction
  StParticle p4;   
  p4.setA(1);
  p4.setm(pt4->M()*1000);
  p4.setZ(0);
  p4.setQ(0);
  p4.setx(p->getx());     // same position
  p4.sety(p->gety()); 
  p4.setz(p->getz());     
  p4.sett(p->gett());     // same time
  p4.setEkin(0);
  p4.setEtotal(pt4->E()*1000);
  p4.setThetaPhi(pt4->Theta(),pt4->Phi());

  if(! newPart) ERR("Pointer newPart not set; Cannot add new particles");
  newPart->push_back(p4); 

 // set fourth neutron energy and direction
  StParticle p5;   
  p5.setA(1);
  p5.setm(pt5->M()*1000);
  p5.setZ(0);
  p5.setQ(0);
  p5.setx(p->getx());     // same position
  p5.sety(p->gety()); 
  p5.setz(p->getz());     
  p5.sett(p->gett());     // same time
  p5.setEkin(0);
  p5.setEtotal(pt5->E()*1000);
  p5.setThetaPhi(pt5->Theta(),pt5->Phi());

  if(! newPart) ERR("Pointer newPart not set; Cannot add new particles");
  newPart->push_back(p5); 


  /*cout << "MOMENTA: " << p->getPz() << endl;
  cout << pt5->E() << endl; //" | " << ppp->getPz() << " | " << p4->getPz() << " | " << p5->getPz() << endl;
  cout << pt4->E() << endl;
  cout << pt3->E() << endl;
  cout << pt2->E()  << endl;
  cout << pt1->E()   << endl;*/
  /*
  //-------
  //Calculating angles, energy for output of the original nn correlations for PRL comment reply
  TLorentzVector lv_F = *pt1;
  TLorentzVector lv_N1 = *pt2;
  TLorentzVector lv_N2 = *pt3;

  TLorentzVector lv_sys = lv_F + lv_N1 + lv_N2;
  TVector3 boost = lv_sys.BoostVector();
  TLorentzVector lv_N1cm = lv_N1;
  lv_N1cm.Boost(-1*boost);
  TLorentzVector lv_N2cm = lv_N2;
  lv_N2cm.Boost(-1*boost);
  TLorentzVector lv_Fcm = lv_F;
  lv_Fcm.Boost(-1*boost);

  //double cosn1n2theta =  cos(lv_N1cm.Angle(lv_N2cm.Vect()));
  double cosn1n2theta =  cos(lv_Fcm.Angle(lv_N1cm.Vect()));
  cout << cosn1n2theta << endl;

  TLorentzVector lvNN = lv_N1 + lv_N2;
  double edecayNN = lvNN.M() - lv_N1.M() - lv_N2.M();
  //cout << edecayNN << endl;
  //-----------
  */



  DSV(p->getA());
  DSV(p->getZ());
  return 0;

}

int StReacStripEvap_5bodydecay::savedValue(size_t part_id, string name, double* v) {
  DSV(part_id);
  DSV(name);

  if (name == "exen" &&  (fExEnMap.find(part_id) != fExEnMap.end()) ) {
    *v = fExEnMap[part_id];
    DSV(*v);
    return 0;
  }
  if (name == "exen2" &&  (fExEnMap2.find(part_id) != fExEnMap2.end()) ) {
    *v = fExEnMap2[part_id];
    DSV(*v);
    return 0;
  }
  if (name == "exen3" &&  (fExEnMap3.find(part_id) != fExEnMap3.end()) ) {
    *v = fExEnMap3[part_id];
    DSV(*v);
    return 0;
  }
  if (name == "exen4" &&  (fExEnMap4.find(part_id) != fExEnMap4.end()) ) {
    *v = fExEnMap4[part_id];
    DSV(*v);
    return 0;
  }
  if (name == "ph" &&  (fPhiMap.find(part_id) != fPhiMap.end()) ) {
    *v = fPhiMap[part_id];
    DSV(*v);
    return 0;
  }
  if (name == "ph2" &&  (fPhiMap2.find(part_id) != fPhiMap2.end()) ) {
    *v = fPhiMap2[part_id];
    DSV(*v);
    return 0;
  }
  if (name == "ph3" &&  (fPhiMap3.find(part_id) != fPhiMap3.end()) ) {
    *v = fPhiMap3[part_id];
    DSV(*v);
    return 0;
  }
  if (name == "ph4" &&  (fPhiMap4.find(part_id) != fPhiMap4.end()) ) {
    *v = fPhiMap4[part_id];
    DSV(*v);
    return 0;
  }
  if (name == "th" &&  (fThetaMap.find(part_id) != fThetaMap.end()) ) {
    *v = fThetaMap[part_id];
    DSV(*v);
    return 0;
  }
  if (name == "th2" &&  (fThetaMap2.find(part_id) != fThetaMap2.end()) ) {
    *v = fThetaMap2[part_id];
    DSV(*v);
    return 0;
  }
  if (name == "th3" &&  (fThetaMap3.find(part_id) != fThetaMap3.end()) ) {
    *v = fThetaMap3[part_id];
    DSV(*v);
    return 0;
  }
  if (name == "th4" &&  (fThetaMap4.find(part_id) != fThetaMap4.end()) ) {
    *v = fThetaMap4[part_id];
    DSV(*v);
    return 0;
  }


  if (name == "pz" &&  (fPzMap.find(part_id) != fPzMap.end()) ) {
    *v = fPzMap[part_id];
    DSV(*v);
    return 0;
  }





  return 1;
}
////////////////////////////5body decay///////////////////////////////


//---------------------4 Neutron Sequential Decay-------------------------//
int StReacStripEvap4neutron::act(StParticle* p, vector<StParticle>* newPart) {

  //Setting up the two decay energies
  double exen     = 0; //Total Decay Energy
  double exen1    = 0; //1st Decay Energy
  double exen2    = 0; //2nd Decay Energy
  double exen3    = 0; //3rd Decay Energy
  double exen4    = 0;
  //Create Varibles to give give directions to particles and Glauber Kick
  double cosTheta;     //Used to set the cos(Theta)
  double theta;        //Used to set the Theta
  double phi;          //Used to set the Phi
  double test_angle;   // test angle to see if random number generators are correct
  double theta3;       // Theta of third neutron, for correlating last 2 neutrons

  // cout << "Check velocity, asymMom: " << fVelocityScale << ", " << fAsymMomentum << endl;

  //for volya 2n
  double erel = 0; //rel energy between 2 neutrons

  //Check that all necassery random number generators are found
  if (!fRngExEn)           ERR("Need fRngExEn");
  if (!fRngExEn1)          ERR("Need fRngExEn1");
  if (!fRngExEn2)          ERR("Need fRngExEn2");
  if (!fRngExEn3)	       ERR("Need fRngExEn3");
  if (!fRngExEn4)	       ERR("Need fRngExEn4");
  if (!fRngUniform)        ERR("Need uniform random number generator");
  if (!fRngGaussGlauber)   ERR("Need gauss random number generator");
  if (!fRngLegendre)       ERR("Need legendre random number generator");

  if (fWhichEnergyDoIReconstruct == "Reconstruct4thEnergy_FILLERET"){// YOU NEED A MUCH MORE EFFICIENT WAY TO DO THIS!
	do {
		exen = fRngExEn->value();

		//cout << "TOTAL: " << exen << endl;
		//exen = 3.141592654;
  	} while (exen < 0);

	do {
		do {
		  exen1 = fRngExEn1->value(0,exen); 
  		} while (exen1 < 0);
		do {
		  exen2 = fRngExEn2->value(0,exen-exen1); 
		} while(exen2 < 0);			
		do {
		  exen3 = fRngExEn3->value(0,exen-exen1-exen2); 
		} while(exen3 < 0);

		exen4 = exen-exen1-exen2-exen3; 
						
    } while (exen4 < 0);
	
	DSV(exen1);
	DSV(exen2);
	DSV(exen3);
	DSV(exen4);
	fExEnMap[0]=exen1;
	fExEnMap2[0]=exen2;
    fExEnMap3[0]=exen3; 
  	fExEnMap4[0]=exen4;

   	} 
   if (fWhichEnergyDoIReconstruct == "volya2n"){


	do{
		exen1 = fRngExEn1->value();
		//exen1=0.01;
  	} 
	 while (exen1 < 0);
	 do{
		exen2 = fRngExEn2->value();
		//exen2 = 10;
  	 }  
     while (exen2 < 0);
    //for volya + bw
    fRngExEn3->value2d(exen,erel);

    exen3 = 0.5 * (exen + erel);
    exen4 = 0.5 * (exen - erel);

    DSV(exen3);
    DSV(exen4);
  	fExEnMap[0]=exen1;
	fExEnMap2[0]=exen2;
    fExEnMap3[0]=exen3;
    fExEnMap4[0]=exen4;

  }

 else if( fWhichEnergyDoIReconstruct == "grigorenko"){
// First a total 5body energy
     do{
         exen4 = fRngExEn4->value();
     }
      while(exen < 0);
     
          
 
     // Now sample a total 3body energy LESS than the 12He
     
      do{ 
         exen = fRngExEn->value(); 
      }
      while( exen < exen4);
     //}

         
     double r;
     //r = ((double) rand() / (RAND_MAX));
     r = 1/3.0;
     // distribute the remaining energy based on a random number between zero and 1
     exen1 = (exen - exen4)*r;
     exen2 = (exen - exen4)*r;
     exen3 = (exen - exen4)*r;
    
     if(exen1 < 0 || exen2 < 0 || exen < 0 || exen3 < 0 || exen4 < 0) cout << "SOMETHING WENT NEGATIVEEEEEEE" << endl;

     //DSV(exenTotal);
 	 //DSV(exenDiNeutron);
 	 fExEnMap[0]=exen1; // thermal 1
  	 fExEnMap2[0]=exen2; // thermal 2
  	 fExEnMap3[0]=exen3;
  	 fExEnMap4[0]=exen4;  

    } // end grigorenko


 else {
	do{
		exen1 = fRngExEn1->value(); //cout << "exen1: " << exen1 << endl;
		//exen1=0.01;
  	} 
	 while (exen1 < 0);
	 do{
		exen2 = fRngExEn2->value(); //cout << "exen2: " << exen2 << endl;
		//exen2 = 10;
  	 }  
	 while (exen2 < 0);
	 do{
		exen3 = fRngExEn3->value(); //cout << "exen3: " << exen3 << endl;
		//exen3 = 0.01;
  	 }  
	 while (exen3 < 0);
	 do{
		exen4 = fRngExEn4->value(); //cout << "exen4: " << exen4 << endl;
		//exen3 = 0.01;
  	 }  
	 while (exen4 < 0);

	DSV(exen1);
	DSV(exen2);
    DSV(exen3);
	DSV(exen4);

	fExEnMap[0]=exen1;
	fExEnMap2[0]=exen2;
 	fExEnMap3[0]=exen3;
	fExEnMap4[0]=exen4;
    
      }


  //debugging stuff -> Decay Energies
  if (debugSwitch == "exen1")
	  cout << exen1 << endl;
  if (debugSwitch == "exen2")
	  cout << exen2 << endl;
  if (debugSwitch == "exen3")
	  cout << exen3 << endl;
  if (debugSwitch == "exen4")
	  cout << exen4 << endl;


  if (!p) ERR("Particle pointer not set.");

  // change particle, keep velocity the same
  //**zwk reduce beta after reaction following LISE++
  double beta = p->getBeta() * fVelocityScale;        // initial beta
  double A = p->getA();            // initial mass number
  p->setA(p->getA() - fNn - fNp);  // remove protons and neutrons
  //cout << "A: " << p->getA() << "N: " << fNn << "Z: " << fNp << endl;
  p->setm(p->getA()*931.494088);        // set new mass
  
  p->setZ(p->getZ() - fNp);        // set new Z
  p->setQ(p->getZ());              // set new Q
  
  p->setBeta(beta);                // set velocity

  TLorentzVector tl;
  tl.SetPxPyPzE(p->getPx(), p->getPy(), p->getPz(), p->getEtotal());

  DSV(tl.Theta());
  DSV(tl.Phi());
  DSV(tl.E() - tl.M());

  fPzMap[0]=-1;
  // Glauber momentum kick 
  double sig, sig0;
  sig0 = 90;  // MeV/c
  sig = p->getA() * ( fNn + fNp ) / ( A - 1);
  if (sig < 0) ERR("sig < 0; sig = %g", sig);
  sig = fGlauberKickScale * sig0 * sqrt(sig);
  DSV(sig);
  
  cosTheta = fRngUniform->value(-1,1);// cos(theta)
  theta    = acos(cosTheta);
  phi      = fRngUniform->value(0,2.*M_PI);  // phi
  DSV(theta);
  DSV(phi);

  fRngGaussGlauber->setCentroid(0);
  fRngGaussGlauber->setSigma(sig);

  double glaubP = fRngGaussGlauber->value();

  //-=-=-==--=-=-=-==
  //idea to create tail in mom. distribution following LISE++, zwk
  if(glaubP < 0) glaubP = glaubP * fAsymMomentum;
  //--------------------

  DSV(glaubP);
  TVector3 vGlaubP(0,0,glaubP);
  vGlaubP.SetTheta(theta);
  vGlaubP.SetPhi(phi);
  
  
  TVector3 vP;                  // total momentum
  vP = tl.Vect() + vGlaubP;
  tl.SetVectM(vP,p->getm());

   //cout << "FRAGMENT: " << tl.Px() << " | " << tl.Py() << " | " << tl.Pz() << " | " << tl.E() << endl;


  DSV(tl.Theta());
  DSV(tl.Phi());
  DSV(tl.E() - tl.M());


  //debugging stuff
  if (debugSwitch == "thetaLABP")
	  cout << tl.Theta()<< endl;
  if (debugSwitch == "phiLABP")
	  cout << tl.Phi()<< endl;

  TVector3 t3Boost;
  t3Boost = tl.BoostVector();  // boost vector 

  // start of neutron evaporation
  p->setA(p->getA() - 1);
  p->setm(p->getA()*931.494088);
  p->setEkin(0);             

  TLorentzVector tl1;  // particle 1 (heavy fragment)
  tl1.SetPxPyPzE(p->getPx(), p->getPy(), p->getPz(), p->getEtotal());
  

  // create neutron
  StParticle pp;   
  pp.setA(1);
  pp.setm(pp.getA()*939.565378);
  pp.setZ(0);
  pp.setQ(0);
  pp.setx(p->getx());     // same position
  pp.sety(p->gety()); 
  pp.setz(p->getz());     
  pp.sett(p->gett());     // same time
  pp.setEkin(0);
  
  TLorentzVector tl2;  // particle 2 (neutron)
  tl2.SetPxPyPzE(pp.getPx(), pp.getPy(), pp.getPz(), pp.getEtotal());
  
  double eN, eF;   // total neutron and fragment energy
  double eCM;   // total CM energy

  eCM = pp.getm() + p->getm() + exen1;  // total CM energy
  eN = gsl_pow_2(eCM) + gsl_pow_2(pp.getm()) - gsl_pow_2(p->getm());
  eN = eN/(2.*eCM);                    // total energy neutron
  eF = gsl_pow_2(eCM) - gsl_pow_2(pp.getm()) + gsl_pow_2(p->getm());
  eF = eF/(2.*eCM);                    // total energy of fragment

  /// \todo check is arg of sqrt is positive
  double pN,pF; // fragment and neutron momentum
  pN = eN*eN - gsl_pow_2(pp.getm());
  pN = sqrt(pN);
  pF = eF*eF - gsl_pow_2(p->getm());
  if (pF < 0)
    pF = 0;
  else 
    pF = sqrt(pF);
  pF = -pF;  // fragment goes in opposit direction

  tl1.SetPxPyPzE(0,0,pF,eF);
  tl2.SetPxPyPzE(0,0,pN,eN);
  
  // set theta and phi for the first decay to some random value
  cosTheta = fRngUniform->value(-1,1);       // cos(theta)
  theta    = acos(cosTheta);
  //test_angle = fRngLegendre->value(-1,1);
  //test_angle = acos(test_angle);
  phi      = fRngUniform->value(0,2.*M_PI);  // phi
  fThetaMap[0] = theta;
  //fThetaMap[0] = test_angle;
  fPhiMap[0]   = phi;
  //cout << "test_angle: " << test_angle << endl;

  tl1.SetTheta(theta); // Set fragment angle
  tl1.SetPhi(phi); // Set fragment phi angle
  tl2.SetTheta(M_PI - theta); // Set neutron angle 180-theta
  tl2.SetPhi(phi + M_PI); // set neutron phi angle

  TVector3 t4Boost;
  t4Boost = tl1.BoostVector();  // boost vector for 24O

  // start of the second neutron evaporation
  p->setA(p->getA()-1);
  p->setm(p->getA()*931.494088);//931.494028
  p->setEkin(0);               // 23O at rest


  // create second neutron
  StParticle ppp;   
  ppp.setA(1);
  ppp.setm(ppp.getA()*939.565378);//939.565346//939.565378
  ppp.setZ(0);
  ppp.setQ(0);
  ppp.setx(p->getx());     // same position
  ppp.sety(p->gety()); 
  ppp.setz(p->getz());     
  ppp.sett(p->gett());     // same time
  ppp.setEkin(0);
  
  TLorentzVector tl3;  // particle 3 (neutron)
  tl3.SetPxPyPzE(ppp.getPx(), ppp.getPy(), ppp.getPz(), ppp.getEtotal());

  double eN2, eF2;   // total neutron and fragment energy
  double eCM2;       // total CM energy

  eCM2 = ppp.getm() + p->getm() + exen2;  // total CM energy
  eN2 = gsl_pow_2(eCM2) + gsl_pow_2(ppp.getm()) - gsl_pow_2(p->getm());
  eN2 = eN2/(2.*eCM2);                    // total energy neutron
  eF2 = gsl_pow_2(eCM2) - gsl_pow_2(ppp.getm()) + gsl_pow_2(p->getm());
  eF2 = eF2/(2.*eCM2);                    // total energy of fragment

  /// \todo check is arg of sqrt is positive
  double pN2, pF2; // fragment and neutron momentum
  pN2 = eN2*eN2 - gsl_pow_2(ppp.getm());
  pN2 = sqrt(pN2);
  pF2 = eF2*eF2 - gsl_pow_2(p->getm());
  if (pF2 < 0)
    pF2 = 0;
  else 
    pF2 = sqrt(pF2);
  pF2 = -pF2;  // fragment goes in opposite direction

  tl1.SetPxPyPzE(0,0,pF2,eF2);
  tl3.SetPxPyPzE(0,0,pN2,eN2);
  
  // set theta and phi to some random value
  cosTheta = fRngUniform->value(-1,1);       // cos(theta)
  theta    = acos(cosTheta);
  phi      = fRngUniform->value(0,2.*M_PI);  // phi
  fThetaMap2[0] = theta;
  fPhiMap2[0]   = phi;
  tl1.SetTheta(theta);        // Set fragment angle
  tl1.SetPhi(phi);            // Set fragment phi angle
  tl3.SetTheta(M_PI - theta); // Set neutron angle 180-theta
  tl3.SetPhi(phi + M_PI);     // Set neutron phi angle

  TVector3 t5Boost;
  t5Boost = tl1.BoostVector(); 

 // start of the third neutron evaporation
  p->setA(p->getA()-1);
  p->setm(p->getA()*931.494088);
  p->setEkin(0);               // 23O at rest


// create third neutron
// this notation is stupid@!
  StParticle pppp;   
  pppp.setA(1);
  pppp.setm(pppp.getA()*939.565378);
  pppp.setZ(0);
  pppp.setQ(0);
  pppp.setx(p->getx());     // same position
  pppp.sety(p->gety()); 
  pppp.setz(p->getz());     
  pppp.sett(p->gett());     // same time
  pppp.setEkin(0);
  
  TLorentzVector tl4;  // particle 4 (neutron)
  tl4.SetPxPyPzE(pppp.getPx(), pppp.getPy(), pppp.getPz(), pppp.getEtotal());

  double eN3, eF3;   // total neutron and fragment energy
  double eCM3;       // total CM energy

  eCM3 = pppp.getm() + p->getm() + exen3;  // total CM energy
  eN3 = gsl_pow_2(eCM3) + gsl_pow_2(pppp.getm()) - gsl_pow_2(p->getm());
  eN3 = eN3/(2.*eCM3);                    // total energy neutron
  eF3 = gsl_pow_2(eCM3) - gsl_pow_2(pppp.getm()) + gsl_pow_2(p->getm());
  eF3 = eF3/(2.*eCM3);                    // total energy of fragment

  /// \todo check is arg of sqrt is positive
  double pN3, pF3; // fragment and neutron momentum
  pN3 = eN3*eN3 - gsl_pow_2(pppp.getm());
  pN3 = sqrt(pN3);
  pF3 = eF3*eF3 - gsl_pow_2(p->getm());
  if (pF3 < 0)
    pF3 = 0;
  else 
    pF3 = sqrt(pF3);
  pF3 = -pF3;  // fragment goes in opposit direction

  tl1.SetPxPyPzE(0,0,pF3,eF3);
  tl4.SetPxPyPzE(0,0,pN3,eN3);
  
  // set theta and phi to some random value
  //
  cosTheta = fRngUniform->value(-1,1);       // cos(theta)
  theta3    = acos(cosTheta);
  phi      = fRngUniform->value(0,2.*M_PI);  // phi
  fThetaMap3[0] = theta3;
  fPhiMap3[0]   = phi;

  tl1.SetTheta(theta3);        // Set fragment angle
  tl1.SetPhi(phi);            // Set fragment phi angle
  tl4.SetTheta(M_PI - theta3); // Set neutron angle 180-theta
  tl4.SetPhi(phi + M_PI);     // Set neutron phi angle

  TVector3 t6Boost;
  t6Boost = tl1.BoostVector();  // boost vector for 24O


 // start of the fourth neutron evaporation
  p->setA(p->getA()-1);
  p->setm(p->getA()*931.494088);
  p->setEkin(0);               // 23O at rest


// create fourth neutron
// this notation is stupid@!
  StParticle p5;   
  p5.setA(1);
  p5.setm(p5.getA()*939.565378);
  p5.setZ(0);
  p5.setQ(0);
  p5.setx(p->getx());     // same position
  p5.sety(p->gety()); 
  p5.setz(p->getz());     
  p5.sett(p->gett());     // same time
  p5.setEkin(0);
  
  TLorentzVector tl5;  // particle 4 (neutron)
  tl5.SetPxPyPzE(p5.getPx(), p5.getPy(), p5.getPz(), p5.getEtotal());

  double eN4, eF4;   // total neutron and fragment energy
  double eCM4;       // total CM energy

  eCM4 = p5.getm() + p->getm() + exen4;  // total CM energy
  eN4 = gsl_pow_2(eCM4) + gsl_pow_2(p5.getm()) - gsl_pow_2(p->getm());
  eN4 = eN4/(2.*eCM4);                    // total energy neutron
  eF4 = gsl_pow_2(eCM4) - gsl_pow_2(p5.getm()) + gsl_pow_2(p->getm());
  eF4 = eF4/(2.*eCM4);                    // total energy of fragment

  /// \todo check is arg of sqrt is positive
  double pN4, pF4; // fragment and neutron momentum
  pN4 = eN4*eN4 - gsl_pow_2(p5.getm());
  pN4 = sqrt(pN4);
  pF4 = eF4*eF4 - gsl_pow_2(p->getm());
  if (pF4 < 0)
    pF4 = 0;
  else 
    pF4 = sqrt(pF4);
    pF4 = -pF4;  // fragment goes in opposit direction

  tl1.SetPxPyPzE(0,0,pF4,eF4);
  tl5.SetPxPyPzE(0,0,pN4,eN4);
  
  // set theta and phi to some random value
  //cosTheta = fRngUniform->value(-1,1);       // cos(theta)
  
  
  if(fWhichEnergyDoIReconstruct == "CorrelatedSequential" || fWhichEnergyDoIReconstruct == "volya2n"){ //cout << "correlated" << endl;
        cosTheta = fRngLegendre->value(-1,1);
        //theta = acos(cosTheta);
        if(acos(cosTheta) > theta3)       theta    = acos(cosTheta) - theta3;
        else if(acos(cosTheta) <= theta3) theta    = theta3 - acos(cosTheta);// Correlate the 3rd and 4th neutron
  }
  else {
        cosTheta = fRngUniform->value(-1,1);
        theta    = acos(cosTheta);
  }
       
   phi      = fRngUniform->value(0,2.*M_PI);  // phi
 
  fThetaMap4[0] = theta;
  fPhiMap4[0]   = phi;
  tl1.SetTheta(theta);        // Set fragment angle
  tl1.SetPhi(phi);            // Set fragment phi angle
  tl5.SetTheta(M_PI - theta); // Set neutron angle 180-theta
  tl5.SetPhi(phi + M_PI);     // Set neutron phi angle





  //Boosting into the frame where the fragment is at rest after the first decay
  tl1.Boost(t6Boost); tl1.Boost(t5Boost); tl1.Boost(t4Boost); 
  tl3.Boost(t4Boost);//Must boost each neutron back to the frame where fragment is at rest after 1st decay
  tl4.Boost(t5Boost); tl4.Boost(t4Boost); // Here we need 2 boosts. 
  tl5.Boost(t6Boost); tl5.Boost(t5Boost); tl5.Boost(t4Boost);

  //Boosting all four particles to the lab frame
  tl1.Boost(t3Boost);
  tl2.Boost(t3Boost);
  tl3.Boost(t3Boost);
  tl4.Boost(t3Boost);
  tl5.Boost(t3Boost);

  // set fragment energy and direction
  p->setEtotal(tl1.E()); /// \todo implement setLVec() to set particle data from 4-vector
  p->setThetaPhi(tl1.Theta(),tl1.Phi()); 
  
  // set neutron energy and direction
  pp.setEtotal(tl2.E());
  pp.setThetaPhi(tl2.Theta(),tl2.Phi()); 

  if(! newPart) ERR("Pointer newPart not set; Cannot add new particles");
  newPart->push_back(pp); 

  // set second neutron energy and direction
  ppp.setEtotal(tl3.E());
  ppp.setThetaPhi(tl3.Theta(),tl3.Phi());

  if(! newPart) ERR("Pointer newPart not set; Cannot add new particles");
  newPart->push_back(ppp); 

  // set third neutron energy and direction
  pppp.setEtotal(tl4.E());
  pppp.setThetaPhi(tl4.Theta(),tl4.Phi());

  if(! newPart) ERR("Pointer newPart not set; Cannot add new particles");
  newPart->push_back(pppp); 

  // set third neutron energy and direction
  p5.setEtotal(tl5.E());
  p5.setThetaPhi(tl5.Theta(),tl5.Phi());

  if(! newPart) ERR("Pointer newPart not set; Cannot add new particles");
  newPart->push_back(p5); 

  DSV(p->getA());
  DSV(p->getZ());

  //cout << "FRAGMENT: " << tl1.Px() << " | " << tl1.Py() << " | " << tl1.Pz() << " | " << tl1.E() << endl;
  //cout << "NEUTRON1: " << tl2.Px() << " | " << tl2.Py() << " | " << tl2.Pz() << " | " << tl2.E() << endl;
  //cout << "NEUTRON2: " << tl3.Px() << " | " << tl3.Py() << " | " << tl3.Pz() << " | " << tl3.E() << endl;
  //cout << "NEUTRON3: " << tl4.Px() << " | " << tl4.Py() << " | " << tl4.Pz() << " | " << tl4.E() << endl;
  //cout << "NEUTRON4: " << tl5.Px() << " | " << tl5.Py() << " | " << tl5.Pz() << " | " << tl5.E() << endl;
  //cout << "TOTAL:    " << tl1.Px() + tl2.Px() + tl3.Px() + tl4.Px() + tl5.Px() << " | " << tl1.Py() + tl2.Py() + tl3.Py() + tl4.Py() + tl5.Py() << " | " << tl1.Pz() + tl2.Pz() + tl3.Pz() + tl4.Pz() + tl5.Pz() << " | " << tl1.E() + tl2.E() + tl3.E() + tl4.E() + tl5.E() << endl;


  return 0;
}

int StReacStripEvap4neutron::savedValue(size_t part_id, string name, double* v) {
  DSV(part_id);
  DSV(name);

  if (name == "exen" &&  (fExEnMap.find(part_id) != fExEnMap.end()) ) {
    *v = fExEnMap[part_id];
    DSV(*v);
    return 0;
  }
  if (name == "exen2" &&  (fExEnMap2.find(part_id) != fExEnMap2.end()) ) {
    *v = fExEnMap2[part_id];
    DSV(*v);
    return 0;
  }
  if (name == "exen3" &&  (fExEnMap3.find(part_id) != fExEnMap3.end()) ) {
    *v = fExEnMap3[part_id];
    DSV(*v);
    return 0;
  }
  if (name == "exen4" &&  (fExEnMap4.find(part_id) != fExEnMap4.end()) ) {
    *v = fExEnMap4[part_id];
    DSV(*v);
    return 0;
  }

  if (name == "ph" &&  (fPhiMap.find(part_id) != fPhiMap.end()) ) {
    *v = fPhiMap[part_id];
    DSV(*v);
    return 0;
  }
  if (name == "ph2" &&  (fPhiMap2.find(part_id) != fPhiMap2.end()) ) {
    *v = fPhiMap2[part_id];
    DSV(*v);
    return 0;
  }
  if (name == "ph3" &&  (fPhiMap3.find(part_id) != fPhiMap3.end()) ) {
    *v = fPhiMap3[part_id];
    DSV(*v);
    return 0;
  }
  if (name == "ph4" &&  (fPhiMap4.find(part_id) != fPhiMap4.end()) ) {
    *v = fPhiMap4[part_id];
    DSV(*v);
    return 0;
  }

  if (name == "th" &&  (fThetaMap.find(part_id) != fThetaMap.end()) ) {
    *v = fThetaMap[part_id];
    DSV(*v);
    return 0;
  }
  if (name == "th2" &&  (fThetaMap2.find(part_id) != fThetaMap2.end()) ) {
    *v = fThetaMap2[part_id];
    DSV(*v);
    return 0;
  }
  if (name == "th3" &&  (fThetaMap3.find(part_id) != fThetaMap3.end()) ) {
    *v = fThetaMap3[part_id];
    DSV(*v);
    return 0;
  }
  if (name == "th4" &&  (fThetaMap4.find(part_id) != fThetaMap4.end()) ) {
    *v = fThetaMap4[part_id];
    DSV(*v);
    return 0;
  }

  if (name == "pz" &&  (fPzMap.find(part_id) != fPzMap.end()) ) {
    *v = fPzMap[part_id];
    DSV(*v);
    return 0;
  }
  return 1;
}// End 4N


//////////////////////////////////////////////////////////////////////
// ==========================================================
//          THREE NEUTRON REACTION CLASSES:
//           4bodydecay, StripEvap3Neutron
// ==========================================================
//////////////////////////////////////////////////////////////////////


//---------------------4-body decay-------------------------//
int StReacStripEvap_4bodydecay::act(StParticle* p, vector<StParticle>* newPart) {

  //Setting up the two decay energies
  double exen     = 0; //Total Decay Energy
  double exen1    = 0; //1st Decay Energy
  double exen2    = 0; //2nd Decay Energy
  double exen3    = 0;
  //Create Varibles to give give directions to particles and Glauber Kick
  double cosTheta;     //Used to set the cos(Theta)
  double theta;        //Used to set the Theta
  double phi;          //Used to set the Phi

  //for volya 2n
  double erel = 0; //rel energy between 2 neutrons

  //Check that all necassery random number generators are found
  if (!fRngExEn)           ERR("Need fRngExEn");
  if (!fRngExEn1)          ERR("Need fRngExEn1");
  if (!fRngExEn2)          ERR("Need fRngExEn2");
  if (!fRngExEn3)	       ERR("Need fRngExEn3");
  if (!fRngUniform)        ERR("Need uniform random number generator");
  if (!fRngGaussGlauber)   ERR("Need gauss random number generator");

  double exenTotal = -1;
  //while(1){
    exenTotal = fRngExEn->value(); //MeV
    //if(exenTotal>0) break;
  //}*/

  DSV(exenTotal);
  DSV(exen2);
  fExEnMap[0]=exenTotal; //cout << "ETOTAL: " << exenTotal << endl;
  fExEnMap2[0]=exen2;  
  fExEnMap3[0]=exen3;

  
  //debugging stuff -> Decay Energies
  if (debugSwitch == "exen1")
	  cout << exen1 << endl;
  if (debugSwitch == "exen2")
	  cout << exen2 << endl;
  if (debugSwitch == "exen3")
	  cout << exen3 << endl;

  if (!p) ERR("Particle pointer not set.");


  // change particle, keep velocity the same
  //**zwk reduce beta after reaction following LISE++
  double beta = p->getBeta() * fVelocityScale;        // initial beta
  double A = p->getA();            // initial mass number
  //cout << "Mass: " << p->getA() << " | " << fNn << " | " << fNp << endl;
  p->setA(p->getA() - fNn - fNp);  // remove protons and neutrons
  p->setm(p->getA()*931.494028);        // set new mass
  p->setZ(p->getZ() - fNp);        // set new Z
  p->setQ(p->getZ());              // set new Q
  p->setBeta(beta);                // set velocity

  //cout << "Incoming Etotal: " << p->getEtotal() << endl;
  //cout << "EDecay: " << exenTotal << endl;

  TLorentzVector tl;
  tl.SetPxPyPzE(p->getPx(), p->getPy(), p->getPz(), p->getEtotal());
  DSV(tl.Theta());
  DSV(tl.Phi());
  DSV(tl.E() - tl.M());

  fPzMap[0]=-1;
  // Glauber momentum kick 
  double sig, sig0;
  sig0 = 90;  // MeV/c
  sig = p->getA() * ( fNn + fNp ) / ( A - 1);
  if (sig < 0) ERR("sig < 0; sig = %g", sig);
  sig = fGlauberKickScale * sig0 * sqrt(sig);
  DSV(sig);
  
  cosTheta = fRngUniform->value(-1,1);// cos(theta)
  theta    = acos(cosTheta);
  phi      = fRngUniform->value(0,2.*M_PI);  // phi
  DSV(theta);
  DSV(phi);

  fRngGaussGlauber->setCentroid(0);
  fRngGaussGlauber->setSigma(sig);

  double glaubP = fRngGaussGlauber->value();
  //-=-=-==--=-=-=-==
  //idea to create tail in mom. distribution following LISE++, zwk
  if(glaubP < 0) glaubP = glaubP * fAsymMomentum;
  //--------------------

  DSV(glaubP);
  TVector3 vGlaubP(0,0,glaubP);
  vGlaubP.SetTheta(theta);
  vGlaubP.SetPhi(phi);
  
  TVector3 vP;                  // total momentum
  vP = tl.Vect() + vGlaubP;
  tl.SetVectM(vP,p->getm());
  DSV(tl.Theta());
  DSV(tl.Phi());
  DSV(tl.E() - tl.M());
  
  //debugging stuff
  if (debugSwitch == "thetaLABP")
    cout << tl.Theta()<< endl;
  if (debugSwitch == "phiLABP")
    cout << tl.Phi()<< endl;
  
  
  
  //Now have the excited fragment in TLorentzVector tl
  //Use 3-body decay from ROOT to decay Frag* -> frag + n +n
  //Units are GeV and GeV/c!!
  
  //set lorentz vector of decaying system (GeV unit)
  //need to set_mass to the same as the frag+2n since we don't know the mass excess of the 
  //excited fragment, otherwise can get E of excited frag < then E of the frag-n-n system
  //and therefore the 3-body decay is forbidden.
  //double set_mass = (p->getA()-4)*931.494028 +  2*(939.565346 + 939.565346);
  //cout << "Total mass: " << set_mass + exenTotal << endl;
  double set_mass = (p->getA()-3)*931.494 + 1.0*931.494 + 1.0*931.494 + 1.0*931.494;
  TLorentzVector t1gev;
  t1gev.SetVectM(vP*(1/1000.), (set_mass+exenTotal)/1000.);

  //cout << vP.x() << " " << vP.y() << " " << vP.z() << endl;
  //cout << p->getm() << " " << p->getA() << endl;
  //cout << exen1 << " " << exen2 << endl;
  //cout << t1gev.M() << "  " << t1gev.Mag() << endl;
  //cout << ( (p->getA()-2)*931.494028/1000. +  939.565346/1000. + 939.565346/1000.) << endl;
  
  //set the masses of the 5 products.
  Double_t Mass4body[4] = { (p->getA()-3)*931.494/1000.,  1.0*931.494/1000., 1.0*931.494/1000., 1.0*931.494/1000.};
  TGenPhaseSpace tPSevent;
  Bool_t bevent = tPSevent.SetDecay(t1gev, 4, Mass4body);
  if(!bevent) cerr << "4-Body Decay is Kinematically FORBIDDEN !!!!!" << endl;

  TLorentzVector *pt1, *pt2, *pt3, *pt4;

  double wtmax = tPSevent.GetWtMax();
  while(1){    
    double weight = tPSevent.Generate();
    double rel_weight = weight / wtmax;
    double ran = fRngUniform->value(0,1);

    //events are weighted (rel_weight) between 0 and 1
    //therefore only select event when ran is less then rel_weight
    //if(ran < rel_weight) break;
    break; 
  }
  pt1 = tPSevent.GetDecay(0); //frag - 2n
  pt2 = tPSevent.GetDecay(1); //neutron1
  pt3 = tPSevent.GetDecay(2); //neutron2
  pt4 = tPSevent.GetDecay(3);

  //The returned Lorentz vectors are automatically boosted into the frame
  //of the originally decaying fragment
  //NEED TO CONVERT BACK FROM GEV TO MEV

  //Set fragment info
  p->setEkin(0);
  p->setA(p->getA()-3);
  p->setm(pt1->M()*1000);
 // set fragment energy and direction
  p->setEtotal(pt1->E()*1000); /// \todo implement setLVec() to set particle data from 4-vector
  p->setThetaPhi(pt1->Theta(),pt1->Phi()); 
  fThetaMap[0]  = pt1->Theta();
  fPhiMap[0]    = pt1->Phi();
  fThetaMap2[0] = pt1->Theta();
  fPhiMap2[0]   = pt1->Phi();
  fThetaMap3[0] = pt1->Theta();
  fPhiMap3[0]   = pt1->Phi();

  
  // set neutron energy and direction
  StParticle pp;
  pp.setZ(0);
  pp.setQ(0);
  pp.setx(p->getx());     // same position
  pp.sety(p->gety()); 
  pp.setz(p->getz());     
  pp.sett(p->gett());     // same time
  pp.setEkin(0);
  pp.setA(1);
  pp.setm(pt2->M()*1000);
  pp.setEtotal(pt2->E()*1000);
  pp.setThetaPhi(pt2->Theta(),pt2->Phi()); 

  if(! newPart) ERR("Pointer newPart not set; Cannot add new particles");
  newPart->push_back(pp); 

  // set second neutron energy and direction
  StParticle ppp;   
  ppp.setA(1);
  ppp.setm(pt3->M()*1000);
  ppp.setZ(0);
  ppp.setQ(0);
  ppp.setx(p->getx());     // same position
  ppp.sety(p->gety()); 
  ppp.setz(p->getz());     
  ppp.sett(p->gett());     // same time
  ppp.setEkin(0);
  ppp.setEtotal(pt3->E()*1000);
  ppp.setThetaPhi(pt3->Theta(),pt3->Phi());

  if(! newPart) ERR("Pointer newPart not set; Cannot add new particles");
  newPart->push_back(ppp); 

 // set third neutron energy and direction
  StParticle p4;   
  p4.setA(1);
  p4.setm(pt4->M()*1000);
  p4.setZ(0);
  p4.setQ(0);
  p4.setx(p->getx());     // same position
  p4.sety(p->gety()); 
  p4.setz(p->getz());     
  p4.sett(p->gett());     // same time
  p4.setEkin(0);
  p4.setEtotal(pt4->E()*1000);
  p4.setThetaPhi(pt4->Theta(),pt4->Phi());

  if(! newPart) ERR("Pointer newPart not set; Cannot add new particles");
  newPart->push_back(p4); 

  DSV(p->getA());
  DSV(p->getZ());
  return 0;

}

int StReacStripEvap_4bodydecay::savedValue(size_t part_id, string name, double* v) {
  DSV(part_id);
  DSV(name);

  if (name == "exen" &&  (fExEnMap.find(part_id) != fExEnMap.end()) ) {
    *v = fExEnMap[part_id];
    DSV(*v);
    return 0;
  }
  if (name == "exen2" &&  (fExEnMap2.find(part_id) != fExEnMap2.end()) ) {
    *v = fExEnMap2[part_id];
    DSV(*v);
    return 0;
  }
  if (name == "exen3" &&  (fExEnMap3.find(part_id) != fExEnMap3.end()) ) {
    *v = fExEnMap3[part_id];
    DSV(*v);
    return 0;
  }
  if (name == "ph" &&  (fPhiMap.find(part_id) != fPhiMap.end()) ) {
    *v = fPhiMap[part_id];
    DSV(*v);
    return 0;
  }
  if (name == "ph2" &&  (fPhiMap2.find(part_id) != fPhiMap2.end()) ) {
    *v = fPhiMap2[part_id];
    DSV(*v);
    return 0;
  }
  if (name == "ph3" &&  (fPhiMap3.find(part_id) != fPhiMap3.end()) ) {
    *v = fPhiMap3[part_id];
    DSV(*v);
    return 0;
  }
  if (name == "th" &&  (fThetaMap.find(part_id) != fThetaMap.end()) ) {
    *v = fThetaMap[part_id];
    DSV(*v);
    return 0;
  }
  if (name == "th2" &&  (fThetaMap2.find(part_id) != fThetaMap2.end()) ) {
    *v = fThetaMap2[part_id];
    DSV(*v);
    return 0;
  }
  if (name == "th3" &&  (fThetaMap3.find(part_id) != fThetaMap3.end()) ) {
    *v = fThetaMap3[part_id];
    DSV(*v);
    return 0;
  }

  if (name == "pz" &&  (fPzMap.find(part_id) != fPzMap.end()) ) {
    *v = fPzMap[part_id];
    DSV(*v);
    return 0;
  }





  return 1;
}
////////////////////////4 body decay///////////////////////////////////////




//---------------------3 Neutron Sequential Decay-------------------------//
int StReacStripEvap3neutron::act(StParticle* p, vector<StParticle>* newPart) { //cout << "Welcome to StReacStripEvap3neutron. Enjoy your stay." << endl;

  //Setting up the two decay energies
  double exen     = 0; //Total Decay Energy
  double exen1    = 0; //1st Decay Energy
  double exen2    = 0; //2nd Decay Energy
  double exen3    = 0; //3rd Decay Energy
  //Create Varibles to give give directions to particles and Glauber Kick
  double cosTheta;     //Used to set the cos(Theta)
  double theta;        //Used to set the Theta
  double theta1;
  double phi;          //Used to set the Phi

  double di_ei = 0;
  double di_ek = 0;

  double exenTotal = -1;
  double exenDiNeutron = -1;

  // cout << "Check velocity, asymMom: " << fVelocityScale << ", " << fAsymMomentum << endl;

  //for volya 2n
  double erel = 0; //rel energy between 2 neutrons

  //Check that all necassery random number generators are found
  if (!fRngExEn)           ERR("Need fRngExEn");
  if (!fRngExEn1)          ERR("Need fRngExEn1");
  if (!fRngExEn2)          ERR("Need fRngExEn2");
  if (!fRngExEn3)	       ERR("Need fRngExEn3");
  if (!fRngUniform)        ERR("Need uniform random number generator");
  if (!fRngGaussGlauber)   ERR("Need gauss random number generator");
  if (!fRngLegendre)       ERR("Need legendre random number generator");


  if (fWhichEnergyDoIReconstruct == "Reconstruct3rdEnergy_FILLERJUNK"){// YOU NEED A MUCH MORE EFFICIENT WAY TO DO THIS!
	do {
		exen = fRngExEn->value();
  	} while (exen < 0);

	do {
		do {
		  exen1 = fRngExEn1->value(0,exen);
  		} while (exen1 < 0);
		do {
		  exen2 = fRngExEn2->value(0,exen-exen1);
		} while(exen2 < 0);

		exen3 = exen-exen1-exen2;
  	} while (exen3 < 0);
	
	DSV(exen1);
	DSV(exen2);
	DSV(exen3);
	fExEnMap[0]=exen1;
	fExEnMap2[0]=exen2;
    fExEnMap3[0]=exen3; 
  
   	} 
  else if (fWhichEnergyDoIReconstruct == "volya2n" || fWhichEnergyDoIReconstruct == "volya2n_corr"){ 


	do{
		exen1 = fRngExEn1->value();
		//exen1=0.01;
  	} 
	 while (exen1 < 0);
    //for volya + bw

    fRngExEn2->value2d(exen,erel);

    exen2 = 0.5 * (exen + erel);
    exen3 = 0.5 * (exen - erel);

    
    //printf("Inside StReactStripEvap2neutron, Volya2n: exen:%f erel:%f  exen2:%f  exen3:%f\n",exen,erel,exen2,exen3);
    //printf("%f\n",erel);
    


    DSV(exen2);
    DSV(exen3);
  	fExEnMap[0]=exen1;
	fExEnMap2[0]=exen2;
    fExEnMap3[0]=exen3;
    
 
  }

  else if(fWhichEnergyDoIReconstruct=="volyadineutron"){ 
      //for volya + bw
    		
            fRngExEn2->value2d(di_ei,di_ek); 
    
   	    	exenTotal = di_ei + di_ek;
    	    exenDiNeutron = di_ei;
		
	do{
        exen1 = fRngExEn1->value();	
    } 
	 while (exen1 < 0);
   
	DSV(exenTotal);
 	DSV(exenDiNeutron);
 	fExEnMap[0]=exen1; // thermal 1
  	fExEnMap2[0]=exenTotal; 
    fExEnMap3[0]=exenDiNeutron;

   } // end volya energy

  
  else {
	do{
		exen1 = fRngExEn1->value();
		//exen1=0.01;
  	} 
	 while (exen1 < 0);
	 do{
		exen2 = fRngExEn2->value();
		//exen2 = 10;
  	 }  
	 while (exen2 < 0);
	 do{
		exen3 = fRngExEn3->value();
		//exen3 = 0.01;
  	 }  
	 while (exen3 < 0);

	DSV(exen1);
	DSV(exen2);
        DSV(exen3);
	fExEnMap[0]=exen1;
	fExEnMap2[0]=exen2;
 	fExEnMap3[0]=exen3;    
      }

  // Temporarily set Edecay to ~0
  //exen1 = 0.0001;
  //exen2 = 0.0001;
  //exen3 = 0.0001;	


  //debugging stuff -> Decay Energies
  if (debugSwitch == "exen1")
	  cout << exen1 << endl;
  if (debugSwitch == "exen2")
	  cout << exen2 << endl;
  if (debugSwitch == "exen3")
	  cout << exen3 << endl;

  if (!p) ERR("Particle pointer not set.");

  // change particle, keep velocity the same
  //**zwk reduce beta after reaction following LISE++
  double beta = p->getBeta() * fVelocityScale;        // initial beta
  double A = p->getA();            // initial mass number
  p->setA(p->getA() - fNn - fNp);  // remove protons and neutrons
  p->setm(p->getA()*931.494028);        // set new mass
  p->setZ(p->getZ() - fNp);        // set new Z
  p->setQ(p->getZ());              // set new Q
  p->setBeta(beta);                // set velocity

  TLorentzVector tl;
  tl.SetPxPyPzE(p->getPx(), p->getPy(), p->getPz(), p->getEtotal());
  DSV(tl.Theta());
  DSV(tl.Phi());
  DSV(tl.E() - tl.M());

  fPzMap[0]=-1;

  // Glauber momentum kick 
  double sig, sig0;
  sig0 = 90;  // MeV/c
  sig = p->getA() * ( fNn + fNp ) / ( A - 1);
  if (sig < 0) ERR("sig < 0; sig = %g", sig);
  sig = fGlauberKickScale * sig0 * sqrt(sig);
  DSV(sig);
  
  cosTheta = fRngUniform->value(-1,1);// cos(theta)
  theta    = acos(cosTheta);
  phi      = fRngUniform->value(0,2.*M_PI);  // phi
  DSV(theta);
  DSV(phi);

  fRngGaussGlauber->setCentroid(0);
  fRngGaussGlauber->setSigma(sig);

  double glaubP = fRngGaussGlauber->value();

  //-=-=-==--=-=-=-==
  //idea to create tail in mom. distribution following LISE++, zwk
  if(glaubP < 0) glaubP = glaubP * fAsymMomentum;
  //--------------------

  DSV(glaubP);
  TVector3 vGlaubP(0,0,glaubP);
  vGlaubP.SetTheta(theta);
  vGlaubP.SetPhi(phi);
  
  TVector3 vP;                  // total momentum
  vP = tl.Vect() + vGlaubP;
  tl.SetVectM(vP,p->getm());

  // Debugging
  //cout << "=========================================================================================" << endl;
  //cout << "POST GLAUBER, PRE DECAY: " << tl.Px() << " | " << tl.Py() << " | " << tl.Pz() << " | " << tl.E() <<  " | " << p->getA() << endl;
  //fprintf(pFile, "%f \t %f \t %f \t %f \t %i \n" ,tl.Px() ,tl.Py() ,tl.Pz() ,tl.E(), p->getA() );


  DSV(tl.Theta());
  DSV(tl.Phi());
  DSV(tl.E() - tl.M());


  //debugging stuff
  if (debugSwitch == "thetaLABP")
	  cout << tl.Theta()<< endl;
  if (debugSwitch == "phiLABP")
	  cout << tl.Phi()<< endl;

  TVector3 t3Boost;
  t3Boost = tl.BoostVector();  // boost vector to post glauber, pre-decay (lab)

  // start of neutron evaporation
  p->setA(p->getA() - 1);
  p->setm(p->getA()*931.494028);
  p->setEkin(0);             

  TLorentzVector tl1;  // particle 1 (heavy fragment)
  tl1.SetPxPyPzE(p->getPx(), p->getPy(), p->getPz(), p->getEtotal());

  // create neutron
  StParticle pp;   
  pp.setA(1);
  pp.setm(pp.getA()*931.494);//neutron = 939.565346
  pp.setZ(0);
  pp.setQ(0);
  pp.setx(p->getx());     // same position
  pp.sety(p->gety()); 
  pp.setz(p->getz());     
  pp.sett(p->gett());     // same time
  pp.setEkin(0);
  
  TLorentzVector tl2;  // particle 2 (neutron)
  tl2.SetPxPyPzE(pp.getPx(), pp.getPy(), pp.getPz(), pp.getEtotal());
  
  double eN, eF;   // total neutron and fragment energy
  double eCM;   // total CM energy

  eCM = pp.getm() + p->getm() + exen1;  // total CM energy
  eN = gsl_pow_2(eCM) + gsl_pow_2(pp.getm()) - gsl_pow_2(p->getm());
  eN = eN/(2.*eCM);                    // total energy neutron
  eF = gsl_pow_2(eCM) - gsl_pow_2(pp.getm()) + gsl_pow_2(p->getm());
  eF = eF/(2.*eCM);                    // total energy of fragment

  /// \todo check is arg of sqrt is positive
  double pN,pF; // fragment and neutron momentum
  pN = eN*eN - gsl_pow_2(pp.getm());
  pN = sqrt(pN);
  pF = eF*eF - gsl_pow_2(p->getm());
  if (pF < 0)
    pF = 0;
  else 
    pF = sqrt(pF);
  pF = -pF;  // fragment goes in opposite direction

  tl1.SetPxPyPzE(0,0,pF,eF);
  tl2.SetPxPyPzE(0,0,pN,eN);
  
  // set theta and phi for the first decay to some random value
  cosTheta = fRngUniform->value(-1,1);       // cos(theta)
  theta    = acos(cosTheta);
  phi      = fRngUniform->value(0,2.*M_PI);  // phi
  fThetaMap[0] = theta;
  fPhiMap[0]   = phi;

  tl1.SetTheta(theta); // Set fragment angle
  tl1.SetPhi(phi); // Set fragment phi angle
  tl2.SetTheta(M_PI - theta); // Set neutron angle 180-theta
  tl2.SetPhi(phi + M_PI); // set neutron phi angle

  TVector3 t4Boost;
  t4Boost = tl1.BoostVector();  // boost vector for 24O
 
    // Debugging

  //tl1.Boost(t3Boost);
  //tl2.Boost(t3Boost);

  //cout << " AFTER 1N EVAPORATION:" << endl;
  //cout << "TOTAL: " << tl1.Px() + tl2.Px() << " | " << tl1.Py() + tl2.Py() << " | " << tl1.Pz() + tl2.Pz() << " | " << tl1.E() + tl2.E() <<  endl;
 
  //tl1.Boost(-t3Boost);
  //tl2.Boost(-t3Boost);


if(fWhichEnergyDoIReconstruct=="volyadineutron"){ 

   // start of di-neutron evaporation
    p->setA(p->getA() - 2);
    p->setm(p->getA()*931.494028);
    p->setEkin(0);             

   // create di-neutron
   StParticle dineutron;   
   dineutron.setA(2);
   dineutron.setm(dineutron.getA()*931.494);//939.565346, 1 amu = 931.454
   dineutron.setZ(0);
   dineutron.setQ(0);
   dineutron.setx(p->getx());     // same position
   dineutron.sety(p->gety()); 
   dineutron.setz(p->getz());     
   dineutron.sett(p->gett());     // same time
   dineutron.setEkin(0);
  
   TLorentzVector tl4;  // di-neutron
   tl4.SetPxPyPzE(dineutron.getPx(), dineutron.getPy(), dineutron.getPz(), dineutron.getEtotal());

 
  
  double e2N_d, eF_d;   // total neutron and fragment energy
  double eCM_d;   // total CM energy

  //Make sure don't have negative decay energies.
  //Need to develop how we choose dineutron and frag-2n energies.
  double exen12_left = exenTotal - exenDiNeutron;
  if(exen12_left <= 0){
    cerr << "ExEn Left is less than ZERO!!" << endl;
  }
  eCM_d = dineutron.getm() + p->getm() + (exen12_left);  // total CM energy 
  e2N_d = gsl_pow_2(eCM_d) + gsl_pow_2(dineutron.getm()) - gsl_pow_2(p->getm());
  e2N_d = e2N_d/(2.*eCM_d);                    // total energy neutron
  eF_d = gsl_pow_2(eCM_d) - gsl_pow_2(dineutron.getm()) + gsl_pow_2(p->getm());
  eF_d = eF_d/(2.*eCM_d);                    // total energy of fragment

  /// \todo check is arg of sqrt is positive
  double p2N_d,pF_d; // fragment and neutron momentum
  p2N_d = e2N_d*e2N_d - gsl_pow_2(dineutron.getm());
  p2N_d = sqrt(p2N_d);
  pF_d = eF_d*eF_d - gsl_pow_2(p->getm());
  if (pF_d < 0)
    pF_d = 0;
  else 
    pF_d = sqrt(pF_d);
  pF_d = -pF_d;  // fragment goes in opposite direction

  tl1.SetPxPyPzE(0,0,pF_d,eF_d); //cout << "pF_d " << pF_d << endl;
  tl4.SetPxPyPzE(0,0,p2N_d,e2N_d);
  
  // set theta and phi for the first decay to some random value
  cosTheta = fRngUniform->value(-1,1);       // cos(theta)
  theta    = acos(cosTheta);
  phi      = fRngUniform->value(0,2.*M_PI);  // phi
  //fThetaMap[0] = theta;
  //fPhiMap[0]   = phi;  
  fThetaMap2[0] = theta;
  fPhiMap2[0]   = phi;

  tl1.SetTheta(theta); // Set fragment angle
  tl1.SetPhi(phi); // Set fragment phi angle
  tl4.SetTheta(M_PI - theta); // Set di-neutron angle 180-theta
  tl4.SetPhi(phi + M_PI); // set di-neutron phi angle

  TVector3 t6Boost;
  t6Boost = tl4.BoostVector();  // boost vector for di-neutron


  // Debugging
 /* tl1.Boost(t4Boost); tl1.Boost(t3Boost);
  tl2.Boost(t3Boost);
  tl4.Boost(t4Boost); tl4.Boost(t3Boost);

  cout << "After di-neutron peels off " << endl;
  cout << "TOTAL: " << tl1.Px() + tl2.Px() +tl4.Px() << " | " << tl1.Py() + tl2.Py() + tl4.Py() << " | " << tl1.Pz() + tl2.Pz() + tl4.Pz() << " | " << tl1.E() + tl2.E() +tl4.E() <<  endl;

  tl1.Boost(-t3Boost); tl1.Boost(-t4Boost);
  tl2.Boost(-t3Boost);
  tl4.Boost(-t3Boost); tl4.Boost(-t4Boost);*/



  // start of the di-neutron decay 
  dineutron.setA(dineutron.getA()-1);
  dineutron.setm(931.494);//939.565346
  dineutron.setEkin(0);               


  // create 4th neutron
  StParticle p4;   
  p4.setA(1);
  p4.setm(p4.getA()*931.494);//938.565346
  p4.setZ(0);
  p4.setQ(0);
  p4.setx(p4.getx());     // same position
  p4.sety(p4.gety()); 
  p4.setz(p4.getz());     
  p4.sett(p4.gett());     // same time
  p4.setEkin(0);
  
  TLorentzVector tl5;  // neutron from dineutron
  tl5.SetPxPyPzE(p4.getPx(), p4.getPy(), p4.getPz(), p4.getEtotal());

  double eN_1, eN_2;   // total neutron and fragment energy
  double eCM2;       // total CM energy

  eCM2 = p4.getm() + dineutron.getm() + exenDiNeutron;  // total CM energy
  eN_2 = gsl_pow_2(eCM2) + gsl_pow_2(p4.getm()) - gsl_pow_2(dineutron.getm());
  eN_2 = eN_2/(2.*eCM2);                    // total energy neutron
  eN_1 = gsl_pow_2(eCM2) - gsl_pow_2(p4.getm()) + gsl_pow_2(dineutron.getm());
  eN_1 = eN_1/(2.*eCM2);                    // total energy of fragment


  /// \todo check is arg of sqrt is positive
  double pN_2, pN_1; // fragment and neutron momentum
  pN_2 = eN_2*eN_2 - gsl_pow_2(p4.getm()); //cout << "pN_2" << pN_2 << endl;
  pN_2 = sqrt(pN_2); //cout << "sqrt(pN_2)" << sqrt(pN_2) << endl;
  pN_1 = eN_1*eN_1 - gsl_pow_2(dineutron.getm());
  if (pN_1 < 0)
    pN_1 = 0;
  else 
    pN_1 = sqrt(pN_1);
  pN_1 = -pN_1;  // fragment goes in opposite direction

  tl4.SetPxPyPzE(0,0,pN_1,eN_1);
  tl5.SetPxPyPzE(0,0,pN_2,eN_2);
  
  // set theta and phi to some random value
  cosTheta = fRngUniform->value(-1,1);       // cos(theta)
  theta    = acos(cosTheta);
  phi      = fRngUniform->value(0,2.*M_PI);  // phi
  //fThetaMap2[0] = theta;
  //fPhiMap2[0]   = phi;
  fThetaMap3[0] = theta;
  fPhiMap3[0]   = phi;


  tl4.SetTheta(theta);        // Set neutron3 angle
  tl4.SetPhi(phi);            // Set neutron3 phi angle
  tl5.SetTheta(M_PI - theta); // Set neutron4 angle 180-theta
  tl5.SetPhi(phi + M_PI);     // Set neutron4 phi angle

  //------------------------------------BOOSTS---------------------------------
  // Sanity list, thanks to perpetuating this god damn stupid notation
  // 	    	|  StParticle	|	Boost		| Lorentz Vector
  //  FRAGMENT	|	p	        |	t3 (to lab)	|  	tl1
  //	1n  	|	pp	        |	t4 (to 1n)	|	tl2
  // =============================== doesn't exist in 3n ======================
  //	di-n	|   dineutron   | 	t6 (to di-n)|	tl4
  //    4n	    |	p4	        |		    	|	tl5

  // This is the correct order of boosts, t6 goes to rest frame of deuteron NOT fragment (unlike sequential)
  // E and P are conserved when exen12_left->0, glaub =0, 1 amu = 1 neutron mass
  
  //Boosting into the frame where the fragment is at rest after the first 1n decay, t3Boost is to the lab.
  //// momentum check 8/13/13
  //tl1.Boost(t4Boost); tl1.Boost(t3Boost);
  //tl2.Boost(t3Boost);
  //tl4.Boost(t6Boost); tl4.Boost(t4Boost); tl4.Boost(t3Boost);
  //tl5.Boost(t6Boost); tl5.Boost(t4Boost); tl5.Boost(t3Boost);

 
  //Boosting all 5 particles to the lab frame
  //tl1.Boost(t3Boost);
  //tl2.Boost(t3Boost);
  //tl3.Boost(t3Boost);
  //tl4.Boost(t3Boost);
  //tl5.Boost(t3Boost);


  
  //Boosting into the frame where the fragment is at rest after the first 1n decay
  // test to find out why edecay is weird
  tl1.Boost(t4Boost); 
  //Must boost each neutron back to the frame where fragment is at rest after 1st decay
  //tl4.Boost(t6Boost); 
  tl4.Boost(t6Boost); tl4.Boost(t4Boost); // Here we need 2 boosts. 
  tl5.Boost(t6Boost); tl5.Boost(t4Boost);

 
  //Boosting all 5 particles to the lab frame
  tl1.Boost(t3Boost);
  tl2.Boost(t3Boost);
  tl4.Boost(t3Boost);
  tl5.Boost(t3Boost);


  /*cout << "================ AFTER DINEUTRON EVAPORTATION =======================" << endl;
  printf("Fragment:  %f | %f | %f | %f  | %i \n" ,tl1.Px() ,tl1.Py() ,tl1.Pz() ,tl1.E(), p->getA() );
  printf("1n:        %f | %f | %f | %f  | %i \n" ,tl2.Px() ,tl2.Py() ,tl2.Pz() ,tl2.E(), pp.getA() );
  //printf("2n:        %f | %f | %f | %f  | %i \n" ,tl3.Px() ,tl3.Py() ,tl3.Pz() ,tl3.E(), ppp.getA() );
  printf("Dineutron  %f | %f | %f | %f  | %i \n" ,tl4.Px() ,tl4.Py() ,tl4.Pz() ,tl4.E(), dineutron.getA() );
  printf("4n:        %f | %f | %f | %f  | %i \n" ,tl5.Px() ,tl5.Py() ,tl5.Pz() ,tl5.E(), p4.getA() );

  printf("Total:     %f | %f | %f | %f  \n" ,tl1.Px() +tl2.Px() + tl4.Px() + tl5.Px(), tl1.Py() + tl2.Py() + tl4.Py() + tl5.Py() , tl1.Pz() + tl2.Pz() + tl4.Pz() + tl5.Pz()  ,tl1.E() + tl2.E() + tl4.E() + tl5.E() );
*/
  //fprintf(pFile1," %f \t %f \t %f \t %f \n", tl1.Px() +tl2.Px() + tl4.Px() + tl5.Px(), tl1.Py() + tl2.Py() + tl4.Py() + tl5.Py() , tl1.Pz() + tl2.Pz() + tl4.Pz() + tl5.Pz()  ,tl1.E() + tl2.E() + tl4.E() + tl5.E() );


  // set fragment energy and direction
  p->setEtotal(tl1.E()); /// \todo implement setLVec() to set particle data from 4-vector
  p->setThetaPhi(tl1.Theta(),tl1.Phi()); 

  // set 1n energy and direction
  pp.setEtotal(tl2.E());
  pp.setThetaPhi(tl2.Theta(),tl2.Phi());
  if(! newPart) ERR("Pointer newPart not set; Cannot add new particles");
  newPart->push_back(pp); 


  // set 2n energy and direction
  //ppp.setEtotal(tl3.E());
  //ppp.setThetaPhi(tl3.Theta(),tl3.Phi());
  //if(! newPart) ERR("Pointer newPart not set; Cannot add new particles");
  //newPart->push_back(ppp); 

   
  // set neutron energy and direction
  dineutron.setEtotal(tl4.E());
  dineutron.setThetaPhi(tl4.Theta(),tl4.Phi()); 

  if(! newPart) ERR("Pointer newPart not set; Cannot add new particles");
  newPart->push_back(dineutron); 

  // set second neutron energy and direction
  p4.setEtotal(tl5.E());
  p4.setThetaPhi(tl5.Theta(),tl5.Phi());

  if(! newPart) ERR("Pointer newPart not set; Cannot add new particles");
  newPart->push_back(p4); 

  }
  //-------------------------------------------------------------------------------------------------------
  // End di neutron decay




else{ 
  // start of the second neutron evaporation
  p->setA(p->getA()-1);
  p->setm(p->getA()*931.494028);
  p->setEkin(0);               // 23O at rest

  // create second neutron
  StParticle ppp;   
  ppp.setA(1);
  ppp.setm(pp.getA()*931.494028);//939.565346
  ppp.setZ(0);
  ppp.setQ(0);
  ppp.setx(p->getx());     // same position
  ppp.sety(p->gety()); 
  ppp.setz(p->getz());     
  ppp.sett(p->gett());     // same time
  ppp.setEkin(0);
  
  TLorentzVector tl3;  // particle 3 (neutron)
  tl3.SetPxPyPzE(ppp.getPx(), ppp.getPy(), ppp.getPz(), ppp.getEtotal());

  double eN2, eF2;   // total neutron and fragment energy
  double eCM2;       // total CM energy

  eCM2 = ppp.getm() + p->getm() + exen2;  // total CM energy
  eN2 = gsl_pow_2(eCM2) + gsl_pow_2(ppp.getm()) - gsl_pow_2(p->getm());
  eN2 = eN2/(2.*eCM2);                    // total energy neutron
  eF2 = gsl_pow_2(eCM2) - gsl_pow_2(ppp.getm()) + gsl_pow_2(p->getm());
  eF2 = eF2/(2.*eCM2);                    // total energy of fragment

  /// \todo check is arg of sqrt is positive
  double pN2, pF2; // fragment and neutron momentum
  pN2 = eN2*eN2 - gsl_pow_2(ppp.getm());
  pN2 = sqrt(pN2);
  pF2 = eF2*eF2 - gsl_pow_2(p->getm());
  if (pF2 < 0)
    pF2 = 0;
  else 
    pF2 = sqrt(pF2);
  pF2 = -pF2;  // fragment goes in opposite direction

  tl1.SetPxPyPzE(0,0,pF2,eF2);
  tl3.SetPxPyPzE(0,0,pN2,eN2);
  
  // set theta and phi to some random value
  cosTheta = fRngUniform->value(-1,1);       // cos(theta)
  theta1    = acos(cosTheta);
  phi      = fRngUniform->value(0,2.*M_PI);  // phi
  fThetaMap2[0] = theta1;
  fPhiMap2[0]   = phi;
  tl1.SetTheta(theta1);        // Set fragment angle
  tl1.SetPhi(phi);            // Set fragment phi angle
  tl3.SetTheta(M_PI - theta1); // Set neutron angle 180-theta
  tl3.SetPhi(phi + M_PI);     // Set neutron phi angle

  TVector3 t5Boost;
  t5Boost = tl1.BoostVector(); 

  // start of the third neutron evaporation
  p->setA(p->getA()-1);
  p->setm(p->getA()*931.494028);
  p->setEkin(0);               // 23O at rest


// create third neutron
// this notation is stupid@!
  StParticle pppp;   
  pppp.setA(1);
  pppp.setm(pp.getA()*931.494028);//939.565346
  pppp.setZ(0);
  pppp.setQ(0);
  pppp.setx(p->getx());     // same position
  pppp.sety(p->gety()); 
  pppp.setz(p->getz());     
  pppp.sett(p->gett());     // same time
  pppp.setEkin(0);
  
  TLorentzVector tl4;  // particle 4 (neutron)
  tl4.SetPxPyPzE(pppp.getPx(), pppp.getPy(), pppp.getPz(), pppp.getEtotal());

  double eN3, eF3;   // total neutron and fragment energy
  double eCM3;       // total CM energy

  eCM3 = pppp.getm() + p->getm() + exen3;  // total CM energy
  eN3 = gsl_pow_2(eCM3) + gsl_pow_2(pppp.getm()) - gsl_pow_2(p->getm());
  eN3 = eN3/(2.*eCM3);                    // total energy neutron
  eF3 = gsl_pow_2(eCM3) - gsl_pow_2(pppp.getm()) + gsl_pow_2(p->getm());
  eF3 = eF3/(2.*eCM3);                    // total energy of fragment

  /// \todo check is arg of sqrt is positive
  double pN3, pF3; // fragment and neutron momentum
  pN3 = eN3*eN3 - gsl_pow_2(pppp.getm());
  pN3 = sqrt(pN3);
  pF3 = eF3*eF3 - gsl_pow_2(p->getm());
  if (pF3 < 0)
    pF3 = 0;
  else 
    pF3 = sqrt(pF3);
  pF3 = -pF3;  // fragment goes in opposit direction

  tl1.SetPxPyPzE(0,0,pF3,eF3);
  tl4.SetPxPyPzE(0,0,pN3,eN3);
  
  // set theta and phi to some random value
  
    
  if(fWhichEnergyDoIReconstruct == "volya2n_corr"){
        cosTheta = fRngLegendre->value(-1,1);
        //theta = acos(cosTheta);
        if(acos(cosTheta) > theta1)       theta    = acos(cosTheta) - theta1;
        else if(acos(cosTheta) <= theta1) theta    = theta1 - acos(cosTheta);// Correlate the 3rd and 4th neutron
  }
  else {
        cosTheta = fRngUniform->value(-1,1);
        theta    = acos(cosTheta);
  }


  //cosTheta = fRngUniform->value(-1,1);       // cos(theta)
  //theta    = acos(cosTheta);
  
  phi      = fRngUniform->value(0,2.*M_PI);  // phi
  fThetaMap3[0] = theta;
  fPhiMap3[0]   = phi;
  tl1.SetTheta(theta);        // Set fragment angle
  tl1.SetPhi(phi);            // Set fragment phi angle
  tl4.SetTheta(M_PI - theta); // Set neutron angle 180-theta
  tl4.SetPhi(phi + M_PI);     // Set neutron phi angle

  //Boosting into the frame where the fragment is at rest after the first decay
  tl1.Boost(t5Boost); tl1.Boost(t4Boost); 
  tl3.Boost(t4Boost);//Must boost each neutron back to the frame where fragment is at rest after 1st decay
  tl4.Boost(t5Boost); tl4.Boost(t4Boost); // Here we need 2 boosts. 
  //Boosting all four particles to the lab frame
  tl1.Boost(t3Boost);
  tl2.Boost(t3Boost);
  tl3.Boost(t3Boost);
  tl4.Boost(t3Boost);

  // set fragment energy and direction
  p->setEtotal(tl1.E()); /// \todo implement setLVec() to set particle data from 4-vector
  p->setThetaPhi(tl1.Theta(),tl1.Phi()); 
  
  // set neutron energy and direction
  pp.setEtotal(tl2.E());
  pp.setThetaPhi(tl2.Theta(),tl2.Phi()); 

  if(! newPart) ERR("Pointer newPart not set; Cannot add new particles");
  newPart->push_back(pp); 

  // set second neutron energy and direction
  ppp.setEtotal(tl3.E());
  ppp.setThetaPhi(tl3.Theta(),tl3.Phi());

  if(! newPart) ERR("Pointer newPart not set; Cannot add new particles");
  newPart->push_back(ppp); 

  // set third neutron energy and direction
  pppp.setEtotal(tl4.E());
  pppp.setThetaPhi(tl4.Theta(),tl4.Phi());

  if(! newPart) ERR("Pointer newPart not set; Cannot add new particles");
  newPart->push_back(pppp); 

  /* cout << "FRAGMENT: " << tl1.Px() << " | " << tl1.Py() << " | " << tl1.Pz() << " | " << tl1.E() << endl;
   cout << "NEUTRON1: " << tl2.Px() << " | " << tl2.Py() << " | " << tl2.Pz() << " | " << tl2.E() << endl;
   cout << "NEUTRON2: " << tl3.Px() << " | " << tl3.Py() << " | " << tl3.Pz() << " | " << tl3.E() << endl;
   cout << "NEUTRON3: " << tl4.Px() << " | " << tl4.Py() << " | " << tl4.Pz() << " | " << tl4.E() << endl;
   cout << "TOTAL:    " << tl1.Px() + tl2.Px() + tl3.Px() + tl4.Px()  << " | " << tl1.Py() + tl2.Py() + tl3.Py() + tl4.Py()  << " | " << tl1.Pz() + tl2.Pz() + tl3.Pz() + tl4.Pz()  << " | " << tl1.E() + tl2.E() + tl3.E() + tl4.E() << endl;*/
  
   //fprintf(pFile1," %f \t %f \t %f \t %f \n", tl1.Px() +tl2.Px() + tl3.Px() + tl4.Px(), tl1.Py() + tl2.Py() + tl3.Py() + tl4.Py() , tl1.Pz() + tl2.Pz() + tl3.Pz() + tl4.Pz()  ,tl1.E() + tl2.E() + tl3.E() + tl4.E() );


}// end else for sequential 

  

  //  You were trying to figure out why 5% events are missing, check other branches besides exen
 // fprintf(pFile, "%f \t %f \t %f \t %f \t %i \n" , ,exen1 ,exen2 ,exen3, erel );


  DSV(p->getA());
  DSV(p->getZ());



  return 0;
}

int StReacStripEvap3neutron::savedValue(size_t part_id, string name, double* v) {
  DSV(part_id);
  DSV(name);
  if (name == "exen" &&  (fExEnMap.find(part_id) != fExEnMap.end()) ) {
    *v = fExEnMap[part_id];
    DSV(*v);
    return 0;
  }
  if (name == "exen2" &&  (fExEnMap2.find(part_id) != fExEnMap2.end()) ) {
    *v = fExEnMap2[part_id];
    DSV(*v);
    return 0;
  }
  if (name == "exen3" &&  (fExEnMap3.find(part_id) != fExEnMap3.end()) ) {
    *v = fExEnMap3[part_id];
    DSV(*v);
    return 0;
  }

  if (name == "ph" &&  (fPhiMap.find(part_id) != fPhiMap.end()) ) {
    *v = fPhiMap[part_id];
    DSV(*v);
    return 0;
  }
  if (name == "ph2" &&  (fPhiMap2.find(part_id) != fPhiMap2.end()) ) {
    *v = fPhiMap2[part_id];
    DSV(*v);
    return 0;
  }
  if (name == "ph3" &&  (fPhiMap3.find(part_id) != fPhiMap3.end()) ) {
    *v = fPhiMap3[part_id];
    DSV(*v);
    return 0;
  }
  if (name == "th" &&  (fThetaMap.find(part_id) != fThetaMap.end()) ) {
    *v = fThetaMap[part_id];
    DSV(*v);
    return 0;
  }
  if (name == "th2" &&  (fThetaMap2.find(part_id) != fThetaMap2.end()) ) {
    *v = fThetaMap2[part_id];
    DSV(*v);
    return 0;
  }
  if (name == "th3" &&  (fThetaMap3.find(part_id) != fThetaMap3.end()) ) {
    *v = fThetaMap3[part_id];
    DSV(*v);
    return 0;
  }

  if (name == "pz" &&  (fPzMap.find(part_id) != fPzMap.end()) ) {
    *v = fPzMap[part_id];
    DSV(*v);
    return 0;
  }
  return 1;
}// End 3N


int StReacStripEvap::act(StParticle* p, vector<StParticle>* newPart) {
  if (!p) ERR("Particle pointer not set.");

  //For momentum distribution
  TLorentzVector lvProj;
  lvProj.SetPxPyPzE(p->getPx(), p->getPy(), p->getPz(), p->getEtotal());

  // change particle, keep velocity the same  
  //**zwk reduce beta after reaction following LISE++
  double beta = p->getBeta() * fVelocityScale;        // initial beta
  double A = p->getA();            // initial mass number
  p->setA(p->getA() - fNn - fNp);  // remove protons and neutrons
  p->setm(p->getA()*931.494028);        // set new mass
  p->setZ(p->getZ() - fNp);        // set new Z
  p->setQ(p->getZ());              // set new Q
  p->setBeta(beta);                // set velocity

  TLorentzVector tl;
  tl.SetPxPyPzE(p->getPx(), p->getPy(), p->getPz(), p->getEtotal());
  DSV(tl.Theta());
  DSV(tl.Phi());
  DSV(tl.E() - tl.M());

  fPzMap[0]=-1;
  if(fUseParaMom){
    //---Parallel Mom. Distribution------
    //Get boost vector of projectile
    TVector3 vecProj = lvProj.BoostVector();
    //Into proj frame
    tl.Boost(-1*vecProj);
    double Pz = fRngParaMom->value();
    //cout << Pz << endl;
    fPzMap[0] = Pz;
    TVector3 fragP_frame (tl.Px(), tl.Py(), Pz);
    tl.SetVectM(fragP_frame, p->getm());
    //boost back to lab-frame
    tl.Boost(vecProj);
    //----------------------------
  }
  else{
    // Glauber momentum kick 
    double sig, sig0;
    sig0 = 90;  // MeV/c
    sig = p->getA() * abs( fNn + fNp ) / ( A - 1);
    if (sig < 0) ERR("sig < 0; sig = %g", sig);
    sig = fGlauberKickScale * sig0 * sqrt(sig);
    DSV(sig);
    
    if (!fRngUniform)      ERR("Need uniform random number generator");
    if (!fRngGaussGlauber) ERR("Need gauss random number generator");
    
    double cosTheta,theta,phi;                 // cos(theta), theta, phi
    cosTheta = fRngUniform->value(-1,1);// cos(theta)
    theta    = acos(cosTheta);
    phi      = fRngUniform->value(0,2.*M_PI);  // phi
    theta = 0.0; // Inserted for test CRH 1/24/08
    phi = 0.0; // Inserted for test CRH 1/24/08
    DSV(theta);
    DSV(phi);
    
    fRngGaussGlauber->setCentroid(0);
    fRngGaussGlauber->setSigma(sig);
    
    double glaubP = fRngGaussGlauber->value();
    //-=-=-==--=-=-=-==
    //idea to create tail in mom. distribution following LISE++, zwk
    if(glaubP < 0) glaubP = glaubP * fAsymMomentum;
    //--------------------

    DSV(glaubP);
    TVector3 vGlaubP(0,0,glaubP);
    vGlaubP.SetTheta(theta);
    vGlaubP.SetPhi(phi);

    TVector3 vP;                  // total momentum
    vP = tl.Vect() + vGlaubP;
    tl.SetVectM(vP,p->getm());

    fPzMap[0] = tl.Pz();
  }

  DSV(tl.Theta());
  DSV(tl.Phi());
  DSV(tl.E() - tl.M());

  //debugging stuff
  if (debugSwitch == "thetaLABP")
	  cout << tl.Theta()<< endl;
  //debugging stuff
  if (debugSwitch == "phiLABP")
	  cout << tl.Phi()<< endl;
  TVector3 t3Boost;
  t3Boost = tl.BoostVector();  // boost vector for 24O

  // start of neutron evaporation
  p->setA(p->getA() - 1);
  p->setm(p->getA()*931.494028);
  p->setEkin(0);               // 23O at rest

  TLorentzVector tl1;  // particle 1 (heavy fragment)
  tl1.SetPxPyPzE(p->getPx(), p->getPy(), p->getPz(), p->getEtotal());

  // create neutron
  StParticle pp;   
  pp.setA(1);
  pp.setm(pp.getA()*939.565346);
  pp.setZ(0);
  pp.setQ(0);
  pp.setx(p->getx());     // same position
  pp.sety(p->gety()); 
  pp.setz(p->getz());     
  pp.sett(p->gett());     // same time
  pp.setEkin(0);
  
  TLorentzVector tl2;  // particle 2 (neutron)
  tl2.SetPxPyPzE(pp.getPx(), pp.getPy(), pp.getPz(), pp.getEtotal());
  
  double eN, eF;   // total neutron and fragment energy
  double eCM;   // total CM energy
  double exen;  // excitation energy

  exen = 0;
  if (!fRngExEn) ERR("Need fRngExEn");
  do {
    exen = fRngExEn->value();
    // printf("Here ########## fRngExEn %f",exen); // Test CRH
  } while (exen < 0);
  DSV(exen);
  fExEnMap[0]=exen;

  //debugging stuff
  if (debugSwitch == "exen")
	  cout << exen<< endl;
  eCM = pp.getm() + p->getm() + exen;  // total CM energy
  eN = gsl_pow_2(eCM) + gsl_pow_2(pp.getm()) - gsl_pow_2(p->getm());
  eN = eN/(2.*eCM);                    // total energy neutron
  eF = gsl_pow_2(eCM) - gsl_pow_2(pp.getm()) + gsl_pow_2(p->getm());
  eF = eF/(2.*eCM);                    // total energy of fragment

  /// \todo check is arg of sqrt is positive
  double pN,pF; // fragment and neutron momentum
  pN = eN*eN - gsl_pow_2(pp.getm());
  pN = sqrt(pN);
  pF = eF*eF - gsl_pow_2(p->getm());
  pF = sqrt(pF);
  pF = -pF;  // fragment goes in opposit direction

  tl1.SetPxPyPzE(0,0,pF,eF);
  tl2.SetPxPyPzE(0,0,pN,eN);
  
  // set theta and phi to some random value
  if (!fRngUniform) ERR("Need random number generator");
  
  double cosTheta = fRngUniform->value(-1,1);       // cos(theta)
  double theta    = acos(cosTheta);
  theta = 0.0; // Test CRH
  double phi      = fRngUniform->value(0,2.*M_PI);  // phi
  phi = 0.0; // Test CRH
  fThetaMap[0] = theta;
  fPhiMap[0]   = phi;

  /* Write to file 
  ofstream angles;
  angles.open("/projects/mona-sim/baumann/angles1n.out",std::ofstream::out | std::ofstream::app);
//  angles << cosTheta << "     " << phi << endl;
  angles << theta << "     " << phi << endl;
  angles.close();
*/

  tl1.SetTheta(theta); // Set fragment angle
  tl2.SetTheta(M_PI - theta); // Set neutron angle 180-theta
  tl1.SetPhi(phi); // Set fragment phi angle
  tl2.SetPhi(phi + M_PI); // set neutron phi angle
  
  // go to the lab
  tl1.Boost(t3Boost);
  tl2.Boost(t3Boost);

  // set fragment energy and direction
  p->setEtotal(tl1.E()); /// \todo implement setLVec() to set particle data from 4-vector
  p->setThetaPhi(tl1.Theta(),tl1.Phi()); 
  
  // set neutron energy and direction
  pp.setEtotal(tl2.E());
  pp.setThetaPhi(tl2.Theta(),tl2.Phi()); 

  if(! newPart) ERR("Pointer newPart not set; Cannot add new particles");
  newPart->push_back(pp); 

  DSV(p->getA());
  DSV(p->getZ());
  return 0;
}

int StReacStripEvap::savedValue(size_t part_id, string name, double* v) {
  DSV(part_id);
  DSV(name);
  if (name == "exen" &&  (fExEnMap.find(part_id) != fExEnMap.end()) ) {
    *v = fExEnMap[part_id];
    DSV(*v);
    return 0;
  }
  if (name == "ph" &&  (fPhiMap.find(part_id) != fPhiMap.end()) ) {
    *v = fPhiMap[part_id];
    DSV(*v);
    return 0;
  }
  if (name == "th" &&  (fThetaMap.find(part_id) != fThetaMap.end()) ) {
    *v = fThetaMap[part_id];
    DSV(*v);
    return 0;
  }
  if (name == "pz" &&  (fPzMap.find(part_id) != fPzMap.end()) ) {
    *v = fPzMap[part_id];
    DSV(*v);
    return 0;
  }
  return 1;
}


//--------------Stripping, Knockout Reactions Only, Glaub_noDecay--------------

int StReacGlaub_noDecay::act(StParticle* p, vector<StParticle>* newPart) {
  if (!p) ERR("Particle pointer not set.");

  //For momentum distribution
  TLorentzVector lvProj;
  lvProj.SetPxPyPzE(p->getPx(), p->getPy(), p->getPz(), p->getEtotal());

  // change particle, keep velocity the same  
  //**zwk reduce beta after reaction following LISE++
  double beta = p->getBeta() * fVelocityScale;        // initial beta
  double A = p->getA();            // initial mass number
  p->setA(p->getA() - fNn - fNp);  // remove protons and neutrons
  p->setm(p->getA()*931.494028);        // set new mass
  p->setZ(p->getZ() - fNp);        // set new Z
  p->setQ(p->getZ());              // set new Q
  p->setBeta(beta);                // set velocity

  TLorentzVector tl;
  tl.SetPxPyPzE(p->getPx(), p->getPy(), p->getPz(), p->getEtotal());
  DSV(tl.Theta());
  DSV(tl.Phi());
  DSV(tl.E() - tl.M());

  fPzMap[0]=-1;


  // Glauber momentum kick 
  double sig, sig0;
  sig0 = 90;  // MeV/c
  sig = p->getA() * abs( fNn + fNp ) / ( A - 1);
  if (sig < 0) ERR("sig < 0; sig = %g", sig);
  sig = fGlauberKickScale * sig0 * sqrt(sig);
  DSV(sig);

  if (!fRngUniform)      ERR("Need uniform random number generator");
  if (!fRngGaussGlauber) ERR("Need gauss random number generator");

  double cosTheta,theta,phi;                 // cos(theta), theta, phi
  cosTheta = fRngUniform->value(-1,1);// cos(theta)
  theta    = acos(cosTheta);
  phi      = fRngUniform->value(0,2.*M_PI);  // phi
  DSV(theta);
  DSV(phi);
    
  fRngGaussGlauber->setCentroid(0);
  fRngGaussGlauber->setSigma(sig);
    
  double glaubP = fRngGaussGlauber->value();
  //-=-=-==--=-=-=-==
  //idea to create tail in mom. distribution following LISE++, zwk
  if(glaubP < 0) glaubP = glaubP * fAsymMomentum;
  //--------------------

  DSV(glaubP);
  TVector3 vGlaubP(0,0,glaubP);

  fPzMap[0] = tl.Pz();

  /*
  if(fUseParaMom){
    //---Parallel Mom. Distribution------
    //Get boost vector of projectile
    TVector3 vecProj = lvProj.BoostVector();
    //Into proj frame
    tl.Boost(-1*vecProj);
    double Pz = fRngParaMom->value();
    //cout << Pz << endl;
    fPzMap[0] = Pz;
    TVector3 fragP_frame (tl.Px(), tl.Py(), Pz);
    tl.SetVectM(fragP_frame, p->getm());
    //boost back to lab-frame
    tl.Boost(vecProj);
    //----------------------------
  }
*/
  //Transverse momentum kick parametrization based on Van Bibber et al., PRL 43:12,840-844 (1979) -JKS, 8 March 2013
  if(fUseParaMom){
  //-=-=-==--=-=-=-==
  //idea to create tail in mom. distribution following LISE++, zwk
    if(glaubP < 0) glaubP = glaubP * fAsymMomentum;
    //--------------------
    double sigperp, sigperp1, sigperp2, sig2;
    sig2 = 200;  // MeV/c
    sigperp1 = p->getA() * abs( fNn + fNp ) / ( A - 1);
    if (sigperp1 < 0) ERR("sigperp1 < 0; sigperp1 = %g", sigperp1);
    sigperp2 = p->getA() * (p->getA()-1) / (A*(A-1));
    if (sigperp2 < 0) ERR("sigperp2 < 0; sigperp2 = %g", sigperp2);
    sigperp = sqrt(fGlauberTransScale1 * fGlauberTransScale1 * sig0 * sig0 * sigperp1 + fGlauberTransScale2 * fGlauberTransScale2 * sig2 * sig2 * sigperp2);
    DSV(sigperp);

    double glaubx,glauby;
    fRngParaMom->setCentroid(0);
    fRngParaMom->setSigma(sigperp);
    glaubx = fRngParaMom->value();
    glauby = fRngParaMom->value();
    vGlaubP.SetXYZ(glaubx,glauby,glaubP);
  }

//  vGlaubP.SetTheta(theta);
//  vGlaubP.SetPhi(phi);

  TVector3 vP;                  // total momentum
  vP = tl.Vect() + vGlaubP;
  tl.SetVectM(vP,p->getm());



  DSV(tl.Theta());
  DSV(tl.Phi());
  DSV(tl.E() - tl.M());

  //debugging stuff
  if (debugSwitch == "thetaLABP")
	  cout << tl.Theta()<< endl;
  //debugging stuff
  if (debugSwitch == "phiLABP")
    cout << tl.Phi()<< endl;
 
  double exen = 0;
  DSV(exen);
  fExEnMap[0]=exen;

  fThetaMap[0] = tl.Theta();
  fPhiMap[0]   = tl.Phi();

  // set fragment energy and direction
  p->setEtotal(tl.E()); /// \todo implement setLVec() to set particle data from 4-vector
  p->setThetaPhi(tl.Theta(),tl.Phi()); 
  
  //zwk, create a "fake" neutron for the code
  StParticle pp;   
  pp.setA(1);
  pp.setm(pp.getA()*939.565346);
  pp.setZ(0);
  pp.setQ(0);
  pp.setx(p->getx());     // same position
  pp.sety(p->gety()); 
  pp.setz(p->getz());     
  pp.sett(p->gett());     // same time
  pp.setEkin(0);
  if(! newPart) ERR("Pointer newPart not set; Cannot add new particles");
  newPart->push_back(pp); 

  DSV(p->getA());
  DSV(p->getZ());
  return 0;
}

int StReacGlaub_noDecay::savedValue(size_t part_id, string name, double* v) {
  DSV(part_id);
  DSV(name);
  if (name == "exen" &&  (fExEnMap.find(part_id) != fExEnMap.end()) ) {
    *v = fExEnMap[part_id];
    DSV(*v);
    return 0;
  }
  if (name == "ph" &&  (fPhiMap.find(part_id) != fPhiMap.end()) ) {
    *v = fPhiMap[part_id];
    DSV(*v);
    return 0;
  }
  if (name == "th" &&  (fThetaMap.find(part_id) != fThetaMap.end()) ) {
    *v = fThetaMap[part_id];
    DSV(*v);
    return 0;
  }
  if (name == "pz" &&  (fPzMap.find(part_id) != fPzMap.end()) ) {
    *v = fPzMap[part_id];
    DSV(*v);
    return 0;
  }
  return 1;
}
//////////////////////////////////////////////////////////////////////
// ==========================================================
//          TWO NEUTRON REACTION CLASSES:
//          StripEvap_DiNeutron, StripEvap2Neutron, 3body
// ==========================================================
//////////////////////////////////////////////////////////////////////


//---------------------2 Neutron Sequential Decay-------------------------//
int StReacStripEvap2neutron::act(StParticle* p, vector<StParticle>* newPart) {

  //Setting up the two decay energies
  double exen     = 0; //Total Decay Energy
  double exen1    = 0; //1st Decay Energy
  double exen2    = 0; //2nd Decay Energy
  //Create Varibles to give give directions to particles and Glauber Kick
  double cosTheta;     //Used to set the cos(Theta)
  double theta;        //Used to set the Theta
  double theta2;       // Theta of second neutron
  double phi;          //Used to set the Phi

  // cout << "Check velocity, asymMom: " << fVelocityScale << ", " << fAsymMomentum << endl;

  //for volya 2n
  double erel = 0; //rel energy between 2 neutrons

  //Check that all necassery random number generators are found
  if (!fRngExEn)           ERR("Need fRngExEn");
  if (!fRngExEn1)          ERR("Need fRngExEn1");
  if (!fRngExEn2)          ERR("Need fRngExEn2");
  if (!fRngUniform)        ERR("Need uniform random number generator");
  if (!fRngGaussGlauber)   ERR("Need gauss random number generator");
  if (!fRngLegendre)       ERR("Need legendre random number generator");

  //Determine the first and second decay energies
  if (fWhichEnergyDoIReconstruct == "volya2n"){

    //for volya + bw
    fRngExEn->value2d(exen,erel);

    exen1 = 0.5 * (exen + erel);
    exen2 = 0.5 * (exen - erel);

    //printf("Inside StReactStripEvap2neutron, Volya2n: exen:%f erel:%f  exen1:%f  exen2:%f\n",exen,erel,exen1,exen2);
    //printf("%f\n",erel);

    DSV(exen1);
    DSV(exen2);
    fExEnMap[0]=exen1;
    fExEnMap2[0]=exen2;
  }
  else if (fWhichEnergyDoIReconstruct == "TwoNeutron_seq")
    {
	  do {
	    exen2 = fRngExEn2->value();
	  } while (exen2 < 0);
	  do {
	    do {
	      exen = fRngExEn->value(0,exen2);    
	    } while (exen < 0);
	    exen1 = exen-exen2; 
	  } while (exen1 < 0);
	  DSV(exen1); //cout << " I made it to the end: " << exen1 << " " << exen2 << endl;
	  DSV(exen2);
	  fExEnMap[0]=exen1;
	  fExEnMap2[0]=exen2;
   	} 




	else if (fWhichEnergyDoIReconstruct == "cos_1st")
    {
      do {
	exen = fRngExEn->value();
      } while (exen < 0);
      do {
	do {
	  exen2 = exen*fRngExEn2->value();
	} while (exen2 < 0);
      		exen1 = exen-exen2;
  	} while (exen1 < 0);
      	DSV(exen1);
      	DSV(exen2);
      	fExEnMap[0]=exen1;
      	fExEnMap2[0]=exen2;
   	} else if (fWhichEnergyDoIReconstruct == "cos_2nd")
   	{
	do {
      		exen = fRngExEn->value();
  	} while (exen < 0);
	do {
		do {
	      		exen1 = exen*fRngExEn1->value();
  		} while (exen1 < 0);
      		exen2 = exen-exen1;
  	} while (exen2 < 0);
      	DSV(exen1);
      	DSV(exen2);
      	fExEnMap[0]=exen1;
      	fExEnMap2[0]=exen2;
   	} else if (fWhichEnergyDoIReconstruct == "Reconstruct1stEnergy")
   	{
	do {
      		exen = fRngExEn->value();
  	} while (exen < 0);
	do {
		do {
	      		exen2 = fRngExEn2->value();
  		} while (exen2 < 0);
      		exen1 = exen-exen2;
		//printf("exen:%f exen1:%f exen2:%f\n",exen,exen1,exen2);
  	} while (exen1 < 0);
      	DSV(exen1);
      	DSV(exen2);
      	fExEnMap[0]=exen1;
      	fExEnMap2[0]=exen2;
   	} else if (fWhichEnergyDoIReconstruct == "Reconstruct2ndEnergy")
      	{
	do {
		exen = fRngExEn->value();
  	} while (exen < 0);
	do {
		do {
		  exen1 = fRngExEn1->value();
  		} while (exen1 < 0);
		exen2 = exen-exen1;
  	} while (exen2 < 0);
	DSV(exen1);
	DSV(exen2);
	fExEnMap[0]=exen1;
	fExEnMap2[0]=exen2;   
   	} else {
	do {
		exen1 = fRngExEn1->value();
  	} while (exen1 < 0);
	do {
		exen2 = fRngExEn2->value();
  	} while (exen2 < 0);
	DSV(exen1);
	DSV(exen2);
	fExEnMap[0]=exen1;
	fExEnMap2[0]=exen2;    
      }

  //Temp added for looknig at efficiency - zwk
  //exen1 = 0.5 * exen;
  //exen2 = exen - exen1;
  //fExEnMap[0] = exen1;
  //fExEnMap2[0] = exen2;
  //cout << exen << " " << exen1 << " " << exen2 << endl;


  //debugging stuff -> Decay Energies
  if (debugSwitch == "exen1")
	  cout << exen1<< endl;
  if (debugSwitch == "exen2")
	  cout << exen2<< endl;

  if (!p) ERR("Particle pointer not set.");

  // change particle, keep velocity the same
  //**zwk reduce beta after reaction following LISE++
  double beta = p->getBeta() * fVelocityScale;        // initial beta
  double A = p->getA();            // initial mass number
  p->setA(p->getA() - fNn - fNp);  // remove protons and neutrons
  p->setm(p->getA()*931.494028);        // set new mass
  p->setZ(p->getZ() - fNp);        // set new Z
  p->setQ(p->getZ());              // set new Q
  p->setBeta(beta);                // set velocity

  TLorentzVector tl;
  tl.SetPxPyPzE(p->getPx(), p->getPy(), p->getPz(), p->getEtotal());
  DSV(tl.Theta());
  DSV(tl.Phi());
  DSV(tl.E() - tl.M());

  fPzMap[0]=-1;
  // Glauber momentum kick 
  double sig, sig0;
  sig0 = 90;  // MeV/c
  sig = p->getA() * ( fNn + fNp ) / ( A - 1);
  if (sig < 0) ERR("sig < 0; sig = %g", sig);
  sig = fGlauberKickScale * sig0 * sqrt(sig);
  DSV(sig);
  
  cosTheta = fRngUniform->value(-1,1);// cos(theta)
  theta    = acos(cosTheta);
  phi      = fRngUniform->value(0,2.*M_PI);  // phi
  DSV(theta);
  DSV(phi);

  fRngGaussGlauber->setCentroid(0);
  fRngGaussGlauber->setSigma(sig);

  double glaubP = fRngGaussGlauber->value();

  //-=-=-==--=-=-=-==
  //idea to create tail in mom. distribution following LISE++, zwk
  if(glaubP < 0) glaubP = glaubP * fAsymMomentum;
  //--------------------

  DSV(glaubP);
  TVector3 vGlaubP(0,0,glaubP);
  vGlaubP.SetTheta(theta);
  vGlaubP.SetPhi(phi);
  
  TVector3 vP;                  // total momentum
  vP = tl.Vect() + vGlaubP;
  tl.SetVectM(vP,p->getm());

/*
  //test changing angular distribution for 14Be(2+)->12Be+n+n, zwk
  double theta_new = 0;
  while(1){
    fRngGaussGlauber->setCentroid(1.2);
    fRngGaussGlauber->setSigma(1.0);
    double theta_new_temp = fRngGaussGlauber->value() * TMath::DegToRad();
    theta_new = theta_new_temp;
    if(theta_new>0) break;
  }
  tl.SetTheta(theta_new);
  //------
*/


  DSV(tl.Theta());
  DSV(tl.Phi());
  DSV(tl.E() - tl.M());


  //debugging stuff
  if (debugSwitch == "thetaLABP")
	  cout << tl.Theta()<< endl;
  if (debugSwitch == "phiLABP")
	  cout << tl.Phi()<< endl;

  TVector3 t3Boost;
  t3Boost = tl.BoostVector();  // boost vector 

  // start of neutron evaporation
  p->setA(p->getA() - 1);
  p->setm(p->getA()*931.494028);
  p->setEkin(0);             

  TLorentzVector tl1;  // particle 1 (heavy fragment)
  tl1.SetPxPyPzE(p->getPx(), p->getPy(), p->getPz(), p->getEtotal());

  // create neutron
  StParticle pp;   
  pp.setA(1);
  pp.setm(pp.getA()*939.565346);
  pp.setZ(0);
  pp.setQ(0);
  pp.setx(p->getx());     // same position
  pp.sety(p->gety()); 
  pp.setz(p->getz());     
  pp.sett(p->gett());     // same time
  pp.setEkin(0);
  
  TLorentzVector tl2;  // particle 2 (neutron)
  tl2.SetPxPyPzE(pp.getPx(), pp.getPy(), pp.getPz(), pp.getEtotal());
  
  double eN, eF;   // total neutron and fragment energy
  double eCM;   // total CM energy

  eCM = pp.getm() + p->getm() + exen1;  // total CM energy
  eN = gsl_pow_2(eCM) + gsl_pow_2(pp.getm()) - gsl_pow_2(p->getm());
  eN = eN/(2.*eCM);                    // total energy neutron
  eF = gsl_pow_2(eCM) - gsl_pow_2(pp.getm()) + gsl_pow_2(p->getm());
  eF = eF/(2.*eCM);                    // total energy of fragment

  /// \todo check is arg of sqrt is positive
  double pN,pF; // fragment and neutron momentum
  pN = eN*eN - gsl_pow_2(pp.getm());
  pN = sqrt(pN);
  pF = eF*eF - gsl_pow_2(p->getm());
  if (pF < 0)
    pF = 0;
  else 
    pF = sqrt(pF);
  pF = -pF;  // fragment goes in opposite direction

  tl1.SetPxPyPzE(0,0,pF,eF);
  tl2.SetPxPyPzE(0,0,pN,eN);
  
  // set theta and phi for the first decay to some random value
  cosTheta = fRngUniform->value(-1,1);       // cos(theta)
  theta2    = acos(cosTheta);
  phi      = fRngUniform->value(0,2.*M_PI);  // phi
  fThetaMap[0] = theta2;
  fPhiMap[0]   = phi;

  tl1.SetTheta(theta2); // Set fragment angle
  tl1.SetPhi(phi); // Set fragment phi angle
  tl2.SetTheta(M_PI - theta2); // Set neutron angle 180-theta
  tl2.SetPhi(phi + M_PI); // set neutron phi angle

  TVector3 t4Boost;
  t4Boost = tl1.BoostVector();  // boost vector for 24O

  // start of the second neutron evaporation
  p->setA(p->getA()-1);
  p->setm(p->getA()*931.494028);
  p->setEkin(0);               // 23O at rest


  // create second neutron
  StParticle ppp;   
  ppp.setA(1);
  ppp.setm(pp.getA()*939.565346);
  ppp.setZ(0);
  ppp.setQ(0);
  ppp.setx(p->getx());     // same position
  ppp.sety(p->gety()); 
  ppp.setz(p->getz());     
  ppp.sett(p->gett());     // same time
  ppp.setEkin(0);
  
  TLorentzVector tl3;  // particle 3 (neutron)
  tl3.SetPxPyPzE(ppp.getPx(), ppp.getPy(), ppp.getPz(), ppp.getEtotal());

  double eN2, eF2;   // total neutron and fragment energy
  double eCM2;       // total CM energy

  eCM2 = ppp.getm() + p->getm() + exen2;  // total CM energy
  eN2 = gsl_pow_2(eCM2) + gsl_pow_2(ppp.getm()) - gsl_pow_2(p->getm());
  eN2 = eN2/(2.*eCM2);                    // total energy neutron
  eF2 = gsl_pow_2(eCM2) - gsl_pow_2(ppp.getm()) + gsl_pow_2(p->getm());
  eF2 = eF2/(2.*eCM2);                    // total energy of fragment

  /// \todo check is arg of sqrt is positive
  double pN2, pF2; // fragment and neutron momentum
  pN2 = eN2*eN2 - gsl_pow_2(ppp.getm());
  pN2 = sqrt(pN2);
  pF2 = eF2*eF2 - gsl_pow_2(p->getm());
  if (pF2 < 0)
    pF2 = 0;
  else 
    pF2 = sqrt(pF2);
  pF2 = -pF2;  // fragment goes in opposit direction

  tl1.SetPxPyPzE(0,0,pF2,eF2);
  tl3.SetPxPyPzE(0,0,pN2,eN2);
  
  // set theta and phi to some random value
  
  /*  if(fWhichEnergyDoIReconstruct == "volya2n"){
        cosTheta = fRngLegendre->value(-1,1);
        //theta = acos(cosTheta);
        if(acos(cosTheta) > theta2)       theta    = acos(cosTheta) - theta2;
        else if(acos(cosTheta) <= theta2) theta    = theta2 - acos(cosTheta);// Correlate the 3rd and 4th neutron
  }
  else {*/
        cosTheta = fRngUniform->value(-1,1);
        theta    = acos(cosTheta);
  //}

  //cosTheta = fRngUniform->value(-1,1);       // cos(theta)
  //theta    = acos(cosTheta);
  phi      = fRngUniform->value(0,2.*M_PI);  // phi
  fThetaMap2[0] = theta;
  fPhiMap2[0]   = phi;
  tl1.SetTheta(theta);        // Set fragment angle
  tl1.SetPhi(phi);            // Set fragment phi angle
  tl3.SetTheta(M_PI - theta); // Set neutron angle 180-theta
  tl3.SetPhi(phi + M_PI);     // Set neutron phi angle


  /*
  //===================================
  //Attempt to do L=1 angular distribution-----
  Double_t rann = fRngUniform->value(-1,1);
  Int_t negRan = 1;
  if(rann<0) negRan = -1;
  Double_t cos_legendre_angle;
  cos_legendre_angle = TMath::Power(rann,2.0);
  Double_t legendre_angle = negRan * TMath::ACos(cos_legendre_angle);

  Double_t theta_n1 = tl2.Theta();
  Double_t theta_n2 = tl3.Theta();
  Double_t phi_n1 = tl2.Phi();
  Double_t phi_n2 = tl3.Phi();
  Double_t new_theta = theta_n1 + legendre_angle;
  tl3.SetPhi(phi_n1);
  tl3.SetTheta(new_theta);
  //printf("\n\nleg_angle: %f  thetan1: %f thetan2: %f  phin1:%f phin2:%f  tl3Theta:%f  tl3Phi:%f new_theta:%f\n",legendre_angle,theta_n1,theta_n2,phi_n1,phi_n2,tl3.Theta(),tl3.Phi(),new_theta);
  //Set fragment theta so still back-to-back decay
  tl1.SetTheta( tl3.Theta() + M_PI);
  tl1.SetPhi( tl3.Phi() - M_PI);
  //cout << tl2.Angle(tl3.Vect()) << endl;
  //======================================
  */

  //Boosting into the frame where the fragment is at rest after the first decay
  tl1.Boost(t4Boost);
  tl3.Boost(t4Boost);

  //Boosting all three particles to the lab frame
  tl1.Boost(t3Boost);
  tl2.Boost(t3Boost);
  tl3.Boost(t3Boost);

  // set fragment energy and direction
  p->setEtotal(tl1.E()); /// \todo implement setLVec() to set particle data from 4-vector
  p->setThetaPhi(tl1.Theta(),tl1.Phi()); 
  
  // set neutron energy and direction
  pp.setEtotal(tl2.E());
  pp.setThetaPhi(tl2.Theta(),tl2.Phi()); 

  if(! newPart) ERR("Pointer newPart not set; Cannot add new particles");
  newPart->push_back(pp); 

  // set second neutron energy and direction
  ppp.setEtotal(tl3.E());
  ppp.setThetaPhi(tl3.Theta(),tl3.Phi());

  if(! newPart) ERR("Pointer newPart not set; Cannot add new particles");
  newPart->push_back(ppp); 

  DSV(p->getA());
  DSV(p->getZ());
  return 0;
}

int StReacStripEvap2neutron::savedValue(size_t part_id, string name, double* v) {
  DSV(part_id);
  DSV(name);
  if (name == "exen" &&  (fExEnMap.find(part_id) != fExEnMap.end()) ) {
    *v = fExEnMap[part_id];
    DSV(*v);
    return 0;
  }
  if (name == "exen2" &&  (fExEnMap2.find(part_id) != fExEnMap2.end()) ) {
    *v = fExEnMap2[part_id];
    DSV(*v);
    return 0;
  }
  if (name == "ph" &&  (fPhiMap.find(part_id) != fPhiMap.end()) ) {
    *v = fPhiMap[part_id];
    DSV(*v);
    return 0;
  }
  if (name == "ph2" &&  (fPhiMap2.find(part_id) != fPhiMap2.end()) ) {
    *v = fPhiMap2[part_id];
    DSV(*v);
    return 0;
  }
  if (name == "th" &&  (fThetaMap.find(part_id) != fThetaMap.end()) ) {
    *v = fThetaMap[part_id];
    DSV(*v);
    return 0;
  }
  if (name == "th2" &&  (fThetaMap2.find(part_id) != fThetaMap2.end()) ) {
    *v = fThetaMap2[part_id];
    DSV(*v);
    return 0;
  }
  if (name == "pz" &&  (fPzMap.find(part_id) != fPzMap.end()) ) {
    *v = fPzMap[part_id];
    DSV(*v);
    return 0;
  }
  return 1;
}

////////////////////////////2n sequential//////////////////////////////////////

//---------------------di-neutron decay-------------------------//
int StReacStripEvap_dineutron::act(StParticle* p, vector<StParticle>* newPart) {

  //Setting up the two decay energies
  double exen     = 0; //Total Decay Energy
  double exen1    = 0; //1st Decay Energy
  double exen2    = 0; //2nd Decay Energy
  //Create Varibles to give give directions to particles and Glauber Kick
  double cosTheta;     //Used to set the cos(Theta)
  double theta;        //Used to set the Theta
  double phi;          //Used to set the Phi

  double di_ei=0, di_ek=0;  //for Volya dineutron, intrinsic and kinetic energy of the dineutron

  //Check that all necassery random number generators are found
  if (!fRngExEn)           ERR("Need fRngExEn");
  if (!fRngExEn1)          ERR("Need fRngExEn1");
  if (!fRngExEn2)          ERR("Need fRngExEn2");
  if (!fRngUniform)        ERR("Need uniform random number generator");
  if (!fRngGaussGlauber)   ERR("Need gauss random number generator");

  //Removed the fWhichEnergyDoIReconstruct stuff (not for di-neutron decay description)

  //debugging stuff -> Decay Energies
  if (debugSwitch == "exen1")
	  cout << exen1<< endl;
  if (debugSwitch == "exen2")
	  cout << exen2<< endl;

  if (!p) ERR("Particle pointer not set.");

  
  double exenTotal = -1;
  double exenDiNeutron = -1;
  //Volya's dineutron description
  if(fWhichEnergyDoIReconstruct=="volyadineutron"){

    //for volya + bw
    fRngExEn->value2d(di_ei,di_ek);
    
    exenTotal = di_ei + di_ek;
    exenDiNeutron = di_ei;
  }
  else{
    
    //Break-up Energy of the fragment+n2 system taken from input of -e 
    while(1){
      exenTotal = fRngExEn->value(); //MeV
      if(exenTotal>0) break;
    }
    //Break-up Energy of the di-neutron taken from input of -e1 
    while(1){
      exenDiNeutron = fRngExEn1->value(); //MeV
      //exenTotal = exenDiNeutron + exenTotal;
      //exenDiNeutron = exenTotal * 0.5;
      if(exenDiNeutron>0 && exenDiNeutron<exenTotal) break;
    }
  }
  
  //cout << exenTotal << "  " << exenDiNeutron << endl;

  DSV(exenTotal);
  DSV(exenDiNeutron);
  fExEnMap[0]=exenTotal;
  fExEnMap2[0]=exenDiNeutron;

  // change particle, keep velocity the same
  //**zwk reduce beta after reaction following LISE++
  double beta = p->getBeta() * fVelocityScale;        // initial beta
  double A = p->getA();            // initial mass number
  p->setA(p->getA() - fNn - fNp);  // remove protons and neutrons
  p->setm(p->getA()*931.494028);        // set new mass
  p->setZ(p->getZ() - fNp);        // set new Z
  p->setQ(p->getZ());              // set new Q
  p->setBeta(beta);                // set velocity

  TLorentzVector tl;
  tl.SetPxPyPzE(p->getPx(), p->getPy(), p->getPz(), p->getEtotal());
  DSV(tl.Theta());
  DSV(tl.Phi());
  DSV(tl.E() - tl.M());

  fPzMap[0]=-1;
  // Glauber momentum kick 
  double sig, sig0;
  sig0 = 90;  // MeV/c
  sig = p->getA() * ( fNn + fNp ) / ( A - 1);
  if (sig < 0) ERR("sig < 0; sig = %g", sig);
  sig = fGlauberKickScale * sig0 * sqrt(sig);
  DSV(sig);
  
  cosTheta = fRngUniform->value(-1,1);// cos(theta)
  theta    = acos(cosTheta);
  phi      = fRngUniform->value(0,2.*M_PI);  // phi
  DSV(theta);
  DSV(phi);

  fRngGaussGlauber->setCentroid(0);
  fRngGaussGlauber->setSigma(sig);

  double glaubP = fRngGaussGlauber->value();
  //-=-=-==--=-=-=-==
  //idea to create tail in mom. distribution following LISE++, zwk
  if(glaubP < 0) glaubP = glaubP * fAsymMomentum;
  //--------------------

  DSV(glaubP);
  TVector3 vGlaubP(0,0,glaubP);
  vGlaubP.SetTheta(theta);
  vGlaubP.SetPhi(phi);
  
  TVector3 vP;                  // total momentum
  vP = tl.Vect() + vGlaubP;
  tl.SetVectM(vP,p->getm());
  DSV(tl.Theta());
  DSV(tl.Phi());
  DSV(tl.E() - tl.M());

  //debugging stuff
  if (debugSwitch == "thetaLABP")
	  cout << tl.Theta()<< endl;
  if (debugSwitch == "phiLABP")
	  cout << tl.Phi()<< endl;

  TVector3 t3Boost;
  t3Boost = tl.BoostVector();  // boost vector 

  // start of di-neutron evaporation
  p->setA(p->getA() - 2);
  p->setm(p->getA()*931.494028);
  p->setEkin(0);             

  TLorentzVector tl1;  // particle 1 (heavy fragment)
  tl1.SetPxPyPzE(p->getPx(), p->getPy(), p->getPz(), p->getEtotal());

  // create di-neutron
  StParticle pp;   
  pp.setA(2);
  pp.setm(pp.getA()*939.565346);
  pp.setZ(0);
  pp.setQ(0);
  pp.setx(p->getx());     // same position
  pp.sety(p->gety()); 
  pp.setz(p->getz());     
  pp.sett(p->gett());     // same time
  pp.setEkin(0);
  
  TLorentzVector tl2;  // particle 2 (di-neutron)
  tl2.SetPxPyPzE(pp.getPx(), pp.getPy(), pp.getPz(), pp.getEtotal());
  
  double e2N, eF;   // total neutron and fragment energy
  double eCM;   // total CM energy

  //Make sure don't have negative decay energies.
  //Need to develop how we choose dineutron and frag-2n energies.
  double exen12_left = exenTotal - exenDiNeutron;
  if(exen12_left < 0){
    cerr << "ExEn Left is less than ZERO!!" << endl;
  }

  eCM = pp.getm() + p->getm() + (exen12_left);  // total CM energy
  e2N = gsl_pow_2(eCM) + gsl_pow_2(pp.getm()) - gsl_pow_2(p->getm());
  e2N = e2N/(2.*eCM);                    // total energy neutron
  eF = gsl_pow_2(eCM) - gsl_pow_2(pp.getm()) + gsl_pow_2(p->getm());
  eF = eF/(2.*eCM);                    // total energy of fragment

  /// \todo check is arg of sqrt is positive
  double p2N,pF; // fragment and neutron momentum
  p2N = e2N*e2N - gsl_pow_2(pp.getm());
  p2N = sqrt(p2N);
  pF = eF*eF - gsl_pow_2(p->getm());
  if (pF < 0)
    pF = 0;
  else 
    pF = sqrt(pF);
  pF = -pF;  // fragment goes in opposit direction

  tl1.SetPxPyPzE(0,0,pF,eF);
  tl2.SetPxPyPzE(0,0,p2N,e2N);
  
  // set theta and phi for the first decay to some random value
  cosTheta = fRngUniform->value(-1,1);       // cos(theta)
  theta    = acos(cosTheta);
  phi      = fRngUniform->value(0,2.*M_PI);  // phi
  fThetaMap[0] = theta;
  fPhiMap[0]   = phi;  
  fThetaMap2[0] = theta;
  fPhiMap2[0]   = phi;

  tl1.SetTheta(theta); // Set fragment angle
  tl1.SetPhi(phi); // Set fragment phi angle
  tl2.SetTheta(M_PI - theta); // Set di-neutron angle 180-theta
  tl2.SetPhi(phi + M_PI); // set di-neutron phi angle

  TVector3 t4Boost;
  t4Boost = tl2.BoostVector();  // boost vector for di-neutron

  // start of the di-neutron decay
  pp.setA(pp.getA()-1);
  pp.setm(939.565346);
  pp.setEkin(0);               // 23O at rest


  // create second neutron
  StParticle ppp;   
  ppp.setA(1);
  ppp.setm(pp.getA()*939.565346);
  ppp.setZ(0);
  ppp.setQ(0);
  ppp.setx(pp.getx());     // same position
  ppp.sety(pp.gety()); 
  ppp.setz(pp.getz());     
  ppp.sett(pp.gett());     // same time
  ppp.setEkin(0);
  
  TLorentzVector tl3;  // particle 3 (neutron)
  tl3.SetPxPyPzE(ppp.getPx(), ppp.getPy(), ppp.getPz(), ppp.getEtotal());

  double eN_1, eN_2;   // total neutron and fragment energy
  double eCM2;       // total CM energy

  eCM2 = ppp.getm() + pp.getm() + exenDiNeutron;  // total CM energy
  eN_2 = gsl_pow_2(eCM2) + gsl_pow_2(ppp.getm()) - gsl_pow_2(pp.getm());
  eN_2 = eN_2/(2.*eCM2);                    // total energy neutron
  eN_1 = gsl_pow_2(eCM2) - gsl_pow_2(ppp.getm()) + gsl_pow_2(pp.getm());
  eN_1 = eN_1/(2.*eCM2);                    // total energy of fragment


  /// \todo check is arg of sqrt is positive
  double pN_2, pN_1; // fragment and neutron momentum
  pN_2 = eN_2*eN_2 - gsl_pow_2(ppp.getm());
  pN_2 = sqrt(pN_2);
  pN_1 = eN_1*eN_1 - gsl_pow_2(pp.getm());
  if (pN_1 < 0)
    pN_1 = 0;
  else 
    pN_1 = sqrt(pN_1);
  pN_1 = -pN_1;  // fragment goes in opposit direction

  tl2.SetPxPyPzE(0,0,pN_1,eN_1);
  tl3.SetPxPyPzE(0,0,pN_2,eN_2);
  
  // set theta and phi to some random value
  cosTheta = fRngUniform->value(-1,1);       // cos(theta)
  theta    = acos(cosTheta);
  phi      = fRngUniform->value(0,2.*M_PI);  // phi
  //fThetaMap2[0] = theta;
  //fPhiMap2[0]   = phi;
  tl2.SetTheta(theta);        // Set neutron1 angle
  tl2.SetPhi(phi);            // Set neutron1 phi angle
  tl3.SetTheta(M_PI - theta); // Set neutron2 angle 180-theta
  tl3.SetPhi(phi + M_PI);     // Set neutron2 phi angle

  //Boosting into the frame where the fragment is at rest after the first decay
  tl2.Boost(t4Boost);
  tl3.Boost(t4Boost);


  /*
  //-------
  //Calculating angles, energy for output of the original nn correlations for PRL comment reply
  TLorentzVector lv_F = tl1;
  TLorentzVector lv_N1 = tl2;
  TLorentzVector lv_N2 = tl3;

  TLorentzVector lv_sys = lv_F + lv_N1 + lv_N2;
  TVector3 boost = lv_sys.BoostVector();
  TLorentzVector lv_N1cm = lv_N1;
  lv_N1cm.Boost(-1*boost);
  TLorentzVector lv_N2cm = lv_N2;
  lv_N2cm.Boost(-1*boost);
  TLorentzVector lv_Fcm = lv_F;
  lv_Fcm.Boost(-1*boost);

  double cosn1n2theta =  cos(lv_N1cm.Angle(lv_N2cm.Vect()));
  //double cosn1n2theta =  cos(lv_Fcm.Angle(lv_N1cm.Vect()));
  //cout << cosn1n2theta << endl;

  TLorentzVector lvNN = lv_N1 + lv_N2;
  double edecayNN = lvNN.M() - lv_N1.M() - lv_N2.M();
  cout << edecayNN << endl;
  //-----------
  */

  //Boosting all three particles to the lab frame
  tl1.Boost(t3Boost);
  tl2.Boost(t3Boost);
  tl3.Boost(t3Boost);

  // set fragment energy and direction
  p->setEtotal(tl1.E()); /// \todo implement setLVec() to set particle data from 4-vector
  p->setThetaPhi(tl1.Theta(),tl1.Phi()); 
  
  // set neutron energy and direction
  pp.setEtotal(tl2.E());
  pp.setThetaPhi(tl2.Theta(),tl2.Phi()); 

  if(! newPart) ERR("Pointer newPart not set; Cannot add new particles");
  newPart->push_back(pp); 

  // set second neutron energy and direction
  ppp.setEtotal(tl3.E());
  ppp.setThetaPhi(tl3.Theta(),tl3.Phi());

  if(! newPart) ERR("Pointer newPart not set; Cannot add new particles");
  newPart->push_back(ppp); 

  DSV(p->getA());
  DSV(p->getZ());
  return 0;
}

int StReacStripEvap_dineutron::savedValue(size_t part_id, string name, double* v) {
  DSV(part_id);
  DSV(name);
  if (name == "exen" &&  (fExEnMap.find(part_id) != fExEnMap.end()) ) {
    *v = fExEnMap[part_id];
    DSV(*v);
    return 0;
  }
  if (name == "exen2" &&  (fExEnMap2.find(part_id) != fExEnMap2.end()) ) {
    *v = fExEnMap2[part_id];
    DSV(*v);
    return 0;
  }
  if (name == "ph" &&  (fPhiMap.find(part_id) != fPhiMap.end()) ) {
    *v = fPhiMap[part_id];
    DSV(*v);
    return 0;
  }
  if (name == "ph2" &&  (fPhiMap2.find(part_id) != fPhiMap2.end()) ) {
    *v = fPhiMap2[part_id];
    DSV(*v);
    return 0;
  }
  if (name == "th" &&  (fThetaMap.find(part_id) != fThetaMap.end()) ) {
    *v = fThetaMap[part_id];
    DSV(*v);
    return 0;
  }
  if (name == "th2" &&  (fThetaMap2.find(part_id) != fThetaMap2.end()) ) {
    *v = fThetaMap2[part_id];
    DSV(*v);
    return 0;
  }
  if (name == "pz" &&  (fPzMap.find(part_id) != fPzMap.end()) ) {
    *v = fPzMap[part_id];
    DSV(*v);
    return 0;
  }
  return 1;
}
////////////////////////////di-neutron decay///////////////////////////////

//---------------------3-body decay-------------------------//
int StReacStripEvap_3bodydecay::act(StParticle* p, vector<StParticle>* newPart) {

  //Setting up the two decay energies
  double exen     = 0; //Total Decay Energy
  double exen1    = 0; //1st Decay Energy
  double exen2    = 0; //2nd Decay Energy
  //Create Varibles to give give directions to particles and Glauber Kick
  double cosTheta;     //Used to set the cos(Theta)
  double theta;        //Used to set the Theta
  double phi;          //Used to set the Phi

  //for volya 2n
  double erel = 0; //rel energy between 2 neutrons

  //Check that all necassery random number generators are found
  if (!fRngExEn)           ERR("Need fRngExEn");
  if (!fRngExEn1)          ERR("Need fRngExEn1");
  if (!fRngExEn2)          ERR("Need fRngExEn2");
  if (!fRngUniform)        ERR("Need uniform random number generator");
  if (!fRngGaussGlauber)   ERR("Need gauss random number generator");

  //Removed the fWhichEnergyDoIReconstruct (since it isn't needed for 3body decay)

  double exenTotal = -1;
  while(1){
    exenTotal = fRngExEn->value(); //MeV
    if(exenTotal>0) break;
  }

//  printf("\nexenTotal: %f\n",exenTotal);

  DSV(exenTotal);
  DSV(exen2);
//  fExEnMap[0]=exenTotal;
//  fExEnMap2[0]=exen2;    
  
  //debugging stuff -> Decay Energies
  if (debugSwitch == "exen1")
	  cout << exen1<< endl;
  if (debugSwitch == "exen2")
	  cout << exen2<< endl;

  if (!p) ERR("Particle pointer not set.");


  // change particle, keep velocity the same
  //**zwk reduce beta after reaction following LISE++
  double beta = p->getBeta() * fVelocityScale;        // initial beta
  double A = p->getA();            // initial mass number
  p->setA(p->getA() - fNn - fNp);  // remove protons and neutrons
  p->setm(p->getA()*931.494028);        // set new mass
  p->setZ(p->getZ() - fNp);        // set new Z
  p->setQ(p->getZ());              // set new Q
  p->setBeta(beta);                // set velocity

  TLorentzVector tl;
  tl.SetPxPyPzE(p->getPx(), p->getPy(), p->getPz(), p->getEtotal());
  DSV(tl.Theta());
  DSV(tl.Phi());
  DSV(tl.E() - tl.M());

  fPzMap[0]=-1;
  // Glauber momentum kick 
  double sig, sig0;
  sig0 = 90;  // MeV/c
  sig = p->getA() * ( fNn + fNp ) / ( A - 1);
  if (sig < 0) ERR("sig < 0; sig = %g", sig);
  sig = fGlauberKickScale * sig0 * sqrt(sig);
  DSV(sig);
  
  cosTheta = fRngUniform->value(-1,1);// cos(theta)
  theta    = acos(cosTheta);
  phi      = fRngUniform->value(0,2.*M_PI);  // phi
  DSV(theta);
  DSV(phi);

  fRngGaussGlauber->setCentroid(0);
  fRngGaussGlauber->setSigma(sig);

  double glaubP = fRngGaussGlauber->value();
  //-=-=-==--=-=-=-==
  //idea to create tail in mom. distribution following LISE++, zwk
  // ******************* HL 01/10/2017 START
  //
  //
  /*
  if(glaubP < 0) glaubP = glaubP * fAsymMomentum;
  //--------------------
  DSV(glaubP);
  TVector3 vGlaubP(0,0,glaubP);
  vGlaubP.SetTheta(theta);
  vGlaubP.SetPhi(phi);
  */
  DSV(glaubP);
//    TVector3 vGlaubP;
    TVector3 vGlaubP(0,0,glaubP);// set via initializer as ZK does (line 1428)
    //Transverse momentum kick parametrization based on Van Bibber et al., PRL 43:12,840-844 (1979) -JKS, 8 March 2013
    if(fUseTransKick){
	  //-=-=-==--=-=-=-==
	  //idea to create tail in mom. distribution following LISE++, zwk
	  if(glaubP < 0) glaubP = glaubP * fAsymMomentum;
  //--------------------
    	double sigperp, sigperp1, sigperp2, sig2;
	sig2 = 200;  // MeV/c
	sigperp1 = p->getA() * abs( fNn + fNp ) / ( A - 1);
    	if (sigperp1 < 0) ERR("sigperp1 < 0; sigperp1 = %g", sigperp1);
	sigperp2 = p->getA() * (p->getA()-1) / (A*(A-1));
    	if (sigperp2 < 0) ERR("sigperp2 < 0; sigperp2 = %g", sigperp2);
    	sigperp = sqrt(fGlauberTransScale1 * fGlauberTransScale1 * sig0 * sig0 * sigperp1 + fGlauberTransScale2 * fGlauberTransScale2 * sig2 * sig2 * sigperp2);
    	DSV(sigperp);

    	double glaubx,glauby;
	fRngGaussGlauberTrans->setCentroid(0);
	fRngGaussGlauberTrans->setSigma(sigperp);
	glaubx = fRngGaussGlauberTrans->value();
	glauby = fRngGaussGlauberTrans->value();
	vGlaubP.SetXYZ(glaubx,glauby,glaubP);
  
    }

    else{
    //-=-=-==--=-=-=-==
    //idea to create tail in mom. distribution following LISE++, zwk
    	if(glaubP < 0)
    	{
    		//cout <<cosTheta <<endl;
		//cout <<abs(glaubP) <<endl;
    		glaubP = glaubP * fAsymMomentum;
	}
    //--------------------
    /* why is this ?? THR 4/17/2017
    double r = glaubP/cosTheta;
    vGlaubP.SetXYZ(0,0,r);
    */
//    vGlaubP.SetXYZ(0,0,glaubP);
    vGlaubP.SetTheta(theta);
    vGlaubP.SetPhi(phi);
    }
   
   //cout <<"Pz:\t" <<vGlaubP.Pz() <<endl;
   //cout <<"glaubP:\t" <<glaubP <<endl;

// *************************************** HL 01/10/17 END



  TVector3 vP;                  // total momentum
  vP = tl.Vect() + vGlaubP;
  tl.SetVectM(vP,p->getm());
  DSV(tl.Theta());
  DSV(tl.Phi());
  DSV(tl.E() - tl.M());
  
  //debugging stuff
  if (debugSwitch == "thetaLABP")
    cout << tl.Theta()<< endl;
  if (debugSwitch == "phiLABP")
    cout << tl.Phi()<< endl;
  
  
  
  //Now have the excited fragment in TLorentzVector tl
  //Use 3-body decay from ROOT to decay Frag* -> frag + n +n
  //Units are GeV and GeV/c!!
  
  //set lorentz vector of decaying system (GeV unit)
  //need to set_mass to the same as the frag+2n since we don't know the mass excess of the 
  //excited fragment, otherwise can get E of excited frag < then E of the frag-n-n system
  //and therefore the 3-body decay is forbidden.
  //double set_mass = (p->getA()-2)*931.494028 +  939.565346 + 939.565346;
  double set_mass = (p->getA()-2)*931.494 +  1.0*931.494 + 1.0*931.494;
  TLorentzVector t1gev;
  t1gev.SetVectM(vP*(1/1000.), (set_mass+exenTotal)/1000.);

  //cout << vP.x() << " " << vP.y() << " " << vP.z() << endl;
  //cout << p->getm() << " " << p->getA() << endl;
  //cout << exen1 << " " << exen2 << endl;
  //cout << t1gev.M() << "  " << t1gev.Mag() << endl;
  //cout << ( (p->getA()-2)*931.494028/1000. +  939.565346/1000. + 939.565346/1000.) << endl;
  
  //set the masses of the 3 products.
  Double_t Mass3body[3] = { (p->getA()-2)*931.494/1000.,  1.0*931.494/1000., 1.0*931.494/1000.};
  TGenPhaseSpace tPSevent;
  Bool_t bevent = tPSevent.SetDecay(t1gev, 3, Mass3body);
  if(!bevent) cerr << "3-Body Decay is Kinematically FORBIDDEN !!!!!" << endl;

  TLorentzVector *pt1, *pt2, *pt3;

  double wtmax = tPSevent.GetWtMax();
  while(1){    
    double weight = tPSevent.Generate();
//    double rel_weight = weight / wtmax;
    double rel_weight = weight / 0.75;
    if(rel_weight>1.)cout << "Weight Normalization error!" << endl;
    double ran = fRngUniform->value(0,1);

    //events are weighted (rel_weight) between 0 and 1
    //therefore only select event when ran is less than rel_weight
//    if(fWeight > 0){ 
      if(ran < rel_weight) break;
//     }
//    break; 
  }
  pt1 = tPSevent.GetDecay(0); //frag - 2n
  pt2 = tPSevent.GetDecay(1); //neutron1
  pt3 = tPSevent.GetDecay(2); //neutron2

  //The returned Lorentz vectors are automatically boosted into the frame
  //of the originally decaying fragment
  //NEED TO CONVERT BACK FROM GEV TO MEV

  /* Calculate decay energies */
  TLorentzVector lv_F = *pt1;
  TLorentzVector lv_N1 = *pt2;
  TLorentzVector lv_N2 = *pt3;

  TLorentzVector lv_sys = lv_F + lv_N1 + lv_N2;
  TVector3 boost = lv_sys.BoostVector();
  TLorentzVector lv_N1cm = lv_N1;
  lv_N1cm.Boost(-1*boost);
  TLorentzVector lv_N2cm = lv_N2;
  lv_N2cm.Boost(-1*boost);
  TLorentzVector lv_Fcm = lv_F;
  lv_Fcm.Boost(-1*boost);

//  ofstream exenergies;
//  exenergies.open("/projects/mona-sim/baumann/exenergies.out",std::ofstream::out | std::ofstream::app);

  // fragment-neutron1
  TLorentzVector lvFN = lv_F + lv_N1;
  exen = lvFN.M() - lv_F.M() - lv_N1.M();
  exen *= 1000.;
  fExEnMap[0]=exen;
//  printf("\nexen: %f\n\n",exen);

  // neutron-neutron
  TLorentzVector lvNN = lv_N1 + lv_N2;
  exen2 = lvNN.M() - lv_N1.M() - lv_N2.M();
  exen2 *= 1000.;
  fExEnMap2[0]=exen2;
//  printf("\nexen2: %f\n\n",exen2);

  // fragment-neutron2
  TLorentzVector lvFN2 = lv_F + lv_N2;
  double exen3 = lvFN2.M() - lv_F.M() - lv_N2.M();
  exen3*=1000.;

  // Jacobi coordinates from mona_analysis.cc (line 2295)
  // 4 cases -> T and Y systems with two ways to label neutrons in each case
  // for now, we'll just consider one possible labelling of the neutron for
  // T and Y: k1 = N1 in each system
/*
  double Tsystem[4];
  double Ysystem[4];
  Double_t M = 931.5;

  TVector3 N1 = lv_N1.Vect();
  TVector3 N2 = lv_N2.Vect();
  TVector3 FF = lv_F.Vect();
  Double_t mass1 = lv_N1.M();
  Double_t mass2 = lv_N2.M();
  Double_t massF = lv_F.M();

  for(int i=0; i<4; i++)
  {
    TVector3 k1, k2, k3;
    Double_t m1, m2, m3;

    if(i==0){
      k1 = N1;
      k2 = N2;
      k3 = FF;
      m1 = mass1;
      m2 = mass2;
      m3 = massF;      
    }
    else if(i==1){
      //T - System, k1 = N2
      k1 = N2;
      k2 = N1;
      k3 = FF;
      m1 = mass2;
      m2 = mass1;
      m3 = massF;      
    }
    else if(i==2){
      //Y - System, k1 = N1
      k1 = N1;
      k2 = FF;
      k3 = N2;
      m1 = mass1;
      m2 = massF;
      m3 = mass2;      
    }
    else if(i==3){
      //Y - System, k1 = N2
      k1 = N2;
      k2 = FF;
      k3 = N1;
      m1 = mass2;
      m2 = massF;
      m3 = mass1;      
    }


    //Note from MJ: Using pow(..., -1.0) instead of dividing with '/' because that operator is not defined for TVector3.
    TVector3 kx = (m2*k1 - m1*k2) * pow(m1 + m2,-1.0);
    TVector3 ky = (m3*(k1+k2) - (m1+m2)*k3) * pow(m1+m2+m3,-1.0);

    Double_t Ex = ( (m1+m2)*kx.Mag2() ) / (2.*M*m1*m2);
    Double_t Ey = ( (m1+m2+m3)*ky.Mag2() ) / (2*M*m3*(m1+m2));

    Double_t Et = Ex + Ey;

    Double_t Ex_Et = Ex / Et;// plot this

    Double_t costheta_k = kx.Dot(ky) / (kx.Mag() * ky.Mag());// plot this

    //Set array to correct values
    if(i==0){
      Tsystem[0] = Ex_Et;
      Tsystem[1] = costheta_k;
    }
    if(i==1){
      Tsystem[2] = Ex_Et;
      Tsystem[3] = costheta_k;
    }
    if(i==2){
      Ysystem[0] = Ex_Et;
      Ysystem[1] = costheta_k;
    }
    if(i==3){
      Ysystem[2] = Ex_Et;
      Ysystem[3] = costheta_k;
    }
  }    
    
  
  //            F-N1                  F-N2                    N-N
  exenergies << exen << "        " << exen3 << "        " << exen2 << "        ";
  exenergies << Tsystem[0] << "        " << Tsystem[1] << "        " << Ysystem[0] << "        " << Ysystem[1] << "        " << endl;
//  exenergies << N1.Theta() << N2.Theta() << FF.Theta() << endl;
  exenergies.close();
*/
  Double_t fE = pt1->E()*1000.; Double_t n1E = pt2->E()*1000.; Double_t n2E = pt3->E()*1000.;
  Double_t fM = pt1->M()*1000.; Double_t n1M = pt2->M()*1000.; Double_t n2M = pt3->M()*1000.;

/*
  exenergies << pt1->E() << "        " << pt1->Theta() <<  "        " << pt1->Phi() << "        ";
  exenergies << pt2->E() << "        " << pt2->Theta() <<  "        " << pt2->Phi() << "        ";
  exenergies << pt3->E() << "        " << pt3->Theta() <<  "        " << pt3->Phi() << endl;
*/
  /* Fragment
  exenergies << setw(15) << setprecision(6) << fE - fM;// fKE
//  exenergies << setw(15) << setprecision(6) << pt1->Theta();
//  exenergies << setw(15) << setprecision(6) << pt1->Phi();
  exenergies << setw(15) << setprecision(6) << lv_Fcm.Theta();
  exenergies << setw(15) << setprecision(6) << lv_Fcm.Phi();
  // Neutron 1
  exenergies << setw(15) << setprecision(6) << n1E - n1M;// n1 KE
//  exenergies << setw(15) << setprecision(6) << pt2->Theta();
//  exenergies << setw(15) << setprecision(6) << pt2->Phi();
  exenergies << setw(15) << setprecision(6) << lv_N1cm.Theta();
  exenergies << setw(15) << setprecision(6) << lv_N1cm.Phi();
  // Neutron 2
  exenergies << setw(15) << setprecision(6) << n2E - n2M;// n2 KE
//  exenergies << setw(15) << setprecision(6) << pt3->Theta();
//  exenergies << setw(15) << setprecision(6) << pt3->Phi() << endl;
  exenergies << setw(15) << setprecision(6) << lv_N2cm.Theta();
  exenergies << setw(15) << setprecision(6) << lv_N2cm.Phi() << endl;
  exenergies.close();
*/

  //Set fragment info
  p->setEkin(0);
  p->setA(p->getA()-2);
  p->setm(pt1->M()*1000);
 // set fragment energy and direction
  p->setEtotal(pt1->E()*1000); /// \todo implement setLVec() to set particle data from 4-vector
  p->setThetaPhi(pt1->Theta(),pt1->Phi()); 
  fThetaMap[0] = pt1->Theta();
  fPhiMap[0]   = pt1->Phi();
  fThetaMap2[0] = pt1->Theta();
  fPhiMap2[0]   = pt1->Phi();
  
  // set neutron energy and direction
  StParticle pp;
  pp.setZ(0);
  pp.setQ(0);
  pp.setx(p->getx());     // same position
  pp.sety(p->gety()); 
  pp.setz(p->getz());     
  pp.sett(p->gett());     // same time
  pp.setEkin(0);
  pp.setA(1);
  pp.setm(pt2->M()*1000);
  pp.setEtotal(pt2->E()*1000);
  pp.setThetaPhi(pt2->Theta(),pt2->Phi()); 

  if(! newPart) ERR("Pointer newPart not set; Cannot add new particles");
  newPart->push_back(pp); 

  // set second neutron energy and direction
  StParticle ppp;   
  ppp.setA(1);
  ppp.setm(pt3->M()*1000);
  ppp.setZ(0);
  ppp.setQ(0);
  ppp.setx(p->getx());     // same position
  ppp.sety(p->gety()); 
  ppp.setz(p->getz());     
  ppp.sett(p->gett());     // same time
  ppp.setEkin(0);
  ppp.setEtotal(pt3->E()*1000);
  ppp.setThetaPhi(pt3->Theta(),pt3->Phi());


  /*
  //-------
  //Calculating angles, energy for output of the original nn correlations for PRL comment reply
  TLorentzVector lv_F = *pt1;
  TLorentzVector lv_N1 = *pt2;
  TLorentzVector lv_N2 = *pt3;

  TLorentzVector lv_sys = lv_F + lv_N1 + lv_N2;
  TVector3 boost = lv_sys.BoostVector();
  TLorentzVector lv_N1cm = lv_N1;
  lv_N1cm.Boost(-1*boost);
  TLorentzVector lv_N2cm = lv_N2;
  lv_N2cm.Boost(-1*boost);
  TLorentzVector lv_Fcm = lv_F;
  lv_Fcm.Boost(-1*boost);

  //double cosn1n2theta =  cos(lv_N1cm.Angle(lv_N2cm.Vect()));
  double cosn1n2theta =  cos(lv_Fcm.Angle(lv_N1cm.Vect()));
  cout << cosn1n2theta << endl;

  TLorentzVector lvNN = lv_N1 + lv_N2;
  double edecayNN = lvNN.M() - lv_N1.M() - lv_N2.M();
  //cout << edecayNN << endl;
  //-----------
  */

  if(! newPart) ERR("Pointer newPart not set; Cannot add new particles");
  newPart->push_back(ppp); 

  DSV(p->getA());
  DSV(p->getZ());
  return 0;
}

int StReacStripEvap_3bodydecay::savedValue(size_t part_id, string name, double* v) {
  DSV(part_id);
  DSV(name);
  if (name == "exen" &&  (fExEnMap.find(part_id) != fExEnMap.end()) ) {
    *v = fExEnMap[part_id];
    DSV(*v);
    return 0;
  }
  if (name == "exen2" &&  (fExEnMap2.find(part_id) != fExEnMap2.end()) ) {
    *v = fExEnMap2[part_id];
    DSV(*v);
    return 0;
  }
  if (name == "ph" &&  (fPhiMap.find(part_id) != fPhiMap.end()) ) {
    *v = fPhiMap[part_id];
    DSV(*v);
    return 0;
  }
  if (name == "ph2" &&  (fPhiMap2.find(part_id) != fPhiMap2.end()) ) {
    *v = fPhiMap2[part_id];
    DSV(*v);
    return 0;
  }
  if (name == "th" &&  (fThetaMap.find(part_id) != fThetaMap.end()) ) {
    *v = fThetaMap[part_id];
    DSV(*v);
    return 0;
  }
  if (name == "th2" &&  (fThetaMap2.find(part_id) != fThetaMap2.end()) ) {
    *v = fThetaMap2[part_id];
    DSV(*v);
    return 0;
  }
  if (name == "pz" &&  (fPzMap.find(part_id) != fPzMap.end()) ) {
    *v = fPzMap[part_id];
    DSV(*v);
    return 0;
  }
  return 1;
}
////////////////////////////3body decay///////////////////////////////


//////////////////////////////////////////////////////////////////////
// ==========================================================
//          ONE NEUTRON REACTION CLASSES:
//          StripEvapTwoBody, (d,p) -> at top
// ==========================================================
//////////////////////////////////////////////////////////////////////


/////////////////TWO BODY REACTION/////////////////////////////////////////////

int StReacStripEvapTwoBody::act(StParticle* p, vector<StParticle>* newPart) {
  if (!p)                ERR("Particle pointer not set.");
  if (!fRngGaussQvalue)  ERR("Need gauss Q-value random number generator");
  
  fPzMap[0]=-1;
  //calculate reaction energy based on input mean and spread
  //cout << "My beam energy is " << p->getEkin()/p->getA() << " and " << p->getA() << endl;
  fRngGaussQvalue->setCentroid(0.);
  fRngGaussQvalue->setSigma(Qspread*Qval);
  double Qchange = fRngGaussQvalue->value();
  double Qset    = Qval + Qchange;
  //debugging stuff
  if (debugSwitch == "qValue")
	  cout << Qset<< endl;
#if 0
	Qset = fRngUniform->value(-15.0, -5.0);
#endif

//cout << "My Qset is " << Qset << endl;

// ***** If you use a target other than Be you need to change the mass in the following lines****
  double Etot_lab = p->getEtotal() + 9.0*931.494028; //adds beam energy to tgt for total energy.
  double B = p->getP() / Etot_lab; //calculates the beta of the CoM relative to lab 
  double G = 1./sqrt(1. - B*B); //calculates gamma of CoM relative to lab

//cout << "My B and G are " << B  << " and "<< G << endl;

  //create 4-vector for our system in lab frame
  TLorentzVector tl;
  tl.SetPxPyPzE(p->getPx(), p->getPy(), p->getPz(), Etot_lab);

  //calculate the boost needed to move from CoM to lab
  TVector3 t3Boost;
  t3Boost = tl.BoostVector();

  //now move to CoM frame
  double Etot_cm = Etot_lab / G; //calculate total energy in CoM
  double Ekin_cm = Etot_cm - p->getm() - 9.0*931.494028 + Qset; //calculate kinetic energy in com


  p->setA(p->getA() - fNn - fNp);  // remove protons and neutrons
  p->setm(p->getA()*931.494028);        // set new mass
  //  p->setm(7.028021*931.494028);        // set new mass
  p->setZ(p->getZ() - fNp);        // set new Z
  p->setQ(p->getZ());              // set new Q
  //cout << "My Etot in com is " << Etot_cm << endl;
  //cout << "My Ekin in com is " << Ekin_cm << endl;
  
  //reset total energy in com frame
    Etot_cm = Ekin_cm + p->getm() + (9. + fNn + fNp)*931.494028;
  //  Etot_cm = Ekin_cm + p->getm() + 10.012937*931.494028;

  //reset total energy of reaction product we are interested in
   p->setEtotal((Etot_cm*Etot_cm + p->getm()*p->getm() - ((9. + fNn + fNp)*931.494028*(9. + fNn + fNp)*931.494028))/(2.*Etot_cm));
   // p->setEtotal((Etot_cm*Etot_cm + p->getm()*p->getm() - (10.012937*931.494028*10.012937*931.494028))/(2.*Etot_cm));
  
  //cout << "My Etot for product is " << p->getEtotal() << endl;
  //cout << "My Ekin and Ekin/A and A are " << p->getEtotal() - p->getm() << " and "<< (p->getEtotal() - p->getm())/p->getA() << " and "<< p->getA() << endl;
/////////////////////////////////////////////////////////////////////?
//gaussian randomization of the angle
double Theta_cmp; 

  Theta_cmp = 0.;
  if (distSwitchBool == 2)
  {
	  if (!fRngGaussGlauber) ERR("Need fRngGaussGlauber");
	  do {
	    Theta_cmp = fRngGaussGlauber->value();
	    if (Theta_cmp > 1.0 || Theta_cmp < -1.0)
	      {
		Theta_cmp = -1.0;
	      } else {
//	      cout << "cos=  " <<Theta_cmp ;
	    Theta_cmp = acos(Theta_cmp);
	    //cout << Theta_cmp  <<endl;
	    }
	  } while (Theta_cmp > aUpLim || Theta_cmp < aLowLim);
	  // fprintf(opf1,"%10.3e\n", Theta_cmp); 	  
//	  cout << "theta=  "<< Theta_cmp  << endl;
  }
  else if (distSwitchBool == 1)
  {
	  if (!fRngGaussGlauber) ERR("Need fRngGaussGlauber");
	  do {
	    Theta_cmp = fRngGaussGlauber->value();
	  } while (Theta_cmp > aUpLim || Theta_cmp < aLowLim);
	  // fprintf(opf1,"%10.3e\n", Theta_cmp); 	  
 //cout << Theta_cmp << endl;
  }
  else if (distSwitchBool == 0)
  {
	 double cosTheta_cmp = fRngUniform->value(cos(aLowLim), cos(aUpLim));
	 Theta_cmp = acos(cosTheta_cmp);
	 // fprintf(opf1,"%10.3e\n", Theta_cmp); 	  
//	 cout << Theta_cmp << endl;
  }
  //debugging stuff
  if (debugSwitch == "thetaCMP")
	  cout << Theta_cmp<< endl;

 ////////////////////////////////////////////////////////////////////
  //randomize theta and phi in the CoM frame
  //double cosTheta_cmp = fRngUniform->value(0.9,1);       // cos(theta) should range -1 1
  //  double Theta_cmp = acos(cosTheta_cmp);
  // double Theta_cmp = 0.0;
  double Phi_cmp      = fRngUniform->value(0,2.*M_PI);  // phi
  //debugging stuff
  if (debugSwitch == "phiCMP")
	  cout << Phi_cmp<< endl;

  //calculate momentum of reaction product in com frame
  double P_cmp = sqrt(p->getEtotal()*p->getEtotal() - gsl_pow_2(p->getm()));
//cout << "My P total in com is " << P_cmp << endl;
//cout << "My total energy is " << p->getEtotal() << endl;

  //set 4-momentum in CoM frame
  tl.SetPxPyPzE(0., 0., P_cmp, p->getEtotal());

//cout << "My theta and phi are " << Theta_cmp * (180. / 3.14159) << " and " << Phi_cmp * (180. / 3.14159) << endl;
  //set the direction of the produced nucleus properly
  tl.SetTheta(Theta_cmp);
  tl.SetPhi(Phi_cmp);
/////////////////////////////// cout << Phi_cmp << endl; 
  //boost back to lab frame
  tl.Boost(t3Boost);
  
  //debugging stuff
  if (debugSwitch == "thetaLABP")
	  cout << tl.Theta() << endl;
  //debugging stuff
  if (debugSwitch == "phiLABP")
	  cout << tl.Phi()<< endl;
  //cout << "My lab energy and ke and ke/a and p are " << tl.E() << " and "<< tl.E() - p->getm() << " and " << (tl.E() - p->getm()) / p->getA() << " and " << endl; 
  
  //now calculate boost needed to move from CoM of reaction product to lab
  t3Boost = tl.BoostVector();

  /////////////////////////////////cout << tl.Phi() << endl;

  // start of neutron evaporation
  p->setA(p->getA() - 1);
  p->setm(p->getA()*931.494028);
  // p->setm(6.018889*931.494028);
  p->setEkin(0);               // 23O at rest

  TLorentzVector tl1;  // particle 1 (heavy fragment)
  tl1.SetPxPyPzE(p->getPx(), p->getPy(), p->getPz(), p->getEtotal());

  // create neutron
  StParticle pp;   
  pp.setA(1);
  // pp.setm(pp.getA()*939.565346);
      pp.setm(931.5);
  pp.setZ(0);
  pp.setQ(0);
  pp.setx(p->getx());     // same position
  pp.sety(p->gety()); 
  pp.setz(p->getz());     
  pp.sett(p->gett());     // same time
  pp.setEkin(0);
  
  TLorentzVector tl2;  // particle 2 (neutron)
  tl2.SetPxPyPzE(pp.getPx(), pp.getPy(), pp.getPz(), pp.getEtotal());
  
  double eN, eF;   // total neutron and fragment energy
  double eCM;   // total CM energy
  double exen;  // excitation energy

  exen = 0;
  if (!fRngExEn) ERR("Need fRngExEn");
  do {
    exen = fRngExEn->value();
  } while (exen < 0);
  DSV(exen);
  fExEnMap[0]=exen;

  //debugging stuff
  if (debugSwitch == "exen")
	  cout << exen<< endl;
  //  cout<<exen<<endl;

  eCM = pp.getm() + p->getm() + exen;  // total CM energy
  eN = gsl_pow_2(eCM) + gsl_pow_2(pp.getm()) - gsl_pow_2(p->getm());
  eN = eN/(2.*eCM);                    // total energy neutron
  eF = gsl_pow_2(eCM) - gsl_pow_2(pp.getm()) + gsl_pow_2(p->getm());
  eF = eF/(2.*eCM);                    // total energy of fragment

  /// \todo check is arg of sqrt is positive
  double pN,pF; // fragment and neutron momentum
  pN = eN*eN - gsl_pow_2(pp.getm());
  pN = sqrt(pN);
  pF = eF*eF - gsl_pow_2(p->getm());
  pF = sqrt(pF);
  pF = -pF;  // fragment goes in opposit direction

  tl1.SetPxPyPzE(0,0,pF,eF);
  tl2.SetPxPyPzE(0,0,pN,eN);
  
  // set theta and phi to some random value
  if (!fRngUniform) ERR("Need random number generator");
  
double  cosTheta = fRngUniform->value(-1,1);       // cos(theta)
double  theta    = acos(cosTheta);
//double  theta    = 0.0;
double  phi      = fRngUniform->value(0,2.*M_PI);  // phi
//double    phi    = 0.0;

  fThetaMap[0] = theta;
  fPhiMap[0]   = phi;

  tl1.SetTheta(theta);
  tl2.SetTheta(M_PI - theta);
  tl1.SetPhi(phi);

  ///////////////////cout << phi << endl;
  tl2.SetPhi(phi + M_PI);
  //cout << "My frag and neutron thetas are " << theta*(180./3.14159) << " and " <<  (M_PI - theta)*(180./3.14159)<< endl;

  // go to the lab
  tl1.Boost(t3Boost);
  tl2.Boost(t3Boost);

 //cout << tl1.Phi() << endl;
  // set fragment energy and direction
  p->setEtotal(tl1.E()); /// \todo implement setLVec() to set particle data from 4-vector
  p->setThetaPhi(tl1.Theta(),tl1.Phi()); 
  
  //cout << "My absolute final frag energy/A is " << (p->getEtotal() - p->getm()) / p->getA() << endl;
  //cout << (p->getEtotal() - p->getm()) / p->getA()<< endl;
  // set neutron energy and direction
  pp.setEtotal(tl2.E());
  pp.setThetaPhi(tl2.Theta(),tl2.Phi()); 
  //cout << "My absolute final neutron energy/A is " << pp.getEtotal() - pp.getm() << endl;
//cout  << pp.getEtotal() - pp.getm() << endl;
  if(! newPart) ERR("Pointer newPart not set; Cannot add new particles");
  newPart->push_back(pp); 

  DSV(p->getA());
  DSV(p->getZ());
  return 0;
}

int StReacStripEvapTwoBody::savedValue(size_t part_id, string name, double* v) {
  DSV(part_id);
  DSV(name);
  if (name == "exen" &&  (fExEnMap.find(part_id) != fExEnMap.end()) ) {
    *v = fExEnMap[part_id];
    DSV(*v);
    return 0;
  }
  if (name == "ph" &&  (fPhiMap.find(part_id) != fPhiMap.end()) ) {
    *v = fPhiMap[part_id];
    DSV(*v);
    return 0;
  }
  if (name == "th" &&  (fThetaMap.find(part_id) != fThetaMap.end()) ) {
    *v = fThetaMap[part_id];
    DSV(*v);
    return 0;
  }
  if (name == "pz" &&  (fPzMap.find(part_id) != fPzMap.end()) ) {
    *v = fPzMap[part_id];
    DSV(*v);
    return 0;
  }
  // fclose(opf1);
  return 1;
}
