#include "st_par_mat_ia.hh"
#include "deb_err.h"
#include "CycSrim.h"
#include <iostream>


using namespace std;
double StParMatInteraction::do_de(StMaterial* m, StParticle* p, int range) {
  return do_de(m, m->getThickness(), p, range);
}

double StParMatInteraction::stragAng(StMaterial* m, StParticle* p) {
  return stragAng(m,m->getThickness(),p);
}

double StRageny::do_de(StMaterial* m, double thickness, StParticle* p, int range) {

/*
  //----------------------------------------------\\
  ||          HERE LIES THE RAGENY ENERGY 	      ||
  ||               LOSS INTERPOLATOR. 		      ||
  ||         IN COMMEMORATION OF IT'S BRAVE       ||
  ||       SERVICE TO THE MONA COLLABORATION      ||
  ||                    R.I.P.  	              ||
  ||                  10/10/2012                  ||
  \\----------------------------------------------// 
*/

/*
  DSV(p->getA());
  DSV(p->getZ());
  DSV(p->getGammaMinOneU());
  DSV(thickness);
  DSV(range);


  if (thickness < 0)  ERR("thickness < 0");
  if (thickness == 0) return 0;
  
  rageny_target_t t;
  double de;
  /////////////////////////////////////////////////////
   


  if (p->getZ() == 0) {  // infinite range for not charged particles
    fData.ti = fData.tf = p->getGammaMinOneU();
    double zero(0);
    fData.ri = fData.rf = 1./zero;
    return 0;
  }

  if (p->getZ() < 0) ERR("Don't know how to deal with negatively charged partiles");
  if (p->getA() <= 0) ERR("Don't know how to deal with particles with A <= 0");

  fData.ti = p->getGammaMinOneU(); // ti should be MeV/u
  fData.f  = rageny_ENERGY;
  if (range) {
    fData.f  = rageny_RANGE;
    fData.ri = thickness;
  }
  
  memset(&t,0,sizeof(t));
  t.n = m->getElemP()->size();  // number of elements

  /// \todo check that t.n is not larger than MAXMAT from rageny!!!
  for (int i=0; i<t.n; i++) {
    t.a[i] = m->getElemP()->at(i).m;
    t.z[i] = m->getElemP()->at(i).Z;
    t.r[i] = m->getElemP()->at(i).a;
  }
  t.t = thickness;

  if (fLookUp) {
    RagenyTargetBeam bbb;
    t.t    = 1;  // need to use fixed thickness
    bbb.t  = t;  // target
    bbb.ap = p->getA();  // beam A
    bbb.zp = p->getZ();  // beam Z

    DEB("bbb");
    DSV(bbb.t.t);
    DSV(bbb.t.n);
    DSV(bbb.t.a[0]);
    DSV(bbb.t.z[0]);
    DSV(bbb.t.r[0]);
    DSV(bbb.ap);
    DSV(bbb.zp);

    DSV(p->getA());
    DSV(p->getZ());

    DSV(this);
    DSV(fTargetBeamTable.size());

    StInterp* interp = fTargetBeamTable[bbb];
    if (!interp) {
      INFO("Setting up interpolator for energy loss for beam A Z: %.1f %.1f", bbb.ap, bbb.zp);
      interp = new StInterpLin();
      fTargetBeamTable[bbb] = interp;
      // add data points to range table
      double enOrig = fData.ti;
      double raOrig = fData.ri;
      interp->add(0,0);  //  zero range <==> zero energy
      for (fData.ti=1; fData.ti < enOrig*2; fData.ti*=1.01){
        fData.f = rageny_ENERGY;
        de = rageny_calc(p->getA(), p->getZ(), &t, &fData);
        DSV(de);
        DSV(fData.ti);
        DSV(fData.ri);
        interp->add(fData.ti,fData.ri);
      }
      fData.ti = enOrig;
      fData.ri = raOrig;
      interp->setup();// do setup
#ifdef DEBUG
      interp->print();
#endif
      INFO("Done");
    } else {
      DEB("Using existing interpolator");
    }

    if (range) {
      fData.ti = interp->inv(fData.ri);
    } else {
      fData.ri = (*interp)(fData.ti);
    }
    DSV(fData.ti);
    DSV(fData.ri); 
    DSV(thickness);
    fData.rf = fData.ri - thickness;
    if (fData.rf < 0) {
      WAR("fData.rf < 0; %g (setting to zero)",fData.rf);
      fData.rf = 0;
    }

    fData.tf = interp->inv(fData.rf);
    DSV(fData.tf);
    DSV(fData.rf); 
   
    de = fData.ti - fData.tf; 
    DSV(de);
  } else {
    
    de = rageny_calc(p->getA(), p->getZ(), &t, &fData);   }
   p->setGammaMinOneU(fData.tf);  // set MeV/u
  
    return de;*/


  if (thickness < 0)  ERR("thickness < 0");
  if (thickness == 0) return 0;

  if (p->getZ() < 0) ERR("Don't know how to deal with negatively charged partiles");
  if (p->getA() <= 0) ERR("Don't know how to deal with particles with A <= 0");

  /* e15118 specific Energy Loss THR 26 July 2017 
  double dE = 0.;
  double etotal = p->getGammaMinOneU();// KE in [MeV/u]
  if (p->getA() == 27){
    dE = 16.68 / pow(etotal,0.769);
    dE *= thickness;
    dE = dE/(p->getA());
  }
  if (p->getA() == 24 || p->getA() == 26){
    dE = 12.79 / pow(etotal,0.762);
    dE *= thickness;
    dE = dE/(p->getA());
  }

  double final_ke = etotal - dE;
  p->setGammaMinOneU(final_ke);
*/

  /* CycSrim Energy Loss*/

  double dE; // energy loss
  double etotal = p->getGammaMinOneU()*(p->getA());// total kinetic energy 
  fSrimTarget->SetThickness(thickness); 
  dE = fSrimTarget->CycSrim::GetEnergyLoss(p->getZ(),p->getA(),etotal); 
  dE = dE/(p->getA()); // MeV/u  could also do dE =/ p->getA()
  double final_ke = etotal/(p->getA()) - dE; // calculate final kinetic energy [MeV/u]
  p->setGammaMinOneU(final_ke);

//   cout << "T:" << thickness << " E: " << etotal << " dE " << dE*p->getA() << " A: " << p->getA() << " Z " << p->getZ() << endl;

   return dE;  
} 

