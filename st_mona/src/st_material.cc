/** \file
 * Implementation of material classes.
 */

#include "st_material.hh"
#include "st_system.hh"
#include "st_histo_tuple.hh"
#include "st_reaction.hh"

#include "deb_err.h"
#include "CycSrim.h"
#include <fstream>
#include <iostream>
#include <iomanip>

ofstream fout;

void StMaterial::addElement(StElement e) {
    
    fout.open("outfile.txt");
    fElem.push_back(e);
    fTotalAb += e.geta();
}

  
void StMaterial::set(string mat) {  /// \todo load predefined material if known by name
    mat="";
    return;
}


int StMaterial::cleanUp() { 
    fEnergyLoss.clear();
    fTargetPos.clear();
    return 0;
}

void StMaterial::SetLifeTime(double t){
/* Original distribution from ZK's code */
  printf("LifeTime has been set to %f\n",t);
  TString formula = Form("exp(-1*(TMath::Log(2)/%f)*x) - exp(-1*(TMath::Log(2)/%f)*(x+0.00001))",t,t);
  printf("Decay calculation:  %s\n",formula.Data());
  fdecay = new TF1("fdecay",formula.Data(),0,t*20); 
  if(t==0.0) fdecay->SetName("zero_lifetime");
  fdecay->SetNpx(6000);

  /* Set upper bound for uniform distribution as twice the value set in the command line arguments */
  lt = t*2.;
}


