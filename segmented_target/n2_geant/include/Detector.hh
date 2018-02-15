// to avoid some circular dependency problems.. specifically with the interface
#ifndef DETECTORS
#define DETECTORS
class DetectorInterface;
class NamedG4AssemblyVolume;
// C++ library includes
#include <vector>
// Geant4 library includes
#include "G4ThreeVector.hh"
#include "G4String.hh"
#include "G4Element.hh"
#include "G4Material.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4VisAttributes.hh"
#include "G4AssemblyVolume.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4IntersectionSolid.hh"
#include "G4Trd.hh"
#include "G4Cons.hh"
#include "G4Tubs.hh"
#include "G4VPhysicalVolume.hh"
// My file includes
#include "DetectorInterface.hh"
//#include "RandomNumberGenerator.hh"

// Generic Detector base class. We derive all our detector descriptions from here.
// This is used to store information about each detector being created in a general way.
class Detector
{
    public:
        Detector();
        virtual ~Detector(); // Pretty sure this needs to be virtual because of virtual fcts

    protected:
        G4ThreeVector position;       // Detector position vector (points to center)
        G4ThreeVector activeSize;     // Size of the active area of detector
        G4ThreeVector spacingSize;     // Size of the detector for spacing issues
        G4ThreeVector facing;         // Detector position vector (points normal to center)
        G4RotationMatrix* rotation;    // The rotation matrix needed to attain the facing
        //TODO: the name should go array/layer/detector because of GEANT convenience
        G4String name;                // Detector Name
        G4String type;                // Detector type
        DetectorInterface* interface; // Interface (to access element/material/volume information)

    public:
        // Set functions
        void setPosition(G4ThreeVector inPos) {position = inPos;}
        void setActiveSize(G4ThreeVector inSize) {activeSize = inSize;}
        void setSpacingSize(G4ThreeVector inSize) {spacingSize = inSize;}
        void setFacing(G4ThreeVector inFace) {facing = inFace;}
        void setRotation(G4RotationMatrix* inRotate) {rotation = inRotate;}
        void setName(G4String inName) {name = inName;}
        void setType(G4String inType) {type = inType;}
        void setDetectorInterface(DetectorInterface* inInterface) {interface = inInterface;}

        // Get functions
        G4ThreeVector getPosition() {return position;}
        G4ThreeVector getActiveSize() {return activeSize;}
        G4ThreeVector getSpacingSize() {return spacingSize;}
        G4ThreeVector getFacing() {return facing;}
        G4RotationMatrix* getRotation() {return rotation;}
        G4String getName() {return name;}
        G4String getType() {return type;}
        DetectorInterface* getDetectorInterface() {return interface;}
        G4LogicalVolume* getLogVol();
        NamedG4AssemblyVolume* getAssembly();

        // Material / Volume creation functions
        //TODO: we tell it where the detector interface is, so maybe clean these up to reflect that
        virtual std::vector<G4Element*> makeElements(std::vector<G4Element*> madeElements); // Creates the elements needed
        virtual std::vector<G4Material*> makeMaterials(std::vector<G4Material*> madeMaterials,std::vector<G4Element*> madeElements); // Creates the Materials needed
        virtual std::vector<G4LogicalVolume*> makeLogVols(std::vector<G4LogicalVolume*> madeLogVols,std::vector<G4Material*> madeMaterials); // Creates the LogVols needed
        virtual std::vector<NamedG4AssemblyVolume*> makeAssemblies(std::vector<NamedG4AssemblyVolume*> madeAssemblies); // Creates the Assemblies needed
        virtual void addPhysVol(G4LogicalVolume* mother); // Creates the Assemblies needed

        // Locator functions
        G4Element* retrieveElement(std::vector<G4Element*> database, G4String search);
        G4Material* retrieveMaterial(std::vector<G4Material*> database, G4String search);
        G4LogicalVolume* retrieveLogVol(std::vector<G4LogicalVolume*> database, G4String search);
        NamedG4AssemblyVolume* retrieveAssembly(std::vector<NamedG4AssemblyVolume*> database, G4String search);

        // Checker functions
        // Think about moving some of this functionality to the interface class instead of the detector
        G4bool hasElement(G4String name,std::vector<G4Element*> existing); // determines if an element has been added to database
        G4bool hasMaterial(G4String name,std::vector<G4Material*> existing); // determines if a material has been added to database
        G4bool hasLogVol(G4String name,std::vector<G4LogicalVolume*> existing); // determines if a LogVol has been added to database
        G4bool hasAssembly(G4String name,std::vector<NamedG4AssemblyVolume*> existing); // determines if a Assembly has been added to database

        // Physics functions
        virtual std::vector<G4double> processLight(G4Step* aStep);
        virtual std::vector<G4double> addResolutions(G4Step* aStep);

};

///////////////////////////
// Begin Derived Classes //
///////////////////////////
// A MoNA bar.
class MoNABar : public Detector
{
    public:
        // Constructor
        MoNABar();

        // Logistical Functions
        std::vector<G4Element*> makeElements(std::vector<G4Element*> madeElements); // Creates the elements needed
        std::vector<G4Material*> makeMaterials(std::vector<G4Material*> madeMaterials,std::vector<G4Element*> madeElements); // Creates the Materials needed
        std::vector<G4LogicalVolume*> makeLogVols(std::vector<G4LogicalVolume*> madeLogVols,std::vector<G4Material*> madeMaterials); // Creates the LogVols needed
        std::vector<NamedG4AssemblyVolume*> makeAssemblies(std::vector<NamedG4AssemblyVolume*> madeAssemblies,std::vector<G4LogicalVolume*> madeLogVols); // Creates the Assemblies needed
        void addPhysVol(G4LogicalVolume* mother);

        // Physics functions
        std::vector<G4double> processLight(G4Step* aStep);
        std::vector<G4double> addResolutions(G4Step* aStep);
};



// A CMDA bar.
class CMDABar: public Detector
{
    public:
        CMDABar(G4ThreeVector barPos,G4ThreeVector barFace,G4String barName);
};

// a ToF scintillator from the PHOBOS TPC
class PHOBOSToFBar: public Detector
{
    public:
        PHOBOSToFBar();
};

// The long CAESAR scintillators
class LongCAESARScint: public Detector
{
    public:
        LongCAESARScint();
};

// The short CAESAR scintillators
class ShortCAESARScint: public Detector
{
    public:
        ShortCAESARScint();
};
#endif //DETECTORS