// Energy loss through foils 
double StRageny::do_de_foil(StMaterial* m, double thickness, StParticle* p){
	if (thickness < 0)  ERR("thickness < 0");
  	if (thickness == 0) return 0;

  	if (p->getZ() < 0) ERR("Don't know how to deal with negatively charged partiles");
 	if (p->getA() <= 0) ERR("Don't know how to deal with particles with A <= 0");

/* Simpler way to calculate dE through the Si detectors of the segmented target, THR 25 July 2017
 * fitting the results of a TRIM calculation gives the functional form 15.06 / x^0.751 for dE/dx(Elec)
 * [MeV/mg/cm2 Si] vs. F-27 energy in [MeV/u]
 *
 * Here i will switch on the particle A and calculate the energy loss from these fits
 * WARNING!!! THIS METHOD SHOULD NOT BE USED FOR IONS OTHER THAN F-27 AT ENERGIES IN THE RANGE
 * 40 - 210 MEV/U
*/

  double dE = 0.;
  double etotal = p->getGammaMinOneU();// KE in [MeV/u]
  if (p->getZ() == 11 && p->getA() == 30){
    dE = 22.3874 / pow(etotal,0.744295);
    dE *= thickness;
    dE  = dE/(p->getA());
  }
  if (p->getZ() == 10 && p->getA() == 28){
    dE = 18.2143 / pow(etotal,0.746891);
    dE *= thickness;
    dE = dE/(p->getA());
  }
  if (p->getZ() == 10 && p->getA()==29){
    dE = 18.2621 / pow(etotal,0.747282);
    dE *= thickness;
    dE = dE/(p->getA());
  }
  if (p->getZ() == 9 && p->getA()==27){
    dE = 15.06 / pow(etotal,0.751);
    dE *= thickness;
    dE = dE/(p->getA());
  }
  if (p->getZ() == 8){
    dE = 11.49 / pow(etotal,0.742);
    dE *= thickness;
    dE = dE/(p->getA());
  }
  // THR 14 February 2018 added fits to Be-14 and Be-12 dE/dx vs Einc
  // to calculate energy loss in silicon
  if (p->getZ() == 4 && p->getA()==14){
    dE = 3.26485 / pow(etotal,0.778487);
    dE *= thickness;
    dE = dE/(p->getA());
  }
  if (p->getZ() == 4 && p->getA()==12){
    dE = 3.14257 / pow(etotal,0.768608);
    dE *= thickness;
    dE = dE/(p->getA());
  }


  double final_ke = etotal - dE;
  p->setGammaMinOneU(final_ke);
/* Original way (prior to 25 July 2017) of doing dE
     double etotal = p->getGammaMinOneU()*(p->getA());
     fSrimFoil->SetThickness(thickness);
     double dE = fSrimFoil->CycSrim::GetEnergyLoss(p->getZ(),p->getA(),etotal);
     dE = dE/(p->getA());
     double final_ke = etotal/(p->getA()) - dE;
     p->setGammaMinOneU(final_ke);
*/


     //return 0;
     return dE;
}


