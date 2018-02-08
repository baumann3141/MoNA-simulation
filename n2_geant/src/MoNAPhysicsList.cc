
#include "MoNAPhysicsList.hh"
#include "G4ParticleTypes.hh"
#include "menate_R.hh"

#include "G4ParticleDefinition.hh"
#include "G4ProcessManager.hh"
#include "G4Decay.hh"

#include "G4EmProcessOptions.hh"
#include "G4ProductionCutsTable.hh"
#include "G4FastSimulationManagerProcess.hh"
#include "G4Region.hh"
#include "G4RegionStore.hh"
#include "G4ExtDEDXTable.hh"

#include "G4EmStandardPhysics_option3.hh"
#include "G4EmStandardPhysics.hh"
#include "G4EmLivermorePhysics.hh"
#include "G4EmPenelopePhysics.hh"
#include "G4EmDNAPhysics.hh"

#include "G4LEnp.hh"
#include "G4PAIModel.hh"
#include "G4HadronElasticProcess.hh"
#include "G4LElastic.hh"
#include "G4ionIonisation.hh"
#include "G4PhotoElectricEffect.hh"
#include "G4LivermorePhotoElectricModel.hh"
#include "G4LivermoreComptonModel.hh"
#include "G4GammaConversion.hh"
#include "G4LivermoreGammaConversionModel.hh"
#include "G4RayleighScattering.hh"
#include "G4LivermoreRayleighModel.hh"
#include "G4eMultipleScattering.hh"
#include "G4UrbanMscModel93.hh"
#include "G4eIonisation.hh"
#include "G4LivermoreIonisationModel.hh"
#include "G4eBremsstrahlung.hh"
#include "G4LivermoreBremsstrahlungModel.hh"
#include "G4hMultipleScattering.hh"
#include "G4ionIonisation.hh"
#include "G4IonParametrisedLossModel.hh"
#include "G4ComptonScattering.hh"
#include "G4LivermoreComptonModel.hh"
#include "G4UniversalFluctuation.hh"
#include "G4NuclearStopping.hh"
#include "G4CoulombScattering.hh"
#include "G4MuMultipleScattering.hh"
#include "G4MuIonisation.hh"
#include "G4hIonisation.hh"
#include "G4hPairProduction.hh"
#include "G4WentzelVIModel.hh"
#include "G4eplusAnnihilation.hh"
#include "G4hBremsstrahlung.hh"
#include "G4MuPairProduction.hh"
#include "G4MuBremsstrahlung.hh"

// cross sections
#include "G4BGGNucleonElasticXS.hh"
#include "G4BGGNucleonInelasticXS.hh"

#include "G4NeutronInelasticProcess.hh"
#include "G4NeutronInelasticCrossSection.hh"
#include "G4NeutronHPElasticData.hh"
#include "G4NeutronHPInelasticData.hh"
#include "G4NeutronHPFissionData.hh"
#include "G4NeutronHPCaptureData.hh"
#include "G4NeutronHPElastic.hh"
#include "G4NeutronHPInelastic.hh"

#include "G4NeutronHPJENDLHEElasticData.hh"
#include "G4NeutronHPJENDLHEInelasticData.hh"

// models
#include "G4LElastic.hh"
#include "G4CascadeInterface.hh"
#include "G4LENeutronInelastic.hh"
#include "G4LFission.hh"
#include "G4LCapture.hh"
#include "G4TheoFSGenerator.hh"
#include "G4GeneratorPrecompoundInterface.hh"
#include "G4ExcitationHandler.hh"
#include "G4PreCompoundModel.hh"
#include "G4QGSMFragmentation.hh"
#include "G4ExcitedStringDecay.hh"
#include "G4QGSParticipants.hh"
#include "G4QGSModel.hh"
#include "G4NeutronHPElastic.hh"
#include "G4NeutronHPInelastic.hh"
#include "G4NeutronHPFission.hh"
#include "G4NeutronHPCapture.hh"
#include "G4HadronFissionProcess.hh"
#include "G4HadronCaptureProcess.hh"

// Baryons
#include "G4Proton.hh"
#include "G4AntiProton.hh"
#include "G4Neutron.hh"
#include "G4AntiNeutron.hh"

