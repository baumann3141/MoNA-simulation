////////////
// Intro: //
////////////
// This creates two classes - a detector interface and a slightly custom version
// of G4AssemblyVolume ... we needed a parameter that that class didn't provide
// to avoid some circular dependency problems
#ifndef DET_INTERFACE
#define DET_INTERFACE
class Geant4RNG;
class Detector;
class TrackerSD;
class OutputHandler;
// C++ library includes
#include <vector>
#include <map>
#include <sstream>
// Geant4 library includes
#include "G4Element.hh"
#include "G4Material.hh"
#include "G4LogicalVolume.hh"
#include "G4AssemblyVolume.hh"
#include "G4SDManager.hh"
// My file includes
#include "Detector.hh"
#include "TrackerSD.hh"
#include "Array.hh"
//#include "RandomNumberGenerator.hh"


class Array;
class NamedG4AssemblyVolume : public G4AssemblyVolume 
{
    private:
        G4String name; // a name
        
    public:
        // Set Functions
        void SetName(G4String inName) {name = inName;}

        // Get Functions
        G4String GetName() {return name;}
};
// The detector interface stores and interprets information
// necessary to build up a variety of detectors. It includes a 
// database of materials and logical volumes so these things
// don't have to be created for every single detector
class DetectorInterface
{
    public:
        DetectorInterface(); // Constructor
    private:
        std::vector<G4Element*> madeElements;   // Keeps track of the created elements
        std::vector<G4Material*> madeMaterials; // Keeps track of the created materials
        std::vector<G4LogicalVolume*> madeLogVols;  // Keeps track of the created logical volumes
        // TODO: it is silly to have maps that are effectively duplicate.... fix this some day
        // TODO: also, want to make a proper check that you don't create too many layers 
        // and outstrip the names
        // ALSO... do you even need this since the names are set by layer in input file?
        std::map<G4String, G4int>* nameToNum;       // Interface for layer names
        std::map<G4int, G4String>* numToName;       // Interface for layer names
        std::vector<NamedG4AssemblyVolume*> madeAssemblies;  // Keeps track of the created logical volumes
        G4SDManager* SDman;    // Sensitive Detector Manager
        TrackerSD* aTrackerSD; // Sensitive detector
        OutputHandler* outHandler; // the output handler class
	Geant4RNG* XRNG;
        // vector of arrays
        std::vector<Array*> array; 
    public:
        // Set functions
        void setMadeElements(std::vector<G4Element*> inElements) {madeElements = inElements;}
        void setMadeMaterials(std::vector<G4Material*> inMaterials) {madeMaterials = inMaterials;}
        void setMadeLogVols(std::vector<G4LogicalVolume*> inLogVols) {madeLogVols = inLogVols;}
        void setMadeAssemblies(std::vector<NamedG4AssemblyVolume*> inAssemblies) {madeAssemblies = inAssemblies;}
        void setSDman(G4SDManager* inSDman) {SDman = inSDman;}
        void setTrackerSD(TrackerSD* inTrackerSD) {aTrackerSD = inTrackerSD;}
        void setOutputHandler(OutputHandler* inOutputHandler) {outHandler = inOutputHandler;}
        void setArrays(std::vector<Array*> a) {array = a;}

        // Get functions
        std::vector<G4Element*> getMadeElements() {return madeElements;}
        std::vector<G4Material*> getMadeMaterials() {return madeMaterials;}
        std::vector<G4LogicalVolume*> getMadeLogVols() {return madeLogVols;}
        std::vector<NamedG4AssemblyVolume*> getMadeAssemblies() {return madeAssemblies;}
        G4SDManager* getSDman() {return SDman;}
        TrackerSD* getTrackerSD() {return aTrackerSD;}
	OutputHandler* getOutputHandler() {return outHandler;}
        std::vector<Array*> getArrays() {return array;}

        // More complex fcts
        Detector* makeDetector(G4int currentDetNum,G4int detsInLayer,G4String type,G4String arrayName,G4int layerNum);
        G4int getNumber(G4String name) {return (*nameToNum)[name];}
        G4String getName(G4int number) {return (*numToName)[number];}
};
#endif //DET_INTERFACE