int StMaterial::act(StParticle* p) {
    
    int doReaction;
    doReaction = 0;

  
#ifdef __WITH_FOIL
    // Calculate additional thickness due to bulge
    double bx; // beam spot x-pos  [m]
    double by; // beam spot y-pos  [m]
    double R;  // beam spot radius, R^2 = bx^2 + by^2 [m]
        bulge += 0.000000000001; // size of bulge [m] This is to prevent dividing by zero.
    
    double length = 0.019; // length from center of foil to edge [m] (1.9 cm)
    double density = 171.5; // density of D2 in kg/m^3
    double kdensity = 1420; // density of Kapton in kg/m^3

    double thickness = 0.0235; // [m] physical thickness of flat foil
    double bthick; // [mm] thickness accounting for bulge
    double dbthick; // [mm] total thickness of foils

    double dbulge = bulge + flatfoil; // [m] additional bulge due to thickness;

    double alpha; // angle [rad]
    double dalpha; // angle for foil
    double Rc; // distance from center of gas to where beam hits [m]
    double dRc; // distance from center of foil to where beam hits [m]   
    double tfoil;//
        tfoil += 0.000000000001;// again to vaoid dividing by zero
    double T; // total thickness foils included

    bx = p->getx();
    by = p->gety();


    R = sqrt(pow(bx,2)+pow(by,2));

    if(R <= length){// Check that beampspot is inside foil
        
        Rc = (pow(length,2)+pow(bulge,2))/(2*bulge);
        alpha = asin(R/Rc);
        
        bthick = 2*(Rc*(cos(alpha)-1) + bulge) + thickness;
    
        bthick = bthick*density*100; //[mg/cm^2];
        fThickness = bthick; // thickness of bulge
    
    
        // bulge from foil
    
        dRc = (pow(length+flatfoil,2)+pow(dbulge,2))/(2*dbulge);
        dalpha = asin(R/dRc);
        tfoil = dRc*(cos(dalpha) - 1) + dbulge - ( Rc*(cos(alpha) - 1) + bulge );
        tfoil = tfoil*kdensity*100; //[mg/cm^2]

    
        // Flat foil    
        fThickness = bthick + 2*tfoil; // thickness of bulge + foil  
        // NOTE: dbthick is thickness in mg/cm^2 of KAPTON assuming the entire target is this material (WITCH IT IS NOT!)
    }
    
  else{
    WAR("R value extends beyond physical target!");
  }
#endif

//  bool kPRINT = true;
  bool kPRINT = false;


  //in nanoseconds
  Double_t lifetime;
  TString name = fdecay->GetName();
  if(name.Contains("zero_lifetime")) lifetime = 0.0;
  else  lifetime = fdecay->GetRandom(); if(kPRINT){ printf("\nstandard LT distribution\n");}
//  else  lifetime = fRngUniform->value(0,lt); printf("\nuniform LT distribution\n");// THR 17 March 2017, set uniform lifetime dist.
  if(kPRINT){
    printf("lifetime is %f ns\n",lifetime);

    // THR 16 March 2017: write out lifetimes to check distribution
    ofstream lifetimes;
    lifetimes.open("/projects/mona-sim/baumann/lifetimes.out",std::ofstream::out | std::ofstream::app);
    lifetimes << lifetime << endl;
    lifetimes.close();
  }

  if (fReactions.size() && fTotReacProb > 0 ) {
      if (!fRngUniform) ERR("Need a uniform RNG since reactions are defined.");
      
      double r;
      r=fRngUniform->value(0,1);
      r=0;  DEB("remove this line"); /// \todo remove this line !!!
      DSV(r);
      DSV(fTotReacProb);
    
      if (r < fTotReacProb) doReaction = 1;
  }
  if(fActive){doReaction=1;}
  else{doReaction=0;}

  //____________________ NO REACTION ________________________
  if ( ! doReaction  ) {
      DEB("No reaction");
      if(kPRINT) printf("No Reaction.\n");
      
      double de, strag;
//      printf("\n thickness: %g\n",fThickness);
      if(fIsSi){de = fPMInt->do_de_foil(this,fThickness,p)*p->getA();}
      else {de = fPMInt->do_de(this,fThickness,p)*p->getA();}
      DSV(de);
      
      strag = fPMInt->stragAng(this, fThickness,p,fStragAngScale);
      DSV(strag);

      fEnergyLoss.push_back(de); // save energyLoss for later histo or tuple action
      fStragAng.push_back(strag);//
      fTargetPos.push_back(-7);  // no reaction -> no target position
    
      return 0;
  } 
  //____________________ YES REACTION _______________________
  else {

    DEB("Yes reaction");
    if(kPRINT) printf(" \n-->Let's do a reaction\n\n");
    
    if (!fRngThick) {
        WAR("Need random number generator for determining the interaction point in the target.\n"
            "None is set.  Using Uniform distribution.");
        fRngThick = fRngUniform;
    }


    double d;  // position where reaction happens
    double de,strag; // energy loss and strag angle (accumulative)
    d = fRngThick->value(0,fThickness);
//    d = 0.5*fThickness;

    //cout << "d: " << d << " bthick: " << bthick << " thickness: " << thickness << " fThickness " << fThickness << endl;
    if(kPRINT) printf("  \n-->Reaction occurs at d=%f of thickness=%f\n\n",d,fThickness);

    /* THR 17 March 2017: write out rxn points
    ofstream rxnpt;
    rxnpt.open("/projects/mona-sim/baumann/rxnpts.out",std::ofstream::out | std::ofstream::app);
    rxnpt << d << endl;
    rxnpt.close();
    */
    

#ifndef __WITH_FOIL
    // _______________________ CASE 0, NO Foil _______________________
    if(d > 0){ //cout << "NO FOIL" << endl;
        DSV(d);
        fTargetPos.push_back(d);  // save for histos/tuples
	// Write out dE for each particle THR 28 April 2017
	ofstream deout;
        deout.open("/projects/mona-sim/baumann/deout.out",std::ofstream::out | std::ofstream::app);
	// Columns are distance to rxn, beam dE, beam straggling, beam velocity,
	// distance to decay, fragment dE, fragment straggling, fragment velocity,
	// distance to exit, daughter dE, daughter straggling, daughter velocity,
	// total distance (should be target thickness), total dE, total straggling

        // energy loss and straggling before

	/* CycSrim dE calculator */
        de = fPMInt->do_de(this,d,p);
        de *= p->getA();

	
    if(kPRINT) printf("  beam de = %f\n  (it should be 0)\n",de);
	/* Simple dE Calculator 
	de = 0.603*d;// linearly interpolate between 0 and max dE for F-27 [MeV]
	double etotal   = p->getGammaMinOneU()*(p->getA());// total KE [MeV]
	double final_ke = (etotal - de)/(p->getA());// final KE [MeV/u]
	p->setGammaMinOneU(final_ke);
        //cout << "before reaction: " << de << " d: " << d << endl;
        if(kPRINT) printf("  \n-->Energy loss is %f\n\n",de);
	if(kPRINT) printf("  \n-->KE before rxn is %f\n",final_ke);
        if(kPRINT) printf("   -->Part Z:%d A:%d\n",p->getZ(),p->getA());
*/
        
        DSV(de);
    
        strag = fPMInt->stragAng(this,d,p,fStragAngScale);  /// \todo here the end energy is used to calc the straggling.  OK???
        DSV(strag);

	double beamVelocity = p->getBeta() * 29.9792458;
	// output beam dE information
	deout << setw(16) << d
		<< setw(16) << de
		<< setw(16) << strag
		<< setw(16) << beamVelocity;
    
        double r;   // random number for which reaction to choose
        double ps;  // probability sum
        r = fRngUniform->value(0,fTotReacProb);
        ps=0;

        vector<StReaction*>::const_iterator ireac;
    
        for (ireac = fReactions.begin(); ireac != fReactions.end(); ireac++) {
            DEB("Considering reaction %s.",(*ireac)->getName().c_str());
            ps += (*ireac)->getProb();
	    break;
	    //take 1st reaction ZK
	   // if (r < ps) break;
        }
    
        DEB("Using reaction %s.",(*ireac)->getName().c_str());
        if(kPRINT) printf("  Using reaction %s.\n",(*ireac)->getName().c_str());

        DSV(r); DSV(ps);
	// zwk force first reaction; THR 7 March 2017
        //if (r > ps) ERR("r > ps %f %f",r,ps);

        size_t id = p->getID();  /// need to save id, in case act screws up the pointer \todo fix this!!!
    
        vector<StParticle> newPart;
    
        DEB("Doing reaction %s.",(*ireac)->getName().c_str());
	if(kPRINT) printf("  Doing reaction %s.\n",(*ireac)->getName().c_str());

        (*ireac)->act(p, &newPart);
    
        // add new particles to the system
        StSystem* sp;
        sp = getSystemP(); 
    
        if (!sp) ERR("System pointer not set.");
    
        for (vector<StParticle>::const_iterator ppp = newPart.begin(); ppp != newPart.end(); ppp++) {
            //sp->addParticle(*ppp);// \todo do energy loss of new particles!!!
	    //Let's not add the "fake" neutron from glaub_nodecay
	    StParticle newp = *ppp;
	    if(kPRINT) printf("  **Particle Z:%d A:%d  NOT ADDED\n",newp.getZ(),newp.getA());
    
        }

        p = &(getSystemP()->getPartVectP()->at(id));  // p is now the new particle \todo should be done by the reaction???
	if(kPRINT) printf("   **Now p is Z:%d  A:%d\n",p->getZ(),p->getA());

        DEB("Done acting.");

	/* ---take into account lifetime of particle----*/
	double velocity = p->getBeta() * 29.9792458;
	double distance = velocity * lifetime;
	if(kPRINT) printf("  **Particle current velocity is %f cm/ns with lifetime of %f ns, distance traveled is %f cm\n",velocity, lifetime, distance);
	//cout << "  Init: " << velocity << endl;
	double distance_mgcm2 = 1.848 * distance * 1000;  //1.848 g/cm3 density of Be
	if(kPRINT) printf("  **  distance traveled in mg/cm2 of Be is %f\n",distance_mgcm2);
	double total_distance = d + distance_mgcm2;
	double distance_4afterdecay = 0;
	if(kPRINT) printf("  ** total distance is then %f and fthickness is %f\n",total_distance,fThickness);
	if(total_distance > fThickness) distance_mgcm2 = fThickness-d;
	else distance_4afterdecay = fThickness - total_distance;
	if(kPRINT) printf("  --> Thickness for energy loss is then %f mg/cm2\n",distance_mgcm2);

	/* CycSrim energy loss and straggling after */
	double tke = p->getGammaMinOneU()*(p->getA());// total KE after rxn
	DSV(distance_mgcm2);
	double o26de = fPMInt->do_de(this,distance_mgcm2,p) * p->getA();
	de += o26de;
	if(kPRINT) printf("  tke = %f; o26de = %f over %f mg/cm^2\n",tke,o26de,distance_mgcm2);
//	de += fPMInt->do_de(this,distance_mgcm2,p) * p->getA();
//	de = fPMInt->do_de(this,distance_mgcm2,p) * p->getA();// only take dE of fragment THR 20 March 2017
	

	/* Simple dE Calculator - oxygen 
	if(kPRINT) printf("\n  --> beam KE is %f\n",final_ke);
	double oxygen_de = final_ke*21.188 - 1403.9;
	de += oxygen_de;// set oxygen dE based on beam energy
	etotal   = p->getGammaMinOneU()*(p->getA());
	final_ke = (etotal - oxygen_de)/(p->getA());// final KE [MeV/u]
	p->setGammaMinOneU(final_ke);

	if(kPRINT) printf("  --> frag dE is %f\n",oxygen_de);
	if(kPRINT) printf("  --> frag energy after rxn is %f\n",etotal);
	if(kPRINT) printf("  \n --> After target, frag energy is %f\n",final_ke);
	if(kPRINT) printf("  --> Particle Z:%d A:%d\n",p->getZ(),p->getA());
*/


	/* Bethe dE Calculator - oxygen-26 
	double beta2 = p->getBeta() * p->getBeta();
	if(kPRINT) printf("\n  --> beam KE is %f\n",final_ke);
	if(kPRINT) printf("\n  --> fragment beta is %f\n",sqrt(beta2));
	const double Constant1 = 8.72277;
	const double Constant2 = 109626.;
	double coefficient = Constant1/beta2;
	double ln1 = Constant2*beta2; ln1 = log(ln1);
	double ln2 = log(1. - beta2);

	double o26de = coefficient * (ln1-ln2-beta2);
	o26de = o26de*distance_mgcm2/1000.;

	de += o26de;// set oxygen dE based on beam energy
	etotal   = p->getGammaMinOneU()*(p->getA());
	final_ke = (etotal - o26de)/(p->getA());// final KE [MeV/u]
	p->setGammaMinOneU(final_ke);

	if(kPRINT) printf("  --> frag dE is %f\n",o26de);
	if(kPRINT) printf("  --> frag energy after rxn is %f\n",etotal);
	if(kPRINT) printf("  \n --> After rxn, frag energy is %f\n",final_ke);
	if(kPRINT) printf("  --> Particle Z:%d A:%d\n",p->getZ(),p->getA());
*/


	double o26st = fPMInt->stragAng(this,distance_mgcm2,p,fStragAngScale);
	strag += o26st;
//	strag += fPMInt->stragAng(this,distance_mgcm2,p,fStragAngScale);
	
	double o26Velocity = p->getBeta() * 29.9792458;
	// output fragment dE information
	deout << setw(16) << distance_mgcm2
		<< setw(16) << o26de
		<< setw(16) << o26st
		<< setw(16) << o26Velocity;


//        fTargetPos.push_back(distance_mgcm2);  // save for histos/tuples

	if(kPRINT) printf(" -->Finish energy loss of p, de=%f throgh thick-d=%f\n",o26de, distance_mgcm2);
	DSV(strag);


	if(kPRINT) printf(" Next Reaction______\n");
	ireac++;
	if(kPRINT) printf("  Using reaction %s.\n",(*ireac)->getName().c_str());

	id = p->getID();  /// need to save id, in case act screws up the pointer \todo fix this!!!
	vector<StParticle> newPart2;
	DEB("Doing reaction %s.",(*ireac)->getName().c_str());
	if(kPRINT) printf("  Doing reaction %s.\n",(*ireac)->getName().c_str());
	(*ireac)->act(p, &newPart2);
	// add new particles to the system
	//StSystem* sp;
	sp = getSystemP(); 
	if (!sp) ERR("System pointer not set.");
        for (vector<StParticle>::const_iterator ppp = newPart2.begin(); ppp != newPart2.end(); ppp++) 
	  {
	    sp->addParticle(*ppp);
	    StParticle newp = *ppp;
	    if(kPRINT) printf("  **Particle Z:%d A:%d  HAS BEEN ADDED\n",newp.getZ(),newp.getA());
	    //// \todo do energy loss of new particles!!!
	  }
	p = &(getSystemP()->getPartVectP()->at(id));  // p is now the new particle \todo should be done by the reaction???

	if(kPRINT) printf("   **Now p is Z:%d  A:%d\n",p->getZ(),p->getA());


	//double velocityMid = p->getBeta() * 29.9792458;
	//cout << "  RxnVel: " << velocityMid << endl;

	DEB("Done acting.");

	double o24de=-999;
	double o24st=-999;
	double o24Velocity=-999;
	double o24dist=-999;
	if(kPRINT) printf(" Now Check to see if need to do more energy loss\n");
	if(kPRINT) printf(" distance_4afterdecay = %f\n",distance_4afterdecay);
	if(distance_4afterdecay > 0){
	
          /* CycSrim energy loss and straggling after */
	  tke = p->getGammaMinOneU()*(p->getA());// tke after decay
          DSV(fThickness-d); 
	  o24de = fPMInt->do_de(this, distance_4afterdecay,p)* p->getA(); // rest of target
//	  de+=o24de;
	  de=o24de;

//	  if(kPRINT) printf("  tke = %f; o24de = %f over %f mg/cm^2\n",tke,o24de,distance_4afterdecay);
//          de += fPMInt->do_de(this, distance_4afterdecay,p)* p->getA(); // rest of target
          //cout << "after reaction: (total) " << de << " t: " << fThickness - d <<  endl;
	  o24st = fPMInt->stragAng(this, distance_4afterdecay,p,fStragAngScale);
	  strag += o24st;
//          strag +=fPMInt->stragAng(this, distance_4afterdecay,p,fStragAngScale);
          o24dist = distance_4afterdecay; 
          DSV(strag);
          DSV(de);

	  o24Velocity = p->getBeta() * 29.9792458;

	  /* Simple dE Calculator - oxygen-24 
	  if(kPRINT) printf("\n  --> beam KE is %f\n",final_ke);
	  o24de = final_ke*21.188 - 1403.9;
	  de += o24de;// set oxygen dE based on beam energy
	  etotal   = p->getGammaMinOneU()*(p->getA());
	  final_ke = (etotal - o24de)/(p->getA());// final KE [MeV/u]
	  p->setGammaMinOneU(final_ke);
*/
	  /* Bethe dE Calculator - oxygen-24 
	  beta2 = p->getBeta() * p->getBeta();
	  if(kPRINT) printf("\n  --> frag KE is %f\n",final_ke);
	  if(kPRINT) printf("\n  --> daughter beta^2 is %f\n",beta2);

	  coefficient = Constant1/beta2;
	  ln1 = Constant2*beta2; ln1 = log(ln1);
	  ln2 = log(1. - beta2);

	  o24de = coefficient * (ln1-ln2-beta2);
	  o24de = o24de*distance_4afterdecay/1000.;

	  de += o24de;// set oxygen dE based on beam energy
	  etotal   = p->getGammaMinOneU()*(p->getA());
	  final_ke = (etotal - o24de)/(p->getA());// final KE [MeV/u]
	  p->setGammaMinOneU(final_ke);

	  o24Velocity = p->getBeta() * 29.9792458;

	  if(kPRINT) printf("  --> daughter dE is %f\n",o24de);
	  if(kPRINT) printf("  --> daughter energy after target is %f\n",etotal);
	  if(kPRINT) printf("  \n --> After target, frag energy is %f\n",final_ke);
	  if(kPRINT) printf("  --> Particle Z:%d A:%d\n",p->getZ(),p->getA());
*/
	}
	/**/
	else{
	  if(kPRINT) printf("\nFragment must have traveled throught target before decaying!\n");
	}

        // output daughter dE information
	deout << setw(16) << distance_4afterdecay
		<< setw(16) << o24de
		<< setw(16) << o24st
		<< setw(16) << o24Velocity;
	

	
    
        fEnergyLoss.push_back(de);
        fStragAng.push_back(strag);

/**/
	double velocityEnd = p->getBeta() * 29.9792458;
	// output final dE information (sum of all processes)
	deout << setw(16) << d+distance_mgcm2+distance_4afterdecay
		<< setw(16) << de
		<< setw(16) << strag
		<< setw(16) << velocityEnd << endl;
	deout.close();

	if(kPRINT) cout << "\nEnd: " << velocityEnd << endl;
	if(kPRINT) printf("\n------------FINISHED EVENT-----------------\n\n");

    }// End Case 0

#endif






#ifdef __WITH_FOIL
    // 3 cases are neccessary to account for the possible reaction points:
    // 		the reaction can happen in the foil, then propogate through D2 and more foil
    // 		or it can pass through foil, happen in D2, and pass through foil again
    // 		or pass through foil AND D2 and happen in the exiting foil

    // _______________________ CASE 1, Reaction In Front Foil _______________________
    if(d <= tfoil && tfoil >= 0){ cout << "CASE1" << endl;
        DSV(d);
        fTargetPos.push_back(d);  // save for histos/tuples
        // energy loss and straggling before
    
        de = fPMInt->do_de_foil(this,d,p);
        de *= p->getA(); 
        DSV(de);
    
        strag = fPMInt->stragAng(this,d,p,fStragAngScale);  /// \todo here the end energy is used to calc the straggling.  OK???
        DSV(strag);
    
        double r;   // random number for which reaction to choose
        double ps;  // probability sum
        r = fRngUniform->value(0,fTotReacProb);
        ps=0;

        vector<StReaction*>::const_iterator ireac;
    
        for (ireac = fReactions.begin(); ireac != fReactions.end(); ireac++) {
            DEB("Considering reaction %s.",(*ireac)->getName().c_str());
            ps += (*ireac)->getProb();
            
            if (r < ps) break;
    
        }
    
        DEB("Using reaction %s.",(*ireac)->getName().c_str());
        DSV(r); DSV(ps);

        if (r > ps) ERR("r > ps %f %f",r,ps);

        size_t id = p->getID();  /// need to save id, in case act screws up the pointer \todo fix this!!!
    
        vector<StParticle> newPart;
    
        DEB("Doing reaction %s.",(*ireac)->getName().c_str());
        (*ireac)->act(p, &newPart);
    
        // add new particles to the system
        StSystem* sp;
        sp = getSystemP(); 
    
        if (!sp) ERR("System pointer not set.");
    
        for (vector<StParticle>::const_iterator ppp = newPart.begin(); ppp != newPart.end(); ppp++) {
            sp->addParticle(*ppp);// \todo do energy loss of new particles!!!
    
        }

        p = &(getSystemP()->getPartVectP()->at(id));  // p is now the new particle \todo should be done by the reaction???
        DEB("Done acting.");

        // energy loss and straggling after
        DSV(fThickness-d); 
        de += fPMInt->do_de_foil(this, tfoil-d,p)* p->getA(); // from d to end of foil
        strag +=fPMInt->stragAng(this,tfoil-d,p,fStragAngScale);
    
        de += fPMInt->do_de(this,bthick,p) * p->getA(); // from foil through D2
        strag +=fPMInt->stragAng(this,bthick,p,fStragAngScale);
    
        de += fPMInt->do_de_foil(this,tfoil,p)*p->getA();
        strag += fPMInt->stragAng(this,tfoil,p,fStragAngScale);
    
        DSV(strag);
        DSV(de);
    
        fEnergyLoss.push_back(de);
        fStragAng.push_back(strag);
  
    }// End Case 1

    //__________________________ CASE 2 Reaction Happens In D2
    else if(tfoil < d  && d <= bthick + tfoil && tfoil >= 0){ cout << "CASE2" << endl;
        DSV(d);
        fTargetPos.push_back(d);  // save for histos/tuples
        // energy loss and straggling before
        de = fPMInt->do_de_foil(this,tfoil,p); // propagate through the foil
        strag = fPMInt->stragAng(this,tfoil,p,fStragAngScale); 
        cout << "energy loss in foil: " << de << " tfoil: " << tfoil << endl;

        de *= p->getA(); 
        de += fPMInt->do_de(this,d-tfoil,p)*p->getA(); // propagate up to the reaction point  
        DSV(de);
        cout << "energy loss of beam upto reaction point: (foil included) " << de << " d-tfoil: " << d-tfoil << endl;

        strag += fPMInt->stragAng(this,d-tfoil,p,fStragAngScale);  /// \todo here the end energy is used to calc the straggling.  OK???
        DSV(strag);

        double r;   // random number for which reaction to choose
        double ps;  // probability sum
        r = fRngUniform->value(0,fTotReacProb);
        ps=0;

        vector<StReaction*>::const_iterator ireac;
    
        for (ireac = fReactions.begin(); ireac != fReactions.end(); ireac++) {
            DEB("Considering reaction %s.",(*ireac)->getName().c_str());
            ps += (*ireac)->getProb();    
        
            if (r < ps) break;
        }
    
        DEB("Using reaction %s.",(*ireac)->getName().c_str());
        DSV(r); DSV(ps);

        if (r > ps) ERR("r > ps %f %f",r,ps);

        size_t id = p->getID();  /// need to save id, in case act screws up the pointer \todo fix this!!!
    
        vector<StParticle> newPart;
    
        DEB("Doing reaction %s.",(*ireac)->getName().c_str());
        (*ireac)->act(p, &newPart);
    
        // add new particles to the system
        StSystem* sp;
        sp = getSystemP(); 
    
        if (!sp) ERR("System pointer not set.");
    
        for (vector<StParticle>::const_iterator ppp = newPart.begin(); ppp != newPart.end(); ppp++) {
            sp->addParticle(*ppp);// \todo do energy loss of new particles!!!

        }

        p = &(getSystemP()->getPartVectP()->at(id));  // p is now the new particle \todo should be done by the reaction???
        DEB("Done acting.");  
        DSV(fThickness-d); 
    
        de += fPMInt->do_de(this, bthick-(d-tfoil),p)* p->getA(); // through remaining D2
        strag += fPMInt->stragAng(this,bthick-(d-tfoil),p,fStragAngScale);
        cout << "through remaining target: " << de << " bthick-(d-tfoil) " << bthick-(d-tfoil) << endl;
    
        de += fPMInt->do_de_foil(this,tfoil,p)*p->getA(); // through foil again
        strag += fPMInt->stragAng(this,tfoil,p,fStragAngScale);
        cout << "though remaining foil: " << de << " tfoil: " << tfoil << endl;
    
        DSV(strag);
        DSV(de);
    
        fEnergyLoss.push_back(de);
        fStragAng.push_back(strag);

    }// End Case 2

    // ______________________ CASE 3, Reaction In Back Foil _________________________
    else if(d >= bthick + tfoil && tfoil >= 0){ cout << "CASE3" << endl;
        DSV(d);
        fTargetPos.push_back(d);  // save for histos/tuples
    
        // energy loss and straggling before
        de = fPMInt->do_de_foil(this,tfoil,p); // propagate through the foil
        strag = fPMInt->stragAng(this,d,p,fStragAngScale); 
    
        //de = fPMInt->do_de(this,d,p);
        de *= p->getA(); 
        de += fPMInt->do_de(this,bthick,p) * p->getA(); // propagate through the D2
    
        strag += fPMInt->stragAng(this,bthick,p,fStragAngScale); 
        de += fPMInt->do_de_foil(this,d-bthick-tfoil,p) * p->getA();// propagate through back foil
    
        strag += fPMInt->stragAng(this,tfoil-bthick-tfoil,p);  

        DSV(de);
        DSV(strag);

    
        double r;   // random number for which reaction to choose
        double ps;  // probability sum
        r = fRngUniform->value(0,fTotReacProb);
        ps=0;

        vector<StReaction*>::const_iterator ireac;
    
        for (ireac = fReactions.begin(); ireac != fReactions.end(); ireac++) {
            DEB("Considering reaction %s.",(*ireac)->getName().c_str());
            ps += (*ireac)->getProb();
        
            if (r < ps) break;
        }

        DEB("Using reaction %s.",(*ireac)->getName().c_str());
        DSV(r); DSV(ps);

        if (r > ps) ERR("r > ps %f %f",r,ps);

        size_t id = p->getID();  /// need to save id, in case act screws up the pointer \todo fix this!!!
    
        vector<StParticle> newPart;
    
        DEB("Doing reaction %s.",(*ireac)->getName().c_str());
        (*ireac)->act(p, &newPart);
    
        // add new particles to the system
        StSystem* sp;
        sp = getSystemP(); 
    
        if (!sp) ERR("System pointer not set.");
        
        for (vector<StParticle>::const_iterator ppp = newPart.begin(); ppp != newPart.end(); ppp++) {
            sp->addParticle(*ppp);// \todo do energy loss of new particles!!!
    
        }
    
        p = &(getSystemP()->getPartVectP()->at(id));  // p is now the new particle \todo should be done by the reaction???
        DEB("Done acting.");

        // energy loss and straggling after
        DSV(fThickness-d); 
    
        de += fPMInt->do_de_foil(this, fThickness-d,p)* p->getA(); // through remaining foil
        strag += fPMInt->stragAng(this,fThickness-d,p,fStragAngScale);
    
        DSV(strag);
        DSV(de);
    
        fEnergyLoss.push_back(de);
        fStragAng.push_back(strag);


    }// End Case 3
#endif


    else{
        //cout << "d: " << d <<  " tfoil: " << tfoil << " bthick: " << bthick << " fThickness: " << fThickness << endl;
        ERR("distance is outside target!"); 
    }
 
  } // yes reaction bracket
  return 0;
}



int StMaterial::fillSpecialNtupFields(string n, int pn) {
    string nn = n.substr(n.find("-")+1);
  
    if (nn == "dE") {
        getSystemP()->getTupleP()->fillField(n, fEnergyLoss.at(pn));
        return 0;
  
    }
  
    if (nn == "TP") {
        getSystemP()->getTupleP()->fillField(n, fTargetPos.at(pn));
        return 0;
  
    }
  
    if (nn.find("R_") != string::npos) {
        string nnn=nn.substr(nn.find("R_")+2); //cout << "string: " << nnn << endl;
        DSV(nn);
        DSV(nnn);
    
        for (vector<StReaction*>::const_iterator reacI=fReactions.begin(); reacI != fReactions.end(); reacI++){
            double v;
      
            if ( (*reacI)->savedValue(pn,nnn,&v)) return 1;
	  
            DSV(v);
            getSystemP()->getTupleP()->fillField(n, v);
      
            return 0;
        }
    }
  
    return 1;
}



void StMaterial::addReaction(StReaction* r) {
//    if (fReactions.size() > 0) ERR("Only one reaction allowed currently");
    // zwk allow more reactions
    fReactions.push_back(r);
    fTotReacProb += r->getProb();
}


