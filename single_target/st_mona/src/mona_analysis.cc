#include "stdlib.h"
#include "deb_err.h"
#include "mona_event.hh"
#include "mona_analysis.hh"
#include <TFile.h>
#include <TVector3.h>
#include <gsl/gsl_pow_int.h>
#include "st_particle.hh"
#include "c_xdr_output_stream.hh"
#include <string>
#include "CycSrim.h"

using namespace std;

void CalMonaEvent::setFragPar(CalMonaEvent* e) {
  setMonaX(e->getMonaX());
  setMonaY(e->getMonaY());
  setMonaZ(e->getMonaZ());
  setMonaTime(e->getMonaTime());
}

void CalMonaEvent::setMonaPar(CalMonaEvent* e) {
  setFragX(e->getFragX());
  setFragTX(e->getFragTX());
  setFragY(e->getFragY());
  setFragTY(e->getFragTY());
  
  setTargetX(e->getTargetX());
  setTargetY(e->getTargetY());
}

void CalMonaEventSim::init() {
  string bn[] = {"b13p1x", "b13p1y", "b13p1z", "b13p1t", "b13p1Ekin",// branch names
		 "b13p2x", "b13p2y", "b13p2z", "b13p2t", "b13p2Ekin",
		 "b13p3x", "b13p3y", "b13p3z", "b13p3t", "b13p3Ekin",
  	         "b13p4x", "b13p4y", "b13p4z", "b13p4t", "b13p4Ekin",
	


	         "b13pn1g1x", "b13pn1g1y", "b13pn1g1z", "b13pn1g1t", "b13pn1g1Edep", "b13pn1g1light",
		 "b13pn1g2x", "b13pn1g2y", "b13pn1g2z", "b13pn1g2t", "b13pn1g2Edep", "b13pn1g2light",
		 "b13pn1g3x", "b13pn1g3y", "b13pn1g3z", "b13pn1g3t", "b13pn1g3Edep", "b13pn1g3light",
		 "b13pn1g4x", "b13pn1g4y", "b13pn1g4z", "b13pn1g4t", "b13pn1g4Edep", "b13pn1g4light",
		 "b13pn1g5x", "b13pn1g5y", "b13pn1g5z", "b13pn1g5t", "b13pn1g5Edep", "b13pn1g5light",
		 "b13pn1g6x", "b13pn1g6y", "b13pn1g6z", "b13pn1g6t", "b13pn1g6Edep", "b13pn1g6light",
	         "b13pn1g7x", "b13pn1g7y", "b13pn1g7z", "b13pn1g7t", "b13pn1g7Edep", "b13pn1g7light",
		 "b13pn1g8x", "b13pn1g8y", "b13pn1g8z", "b13pn1g8t", "b13pn1g8Edep", "b13pn1g8light",
		 "b13pn1g9x", "b13pn1g9y", "b13pn1g9z", "b13pn1g9t", "b13pn1g9Edep", "b13pn1g9light",
		 "b13pn1g10x", "b13pn1g10y", "b13pn1g10z", "b13pn1g10t", "b13pn1g10Edep", "b13pn1g10light",// End N1

	         "b13pn2g1x", "b13pn2g1y", "b13pn2g1z", "b13pn2g1t", "b13pn2g1Edep", "b13pn2g1light",
		 "b13pn2g2x", "b13pn2g2y", "b13pn2g2z", "b13pn2g2t", "b13pn2g2Edep", "b13pn2g2light",
		 "b13pn2g3x", "b13pn2g3y", "b13pn2g3z", "b13pn2g3t", "b13pn2g3Edep", "b13pn2g3light",
		 "b13pn2g4x", "b13pn2g4y", "b13pn2g4z", "b13pn2g4t", "b13pn2g4Edep", "b13pn2g4light",
		 "b13pn2g5x", "b13pn2g5y", "b13pn2g5z", "b13pn2g5t", "b13pn2g5Edep", "b13pn2g5light",
		 "b13pn2g6x", "b13pn2g6y", "b13pn2g6z", "b13pn2g6t", "b13pn2g6Edep", "b13pn2g6light",
	         "b13pn2g7x", "b13pn2g7y", "b13pn2g7z", "b13pn2g7t", "b13pn2g7Edep", "b13pn2g7light",
		 "b13pn2g8x", "b13pn2g8y", "b13pn2g8z", "b13pn2g8t", "b13pn2g8Edep", "b13pn2g8light",
		 "b13pn2g9x", "b13pn2g9y", "b13pn2g9z", "b13pn2g9t", "b13pn2g9Edep", "b13pn2g9light",
		 "b13pn2g10x", "b13pn2g10y", "b13pn2g10z", "b13pn2g10t", "b13pn2g10Edep", "b13pn2g10light",// End N2

	         "b13pn3g1x", "b13pn3g1y", "b13pn3g1z", "b13pn3g1t", "b13pn3g1Edep", "b13pn3g1light",
		 "b13pn3g2x", "b13pn3g2y", "b13pn3g2z", "b13pn3g2t", "b13pn3g2Edep", "b13pn3g2light",
		 "b13pn3g3x", "b13pn3g3y", "b13pn3g3z", "b13pn3g3t", "b13pn3g3Edep", "b13pn3g3light",
		 "b13pn3g4x", "b13pn3g4y", "b13pn3g4z", "b13pn3g4t", "b13pn3g4Edep", "b13pn3g4light",
		 "b13pn3g5x", "b13pn3g5y", "b13pn3g5z", "b13pn3g5t", "b13pn3g5Edep", "b13pn3g5light",
		 "b13pn3g6x", "b13pn3g6y", "b13pn3g6z", "b13pn3g6t", "b13pn3g6Edep", "b13pn3g6light",
	         "b13pn3g7x", "b13pn3g7y", "b13pn3g7z", "b13pn3g7t", "b13pn3g7Edep", "b13pn3g7light",
		 "b13pn3g8x", "b13pn3g8y", "b13pn3g8z", "b13pn3g8t", "b13pn3g8Edep", "b13pn3g8light",
		 "b13pn3g9x", "b13pn3g9y", "b13pn3g9z", "b13pn3g9t", "b13pn3g9Edep", "b13pn3g9light",
		 "b13pn3g10x", "b13pn3g10y", "b13pn3g10z", "b13pn3g10t", "b13pn3g10Edep", "b13pn3g10light",// End N3


	         "b13pg1x", "b13pg1y", "b13pg1z", "b13pg1t", "b13pg1Edep", "b13pg1light", // Other bullshit
		 "b13pg2x", "b13pg2y", "b13pg2z", "b13pg2t", "b13pg2Edep", "b13pg2light",
		 "b13pg3x", "b13pg3y", "b13pg3z", "b13pg3t", "b13pg3Edep", "b13pg3light",
		 "b13pg4x", "b13pg4y", "b13pg4z", "b13pg4t", "b13pg4Edep", "b13pg4light",
		 "b13pg5x", "b13pg5y", "b13pg5z", "b13pg5t", "b13pg5Edep", "b13pg5light",
		 "b13pg6x", "b13pg6y", "b13pg6z", "b13pg6t", "b13pg6Edep", "b13pg6light",
	         "b13pg7x", "b13pg7y", "b13pg7z", "b13pg7t", "b13pg7Edep", "b13pg7light",
		 "b13pg8x", "b13pg8y", "b13pg8z", "b13pg8t", "b13pg8Edep", "b13pg8light",
		 "b13pg9x", "b13pg9y", "b13pg9z", "b13pg9t", "b13pg9Edep", "b13pg9light",
		 "b13pg10x", "b13pg10y", "b13pg10z", "b13pg10t", "b13pg10Edep", "b13pg10light",
	         "b13pg11x", "b13pg11y", "b13pg11z", "b13pg11t", "b13pg11Edep", "b13pg11light",
		 "b13pg12x", "b13pg12y", "b13pg12z", "b13pg12t", "b13pg12Edep", "b13pg12light",
		 "b13pg13x", "b13pg13y", "b13pg13z", "b13pg13t", "b13pg13Edep", "b13pg13light",
		 "b13pg14x", "b13pg14y", "b13pg14z", "b13pg14t", "b13pg14Edep", "b13pg14light",
		 "b13pg15x", "b13pg15y", "b13pg15z", "b13pg15t", "b13pg15Edep", "b13pg15light",
		 "b13pg16x", "b13pg16y", "b13pg16z", "b13pg16t", "b13pg16Edep", "b13pg16light",
	         "b13pg17x", "b13pg17y", "b13pg17z", "b13pg17t", "b13pg17Edep", "b13pg17light",
		 "b13pg18x", "b13pg18y", "b13pg18z", "b13pg18t", "b13pg18Edep", "b13pg18light",
		 "b13pg19x", "b13pg19y", "b13pg19z", "b13pg19t", "b13pg19Edep", "b13pg19light",
		 "b13pg20x", "b13pg20y", "b13pg20z", "b13pg20t", "b13pg20Edep", "b13pg20light",

                 "b7p0x", "b7p0tx", "b7p0y", "b7p0ty", "b7p0t", "b7p0Ekin",

                 "b2p0x", "b2p0y",
  };

  double* ps[] = {&mx,&my,&mz,&mt,&mEkin,// pointers
		  &m2x,&m2y,&m2z,&m2t,&m2Ekin,
		  &m3x,&m3y,&m3z,&m3t,&m3Ekin,
	          &m4x,&m4y,&m4z,&m4t,&m4Ekin,


		  &mN1G1x,&mN1G1y,&mN1G1z,&mN1G1t,&mN1G1Edep,&mN1G1light,
		  &mN1G2x,&mN1G2y,&mN1G2z,&mN1G2t,&mN1G2Edep,&mN1G2light,
		  &mN1G3x,&mN1G3y,&mN1G3z,&mN1G3t,&mN1G3Edep,&mN1G3light,
		  &mN1G4x,&mN1G4y,&mN1G4z,&mN1G4t,&mN1G4Edep,&mN1G4light,
		  &mN1G5x,&mN1G5y,&mN1G5z,&mN1G5t,&mN1G5Edep,&mN1G5light,
		  &mN1G6x,&mN1G6y,&mN1G6z,&mN1G6t,&mN1G6Edep,&mN1G6light,
		  &mN1G7x,&mN1G7y,&mN1G7z,&mN1G7t,&mN1G7Edep,&mN1G7light,
		  &mN1G8x,&mN1G8y,&mN1G8z,&mN1G8t,&mN1G8Edep,&mN1G8light,
		  &mN1G9x,&mN1G9y,&mN1G9z,&mN1G9t,&mN1G9Edep,&mN1G9light,
		  &mN1G10x,&mN1G10y,&mN1G10z,&mN1G10t,&mN1G10Edep,&mN1G10light, // End N1

		  &mN2G1x,&mN2G1y,&mN2G1z,&mN2G1t,&mN2G1Edep,&mN2G1light,
		  &mN2G2x,&mN2G2y,&mN2G2z,&mN2G2t,&mN2G2Edep,&mN2G2light,
		  &mN2G3x,&mN2G3y,&mN2G3z,&mN2G3t,&mN2G3Edep,&mN2G3light,
		  &mN2G4x,&mN2G4y,&mN2G4z,&mN2G4t,&mN2G4Edep,&mN2G4light,
		  &mN2G5x,&mN2G5y,&mN2G5z,&mN2G5t,&mN2G5Edep,&mN2G5light,
		  &mN2G6x,&mN2G6y,&mN2G6z,&mN2G6t,&mN2G6Edep,&mN2G6light,
		  &mN2G7x,&mN2G7y,&mN2G7z,&mN2G7t,&mN2G7Edep,&mN2G7light,
		  &mN2G8x,&mN2G8y,&mN2G8z,&mN2G8t,&mN2G8Edep,&mN2G8light,
		  &mN2G9x,&mN2G9y,&mN2G9z,&mN2G9t,&mN2G9Edep,&mN2G9light,
		  &mN2G10x,&mN2G10y,&mN2G10z,&mN2G10t,&mN2G10Edep,&mN2G10light, // End N2

		  &mN3G1x,&mN3G1y,&mN3G1z,&mN3G1t,&mN3G1Edep,&mN3G1light,
		  &mN3G2x,&mN3G2y,&mN3G2z,&mN3G2t,&mN3G2Edep,&mN3G2light,
		  &mN3G3x,&mN3G3y,&mN3G3z,&mN3G3t,&mN3G3Edep,&mN3G3light,
		  &mN3G4x,&mN3G4y,&mN3G4z,&mN3G4t,&mN3G4Edep,&mN3G4light,
		  &mN3G5x,&mN3G5y,&mN3G5z,&mN3G5t,&mN3G5Edep,&mN3G5light,
		  &mN3G6x,&mN3G6y,&mN3G6z,&mN3G6t,&mN3G6Edep,&mN3G6light,
		  &mN3G7x,&mN3G7y,&mN3G7z,&mN3G7t,&mN3G7Edep,&mN3G7light,
		  &mN3G8x,&mN3G8y,&mN3G8z,&mN3G8t,&mN3G8Edep,&mN3G8light,
		  &mN3G9x,&mN3G9y,&mN3G9z,&mN3G9t,&mN3G9Edep,&mN3G9light,
		  &mN3G10x,&mN3G10y,&mN3G10z,&mN3G10t,&mN3G10Edep,&mN3G10light, // End N3

		  &mG1x,&mG1y,&mG1z,&mG1t,&mG1Edep,&mG1light,
		  &mG2x,&mG2y,&mG2z,&mG2t,&mG2Edep,&mG2light,
		  &mG3x,&mG3y,&mG3z,&mG3t,&mG3Edep,&mG3light,
		  &mG4x,&mG4y,&mG4z,&mG4t,&mG4Edep,&mG4light,
		  &mG5x,&mG5y,&mG5z,&mG5t,&mG5Edep,&mG5light,
		  &mG6x,&mG6y,&mG6z,&mG6t,&mG6Edep,&mG6light,
		  &mG7x,&mG7y,&mG7z,&mG7t,&mG7Edep,&mG7light,
		  &mG8x,&mG8y,&mG8z,&mG8t,&mG8Edep,&mG8light,
		  &mG9x,&mG9y,&mG9z,&mG9t,&mG9Edep,&mG9light,
		  &mG10x,&mG10y,&mG10z,&mG10t,&mG10Edep,&mG10light,
		  &mG11x,&mG11y,&mG11z,&mG11t,&mG11Edep,&mG11light,
		  &mG12x,&mG12y,&mG12z,&mG12t,&mG12Edep,&mG12light,
		  &mG13x,&mG13y,&mG13z,&mG13t,&mG13Edep,&mG13light,
		  &mG14x,&mG14y,&mG14z,&mG14t,&mG14Edep,&mG14light,
		  &mG15x,&mG15y,&mG15z,&mG15t,&mG15Edep,&mG15light,
		  &mG16x,&mG16y,&mG16z,&mG16t,&mG16Edep,&mG16light,
		  &mG17x,&mG17y,&mG17z,&mG17t,&mG17Edep,&mG17light,
		  &mG18x,&mG18y,&mG18z,&mG18t,&mG18Edep,&mG18light,
		  &mG19x,&mG19y,&mG19z,&mG19t,&mG19Edep,&mG19light,
		  &mG20x,&mG20y,&mG20z,&mG20t,&mG20Edep,&mG20light,

                  &fx,&ftx,&fy,&fty,&ft,&fEkin, 

                  &tx,&ty};   

  string bnint[] = { "b13pn1gmultiplicity", "b13pn2gmultiplicity","b13pn3gmultiplicity", "b13pn4gmultiplicity"};

  int* psint[] = {&mN1GM,&mN2GM,&mN3GM,&mN4GM};//   

  for (size_t i(0); i < sizeof(ps)/sizeof(double*); i++) {
    DSV(i);
    DSV(bn[i]);
    DSV(ps[i]);
    fT->SetBranchAddress(bn[i].c_str(),ps[i]);  /// \todo generate error in case of error
  }

    for (size_t i(0); i < sizeof(psint)/sizeof(double*); i++) {
    DSV(i);
    DSV(bnint[i]);
    DSV(psint[i]);
    fT->SetBranchAddress(bnint[i].c_str(),psint[i]);  /// \todo generate error in case of error
  }

  DEB("Done");
}

