// MoNA specific include files
#include "PrimaryGeneratorAction.hh"

// Geant4 included libraries
#include "G4Event.hh"
#include "G4HEPEvtInterface.hh"
#include "G4ios.hh"
#include "G4Event.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "Randomize.hh"
#include "G4UImanager.hh"

// default constructor
PrimaryGeneratorAction::PrimaryGeneratorAction()
{
  //const char* filename ="temp.indat";
    //HEPEvt = new G4HEPEvtInterface(filename);
    //targetPosition.set(0.*cm,0.*cm,0.*cm);

  kUseHEP = true;

}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  if(kUseHEP) delete HEPEvt;
  else        delete particleGun;
}

void PrimaryGeneratorAction::SetNewInputFile(G4String ifileName)
{

  if(ifileName.contains("c1md") || ifileName.contains("c2md") || ifileName.contains("c3md") ) kUseHEP = false;

  G4cout<<"Inputfile is now set to " << ifileName << "  kUseHEP is: " << kUseHEP << G4endl;

  if(!kUseHEP){
    fInputFileObject = InputFileObject::getInstance(); 
    fInputFileObject->SetFileName(ifileName);
    fNumInputLines = fInputFileObject->GetNumInputFileLines();
    particleGun = new G4ParticleGun(1);
  }
  else   HEPEvt = new G4HEPEvtInterface(ifileName);

  G4cout << "Inside PrimaryGeneratorAction - TgtPos x:"<<fTargetPosition.x()<<" y:"<<fTargetPosition.y()<<" z:"<<fTargetPosition.z()<<G4endl;

}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{ 
  if ((anEvent->GetEventID()+1) % 10000 == 0)  G4cout << ">>> Event " << anEvent->GetEventID()+1 << G4endl;


  //To output tracking info for specific event #
  //if((anEvent->GetEventID()+1) > 15){
    //G4UImanager* UI = G4UImanager::GetUIpointer();
    //UI->ApplyCommand("/tracking/verbose 2");
  //}


  if(kUseHEP) HEPEvt->GeneratePrimaryVertex(anEvent);  
  else{
    bool EndofFile = fInputFileObject->GetNextNeutron();
    if(EndofFile) G4cout << "**--** Reached End of CoMD File,Last COMD_Event#: " << fEventNum << G4endl;

    particleGun->SetParticleDefinition(G4ParticleTable::GetParticleTable()->FindParticle("neutron"));    
    particleGun->SetParticlePosition(fTargetPosition);
    //particleGun->SetParticlePosition(G4ThreeVector(0,0,0));

    fEventNum = fInputFileObject->GetEventNumber();
    G4double ekin = fInputFileObject->GetFragKE();
    G4ThreeVector pv =  fInputFileObject->GetFragPrimVector();
    particleGun->SetParticleMomentumDirection(pv);
    particleGun->SetParticleEnergy(ekin);  
    particleGun->GeneratePrimaryVertex(anEvent);
  }
  
  /*
  G4ParticleGun * particleGun = new G4ParticleGun(1);
  particleGun->SetParticleDefinition(G4ParticleTable::GetParticleTable()->FindParticle("neutron"));
  particleGun->SetParticlePosition(G4ThreeVector(0,0,0));

  G4double radius = (50*cm)*sqrt(G4UniformRand());
  G4double angle = 2*3.1415926*G4UniformRand();
  G4double energy = (40+10*G4UniformRand())*MeV;

  particleGun->SetParticleMomentumDirection(G4ThreeVector(cos(angle)*radius,
							  sin(angle)*radius,
							  844*cm));
  particleGun->SetParticleEnergy(energy);
  particleGun->GeneratePrimaryVertex(anEvent);
  */
}
