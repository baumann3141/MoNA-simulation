// C++ library includes
// Geant4 library includes
#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"
// My file includes
#include "InputFileObject.hh"


class G4VPrimaryGenerator;
class G4Event;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
    public:
        PrimaryGeneratorAction();
        ~PrimaryGeneratorAction();

    public:
        void SetNewInputFile(G4String ifileName);
        void GeneratePrimaries(G4Event* anEvent);
  G4int GetNumInputFileLines() {return fNumInputLines;}

    private:
        G4VPrimaryGenerator* HEPEvt;
        //G4VPrimaryGenerator* particleGun;
        G4ThreeVector fTargetPosition; 
        G4ParticleGun *particleGun;

  G4int  fNumInputLines;
       InputFileObject *fInputFileObject;
       bool kUseHEP;
  G4int fEventNum;

    public:
        // Set functions
         void setTargetPosition(G4ThreeVector position) {fTargetPosition = position;}

        // Get functions
        //G4ThreeVector getTargetPosition() {return targetPosition;}
};
//   
//

/*
#ifndef XriPrimaryGeneratorAction_h
#define XriPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

//class XriDetectorConstruction; 
class G4ParticleGun;
class G4Event;
//class XriPrimaryGeneratorMessenger; 

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    PrimaryGeneratorAction();
    ~PrimaryGeneratorAction();

  public:
    void GeneratePrimaries(G4Event *anEvent);
//    void SetRndmFlag(G4String val){rndmFlag = val;};

  private:
    G4ParticleGun* particleGun;

//    XriPrimaryGeneratorMessenger *gunMessenger;
//    G4String rndmFlag;
};

#endif

*/