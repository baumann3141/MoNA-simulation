#include "DetectorInterface.hh"

DetectorInterface::DetectorInterface()
{
    madeElements.clear();
    madeMaterials.clear();
    madeLogVols.clear();
    nameToNum = new std::map<G4String, G4int>;
    numToName = new std::map<G4int, G4String>;
    // This stuff is for the sensitive detectors and hit classes
    SDman = G4SDManager::GetSDMpointer();
    aTrackerSD = new TrackerSD( "TrackerSD" );
    // Tell the sensistive detectors about the detector interface
    aTrackerSD->setDetInterface(this);
    SDman->AddNewDetector( aTrackerSD );
    // This makes necessary associations for layer names
    // NOTE: this puts a limit on the number of layers you can have at 0-25
    // If you want to have more than 26 layers, you must make a new naming scheme
    // TODO: this is a massively inelegant way of doing things... clean it up
    // TODO: Given that you specify names in the input file... this might be unnecessary
    //       cetainly need to think about if this really is needed
    nameToNum->insert(std::make_pair("A",0));
    nameToNum->insert(std::make_pair("B",1));
    nameToNum->insert(std::make_pair("C",2));
    nameToNum->insert(std::make_pair("D",3));
    nameToNum->insert(std::make_pair("E",4));
    nameToNum->insert(std::make_pair("F",5));
    nameToNum->insert(std::make_pair("G",6));
    nameToNum->insert(std::make_pair("H",7));
    nameToNum->insert(std::make_pair("I",8));
    nameToNum->insert(std::make_pair("J",9));
    nameToNum->insert(std::make_pair("K",10));
    nameToNum->insert(std::make_pair("L",11));
    nameToNum->insert(std::make_pair("M",12));
    nameToNum->insert(std::make_pair("N",13));
    nameToNum->insert(std::make_pair("O",14));
    nameToNum->insert(std::make_pair("P",15));
    nameToNum->insert(std::make_pair("Q",16));
    nameToNum->insert(std::make_pair("R",17));
    nameToNum->insert(std::make_pair("S",18));
    nameToNum->insert(std::make_pair("T",19));
    nameToNum->insert(std::make_pair("U",20));
    nameToNum->insert(std::make_pair("V",21));
    nameToNum->insert(std::make_pair("W",22));
    nameToNum->insert(std::make_pair("X",23));
    nameToNum->insert(std::make_pair("Y",24));
    nameToNum->insert(std::make_pair("Z",25));

    // Now go the other way
    numToName->insert(std::make_pair(0,"A"));
    numToName->insert(std::make_pair(1,"B"));
    numToName->insert(std::make_pair(2,"C"));
    numToName->insert(std::make_pair(3,"D"));
    numToName->insert(std::make_pair(4,"E"));
    numToName->insert(std::make_pair(5,"F"));
    numToName->insert(std::make_pair(6,"G"));
    numToName->insert(std::make_pair(7,"H"));
    numToName->insert(std::make_pair(8,"I"));
    numToName->insert(std::make_pair(9,"J"));
    numToName->insert(std::make_pair(10,"K"));
    numToName->insert(std::make_pair(11,"L"));
    numToName->insert(std::make_pair(12,"M"));
    numToName->insert(std::make_pair(13,"N"));
    numToName->insert(std::make_pair(14,"O"));
    numToName->insert(std::make_pair(15,"P"));
    numToName->insert(std::make_pair(16,"Q"));
    numToName->insert(std::make_pair(17,"R"));
    numToName->insert(std::make_pair(18,"S"));
    numToName->insert(std::make_pair(19,"T"));
    numToName->insert(std::make_pair(20,"U"));
    numToName->insert(std::make_pair(21,"V"));
    numToName->insert(std::make_pair(22,"W"));
    numToName->insert(std::make_pair(23,"X"));
    numToName->insert(std::make_pair(24,"Y"));
    numToName->insert(std::make_pair(25,"Z"));
}

// Create a new detector
// TODO: I don't think all these arguments are necessary any more
Detector* DetectorInterface::makeDetector(G4int currentDetNum,G4int detsInLayer,G4String type,G4String arrayName,G4int layerNum)
{
    G4String numString;
    std::stringstream convert;
    convert << currentDetNum;
    numString = convert.str();
    // First, create the name of the detector... goes Array.Layer.BarNumber
    G4String name = arrayName + "." + getName(layerNum) + "." + numString;
    //TODO: fully implement all the current detector designs here
    if (type == "MoNABar")
    {
        MoNABar* newBar = new MoNABar();
        newBar->setName(name);
        // Need to tell the detector where the database is
        // TODO: if this is not too memory intensive, change other functions to rely on the pointer
        newBar->setDetectorInterface(this);
        // Add the elements, materials, and volumes of the bar to the database
        setMadeElements(newBar->makeElements(madeElements));
        setMadeMaterials(newBar->makeMaterials(madeMaterials,madeElements));
        setMadeLogVols(newBar->makeLogVols(madeLogVols,madeMaterials));
        setMadeAssemblies(newBar->makeAssemblies(madeAssemblies,madeLogVols));
        return newBar;
    } else
    {
        G4cout << "I failed to find a match to given detector type\n";
    }
}
