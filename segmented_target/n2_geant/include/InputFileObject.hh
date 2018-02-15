// **********************************************************************

#ifndef InputFileObject_h
#define InputFileObject_h 1

#include <fstream>
#include "globals.hh"
#include <vector>
#include "G4ThreeVector.hh"
#include "TString.h"


class InputFileObject {
public:

  virtual ~InputFileObject();
  static InputFileObject* getInstance();
  static void dispose();

	void SetFileName(G4String inName);
	bool ReadAndSetNeutronInfo();
	bool GetNextNeutron();
	
	G4ThreeVector GetFragPrimVector() {return fPrimVector;}
	G4double GetFragKE() {return fEkin;}
        G4int GetEventNumber() {return fEventNumber;}
        G4int GetNumInputFileLines() {return fNumLines;}
private:

  InputFileObject();
  static InputFileObject* instance;
  
  std::fstream fInFile;
  
  G4int fEventNumber;
  G4int fNumLines;


  G4ThreeVector fPrimVector;
  G4double fEkin;
};

#endif

