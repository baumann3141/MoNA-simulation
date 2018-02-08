// C++ library includes
// Geant4 library includes
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4Track.hh"
// My file includes
#include "TrackerSD.hh"

// Constructor
TrackerSD::TrackerSD(G4String name) : G4VSensitiveDetector(name)
{
    G4String HCname;
    collectionName.insert(HCname="trackerCollection");
}

// Destructor
TrackerSD::~TrackerSD()
{;}

// Happens at the beginning of each event
void TrackerSD::Initialize(G4HCofThisEvent* HCE)
{
  fZ2mult=0;
  fZ1mult=0;
  fZGtmult=0;
  fGmult=0;
  for(int i=0; i<1000; i++) fIDArray[i]=0;
  fIDcount=0;

    // We make the collection object to collect all the hits that will happen this event
    trackerCollection = new TrackerHitsCollection(SensitiveDetectorName,collectionName[0]); 
    static G4int HCID = -1;
    if (HCID<0)
    {
        HCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
    }
    HCE->AddHitsCollection(HCID, trackerCollection); 
}
// a "readout geometry", which we currently do not have
G4bool TrackerSD::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{

    G4double edep = aStep->GetTotalEnergyDeposit();

    if(edep==0.) return false;

    // This is the primary purpose of this function - to make a hit class
    TrackerHit* newHit = new TrackerHit();
    newHit->SetTrackID(aStep->GetTrack()->GetTrackID());

    
    newHit->SetEdep(edep);
    newHit->SetChamberNb(aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber());
    newHit->SetPos(aStep->GetPostStepPoint()->GetPosition());
    newHit->SetName(aStep->GetPreStepPoint()-> GetPhysicalVolume()->GetName());
    newHit->SetTime(aStep->GetPreStepPoint()->GetGlobalTime());

    // Now for the fun part... doing some functions specific to the detector
    G4String name = aStep->GetPreStepPoint()-> GetPhysicalVolume()->GetName();
    G4int first = name.find_first_of(".");
    G4int last = name.find_last_of(".");
    G4int size = first;
    G4String arrayName = name.substr(0,size);
    size = last - first - 1;
    G4String layerName = name.substr(first+1,size);
    size = name.size() - last - 1;
    G4String barName = name.substr(last + 1,size);
    //G4cout << "The whole name is" << name << G4endl;
    //G4cout << "My struck detector info is Array: " << arrayName << " Layer: " << layerName << " Bar: " << barName << G4endl;
    
    // Now search for the right detector
    G4double LIGHT=0;
    std::vector<Array*> arrays = getDetInterface()->getArrays();
    size = arrays.size();
    for (G4int i = 0; i < size; i++)
    {
        if (arrays[i]->getArrayName() == arrayName)
        {
            // TODO: It seems to me that some of this stuff might be better to move into the specific arrays
            G4int layernum = getDetInterface()->getNumber(layerName);
            G4int detnum = std::atoi(barName);
            //G4cout << "I found detector " << layernum << " " << detnum << " " << name << G4endl;
            std::vector<G4double> lightInfo = arrays[i]->getLayers()[layernum]->getDetectors()[detnum]->processLight(aStep);

	    // TODO: this is more or less hardwired for mona bars. generalize it
            if (lightInfo.size() == 2)
            {
                newHit->SetLightL(lightInfo[0]);
                newHit->SetLightR(lightInfo[1]);
                
		//changed zwk 8/24/11  ... 
		//newHit->SetLight(lightInfo[0]+lightInfo[1]);
                newHit->SetLight(sqrt(lightInfo[0]*lightInfo[1]));

		LIGHT = sqrt(lightInfo[0]*lightInfo[1]);
            } else {
                G4cout << "Something went wrong with the light output analysis!\n";
                break;
            }

            std::vector<G4double> AfterResolutions = arrays[i]->getLayers()[layernum]->getDetectors()[detnum]->addResolutions(aStep);
            if (AfterResolutions.size() == 2)
            {
		G4ThreeVector newhitloc = aStep->GetPostStepPoint()->GetPosition();
		//G4cout << newhitloc.getZ() << G4endl;
		//X and T resolutions should be applied after TrackerHits are summed.
		//Moving these to OutputHandler.cc
		//newhitloc.setX(AfterResolutions[0]);
                //newhitloc.setY(AfterResolutions[1]);
                //newhitloc.setZ(AfterResolutions[2]);
		//newHit->SetTime(AfterResolutions[3]);
                newhitloc.setY(AfterResolutions[0]);
                newhitloc.setZ(AfterResolutions[1]);
		newHit->SetPos(newhitloc);
                break;
            } else {
                G4cout << "Something went wrong with adding resolutions!\n";
                break;
            }
        }
    }


    // Add this hit to the collection of hits for this event
    // TODO: There must be a better way of handling the time cuts in MoNA
    if (aStep->GetPreStepPoint()->GetGlobalTime()/ns < 300)
    {
        trackerCollection->insert(newHit);
    }

    //newHit->Print();
    //newHit->Draw();


    /*
    //ZWK trying to count the created particles and types
    G4int trackID = aStep->GetTrack()->GetTrackID();
    G4bool newtrack = CheckTrackID(trackID);
    if(newtrack){
      fIDArray[fIDcount] = trackID;
      G4int zz = aStep->GetTrack()->GetDefinition()->GetAtomicNumber();
      G4int aa = aStep->GetTrack()->GetDefinition()->GetAtomicMass();
      G4String pName = aStep->GetTrack()->GetDefinition()->GetParticleName();
      G4cout << "TrackID: "<<fIDArray[fIDcount] << "  ParentID: " << aStep->GetTrack()->GetParentID() << "  " <<   pName << ",  Z="<<zz << "  A=" << aa << G4endl;
      //G4cout << "     Step#: " << aStep->GetTrack()->GetCurrentStepNumber() << G4endl;

      if(zz>1 && zz!=6) fZ2mult++;
      else if(zz==1 ) fZ1mult++;
      else if(zz==6 )  fZGtmult++;

      //if(pName.contains("neutron")) fZ1mult++;
      if(pName.contains("gamma")) fGmult++;
      //G4cout << fZ1mult << G4endl;
      fIDcount++;
      } */
        


    return true;
}


// This controls what happens at the end of each event
// You will note that it loops over all the hits in the sensitive detector
// TODO: make this output some information to ROOT
void TrackerSD::EndOfEvent(G4HCofThisEvent*)
{
    if (verboseLevel>=1)
    { 
        G4int NbHits = trackerCollection->entries();
        G4cout << "\n-------->Hits Collection: in this event they are " << NbHits 
            << " hits in the tracker chambers: " << G4endl;
        // Here is where you might be considering the wondrous output
        for (G4int i=0;i<NbHits;i++) (*trackerCollection)[i]->Print();
    }
    // This will actually cause the writing to file
    // We pass all the information along instead of doing much here because I wish to
    // keep this part of the code separated from how we want to arrange the data.
    // If we want to start doing more complex detector simulation, that may change
    G4int pMult[4] = {fZ1mult, fZ2mult, fZGtmult, fGmult};
    getOutputHandler()->writeEventData(getTrackerCollection(),pMult);
}


G4bool TrackerSD::CheckTrackID(G4int trackID){

  if(fIDcount==0) return true;

  G4bool test = true;
  for(int i=0; i<fIDcount; i++){

    if(fIDArray[i] == trackID) test = false;

  }

  return test;
}
