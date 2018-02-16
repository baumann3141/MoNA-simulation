using namespace std;
#include <algorithm>
#include <cmath>
#include <iostream>

#include "TH1.h"
#include "TH2.h"
#include "TTree.h"
#include "TVector3.h"

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
///// Class containing information on a single neutron hit.
///// Contains values of X, Y, Z, and TOF.
///// Calculates the velocity from fragment to target.
///// Calculates the distance between two neutron hits.
///// Calculates the velocity necessary to travel between two hit locations.
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////

class NHit
{
public:
  //The mass of the neutron in MeV/c2.
  const static double Namu = 939.57;

  double X,Y,Z,TOF;
  double V;
  
  bool IsValid();
  void MeterToCentimeter();
  double Theta();
  void CalculateV();
  bool operator < (NHit);
  double DistanceToHit (NHit);
  double DistanceFromTarget();
  double BetaToHit (NHit);
  double BetaFromTarget();
  double Gamma();
  double Energy();
  double Momentum();
  double ScatteringAngle(NHit);
};

bool NHit::IsValid(){
  return (X!=-1 && Y!=-1 && Z!=-1 && TOF>0 && V>0);
}

void NHit::MeterToCentimeter(){
  X *= 100;
  Y *= 100;
  Z *= 100;
}

double NHit::Theta(){
  double r = sqrt(X*X + Y*Y);
  double theta = atan(r/Z);
  return theta;
}

//Calculates the velocity from the target to the neutron hit.
void NHit::CalculateV(){
  if (TOF != 0){
    V = DistanceFromTarget()/TOF;
  } else {
    V = -1;
  }
}

//This operator is necessary for the sorting.
//If velocity-sorting is desired, have the first line uncommented.
//If time-sorting is desired, have the second line uncommented.
bool NHit::operator< (NHit other){return (other.V < V);}
//bool NHit::operator< (NHit other){return (T < other.T);}

//Determines the distance between this hit and another hit.
double NHit::DistanceToHit (NHit other){
  double dX = X - other.X;
  double dY = Y - other.Y;
  double dZ = Z - other.Z;
  return std::sqrt(dX*dX + dY*dY + dZ*dZ);
}

//Returns the distance on the neutron hit to the target.
double NHit::DistanceFromTarget(){return std::sqrt(X*X + Y*Y + Z*Z);}

//Determines the speed (in c) necessary to travel between two hits in time.
double NHit::BetaToHit (NHit other){
  double time = TOF-other.TOF;
  //Warning.  Do not replace the next line with the absolute value function.
  //It will cast as an int, rounding down, then cast back to a double.
  if (time < 0){time *= -1;}
  double vel = DistanceToHit(other)/time;
  return vel/30;
}

//Returns V in terms of the speed of light.
double NHit::BetaFromTarget(){return V/30;}

//Returns the gamma factor of the neutron.
double NHit::Gamma(){return 1/sqrt(1-std::pow(BetaFromTarget(),2));}

//Returns the energy of the neutron.
double NHit::Energy(){return Namu*Gamma();}

//Returns the magnitude of the momentum of the neutron.
double NHit::Momentum(){return Gamma() * Namu * BetaFromTarget();}

//Returns the scattering angle, from this hit to the other hit.
//Return value is in radians.
double NHit::ScatteringAngle (NHit other){
  double dX = other.X-X;
  double dY = other.Y-Y;
  double dZ = other.Z-Z;
  double mag_self = std::sqrt(X*X + Y*Y + Z*Z);
  double mag_delta = std::sqrt(dX*dX + dY*dY + dZ*dZ);

  double cos_angle = (X*dX + Y*dY + Z*dZ)/(mag_self*mag_delta);
  return std::acos(cos_angle);
}

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
///// Class containing fragment parameters.
///// Contains ata, bta, and target.e
///// If passed a single neutron hit, will calculate edecay
///// If passed two neutron hits, will calculate edecay_3body.
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////

class Frag{
public:
  Frag();
  
  //Mass of the fragment in MeV/c2.
  double Fragamu;
  double FragA;
  double FragZ;

  double ata,bta,target_e;
  double Energy();
  double Momentum();
  
  double TotalEnergy(NHit);
  double TotalEnergy(NHit,NHit);
  double DecayEnergy(NHit);
  double DecayEnergy(NHit,NHit);
  double CosOpeningAngle(NHit);
  double CosOpeningAngle(NHit,NHit);
};

