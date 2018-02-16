// MoNAPhysicsList
//  Construct/define particles and physics processes
//
//  Particle defined in ExampleN01
//    geantino
//
//  Process defined in ExampleN01
//    transportation
//

#ifndef MoNAPhysicsList_h
#define MoNAPhysicsList_h 1

#include "G4VUserPhysicsList.hh"
#include "globals.hh"

class MoNAPhysicsList: public G4VUserPhysicsList
{
  public:
    MoNAPhysicsList();
    ~MoNAPhysicsList();

  protected:
    // Construct particle and physics process
    void ConstructParticle();
    void ConstructProcess();
    void SetCuts();
    void ConstructGeneral();

};

#endif