// CYCSRIM range and dE

double StRageny::range(CycSrim* fSrimTarget, StParticle* p) {
  double etotal = p->getGammaMinOneU()*(p->getA());
  double range = fSrimTarget->GetRange(p->getZ(),p->getA(),etotal,kTRUE);
  return range;
}

double StRageny::energy(CycSrim* fSrimTarget, double thickness, StParticle* p) {
  double etotal = p->getGammaMinOneU()*(p->getA());
  fSrimTarget->SetThickness(thickness);
  double dE = fSrimTarget->CycSrim::GetEnergyLoss(p->getZ(),p->getA(),etotal);
  //double dE = fSrimTarget->CycSrim::GetEnergyLossFromResidual(p->getZ(),p->getA(),etotal);
  dE = dE/(p->getA());
  return dE;
}


double StRageny::range_foil(CycSrim* fSrimTarget, StParticle* p) {
  double etotal = p->getGammaMinOneU()*(p->getA());
  double range = fSrimTarget->GetRange(p->getZ(),p->getA(),etotal,kTRUE);
  return range;
}

double StRageny::energy_foil(CycSrim* fSrimTarget, double thickness, StParticle* p) {
  double etotal = p->getGammaMinOneU()*(p->getA());
  fSrimTarget->SetThickness(thickness);
  double dE = fSrimTarget->CycSrim::GetEnergyLoss(p->getZ(),p->getA(),etotal);
  //double dE = fSrimTarget->CycSrim::GetEnergyLossFromResidual(p->getZ(),p->getA(),etotal);
  dE = dE/(p->getA());
  return dE;
}





/*
// RAGNEY range and dE
double StRageny::range(StMaterial *m, StParticle* p) {
  DEB("start");
  do_de(m,1,p); 
  DSV(fData.ri);
  return fData.ri;
}

double StRageny::energy(StMaterial* m, double thickness, StParticle* p) {
  DEB("start");
  do_de(m,thickness,p,1);   
  DSV(fData.ti);
  return fData.ti;
}*/

// Angular straggling
double StRageny::stragAng(double z1, double z2, double t, double A2, double e) {
  /// according to R. Anne et al., NIMB 34, 295 (1988)  [Equations (18) and (19)]

  double z12sum;  // term in brackets
  double tau; // reduced target thickness

  double red_alpha; // reduces strag angle
  double alpha;     // strag angle

  z12sum = cbrt(z1*z1) + cbrt(z2*z2);
  tau = 41.5 * 1000. * t / A2 / z12sum;
  DSV(z1);
  DSV(z2);
  DSV(t);
  DSV(A2);
  DSV(tau);

  red_alpha = pow(tau,0.55);  // Equation (21)
  DSV(red_alpha);

  DSV(e);
  alpha = red_alpha * z1 * z2 * sqrt(z12sum) / 16.26 / e;
  DEB("alpha HWHM (mrad): %g",alpha);

  // according to the paper this value is HWHM, i.e. half the FWHM
  alpha *= 2;      // FWHM
  alpha /= 2.355;  // sigma
  DEB("alpha sigma (mrad): %g",alpha);
    
  alpha /= 1000;  // conversion from mrad to rad

  return alpha;
}

