#include "MENATE_R_Adder.hh"
#include "menate_R.hh"

#include "globals.hh"
#include "G4ios.hh"
#include <iomanip>   
#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleTypes.hh"
#include <stdio.h>
#include "G4ios.hh"
#include "G4ProcessManager.hh"

MENATE_R_Adder::MENATE_R_Adder(const G4String& name)
                    :  G4VPhysicsConstructor(name) 
{}

void MENATE_R_Adder::CreateModels()
{
  
}

MENATE_R_Adder::~MENATE_R_Adder() 
{
}

void MENATE_R_Adder::ConstructParticle()
{;}


void MENATE_R_Adder::ConstructProcess()
{
  G4ProcessManager * theProcMan = G4Neutron::Neutron()->GetProcessManager();
  menate_R* theMENATE = new menate_R("menate_neutron");
  theProcMan->AddDiscreteProcess(theMENATE);
}