void CalMonaEvent::writeFilterHeader(CXdrOutputStream* o) {
  *o<<string("header");
  // other things here...  (like in c_xdr_output_stream.cc at the bottom)
 *o<< int(256);
 *o<< string("n1X_hit");
 *o<< string("n1Y_hit");
 *o<< string("n1Z_hit");
 *o<< string("n1ToF_hit");
 *o<< string("n2X_hit");
 *o<< string("n2Y_hit");
 *o<< string("n2Z_hit");
 *o<< string("n2ToF_hit");
 *o<< string("n3X_hit");
 *o<< string("n3Y_hit");
 *o<< string("n3Z_hit");
 *o<< string("n3ToF_hit");
 *o<< string("n4X_hit");
 *o<< string("n4Y_hit");
 *o<< string("n4Z_hit");
 *o<< string("n4ToF_hit");
 *o<< string("n1gMultiplicity");
 *o<< string("n1g1X_hit");
 *o<< string("n1g1Y_hit");
 *o<< string("n1g1Z_hit");
 *o<< string("n1g1ToF_hit");
 *o<< string("n1g1Edep_hit");
 *o<< string("n1g1light_hit");
 *o<< string("n1g2X_hit");
 *o<< string("n1g2Y_hit");
 *o<< string("n1g2Z_hit");
 *o<< string("n1g2ToF_hit");
 *o<< string("n1g2Edep_hit");
 *o<< string("n1g2light_hit");
 *o<< string("n1g3X_hit");
 *o<< string("n1g3Y_hit");
 *o<< string("n1g3Z_hit");
 *o<< string("n1g3ToF_hit");
 *o<< string("n1g3Edep_hit");
 *o<< string("n1g3light_hit");
 *o<< string("n1g4X_hit");
 *o<< string("n1g4Y_hit");
 *o<< string("n1g4Z_hit");
 *o<< string("n1g4ToF_hit");
 *o<< string("n1g4Edep_hit");
 *o<< string("n1g4light_hit");
 *o<< string("n1g5X_hit");
 *o<< string("n1g5Y_hit");
 *o<< string("n1g5Z_hit");
 *o<< string("n1g5ToF_hit");
 *o<< string("n1g5Edep_hit");
 *o<< string("n1g5light_hit");
 *o<< string("n1g6X_hit");
 *o<< string("n1g6Y_hit");
 *o<< string("n1g6Z_hit");
 *o<< string("n1g6ToF_hit");
 *o<< string("n1g6Edep_hit");
 *o<< string("n1g6light_hit");
 *o<< string("n1g7X_hit");
 *o<< string("n1g7Y_hit");
 *o<< string("n1g7Z_hit");
 *o<< string("n1g7ToF_hit");
 *o<< string("n1g7Edep_hit");
 *o<< string("n1g7light_hit");
 *o<< string("n1g8X_hit");
 *o<< string("n1g8Y_hit");
 *o<< string("n1g8Z_hit");
 *o<< string("n1g8ToF_hit");
 *o<< string("n1g8Edep_hit");
 *o<< string("n1g8light_hit");
 *o<< string("n1g9X_hit");
 *o<< string("n1g9Y_hit");
 *o<< string("n1g9Z_hit");
 *o<< string("n1g9ToF_hit");
 *o<< string("n1g9Edep_hit");
 *o<< string("n1g9light_hit");
 *o<< string("n1g10X_hit");
 *o<< string("n1g10Y_hit");
 *o<< string("n1g10Z_hit");
 *o<< string("n1g10ToF_hit");
 *o<< string("n1g10Edep_hit");
 *o<< string("n1g10light_hit");
 *o<< string("n2gMultiplicity");// Start 2N
 *o<< string("n2g1X_hit");
 *o<< string("n2g1Y_hit");
 *o<< string("n2g1Z_hit");
 *o<< string("n2g1ToF_hit");
 *o<< string("n2g1Edep_hit");
 *o<< string("n2g1light_hit");
 *o<< string("n2g2X_hit");
 *o<< string("n2g2Y_hit");
 *o<< string("n2g2Z_hit");
 *o<< string("n2g2ToF_hit");
 *o<< string("n2g2Edep_hit");
 *o<< string("n2g2light_hit");
 *o<< string("n2g3X_hit");
 *o<< string("n2g3Y_hit");
 *o<< string("n2g3Z_hit");
 *o<< string("n2g3ToF_hit");
 *o<< string("n2g3Edep_hit");
 *o<< string("n2g3light_hit");
 *o<< string("n2g4X_hit");
 *o<< string("n2g4Y_hit");
 *o<< string("n2g4Z_hit");
 *o<< string("n2g4ToF_hit");
 *o<< string("n2g4Edep_hit");
 *o<< string("n2g4light_hit");
 *o<< string("n2g5X_hit");
 *o<< string("n2g5Y_hit");
 *o<< string("n2g5Z_hit");
 *o<< string("n2g5ToF_hit");
 *o<< string("n2g5Edep_hit");
 *o<< string("n2g5light_hit");
 *o<< string("n2g6X_hit");
 *o<< string("n2g6Y_hit");
 *o<< string("n2g6Z_hit");
 *o<< string("n2g6ToF_hit");
 *o<< string("n2g6Edep_hit");
 *o<< string("n2g6light_hit");
 *o<< string("n2g7X_hit");
 *o<< string("n2g7Y_hit");
 *o<< string("n2g7Z_hit");
 *o<< string("n2g7ToF_hit");
 *o<< string("n2g7Edep_hit");
 *o<< string("n2g7light_hit");
 *o<< string("n2g8X_hit");
 *o<< string("n2g8Y_hit");
 *o<< string("n2g8Z_hit");
 *o<< string("n2g8ToF_hit");
 *o<< string("n2g8Edep_hit");
 *o<< string("n2g8light_hit");
 *o<< string("n2g9X_hit");
 *o<< string("n2g9Y_hit");
 *o<< string("n2g9Z_hit");
 *o<< string("n2g9ToF_hit");
 *o<< string("n2g9Edep_hit");
 *o<< string("n2g9light_hit");
 *o<< string("n2g10X_hit");
 *o<< string("n2g10Y_hit");
 *o<< string("n2g10Z_hit");
 *o<< string("n2g10ToF_hit");
 *o<< string("n2g10Edep_hit");
 *o<< string("n2g10light_hit");// End 2N
 *o<< string("n3gMultiplicity");// Start 3N
 *o<< string("n3g1X_hit");
 *o<< string("n3g1Y_hit");
 *o<< string("n3g1Z_hit");
 *o<< string("n3g1ToF_hit");
 *o<< string("n3g1Edep_hit");
 *o<< string("n3g1light_hit");
 *o<< string("n3g2X_hit");
 *o<< string("n3g2Y_hit");
 *o<< string("n3g2Z_hit");
 *o<< string("n3g2ToF_hit");
 *o<< string("n3g2Edep_hit");
 *o<< string("n3g2light_hit");
 *o<< string("n3g3X_hit");
 *o<< string("n3g3Y_hit");
 *o<< string("n3g3Z_hit");
 *o<< string("n3g3ToF_hit");
 *o<< string("n3g3Edep_hit");
 *o<< string("n3g3light_hit");
 *o<< string("n3g4X_hit");
 *o<< string("n3g4Y_hit");
 *o<< string("n3g4Z_hit");
 *o<< string("n3g4ToF_hit");
 *o<< string("n3g4Edep_hit");
 *o<< string("n3g4light_hit");
 *o<< string("n3g5X_hit");
 *o<< string("n3g5Y_hit");
 *o<< string("n3g5Z_hit");
 *o<< string("n3g5ToF_hit");
 *o<< string("n3g5Edep_hit");
 *o<< string("n3g5light_hit");
 *o<< string("n3g6X_hit");
 *o<< string("n3g6Y_hit");
 *o<< string("n3g6Z_hit");
 *o<< string("n3g6ToF_hit");
 *o<< string("n3g6Edep_hit");
 *o<< string("n3g6light_hit");
 *o<< string("n3g7X_hit");
 *o<< string("n3g7Y_hit");
 *o<< string("n3g7Z_hit");
 *o<< string("n3g7ToF_hit");
 *o<< string("n3g7Edep_hit");
 *o<< string("n3g7light_hit");
 *o<< string("n3g8X_hit");
 *o<< string("n3g8Y_hit");
 *o<< string("n3g8Z_hit");
 *o<< string("n3g8ToF_hit");
 *o<< string("n3g8Edep_hit");
 *o<< string("n3g8light_hit");
 *o<< string("n3g9X_hit");
 *o<< string("n3g9Y_hit");
 *o<< string("n3g9Z_hit");
 *o<< string("n3g9ToF_hit");
 *o<< string("n3g9Edep_hit");
 *o<< string("n3g9light_hit");
 *o<< string("n3g10X_hit");
 *o<< string("n3g10Y_hit");
 *o<< string("n3g10Z_hit");
 *o<< string("n3g10ToF_hit");
 *o<< string("n3g10Edep_hit");
 *o<< string("n3g10light_hit");// End 3N
 *o<< string("n4gMultiplicity");// Start 4N
 *o<< string("n4g1X_hit");
 *o<< string("n4g1Y_hit");
 *o<< string("n4g1Z_hit");
 *o<< string("n4g1ToF_hit");
 *o<< string("n4g1Edep_hit");
 *o<< string("n4g1light_hit");
 *o<< string("n4g2X_hit");
 *o<< string("n4g2Y_hit");
 *o<< string("n4g2Z_hit");
 *o<< string("n4g2ToF_hit");
 *o<< string("n4g2Edep_hit");
 *o<< string("n4g2light_hit");
 *o<< string("n4g3X_hit");
 *o<< string("n4g3Y_hit");
 *o<< string("n4g3Z_hit");
 *o<< string("n4g3ToF_hit");
 *o<< string("n4g3Edep_hit");
 *o<< string("n4g3light_hit");
 *o<< string("n4g4X_hit");
 *o<< string("n4g4Y_hit");
 *o<< string("n4g4Z_hit");
 *o<< string("n4g4ToF_hit");
 *o<< string("n4g4Edep_hit");
 *o<< string("n4g4light_hit");
 *o<< string("n4g5X_hit");
 *o<< string("n4g5Y_hit");
 *o<< string("n4g5Z_hit");
 *o<< string("n4g5ToF_hit");
 *o<< string("n4g5Edep_hit");
 *o<< string("n4g5light_hit");
 *o<< string("n4g6X_hit");
 *o<< string("n4g6Y_hit");
 *o<< string("n4g6Z_hit");
 *o<< string("n4g6ToF_hit");
 *o<< string("n4g6Edep_hit");
 *o<< string("n4g6light_hit");
 *o<< string("n4g7X_hit");
 *o<< string("n4g7Y_hit");
 *o<< string("n4g7Z_hit");
 *o<< string("n4g7ToF_hit");
 *o<< string("n4g7Edep_hit");
 *o<< string("n4g7light_hit");
 *o<< string("n4g8X_hit");
 *o<< string("n4g8Y_hit");
 *o<< string("n4g8Z_hit");
 *o<< string("n4g8ToF_hit");
 *o<< string("n4g8Edep_hit");
 *o<< string("n4g8light_hit");
 *o<< string("n4g9X_hit");
 *o<< string("n4g9Y_hit");
 *o<< string("n4g9Z_hit");
 *o<< string("n4g9ToF_hit");
 *o<< string("n4g9Edep_hit");
 *o<< string("n4g9light_hit");
 *o<< string("n4g10X_hit");
 *o<< string("n4g10Y_hit");
 *o<< string("n4g10Z_hit");
 *o<< string("n4g10ToF_hit");
 *o<< string("n4g10Edep_hit");
 *o<< string("n4g10light_hit");// End 4N

 *o<< string("g1X_hit");
 *o<< string("g1Y_hit");
 *o<< string("g1Z_hit");
 *o<< string("g1ToF_hit");
 *o<< string("g1Edep_hit");
 *o<< string("g1light_hit");
 *o<< string("g2X_hit");
 *o<< string("g2Y_hit");
 *o<< string("g2Z_hit");
 *o<< string("g2ToF_hit");
 *o<< string("g2Edep_hit");
 *o<< string("g2light_hit");
 *o<< string("g3X_hit");
 *o<< string("g3Y_hit");
 *o<< string("g3Z_hit");
 *o<< string("g3ToF_hit");
 *o<< string("g3Edep_hit");
 *o<< string("g3light_hit");
 *o<< string("g4X_hit");
 *o<< string("g4Y_hit");
 *o<< string("g4Z_hit");
 *o<< string("g4ToF_hit");
 *o<< string("g4Edep_hit");
 *o<< string("g4light_hit");
 *o<< string("g5X_hit");
 *o<< string("g5Y_hit");
 *o<< string("g5Z_hit");
 *o<< string("g5ToF_hit");
 *o<< string("g5Edep_hit");
 *o<< string("g5light_hit");
 *o<< string("g6X_hit");
 *o<< string("g6Y_hit");
 *o<< string("g6Z_hit");
 *o<< string("g6ToF_hit");
 *o<< string("g6Edep_hit");
 *o<< string("g6light_hit");
 *o<< string("g7X_hit");
 *o<< string("g7Y_hit");
 *o<< string("g7Z_hit");
 *o<< string("g7ToF_hit");
 *o<< string("g7Edep_hit");
 *o<< string("g7light_hit");
 *o<< string("g8X_hit");
 *o<< string("g8Y_hit");
 *o<< string("g8Z_hit");
 *o<< string("g8ToF_hit");
 *o<< string("g8Edep_hit");
 *o<< string("g8light_hit");
 *o<< string("g9X_hit");
 *o<< string("g9Y_hit");
 *o<< string("g9Z_hit");
 *o<< string("g9ToF_hit");
 *o<< string("g9Edep_hit");
 *o<< string("g9light_hit");
 *o<< string("g10X_hit");
 *o<< string("g10Y_hit");
 *o<< string("g10Z_hit");
 *o<< string("g10ToF_hit");
 *o<< string("g10Edep_hit");
 *o<< string("g10light_hit");
 *o<< string("g11X_hit");
 *o<< string("g1Y_hit");
 *o<< string("g11Z_hit");
 *o<< string("g11ToF_hit");
 *o<< string("g11Edep_hit");
 *o<< string("g11light_hit");
 *o<< string("g12X_hit");
 *o<< string("g12Y_hit");
 *o<< string("g12Z_hit");
 *o<< string("g12ToF_hit");
 *o<< string("g12Edep_hit");
 *o<< string("g12light_hit");
 *o<< string("g13X_hit");
 *o<< string("g13Y_hit");
 *o<< string("g13Z_hit");
 *o<< string("g13ToF_hit");
 *o<< string("g13Edep_hit");
 *o<< string("g13light_hit");
 *o<< string("g14X_hit");
 *o<< string("g14Y_hit");
 *o<< string("g14Z_hit");
 *o<< string("g14ToF_hit");
 *o<< string("g14Edep_hit");
 *o<< string("g14light_hit");
 *o<< string("g15X_hit");
 *o<< string("g15Y_hit");
 *o<< string("g15Z_hit");
 *o<< string("g15ToF_hit");
 *o<< string("g15Edep_hit");
 *o<< string("g15light_hit");
 *o<< string("g16X_hit");
 *o<< string("g16Y_hit");
 *o<< string("g16Z_hit");
 *o<< string("g16ToF_hit");
 *o<< string("g16Edep_hit");
 *o<< string("g16light_hit");
 *o<< string("g17X_hit");
 *o<< string("g17Y_hit");
 *o<< string("g17Z_hit");
 *o<< string("g17ToF_hit");
 *o<< string("g17Edep_hit");
 *o<< string("g17light_hit");
 *o<< string("g18X_hit");
 *o<< string("g18Y_hit");
 *o<< string("g18Z_hit");
 *o<< string("g18ToF_hit");
 *o<< string("g18Edep_hit");
 *o<< string("g18light_hit");
 *o<< string("g19X_hit");
 *o<< string("g19Y_hit");
 *o<< string("g19Z_hit");
 *o<< string("g19ToF_hit");
 *o<< string("g19Edep_hit");
 *o<< string("g19light_hit");
 *o<< string("g20X_hit");
 *o<< string("g20Y_hit");
 *o<< string("g20Z_hit");
 *o<< string("g20ToF_hit");
 *o<< string("g20Edep_hit");
 *o<< string("g20light_hit");
 *o<< string("frag_xfp");
 *o<< string("frag_afp");
 *o<< string("frag_yfp");
 *o<< string("frag_bfp");
 *o<< string("frag_time");
 *o<< string("target_x");
}