double StRageny::stragAng(StMaterial* m, double thickness, StParticle* p, double scale) {
  /// according to R. Anne et al., NIMB 34, 295 (1988)
  if (!m) ERR("Paterial pointer not set");
  if (!p) ERR("Particle pointer not set");

  if ( m->getElemP()->size() != 1 ) {
    WAR("Straggling for composites not yet implemented: n = %d", m->getElemP()->size());
  }
  double z1,z2;
  double t;
  double A;  // target mass number!!!
  double e;
  z1 = p->getZ();
  z2 = m->getElemP()->at(0).getZ();
  t = thickness;
  A = m->getElemP()->at(0).getm();  // mass number!!! getm() gives molar mass!!!
  e = p->getEkin();
  
  DSV(z1);
  DSV(z2);
  DSV(t);
  DSV(A);
  DSV(e);
  double alpha;
  alpha = stragAng(z1,z2,t,A,e);
  alpha *= scale;   // scale with scale parameter

  // choose theta according to gauss
  fRngGauss->setCentroid(0);
  fRngGauss->setSigma(alpha);
  double theta, phi;
  theta = fRngGauss->value();
  phi   = fRngUniform->value(0,2*M_PI);
  DSV(theta);
  DSV(phi);

  // choose phi uniformly
  p->addThetaPhi(theta,phi);

  return theta;
}



// Test of Ragney interpolater

#ifdef TEST
// compile as:
// g++ -DTEST -DDEBUG st_par_mat_ia.cc -lm st_interp_invers.o $(gsl-config --libs) rageny.o -lg2c st_particle.o  $(root-config --libs) -Wl,-rpath=$(root-config --libdir)
// run: ./a.out

//#include <iostream>

int main () {
  StRageny s;
  cout<<endl<<"Test of stragling calculation"<<endl<<endl;
  cout<<"Test of straggling for 16O on 9Be with energy per A of 26.3"<<endl;
  cout<<"Result should be (according to Anne et al. Table 2 first row):"<<endl;
  cout<<"tau: 31500     alpha_12_bar: 300    alpha (HWHM): 3.5 mrad"<<endl;
  cout<<"E=26.3*16 result="<<s.stragAng(8,4,44.5,9,26.3*16)<<endl;

  cout<<endl;

  cout<<"Test of energy loss calculation"<<endl<<endl;

  StElement Be9;
  Be9.setZ(4);
  Be9.setm(9);

  StMaterial* m= new StMaterial();
  m->addElement(Be9);
  m->setThickness(10);
  
  // check range energy table
  StRageny rag(0), ragwl(1);  // rageny with and w/o lookup table
  StParticle p;
  p.setA(26);
  p.setm(p.getA()*931.494028);
  p.setZ(10);
  //  p.setQ(p.getZ());
  double en;
  for (en=100; en>1; en*=.9) { // loop over energy 
    DSV(en);
    double r1,r2,d1,d2;

    p.setGammaMinOneU(en);   // ranges
    r1 = rag.range  (m,&p);
    p.setGammaMinOneU(en);
    r2 = ragwl.range(m,&p);

    p.setGammaMinOneU(en);   // energy loss
    d1 = rag.do_de  (m, m->getThickness(), &p);
    p.setGammaMinOneU(en);
    d2 = ragwl.do_de(m, m->getThickness(), &p);

    DSV(r1);
    DSV(r2);
    
    printf("%6.2f %9.3f  %9.3f %6.3f   %6.3f (%7.3f) %6.3f (%7.3f) %6.3f  \n",en,r1,r2,100*(r2/r1-1),d1,d1*p.getA(),d2,d2*p.getA(),100*(d2/d1-1));
  }
  
  m->setThickness(1);
  for (double t=1000; t<2000; t++) {
    double de;
    double r;
    p.setEkin(t);
    de = ragwl.do_de(m, m->getThickness(), &p);
    de *= p.getA();
    p.setEkin(t);
    r = ragwl.range(m,&p);
    DSV(ragwl.energy(m,r-100,&p));
    printf("%g %10.2f %10.5f \n",t,r,de);
  }

  return 0;
}

#endif