// Nuclei
#include "G4Deuteron.hh"
#include "G4Triton.hh"
#include "G4Alpha.hh"
#include "G4GenericIon.hh"

#include "G4Gamma.hh"
#include "G4OpticalPhoton.hh"

// leptons
#include "G4MuonPlus.hh"
#include "G4MuonMinus.hh"
#include "G4NeutrinoMu.hh"
#include "G4AntiNeutrinoMu.hh"

#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4NeutrinoE.hh"
#include "G4AntiNeutrinoE.hh"
#include "G4KaonPlus.hh"
#include "G4KaonMinus.hh"

#include "G4Geantino.hh"
#include "G4ShortLivedConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4MesonConstructor.hh"



MoNAPhysicsList::MoNAPhysicsList() :  G4VUserPhysicsList()
{
  // default cut value  (1.0mm) 
  //defaultCutValue = 0.1*mm;
  //defaultCutValue = 0.5*mm;
}


MoNAPhysicsList::~MoNAPhysicsList()
{;}

void MoNAPhysicsList::ConstructParticle()
{
  // In this method, static member functions should be called
  // for all particles which you want to use.
  // This ensures that objects of these particle types will be
  // created in the program. 


// ions
  G4Deuteron::Deuteron();
  G4Triton::Triton();
  G4He3::He3();
  G4Alpha::Alpha();
  G4GenericIon::GenericIonDefinition();

  
  G4OpticalPhoton::OpticalPhotonDefinition();
  
  //G4Geantino::Definition();

  //  Construct  resonaces and quarks
  G4ShortLivedConstructor pShortLivedConstructor;
  pShortLivedConstructor.ConstructParticle(); 

  G4MesonConstructor pMesonConstructor;
  pMesonConstructor.ConstructParticle();

  //  Construct all baryons
  G4BaryonConstructor pBaryonConstructor;
  pBaryonConstructor.ConstructParticle();

  G4Electron::ElectronDefinition();
  G4Positron::PositronDefinition();
  G4MuonMinus::MuonMinusDefinition();
  G4MuonPlus::MuonPlusDefinition();
  G4TauMinus::TauMinusDefinition();
  G4TauPlus::TauPlusDefinition();

  G4NeutrinoE::NeutrinoEDefinition();
  G4AntiNeutrinoE::AntiNeutrinoEDefinition();
  G4NeutrinoMu::NeutrinoMuDefinition();
  G4AntiNeutrinoMu::AntiNeutrinoMuDefinition();
  G4NeutrinoTau::NeutrinoTauDefinition();
  G4AntiNeutrinoTau::AntiNeutrinoTauDefinition();

  // pseudo-particles
  G4Geantino::GeantinoDefinition();
  G4ChargedGeantino::ChargedGeantinoDefinition();
  // gamma
  G4Gamma::GammaDefinition();  
  
}