void CalMonaEvent::writeFilter(CXdrOutputStream* o) {
  //     float fake =111;

  // Require enough space in 8192 buffer for entire "event" sequence
  size_t intsize = o->sizeofInt(); // i.e. int (0x3FF)
  size_t floatsize = o->sizeofFloat(); //  i.e. float(getMonaX())
  size_t headsize = o->sizeofString("event"); // i.e. string("event")
  size_t evtsize = 254*floatsize + 3*intsize + headsize;
  o->Require(evtsize);
  // If Require is not meet it closes buffer and open new one.
  // If Require is meet is continues to write to current buffer.

 *o<<string("event");
 *o<< int (0xffffffff);
 *o<< int (0xffffffff);
 *o<< int (0xffffffff);
 *o<< int (0xffffffff);
 *o<< int (0xffffffff);
 *o<< int (0xffffffff);
 *o<< int (0xffffffff);
 *o<< int (0xffffffff); 
 *o<< float(getMonaX());
 *o<< float(getMonaY());
 *o<< float(getMonaZ());
 *o<< float(getMonaTime());
 *o<< float(getMonaX2());
 *o<< float(getMonaY2());
 *o<< float(getMonaZ2());
 *o<< float(getMonaTime3());
 *o<< float(getMonaX3());
 *o<< float(getMonaY3());
 *o<< float(getMonaZ3());
 *o<< float(getMonaTime4());
 *o<< float(getMonaX4());
 *o<< float(getMonaY4());
 *o<< float(getMonaZ4());
 *o<< int(getMonaN1GMultiplicity());// Start1N
 *o<< float(getMonaN1G1X());
 *o<< float(getMonaN1G1Y());
 *o<< float(getMonaN1G1Z());
 *o<< float(getMonaN1G1Time());
 *o<< float(getMonaN1G1Edep());
 *o<< float(getMonaN1G1light());
 *o<< float(getMonaN1G2X());
 *o<< float(getMonaN1G2Y());
 *o<< float(getMonaN1G2Z());
 *o<< float(getMonaN1G2Time());
 *o<< float(getMonaN1G2Edep());
 *o<< float(getMonaN1G2light());
 *o<< float(getMonaN1G3X());
 *o<< float(getMonaN1G3Y());
 *o<< float(getMonaN1G3Z());
 *o<< float(getMonaN1G3Time());
 *o<< float(getMonaN1G3Edep());
 *o<< float(getMonaN1G3light());
 *o<< float(getMonaN1G4X());
 *o<< float(getMonaN1G4Y());
 *o<< float(getMonaN1G4Z());
 *o<< float(getMonaN1G4Time());
 *o<< float(getMonaN1G4Edep());
 *o<< float(getMonaN1G4light());
 *o<< float(getMonaN1G5X());
 *o<< float(getMonaN1G5Y());
 *o<< float(getMonaN1G5Z());
 *o<< float(getMonaN1G5Time());
 *o<< float(getMonaN1G5Edep());
 *o<< float(getMonaN1G5light());
 *o<< float(getMonaN1G6X());
 *o<< float(getMonaN1G6Y());
 *o<< float(getMonaN1G6Z());
 *o<< float(getMonaN1G6Time());
 *o<< float(getMonaN1G6Edep());
 *o<< float(getMonaN1G6light());
 *o<< float(getMonaN1G7X());
 *o<< float(getMonaN1G7Y());
 *o<< float(getMonaN1G7Z());
 *o<< float(getMonaN1G7Time());
 *o<< float(getMonaN1G7Edep());
 *o<< float(getMonaN1G7light());
 *o<< float(getMonaN1G8X());
 *o<< float(getMonaN1G8Y());
 *o<< float(getMonaN1G8Z());
 *o<< float(getMonaN1G8Time());
 *o<< float(getMonaN1G8Edep());
 *o<< float(getMonaN1G8light());
 *o<< float(getMonaN1G9X());
 *o<< float(getMonaN1G9Y());
 *o<< float(getMonaN1G9Z());
 *o<< float(getMonaN1G9Time());
 *o<< float(getMonaN1G9Edep());
 *o<< float(getMonaN1G9light());
 *o<< float(getMonaN1G10X());
 *o<< float(getMonaN1G10Y());
 *o<< float(getMonaN1G10Z());
 *o<< float(getMonaN1G10Time());
 *o<< float(getMonaN1G10Edep());
 *o<< float(getMonaN1G10light());// End1N
 *o<< int(getMonaN2GMultiplicity());// Start2N
 *o<< float(getMonaN2G1X());
 *o<< float(getMonaN2G1Y());
 *o<< float(getMonaN2G1Z());
 *o<< float(getMonaN2G1Time());
 *o<< float(getMonaN2G1Edep());
 *o<< float(getMonaN2G1light());
 *o<< float(getMonaN2G2X());
 *o<< float(getMonaN2G2Y());
 *o<< float(getMonaN2G2Z());
 *o<< float(getMonaN2G2Time());
 *o<< float(getMonaN2G2Edep());
 *o<< float(getMonaN2G2light());
 *o<< float(getMonaN2G3X());
 *o<< float(getMonaN2G3Y());
 *o<< float(getMonaN2G3Z());
 *o<< float(getMonaN2G3Time());
 *o<< float(getMonaN2G3Edep());
 *o<< float(getMonaN2G3light());
 *o<< float(getMonaN2G4X());
 *o<< float(getMonaN2G4Y());
 *o<< float(getMonaN2G4Z());
 *o<< float(getMonaN2G4Time());
 *o<< float(getMonaN2G4Edep());
 *o<< float(getMonaN2G4light());
 *o<< float(getMonaN2G5X());
 *o<< float(getMonaN2G5Y());
 *o<< float(getMonaN2G5Z());
 *o<< float(getMonaN2G5Time());
 *o<< float(getMonaN2G5Edep());
 *o<< float(getMonaN2G5light());
 *o<< float(getMonaN2G6X());
 *o<< float(getMonaN2G6Y());
 *o<< float(getMonaN2G6Z());
 *o<< float(getMonaN2G6Time());
 *o<< float(getMonaN2G6Edep());
 *o<< float(getMonaN2G6light());
 *o<< float(getMonaN2G7X());
 *o<< float(getMonaN2G7Y());
 *o<< float(getMonaN2G7Z());
 *o<< float(getMonaN2G7Time());
 *o<< float(getMonaN2G7Edep());
 *o<< float(getMonaN2G7light());
 *o<< float(getMonaN2G8X());
 *o<< float(getMonaN2G8Y());
 *o<< float(getMonaN2G8Z());
 *o<< float(getMonaN2G8Time());
 *o<< float(getMonaN2G8Edep());
 *o<< float(getMonaN2G8light());
 *o<< float(getMonaN2G9X());
 *o<< float(getMonaN2G9Y());
 *o<< float(getMonaN2G9Z());
 *o<< float(getMonaN2G9Time());
 *o<< float(getMonaN2G9Edep());
 *o<< float(getMonaN2G9light());
 *o<< float(getMonaN2G10X());
 *o<< float(getMonaN2G10Y());
 *o<< float(getMonaN2G10Z());
 *o<< float(getMonaN2G10Time());
 *o<< float(getMonaN2G10Edep());
 *o<< float(getMonaN2G10light());// End 2N
 *o<< int(getMonaN3GMultiplicity());// Start3N
 *o<< float(getMonaN3G1X());
 *o<< float(getMonaN3G1Y());
 *o<< float(getMonaN3G1Z());
 *o<< float(getMonaN3G1Time());
 *o<< float(getMonaN3G1Edep());
 *o<< float(getMonaN3G1light());
 *o<< float(getMonaN3G2X());
 *o<< float(getMonaN3G2Y());
 *o<< float(getMonaN3G2Z());
 *o<< float(getMonaN3G2Time());
 *o<< float(getMonaN3G2Edep());
 *o<< float(getMonaN3G2light());
 *o<< float(getMonaN3G3X());
 *o<< float(getMonaN3G3Y());
 *o<< float(getMonaN3G3Z());
 *o<< float(getMonaN3G3Time());
 *o<< float(getMonaN3G3Edep());
 *o<< float(getMonaN3G3light());
 *o<< float(getMonaN3G4X());
 *o<< float(getMonaN3G4Y());
 *o<< float(getMonaN3G4Z());
 *o<< float(getMonaN3G4Time());
 *o<< float(getMonaN3G4Edep());
 *o<< float(getMonaN3G4light());
 *o<< float(getMonaN3G5X());
 *o<< float(getMonaN3G5Y());
 *o<< float(getMonaN3G5Z());
 *o<< float(getMonaN3G5Time());
 *o<< float(getMonaN3G5Edep());
 *o<< float(getMonaN3G5light());
 *o<< float(getMonaN3G6X());
 *o<< float(getMonaN3G6Y());
 *o<< float(getMonaN3G6Z());
 *o<< float(getMonaN3G6Time());
 *o<< float(getMonaN3G6Edep());
 *o<< float(getMonaN3G6light());
 *o<< float(getMonaN3G7X());
 *o<< float(getMonaN3G7Y());
 *o<< float(getMonaN3G7Z());
 *o<< float(getMonaN3G7Time());
 *o<< float(getMonaN3G7Edep());
 *o<< float(getMonaN3G7light());
 *o<< float(getMonaN3G8X());
 *o<< float(getMonaN3G8Y());
 *o<< float(getMonaN3G8Z());
 *o<< float(getMonaN3G8Time());
 *o<< float(getMonaN3G8Edep());
 *o<< float(getMonaN3G8light());
 *o<< float(getMonaN3G9X());
 *o<< float(getMonaN3G9Y());
 *o<< float(getMonaN3G9Z());
 *o<< float(getMonaN3G9Time());
 *o<< float(getMonaN3G9Edep());
 *o<< float(getMonaN3G9light());
 *o<< float(getMonaN3G10X());
 *o<< float(getMonaN3G10Y());
 *o<< float(getMonaN3G10Z());
 *o<< float(getMonaN3G10Time());
 *o<< float(getMonaN3G10Edep());
 *o<< float(getMonaN3G10light());// End 3N

 *o<< int(getMonaN4GMultiplicity());// Start4N
 *o<< float(getMonaN4G1X());
 *o<< float(getMonaN4G1Y());
 *o<< float(getMonaN4G1Z());
 *o<< float(getMonaN4G1Time());
 *o<< float(getMonaN4G1Edep());
 *o<< float(getMonaN4G1light());
 *o<< float(getMonaN4G2X());
 *o<< float(getMonaN4G2Y());
 *o<< float(getMonaN4G2Z());
 *o<< float(getMonaN4G2Time());
 *o<< float(getMonaN4G2Edep());
 *o<< float(getMonaN4G2light());
 *o<< float(getMonaN4G3X());
 *o<< float(getMonaN4G3Y());
 *o<< float(getMonaN4G3Z());
 *o<< float(getMonaN4G3Time());
 *o<< float(getMonaN4G3Edep());
 *o<< float(getMonaN4G3light());
 *o<< float(getMonaN4G4X());
 *o<< float(getMonaN4G4Y());
 *o<< float(getMonaN4G4Z());
 *o<< float(getMonaN4G4Time());
 *o<< float(getMonaN4G4Edep());
 *o<< float(getMonaN4G4light());
 *o<< float(getMonaN4G5X());
 *o<< float(getMonaN4G5Y());
 *o<< float(getMonaN4G5Z());
 *o<< float(getMonaN4G5Time());
 *o<< float(getMonaN4G5Edep());
 *o<< float(getMonaN4G5light());
 *o<< float(getMonaN4G6X());
 *o<< float(getMonaN4G6Y());
 *o<< float(getMonaN4G6Z());
 *o<< float(getMonaN4G6Time());
 *o<< float(getMonaN4G6Edep());
 *o<< float(getMonaN4G6light());
 *o<< float(getMonaN4G7X());
 *o<< float(getMonaN4G7Y());
 *o<< float(getMonaN4G7Z());
 *o<< float(getMonaN4G7Time());
 *o<< float(getMonaN4G7Edep());
 *o<< float(getMonaN4G7light());
 *o<< float(getMonaN4G8X());
 *o<< float(getMonaN4G8Y());
 *o<< float(getMonaN4G8Z());
 *o<< float(getMonaN4G8Time());
 *o<< float(getMonaN4G8Edep());
 *o<< float(getMonaN4G8light());
 *o<< float(getMonaN4G9X());
 *o<< float(getMonaN4G9Y());
 *o<< float(getMonaN4G9Z());
 *o<< float(getMonaN4G9Time());
 *o<< float(getMonaN4G9Edep());
 *o<< float(getMonaN4G9light());
 *o<< float(getMonaN4G10X());
 *o<< float(getMonaN4G10Y());
 *o<< float(getMonaN4G10Z());
 *o<< float(getMonaN4G10Time());
 *o<< float(getMonaN4G10Edep());
 *o<< float(getMonaN4G10light());// End 4N

 *o<< float(getMonaG1X());
 *o<< float(getMonaG1Y());
 *o<< float(getMonaG1Z());
 *o<< float(getMonaG1Time());
 *o<< float(getMonaG1Edep());
 *o<< float(getMonaG1light());
 *o<< float(getMonaG2X());
 *o<< float(getMonaG2Y());
 *o<< float(getMonaG2Z());
 *o<< float(getMonaG2Time());
 *o<< float(getMonaG2Edep());
 *o<< float(getMonaG2light());
 *o<< float(getMonaG3X());
 *o<< float(getMonaG3Y());
 *o<< float(getMonaG3Z());
 *o<< float(getMonaG3Time());
 *o<< float(getMonaG3Edep());
 *o<< float(getMonaG3light());
 *o<< float(getMonaG4X());
 *o<< float(getMonaG4Y());
 *o<< float(getMonaG4Z());
 *o<< float(getMonaG4Time());
 *o<< float(getMonaG4Edep());
 *o<< float(getMonaG4light());
 *o<< float(getMonaG5X());
 *o<< float(getMonaG5Y());
 *o<< float(getMonaG5Z());
 *o<< float(getMonaG5Time());
 *o<< float(getMonaG5Edep());
 *o<< float(getMonaG5light());
 *o<< float(getMonaG6X());
 *o<< float(getMonaG6Y());
 *o<< float(getMonaG6Z());
 *o<< float(getMonaG6Time());
 *o<< float(getMonaG6Edep());
 *o<< float(getMonaG6light());
 *o<< float(getMonaG7X());
 *o<< float(getMonaG7Y());
 *o<< float(getMonaG7Z());
 *o<< float(getMonaG7Time());
 *o<< float(getMonaG7Edep());
 *o<< float(getMonaG7light());
 *o<< float(getMonaG8X());
 *o<< float(getMonaG8Y());
 *o<< float(getMonaG8Z());
 *o<< float(getMonaG8Time());
 *o<< float(getMonaG8Edep());
 *o<< float(getMonaG8light());
 *o<< float(getMonaG9X());
 *o<< float(getMonaG9Y());
 *o<< float(getMonaG9Z());
 *o<< float(getMonaG9Time());
 *o<< float(getMonaG9Edep());
 *o<< float(getMonaG9light());
 *o<< float(getMonaG10X());
 *o<< float(getMonaG10Y());
 *o<< float(getMonaG10Z());
 *o<< float(getMonaG10Time());
 *o<< float(getMonaG10Edep());
 *o<< float(getMonaG10light());
 *o<< float(getMonaG11X());
 *o<< float(getMonaG11Y());
 *o<< float(getMonaG11Z());
 *o<< float(getMonaG11Time());
 *o<< float(getMonaG11Edep());
 *o<< float(getMonaG11light());
 *o<< float(getMonaG12X());
 *o<< float(getMonaG12Y());
 *o<< float(getMonaG12Z());
 *o<< float(getMonaG12Time());
 *o<< float(getMonaG12Edep());
 *o<< float(getMonaG12light());
 *o<< float(getMonaG13X());
 *o<< float(getMonaG13Y());
 *o<< float(getMonaG13Z());
 *o<< float(getMonaG13Time());
 *o<< float(getMonaG13Edep());
 *o<< float(getMonaG13light());
 *o<< float(getMonaG14X());
 *o<< float(getMonaG14Y());
 *o<< float(getMonaG14Z());
 *o<< float(getMonaG14Time());
 *o<< float(getMonaG14Edep());
 *o<< float(getMonaG14light());
 *o<< float(getMonaG15X());
 *o<< float(getMonaG15Y());
 *o<< float(getMonaG15Z());
 *o<< float(getMonaG15Time());
 *o<< float(getMonaG15Edep());
 *o<< float(getMonaG15light());
 *o<< float(getMonaG16X());
 *o<< float(getMonaG16Y());
 *o<< float(getMonaG16Z());
 *o<< float(getMonaG16Time());
 *o<< float(getMonaG16Edep());
 *o<< float(getMonaG16light());
 *o<< float(getMonaG17X());
 *o<< float(getMonaG17Y());
 *o<< float(getMonaG17Z());
 *o<< float(getMonaG17Time());
 *o<< float(getMonaG17Edep());
 *o<< float(getMonaG17light());
 *o<< float(getMonaG18X());
 *o<< float(getMonaG18Y());
 *o<< float(getMonaG18Z());
 *o<< float(getMonaG18Time());
 *o<< float(getMonaG18Edep());
 *o<< float(getMonaG18light());
 *o<< float(getMonaG19X());
 *o<< float(getMonaG19Y());
 *o<< float(getMonaG19Z());
 *o<< float(getMonaG19Time());
 *o<< float(getMonaG19Edep());
 *o<< float(getMonaG19light());
 *o<< float(getMonaG20X());
 *o<< float(getMonaG20Y());
 *o<< float(getMonaG20Z());
 *o<< float(getMonaG20Time());
 *o<< float(getMonaG20Edep());
 *o<< float(getMonaG20light());
 *o<< float(getFragX());
 *o<< float(getFragTX());
 *o<< float(getFragY());
 *o<< float(getFragTY());
 *o<< float(getFragTime());
 *o<< float(getTargetX());
 
 /*
 *o<< fake;
 *o<< fake;
 *o<< fake;
 *o<< fake;
 *o<< fake;
 *o<< fake;
 *o<< fake;
 *o<< fake;
 *o<< fake;
 *o<< fake;
 */

}







