  //ANK i think maybe i add the one neutron stripping here
  if(fWhichEnergyDoIReconstruct=="4body_constrained"){

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
  t1gev.SetVectM(vP*(1/1000.), (set_mass+exenTotal)/1000.);   //ANK: need to define exenTotal (should be 3-body decay energy-> difference between 4-body and 2-body energies)

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
    double rel_weight = weight / wtmax;
    double ran = fRngUniform->value(0,1);

    //events are weighted (rel_weight) between 0 and 1
    //therefore only select event when ran is less then rel_weight
    if(fWeight > 0){ 
      if(ran < rel_weight) break;
     }
    break; 
  }
  pt1 = tPSevent.GetDecay(0); //frag - 2n
  pt2 = tPSevent.GetDecay(1); //neutron1
  pt3 = tPSevent.GetDecay(2); //neutron2

  //The returned Lorentz vectors are automatically boosted into the frame
  //of the originally decaying fragment
  //NEED TO CONVERT BACK FROM GEV TO MEV

  //ANK: this commented-out section may be redundant since we deal with it in the next step (1-neutron decay)
  //Set fragment info
  /*
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
  */


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
 
  //ANK: save boost vector of fragment to rest frame of A-2
  TVector3 FragBoost;
  FragBoost = pt1.BoostVector();//ANK: this is the boost vector of the A-2 for center of mass 


  /////// END OF PHASE SPACE /////////////////////////////////////////////////////////////////////////


 // Remove one neutron, then do a 3body or di-neutron decay
    // start of neutron evaporation
    p->setA(p->getA() - 1);
    p->setm(p->getA()*931.494088);
    p->setEkin(0);             
    
    TLorentzVector tl1;  // particle 1 (heavy fragment)
    tl1.SetPxPyPzE(p->getPx(), p->getPy(), p->getPz(), p->getEtotal());
    
    // create neutron
    StParticle p4;   
    p4.setA(1);
    p4.setm(pp.getA()*939.565346);//939.565346
    p4.setZ(0);
    p4.setQ(0);
    p4.setx(p->getx());     // same position
    p4.sety(p->gety()); 
    p4.setz(p->getz());     
    p4.sett(p->gett());     // same time
    p4.setEkin(0);
    
    TLorentzVector tl2;  // particle 2 (neutron)
    tl2.SetPxPyPzE(p4.getPx(), p4.getPy(), p4.getPz(), p4.getEtotal());
    
    double eN, eF;   // total neutron and fragment energy
    double eCM;   // total CM energy
    
    eCM = p4.getm() + p->getm() + exen1;  // total CM energy  ANK: exen1 is two-body decay energy that user sets
    eN = gsl_pow_2(eCM) + gsl_pow_2(p4.getm()) - gsl_pow_2(p->getm());
    eN = eN/(2.*eCM);                    // total energy neutron
    eF = gsl_pow_2(eCM) - gsl_pow_2(p4.getm()) + gsl_pow_2(p->getm());
    eF = eF/(2.*eCM);                    // total energy of fragment
    
    /// \todo check is arg of sqrt is positive
    double pN,pF; // fragment and neutron momentum
    pN = eN*eN - gsl_pow_2(p4.getm());
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
    t4Boost = tl1.BoostVector();  // boost vector for the now A-3 final fragment, takes you to CM frame of 1-neutron decay

    //ANK: need to boost particles from 1-neutron decay by the boost from the phase space decay
    tl1.Boost(FragBoost);
    tl2.Boost(FragBoost);


  // set fragment energy and direction
  p->setEtotal(tl1.E()); /// \todo implement setLVec() to set particle data from 4-vector
  p->setThetaPhi(tl1.Theta(),tl1.Phi()); 

  // set 1n energy and direction
  p4.setEtotal(tl2.E());
  p4.setThetaPhi(tl2.Theta(),tl2.Phi());
  if(! newPart) ERR("Pointer newPart not set; Cannot add new particles");
  newPart->push_back(p4); 


  }
