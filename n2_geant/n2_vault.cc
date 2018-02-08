// C++ library includes
#include <fstream>
#include <vector>
#include <sstream>
#include <stdlib.h>

// Geant4 library includes
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "MoNAPhysicsList.hh"
//#include "LHEP_PRECO_HP.hh"
//#include "QGSP_BIC_HP_modified.hh"
#include "G4UIterminal.hh"
#include "G4VisExecutive.hh"
#include "G4String.hh"
#include "G4ios.hh"
#include "G4ThreeVector.hh"
#include "G4UserSteppingAction.hh"

// My file includes
#include "PrimaryGeneratorAction.hh"
#include "DetectorConstruction.hh"
#include "InputHandler.hh"
#include "OutputHandler.hh"

//#include "MENATE_R_Adder.hh"

// GLOBAL TODO: error checking and debugging options are needed pretty much everywhere
// also, there are plenty of compiler warnings you should fix
// Main - this is the overall flow
int main(int argc, char *argv[])
{
    if ( argc != 7 ) // argc should be 2 for correct execution
    {
        // We print argv[0] assuming it is the program name
        //std::cout<<"usage: "<< argv[0] <<" <mode> <threshold> <vis>\n";
      G4cout << "Problem with Number of arguments..." << G4endl;
	for(int i=0; i<argc; i++){
	  std::cout<<"arg["<<i<<"]: "<< argv[i] << std::endl;
	}
        return 1;
    }
    else 
    {
        G4String mode = argv[1];                   // Run mode
        G4double threshold = atof(argv[2])*MeV;          // Hardware thresholds
        G4String vis = argv[3];

	G4String dirdata = argv[5];
	//added for running multiple g4 simulations at once
	G4String uniqueID = argv[4];	
	G4cout << "UniqueID set as: " << uniqueID << G4endl;

	G4String monaConfigFile = argv[6];
	G4cout << "MoNA Det. Config File: " << monaConfigFile << G4endl;

        InputHandler  inHandle; // This handles the checking of input files

	G4String newrootname = dirdata+"/default"+uniqueID+".root";
	if(mode == "InputFile") newrootname = dirdata+"/"+uniqueID+"_Filtered.root";
        OutputHandler* outHandle = new OutputHandler(newrootname);// This handles how things get written to file
        DetectorInterface* detectorInterface = new DetectorInterface; // Important for communicating between arrays, layers, detectors, etc
        G4String inLine;
        G4ThreeVector targetPos(0.*cm, 0.*cm, 0.*cm); // Target position vector
        // First we need to open up the detector configuration file and get the needed info
        //std::ifstream monaIn ("detector_config");
	//zwk adjusted to read input of the config file
        std::ifstream monaIn (monaConfigFile);
        // vector of arrays
        std::vector<Array*> array;
        array.clear();
        // Check to see if the file was opened correctly
        if (monaIn.is_open())
        {
            G4cout << "Reading detector configuration file\n";
            // Now read in the file
            while (! monaIn.eof())
            {
                inLine = "";
                std::getline(monaIn,inLine);
                //G4cout << inLine << G4endl;

                //Always skip commented lines
                if (!inLine.compare(0,1,"#"))
                {
		  //G4cout << "I'm skipping a comment\n";
                    continue;
                }
                // Look for new array declaration
                if (!inLine.compare(0,1,"!"))
                {
                    G4String arrayName = inLine.substr(1,inLine.size() - 1);
                    // check if we are describing the target
                    if (arrayName.substr(0,6) == "target")
                    {
                        //make the target position
                        inLine = "";
                        std::getline(monaIn,inLine);
                        // check that the target position line is formatted correctly
                        //TODO: get the format checker working
                        if (inHandle.validLayerInput(inLine,2))
                        {
                            // Converting a string to the doubles needed for the vector.
                            // Yes, c++ isn't as pretty as python here
                            G4String xS,yS,zS;
                            std::istringstream breaker(inLine);
                            breaker >> xS >> yS >> zS;
                            G4cout << "I found the target\n"
                                << "it has coords " << xS << yS << zS <<  G4endl;
                            targetPos.setX(strtod(xS,NULL)*cm); // we set the target vector
                            targetPos.setY(strtod(yS,NULL)*cm);
                            targetPos.setZ(strtod(zS,NULL)*cm);
                            continue;
                        } else
                        {
                            G4cout << "Improperly formatted target information... quitting\n";
                            return 1;
                        }
                    } else
                    {
                        // Actually make a new array
                        G4cout << "I am making a new array\n";
                        array.push_back(new Array(arrayName));
                        array.back()->setTgtPos(targetPos);
                        continue;
                    }
                }

                // Now we've eleminated all options but a new layer
                // So let's check that the line is formatted correctly and that we have
                // an array to add it to, and add the layer if that is true
                //don't want to try to process the end of file marker
                if (monaIn.eof())
                    break;
                if ((!array.empty()) && inHandle.validLayerInput(inLine,9))
                {
		  //G4cout << "Adding a new layer with info" << inLine << G4endl;
                    // add the layer to the most recently declared array
                    array.back()->addLayer(inLine,detectorInterface);
                } else
                {
                    G4cout << "I'm trying to add a layer, but either there isn't an array to add it to, or the layer information line isn't formatted properly. Exiting...\n";
                    return 1;
                }
            }
            // Close the file as we don't need it any more
            monaIn.close();
        }
        // What to do if the file wasn't opened correctly
        else
        {
            // Stop the program with an error message.
            G4cout << "Unable to open mona configuration file\n";
            return 1;
        }

        // Start passing the initial setup data to the output handler
        outHandle->setBuiltArrays(array);
        outHandle->initializeOutput();
        outHandle->setThreshold(threshold);
        //outHandle->writeConfiguration();
  	Geant4GSLrng* r = new Geant4GSLrng();
	Geant4RNGGauss* MoNAXRes = new Geant4RNGGaussGSL(r);

	//Geant4RNG* MoNAXRes(0);
	//MoNAXRes = new Geant4RNGGaussGSL(r,0,3);

        // Now pass the output handler to the detector interface so it can mediate 
        // between here and the sensitive detectors
        detectorInterface->setOutputHandler(outHandle);
        detectorInterface->getTrackerSD()->setOutputHandler(outHandle);
        //detectorInterface->getTrackerSD()->setXRNG(MoNAXRes);
        detectorInterface->setArrays(array);

        // construct the default run manager
        G4RunManager* runManager = new G4RunManager;

        // set mandatory initialization classes
        // TODO: get a real detector construction class set up
        DetectorConstruction* constructionManager = new DetectorConstruction;
        //This passes the information about how the array is set up 
        constructionManager->setDetInfo(array);
        runManager->SetUserInitialization(constructionManager);

	
	//----Physics List--------
	G4VUserPhysicsList* physics = new MoNAPhysicsList;
	runManager->SetUserInitialization(physics);
	//

	//zwk comment out below
        // TODO: create a good physics list
        //runManager->SetUserInitialization(new LHEP_PRECO_HP);
        //mam - Changing the physics list -- recommended by SMM on 5-7-10
	//G4VModularPhysicsList* physics = new QGSP_BIC_HP;
	//physics->RegisterPhysics(new MENATE_R_Adder);
        //runManager->SetUserInitialization(physics);
        //G4VUserPhysicsList* physics = new QGSP;
        //runManager->SetUserInitialization(physics);

        // set mandatory user action class
        PrimaryGeneratorAction* eventGenManager = new PrimaryGeneratorAction;
	G4String newfname = dirdata+"/temp"+uniqueID+".indat";
	if(mode == "InputFile") newfname = dirdata+"/"+uniqueID+"_Neutrons.dat";
        eventGenManager->setTargetPosition(targetPos);
	eventGenManager->SetNewInputFile(newfname);
        runManager->SetUserAction(eventGenManager);


        // initialize G4 kernel
        runManager->Initialize();

        G4UImanager* UI = G4UImanager::GetUIpointer();
        // Batch or manual mode
        if (mode == "batch") {
	  G4String command = "/control/execute ";
	  G4String newtemp = dirdata+"/temp"+uniqueID+".mac";
	  G4String fileName = newtemp;
	  UI->ApplyCommand(command+fileName);
        } 
	else if (mode == "manual") {
	  G4UIsession *XriSession = new G4UIterminal;
	  if (vis == "vison") {
	    G4VisManager *visManager = new G4VisExecutive;
	    visManager->Initialize();
	    UI->ApplyCommand("/control/execute vis.mac");
	    XriSession->SessionStart();
	    delete visManager;
	    delete XriSession;
	  }
	  else if (vis == "visoff") {
	    XriSession->SessionStart();
	    delete XriSession;
	  }
        } 
	else if(mode == "InputFile"){
	  runManager->BeamOn(eventGenManager->GetNumInputFileLines() );
	}
	else
	  {
            G4cout << "Failed to provide a valid run mode... exiting\n";
            // job termination
            delete runManager;
            // Close out the output file
            outHandle->closeout();
            return 1;
	  }

        // job termination
        delete runManager;
        // Close out the output file
        outHandle->closeout();
        
        return 0;
    }
}
