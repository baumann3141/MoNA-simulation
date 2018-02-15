
#include "InputFileObject.hh"

InputFileObject* InputFileObject::instance = 0;

//_________________________________________________
InputFileObject::InputFileObject()
{

  fEventNumber = 0;

}


//_______________________________________________
InputFileObject::~InputFileObject()
{
}

//___________________________________________________________________________________________
void InputFileObject::SetFileName(G4String inName){
	
  G4cout << "Inside InputFileObject::SetFileName: " << inName << G4endl;
  //TString dir = "/evtdata/08016a/simdata/kohley_simdata/COMD_output/RootFiles/";
  //TString name = "c1md_s30be9_t400_Neutrons.dat";
  
  if(fInFile.is_open()){
    fInFile.close();
    fInFile.clear();
  }
   
  fInFile.open(inName.data(),fstream::in);
  if(!fInFile){printf("File: %s  NOT OPENED\n", inName.data());}
  else{	printf("Opening %s\n",inName.data());	}


  //Count number of lines to supply G4 with total number of events
  TString dataline;  	
  fNumLines = 0;
  while(1){
    dataline.ReadLine(fInFile); 
    fNumLines++;
    if(fInFile.eof()) break;
  }
  G4cout << "Total Number of Lines (Events) for Neutron File Input is " << fNumLines << G4endl;


  //Re-open inputfile to start at top
  fInFile.close();
  fInFile.clear();
  fInFile.open(inName.data(),fstream::in);
}


//_______________________________________________
bool InputFileObject::GetNextNeutron(){

  bool EndOfFile = ReadAndSetNeutronInfo();

  return EndOfFile;

}

//__________________________________________
bool InputFileObject::ReadAndSetNeutronInfo(){
	

  TString dataline;  	
  if(fInFile.eof()){
    G4cout << "!!!!!!!!! End of InputFile !!!!!!!!!" << G4endl;
    return true;
  }

  dataline.ReadLine(fInFile); 
  
  Int_t event;
  Double_t x, y, z, ke;
  sscanf(dataline.Data(), "%d %lf %lf %lf %lf",&event, &x, &y, &z, &ke);

  fEventNumber = event;
  fPrimVector.set(x,y,z);  
  fEkin = ke*MeV;  


 if( (fEventNumber%25000)==0) printf("  --> InputFile Event #: %d \n",fEventNumber);	
 
 return false;
}

//_________________________________________________________
InputFileObject* InputFileObject::getInstance()
{
  if (instance == 0) instance = new InputFileObject();
  return instance;
}

//_________________________________________
void InputFileObject::dispose()
{
  if (instance != 0)
  {
    delete instance;
    instance = 0;
  }
}