int CalMonaEventSim::getEvent(long n) {
  int nb; // number of bytes read (returned by GetEntry())
  double zero = 0;
  DSV(fEventNum);

  if ( n < 0 ) n = fEventNum++;
  DSV(fT);
  DSV(n);
  if (!fT) ERR("Tree pointer not set!!!");
  nb = fT -> GetEntry(n);
  if (!nb) return 0;

  DSV(n);
  DSV(nb);
  DSV(mx);
  DSV(mt);
  DSV(mEkin);
  DSV(m2x);
  DSV(m2t);
  DSV(m2Ekin);

  if (mEkin > 0) {
  	setMonaX(mx);
  	setMonaY(my);
  	setMonaZ(mz);
  	setMonaTime(mt);
  	} else {
  	setMonaX(0./zero);
  	setMonaY(0./zero);
  	setMonaZ(0./zero);
  	setMonaTime(0./zero);
	}

  if (m2Ekin > 0) {
  	setMonaX2(m2x);
  	setMonaY2(m2y);
  	setMonaZ2(m2z);
  	setMonaTime2(m2t);
	} else {
  	setMonaX2(0./zero);
  	setMonaY2(0./zero);
  	setMonaZ2(0./zero);
	setMonaTime2(0./zero);
	}
  if (m3Ekin > 0) {
  	setMonaX3(m3x);
  	setMonaY3(m3y);
  	setMonaZ3(m3z);
  	setMonaTime3(m3t);
	} else {
  	setMonaX3(0./zero);
  	setMonaY3(0./zero);
  	setMonaZ3(0./zero);
	setMonaTime3(0./zero);
	}
  if (m4Ekin > 0) {
  	setMonaX4(m4x);
  	setMonaY4(m4y);
  	setMonaZ4(m4z);
  	setMonaTime4(m4t);
	} else {
  	setMonaX4(0./zero);
  	setMonaY4(0./zero);
  	setMonaZ4(0./zero);
	setMonaTime4(0./zero);
	}

  // Start N1
  setMonaN1GMultiplicity(mN1GM);
  setMonaN1G1X(mN1G1x);
  setMonaN1G1Y(mN1G1y);
  setMonaN1G1Z(mN1G1z);
  setMonaN1G1Time(mN1G1t);
  setMonaN1G1Edep(mN1G1Edep);
  setMonaN1G1light(mN1G1light);
  setMonaN1G2X(mN1G2x);
  setMonaN1G2Y(mN1G2y);
  setMonaN1G2Z(mN1G2z);
  setMonaN1G2Time(mN1G2t);
  setMonaN1G2Edep(mN1G2Edep);
  setMonaN1G2light(mN1G2light);
  setMonaN1G3X(mN1G3x);
  setMonaN1G3Y(mN1G3y);
  setMonaN1G3Z(mN1G3z);
  setMonaN1G3Time(mN1G3t);
  setMonaN1G3Edep(mN1G3Edep);
  setMonaN1G3light(mN1G3light);
  setMonaN1G4X(mN1G4x);
  setMonaN1G4Y(mN1G4y);
  setMonaN1G4Z(mN1G4z);
  setMonaN1G4Time(mN1G4t);
  setMonaN1G4Edep(mN1G4Edep);
  setMonaN1G4light(mN1G4light);
  setMonaN1G5X(mN1G5x);
  setMonaN1G5Y(mN1G5y);
  setMonaN1G5Z(mN1G5z);
  setMonaN1G5Time(mN1G5t);
  setMonaN1G5Edep(mN1G5Edep);
  setMonaN1G5light(mN1G5light);
  setMonaN1G6X(mN1G6x);
  setMonaN1G6Y(mN1G6y);
  setMonaN1G6Z(mN1G6z);
  setMonaN1G6Time(mN1G6t);
  setMonaN1G6Edep(mN1G6Edep);
  setMonaN1G6light(mN1G6light);
  setMonaN1G7X(mN1G7x);
  setMonaN1G7Y(mN1G7y);
  setMonaN1G7Z(mN1G7z);
  setMonaN1G7Time(mN1G7t);
  setMonaN1G7Edep(mN1G7Edep);
  setMonaN1G7light(mN1G7light);
  setMonaN1G8X(mN1G8x);
  setMonaN1G8Y(mN1G8y);
  setMonaN1G8Z(mN1G8z);
  setMonaN1G8Time(mN1G8t);
  setMonaN1G8Edep(mN1G8Edep);
  setMonaN1G8light(mN1G8light);
  setMonaN1G9X(mN1G9x);
  setMonaN1G9Y(mN1G9y);
  setMonaN1G9Z(mN1G9z);
  setMonaN1G9Time(mN1G9t);
  setMonaN1G9Edep(mN1G9Edep);
  setMonaN1G9light(mN1G9light);
  setMonaN1G10X(mN1G10x);
  setMonaN1G10Y(mN1G10y);
  setMonaN1G10Z(mN1G10z);
  setMonaN1G10Time(mN1G10t);
  setMonaN1G10Edep(mN1G10Edep);
  setMonaN1G10light(mN1G10light);
  // End N1
  // Start N2
  setMonaN2GMultiplicity(mN2GM);
  setMonaN2G1X(mN2G1x);
  setMonaN2G1Y(mN2G1y);
  setMonaN2G1Z(mN2G1z);
  setMonaN2G1Time(mN2G1t);
  setMonaN2G1Edep(mN2G1Edep);
  setMonaN2G1light(mN2G1light);
  setMonaN2G2X(mN2G2x);
  setMonaN2G2Y(mN2G2y);
  setMonaN2G2Z(mN2G2z);
  setMonaN2G2Time(mN2G2t);
  setMonaN2G2Edep(mN2G2Edep);
  setMonaN2G2light(mN2G2light);
  setMonaN2G3X(mN2G3x);
  setMonaN2G3Y(mN2G3y);
  setMonaN2G3Z(mN2G3z);
  setMonaN2G3Time(mN2G3t);
  setMonaN2G3Edep(mN2G3Edep);
  setMonaN2G3light(mN2G3light);
  setMonaN2G4X(mN2G4x);
  setMonaN2G4Y(mN2G4y);
  setMonaN2G4Z(mN2G4z);
  setMonaN2G4Time(mN2G4t);
  setMonaN2G4Edep(mN2G4Edep);
  setMonaN2G4light(mN2G4light);
  setMonaN2G5X(mN2G5x);
  setMonaN2G5Y(mN2G5y);
  setMonaN2G5Z(mN2G5z);
  setMonaN2G5Time(mN2G5t);
  setMonaN2G5Edep(mN2G5Edep);
  setMonaN2G5light(mN2G5light);
  setMonaN2G6X(mN2G6x);
  setMonaN2G6Y(mN2G6y);
  setMonaN2G6Z(mN2G6z);
  setMonaN2G6Time(mN2G6t);
  setMonaN2G6Edep(mN2G6Edep);
  setMonaN2G6light(mN2G6light);
  setMonaN2G7X(mN2G7x);
  setMonaN2G7Y(mN2G7y);
  setMonaN2G7Z(mN2G7z);
  setMonaN2G7Time(mN2G7t);
  setMonaN2G7Edep(mN2G7Edep);
  setMonaN2G7light(mN2G7light);
  setMonaN2G8X(mN2G8x);
  setMonaN2G8Y(mN2G8y);
  setMonaN2G8Z(mN2G8z);
  setMonaN2G8Time(mN2G8t);
  setMonaN2G8Edep(mN2G8Edep);
  setMonaN2G8light(mN2G8light);
  setMonaN2G9X(mN2G9x);
  setMonaN2G9Y(mN2G9y);
  setMonaN2G9Z(mN2G9z);
  setMonaN2G9Time(mN2G9t);
  setMonaN2G9Edep(mN2G9Edep);
  setMonaN2G9light(mN2G9light);
  setMonaN2G10X(mN2G10x);
  setMonaN2G10Y(mN2G10y);
  setMonaN2G10Z(mN2G10z);
  setMonaN2G10Time(mN2G10t);
  setMonaN2G10Edep(mN2G10Edep);
  setMonaN2G10light(mN2G10light);
  // End N2
 // Start N3
  setMonaN3GMultiplicity(mN3GM);
  setMonaN3G1X(mN3G1x);
  setMonaN3G1Y(mN3G1y);
  setMonaN3G1Z(mN3G1z);
  setMonaN3G1Time(mN3G1t);
  setMonaN3G1Edep(mN3G1Edep);
  setMonaN3G1light(mN3G1light);
  setMonaN3G2X(mN3G2x);
  setMonaN3G2Y(mN3G2y);
  setMonaN3G2Z(mN3G2z);
  setMonaN3G2Time(mN3G2t);
  setMonaN3G2Edep(mN3G2Edep);
  setMonaN3G2light(mN3G2light);
  setMonaN3G3X(mN3G3x);
  setMonaN3G3Y(mN3G3y);
  setMonaN3G3Z(mN3G3z);
  setMonaN3G3Time(mN3G3t);
  setMonaN3G3Edep(mN3G3Edep);
  setMonaN3G3light(mN3G3light);
  setMonaN3G4X(mN3G4x);
  setMonaN3G4Y(mN3G4y);
  setMonaN3G4Z(mN3G4z);
  setMonaN3G4Time(mN3G4t);
  setMonaN3G4Edep(mN3G4Edep);
  setMonaN3G4light(mN3G4light);
  setMonaN3G5X(mN3G5x);
  setMonaN3G5Y(mN3G5y);
  setMonaN3G5Z(mN3G5z);
  setMonaN3G5Time(mN3G5t);
  setMonaN3G5Edep(mN3G5Edep);
  setMonaN3G5light(mN3G5light);
  setMonaN3G6X(mN3G6x);
  setMonaN3G6Y(mN3G6y);
  setMonaN3G6Z(mN3G6z);
  setMonaN3G6Time(mN3G6t);
  setMonaN3G6Edep(mN3G6Edep);
  setMonaN3G6light(mN3G6light);
  setMonaN3G7X(mN3G7x);
  setMonaN3G7Y(mN3G7y);
  setMonaN3G7Z(mN3G7z);
  setMonaN3G7Time(mN3G7t);
  setMonaN3G7Edep(mN3G7Edep);
  setMonaN3G7light(mN3G7light);
  setMonaN3G8X(mN3G8x);
  setMonaN3G8Y(mN3G8y);
  setMonaN3G8Z(mN3G8z);
  setMonaN3G8Time(mN3G8t);
  setMonaN3G8Edep(mN3G8Edep);
  setMonaN3G8light(mN3G8light);
  setMonaN3G9X(mN3G9x);
  setMonaN3G9Y(mN3G9y);
  setMonaN3G9Z(mN3G9z);
  setMonaN3G9Time(mN3G9t);
  setMonaN3G9Edep(mN3G9Edep);
  setMonaN3G9light(mN3G9light);
  setMonaN3G10X(mN3G10x);
  setMonaN3G10Y(mN3G10y);
  setMonaN3G10Z(mN3G10z);
  setMonaN3G10Time(mN3G10t);
  setMonaN3G10Edep(mN3G10Edep);
  setMonaN3G10light(mN3G10light);
  // End N3
 // Start N4
  setMonaN4GMultiplicity(mN4GM);
  setMonaN4G1X(mN4G1x);
  setMonaN4G1Y(mN4G1y);
  setMonaN4G1Z(mN4G1z);
  setMonaN4G1Time(mN4G1t);
  setMonaN4G1Edep(mN4G1Edep);
  setMonaN4G1light(mN4G1light);
  setMonaN4G2X(mN4G2x);
  setMonaN4G2Y(mN4G2y);
  setMonaN4G2Z(mN4G2z);
  setMonaN4G2Time(mN4G2t);
  setMonaN4G2Edep(mN4G2Edep);
  setMonaN4G2light(mN4G2light);
  setMonaN4G3X(mN4G3x);
  setMonaN4G3Y(mN4G3y);
  setMonaN4G3Z(mN4G3z);
  setMonaN4G3Time(mN4G3t);
  setMonaN4G3Edep(mN4G3Edep);
  setMonaN4G3light(mN4G3light);
  setMonaN4G4X(mN4G4x);
  setMonaN4G4Y(mN4G4y);
  setMonaN4G4Z(mN4G4z);
  setMonaN4G4Time(mN4G4t);
  setMonaN4G4Edep(mN4G4Edep);
  setMonaN4G4light(mN4G4light);
  setMonaN4G5X(mN4G5x);
  setMonaN4G5Y(mN4G5y);
  setMonaN4G5Z(mN4G5z);
  setMonaN4G5Time(mN4G5t);
  setMonaN4G5Edep(mN4G5Edep);
  setMonaN4G5light(mN4G5light);
  setMonaN4G6X(mN4G6x);
  setMonaN4G6Y(mN4G6y);
  setMonaN4G6Z(mN4G6z);
  setMonaN4G6Time(mN4G6t);
  setMonaN4G6Edep(mN4G6Edep);
  setMonaN4G6light(mN4G6light);
  setMonaN4G7X(mN4G7x);
  setMonaN4G7Y(mN4G7y);
  setMonaN4G7Z(mN4G7z);
  setMonaN4G7Time(mN4G7t);
  setMonaN4G7Edep(mN4G7Edep);
  setMonaN4G7light(mN4G7light);
  setMonaN4G8X(mN4G8x);
  setMonaN4G8Y(mN4G8y);
  setMonaN4G8Z(mN4G8z);
  setMonaN4G8Time(mN4G8t);
  setMonaN4G8Edep(mN4G8Edep);
  setMonaN4G8light(mN4G8light);
  setMonaN4G9X(mN4G9x);
  setMonaN4G9Y(mN4G9y);
  setMonaN4G9Z(mN4G9z);
  setMonaN4G9Time(mN4G9t);
  setMonaN4G9Edep(mN4G9Edep);
  setMonaN4G9light(mN4G9light);
  setMonaN4G10X(mN4G10x);
  setMonaN4G10Y(mN4G10y);
  setMonaN4G10Z(mN4G10z);
  setMonaN4G10Time(mN4G10t);
  setMonaN4G10Edep(mN4G10Edep);
  setMonaN4G10light(mN4G10light);
  // End N4


  setMonaG1X(mG1x);
  setMonaG1Y(mG1y);
  setMonaG1Z(mG1z);
  setMonaG1Time(mG1t);
  setMonaG1Edep(mG1Edep);
  setMonaG1light(mG1light);
  setMonaG2X(mG2x);
  setMonaG2Y(mG2y);
  setMonaG2Z(mG2z);
  setMonaG2Time(mG2t);
  setMonaG2Edep(mG2Edep);
  setMonaG2light(mG2light);
  setMonaG3X(mG3x);
  setMonaG3Y(mG3y);
  setMonaG3Z(mG3z);
  setMonaG3Time(mG3t);
  setMonaG3Edep(mG3Edep);
  setMonaG3light(mG3light);
  setMonaG4X(mG4x);
  setMonaG4Y(mG4y);
  setMonaG4Z(mG4z);
  setMonaG4Time(mG4t);
  setMonaG4Edep(mG4Edep);
  setMonaG4light(mG4light);
  setMonaG5X(mG5x);
  setMonaG5Y(mG5y);
  setMonaG5Z(mG5z);
  setMonaG5Time(mG5t);
  setMonaG5Edep(mG5Edep);
  setMonaG5light(mG5light);
  setMonaG6X(mG6x);
  setMonaG6Y(mG6y);
  setMonaG6Z(mG6z);
  setMonaG6Time(mG6t);
  setMonaG6Edep(mG6Edep);
  setMonaG6light(mG6light);
  setMonaG7X(mG7x);
  setMonaG7Y(mG7y);
  setMonaG7Z(mG7z);
  setMonaG7Time(mG7t);
  setMonaG7Edep(mG7Edep);
  setMonaG7light(mG7light);
  setMonaG8X(mG8x);
  setMonaG8Y(mG8y);
  setMonaG8Z(mG8z);
  setMonaG8Time(mG8t);
  setMonaG8Edep(mG8Edep);
  setMonaG8light(mG8light);
  setMonaG9X(mG9x);
  setMonaG9Y(mG9y);
  setMonaG9Z(mG9z);
  setMonaG9Time(mG9t);
  setMonaG9Edep(mG9Edep);
  setMonaG9light(mG9light);
  setMonaG10X(mG10x);
  setMonaG10Y(mG10y);
  setMonaG10Z(mG10z);
  setMonaG10Time(mG10t);
  setMonaG10Edep(mG10Edep);
  setMonaG10light(mG10light);
  setMonaG11X(mG11x);
  setMonaG11Y(mG11y);
  setMonaG11Z(mG11z);
  setMonaG11Time(mG11t);
  setMonaG11Edep(mG11Edep);
  setMonaG11light(mG11light);
  setMonaG12X(mG12x);
  setMonaG12Y(mG12y);
  setMonaG12Z(mG12z);
  setMonaG12Time(mG12t);
  setMonaG12Edep(mG12Edep);
  setMonaG12light(mG12light);
  setMonaG13X(mG13x);
  setMonaG13Y(mG13y);
  setMonaG13Z(mG13z);
  setMonaG13Time(mG13t);
  setMonaG13Edep(mG13Edep);
  setMonaG13light(mG13light);
  setMonaG14X(mG14x);
  setMonaG14Y(mG14y);
  setMonaG14Z(mG14z);
  setMonaG14Time(mG14t);
  setMonaG14Edep(mG14Edep);
  setMonaG14light(mG14light);
  setMonaG15X(mG15x);
  setMonaG15Y(mG15y);
  setMonaG15Z(mG15z);
  setMonaG15Time(mG15t);
  setMonaG15Edep(mG15Edep);
  setMonaG15light(mG15light);
  setMonaG16X(mG16x);
  setMonaG16Y(mG16y);
  setMonaG16Z(mG16z);
  setMonaG16Time(mG16t);
  setMonaG16Edep(mG16Edep);
  setMonaG16light(mG16light);
  setMonaG17X(mG17x);
  setMonaG17Y(mG17y);
  setMonaG17Z(mG17z);
  setMonaG17Time(mG17t);
  setMonaG17Edep(mG17Edep);
  setMonaG17light(mG17light);
  setMonaG18X(mG18x);
  setMonaG18Y(mG18y);
  setMonaG18Z(mG18z);
  setMonaG18Time(mG18t);
  setMonaG18Edep(mG18Edep);
  setMonaG18light(mG18light);
  setMonaG19X(mG19x);
  setMonaG19Y(mG19y);
  setMonaG19Z(mG19z);
  setMonaG19Time(mG19t);
  setMonaG19Edep(mG19Edep);
  setMonaG19light(mG19light);
  setMonaG20X(mG20x);
  setMonaG20Y(mG20y);
  setMonaG20Z(mG20z);
  setMonaG20Time(mG20t);
  setMonaG20Edep(mG20Edep);
  setMonaG20light(mG20light);

  if (fEkin <= 0) { // set values to nan for stopped particles
    	setFragX(0./zero);
    	setFragTX(0./zero);
    	setFragY(0./zero);
    	setFragTY(0./zero);
    	setFragTime(0./zero);
  	} else {
  	setFragX(fx);
  	setFragTX(ftx);
  	setFragY(fy);
  	setFragTY(fty);
  	setFragTime(ft);
  	}
  
  setTargetX(tx);
  setTargetY(ty);
  
  return nb;
}


