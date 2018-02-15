#ifndef ARRAY
#define ARRAY
// C++ library includes
#include <vector>
#include <sstream>
// Geant4 library includes
#include "G4ThreeVector.hh"
#include "G4String.hh"
#include "G4ios.hh"
// My file includes
#include "Layer.hh"
// Forward declaration
class Layer;
// This class is intended to handle "arrays" which simply are collections
// of layers. Intended to characterize MoNA, it is written generally enough
// to characterize any array which can be broken down into layers.
class Array
{
    public:
        Array(G4String name);
//        ~Array();
    private:
        G4ThreeVector tgtPos;            // Target position vector
        G4String arrayName;              // String name of the array
        // TODO: change this to pointers
        // TODO: think about changing these vectors to maps
        std::vector<Layer*> layer;        // Layers which the array contains

    public:
        // Setting functions
        void setTgtPos(G4ThreeVector posVec){tgtPos = posVec;}
        void setArrayName(G4String name){arrayName = name;}

        // Getting functions
        G4ThreeVector getTgtPos(){return tgtPos;}
        G4String getArrayName(){return arrayName;}
        std::vector<Layer*> getLayers(){return layer;}        // Layers which the array contains

        // Adding functions
        void addLayer(G4String info,DetectorInterface* detectorInterface); // Add a layer to the array

};
#endif //ARRAY