Frag::Frag(){
  Fragamu = 10331.7;
  FragA = 11;
  FragZ = 3;
}

//Returns the total energy of the fragment.
double Frag::Energy(){return target_e*FragA + Fragamu;}

//Returns the magnitude of the momentum of the fragment.
double Frag::Momentum(){return std::sqrt(std::pow(Energy(),2) - std::pow(Fragamu,2));}

//Returns the cosine of the opening angle.
double Frag::CosOpeningAngle (NHit Neutron){
  double xpos = std::tan(ata/1000);
  double ypos = std::tan(bta/1000);
  double FragDist = std::sqrt(xpos*xpos + ypos*ypos + 1*1);
  
  double DotProduct = Neutron.X*xpos + Neutron.Y*ypos + Neutron.Z*1;

  return DotProduct/(FragDist * Neutron.DistanceFromTarget());
}

//Returns the opening angle between two neutrons.
double Frag::CosOpeningAngle (NHit Neutron, NHit OtherNeutron){
  double DotProduct = Neutron.X*OtherNeutron.X +
    Neutron.Y*OtherNeutron.Y +
    Neutron.Z*OtherNeutron.Z;
  return DotProduct/(Neutron.DistanceFromTarget()*OtherNeutron.DistanceFromTarget());
}

//Calculates the total energy of the fragment-neutron system.
//Uses invariant relativistic mass.
double Frag::TotalEnergy (NHit Neutron){
  double a = Neutron.Namu*Neutron.Namu + Fragamu*Fragamu;
  double b = Energy()*Neutron.Energy() - Momentum()*Neutron.Momentum()*CosOpeningAngle(Neutron);
  return std::sqrt(a + 2*b);
}

//Calculates the decay energy of the fragment with the neutron provided.
//Subtracts the rest energy of system from the total energy.
double Frag::DecayEnergy (NHit Neutron){
  return TotalEnergy(Neutron) - Neutron.Namu - Fragamu;
}

//Calculates the total energy of the 3-body system of fragment and 2 neutrons.
//Uses invariant relativistic mass.
double Frag::TotalEnergy (NHit Neutron, NHit OtherNeutron){
  double a = Fragamu*Fragamu + 2*Neutron.Namu*Neutron.Namu;
  double b = Energy()*Neutron.Energy() +
    Energy()*OtherNeutron.Energy() +
    Neutron.Energy()*OtherNeutron.Energy();
  double c = Momentum()*Neutron.Momentum()*CosOpeningAngle(Neutron) +
    Momentum()*OtherNeutron.Momentum()*CosOpeningAngle(OtherNeutron) +
    Neutron.Momentum()*OtherNeutron.Momentum()*CosOpeningAngle(Neutron,OtherNeutron);
  return std::sqrt(a+2*b-2*c);
}

//Calculates the 3-body decay energy by subtracting rest energy from total energy.
double Frag::DecayEnergy (NHit Neutron, NHit OtherNeutron){
  return TotalEnergy(Neutron,OtherNeutron) - 2*Neutron.Namu - Fragamu;
}


//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
///// Main function to call to run the neutron discriminator.
///// Will read through tree passed and find valid 2-neutron hits.
///// Will make spectra of different 2-neutron parameters.
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////

//Parameter descriptions.