void MonaAnalyzedEvent::reset() {
  //exen  = -99;
  //exen1 = -99;
  //exen2 = -99;
  EDecay1 = -99;
  EDecay2 = -99;
  EDecayTotal = -99;
  lvN.SetXYZT(0,0,0,0);
  lvN2.SetXYZT(0,0,0,0);
  lvG1N.SetXYZT(0,0,0,0);
  lvG2N.SetXYZT(0,0,0,0);
  lvG3N.SetXYZT(0,0,0,0);
  lvG4N.SetXYZT(0,0,0,0);
  lvG5N.SetXYZT(0,0,0,0);
  lvG6N.SetXYZT(0,0,0,0); 
  lvFCosy.SetXYZT(0,0,0,0);
  lvFSchill.SetXYZT(0,0,0,0);
  lvFFit.SetXYZT(0,0,0,0);
  vTPos.SetXYZ(0,0,0);
  memset(deltaE,0,sizeof(deltaE));
  memset(altExenCosy,0,sizeof(altExenCosy));
  memset(altExenSchill,0,sizeof(altExenSchill));
  memset(altExenFit,0,sizeof(altExenFit));
  tLab = -7;
  tLab2 = -7;
  vRel = -1e99;
  vRel2 = -1e99;
  nVel = -1e99;
  nVel2 = -1e99;
  fVel = -1e99;
  fragKinE = -1e5;
  neutronKinE = -1e99;
  nTheta = -1e5;
  nPhi = -1e5;
  neutronKinE2 = -1e99;
  nTheta2 = -1e5;
  nPhi2 = -1e5;
  fTheta = -1e5;
  fPhi = -1e5;
  fpATA = -1e5;
  fpBTA = -1e5;
  fpYTA = -1e5;
  fAngleX = -1e5;
  fAngleY = -1e5;
  nAngleX = -1e5;
  nAngleY = -1e5;
  nAngleX2 = -1e5;
  nAngleY2 = -1e5;
  delta = -1e5;
  fYfp = -1e5;
  fBfp = -1e5;
  fAfp = -1e5;
  fXfp = -1e5;
}


void MonaAnalyzedEvent::initTarget() {
  cout << "Entering MonaAnalyzedEvent::initTarget()" << endl;

  if (targA <= 1) ERR("Target A is smaller than 2!!! %d",targA);
  if (targZ < 1) ERR("Target Z is smaller than 1!!! %d",targZ);
  if (dTarg <= 0.0) ERR("Target thickness is 0!!! %f",dTarg);

  matTarget= new StMaterial();
 
  StElement eleTarget;
  eleTarget.setZ(targZ);
  eleTarget.setm(targA);   /// \todo need exact mass here


  matTarget->addElement(eleTarget);
  matTarget->setThickness(dTarg);
  //cout << "End of initTarget()" << endl;
}

MonaAnalyzedEvent::MonaAnalyzedEvent():fragA(0),fragQ(0),dTarg(0),targA(0),targZ(0),monaDist(0),matTarget(0),fragMassExcess(0),srimTarget(0), srimFoil(0) {
  //empty because initTarget will create matTarget
}

int MonaAnalyzedEvent::applyInvMap(ParticleCoordinates* c) {
  // using Schiller map
  double co[] = { c->xd, c->txd, c->yd, c->tyd, c->xt };  // input coordinates

  //                txt  yt tyt del  len
  double cop[] = {   0,   0,  0,  0,   0  };  // coordiante array after mapping

  for (size_t i(0); i < fInvMap.size(); i++) {  // loop over map entries

    double f = 1;  // factor
    for (size_t j(0); j < 5 ; j++) {
      f *= gsl_pow_int(co[j],fInvMap.getLine(i).power[j]);
    }
    
    // calculate new coordinates
    for (size_t k(0); k < 5; k++) {
      cop[k] += fInvMap.getLine(i).coef[k] * f;
    }
  }
  c->txt = cop[0];
  c->yt  = cop[1];
  c->tyt = cop[2];
  c->d   = cop[3];
  c->l   = cop[4];
 
  return 0;
}


