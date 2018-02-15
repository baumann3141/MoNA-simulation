#ifndef LAYER
#define LAYER
// C++ library includes
#include <vector>
// Geant4 library includes
#include "G4ThreeVector.hh"
#include "G4String.hh"
#include "G4ios.hh"
// My file includes
#include "DetectorInterface.hh"

// This class is meant to characterize a MoNA layer
// but is written generally enough to handle any generalized layer
// TODO: add a layer style interpreter
class Layer
{
    public:
        Layer(); //DetectorInterface* detectorInterface
        ~Layer();

    private:
        G4ThreeVector position;       // Layer position vector (points to center)
        G4ThreeVector facing;         // Layer position vector (points normal to center)
        G4String name;                // Layer Name
        G4String type;                // Layer type (i.e. what kind of bars it contains)
        G4int nBars;                  // Number of bars the layer contains
        std::vector<Detector*> detectors;        // Detectors which the layer contains
        //DetectorInterface* interface;   // link to the detector interface

    public:
        // Set functions
        void setPosition(G4ThreeVector inPos) {position = inPos;}
        void setFacing(G4ThreeVector inFace) {facing = inFace;}
        void setName(G4String inName) {name = inName;}
        void setType(G4String inType) {type = inType;}
        void setNBars(G4int n) {nBars = n;}
        //void setDetectorInterface(DetectorInterface* inInterface) {interface = inInterface;}

        // Get functions
        G4ThreeVector getPosition() {return position;}
        G4ThreeVector getFacing() {return facing;}
        G4String getName() {return name;}
        G4String getType() {return type;}
        G4int getNBars() {return nBars;}
        std::vector<Detector*> getDetectors() {return detectors;}
        //DetectorInterface* getDetectorInterface() {return interface;}

        // More complex functions
        // Adds detectors to the layer
        void addDetectors(G4int nBars,G4ThreeVector pos,DetectorInterface* detectorInterface,G4String type,G4String arrayName,G4int layerNum, G4double facingRotation); 

};
#endif //LAYER