void MoNAPhysicsList::ConstructProcess()
{
  // Define transportation process
  AddTransportation();

  
  theParticleIterator->reset();

  while( (*theParticleIterator)() ){
  
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4String particleName = particle->GetParticleName();
    //G4cout << particleName << G4endl;
    //Applicability range for Livermore models
    //for higher energies, the Standard models are used   
    G4double LivermoreHighEnergyLimit = GeV;

    if (particleName == "gamma") {

      G4PhotoElectricEffect* thePhotoElectricEffect = new G4PhotoElectricEffect();
      G4LivermorePhotoElectricModel* theLivermorePhotoElectricModel = 
	new G4LivermorePhotoElectricModel();
      theLivermorePhotoElectricModel->SetHighEnergyLimit(LivermoreHighEnergyLimit);
      thePhotoElectricEffect->AddEmModel(0, theLivermorePhotoElectricModel);
      pmanager->AddDiscreteProcess(thePhotoElectricEffect);

      G4ComptonScattering* theComptonScattering = new G4ComptonScattering();
      G4LivermoreComptonModel* theLivermoreComptonModel = 
	new G4LivermoreComptonModel();
      theLivermoreComptonModel->SetHighEnergyLimit(LivermoreHighEnergyLimit);
      theComptonScattering->AddEmModel(0, theLivermoreComptonModel);
      pmanager->AddDiscreteProcess(theComptonScattering);

      G4GammaConversion* theGammaConversion = new G4GammaConversion();
      G4LivermoreGammaConversionModel* theLivermoreGammaConversionModel = 
	new G4LivermoreGammaConversionModel();
      theLivermoreGammaConversionModel->SetHighEnergyLimit(LivermoreHighEnergyLimit);
      theGammaConversion->AddEmModel(0, theLivermoreGammaConversionModel);
      pmanager->AddDiscreteProcess(theGammaConversion);

      G4RayleighScattering* theRayleigh = new G4RayleighScattering();
      G4LivermoreRayleighModel* theRayleighModel = new G4LivermoreRayleighModel();
      theRayleighModel->SetHighEnergyLimit(LivermoreHighEnergyLimit);
      theRayleigh->AddEmModel(0, theRayleighModel);
      pmanager->AddDiscreteProcess(theRayleigh);

    } else if (particleName == "e-") {

      G4eMultipleScattering* msc = new G4eMultipleScattering();
      msc->AddEmModel(0, new G4UrbanMscModel93());
      msc->SetStepLimitType(fUseDistanceToBoundary);
      pmanager->AddProcess(msc,                   -1, 1, 1);
      
      // Ionisation
      G4eIonisation* eIoni = new G4eIonisation();
      G4LivermoreIonisationModel* theIoniLivermore = new
        G4LivermoreIonisationModel();
      theIoniLivermore->SetHighEnergyLimit(LivermoreHighEnergyLimit); 
      eIoni->AddEmModel(0, theIoniLivermore, new G4UniversalFluctuation() );
      //eIoni->SetStepFunction(0.05, 100*um); //     
      pmanager->AddProcess(eIoni,                 -1, 3, 3);
      
      // Bremsstrahlung
      G4eBremsstrahlung* eBrem = new G4eBremsstrahlung();
      G4LivermoreBremsstrahlungModel* theBremLivermore = new
        G4LivermoreBremsstrahlungModel();
      theBremLivermore->SetHighEnergyLimit(LivermoreHighEnergyLimit);
      eBrem->AddEmModel(0, theBremLivermore);
      pmanager->AddProcess(eBrem, -1,-4, 4);

    } else if (particleName == "e+") {

      // Identical to G4EmStandardPhysics_option3
      
      G4eMultipleScattering* msc = new G4eMultipleScattering();
      msc->AddEmModel(0, new G4UrbanMscModel93());
      msc->SetStepLimitType(fUseDistanceToBoundary);
      pmanager->AddProcess(msc,                   -1, 1, 1);

      G4eIonisation* eIoni = new G4eIonisation();
      //eIoni->SetStepFunction(0.2, 100*um);      

      pmanager->AddProcess(eIoni,                 -1, 2, 2);
      pmanager->AddProcess(new G4eBremsstrahlung, -1,-3, 3);      
      pmanager->AddProcess(new G4eplusAnnihilation,0,-1, 4);

    } else if (particleName == "mu+" ||
               particleName == "mu-"    ) {

      // Identical to G4EmStandardPhysics_option3
      
      G4MuMultipleScattering* msc = new G4MuMultipleScattering();
      msc->AddEmModel(0, new G4WentzelVIModel());
      pmanager->AddProcess(msc,                       -1, 1, 1);

      G4MuIonisation* muIoni = new G4MuIonisation();
      //muIoni->SetStepFunction(0.2, 50*um);          

      pmanager->AddProcess(muIoni,                    -1, 2, 2);
      pmanager->AddProcess(new G4MuBremsstrahlung,    -1,-3, 3);
      pmanager->AddProcess(new G4MuPairProduction,    -1,-4, 4);
      pmanager->AddDiscreteProcess(new G4CoulombScattering());

    } else if (particleName == "GenericIon") {

      pmanager->AddProcess(new G4hMultipleScattering, -1, 1, 1);
      
      G4ionIonisation* ionIoni = new G4ionIonisation();
      ionIoni->SetEmModel(new G4IonParametrisedLossModel());
      //ionIoni->SetStepFunction(0.1, 10*um);
      pmanager->AddProcess(ionIoni,                   -1, 2, 2);
      pmanager->AddProcess(new G4NuclearStopping(),   -1, 3,-1);

    } else if (particleName == "alpha" ||
               particleName == "He3" ) {

      // Identical to G4EmStandardPhysics_option3
      
      pmanager->AddProcess(new G4hMultipleScattering, -1, 1, 1);

      G4ionIonisation* ionIoni = new G4ionIonisation();
      //ionIoni->SetStepFunction(0.1, 20*um);
      pmanager->AddProcess(ionIoni,                   -1, 2, 2);
      pmanager->AddProcess(new G4NuclearStopping(),   -1, 3,-1);

    } else if (particleName == "pi+" ||
               particleName == "pi-" ||
	       particleName == "kaon+" ||
               particleName == "kaon-" ||
               particleName == "proton" ) {

      // Identical to G4EmStandardPhysics_option3
      
      pmanager->AddProcess(new G4hMultipleScattering, -1, 1, 1);
      
      G4hIonisation* hIoni = new G4hIonisation();
      //hIoni->SetStepFunction(0.2, 50*um);

      pmanager->AddProcess(hIoni,                     -1, 2, 2);      
      pmanager->AddProcess(new G4hBremsstrahlung,     -1,-3, 3);
      pmanager->AddProcess(new G4hPairProduction,     -1,-4, 4);

    } else if (particleName == "B+" ||
	       particleName == "B-" ||
	       particleName == "D+" ||
	       particleName == "D-" ||
	       particleName == "Ds+" ||
	       particleName == "Ds-" ||
               particleName == "anti_lambda_c+" ||
               particleName == "anti_omega-" ||
               particleName == "anti_proton" ||
               particleName == "anti_sigma_c+" ||
               particleName == "anti_sigma_c++" ||
               particleName == "anti_sigma+" ||
               particleName == "anti_sigma-" ||
               particleName == "anti_xi_c+" ||
               particleName == "anti_xi-" ||
               particleName == "deuteron" ||
	       particleName == "lambda_c+" ||
               particleName == "omega-" ||
               particleName == "sigma_c+" ||
               particleName == "sigma_c++" ||
               particleName == "sigma+" ||
               particleName == "sigma-" ||
               particleName == "tau+" ||
               particleName == "tau-" ||
               particleName == "triton" ||
               particleName == "xi_c+" ||
               particleName == "xi-" ) {

      // Identical to G4EmStandardPhysics_option3
      
      pmanager->AddProcess(new G4hMultipleScattering, -1, 1, 1);
      pmanager->AddProcess(new G4hIonisation,         -1, 2, 2);

    } else if(particleName == "neutron"){
    

      G4bool UseMenate = true;
      //G4bool UseMenate = false;

      if(UseMenate){
	//------------------MENATE_R-------------------------------------------
	G4String NeutronProcessName = "menateR_neutron";
	menate_R* theMenate = new menate_R(NeutronProcessName);
	theMenate->SetMeanFreePathCalcMethod("ORIGINAL");
	
	pmanager->AddDiscreteProcess(theMenate);
	G4cout <<"||--------------------------------------------------||" << G4endl;
	G4cout <<"         MENATE_R Added to Process Manager            " << G4endl;
	G4cout <<"||--------------------------------------------------||" << G4endl;
	//-------------------------------------------------------------------------
      }
      else if(!UseMenate){      
      
	//--------------------------------------------------------------
	//----Geant4 with JENDL Libraries (should cover 20Mev -> ~3 GeV)-------------

	G4cout << "---------------Using HP Neutron Data------------------" << G4endl;
	G4cout << "---Directory: " << getenv("G4NEUTRONHPDATA") << "---" << G4endl;
	G4bool HPN;
	if (getenv("G4NEUTRONHPDATA")) {
	  G4cout << "physics list: Constructing high precision neutron models" << G4endl;
	  G4cout << "              using cross sections in "
		 << getenv("G4NEUTRONHPDATA") << G4endl;
	  HPN = true;
	} else {
	  G4cout << "physics list: Constructing non-high-precision neutron models" << G4endl;
	  HPN = false;
	}
      
	//G4ProcessManager* pmanager = G4Neutron::Neutron()->GetProcessManager();
      
	G4double HPNLimit = 19.9*MeV;
      
	// Neutron elastic process, models and cross sections
      
	G4HadronElasticProcess* elasticProcess = new G4HadronElasticProcess();
      
      
	G4LElastic* theLElasticModel = new G4LElastic();
      
	G4BGGNucleonElasticXS* elastic_XC_0 = new G4BGGNucleonElasticXS( G4Neutron::Neutron() );
	elasticProcess->AddDataSet( elastic_XC_0 );
      
	if (HPN) {
	
	  // Use JENDL HE model 20 MeV ~ 3GeV
	  G4NeutronHPJENDLHEElasticData* elastic_XC_1 = new G4NeutronHPJENDLHEElasticData();
	  elasticProcess->AddDataSet( elastic_XC_1 );
	
	  theLElasticModel->SetMinEnergy(HPNLimit);
	  G4NeutronHPElastic* theHPElasticModel = new G4NeutronHPElastic();
	  G4NeutronHPElasticData* theHPElasticData = new G4NeutronHPElasticData();
	  elasticProcess->RegisterMe(theHPElasticModel);
	  elasticProcess->AddDataSet(theHPElasticData);
	}
      
	elasticProcess->RegisterMe(theLElasticModel);
	pmanager->AddDiscreteProcess(elasticProcess);
       
      
	// Neutron inelastic process, models and cross sections
	// Use Quark-Gluon String Model between 15 GeV and 100 TeV
      
	G4NeutronInelasticProcess* ninelProc = new G4NeutronInelasticProcess();
      
	G4TheoFSGenerator* QGSPModel = new G4TheoFSGenerator();
	G4GeneratorPrecompoundInterface* theCascade = 
	  new G4GeneratorPrecompoundInterface();
	G4ExcitationHandler* exHandler = new G4ExcitationHandler();

	//--Test of fermi-breakup
	//exHandler->SetMinEForMultiFrag(3.0*MeV);
	//exHandler->SetMaxAandZForFermiBreakUp(17,9);
	//--------

	G4PreCompoundModel* preCompound = new G4PreCompoundModel(exHandler);
	theCascade->SetDeExcitation(preCompound);
	QGSPModel->SetTransport(theCascade);
	G4QGSMFragmentation* frag = new G4QGSMFragmentation();
	G4ExcitedStringDecay* stringDecay = new G4ExcitedStringDecay(frag);
	G4QGSModel<G4QGSParticipants>* stringModel = 
	  new G4QGSModel<G4QGSParticipants>();
	stringModel->SetFragmentationModel(stringDecay);
	QGSPModel->SetHighEnergyGenerator(stringModel);
	QGSPModel->SetMinEnergy(15*GeV);
	QGSPModel->SetMaxEnergy(100*TeV);
      
	// Use LEP model between 9.5 and 25 GeV
	G4LENeutronInelastic* LEPnModel = new G4LENeutronInelastic();
	LEPnModel->SetMinEnergy(9.5*GeV);
	LEPnModel->SetMaxEnergy(25.0*GeV);
      
	// Use Bertini cascade model between 0 and 9.9 GeV
	G4CascadeInterface* theBertiniModel = new G4CascadeInterface();
	theBertiniModel->SetMaxEnergy(9.9*GeV);
      
	G4BGGNucleonInelasticXS* inelastic_XC_0 = new G4BGGNucleonInelasticXS( G4Neutron::Neutron() );
	ninelProc->AddDataSet( inelastic_XC_0 );
      
	if (HPN) {
	
	  // Use JENDL HE model 20 MeV ~ 3GeV
	  G4NeutronHPJENDLHEInelasticData* inelastic_XC_1 = new G4NeutronHPJENDLHEInelasticData();
	  ninelProc->AddDataSet( inelastic_XC_1 );
	
	  // Use high precision neutron model below 20 MeV 
	  theBertiniModel->SetMinEnergy(HPNLimit);
	  G4NeutronHPInelastic* theHPInelasticModel = new G4NeutronHPInelastic();
	  G4NeutronHPInelasticData* theHPInelasticData = new G4NeutronHPInelasticData();
	  ninelProc->RegisterMe(theHPInelasticModel);
	  ninelProc->AddDataSet(theHPInelasticData);
	} 
      
	ninelProc->RegisterMe(QGSPModel);
	ninelProc->RegisterMe(LEPnModel);
	ninelProc->RegisterMe(theBertiniModel);
	pmanager->AddDiscreteProcess(ninelProc);
	

	// Neutron-induced fission process, models and cross sections
      
	G4HadronFissionProcess* neutronFission = new G4HadronFissionProcess();
	G4LFission* theLFissionModel = new G4LFission();
	theLFissionModel->SetMaxEnergy(20.*TeV);
	if (HPN) {
	  theLFissionModel->SetMinEnergy(HPNLimit);
	  G4NeutronHPFission* theHPFissionModel = new G4NeutronHPFission();
	  G4NeutronHPFissionData* theHPFissionData=new G4NeutronHPFissionData();
	  neutronFission->RegisterMe(theHPFissionModel);
	  neutronFission->AddDataSet(theHPFissionData);
	}
	neutronFission->RegisterMe(theLFissionModel);
	pmanager->AddDiscreteProcess(neutronFission);
      
	// Neutron capture process, models and cross sections
	
	G4HadronCaptureProcess* neutronCapture = new G4HadronCaptureProcess();
	G4LCapture* theLCaptureModel = new G4LCapture();
	theLCaptureModel->SetMaxEnergy(20.*TeV);
	if (HPN) {
	  theLCaptureModel->SetMinEnergy(HPNLimit);
	  G4NeutronHPCapture* theHPCaptureModel = new G4NeutronHPCapture();
	  G4NeutronHPCaptureData* theHPCaptureData = new G4NeutronHPCaptureData();
	  neutronCapture->RegisterMe(theHPCaptureModel);
	  neutronCapture->AddDataSet(theHPCaptureData);
	}
	neutronCapture->RegisterMe(theLCaptureModel);
	pmanager->AddDiscreteProcess(neutronCapture);
	
	
      
	G4cout << "--------Finished Loading G4 Neutron Process----" << G4endl;
      
	//--------------------Geant4 JENDL Neutrons-----------------------
      }
       
    }
  }  

  ConstructGeneral();
    
  G4cout << "Finished Constructing PhysicsList" << G4endl;
}