int MonaAnalyzedEvent::ana(CalMonaEvent* data) {
  //########## Creates the 4-vector for the neutrons ##########//
  //-----> Add the target's position <-----//
  //cout << "ana(CalMonaevent* data) " << endl;
  vTPos.SetX(data->getTargetX()); // target's x-position
  vTPos.SetY(data->getTargetY());
  //vTPos.SetX(0);
  //vTPos.SetY(0);
  vTPos.SetZ(0);
  
  //-----> Add the 1st neutron <-----//
  vNPos.SetX(data->getMonaX());                                // mona x-position
  vNPos.SetY(data->getMonaY());                                // mona y-position
  vNPos.SetZ(data->getMonaZ());                                // mona z-position

  TVector3 vN = vNPos - vTPos;                                 // neutron path travelled vector
  double nVelo = vN.Mag()/data->getMonaTime();                 // the velocity of the neutron

  TVector3 vVeloN = nVelo * vN.Unit();
  TVector3 vBetaN = 1./0.299792458*vVeloN;                     /// \todo use proper constants here!!!

  double gamma = 1./sqrt(1.-(vBetaN.Mag()*vBetaN.Mag()));      // neutron's gamma 

  lvN.SetVectM(vBetaN*gamma*939.565346,939.565346);            // neutron 4-vector

  //-----> Add the 2nd neutron <-----//

  vN2Pos.SetX(data->getMonaX2());                              // mona x-position
  vN2Pos.SetY(data->getMonaY2());                              // mona y-position
  vN2Pos.SetZ(data->getMonaZ2());                              // mona z-position

  TVector3 vN2 = vN2Pos - vTPos;                              // neutron path travelled vector
  double nVelo2 = vN2.Mag()/data->getMonaTime2();              // the velocity of the neutron

  TVector3 vVeloN2 = nVelo2 * vN2.Unit();
  TVector3 vBetaN2 = 1./0.299792458*vVeloN2;                   /// \todo use proper constants here!!!

  double gamma2 = 1./sqrt(1.-(vBetaN2.Mag()*vBetaN2.Mag()));   // neutron's gamma 

  lvN2.SetVectM(vBetaN2*gamma2*939.565346,939.565346);         // neutron 4-vector


  //-----> Add the 3rd neutron <-----//

  vN3Pos.SetX(data->getMonaX3());                              // mona x-position
  vN3Pos.SetY(data->getMonaY3());                              // mona y-position
  vN3Pos.SetZ(data->getMonaZ3());                              // mona z-position

  TVector3 vN3 = vN3Pos - vTPos;;                              // neutron path travelled vector
  double nVelo3 = vN3.Mag()/data->getMonaTime3();              // the velocity of the neutron

  TVector3 vVeloN3 = nVelo3 * vN3.Unit();
  TVector3 vBetaN3 = 1./0.299792458*vVeloN3;                   /// \todo use proper constants here!!!

  double gamma3 = 1./sqrt(1.-(vBetaN3.Mag()*vBetaN3.Mag()));   // neutron's gamma 

  lvN3.SetVectM(vBetaN3*gamma3*939.565346,939.565346);         // neutron 4-vector

  //-----> Add the 4th neutron <-----//

  vN4Pos.SetX(data->getMonaX4());                              // mona x-position
  vN4Pos.SetY(data->getMonaY4());                              // mona y-position
  vN4Pos.SetZ(data->getMonaZ4());                              // mona z-position

  TVector3 vN4 = vN4Pos - vTPos;;                              // neutron path travelled vector
  double nVelo4 = vN4.Mag()/data->getMonaTime4();              // the velocity of the neutron

  TVector3 vVeloN4 = nVelo4 * vN4.Unit();
  TVector3 vBetaN4 = 1./0.299792458*vVeloN4;                   /// \todo use proper constants here!!!

  double gamma4 = 1./sqrt(1.-(vBetaN4.Mag()*vBetaN4.Mag()));   // neutron's gamma 

  lvN4.SetVectM(vBetaN4*gamma4*939.565346,939.565346);         // neutron 4-vector


  //-----> Add the 1st Geant4 neutron <-----//
  vG1NPos.SetX(data->getMonaG1X());                            // mona x-position
  vG1NPos.SetY(data->getMonaG1Y());                            // mona y-position
  vG1NPos.SetZ(data->getMonaG1Z());                            // mona z-position

  TVector3 vG1N = vG1NPos - vTPos;                             // neutron path travelled vector
  double G1nVelo = vG1N.Mag()/data->getMonaG1Time();           // the velocity of the neutron

  TVector3 vVeloG1N = G1nVelo * vG1N.Unit();
  TVector3 vBetaG1N = 1./0.299792458*vVeloG1N;                 /// \todo use proper constants here!!!

  double gammaG1 = 1./sqrt(1.-(vBetaG1N.Mag()*vBetaG1N.Mag()));// neutron's gamma 

  lvG1N.SetVectM(vBetaG1N*gammaG1*939.565346,939.565346);      // neutron 4-vector
  
  //-----> Add the 2nd Geant4 neutron <-----//
  vG2NPos.SetX(data->getMonaG2X());//getMonaG2X                            // mona x-position
  vG2NPos.SetY(data->getMonaG2Y());                            // mona y-position
  vG2NPos.SetZ(data->getMonaG2Z());                            // mona z-position

  TVector3 vG2N = vG2NPos - vTPos;                             // neutron path travelled vector
  double G2nVelo = vG2N.Mag()/data->getMonaG2Time();           // the velocity of the neutron

  TVector3 vVeloG2N = G2nVelo * vG2N.Unit();
  TVector3 vBetaG2N = 1./0.299792458*vVeloG2N;                 /// \todo use proper constants here!!!

  double gammaG2 = 1./sqrt(1.-(vBetaG2N.Mag()*vBetaG2N.Mag()));// neutron's gamma 

  lvG2N.SetVectM(vBetaG2N*gammaG2*939.565346,939.565346);      // neutron 4-vector

  //-----> Add the 3rd Geant4 neutron <-----//
  vG3NPos.SetX(data->getMonaG3X());                            // mona x-position
  vG3NPos.SetY(data->getMonaG3Y());                            // mona y-position
  vG3NPos.SetZ(data->getMonaG3Z());                            // mona z-position

  TVector3 vG3N = vG3NPos - vTPos;                             // neutron path travelled vector
  double G3nVelo = vG3N.Mag()/data->getMonaG3Time();           // the velocity of the neutron

  TVector3 vVeloG3N = G3nVelo * vG3N.Unit();
  TVector3 vBetaG3N = 1./0.299792458*vVeloG3N;                 /// \todo use proper constants here!!!

  double gammaG3 = 1./sqrt(1.-(vBetaG3N.Mag()*vBetaG3N.Mag()));// neutron's gamma 

  lvG3N.SetVectM(vBetaG3N*gammaG3*939.565346,939.565346);      // neutron 4-vector

  //-----> Add the 4th Geant4 neutron <-----//
  vG4NPos.SetX(data->getMonaG4X());                            // mona x-position
  vG4NPos.SetY(data->getMonaG4Y());                            // mona y-position
  vG4NPos.SetZ(data->getMonaG4Z());                            // mona z-position

  TVector3 vG4N = vG4NPos - vTPos;                             // neutron path travelled vector
  double G4nVelo = vG4N.Mag()/data->getMonaG4Time();           // the velocity of the neutron

  TVector3 vVeloG4N = G4nVelo * vG4N.Unit();
  TVector3 vBetaG4N = 1./0.299792458*vVeloG4N;                 /// \todo use proper constants here!!!

  double gammaG4 = 1./sqrt(1.-(vBetaG4N.Mag()*vBetaG4N.Mag()));// neutron's gamma 

  lvG4N.SetVectM(vBetaG4N*gammaG4*939.565346,939.565346);      // neutron 4-vector

  //-----> Add the 5th Geant4 neutron <-----//
  vG5NPos.SetX(data->getMonaG5X());                            // mona x-position
  vG5NPos.SetY(data->getMonaG5Y());                            // mona y-position
  vG5NPos.SetZ(data->getMonaG5Z());                            // mona z-position

  TVector3 vG5N = vG5NPos - vTPos;                             // neutron path travelled vector
  double G5nVelo = vG5N.Mag()/data->getMonaG5Time();           // the velocity of the neutron

  TVector3 vVeloG5N = G5nVelo * vG5N.Unit();
  TVector3 vBetaG5N = 1./0.299792458*vVeloG5N;                 /// \todo use proper constants here!!!

  double gammaG5 = 1./sqrt(1.-(vBetaG5N.Mag()*vBetaG5N.Mag()));// neutron's gamma 

  lvG5N.SetVectM(vBetaG5N*gammaG5*939.565346,939.565346);      // neutron 4-vector

  //-----> Add the 6th Geant4 neutron <-----//
  vG6NPos.SetX(data->getMonaG6X());                            // mona x-position
  vG6NPos.SetY(data->getMonaG6Y());                            // mona y-position
  vG6NPos.SetZ(data->getMonaG6Z());                            // mona z-position

  TVector3 vG6N = vG6NPos - vTPos;                             // neutron path travelled vector
  double G6nVelo = vG6N.Mag()/data->getMonaG6Time();           // the velocity of the neutron

  TVector3 vVeloG6N = G6nVelo * vG6N.Unit();
  TVector3 vBetaG6N = 1./0.299792458*vVeloG6N;                 /// \todo use proper constants here!!!

  double gammaG6 = 1./sqrt(1.-(vBetaG6N.Mag()*vBetaG6N.Mag()));// neutron's gamma 

  lvG6N.SetVectM(vBetaG6N*gammaG6*939.565346,939.565346);      // neutron 4-vector

  //########### Creates the 4-vector for the fragment ##########//
  double m, p0, e0; // nominal momentum and energy of map
  p0 = fMap.getBrho() * fragQ * 299.792458;
  m  = fragA*931.494028;
  e0 = sqrt(m*m + p0*p0) - m;                           
  StParticle p; // use particle class for conversion to 4 vector
  p.setZ(fragQ);
  p.setA(fragA);
  p.setm(p.getA()*931.494028 + fragMassExcess);

  // 3 different methods: COSY-map, Schiller-map, Fit
  enum recType {COSY, SCHILL, FIT};
  for (size_t iRecon(0); iRecon < 3; iRecon++){
    ParticleCoordinates c;
    memset(&c,0,sizeof(c));
    c.xt  = data->getTargetX();  // fill coordinates with known data
    c.yt  = data->getTargetY();
    c.xd  = data->getFragX();
    c.yd  = data->getFragY();
    c.txd = data->getFragTX();
    c.tyd = data->getFragTY();
    c.t   = data->getFragTime();

 
    if ( iRecon == COSY ) c.xt = 0;
    DEB("Applying inverse map with c.xt = %g",c.xt);
    applyInvMap(&c);   // apply inverse map
    
    double e = e0 * ( 1 + c.d );
    if (e < 0) 
    {
      WAR("Reconstructed energy < 0: e = %g, c.d = %g; setting to zero",e,c.d);
      e = 0;
    }
    DSV(e);
    
    p.setx(c.xt);
    p.settx(c.txt);
    p.sety(c.yt);
    p.setty(c.tyt);
    p.setEkin(e);
    
    TLorentzVector lvF;  // fragment 4-vector

    lvF.SetPxPyPzE(p.getPx(),p.getPy(),p.getPz(),p.getEtotal());
    DSV(lvF.Px()/lvF.P());

    DSV(p.getPx()/p.getP());

    DSV(p.gettx());
    if (iRecon == COSY)   lvFCosy   = lvF;
    if (iRecon == SCHILL) lvFSchill = lvF;
    if (iRecon == FIT)    lvFFit    = lvF;
    double range, energy;
    deltaE[0] = 0;  // no correction
    
    //adding the manual control of deltaE for Schiller map
    if (manDESwitch == 1)
    {      
	    deltaE[1] = manDE;
    } else
    {    
	    // Ragney Implementation
            /*p.setEkin(e0);
            range = fRageny.range(matTarget,&p);
 	    p.setEkin(e0); // reset p.setEkin() since do_de changes KE
 	    energy = fRageny.energy(matTarget,range + dTarg/2,&p);
           	   
            deltaE[1] = p.getA()*energy - e0; // correction using nominal energy
            */

            // CycSrim Implementation
            p.setEkin(e0);

    
            #ifdef __WITH_FOIL
            deltaE[1] = p.getA()*(fRageny.energy_foil(srimFoil,tflatfoil/2,&p));
            deltaE[1] += p.getA()*(fRageny.energy(srimTarget,dTarg/2,&p));
            deltaE[1] += p.getA()*(fRageny.energy_foil(srimFoil,tflatfoil/2,&p));
            #else
            deltaE[1] = p.getA()*(fRageny.energy(srimTarget,dTarg/2,&p));
            #endif 

                             
    }

   // Ragney Implemenation
  /* p.setEkin(e);
     range = fRageny.range(matTarget,&p); 
     p.setEkin(e);
     energy = fRageny.energy(matTarget,range + dTarg/2,&p);

     deltaE[2] = p.getA()*energy - e; // correction using measured energy
     */

   // Calcualte thickness of target assuming we can measure the bulge
   //
   // This shit is so stupid

  // Calculate additional thickness due to bulge
    double bx; // beam spot x-pos  [m]
    double by; // beam spot y-pos  [m]
    double R;  // beam spot radius, R^2 = bx^2 + by^2 [m]
    //bulge += 0.000000001; // size of bulge [m] This is to prevent dividing by zero.
    double length = 0.019; // length from center of foil to edge [m] (1.9 cm)
    double density = 171.5; // density of D2 in kg/m^3
    double kdensity = 1420; // density of Kapton in kg/m^3
    double thickness = 0.0235; // [m] physical thickness of flat deuterium
    double bthick; // [mm] thickness accounting for bulge
    double dbulge = bulge + tflatfoil; // [m] additional bulge due to thickness;
    double alpha; // angle [rad]
    double dalpha; // angle for foil
    double Rc; // distance from center of gas to where beam hits [m]
    double dRc; // distance from center of foil to where beam hits [m]   
    double tfoil;//
    
    tfoil += 0.000000000001;
    bx = p.getx();
    by = p.gety();

    R = sqrt(pow(bx,2)+pow(by,2));

    if(R < length){
 
    Rc = (pow(length,2)+pow(bulge,2))/(2*bulge);
      
    alpha = asin(R/Rc);
    bthick = 2*(Rc*(cos(alpha)-1) + bulge) + thickness;
    bthick = bthick*density*100; //[mg/cm^2];
        
    // bulge from foil
    dRc = (pow(length+tflatfoil,2)+pow(dbulge,2))/(2*dbulge);
    dalpha = asin(R/dRc);
    tfoil = dRc*(cos(dalpha) - 1) + dbulge - ( Rc*(cos(alpha) - 1) + bulge );
    tfoil = tfoil*kdensity*100; //[mg/cm^2]

    }
   //cout << "bthick: " << bthick << " tfoil: " << tfoil << " maxbulge: " << bulge << endl;

  // CycSrim Implementation 
    p.setEkin(e);

    //cout << "e: " << e << endl;
  
    #ifdef __WITH_FOIL
    // Calculate energy loss at dTarg/2 from CycSrim:
    deltaE[2] = p.getA()*(fRageny.energy_foil(srimFoil,tfoil/2,&p));//tflatfoil/2     
    deltaE[2] += p.getA()*(fRageny.energy(srimTarget,bthick/2,&p));//dTarg/2
    deltaE[2] += p.getA()*(fRageny.energy_foil(srimFoil,tfoil/2,&p));//tflatfoil/2
    #else
    deltaE[2] = p.getA()*(fRageny.energy(srimTarget,dTarg/2,&p)); 
    #endif
    
    DSV(e0);
    DSV(e);
    DSV(deltaE[0]);
    DSV(deltaE[1]);
    DSV(deltaE[2]);

    // three ways to correct the energy
    for (size_t iEnCorr(0); iEnCorr < 3; iEnCorr++) {
      p.setEkin(e + deltaE[iEnCorr]);
      lvF.SetPxPyPzE(p.getPx(),p.getPy(),p.getPz(),p.getEtotal());

      TLorentzVector lvT;  //total 4-vector

      lvT = lvN + lvN2 + lvF;
      double eRecon = lvT.M() - lvN.M() - lvN2.M() - lvF.M();      // decay energy
      lvT = lvN +lvF;
      EDecay1 = lvT.M() - lvN.M() - lvF.M();
      lvT = lvN2 +lvF;
      EDecay2 = lvT.M() - lvN2.M() - lvF.M();

      lvT = lvN3 +lvF;
      EDecay3 = lvT.M() - lvN3.M() - lvF.M();
      
      lvT = lvG1N +lvF;
      EDecayG1 = lvT.M() - lvG1N.M() - lvF.M();
      lvT = lvG2N +lvF;
      EDecayG2 = lvT.M() - lvG2N.M() - lvF.M();
      lvT = lvG3N +lvF;
      EDecayG3 = lvT.M() - lvG3N.M() - lvF.M();
      lvT = lvG4N +lvF;
      EDecayG4 = lvT.M() - lvG4N.M() - lvF.M();
      lvT = lvG5N +lvF;
      EDecayG5 = lvT.M() - lvG5N.M() - lvF.M();
      lvT = lvG6N +lvF;
      EDecayG6 = lvT.M() - lvG6N.M() - lvF.M();


      //2n Edecay
      lvT = lvG1N +lvG2N;
      EDecayG_2neutron = lvT.M() - lvG1N.M() - lvG2N.M();


      TLorentzVector lvTemp;  //temp 4-vector
      lvT = lvG1N + lvG2N + lvF;
      lvTemp = lvG1N + lvF;
      EDecayTotalG1and2 = lvT.M() - lvG2N.M() - lvTemp.M() + EDecayG1;


      TLorentzVector lvTemp_Sys3body;
      lvTemp_Sys3body = lvF + lvG3N + lvG4N;
      EDecayTotalG3and4 = lvTemp_Sys3body.M() - lvG3N.M() - lvG4N.M() - lvF.M();

      //3n Edecay
      TLorentzVector lvT3 = lvG1N + lvG2N + lvG3N;
      EDecayG_3neutron = lvT3.M() - lvG1N.M() - lvG2N.M() - lvG3N.M();

      lvT3 = lvG1N + lvG2N + lvG3N + lvF;
      lvTemp = lvG1N + lvG2N + lvF;
      EDecayTotalG12and3 = lvT.M() - lvG3N.M() - lvTemp.M() + EDecayG1 + EDecayG2; 
     

      lvTemp = lvN + lvN2 + lvN3  + lvF;
      EDecay4body_True = lvTemp.M() - lvN.M() - lvN2.M() - lvN3.M() - lvF.M();


      TLorentzVector lvTemp_Sys;
      lvTemp_Sys = lvF + lvG1N + lvG2N + lvG3N;
      //lvTemp = lvG1N + lvG2N + lvG3N + lvF;
      //EDecay4body = lvF.M();
      EDecay4body = lvTemp_Sys.M() - lvG1N.M() - lvG2N.M() - lvG3N.M() - lvF.M();


      lvTemp = lvN + lvN2 + lvN3 + lvN4 + lvF;
      EDecay5body_True = lvTemp.M() - lvN.M() - lvN2.M() - lvN3.M() - lvN4.M() - lvF.M();


      //4n Edecay
      TLorentzVector lvTemp_Sys4n;
      lvTemp_Sys4n = lvF + lvG1N + lvG2N + lvG3N + lvG4N;
      EDecay5body = lvTemp_Sys4n.M() - lvG1N.M() - lvG2N.M() - lvG3N.M() - lvG4N.M() - lvF.M();
		
      EDecayGHigh = EDecayG1;
      EDecayGLow = EDecayG2;
      if(EDecayG1 < EDecayG2) {
      		
	EDecayGHigh = EDecayG2;
	EDecayGLow = EDecayG1;
	//commented zwk so don't sort the edecay
	//double temp_G = EDecayG1;
	//EDecayG1 = EDecayG2;
	//EDecayG2 = temp_G;
      }


      if (iRecon == COSY)   altExenCosy[iEnCorr]   = eRecon;
      if (iRecon == SCHILL) altExenSchill[iEnCorr] = eRecon;
      if (iRecon == FIT)    altExenFit[iEnCorr]    = eRecon;

      if (iRecon == SCHILL && iEnCorr == 1) {
        tLab = lvF.Vect().Angle(lvN.Vect())*57.2957795;
        tLab2 = lvF.Vect().Angle(lvN2.Vect())*57.2957795;
        vRel = lvN.Beta()*29.9792458 - lvF.Beta()*29.9792458;
   	vRelG1 = lvG1N.Beta()*29.9792458 - lvF.Beta()*29.9792458; //ANK:
	vRelG2 = lvG2N.Beta()*29.9792458 - lvF.Beta()*29.9792458; //ANK:
        vRel2 = lvN2.Beta()*29.9792458 - lvF.Beta()*29.9792458;
        nVel = lvN.Beta()*29.9792458;         // Neutron velocity
	    nVel2 = lvN2.Beta()*29.9792458;       // Neutron velocity
	    fVel = lvF.Beta()*29.9792458;         // Fragment velocity
	    fragKinE = (lvF.T() - lvF.M())/fragA; // Fragment kinetic Energy MeV/u
	    neutronKinE = lvN.T() - lvN.M();      // 1st Neutron kinetic Energy
    	neutronKinE2 = lvN2.T() - lvN2.M();   // 2nd Neutron kinetic Energy
	    nTheta = lvN.Theta()*57.2957795;      // 1st Neutron's Theta, Convert to degress 1 rad = 57.2957795 deg
    	nPhi = lvN.Phi()*57.2957795;          // 1st Neutron Phi, Convert to degress 1 rad = 57.2957795 deg
	    nTheta2 = lvN2.Theta()*57.2957795;    // 2nd Neutron's Theta, Convert to degress 1 rad = 57.2957795 deg
	    nPhi2 = lvN2.Phi()*57.2957795;        // 2nd Neutron's Phi, Convert to degress 1 rad = 57.2957795 deg
	    fTheta = lvF.Theta()*57.2957795;      // Fragment Theta, Convert to degress 1 rad = 57.2957795 deg
	    fPhi = lvF.Phi()*57.2957795;          // Fragment Phi, Convert to degress 1 rad = 57.2957795 deg
	    fAngleX = ((3.14159/2)-acos(sin(fTheta/57.2957795)*cos(fPhi/57.2957795))); // x-angle at target IN RADIANS!!!
	    fAngleY = (asin(sin(fTheta/57.2957795)*sin(fPhi/57.2957795))); // y-angle at target IN RADIANS!!!
	    nAngleX = ((3.14159/2)-acos(sin(nTheta/57.2957795)*cos(nPhi/57.2957795))); // x-angle at target IN RADIANS!!!
	    nAngleY = (asin(sin(nTheta/57.2957795)*sin(nPhi/57.2957795))); // y-angle at target IN RADIANS!!!
	    nAngleX2 = ((3.14159/2)-acos(sin(nTheta2/57.2957795)*cos(nPhi2/57.2957795))); // x-angle at target IN RADIANS!!!
	    nAngleY2 = (asin(sin(nTheta2/57.2957795)*sin(nPhi2/57.2957795))); // y-angle at target IN RADIANS!!!

	fpATA = c.txt;
	fpBTA = c.tyt;
	fpYTA = c.yt;
	delta = c.d;
	fYfp = c.yd;
	fBfp = c.tyd;
	fXfp = c.xd;
	fAfp = c.txd;

	/*
	//----ZWK calculation for angular correlations in CM of decaying frame----

	Double_t M1 = 931.494088 * fragA + fragMassExcess;
	Double_t M2 = 939.565560;

	//lab-frame info
	Double_t ffd = sqrt( lvF.Z()*lvF.Z() + lvF.X()*lvF.X() +  lvF.Y()*lvF.Y());
	Double_t ffz = lvF.Z();
	Double_t ffx = lvF.X();
	Double_t ffy = lvF.Y();

	Double_t ff_theta = acos(ffz / ffd);
	Double_t ff_phi = TMath::ATan2(ffy,ffx);

	Double_t P1 = sqrt(lvF.E()*lvF.E() - M1*M1); 

	Double_t ff_px = P1 * sin(ff_theta) * cos(ff_phi);
	Double_t ff_py = P1 * sin(ff_theta) * sin(ff_phi);
	Double_t ff_pz = P1 * cos(ff_theta);

	Double_t ff_ke = lvF.E() - M1;

	Double_t ff_gamma = 1 + (ff_ke / M1);
	Double_t ff_beta = sqrt(1. - 1./(ff_gamma*ff_gamma)); 
	Double_t ff_bx = ff_beta * sin(ff_theta) * cos(ff_phi);
	Double_t ff_by = ff_beta * sin(ff_theta) * sin(ff_phi);
	Double_t ff_bz = ff_beta * cos(ff_theta);

	//neutrons
	Double_t nnz = lvG1N.Z();
	Double_t nnx = lvG1N.X();
	Double_t nny = lvG1N.Y();
	Double_t nnd = sqrt(nnz*nnz+nny*nny+nnx*nnx);
	Double_t nnz2 = lvG2N.Z();
	Double_t nnx2 = lvG2N.X();
	Double_t nny2 = lvG2N.Y();
	Double_t nnd2 = sqrt(nnz2*nnz2+nny2*nny2+nnx2*nnx2);

	Double_t gN1KE = lvG1N.E() - 939.565;
	Double_t gN2KE = lvG2N.E() - 939.565;
	//neutron 1
	Double_t n_theta = acos(nnz / nnd);
	Double_t n_phi = 1*TMath::ATan2(nny,nnx);

	Double_t P2 = sqrt(lvG1N.E()*lvG1N.E() - M2*M2);

	Double_t n_px = P2 * sin(n_theta) * cos(n_phi);
	Double_t n_py = P2 * sin(n_theta) * sin(n_phi);
	Double_t n_pz = P2 * cos(n_theta);

	Double_t n_gamma = 1 + (gN1KE / M2);
	Double_t n_beta = sqrt(1. - 1./(n_gamma*n_gamma)); 
	Double_t n_bx = n_beta * sin(n_theta) * cos(n_phi);
	Double_t n_by = n_beta * sin(n_theta) * sin(n_phi);
	Double_t n_bz = n_beta * cos(n_theta);

	//neutron 2
	Double_t n2_theta = acos(nnz2 / nnd2);
	Double_t n2_phi = 1*TMath::ATan2(nny2,nnx2);

	Double_t P3 = sqrt(lvG2N.E()*lvG2N.E() - M2*M2);

	Double_t n2_px = P3 * sin(n2_theta) * cos(n2_phi);
	Double_t n2_py = P3 * sin(n2_theta) * sin(n2_phi);
	Double_t n2_pz = P3 * cos(n2_theta);

	Double_t n2_gamma = 1 + (gN2KE / M2);
	Double_t n2_beta = sqrt(1. - 1./(n2_gamma*n2_gamma)); 
	Double_t n2_bx = n2_beta * sin(n2_theta) * cos(n2_phi);
	Double_t n2_by = n2_beta * sin(n2_theta) * sin(n2_phi);
	Double_t n2_bz = n2_beta * cos(n2_theta);


	//Cacluate total momentum of the decay system or frame
	Double_t PxCM = ff_px + n_px + n2_px;
	Double_t PyCM = ff_py + n_py + n2_py;
	Double_t PzCM = ff_pz + n_pz + n2_pz;
	Double_t PtotCM = sqrt( PxCM*PxCM + PyCM*PyCM + PzCM*PzCM);

	Double_t thetaCM = acos(PzCM / PtotCM);
	Double_t phiCM = TMath::ATan2(PyCM, PxCM);

	Double_t MassCM = M1 + M2 + M2;
	Double_t KECM = sqrt( MassCM*MassCM + PtotCM*PtotCM) - MassCM;
	Double_t GammaCM = 1 + (KECM / MassCM);
	Double_t BetaCM = sqrt(1. - 1./(GammaCM*GammaCM));

	Double_t BxCM = BetaCM * sin(thetaCM) * cos(phiCM);
	Double_t ByCM = BetaCM * sin(thetaCM) * sin(phiCM);
	Double_t BzCM = BetaCM * cos(thetaCM);

	//--CM vectors of neutrons---
	Double_t n_bxCM = n_bx - BxCM;
	Double_t n_byCM = n_by - ByCM;
	Double_t n_bzCM = n_bz - BzCM;
	Double_t n_bCM = sqrt(n_bxCM*n_bxCM + n_byCM*n_byCM + n_bzCM*n_bzCM);

	Double_t n2_bxCM = n2_bx - BxCM;
	Double_t n2_byCM = n2_by - ByCM;
	Double_t n2_bzCM = n2_bz - BzCM;
	Double_t n2_bCM = sqrt(n2_bxCM*n2_bxCM + n2_byCM*n2_byCM + n2_bzCM*n2_bzCM);

	Double_t ff_bxCM = ff_bx - BxCM;
	Double_t ff_byCM = ff_by - ByCM;
	Double_t ff_bzCM = ff_bz - BzCM;
	Double_t ff_bCM = sqrt(ff_bxCM*ff_bxCM + ff_byCM*ff_byCM + ff_bzCM*ff_bzCM);

	//di-neutron-stuff
	Double_t din_PxCM = M2*n_bxCM + M2*n2_bxCM;
	Double_t din_PyCM = M2*n_byCM + M2*n2_byCM;
	Double_t din_PzCM = M2*n_bzCM + M2*n2_bzCM;
	Double_t din_PCM = sqrt(din_PxCM*din_PxCM + din_PyCM*din_PyCM + din_PzCM*din_PzCM);

	Double_t din_BxCM = din_PxCM/(2*M2);  //mass dineutorn = 2*M2
	Double_t din_ByCM = din_PyCM/(2*M2);
	Double_t din_BzCM = din_PzCM/(2*M2);
	Double_t din_BetaCM = sqrt(din_BxCM*din_BxCM + din_ByCM*din_ByCM + din_BzCM*din_BzCM);

	//Angle between 2 neutrons
	Double_t n1n2CM_dot = n_bxCM*n2_bxCM + n_byCM*n2_byCM + n_bzCM*n2_bzCM;
	n1n2costhetaCM = n1n2CM_dot / (n2_bCM * n_bCM);
	n1n2thetaCM = 1*acos(n1n2costhetaCM);
	n1n2thetaCM_deg = acos(n1n2costhetaCM)*180.0/3.14;

	//Angle between frag and 1 neutrons
	Double_t ffn1CM_dot = n_bxCM*ff_bxCM + n_byCM*ff_byCM + n_bzCM*ff_bzCM;
	ffn1costhetaCM = ffn1CM_dot / (ff_bCM * n_bCM);
	ffn1thetaCM = 1*acos(ffn1costhetaCM);
	ffn1thetaCM_deg = acos(ffn1costhetaCM)*180.0/3.14;

	//Angle between frag and di-neutron
	Double_t ffdinCM_dot = din_BxCM*ff_bxCM + din_ByCM*ff_byCM + din_BzCM*ff_bzCM;
	ffdincosthetaCM = ffdinCM_dot / (ff_bCM * din_BetaCM);
	ffdinthetaCM = 1*acos(ffdincosthetaCM);
	ffdinthetaCM_deg = acos(ffdincosthetaCM)*180.0/3.14;	
	//---------------------------------------------------------------
	*/

	//TLorentzVector lvF_sys = lvF + lvG1N + lvG2N;
	//TVector3 boost = lvF_sys.BoostVector();
	//TLorentzVector lvG1Ncm = lvG1N;
	//lvG1Ncm.Boost(-1*boost);
	//TLorentzVector lvG2Ncm = lvG2N;
	//lvG2Ncm.Boost(-1*boost);
	//TLorentzVector lvFcm = lvF;
	//lvFcm.Boost(-1*boost);
	//printf("\n\nFragment: X:%lf Y:%lf Z:%lf\n",lvFcm.Px(),lvFcm.Py(),lvFcm.Pz());
	//printf("N1: X:%lf Y:%lf Z:%lf\n",lvG1Ncm.Px(),lvG1Ncm.Py(),lvG1Ncm.Pz());
	//printf("N2: X:%lf Y:%lf Z:%lf\n",lvG2Ncm.Px(),lvG2Ncm.Py(),lvG2Ncm.Pz());
	//printf("cosN1N2: %lf\n", cos(lvG1Ncm.Angle(lvG2Ncm.Vect())));	
	//TVector3 FF(ff_bxCM*M1, ff_byCM*M1, ff_bzCM*M1);
	//TVector3 N1(n_bxCM*M2, n_byCM*M2, n_bzCM*M2);
	//TVector3 N2(n2_bxCM*M2, n2_byCM*M2, n2_bzCM*M2);
	//printf("zFragment: X:%lf Y:%lf Z:%lf\n",FF.x(),FF.y(),FF.z());
	//printf("zN1: X:%lf Y:%lf Z:%lf\n",N1.x(),N1.y(),N1.z());
	//printf("zN2: X:%lf Y:%lf Z:%lf\n",N2.x(),N2.y(),N2.z());
	//printf("zCos: %lf\n",n1n2costhetaCM);
		

	//test for momentum distributions....
	double halftgt_KE_proj = 79.5; //MeV/u
	double projA = 26;
	double projE =  halftgt_KE_proj*projA + projA*931.494;
	double projP = sqrt( projE*projE - pow(projA*931.494, 2.0) );
	TLorentzVector lvProj;
	lvProj.SetPxPyPzE(0, 0, projP, projE);

	TLorentzVector lvFN = lvF + lvG1N;

	TVector3 projBoost = lvProj.BoostVector();
	lvFN.Boost(-1*projBoost);
	Pz_FragN = lvFN.Pz();

	TLorentzVector lvFonly = lvF;       
	lvFonly.Boost(-1*projBoost);
	Pz_Frag = lvFonly.Pz();

    Ppar_Frag = lvF.Pz();
	Pperp_Frag = sqrt(lvF.Px()*lvF.Px() + lvF.Py()*lvF.Py());
    Ptot_Frag = sqrt(Ppar_Frag*Ppar_Frag+Pperp_Frag*Pperp_Frag);

    

	//JACOBI COORDINATE SYSTEM VALUES
	double Tsystem[4]={0};
	double Ysystem[4]={0};
	double cosn1n2theta;

	double massF = fragA;
	double massN1 = 1;
	double massN2 = 1;

	double n1pp=-1, n2pp=-1, n1pt=-1, n2pt=-1;

	TLorentzVector lTot = lvF + lvG1N + lvG2N;
	Theta_3body = lTot.Theta() * TMath::RadToDeg();
	Phi_3body = lTot.Phi() * TMath::RadToDeg();

	JacobiCorr(lvF, lvG1N, lvG2N, massF, massN1, massN2, Tsystem, Ysystem, cosn1n2theta, n1pp, n2pp, n1pt, n2pt);

	n1n2costhetaCM = cosn1n2theta;

	Tsys_Ex_Et1 = Tsystem[0];
	Tsys_Ex_Et2 = Tsystem[2];
	Ysys_Ex_Et1 = Ysystem[0];
	Ysys_Ex_Et2 = Ysystem[2];

	Tsys_CosTheta1 = Tsystem[1];
	Tsys_CosTheta2 = Tsystem[3];
	Ysys_CosTheta1 = Ysystem[1];
	Ysys_CosTheta2 = Ysystem[3];

	nnCM_ppar1 = n1pp;
	nnCM_ppar2 = n2pp;
	nnCM_pperp1 = n1pt;
	nnCM_pperp2 = n2pt;

      }
    }
  }

  //
  // calculate tLab and vRel  !!!
  //

  EDecayTotal = altExenSchill[1];
  
  return 0;
}