TTree * NeutronDiscriminator(TTree * input, double RadiusCut, double VelocityCut, const char* outfilename = "temp.root", string frag = "11Li", double target_eloss = -1)
{
  //Creates the tags to read in from the data file.
  //Pointers to the values stored in the "NHit" variables are 
  NHit Hits[6];
  Frag Fragment;
  int multiplicity;
  int n1multiplicity,n2multiplicity;

  double multiplicity_d;
  cal::mona* mcal = new cal::mona();
  cal::sweeper* scal = new cal::sweeper();

  if (target_eloss == -1){
    var::sweeper svar;
    target_eloss = svar.target_eloss;
  }

  bool FromSim = false;
  int InputType = -1;

  //Changes the fragment parameters based on those given in the parameters.
  if (frag=="11Li"){
    Fragment.Fragamu = 10331.7;
    Fragment.FragA = 11;
    Fragment.FragZ = 3;
  } else if (frag=="22O"){
    Fragment.Fragamu = 20498.10;
    Fragment.FragA = 22;
    Fragment.FragZ = 8;
  } else if (frag=="23O"){
    Fragment.Fragamu = 21434.90;
    Fragment.FragA = 23;
    Fragment.FragZ = 8;
  } else if (frag=="24O"){
    Fragment.Fragamu = 22370.68;
    Fragment.FragA = 24;
    Fragment.FragZ = 8;
  } else if (frag=="13Be"){
    Fragment.Fragamu = 12140.91;
    Fragment.FragA = 13;
    Fragment.FragZ = 4;
  } else {
    cout << "Unknown fragment given: " << frag << endl;
    cout << "Current options are '11Li', '13Be', '22O', '23O', and '24O'" << endl;
    return new TTree();
  }

  if (input->FindBranch("X_hit_1")!=0){
    InputType = 1;
    
    input->SetBranchAddress("X_hit_1",&(Hits[0].X));
    input->SetBranchAddress("X_hit_2",&(Hits[1].X));
    input->SetBranchAddress("X_hit_3",&(Hits[2].X));
    input->SetBranchAddress("X_hit_4",&(Hits[3].X));
    input->SetBranchAddress("X_hit_5",&(Hits[4].X));
    input->SetBranchAddress("X_hit_6",&(Hits[5].X));
    input->SetBranchAddress("Y_hit_1",&(Hits[0].Y));
    input->SetBranchAddress("Y_hit_2",&(Hits[1].Y));
    input->SetBranchAddress("Y_hit_3",&(Hits[2].Y));
    input->SetBranchAddress("Y_hit_4",&(Hits[3].Y));
    input->SetBranchAddress("Y_hit_5",&(Hits[4].Y));
    input->SetBranchAddress("Y_hit_6",&(Hits[5].Y));
    input->SetBranchAddress("Z_hit_1",&(Hits[0].Z));
    input->SetBranchAddress("Z_hit_2",&(Hits[1].Z));
    input->SetBranchAddress("Z_hit_3",&(Hits[2].Z));
    input->SetBranchAddress("Z_hit_4",&(Hits[3].Z));
    input->SetBranchAddress("Z_hit_5",&(Hits[4].Z));
    input->SetBranchAddress("Z_hit_6",&(Hits[5].Z));
    input->SetBranchAddress("TOF_hit_1",&(Hits[0].TOF));
    input->SetBranchAddress("TOF_hit_2",&(Hits[1].TOF));
    input->SetBranchAddress("TOF_hit_3",&(Hits[2].TOF));
    input->SetBranchAddress("TOF_hit_4",&(Hits[3].TOF));
    input->SetBranchAddress("TOF_hit_5",&(Hits[4].TOF));
    input->SetBranchAddress("TOF_hit_6",&(Hits[5].TOF));    
    input->SetBranchAddress("sweeper.fp.track.ata",&(Fragment.ata));
    input->SetBranchAddress("sweeper.fp.track.bta",&(Fragment.bta));
    input->SetBranchAddress("target.e",&(Fragment.target_e));
    input->SetBranchAddress("Multi_hit",&multiplicity_d);
    FromSim = false;
  } else if (input->FindBranch("mult")!=0){
    InputType = 2;
    input->SetBranchAddress("mona",&mcal);
    input->SetBranchAddress("sweeper",&scal);
    FromSim = false;
  } else if (input->FindBranch("vG1NPos")!=0) {
    InputType = 3;
    //I don't know why I need to run "SetMakeClass", but I cannot grab values otherwise.  Hereby declared magic and not to be touched.
    input->SetMakeClass(1);
    
    input->SetBranchAddress("vG1NPos.fX",&(Hits[0].X));
    input->SetBranchAddress("vG2NPos.fX",&(Hits[1].X));
    input->SetBranchAddress("vG3NPos.fX",&(Hits[2].X));
    input->SetBranchAddress("vG4NPos.fX",&(Hits[3].X));
    input->SetBranchAddress("vG5NPos.fX",&(Hits[4].X));
    input->SetBranchAddress("vG6NPos.fX",&(Hits[5].X));
    input->SetBranchAddress("vG1NPos.fY",&(Hits[0].Y));
    input->SetBranchAddress("vG2NPos.fY",&(Hits[1].Y));
    input->SetBranchAddress("vG3NPos.fY",&(Hits[2].Y));
    input->SetBranchAddress("vG4NPos.fY",&(Hits[3].Y));
    input->SetBranchAddress("vG5NPos.fY",&(Hits[4].Y));
    input->SetBranchAddress("vG6NPos.fY",&(Hits[5].Y));
    input->SetBranchAddress("vG1NPos.fZ",&(Hits[0].Z));
    input->SetBranchAddress("vG2NPos.fZ",&(Hits[1].Z));
    input->SetBranchAddress("vG3NPos.fZ",&(Hits[2].Z));
    input->SetBranchAddress("vG4NPos.fZ",&(Hits[3].Z));
    input->SetBranchAddress("vG5NPos.fZ",&(Hits[4].Z));
    input->SetBranchAddress("vG6NPos.fZ",&(Hits[5].Z));
    input->SetBranchAddress("vG1NTime",&(Hits[0].TOF));
    input->SetBranchAddress("vG2NTime",&(Hits[1].TOF));
    input->SetBranchAddress("vG3NTime",&(Hits[2].TOF));
    input->SetBranchAddress("vG4NTime",&(Hits[3].TOF));
    input->SetBranchAddress("vG5NTime",&(Hits[4].TOF));
    input->SetBranchAddress("vG6NTime",&(Hits[5].TOF));
    input->SetBranchAddress("fpATA",&(Fragment.ata));
    input->SetBranchAddress("fpBTA",&(Fragment.bta));
    input->SetBranchAddress("fragKinE",&(Fragment.target_e));
    input->SetBranchAddress("gmultiplicity",&multiplicity);

    //These are the parameters giving the multiplicity of each neutron.
    //Not present in the data since we have no way of knowing.
    input->SetBranchAddress("n1gmultiplicity",&n1multiplicity);
    input->SetBranchAddress("n2gmultiplicity",&n2multiplicity);

    //The simulation has parameters in meters instead of centimeters.
    //Flag here to change units each time that an event is loaded.
    //Also, must extract values from TVector3 into double in NHit
    FromSim = true;
  } else {
    cout << "Improper input tree." << endl;
    cout << "Stopping now." << endl;
    cout << "Currently, can handle input from 05034 converted filter files and from Geant4 simulation" << endl;
    return new TTree();
  }
  
  //Making output tree.
  TFile * outfile = new TFile(outfilename,"recreate");
  TTree * output = new TTree("et","Edecay calculations");
  double e_2body_t1, e_2body_t2, e_2body_low, e_2body_high;
  double e_3body;
  double NVelocity, NDistance, NDeltaT, NScatteringAngle;
  int TrueTwoNeutron;
  output->Branch("e_2body_t1",&e_2body_t1,"2body_t1/D");
  output->Branch("e_2body_t2",&e_2body_t2,"2body_t2/D");
  output->Branch("e_2body_low",&e_2body_low,"2body_high/D");
  output->Branch("e_2body_high",&e_2body_high,"2body_low/D");
  output->Branch("e_3body",&e_3body,"3body/D");
  output->Branch("NVelocity",&NVelocity,"NVelocity/D");
  output->Branch("NDistance",&NDistance,"NDistance/D");
  output->Branch("NDeltaT",&NDeltaT,"NDeltaT/D");
  output->Branch("NScatteringAngle",&NScatteringAngle,"NScatteringAngle/D");
  output->Branch("TrueTwoNeutron",&TrueTwoNeutron,"TrueTwoNeutron/I");

  output->AddFriend(input);

  int Entries = input->GetEntries();
  int ValidEvents = 0;
  for (int EventCounter=0;EventCounter<Entries;EventCounter++){
  //for (int EventCounter=0;EventCounter<2001;EventCounter++){
    if (EventCounter % 1000 == 0){
      cout << "Analyzing event number " << EventCounter << " of " << Entries << endl;
    }
    
    //Places all input values into the holder variables.
    input->GetEntry(EventCounter);
    
    if (InputType==1){
      //The data root files store multiplicity as a double, but I want an int.
      multiplicity = (int) multiplicity_d;
    } else if (InputType==2){
      //I can't store the values directly to Hits[] and Fragment.
      //Must read from the mcal, scal to write to Hits[] and Fragment.
      Fragment.ata = scal->itrack.tx;
      Fragment.bta = scal->itrack.ty;
      //scal->e is at the end of the target.  Must add target_eloss for energy at center of target.
      Fragment.target_e = scal->e + target_eloss;
      multiplicity = mcal->mult;
      for (int i=0;i<6;i++){
	Hits[i].X = mcal->hit.x[i];
	Hits[i].Y = mcal->hit.y[i];
	Hits[i].Z = mcal->hit.z[i];
	Hits[i].TOF = mcal->hit.t[i];
      }
    } else if (InputType==3){
      //The simulation gives distances in meters, but I want cm.
      for (int i=0;i<6;i++){
	Hits[i].MeterToCentimeter();
      }
    }

    //Determines the velocity of the neutron found in each hit, then sorts by velocity.
    for (int i=0;i<6;i++){Hits[i].CalculateV();}
    std::sort(Hits,Hits+6);

    //Searches for a second neutron hit, assuming that the first velocity-sorted hit is valid.
    //"IsValid" makes sure that information was found from the input tree.
    //The distance between the hits must be greater than Radius.
    //The speed necessary to travel between the two hits must be greater than BetaMin.
    bool FoundSecond = false;
    int SecondNeutron = 1;
    for (int i=1;i<6;i++){
      if (Hits[i].IsValid() &&
	  Hits[i].DistanceToHit(Hits[0])>RadiusCut &&
	  Hits[i].BetaToHit(Hits[0])>VelocityCut){
	FoundSecond = true;
	SecondNeutron = i;
	break;
      }
    }

    //Sets the value for TrueTwoNeutron
    bool KnownTwoNeutron =
      (FromSim && (n1multiplicity==1) && (n2multiplicity==1));
    bool KnownOneNeutron =
      (FromSim && (multiplicity==2) &&
       ( (n1multiplicity==0) || (n2multiplicity==0) ) );
    if (KnownOneNeutron){
      TrueTwoNeutron = -1;
    } else if (KnownTwoNeutron){
      TrueTwoNeutron = 1;
    } else {
      TrueTwoNeutron = 0;
    }
    
    if (Hits[0].IsValid()){
      e_2body_t1 = Fragment.DecayEnergy(Hits[0]);
    } else {
      e_2body_t1 = -1;
    }
    //If there is a second neutron, fill the histogram about the neutron.
    if (FoundSecond) {
      ValidEvents++;
      e_2body_t2 = Fragment.DecayEnergy(Hits[SecondNeutron]);
      e_2body_low = std::min(e_2body_t1,e_2body_t2);
      e_2body_high = std::max(e_2body_t1,e_2body_t2);
      e_3body = Fragment.DecayEnergy(Hits[0],Hits[SecondNeutron]);
    } else {
      e_2body_t2 = -1;
      e_2body_low = -1;
      e_2body_high = -1;
      e_3body = -1;
    }
    if (Hits[0].IsValid() &&
	Hits[SecondNeutron].IsValid()){
      NVelocity = Hits[SecondNeutron].BetaToHit(Hits[0]);
      NDistance = Hits[SecondNeutron].DistanceToHit(Hits[0]);
      NDeltaT = Hits[SecondNeutron].TOF - Hits[0].TOF;
      if (NDeltaT > 0){
	NScatteringAngle = Hits[0].ScatteringAngle(Hits[SecondNeutron]);
      } else {
	NDeltaT *= -1;
	NScatteringAngle = Hits[SecondNeutron].ScatteringAngle(Hits[0]);
      }
    } else {
      NVelocity = -1;
      NDistance = -1;
      NDeltaT = -1;
      NScatteringAngle = -1;
    }
    output->Fill();
  }
  cout << "Of " << Entries << " total events, " << ValidEvents << " passed the velocity and distance cuts and had decay energies calculated." << endl;
  //cout << "Use 'et' instead of 't' to refer to the data tree, now." << endl;
  //  cout << "Note that if the input was from a TChain, you must perform the following steps" << endl;
  cout << "Now, to use the created file, follow these steps." << endl;
  cout << "1.  Restart root." << endl;
  cout << "2.  Reload the file." << endl;
  cout << "3.  Load the newly created root file." << endl;
  cout << "4.  Add the TTree et as a friend to your file." << endl;
  cout << "       e.g. t->AddFriend(\"et\");" << endl;
  cout.flush();
  output->Write();

  outfile->Close();
  return output;
}