//_______________________________________________________________________
void MoNAPhysicsList::ConstructGeneral()
{
  // Add Decay Process
  G4Decay* theDecayProcess = new G4Decay();
  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    //G4cout << "HEREHREHRE "  << particle->GetParticleName() << G4endl;
    G4String particleName = particle->GetParticleName();
    if (theDecayProcess->IsApplicable(*particle) && !particleName.contains("kaon0L")) {
      //G4cout << "Inside Decay" << G4endl;
      pmanager ->AddProcess(theDecayProcess);
      // set ordering for PostStepDoIt and AtRestDoIt
      pmanager ->SetProcessOrdering(theDecayProcess, idxPostStep);
      pmanager ->SetProcessOrdering(theDecayProcess, idxAtRest);
    }
  }
  
}

void MoNAPhysicsList::SetCuts()
{

  SetCutsWithDefault();

  //stuff below for specific ORNL fission chamber

  /*	// Em options
  //      
  G4EmProcessOptions opt;
  opt.SetVerbose(0);
  
  // Multiple Coulomb scattering
  //
  //opt.SetMscStepLimitation(fUseDistanceToBoundary);
  //opt.SetMscRangeFactor(0.02);
    
  // Physics tables
  //

  opt.SetMinEnergy(100*eV);
 	//opt.SetMinEnergy(1*eV);
  opt.SetMaxEnergy(10*TeV);
  opt.SetDEDXBinning(220);
  opt.SetLambdaBinning(220);

  //opt.SetSplineFlag(true);
  opt.SetPolarAngleLimit(0.2);
    
  // Ionization
  //
  //opt.SetSubCutoff(true);  

  SetVerboseLevel(0);   
                                                          
  SetCutsWithDefault();   
  SetCutValue(0.01*um, "e-");
  
  G4double lowLimit = 0.1 * eV;
  G4double highLimit = 100. * GeV;
  G4ProductionCutsTable::GetProductionCutsTable()->SetEnergyRange(lowLimit, highLimit); */
  
  //DumpCutValuesTable();
 
}