//void MonaAnalyzedEvent::JacobiCorr(TVector3 FF, TVector3 N1, TVector3 N2, Double_t massF, Double_t massN1, Double_t massN2, double Tsystem[4], double Ysystem[4]) {
void MonaAnalyzedEvent::JacobiCorr(TLorentzVector lv_F, TLorentzVector lv_N1, TLorentzVector lv_N2, Double_t massF, Double_t massN1, Double_t massN2, double Tsystem[4], double Ysystem[4], double &cosn1n2theta,  double &nCM_ppar1, double &nCM_ppar2, double &nCM_pperp1, double &nCM_pperp2) {
  //Following analysis was largely taken form Description of Charity et al. PRC 84, 014320 (2011)
  //cout << "JacobiCorr" << endl;
  //Module uses the TLorentz Vectors in the lab-frame and transforms to the center-of-mass of the system before calculating the energy and angle.
  //4 Cases -> T and Y systems and two ways to label the neutrons
  

  //Go into CM frame--------
  TLorentzVector lv_sys = lv_F + lv_N1 + lv_N2;
  TVector3 boost = lv_sys.BoostVector();
  TLorentzVector lv_N1cm = lv_N1;
  lv_N1cm.Boost(-1*boost);
  TLorentzVector lv_N2cm = lv_N2;
  lv_N2cm.Boost(-1*boost);
  TLorentzVector lv_Fcm = lv_F;
  lv_Fcm.Boost(-1*boost);

  cosn1n2theta =  cos(lv_N1cm.Angle(lv_N2cm.Vect()));


  //calculate parallel and perp in CM, J. Schiffer idea..
  nCM_ppar1 = lv_N1cm.Pz();
  nCM_ppar2 = lv_N2cm.Pz();
  nCM_pperp1 = lv_N1cm.Pt();
  nCM_pperp2 = lv_N2cm.Pt();


  TVector3 N1 = lv_N1cm.Vect();
  TVector3 N2 = lv_N2cm.Vect();
  TVector3 FF = lv_Fcm.Vect();

  Double_t M = 931.5; 

  for(int i=0; i<4; i++){
    
    TVector3 k1, k2, k3;
    Double_t m1, m2, m3;

    if(i==0){
      //T - System, k1 = N1
      k1 = N1;
      k2 = N2;
      k3 = FF;
      m1 = massN1;
      m2 = massN2;
      m3 = massF;      
    }
    else if(i==1){
      //T - System, k1 = N2
      k1 = N2;
      k2 = N1;
      k3 = FF;
      m1 = massN2;
      m2 = massN1;
      m3 = massF;      
    }
    else if(i==2){
      //Y - System, k1 = N1
      k1 = N1;
      k2 = FF;
      k3 = N2;
      m1 = massN1;
      m2 = massF;
      m3 = massN2;      
    }
    else if(i==3){
      //Y - System, k1 = N2
      k1 = N2;
      k2 = FF;
      k3 = N1;
      m1 = massN2;
      m2 = massF;
      m3 = massN1;      
    }

    //Using pow(..., -1.0) instead of dividing with '/' because that operator is not defined for TVector3.
    TVector3 kx = (m2*k1 - m1*k2) * pow(m1 + m2,-1.0);
    TVector3 ky = (m3*(k1+k2) - (m1+m2)*k3) * pow(m1+m2+m3,-1.0); 
    
    Double_t Ex = ( (m1+m2)*kx.Mag2() ) / (2*M*m1*m2);
    Double_t Ey = ( (m1+m2+m3)*ky.Mag2() ) / (2*M*m3*(m1+m2));

    Double_t Et = Ex + Ey;

    Double_t Ex_Et = Ex / Et;

    Double_t costheta_k =  kx.Dot(ky) / (kx.Mag() * ky.Mag());
    Double_t theta_k = TMath::ACos(costheta_k);

    //Set array to correct values
    if(i==0){
      Tsystem[0] = Ex_Et;
      Tsystem[1] = costheta_k;
    }
    if(i==1){
      Tsystem[2] = Ex_Et;
      Tsystem[3] = costheta_k;
    }
    if(i==2){
      Ysystem[0] = Ex_Et;
      Ysystem[1] = costheta_k;
    }
    if(i==3){
      Ysystem[2] = Ex_Et;
      Ysystem[3] = costheta_k;
    }


  }//i
  //cout << "end Jacobi" << endl;

}



// ##############
void usage() {
  printf("\n"
         "usage:\n"
         "-v   verbose\n"                             //1
         "-if  input  file         (st_mona.root)\n"  //2
         "-of  output file         (ana_mona.root)\n" //3
         "-it  input  tree name    (t)\n"             //4
         "-ot  output tree name    (at)\n"            //5
         "-frag fragment           (23O)\n"           //6
         "-mix number of events to mix (0)\n"         //7
         "-flt do not analyze the data, but write filtered file\n"  //8
         "\n"
         "Event mixing (using option -mix <n> with n > 0) mixes each event with\n"
         "n previous events.  Mona x, y, z, and t are taken from one event and\n"
         "Fragment x, y, tx, ty and Target x, y are taken from another event.\n"
         "And vice versa!!!\n"
	 "-manDE <value> allows you to control the deltaE parameter manually.\n"
	 "              Input the value you want to be added back to energy.\n"
       );
  exit(0);
}

#include "TFile.h"
#include "TTree.h"
#include <TString.h> 
#include "TObjString.h"

using namespace std;

