// C++ library includes
// Geant4 library includes
#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4SubtractionSolid.hh"
#include "G4Trd.hh"
#include "G4NistManager.hh"
// My file includes
#include "Array.hh"

class DetectorConstruction : public G4VUserDetectorConstruction
{
    public:
        DetectorConstruction();
        ~DetectorConstruction();

    private:
        std::vector<Array*> detectorInfo;
        std::vector<G4VPhysicalVolume*> physVols;
    public:
        G4VPhysicalVolume* Construct();
        //TODO:add setDetInfo function
        void setDetInfo(std::vector<Array*> info) {detectorInfo = info;}
        void addPhysVol(G4VPhysicalVolume* inputVolume) {physVols.push_back(inputVolume);}


};
