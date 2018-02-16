// C++ library includes
#include <sstream>
#include <stdio.h>
// Geant4 library includes
#include "Randomize.hh"
// My file includes
#include "OutputHandler.hh"

//ClassImp(ArrayHits) 
// Default Constructor
ArrayHits::ArrayHits()
{
    maxHits = 10;
    hits.resize(maxHits);
    /*
    hitPos.clear();
    hitTime.clear();
    hitEnergy.clear();
    hitName.clear();
    hitDetID.clear();
    */
}
// Default Destructor
ArrayHits::~ArrayHits()
{
    ;
}
// Default Constructor
OutputHandler::OutputHandler()
{
    oFileName = "default.root";
    oFileType = "root";
    outFile = new TFile(oFileName,"recreate");
    outTree = new TTree("t","Simulation Output");
    outBranches.clear();
    outArrayHits.clear();
}
OutputHandler::OutputHandler(G4String rootfilename)
{
    oFileName = rootfilename;
    oFileType = "root";
    outFile = new TFile(oFileName,"recreate");
    outTree = new TTree("t","Simulation Output");
    outBranches.clear();
    outArrayHits.clear();

    fInputFileObject = InputFileObject::getInstance(); 
}
// Default destructor
OutputHandler::~OutputHandler()
{
  G4cout << "Inside OutputHandler::~OutputHandler Deconstuctor" << G4endl;;
}
// This initializes the output (things like creating the tree)
void OutputHandler::initializeOutput()
{
    G4cout << "Initializing output\n";
    // Loop over the arrays and build a branch for hit info for each one
    for (G4int i = 0; i < builtArrays.size(); i++)
    {
        // Get the name of the array being considered
        G4String name = builtArrays[i]->getArrayName();
        // Add to the array hits vector
        outArrayHits[name] = new ArrayHits();
        G4cout << "I am adding the array " << name << G4endl;

        // Multiplicity branch
        G4String hitString = name+".multiplicity";
        G4String hitString2 = hitString+"/I";
        char hitChar[100];
        char hitChar2[100];
        std::strcpy(hitChar, hitString.c_str());
        std::strcpy(hitChar2, hitString2.c_str());
        outBranches[hitString] = outTree->Branch(hitChar,outArrayHits[name]->getMultiHitA(),hitChar2); //(TBranch*)

        hitString = name+".Z2mult";
        hitString2 = hitString+"/I";
        std::strcpy(hitChar, hitString.c_str());
        std::strcpy(hitChar2, hitString2.c_str());
        outBranches[hitString] = outTree->Branch(hitChar,outArrayHits[name]->getZ2MultA(),hitChar2); //(TBranch*)

        hitString = name+".Z1mult";
        hitString2 = hitString+"/I";
        std::strcpy(hitChar, hitString.c_str());
        std::strcpy(hitChar2, hitString2.c_str());
        outBranches[hitString] = outTree->Branch(hitChar,outArrayHits[name]->getZ1MultA(),hitChar2); //(TBranch*)

        hitString = name+".ZGt2mult";
        hitString2 = hitString+"/I";
        std::strcpy(hitChar, hitString.c_str());
        std::strcpy(hitChar2, hitString2.c_str());
        outBranches[hitString] = outTree->Branch(hitChar,outArrayHits[name]->getZGt2MultA(),hitChar2); //(TBranch*)

        hitString = name+".Gmult";
        hitString2 = hitString+"/I";
        std::strcpy(hitChar, hitString.c_str());
        std::strcpy(hitChar2, hitString2.c_str());
        outBranches[hitString] = outTree->Branch(hitChar,outArrayHits[name]->getGMultA(),hitChar2); //(TBranch*)

        hitString = name+".InputFileEventID";
        hitString2 = hitString+"/I";
        std::strcpy(hitChar, hitString.c_str());
        std::strcpy(hitChar2, hitString2.c_str());
        outBranches[hitString] = outTree->Branch(hitChar,outArrayHits[name]->getInputFileEventIDA(),hitChar2); //(TBranch*)




        // TODO: add a multilimit parameter that can be set by the array and use that here
        // instead of the hard coded 10
        for (G4int j = 0; j < 10; j++)
        {

            std::ostringstream hitnum;
            // Output an int
            hitnum << j+1;

            // Time branches
            G4String hitString = name+".Hit."+hitnum.str()+".t";
            G4String hitString2 = hitString+"/D";
            char hitChar[100];
            char hitChar2[100];
            std::strcpy(hitChar, hitString.c_str());
            std::strcpy(hitChar2, hitString2.c_str());
            outBranches[hitString] = outTree->Branch(hitChar,outArrayHits[name]->getHitVecA(j)->GetTimeA(),hitChar2); //(TBranch*)

            // Deposited energy branch
            hitString = name+".Hit."+hitnum.str()+".e";
            hitString2 = hitString+"/D";
            std::strcpy(hitChar, hitString.c_str());
            std::strcpy(hitChar2, hitString2.c_str());
            outBranches[hitString] = outTree->Branch(hitChar,outArrayHits[name]->getHitVecA(j)->GetEdepA(),hitChar2); //(TBranch*)

            // Light output branch
            hitString = name+".Hit."+hitnum.str()+".light";
            hitString2 = hitString+"/D";
            std::strcpy(hitChar, hitString.c_str());
            std::strcpy(hitChar2, hitString2.c_str());
            outBranches[hitString] = outTree->Branch(hitChar,outArrayHits[name]->getHitVecA(j)->GetLightA(),hitChar2); //(TBranch*)

            // Name of struck bar
            hitString = name+".Hit."+hitnum.str()+".name";
            hitString2 = hitString+"/C";
            std::strcpy(hitChar, hitString.c_str());
            std::strcpy(hitChar2, hitString2.c_str());
            outBranches[hitString] = outTree->Branch(hitChar,outArrayHits[name]->getHitVecA(j)->GetNameA(),hitChar2); //(TBranch*)

            // X position
            hitString = name+".Hit."+hitnum.str()+".x";
            hitString2 = hitString+"/D";
            std::strcpy(hitChar, hitString.c_str());
            std::strcpy(hitChar2, hitString2.c_str());
            outBranches[hitString] = outTree->Branch(hitChar,outArrayHits[name]->getHitVecA(j)->GetXA(),hitChar2); //(TBranch*)

            // Y position
            hitString = name+".Hit."+hitnum.str()+".y";
            hitString2 = hitString+"/D";
            std::strcpy(hitChar, hitString.c_str());
            std::strcpy(hitChar2, hitString2.c_str());
            outBranches[hitString] = outTree->Branch(hitChar,outArrayHits[name]->getHitVecA(j)->GetYA(),hitChar2); //(TBranch*)

            // Z position
            hitString = name+".Hit."+hitnum.str()+".z";
            hitString2 = hitString+"/D";
            std::strcpy(hitChar, hitString.c_str());
            std::strcpy(hitChar2, hitString2.c_str());
            outBranches[hitString] = outTree->Branch(hitChar,outArrayHits[name]->getHitVecA(j)->GetZA(),hitChar2); //(TBranch*)
        }
    }
}
// This writes the initial configuration of the setup to file
void OutputHandler::writeConfiguration()
{
    /*
    G4cout << "Writing Configuration\n";
    // TODO: this doesn't seem to be working. Troubleshoot
    getOutFile()->WriteObject(getBuiltArrays()[0],"arrays");
    */
    ;
}
// This writes the data output recorded by each event to file
void OutputHandler::writeEventData(TrackerHitsCollection* trackerCollection, G4int *pMult)
{

  

    //G4cout << "Begin data output processing\n";
    // First, we need to create a dictionary of hits to make the sorting easier
    typedef std::map<G4String,TrackerHit*> TrackerHitMap;
    TrackerHitMap hitDict;
    // Also create a dictionary of vectors (will be used later)
    typedef std::map<G4String,std::vector<G4String> > StringVectorMap; // done for convenience
    StringVectorMap sortedHitNames;

    G4int size = trackerCollection->entries();
    // This is where all the sorting and such needs to happen
    // On the outside we're looping over all the hits
    // and do the sorting of hits in the same bar
    // ... this will be used to sort by array and time
    for (G4int i = 0; i < size; i++)
    {
        TrackerHit* currentHit = (*trackerCollection)[i];
        G4String currentKey = currentHit->GetName();
	if (currentHit->GetZ()!=0)
		//already messed here
	//G4cout << currentHit->GetZ() << G4endl;
        // Check for a duplicate key
        if (hitDict[currentKey])
        {
            //G4cout << "I found a duplicate key in bar " << currentKey << "\n";
            // Here we need to iterate the deposited energy and 
            // do the weighted average of time and position by energy
            // Start by getting the dictionary settings
            G4double oldEnergy = hitDict[currentKey]->GetEdep();
            G4double oldLight = hitDict[currentKey]->GetLight();
            G4double oldLightL = hitDict[currentKey]->GetLightL();
            G4double oldLightR = hitDict[currentKey]->GetLightR();
            G4ThreeVector oldPos = hitDict[currentKey]->GetPos();
            G4double oldTime =  hitDict[currentKey]->GetTime();
            //G4cout << "Old time: " << oldTime << "\n";
            //G4cout << "Old Energy: " << oldEnergy << "\n";

            // Now get the current hit object settings
            G4double currentEnergy = currentHit->GetEdep();
            G4double currentLight = currentHit->GetLight();
            G4double currentLightL = currentHit->GetLightL();
            G4double currentLightR = currentHit->GetLightR();
            G4ThreeVector currentPos = currentHit->GetPos();
            G4double currentTime = currentHit->GetTime();
            //G4cout << "Current time: " << currentTime << "\n";
            //G4cout << "Current Energy: " << currentEnergy << "\n";

            // Now update the deposited energy of the detector
            hitDict[currentKey]->SetEdep(oldEnergy + currentEnergy);
	    //changed zwk 8/24/11
            //hitDict[currentKey]->SetLight(oldLight + currentLight);
            hitDict[currentKey]->SetLight(sqrt( (oldLightL+currentLightL) * (oldLightR+currentLightR) ));
            hitDict[currentKey]->SetLightL(oldLightL + currentLightL);
            hitDict[currentKey]->SetLightR(oldLightR + currentLightR);

            // Do the average of times and positions weighted by energy
            // TODO: check that this is robust and that units aren't affecting it
            //hitDict[currentKey]->SetTime((oldEnergy*oldTime + currentEnergy*currentTime)/(oldEnergy + currentEnergy));

	    //zwk changed 8/24/11 (time should be of the first light above threshold)
	    if(oldLightL>getThreshold() && oldLightR>getThreshold()) hitDict[currentKey]->SetTime(oldTime);
            else hitDict[currentKey]->SetTime((oldLight*oldTime + currentLight*currentTime)/(oldLight + currentLight));

            //G4cout << "Corrected time: " << hitDict[currentKey]->GetTime() << "\n";
            //G4cout << "Corrected energy: " << hitDict[currentKey]->GetEdep() << "\n\n";
            //hitDict[currentKey]->SetPos((oldEnergy*oldPos + currentEnergy*currentPos)/(oldEnergy + currentEnergy));
            hitDict[currentKey]->SetPos((oldLight*oldPos + currentLight*currentPos)/(oldLight + currentLight));
        } else
        {
            //G4cout << "I am making a new entry\n";
            // Here we simply add a new entry to the dictionary
            // because no previous hits were recorded in that detector
            hitDict[currentKey] = currentHit;
        }
    }

    // Now we need to sort by array and time. This will be done with a dictionary of vectors.
    // The key to the dictionary will be the array that the hit is in, and the vectors will
    // be the names of the struck detectors (used as keys for the dictionary we just made).
    // The idea is that we can get our array and time sorting done in one go.
    typedef std::vector<G4String> strVec;

    // On the outside we will be iterating over the whole dictionary
    for(TrackerHitMap::const_iterator it = hitDict.begin(); it != hitDict.end(); ++it)
    {
        // Record the key (iterator first and second attributes access the key and value of the
        // relevant dictionary)
        G4String struckDet = it->first;
        //G4cout << "I'm working on the hit in bar " << struckDet << G4endl;
        TrackerHit* currentDetHit = it->second;

	// TODO: putting the resolutions here in a fixed way is another dangerous temporary measure
	// that should be fixed (generalized) before other detector systems are added to the sim
	//Apply the resolution here, before the application of the threshold.
	//kfactor of 0.0806 gives 3% FWHM of peak at 40 MeVee
	
	G4double kfactor = 0.0806;
	G4double lightResL = currentDetHit->GetLightL()*G4RandGauss::shoot(1,kfactor/sqrt(currentDetHit->GetLightL()));
	currentDetHit->SetLightL(lightResL);
	G4double lightResR = currentDetHit->GetLightR()*G4RandGauss::shoot(1,kfactor/sqrt(currentDetHit->GetLightR()));
	currentDetHit->SetLightR(lightResR);

	//Applying resolution of X and T in MoNA
	G4ThreeVector Pos = currentDetHit->GetPos();
	Pos.setX(G4RandGauss::shoot(Pos.getX(),3*cm));
	currentDetHit->SetPos(Pos);
	//Normal t resolution is 0.18 ns.
	//Performing test with higher value of t resolution.
	currentDetHit->SetTime(G4RandGauss::shoot(currentDetHit->GetTime(),
						  0.18*ns));
		 

        // Apply threshold cuts here because the hits below the threshold won't make it to the output file
        // TODO: this again is a stopgap measure to get things working. Using digitization classes would be better
        if (currentDetHit->GetLightL() < getThreshold() || currentDetHit->GetLightR() < getThreshold())
        {
            //G4cout << "Hit below threshold... skipping\n";
            continue;
        }
        // Also want to know what array got hit
        // TODO: if you ever change how you do your naming system, this will have to change
        G4String struckArray = struckDet.substr(0,struckDet.find_first_of("."));
        //G4cout << "The struck array is " << struckArray << G4endl;

        // Now we check the vector dictionary for this key
        // This will check that the dictionary has the key and that the key has
        // a non-zero length vector
        if (sortedHitNames[struckArray].size())
        {
            //G4cout << "Looking at previously struck array... time sorting\n";
            // We need to look at the array's vector of struck detectors to properly time sort
            // TODO: put the typedef somewhere better because I keep using it
            strVec* hits = &sortedHitNames[struckArray];
            // this is the iterater that points to where the current hit should be
            // placed for time sorting.. defaults to the end of the vector
            strVec::iterator location = hits->begin();
            //unsigned int location = hits->size();
            // Now we are going to loop over the vector
            for(strVec::iterator it = hits->begin(); it != hits->end(); it++)
            //for(int i = hits->size()-1; i > 0; i--)
            {
                // we need to check the times of the elements against each other
                //G4cout << "I am comparing hits " << hitDict[*it]->GetName() << " and " << currentDetHit->GetName() << " or " << struckDet << G4endl;
                //G4cout << "I am comparing times " << hitDict[*it]->GetTime() << " and " << currentDetHit->GetTime() << G4endl;
                if (hitDict[*it]->GetTime() < currentDetHit->GetTime())
                {
                    // set the location to insert before
                    //G4cout << "Need to swap\n";
                    it++;
                    location = it;
                    it--;
                    //location = i;
                } else
                {
                    //G4cout << "Not swapping\n";
                }
            }
            // Now we place the detector hit in its correct time-sorted location
            // TODO:dunno if this works
            //hits->insert(location,struckDet);
            hits->insert(location,struckDet);
            //for (int i = 0; i < hits->size(); i++)
            //    G4cout << "Ordered set is " << (*hits)[i] << G4endl;
        } else
        {
            //G4cout << "Looking at unstruck array... creating entry\n";
            // We simply create the entry in the sorted hit dictionary since there doesn't need
            // to be any time sorting
            sortedHitNames[struckArray].push_back(struckDet);
        }

    }

    // So now we effectively have a map to where all of this event's hits should go. Now we need to turn
    // this into something we can pass to the actual output trees.
    // We will do this by looping over all of the arrays we want to write out, and looking for hits to
    // write to them. If there are too many hits, we truncate. If there are "too few" hits or no hits,
    // we write some sort of "invalid hit" to the relevant hits in the output tree
    for(std::map<G4String,ArrayHits*>::const_iterator it = outArrayHits.begin(); it != outArrayHits.end(); ++it)
    {
        G4String currentArray = it->first;
        ArrayHits* currentArrayHits = it->second;
        // Now we look up the hits we want to write out
//        if (sortedHitNames[currentArray].size())
        std::vector<G4String> relevantHitKeys = sortedHitNames[currentArray]; // this could end up as an empty vector
        G4int nHits = relevantHitKeys.size();
        // Pass the actual multiplicity of the event to the currentArray
        currentArrayHits->setMultiHit(nHits);
        G4int maxHits = currentArrayHits->getMaxHits();
	if(nHits > maxHits) G4cout << " nHits > maxHits, nHits = " << nHits << G4endl;
        //G4cout << "MY maxHits is "<< maxHits << G4endl;
        // Going to build a temporary 
        std::vector<TrackerHit*> tempHits;
        // We need to make sure that we don't go over the limit for the max number of output hits
        //G4cout << "nHits is " << nHits << "\n";
        for (G4int i = 0;i < maxHits; i++)
        {
            // Also have to make sure we deal with having fewer than the max number of output hits
            if (i >= nHits)
            {
                // Create "invalid hit"
                TrackerHit* t = new TrackerHit;
                tempHits.push_back(t); // TODO: be sure that the default constructor is an invalid hit
                //G4cout << "Passing invalid hit\n";
                //G4cout << "This invalid hit has time " << tempHits.back()->GetTime() << "\n";
                //G4cout << "This compares to  " << t->GetTime() << "\n";
            } else
            {
                // Actually pass a good hit
                tempHits.push_back(hitDict[relevantHitKeys[i]]);
            }
        }

        // Now that we have created the vector of hit information we want to write, we pass it off
        //currentArrayHits->setHits(tempHits);
        outArrayHits[currentArray]->setMultiHit(nHits);
        outArrayHits[currentArray]->setZ1Mult(pMult[0]);
        outArrayHits[currentArray]->setZ2Mult(pMult[1]);
        outArrayHits[currentArray]->setZGt2Mult(pMult[2]);
        outArrayHits[currentArray]->setGMult(pMult[3]);
        outArrayHits[currentArray]->SetInputFileEventID(fInputFileObject->GetEventNumber());
        for (int eye = 0; eye < 10; eye++)
        {
            outArrayHits[currentArray]->getHitVecA(eye)->SetTime(tempHits[eye]->GetTime());
            outArrayHits[currentArray]->getHitVecA(eye)->SetEdep(tempHits[eye]->GetEdep());

	    //zwk changed 8/24/11
            outArrayHits[currentArray]->getHitVecA(eye)->SetLight(sqrt(tempHits[eye]->GetLightL()*tempHits[eye]->GetLightR()) );
            //outArrayHits[currentArray]->getHitVecA(eye)->SetLight(tempHits[eye]->GetLightL()+tempHits[eye]->GetLightR());
            outArrayHits[currentArray]->getHitVecA(eye)->SetX(tempHits[eye]->GetX());
            outArrayHits[currentArray]->getHitVecA(eye)->SetY(tempHits[eye]->GetY());
            outArrayHits[currentArray]->getHitVecA(eye)->SetZ(tempHits[eye]->GetZ());
	    // have been changed
	    //G4cout <<tempHits[eye]->GetZ() << G4endl;
        }
    }
    // Finally, the fill and write commands can be run
    getOutTree()->Fill();
    //getOutTree()->OptimizeBaskets();
}
// This specifies what actions you want taken after the run
void OutputHandler::closeout()
{
    getOutTree()->Write();
    getOutFile()->Close();
}