int main (size_t argc, char* argv[] ) {
  cout << "\nEntering MoNA Analysis main().\n";
  //get location of executable so we can find input files
  string execloc = argv[0];
  short execlength = 13;
  execloc.erase(execloc.length() - execlength, execlength);

  /*--------------*/
  double manDeltaE = 0.0;           //for manual control of energy added back
  int manESwitch = 0;               //switch for manual control of energy
  string inputFile(execloc + "../root_files/st_mona.root");
  string outputFile(execloc + "../root_files/ana_mona.root");
  string iTreeName("t");
  string oTreeName("at");
  int writeFilter(0);
  string frag("23O");
  size_t nMix = 0;
  int verbose;
  int dT_change = 0;
  double target_thickness = 0;
  double tflatfoil = 0.0;
  double bulge = 0.0;

  int FragA = 0;
  int FragQ = 0;
  int TargA = 0;
  int TargZ = 0;
  string cosymap;
  string inputfile;
 // Read in input file here

  string INPUT_DATA_DIR = "/projects/mona-sim/baumann/single_target/st_mona/input_files/";
  string INPUT_FILE     = "input_file_TB.txt";
  inputfile = INPUT_DATA_DIR + INPUT_FILE; 
  
 // Read in Input File
 //ifstream myfile("/projects/mona-sim/jonesm/st_mona/input_files/input.txt");
 ifstream myfile(inputfile.c_str());  
//ifstream myfile;
 //myfile.open(inputfile,ios::in);
 string line;
 string VAR_NAME;
 string VAR_VALUE;
 int delimiter;
 int newline;
 int length;
 char var_name[100];
 char var_value[100];
 char* pch;
 size_t name_length;
 size_t value_length;
 if (myfile.is_open())
   {
     while ( getline(myfile,line) ) 
       {
         getline(myfile,line,';');

         if(line.find_first_of("#") != line.find_first_not_of(" \t")){// If the first non-whitespace char is NOT a # (comment)  
            delimiter = line.find("=");// cout << " delimiter " << delimiter << endl;
            length = line.length();

         if(line.length() != 0 ) {// Make sure string is not NULL
         name_length =line.copy(var_name,delimiter,0); 
         var_name[name_length] = '\0';
         value_length = line.copy(var_value,line.length(),delimiter+1); 
         var_value[value_length] = '\0';
         
	  // Remove whitespace from var_name and var_value
             VAR_NAME  = string(var_name);
             VAR_VALUE = string(var_value);
             int strBegin = VAR_NAME.find_first_not_of(" \t"); int strBegin1 = VAR_VALUE.find_first_not_of(" \t");
  	     int strEnd   = VAR_NAME.find_last_not_of(" \t");  int strEnd1   = VAR_VALUE.find_last_not_of(" \t");
  	     int strRange = strEnd - strBegin +1;	       int strRange1 = strEnd1 - strBegin1 + 1;
             VAR_NAME = VAR_NAME.substr(strBegin, strRange);   VAR_VALUE = VAR_VALUE.substr(strBegin1, strRange1);
	     strcpy(var_name,VAR_NAME.c_str()); 	       
             strcpy(var_value,VAR_VALUE.c_str());
          }
         cout << "var_name: " << var_name << " var_value: " << var_value << endl;
         
         // Check to see if var_value contains quotations, if not convert to a double
         pch = strpbrk(var_value,"\""); 
         if(pch == NULL) {// convert var_value to a double or int
             double dvalue = atof(var_value);
             double ivalue = round(dvalue);

	     // List doubles to set and check name of variable        
                  if( strcmp(var_name,"dTarget") == 0 ) {  	target_thickness = dvalue; }
	     else if( strcmp(var_name,"tflatfoil") == 0 ) { 	tflatfoil 	 = dvalue; }
	     else if( strcmp(var_name, "maxbulge") == 0 ) {	bulge 		 = dvalue; }			
	     
             // List ints and check variable name

             else if( strcmp(var_name,"fragA") == 0 ) 		FragA	  	= ivalue;
             else if( strcmp(var_name,"fragZ") == 0 ) 		FragQ		= ivalue;
             else if( strcmp(var_name,"targA") == 0 ) 		TargA	 	= ivalue;
             else if( strcmp(var_name,"targZ") == 0 ) 		TargZ		= ivalue;
           

             } 
         else if (pch != NULL) {// Do not convert to a double
             // Cut off quotation marks and keep variable as a string.
             int valBegin = VAR_VALUE.find_first_of("\"")+1;
             int valEnd   = VAR_VALUE.find_last_of("\"");
             int valRange = valEnd - valBegin;
             VAR_VALUE = VAR_VALUE.substr(valBegin,valRange);
             strcpy(var_value,VAR_VALUE.c_str());
             
	     // List strings and check variable name
	        if( strcmp(var_name,"cosymap") == 0) 	       cosymap		= var_value;

             }
         }
       }
      myfile.close();
    }
  else  ERR("Could not find input file!");

// Set arguments afterwards so that we can overwrite input file (if we so desire)

  for (size_t i = 1; i < argc; i++) {
    string sw=argv[i];
    if (sw == "-v")          verbose     = 1;                    // 1
    else if (sw == "-if")    inputFile   =      argv[++i];       // 2
    else if (sw == "-of")    outputFile  =      argv[++i];       // 3
    else if (sw == "-it")    iTreeName   =      argv[++i];       // 4
    else if (sw == "-ot")    oTreeName   =      argv[++i];       // 5
    else if (sw == "-flt")   writeFilter =      1;               // 8
    else if (sw == "-frag")  frag        =      argv[++i];       // 6
    else if (sw == "-mix")   nMix        = atoi(argv[++i]);      // 7
    else if (sw == "-dT") { dT_change = 1;
      target_thickness =  atof(argv[++i]);
    }
    else if (sw == "-manDE") { 
	    manESwitch = 1;
	    manDeltaE = atof(argv[++i]);
    }
    else if (sw == "-?" || sw == "--help" || sw == "-help") usage();
    else ERR("Uknown option %s",sw.c_str());
  }

  CalMonaEventSim data;  // calibrated mona event with data source

  // file and tree of simulated data
  TFile fin(inputFile.c_str());
  TTree* tin = (TTree*) fin.Get(iTreeName.c_str());
  if (!tin) ERR("No tree pointer found!!!");
  data.setTree(tin);

  if (writeFilter) {
    INFO("Writing filtered output file %s",outputFile.c_str());
    CXdrOutputStream o(outputFile.c_str());

    data.writeFilterHeader(&o);

    //  loop over all events
    DEB("Looping over events");
    while( data.getEvent() ) { // as long as there are good events
      data.writeFilter(&o);
    }

    o.Close();
    return 0;
  }

  MonaAnalyzedEvent* e = new MonaAnalyzedEvent;

 // Set fragA/Q, targA/Z and dTarg from Input file
 e->fragA = FragA;
 e->fragQ = FragQ;
 e->targA = TargA;
 e->targZ = TargZ;
 e->dTarg = target_thickness;
 e->tflatfoil = tflatfoil;
 e->bulge = bulge;
  
 const char *COSYmap = cosymap.c_str();
#include "maps.icc"
	     if ( strcmp(COSYmap, "m23O") == 0) 		e->setMaps(m23O, m23Oi);
	else if ( strcmp(COSYmap, "m22O") == 0) 		e->setMaps(m22O, m22Oi);
	else if ( strcmp(COSYmap, "m6He") == 0) 		e->setMaps(m6He, m6Hei);
	else if ( strcmp(COSYmap, "mJenna") == 0) 		e->setMaps(mJenna, mJennai);
	else if ( strcmp(COSYmap, "m8he_jesse") == 0) 	e->setMaps(m8he_jesse, m8he_jessei);
	else if ( strcmp(COSYmap, "m8Li") == 0) 		e->setMaps(m8Li, m8Lii);
	else if ( strcmp(COSYmap, "m9Li") == 0) 		e->setMaps(m9Li, m9Lii);
	else if ( strcmp(COSYmap, "m11Li_BeBeam") == 0) e->setMaps(m11Li_BeBeam, m11Lii_BeBeam);
	else if ( strcmp(COSYmap, "m10Bea") == 0)		e->setMaps(m10Bea, m10Beai);
 	else if ( strcmp(COSYmap, "m10Beb") == 0) 		e->setMaps(m10Beb, m10Bebi);
	else if ( strcmp(COSYmap, "m11Bec") == 0) 		e->setMaps(m11Bec, m11Beci);
	else if ( strcmp(COSYmap, "m12be_jesse") == 0) 	e->setMaps(m12be_jesse, m12be_jessei);
	else if ( strcmp(COSYmap, "m14Be_B17beam") == 0)e->setMaps(m14Be_B17beam, m14Be_B17beami);
	else if ( strcmp(COSYmap, "m14Be") == 0) 		e->setMaps(m14Be, m14Bei);
	else if ( strcmp(COSYmap, "m13B") == 0)  		e->setMaps(m13B, m13Bi);
	else if ( strcmp(COSYmap, "m15B_06025") == 0) 	e->setMaps(m15B_06025, m15B_06025i);
	else if ( strcmp(COSYmap, "m16C") == 0) 		e->setMaps(m16C, m16Ci);
	else if ( strcmp(COSYmap, "m18Of") == 0) 		e->setMaps(m18Of, m18Ofi);
	else if ( strcmp(COSYmap, "m19O") == 0) 		e->setMaps(m19O, m19Oi);
	else if ( strcmp(COSYmap, "m20O") == 0) 		e->setMaps(m20O, m20Oi);
	else if ( strcmp(COSYmap, "m22Oa") == 0) 		e->setMaps(m22O, m22Oi);
	else if ( strcmp(COSYmap, "m22Of") == 0) 		e->setMaps(m22Of, m22Ofi);
	else if ( strcmp(COSYmap, "m22o_09028") == 0) 	e->setMaps(m22o_09028, m22o_09028i);
	else if ( strcmp(COSYmap, "m23O") == 0) 		e->setMaps(m23O, m23Oi);
	else if ( strcmp(COSYmap, "m23Oa") == 0) 		e->setMaps(m23Oa, m23Oai);
	else if ( strcmp(COSYmap, "m23o_09028") == 0) 	e->setMaps(m23o_09028, m23o_09028i);
	else if ( strcmp(COSYmap, "m24O") == 0) 		e->setMaps(m24O, m24Oi);
	else if ( strcmp(COSYmap, "m24o_09028") == 0) 	e->setMaps(m24o_09028, m24o_09028i);
	else if ( strcmp(COSYmap, "m24o_hope") == 0) 	e->setMaps(m24o_hope, m24o_hopei);
	else if ( strcmp(COSYmap, "m26f") == 0) 		e->setMaps(m26f, m26fi);
	else if ( strcmp(COSYmap, "m25Ne") == 0) 		e->setMaps(m25Ne, m25Nei);
	else if ( strcmp(COSYmap, "m26ne_calem") == 0) 	e->setMaps(m26ne_calem, m26ne_calemi);
	else if ( strcmp(COSYmap, "m12Be_06025") == 0)  /*e->setMaps(m12Be_06025, m12Be_06025i);*//*DV 10/31/16*/ cout << "\n\n\n CHECK MONA_ANALYSIS.CC LINE 2635 \n\n\n";
	else if ( strcmp(COSYmap, "m23O_10023") == 0)  e->setMaps(m23O_10023, m23O_10023i);
	else if ( strcmp(COSYmap, "m7Li_e11028") == 0)  e->setMaps(m7Li_e11028, m7Li_e11028i);//ANK: 10/7/2015
	else if ( strcmp(COSYmap, "m6He_e11028") == 0)  e->setMaps(m6He_e11028, m6He_e11028i);//ANK: 10/7/2015
        else if (strcmp(COSYmap, "m11Be_e15091") == 0) e->setMaps(m11Be_e15091, m11Be_e15091i); // DV: 11/16/2017.
        else if (strcmp(COSYmap, "m12B_e15091") == 0) e->setMaps(m12B_e15091, m12B_e15091i); // DV: 11/16/2017.
        else if (strcmp(COSYmap, "m8He_11Be_e15091") == 0) e->setMaps(m8He_11Be_e15091, m8He_11Be_e15091i); // DV: 10/30/2017.
        else if (strcmp(COSYmap, "m8He_12B_e15091") == 0) e->setMaps(m8He_12B_e15091, m8He_12B_e15091i); // DV: 10/30/2017.

 //  Targets are defined below, be sure to check that it is the target you want.
 //  Yes they all look the same and its redundant, but it allows one to change targets specifically
 //  and changing targets isn't exactly something you want to do in an input file.
 //  or is it? The advantage would be defining the target ONCE.
 //  You are welcome to change this as you see fit.

    CycSrim *srimBe = new CycSrim(CycSrim::SrimMaterialBeryllium, e->dTarg, CycSrim::kUnitsMgCm2);
    CycSrim *srimD2 = new CycSrim(CycSrim::SrimMaterialDeuterium, e->dTarg, CycSrim::kUnitsMgCm2);
    CycSrim *Kapton = new CycSrim(CycSrim::SrimMaterialKapton, tflatfoil, CycSrim::kUnitsMgCm2);

       if (frag == "23O")   		e->setCycSrim(srimBe);
  else if (frag == "22O")      		e->setCycSrim(srimBe);
  else if (frag == "6He")    		e->setCycSrim(srimBe);
  else if (frag == "8Li")     		e->setCycSrim(srimBe);
  else if (frag == "10Bea")     	e->setCycSrim(srimBe);
  else if (frag == "10Beb")     	e->setCycSrim(srimBe);
  else if (frag == "24O_Calem"){     	e->setCycSrim(srimBe);
					e->setSrimFoil(Kapton);
				}
  else if (frag == "24O_Jones"){
					e->setCycSrim(srimD2);
					e->setSrimFoil(Kapton);
				}
  else if (frag == "23O_Calem") 	e->setCycSrim(srimBe);
  else if (frag == "26F") 		e->setCycSrim(srimBe);
  else if (frag == "23Oa")      	e->setCycSrim(srimBe);
  else if (frag == "22Oa") 		e->setCycSrim(srimBe);
  else if (frag == "20O") 		e->setCycSrim(srimBe);
  else if (frag == "13B") 		e->setCycSrim(srimBe);
  else if (frag == "25Ne") 		e->setCycSrim(srimBe);
  else if (frag == "16C") 		e->setCycSrim(srimBe);
  else if (frag == "19O") 		e->setCycSrim(srimBe);
  else if (frag == "z3n6") 		e->setCycSrim(srimBe);
  else if (frag == "z8n12") 		e->setCycSrim(srimBe);
  else if (frag == "11Li") 		e->setCycSrim(srimBe);
  else if (frag == "11Li_BeBeam") 	e->setCycSrim(srimBe);
  else if (frag == "14Be") 		e->setCycSrim(srimBe);
  else if (frag == "14Be_B17beam") 	e->setCycSrim(srimBe);
  else if (frag == "JENNA") 		e->setCycSrim(srimBe);
  else if (frag == "15B_06025") 	e->setCycSrim(srimBe);
  else if (frag == "24O_08026") 	e->setCycSrim(srimBe);
  else if (frag == "Ne26_05039") 	e->setCycSrim(srimBe);
  else if (frag == "He8_09067") {	e->setCycSrim(srimBe);
					e->setSrimFoil(Kapton);
				}
  else if (frag == "Be12_09067") 	e->setCycSrim(srimBe);
  else if (frag == "O24_09028") {
    	INFO("Using settings for fragment O24 from Schiller/Hope 09028 Experiment");
	e->fragMassExcess=18974.457/1000.;
	e->setCycSrim(srimBe);
  }
  else if (frag == "O23_09028") {
    	INFO("Using settings for fragment O24 from Schiller/Hope 09028 Experiment");
    	e->fragMassExcess=14616.373/1000.;
    	e->setCycSrim(srimBe);   
  }
  else if (frag == "O22_09028") {
    	INFO("Using settings for fragment O24 from Schiller/Hope 09028 Experiment");
    	e->fragMassExcess=9284.346/1000.;
    	e->setCycSrim(srimBe);
  }
  else if (frag == "Be12_09067")	e->setCycSrim(srimBe);

  else if (frag == "15B_06025") 	e->setCycSrim(srimBe);
  else if (frag == "14Be_06025") 	e->setCycSrim(srimBe);	
  else if (frag == "Li7_e11028") 	e->setCycSrim(srimBe); //ANK: 10/7/2015
  else if (frag == "He6_e11028") 	e->setCycSrim(srimBe); //ANK: 10/7/2015
  else if (frag == "11Be_e15091") 	e->setCycSrim(srimBe); // DV: 11/16/2017.
  else if (frag == "12B_e15091") 	e->setCycSrim(srimBe); // DV: 11/16/2017.
  else if (frag == "He8_11Be_e15091") 	e->setCycSrim(srimBe); // DV: 10/30/2017.
  else if (frag == "He8_12B_e15091") 	e->setCycSrim(srimBe); // DV: 10/30/2017.
  
  else ERR("Unknown fragment %s",frag.c_str());

  e->initTarget();
  INFO("Initilized Target");

  TFile f(outputFile.c_str(),"RECREATE");
  f.cd();
  TTree t(oTreeName.c_str(),"Analyzed Tree");
  
  // add branch
  DEB("Adding branch");
  t.Branch("event",&e);  

  //  loop over all events
  DEB("Looping over events");

  while( data.getEvent() ) { // as long as there are good events

    /// \todo for event mixing: make sure that valid events are used, where
    ///       fragment and neutron data is present!!!
    
    for (size_t i = 0; i <= nMix; i++) {
      long n1, n2; // event numbers
      n1 = data.getEventNum();  // event number event currently load in data

      /// \todo mix with random event or with previouse events???
      /// for now mix with previous events
      n2 = n1 - i;
      if ( n2 < 0 ) continue;  /// event number starts from zero

      if (nMix && n1 == n2) continue;  /// if mixing then don't "mix" the same event

      DEB("Mixing events %ld %ld",n1,n2);
      CalMonaEventSim data1, data2;  // data structure


      /// DON'T call data1(2).getEntry() !!! the branch addresses are set wrong!!!
      
      /// \todo better use circular buffer here to store the needed events!!!

      // fill data2 with "old" event
      data1 = data;
      data.getEvent(n2);
      data2 = data;

      DEB("Resetting");

      e->reset();

      DEB("Analyzing Frag from %ld, Mona from %ld",n1,n2);

      data.setFragPar(&data1);
      data.setMonaPar(&data2);
      e->manDESwitch = manESwitch;
      e->manDE = manDeltaE;
      e->ana(&data);   // analyzed data
      
      DEB("Filling");

      t.Fill();        // fill tree  \todo fill only if event is valied (returned from ana())
      
      if ( n1 == n2 ) continue;   // if events are the same (no mixing) then don't analyze again

      DEB("Resetting");
      e->reset();

      DEB("Analyzing Frag from %ld, MoNA from %ld",n2,n1);
      data.setFragPar(&data2);
      data.setMonaPar(&data1);
      e->ana(&data);   // analyzed data
      
      DEB("Filling");
      t.Fill();        // fill tree  \todo fill only if event is valied (returned from ana())
      
      DEB("Done");
    } 
  }

  f.Write();
  f.Close();
  fin.Close();
  return 0;
}

