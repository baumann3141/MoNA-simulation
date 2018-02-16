// C++ library includes
//#include <vector>
// Geant4 library includes
//#include "G4ThreeVector.hh"
#include "G4String.hh"
#include "G4ios.hh"
// My file includes



// This class is meant to handle the checking of proper inputs
// For now it really is just for the input files...
// TODO: add more robust input checking & proper exception handling
class InputHandler
{
    public:
       // InputHandler();
        //~InputHandler();
    public:
        // Checker functions
        bool validLayerInput(G4String input, int columns);
};

