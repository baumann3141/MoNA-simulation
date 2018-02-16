#ifndef MONA_EVENT_HH
#define MONA_EVENT_HH

#include <TTree.h>

#include "c_xdr_output_stream.hh"
#include <string>

/** Calibrated Mona Event class (used by Analysis class)
 *   all times are relative to the target time
 *   z-coordinates are relative to the target position
 *   coordinate system (looking downstream)
 *      x - left
 *      y - up
 *      z - downstream
 */
class CalMonaEvent {
private:

  double fTargetX, fTargetY;                          // target x, y and time

  double fMonaX, fMonaY, fMonaZ, fMonaTime;           // 1st neutron's x/y/z/t
  double fMonaX2, fMonaY2, fMonaZ2, fMonaTime2;       // 2nd neutron's x/y/z/t
  double fMonaX3, fMonaY3, fMonaZ3, fMonaTime3;	      // 3rd neutron's x/y/z/t
  double fMonaX4, fMonaY4, fMonaZ4, fMonaTime4;

  double array[2];
  //for(i=0;i<2;i++){
//	array[i] = 1.0;
//	}


  int    fMonaN1GMultiplicity;

  double fMonaN1G1X, fMonaN1G1Y, fMonaN1G1Z, fMonaN1G1Time, fMonaN1G1Edep, fMonaN1G1light;   	// 1st neutron run through Geant4 x/y/z/t/Edep
  double fMonaN1G2X, fMonaN1G2Y, fMonaN1G2Z, fMonaN1G2Time, fMonaN1G2Edep, fMonaN1G2light;   	// 1st neutron run through Geant4 x/y/z/t/Edep
  double fMonaN1G3X, fMonaN1G3Y, fMonaN1G3Z, fMonaN1G3Time, fMonaN1G3Edep, fMonaN1G3light;   	// 1st neutron run through Geant4 x/y/z/t/Edep
  double fMonaN1G4X, fMonaN1G4Y, fMonaN1G4Z, fMonaN1G4Time, fMonaN1G4Edep, fMonaN1G4light;   	// 1st neutron run through Geant4 x/y/z/t/Edep
  double fMonaN1G5X, fMonaN1G5Y, fMonaN1G5Z, fMonaN1G5Time, fMonaN1G5Edep, fMonaN1G5light;   	// 1st neutron run through Geant4 x/y/z/t/Edep
  double fMonaN1G6X, fMonaN1G6Y, fMonaN1G6Z, fMonaN1G6Time, fMonaN1G6Edep, fMonaN1G6light;   	// 1st neutron run through Geant4 x/y/z/t/Edep
  double fMonaN1G7X, fMonaN1G7Y, fMonaN1G7Z, fMonaN1G7Time, fMonaN1G7Edep, fMonaN1G7light;   	// 1st neutron run through Geant4 x/y/z/t/Edep
  double fMonaN1G8X, fMonaN1G8Y, fMonaN1G8Z, fMonaN1G8Time, fMonaN1G8Edep, fMonaN1G8light;   	// 1st neutron run through Geant4 x/y/z/t/Edep
  double fMonaN1G9X, fMonaN1G9Y, fMonaN1G9Z, fMonaN1G9Time, fMonaN1G9Edep, fMonaN1G9light;      // 1st neutron run through Geant4 x/y/z/t/Edep
  double fMonaN1G10X, fMonaN1G10Y, fMonaN1G10Z, fMonaN1G10Time, fMonaN1G10Edep, fMonaN1G10light; // 1st neutron run through Geant4 x/y/z/t/Edep

  int    fMonaN2GMultiplicity;
  double fMonaN2G1X, fMonaN2G1Y, fMonaN2G1Z, fMonaN2G1Time, fMonaN2G1Edep, fMonaN2G1light;   	// 2nd neutron run through Geant4 x/y/z/t/Edep
  double fMonaN2G2X, fMonaN2G2Y, fMonaN2G2Z, fMonaN2G2Time, fMonaN2G2Edep, fMonaN2G2light;   	// 2nd neutron run through Geant4 x/y/z/t/Edep
  double fMonaN2G3X, fMonaN2G3Y, fMonaN2G3Z, fMonaN2G3Time, fMonaN2G3Edep, fMonaN2G3light;   	// 2nd neutron run through Geant4 x/y/z/t/Edep
  double fMonaN2G4X, fMonaN2G4Y, fMonaN2G4Z, fMonaN2G4Time, fMonaN2G4Edep, fMonaN2G4light;   	// 2nd neutron run through Geant4 x/y/z/t/Edep
  double fMonaN2G5X, fMonaN2G5Y, fMonaN2G5Z, fMonaN2G5Time, fMonaN2G5Edep, fMonaN2G5light;   	// 2nd neutron run through Geant4 x/y/z/t/Edep
  double fMonaN2G6X, fMonaN2G6Y, fMonaN2G6Z, fMonaN2G6Time, fMonaN2G6Edep, fMonaN2G6light;   	// 2nd neutron run through Geant4 x/y/z/t/Edep
  double fMonaN2G7X, fMonaN2G7Y, fMonaN2G7Z, fMonaN2G7Time, fMonaN2G7Edep, fMonaN2G7light;   	// 2nd neutron run through Geant4 x/y/z/t/Edep
  double fMonaN2G8X, fMonaN2G8Y, fMonaN2G8Z, fMonaN2G8Time, fMonaN2G8Edep, fMonaN2G8light;   	// 2nd neutron run through Geant4 x/y/z/t/Edep
  double fMonaN2G9X, fMonaN2G9Y, fMonaN2G9Z, fMonaN2G9Time, fMonaN2G9Edep, fMonaN2G9light;      // 2nd neutron run through Geant4 x/y/z/t/Edep
  double fMonaN2G10X, fMonaN2G10Y, fMonaN2G10Z, fMonaN2G10Time, fMonaN2G10Edep, fMonaN2G10light; // 2nd neutron run through Geant4 x/y/z/t/Edep


  int    fMonaN3GMultiplicity;
  double fMonaN3G1X, fMonaN3G1Y, fMonaN3G1Z, fMonaN3G1Time, fMonaN3G1Edep, fMonaN3G1light;   	// 3rd neutron run through Geant4 x/y/z/t/Edep
  double fMonaN3G2X, fMonaN3G2Y, fMonaN3G2Z, fMonaN3G2Time, fMonaN3G2Edep, fMonaN3G2light;   	// ''  neutron run through Geant4 x/y/z/t/Edep
  double fMonaN3G3X, fMonaN3G3Y, fMonaN3G3Z, fMonaN3G3Time, fMonaN3G3Edep, fMonaN3G3light;   	//     neutron run through Geant4 x/y/z/t/Edep
  double fMonaN3G4X, fMonaN3G4Y, fMonaN3G4Z, fMonaN3G4Time, fMonaN3G4Edep, fMonaN3G4light;   	//     neutron run through Geant4 x/y/z/t/Edep
  double fMonaN3G5X, fMonaN3G5Y, fMonaN3G5Z, fMonaN3G5Time, fMonaN3G5Edep, fMonaN3G5light;   	//     neutron run through Geant4 x/y/z/t/Edep
  double fMonaN3G6X, fMonaN3G6Y, fMonaN3G6Z, fMonaN3G6Time, fMonaN3G6Edep, fMonaN3G6light;   	//     neutron run through Geant4 x/y/z/t/Edep
  double fMonaN3G7X, fMonaN3G7Y, fMonaN3G7Z, fMonaN3G7Time, fMonaN3G7Edep, fMonaN3G7light;   	//     neutron run through Geant4 x/y/z/t/Edep
  double fMonaN3G8X, fMonaN3G8Y, fMonaN3G8Z, fMonaN3G8Time, fMonaN3G8Edep, fMonaN3G8light;   	//     neutron run through Geant4 x/y/z/t/Edep
  double fMonaN3G9X, fMonaN3G9Y, fMonaN3G9Z, fMonaN3G9Time, fMonaN3G9Edep, fMonaN3G9light;      //     neutron run through Geant4 x/y/z/t/Edep
  double fMonaN3G10X, fMonaN3G10Y, fMonaN3G10Z, fMonaN3G10Time, fMonaN3G10Edep, fMonaN3G10light; //    neutron run through Geant4 x/y/z/t/Edep

  int    fMonaN4GMultiplicity;
  double fMonaN4G1X, fMonaN4G1Y, fMonaN4G1Z, fMonaN4G1Time, fMonaN4G1Edep, fMonaN4G1light;   	// 4th neutron run through Geant4 x/y/z/t/Edep
  double fMonaN4G2X, fMonaN4G2Y, fMonaN4G2Z, fMonaN4G2Time, fMonaN4G2Edep, fMonaN4G2light;   	// ''  neutron run through Geant4 x/y/z/t/Edep
  double fMonaN4G3X, fMonaN4G3Y, fMonaN4G3Z, fMonaN4G3Time, fMonaN4G3Edep, fMonaN4G3light;   	//     neutron run through Geant4 x/y/z/t/Edep
  double fMonaN4G4X, fMonaN4G4Y, fMonaN4G4Z, fMonaN4G4Time, fMonaN4G4Edep, fMonaN4G4light;   	//     neutron run through Geant4 x/y/z/t/Edep
  double fMonaN4G5X, fMonaN4G5Y, fMonaN4G5Z, fMonaN4G5Time, fMonaN4G5Edep, fMonaN4G5light;   	//     neutron run through Geant4 x/y/z/t/Edep
  double fMonaN4G6X, fMonaN4G6Y, fMonaN4G6Z, fMonaN4G6Time, fMonaN4G6Edep, fMonaN4G6light;   	//     neutron run through Geant4 x/y/z/t/Edep
  double fMonaN4G7X, fMonaN4G7Y, fMonaN4G7Z, fMonaN4G7Time, fMonaN4G7Edep, fMonaN4G7light;   	//     neutron run through Geant4 x/y/z/t/Edep
  double fMonaN4G8X, fMonaN4G8Y, fMonaN4G8Z, fMonaN4G8Time, fMonaN4G8Edep, fMonaN4G8light;   	//     neutron run through Geant4 x/y/z/t/Edep
  double fMonaN4G9X, fMonaN4G9Y, fMonaN4G9Z, fMonaN4G9Time, fMonaN4G9Edep, fMonaN4G9light;      //     neutron run through Geant4 x/y/z/t/Edep
  double fMonaN4G10X, fMonaN4G10Y, fMonaN4G10Z, fMonaN4G10Time, fMonaN4G10Edep, fMonaN4G10light; //    neutron run through Geant4 x/y/z/t/Edep





  double fMonaG1X, fMonaG1Y, fMonaG1Z, fMonaG1Time, fMonaG1Edep, fMonaG1light;   	// 1st Geant4 x/y/z/t/Edep
  double fMonaG2X, fMonaG2Y, fMonaG2Z, fMonaG2Time, fMonaG2Edep, fMonaG2light;   	// 1st Geant4 x/y/z/t/Edep
  double fMonaG3X, fMonaG3Y, fMonaG3Z, fMonaG3Time, fMonaG3Edep, fMonaG3light;   	// 1st Geant4 x/y/z/t/Edep
  double fMonaG4X, fMonaG4Y, fMonaG4Z, fMonaG4Time, fMonaG4Edep, fMonaG4light;   	// 1st Geant4 x/y/z/t/Edep
  double fMonaG5X, fMonaG5Y, fMonaG5Z, fMonaG5Time, fMonaG5Edep, fMonaG5light;   	// 1st Geant4 x/y/z/t/Edep
  double fMonaG6X, fMonaG6Y, fMonaG6Z, fMonaG6Time, fMonaG6Edep, fMonaG6light;   	// 1st Geant4 x/y/z/t/Edep
  double fMonaG7X, fMonaG7Y, fMonaG7Z, fMonaG7Time, fMonaG7Edep, fMonaG7light;   	// 1st Geant4 x/y/z/t/Edep
  double fMonaG8X, fMonaG8Y, fMonaG8Z, fMonaG8Time, fMonaG8Edep, fMonaG8light;   	// 1st Geant4 x/y/z/t/Edep
  double fMonaG9X, fMonaG9Y, fMonaG9Z, fMonaG9Time, fMonaG9Edep, fMonaG9light;      	// 1st Geant4 x/y/z/t/Edep
  double fMonaG10X, fMonaG10Y, fMonaG10Z, fMonaG10Time, fMonaG10Edep, fMonaG10light; 	// 1st Geant4 x/y/z/t/Edep
  double fMonaG11X, fMonaG11Y, fMonaG11Z, fMonaG11Time, fMonaG11Edep, fMonaG11light;   	// 1st Geant4 x/y/z/t/Edep
  double fMonaG12X, fMonaG12Y, fMonaG12Z, fMonaG12Time, fMonaG12Edep, fMonaG12light;   	// 1st Geant4 x/y/z/t/Edep
  double fMonaG13X, fMonaG13Y, fMonaG13Z, fMonaG13Time, fMonaG13Edep, fMonaG13light;   	// 1st Geant4 x/y/z/t/Edep
  double fMonaG14X, fMonaG14Y, fMonaG14Z, fMonaG14Time, fMonaG14Edep, fMonaG14light;   	// 1st Geant4 x/y/z/t/Edep
  double fMonaG15X, fMonaG15Y, fMonaG15Z, fMonaG15Time, fMonaG15Edep, fMonaG15light;   	// 1st Geant4 x/y/z/t/Edep
  double fMonaG16X, fMonaG16Y, fMonaG16Z, fMonaG16Time, fMonaG16Edep, fMonaG16light;   	// 1st Geant4 x/y/z/t/Edep
  double fMonaG17X, fMonaG17Y, fMonaG17Z, fMonaG17Time, fMonaG17Edep, fMonaG17light;   	// 1st Geant4 x/y/z/t/Edep
  double fMonaG18X, fMonaG18Y, fMonaG18Z, fMonaG18Time, fMonaG18Edep, fMonaG18light;   	// 1st Geant4 x/y/z/t/Edep
  double fMonaG19X, fMonaG19Y, fMonaG19Z, fMonaG19Time, fMonaG19Edep, fMonaG19light;    // 1st Geant4 x/y/z/t/Edep
  double fMonaG20X, fMonaG20Y, fMonaG20Z, fMonaG20Time, fMonaG20Edep, fMonaG20light; 	// 1st Geant4 x/y/z/t/Edep

  double fFragX, fFragTX, fFragY, fFragTY, fFragTime; // fragment x, theta_x, y, theta_y and time

public:
  double getMonaX()    { return fMonaX;    }
  double getMonaY()    { return fMonaY;    }
  double getMonaZ()    { return fMonaZ;    }
  double getMonaTime() { return fMonaTime; }

  double getMonaX2()    { return fMonaX2;    }
  double getMonaY2()    { return fMonaY2;    }
  double getMonaZ2()    { return fMonaZ2;    }
  double getMonaTime2() { return fMonaTime2; }

  double getMonaX3()    { return fMonaX3;    }
  double getMonaY3()    { return fMonaY3;    }
  double getMonaZ3()    { return fMonaZ3;    }
  double getMonaTime3() { return fMonaTime3; }
 
  double getMonaX4()    { return fMonaX4;    }
  double getMonaY4()    { return fMonaY4;    }
  double getMonaZ4()    { return fMonaZ4;    }
  double getMonaTime4() { return fMonaTime4; }




  int    getMonaN1GMultiplicity() { return fMonaN1GMultiplicity; }// N1
  double getMonaN1G1X()           { return fMonaN1G1X;           }
  double getMonaN1G1Y()           { return fMonaN1G1Y;           }
  double getMonaN1G1Z()           { return fMonaN1G1Z;           }
  double getMonaN1G1Time()        { return fMonaN1G1Time;        }
  double getMonaN1G1Edep()        { return fMonaN1G1Edep;        }
  double getMonaN1G1light()       { return fMonaN1G1light;       }
  double getMonaN1G2X()           { return fMonaN1G2X;           }
  double getMonaN1G2Y()           { return fMonaN1G2Y;           }
  double getMonaN1G2Z()           { return fMonaN1G2Z;           }
  double getMonaN1G2Time()        { return fMonaN1G2Time;        }
  double getMonaN1G2Edep()        { return fMonaN1G2Edep;        }
  double getMonaN1G2light()       { return fMonaN1G2light;       }
  double getMonaN1G3X()           { return fMonaN1G3X;           }
  double getMonaN1G3Y()           { return fMonaN1G3Y;           }
  double getMonaN1G3Z()           { return fMonaN1G3Z;           }
  double getMonaN1G3Time()        { return fMonaN1G3Time;        }
  double getMonaN1G3Edep()        { return fMonaN1G3Edep;        }
  double getMonaN1G3light()       { return fMonaN1G3light;       }
  double getMonaN1G4X()           { return fMonaN1G4X;           }
  double getMonaN1G4Y()           { return fMonaN1G4Y;           }
  double getMonaN1G4Z()           { return fMonaN1G4Z;           }
  double getMonaN1G4Time()        { return fMonaN1G4Time;        }
  double getMonaN1G4Edep()        { return fMonaN1G4Edep;        }
  double getMonaN1G4light()       { return fMonaN1G4light;       }
  double getMonaN1G5X()           { return fMonaN1G5X;           }
  double getMonaN1G5Y()           { return fMonaN1G5Y;           }
  double getMonaN1G5Z()           { return fMonaN1G5Z;           }
  double getMonaN1G5Time()        { return fMonaN1G5Time;        }
  double getMonaN1G5Edep()        { return fMonaN1G5Edep;        }
  double getMonaN1G5light()       { return fMonaN1G5light;       }
  double getMonaN1G6X()           { return fMonaN1G6X;           }
  double getMonaN1G6Y()           { return fMonaN1G6Y;           }
  double getMonaN1G6Z()           { return fMonaN1G6Z;           }
  double getMonaN1G6Time()        { return fMonaN1G6Time;        }
  double getMonaN1G6Edep()        { return fMonaN1G6Edep;        }
  double getMonaN1G6light()       { return fMonaN1G6light;       }
  double getMonaN1G7X()           { return fMonaN1G7X;           }
  double getMonaN1G7Y()           { return fMonaN1G7Y;           }
  double getMonaN1G7Z()           { return fMonaN1G7Z;           }
  double getMonaN1G7Time()        { return fMonaN1G7Time;        }
  double getMonaN1G7Edep()        { return fMonaN1G7Edep;        }
  double getMonaN1G7light()       { return fMonaN1G7light;       }
  double getMonaN1G8X()           { return fMonaN1G8X;           }
  double getMonaN1G8Y()           { return fMonaN1G8Y;           }
  double getMonaN1G8Z()           { return fMonaN1G8Z;           }
  double getMonaN1G8Time()        { return fMonaN1G8Time;        }
  double getMonaN1G8Edep()        { return fMonaN1G8Edep;        }
  double getMonaN1G8light()       { return fMonaN1G8light;       }
  double getMonaN1G9X()           { return fMonaN1G9X;           }
  double getMonaN1G9Y()           { return fMonaN1G9Y;           }
  double getMonaN1G9Z()           { return fMonaN1G9Z;           }
  double getMonaN1G9Time()        { return fMonaN1G9Time;        }
  double getMonaN1G9Edep()        { return fMonaN1G9Edep;        }
  double getMonaN1G9light()       { return fMonaN1G9light;       }
  double getMonaN1G10X()          { return fMonaN1G10X;          }
  double getMonaN1G10Y()          { return fMonaN1G10Y;          }
  double getMonaN1G10Z()          { return fMonaN1G10Z;          }
  double getMonaN1G10Time()       { return fMonaN1G10Time;       }
  double getMonaN1G10Edep()       { return fMonaN1G10Edep;       }
  double getMonaN1G10light()      { return fMonaN1G10light;      }

  int    getMonaN2GMultiplicity() { return fMonaN2GMultiplicity; }//N2
  double getMonaN2G1X()           { return fMonaN2G1X;           }
  double getMonaN2G1Y()           { return fMonaN2G1Y;           }
  double getMonaN2G1Z()           { return fMonaN2G1Z;           }
  double getMonaN2G1Time()        { return fMonaN2G1Time;        }
  double getMonaN2G1Edep()        { return fMonaN2G1Edep;        }
  double getMonaN2G1light()       { return fMonaN2G1light;       }
  double getMonaN2G2X()           { return fMonaN2G2X;           }
  double getMonaN2G2Y()           { return fMonaN2G2Y;           }
  double getMonaN2G2Z()           { return fMonaN2G2Z;           }
  double getMonaN2G2Time()        { return fMonaN2G2Time;        }
  double getMonaN2G2Edep()        { return fMonaN2G2Edep;        }
  double getMonaN2G2light()       { return fMonaN2G2light;       }
  double getMonaN2G3X()           { return fMonaN2G3X;           }
  double getMonaN2G3Y()           { return fMonaN2G3Y;           }
  double getMonaN2G3Z()           { return fMonaN2G3Z;           }
  double getMonaN2G3Time()        { return fMonaN2G3Time;        }
  double getMonaN2G3Edep()        { return fMonaN2G3Edep;        }
  double getMonaN2G3light()       { return fMonaN2G3light;       }
  double getMonaN2G4X()           { return fMonaN2G4X;           }
  double getMonaN2G4Y()           { return fMonaN2G4Y;           }
  double getMonaN2G4Z()           { return fMonaN2G4Z;           }
  double getMonaN2G4Time()        { return fMonaN2G4Time;        }
  double getMonaN2G4Edep()        { return fMonaN2G4Edep;        }
  double getMonaN2G4light()       { return fMonaN2G4light;       }
  double getMonaN2G5X()           { return fMonaN2G5X;           }
  double getMonaN2G5Y()           { return fMonaN2G5Y;           }
  double getMonaN2G5Z()           { return fMonaN2G5Z;           }
  double getMonaN2G5Time()        { return fMonaN2G5Time;        }
  double getMonaN2G5Edep()        { return fMonaN2G5Edep;        }
  double getMonaN2G5light()       { return fMonaN2G5light;       }
  double getMonaN2G6X()           { return fMonaN2G6X;           }
  double getMonaN2G6Y()           { return fMonaN2G6Y;           }
  double getMonaN2G6Z()           { return fMonaN2G6Z;           }
  double getMonaN2G6Time()        { return fMonaN2G6Time;        }
  double getMonaN2G6Edep()        { return fMonaN2G6Edep;        }
  double getMonaN2G6light()       { return fMonaN2G6light;       }
  double getMonaN2G7X()           { return fMonaN2G7X;           }
  double getMonaN2G7Y()           { return fMonaN2G7Y;           }
  double getMonaN2G7Z()           { return fMonaN2G7Z;           }
  double getMonaN2G7Time()        { return fMonaN2G7Time;        }
  double getMonaN2G7Edep()        { return fMonaN2G7Edep;        }
  double getMonaN2G7light()       { return fMonaN2G7light;       }
  double getMonaN2G8X()           { return fMonaN2G8X;           }
  double getMonaN2G8Y()           { return fMonaN2G8Y;           }
  double getMonaN2G8Z()           { return fMonaN2G8Z;           }
  double getMonaN2G8Time()        { return fMonaN2G8Time;        }
  double getMonaN2G8Edep()        { return fMonaN2G8Edep;        }
  double getMonaN2G8light()       { return fMonaN2G8light;       }
  double getMonaN2G9X()           { return fMonaN2G9X;           }
  double getMonaN2G9Y()           { return fMonaN2G9Y;           }
  double getMonaN2G9Z()           { return fMonaN2G9Z;           }
  double getMonaN2G9Time()        { return fMonaN2G9Time;        }
  double getMonaN2G9Edep()        { return fMonaN2G9Edep;        }
  double getMonaN2G9light()       { return fMonaN2G9light;       }
  double getMonaN2G10X()          { return fMonaN2G10X;          }
  double getMonaN2G10Y()          { return fMonaN2G10Y;          }
  double getMonaN2G10Z()          { return fMonaN2G10Z;          }
  double getMonaN2G10Time()       { return fMonaN2G10Time;       }
  double getMonaN2G10Edep()       { return fMonaN2G10Edep;       }
  double getMonaN2G10light()      { return fMonaN2G10light;      }

  int    getMonaN3GMultiplicity() { return fMonaN3GMultiplicity; }//N3
  double getMonaN3G1X()           { return fMonaN3G1X;           }
  double getMonaN3G1Y()           { return fMonaN3G1Y;           }
  double getMonaN3G1Z()           { return fMonaN3G1Z;           }
  double getMonaN3G1Time()        { return fMonaN3G1Time;        }
  double getMonaN3G1Edep()        { return fMonaN3G1Edep;        }
  double getMonaN3G1light()       { return fMonaN3G1light;       }
  double getMonaN3G2X()           { return fMonaN3G2X;           }
  double getMonaN3G2Y()           { return fMonaN3G2Y;           }
  double getMonaN3G2Z()           { return fMonaN3G2Z;           }
  double getMonaN3G2Time()        { return fMonaN3G2Time;        }
  double getMonaN3G2Edep()        { return fMonaN3G2Edep;        }
  double getMonaN3G2light()       { return fMonaN3G2light;       }
  double getMonaN3G3X()           { return fMonaN3G3X;           }
  double getMonaN3G3Y()           { return fMonaN3G3Y;           }
  double getMonaN3G3Z()           { return fMonaN3G3Z;           }
  double getMonaN3G3Time()        { return fMonaN3G3Time;        }
  double getMonaN3G3Edep()        { return fMonaN3G3Edep;        }
  double getMonaN3G3light()       { return fMonaN3G3light;       }
  double getMonaN3G4X()           { return fMonaN3G4X;           }
  double getMonaN3G4Y()           { return fMonaN3G4Y;           }
  double getMonaN3G4Z()           { return fMonaN3G4Z;           }
  double getMonaN3G4Time()        { return fMonaN3G4Time;        }
  double getMonaN3G4Edep()        { return fMonaN3G4Edep;        }
  double getMonaN3G4light()       { return fMonaN3G4light;       }
  double getMonaN3G5X()           { return fMonaN3G5X;           }
  double getMonaN3G5Y()           { return fMonaN3G5Y;           }
  double getMonaN3G5Z()           { return fMonaN3G5Z;           }
  double getMonaN3G5Time()        { return fMonaN3G5Time;        }
  double getMonaN3G5Edep()        { return fMonaN3G5Edep;        }
  double getMonaN3G5light()       { return fMonaN3G5light;       }
  double getMonaN3G6X()           { return fMonaN3G6X;           }
  double getMonaN3G6Y()           { return fMonaN3G6Y;           }
  double getMonaN3G6Z()           { return fMonaN3G6Z;           }
  double getMonaN3G6Time()        { return fMonaN3G6Time;        }
  double getMonaN3G6Edep()        { return fMonaN3G6Edep;        }
  double getMonaN3G6light()       { return fMonaN3G6light;       }
  double getMonaN3G7X()           { return fMonaN3G7X;           }
  double getMonaN3G7Y()           { return fMonaN3G7Y;           }
  double getMonaN3G7Z()           { return fMonaN3G7Z;           }
  double getMonaN3G7Time()        { return fMonaN3G7Time;        }
  double getMonaN3G7Edep()        { return fMonaN3G7Edep;        }
  double getMonaN3G7light()       { return fMonaN3G7light;       }
  double getMonaN3G8X()           { return fMonaN3G8X;           }
  double getMonaN3G8Y()           { return fMonaN3G8Y;           }
  double getMonaN3G8Z()           { return fMonaN3G8Z;           }
  double getMonaN3G8Time()        { return fMonaN3G8Time;        }
  double getMonaN3G8Edep()        { return fMonaN3G8Edep;        }
  double getMonaN3G8light()       { return fMonaN3G8light;       }
  double getMonaN3G9X()           { return fMonaN3G9X;           }
  double getMonaN3G9Y()           { return fMonaN3G9Y;           }
  double getMonaN3G9Z()           { return fMonaN3G9Z;           }
  double getMonaN3G9Time()        { return fMonaN3G9Time;        }
  double getMonaN3G9Edep()        { return fMonaN3G9Edep;        }
  double getMonaN3G9light()       { return fMonaN3G9light;       }
  double getMonaN3G10X()          { return fMonaN3G10X;          }
  double getMonaN3G10Y()          { return fMonaN3G10Y;          }
  double getMonaN3G10Z()          { return fMonaN3G10Z;          }
  double getMonaN3G10Time()       { return fMonaN3G10Time;       }
  double getMonaN3G10Edep()       { return fMonaN3G10Edep;       }
  double getMonaN3G10light()      { return fMonaN3G10light;      }// End N3

  int    getMonaN4GMultiplicity() { return fMonaN4GMultiplicity; }//N4
  double getMonaN4G1X()           { return fMonaN4G1X;           }
  double getMonaN4G1Y()           { return fMonaN4G1Y;           }
  double getMonaN4G1Z()           { return fMonaN4G1Z;           }
  double getMonaN4G1Time()        { return fMonaN4G1Time;        }
  double getMonaN4G1Edep()        { return fMonaN4G1Edep;        }
  double getMonaN4G1light()       { return fMonaN4G1light;       }
  double getMonaN4G2X()           { return fMonaN4G2X;           }
  double getMonaN4G2Y()           { return fMonaN4G2Y;           }
  double getMonaN4G2Z()           { return fMonaN4G2Z;           }
  double getMonaN4G2Time()        { return fMonaN4G2Time;        }
  double getMonaN4G2Edep()        { return fMonaN4G2Edep;        }
  double getMonaN4G2light()       { return fMonaN4G2light;       }
  double getMonaN4G3X()           { return fMonaN4G3X;           }
  double getMonaN4G3Y()           { return fMonaN4G3Y;           }
  double getMonaN4G3Z()           { return fMonaN4G3Z;           }
  double getMonaN4G3Time()        { return fMonaN4G3Time;        }
  double getMonaN4G3Edep()        { return fMonaN4G3Edep;        }
  double getMonaN4G3light()       { return fMonaN4G3light;       }
  double getMonaN4G4X()           { return fMonaN4G4X;           }
  double getMonaN4G4Y()           { return fMonaN4G4Y;           }
  double getMonaN4G4Z()           { return fMonaN4G4Z;           }
  double getMonaN4G4Time()        { return fMonaN4G4Time;        }
  double getMonaN4G4Edep()        { return fMonaN4G4Edep;        }
  double getMonaN4G4light()       { return fMonaN4G4light;       }
  double getMonaN4G5X()           { return fMonaN4G5X;           }
  double getMonaN4G5Y()           { return fMonaN4G5Y;           }
  double getMonaN4G5Z()           { return fMonaN4G5Z;           }
  double getMonaN4G5Time()        { return fMonaN4G5Time;        }
  double getMonaN4G5Edep()        { return fMonaN4G5Edep;        }
  double getMonaN4G5light()       { return fMonaN4G5light;       }
  double getMonaN4G6X()           { return fMonaN4G6X;           }
  double getMonaN4G6Y()           { return fMonaN4G6Y;           }
  double getMonaN4G6Z()           { return fMonaN4G6Z;           }
  double getMonaN4G6Time()        { return fMonaN4G6Time;        }
  double getMonaN4G6Edep()        { return fMonaN4G6Edep;        }
  double getMonaN4G6light()       { return fMonaN4G6light;       }
  double getMonaN4G7X()           { return fMonaN4G7X;           }
  double getMonaN4G7Y()           { return fMonaN4G7Y;           }
  double getMonaN4G7Z()           { return fMonaN4G7Z;           }
  double getMonaN4G7Time()        { return fMonaN4G7Time;        }
  double getMonaN4G7Edep()        { return fMonaN4G7Edep;        }
  double getMonaN4G7light()       { return fMonaN4G7light;       }
  double getMonaN4G8X()           { return fMonaN4G8X;           }
  double getMonaN4G8Y()           { return fMonaN4G8Y;           }
  double getMonaN4G8Z()           { return fMonaN4G8Z;           }
  double getMonaN4G8Time()        { return fMonaN4G8Time;        }
  double getMonaN4G8Edep()        { return fMonaN4G8Edep;        }
  double getMonaN4G8light()       { return fMonaN4G8light;       }
  double getMonaN4G9X()           { return fMonaN4G9X;           }
  double getMonaN4G9Y()           { return fMonaN4G9Y;           }
  double getMonaN4G9Z()           { return fMonaN4G9Z;           }
  double getMonaN4G9Time()        { return fMonaN4G9Time;        }
  double getMonaN4G9Edep()        { return fMonaN4G9Edep;        }
  double getMonaN4G9light()       { return fMonaN4G9light;       }
  double getMonaN4G10X()          { return fMonaN4G10X;          }
  double getMonaN4G10Y()          { return fMonaN4G10Y;          }
  double getMonaN4G10Z()          { return fMonaN4G10Z;          }
  double getMonaN4G10Time()       { return fMonaN4G10Time;       }
  double getMonaN4G10Edep()       { return fMonaN4G10Edep;       }
  double getMonaN4G10light()      { return fMonaN4G10light;      }// End N4


  double getMonaG1X()             { return fMonaG1X;             }
  double getMonaG1Y()             { return fMonaG1Y;             }
  double getMonaG1Z()             { return fMonaG1Z;             }
  double getMonaG1Time()          { return fMonaG1Time;          }
  double getMonaG1Edep()          { return fMonaG1Edep;          }
  double getMonaG1light()         { return fMonaG1light;         }
  double getMonaG2X()             { return fMonaG2X;             }
  double getMonaG2Y()             { return fMonaG2Y;             }
  double getMonaG2Z()             { return fMonaG2Z;             }
  double getMonaG2Time()          { return fMonaG2Time;          }
  double getMonaG2Edep()          { return fMonaG2Edep;          }
  double getMonaG2light()         { return fMonaG2light;         }
  double getMonaG3X()             { return fMonaG3X;             }
  double getMonaG3Y()             { return fMonaG3Y;             }
  double getMonaG3Z()             { return fMonaG3Z;             }
  double getMonaG3Time()          { return fMonaG3Time;          }
  double getMonaG3Edep()          { return fMonaG3Edep;          }
  double getMonaG3light()         { return fMonaG3light;         }
  double getMonaG4X()             { return fMonaG4X;             }
  double getMonaG4Y()             { return fMonaG4Y;             }
  double getMonaG4Z()             { return fMonaG4Z;             }
  double getMonaG4Time()          { return fMonaG4Time;          }
  double getMonaG4Edep()          { return fMonaG4Edep;          }
  double getMonaG4light()         { return fMonaG4light;         }
  double getMonaG5X()             { return fMonaG5X;             }
  double getMonaG5Y()             { return fMonaG5Y;             }
  double getMonaG5Z()             { return fMonaG5Z;             }
  double getMonaG5Time()          { return fMonaG5Time;          }
  double getMonaG5Edep()          { return fMonaG5Edep;          }
  double getMonaG5light()         { return fMonaG5light;         }
  double getMonaG6X()             { return fMonaG6X;             }
  double getMonaG6Y()             { return fMonaG6Y;             }
  double getMonaG6Z()             { return fMonaG6Z;             }
  double getMonaG6Time()          { return fMonaG6Time;          }
  double getMonaG6Edep()          { return fMonaG6Edep;          }
  double getMonaG6light()         { return fMonaG6light;         }
  double getMonaG7X()             { return fMonaG7X;             }
  double getMonaG7Y()             { return fMonaG7Y;             }
  double getMonaG7Z()             { return fMonaG7Z;             }
  double getMonaG7Time()          { return fMonaG7Time;          }
  double getMonaG7Edep()          { return fMonaG7Edep;          }
  double getMonaG7light()         { return fMonaG7light;         }
  double getMonaG8X()             { return fMonaG8X;             }
  double getMonaG8Y()             { return fMonaG8Y;             }
  double getMonaG8Z()             { return fMonaG8Z;             }
  double getMonaG8Time()          { return fMonaG8Time;          }
  double getMonaG8Edep()          { return fMonaG8Edep;          }
  double getMonaG8light()         { return fMonaG8light;         }
  double getMonaG9X()             { return fMonaG9X;             }
  double getMonaG9Y()             { return fMonaG9Y;             }
  double getMonaG9Z()             { return fMonaG9Z;             }
  double getMonaG9Time()          { return fMonaG9Time;          }
  double getMonaG9Edep()          { return fMonaG9Edep;          }
  double getMonaG9light()         { return fMonaG9light;         }
  double getMonaG10X()            { return fMonaG10X;            }
  double getMonaG10Y()            { return fMonaG10Y;            }
  double getMonaG10Z()            { return fMonaG10Z;            }
  double getMonaG10Time()         { return fMonaG10Time;         }
  double getMonaG10Edep()         { return fMonaG10Edep;         }
  double getMonaG10light()        { return fMonaG10light;        }
  double getMonaG11X()            { return fMonaG11X;            }
  double getMonaG11Y()            { return fMonaG11Y;            }
  double getMonaG11Z()            { return fMonaG11Z;            }
  double getMonaG11Time()         { return fMonaG11Time;         }
  double getMonaG11Edep()         { return fMonaG11Edep;         }
  double getMonaG11light()        { return fMonaG11light;        }
  double getMonaG12X()            { return fMonaG12X;            }
  double getMonaG12Y()            { return fMonaG12Y;            }
  double getMonaG12Z()            { return fMonaG12Z;            }
  double getMonaG12Time()         { return fMonaG12Time;         }
  double getMonaG12Edep()         { return fMonaG12Edep;         }
  double getMonaG12light()        { return fMonaG12light;        }
  double getMonaG13X()            { return fMonaG13X;            }
  double getMonaG13Y()            { return fMonaG13Y;            }
  double getMonaG13Z()            { return fMonaG13Z;            }
  double getMonaG13Time()         { return fMonaG13Time;         }
  double getMonaG13Edep()         { return fMonaG13Edep;         }
  double getMonaG13light()        { return fMonaG13light;        }
  double getMonaG14X()            { return fMonaG14X;            }
  double getMonaG14Y()            { return fMonaG14Y;            }
  double getMonaG14Z()            { return fMonaG14Z;            }
  double getMonaG14Time()         { return fMonaG14Time;         }
  double getMonaG14Edep()         { return fMonaG14Edep;         }
  double getMonaG14light()        { return fMonaG14light;        }
  double getMonaG15X()            { return fMonaG15X;            }
  double getMonaG15Y()            { return fMonaG15Y;            }
  double getMonaG15Z()            { return fMonaG15Z;            }
  double getMonaG15Time()         { return fMonaG15Time;         }
  double getMonaG15Edep()         { return fMonaG15Edep;         }
  double getMonaG15light()        { return fMonaG15light;        }
  double getMonaG16X()            { return fMonaG16X;            }
  double getMonaG16Y()            { return fMonaG16Y;            }
  double getMonaG16Z()            { return fMonaG16Z;            }
  double getMonaG16Time()         { return fMonaG16Time;         }
  double getMonaG16Edep()         { return fMonaG16Edep;         }
  double getMonaG16light()        { return fMonaG16light;        }
  double getMonaG17X()            { return fMonaG17X;            }
  double getMonaG17Y()            { return fMonaG17Y;            }
  double getMonaG17Z()            { return fMonaG17Z;            }
  double getMonaG17Time()         { return fMonaG17Time;         }
  double getMonaG17Edep()         { return fMonaG17Edep;         }
  double getMonaG17light()        { return fMonaG17light;        }
  double getMonaG18X()            { return fMonaG18X;            }
  double getMonaG18Y()            { return fMonaG18Y;            }
  double getMonaG18Z()            { return fMonaG18Z;            }
  double getMonaG18Time()         { return fMonaG18Time;         }
  double getMonaG18Edep()         { return fMonaG18Edep;         }
  double getMonaG18light()        { return fMonaG18light;        }
  double getMonaG19X()            { return fMonaG19X;            }
  double getMonaG19Y()            { return fMonaG19Y;            }
  double getMonaG19Z()            { return fMonaG19Z;            }
  double getMonaG19Time()         { return fMonaG19Time;         }
  double getMonaG19Edep()         { return fMonaG19Edep;         }
  double getMonaG19light()        { return fMonaG19light;        }
  double getMonaG20X()            { return fMonaG20X;            }
  double getMonaG20Y()            { return fMonaG20Y;            }
  double getMonaG20Z()            { return fMonaG20Z;            }
  double getMonaG20Time()         { return fMonaG20Time;         }
  double getMonaG20Edep()         { return fMonaG20Edep;         }
  double getMonaG20light()        { return fMonaG20light;        }

  double getFragX()    { return fFragX;    }
  double getFragTX()   { return fFragTX;   }
  double getFragY()    { return fFragY;    } 
  double getFragTY()   { return fFragTY;   }
  double getFragTime() { return fFragTime; }

  double getTargetX()  { return fTargetX;  }
  double getTargetY()  { return fTargetY;  } 

public:
  /// set parameters of Fragment according to *event
  virtual void setFragPar(CalMonaEvent* event);

  /// set parameters of Mona according to *event
  virtual void setMonaPar(CalMonaEvent* event);

protected:
  void setMonaX(double v)    { fMonaX = v;    }
  void setMonaY(double v)    { fMonaY= v;     }
  void setMonaZ(double v)    { fMonaZ = v;    }
  void setMonaTime(double v) { fMonaTime = v; }

  void setMonaX2(double v)    { fMonaX2 = v;    }
  void setMonaY2(double v)    { fMonaY2 = v;    }
  void setMonaZ2(double v)    { fMonaZ2 = v;    }
  void setMonaTime2(double v) { fMonaTime2 = v; }

  void setMonaX3(double v)    { fMonaX3 = v;    }
  void setMonaY3(double v)    { fMonaY3 = v;    }
  void setMonaZ3(double v)    { fMonaZ3 = v;    }
  void setMonaTime3(double v) { fMonaTime3 = v; }

  void setMonaX4(double v)    { fMonaX4 = v;    }
  void setMonaY4(double v)    { fMonaY4 = v;    }
  void setMonaZ4(double v)    { fMonaZ4 = v;    }
  void setMonaTime4(double v) { fMonaTime4 = v; }




  void setMonaN1GMultiplicity(int v)    { fMonaN1GMultiplicity = v;  }// N1
  void setMonaN1G1X(double v)           { fMonaN1G1X = v;            }
  void setMonaN1G1Y(double v)           { fMonaN1G1Y= v;             }
  void setMonaN1G1Z(double v)           { fMonaN1G1Z = v;            }
  void setMonaN1G1Time(double v)        { fMonaN1G1Time = v;         }
  void setMonaN1G1Edep(double v)        { fMonaN1G1Edep = v;         }
  void setMonaN1G1light(double v)       { fMonaN1G1light = v;        }
  void setMonaN1G2X(double v)           { fMonaN1G2X = v;            }
  void setMonaN1G2Y(double v)           { fMonaN1G2Y= v; 	     }
  void setMonaN1G2Z(double v)           { fMonaN1G2Z = v;	     }
  void setMonaN1G2Time(double v)        { fMonaN1G2Time = v;         }
  void setMonaN1G2Edep(double v)        { fMonaN1G2Edep = v;         }
  void setMonaN1G2light(double v)       { fMonaN1G2light = v;        }
  void setMonaN1G3X(double v)           { fMonaN1G3X = v;	     }
  void setMonaN1G3Y(double v)           { fMonaN1G3Y= v; 	     }
  void setMonaN1G3Z(double v)           { fMonaN1G3Z = v;	     }
  void setMonaN1G3Time(double v)        { fMonaN1G3Time = v;         }
  void setMonaN1G3Edep(double v)        { fMonaN1G3Edep = v;         }
  void setMonaN1G3light(double v)       { fMonaN1G3light = v;        }
  void setMonaN1G4X(double v)           { fMonaN1G4X = v;	     }
  void setMonaN1G4Y(double v)           { fMonaN1G4Y= v; 	     }
  void setMonaN1G4Z(double v)           { fMonaN1G4Z = v;	     }
  void setMonaN1G4Time(double v)        { fMonaN1G4Time = v;         }
  void setMonaN1G4Edep(double v)        { fMonaN1G4Edep = v;         }
  void setMonaN1G4light(double v)       { fMonaN1G4light = v;        }
  void setMonaN1G5X(double v)           { fMonaN1G5X = v;	     }
  void setMonaN1G5Y(double v)           { fMonaN1G5Y= v; 	     }
  void setMonaN1G5Z(double v)           { fMonaN1G5Z = v;	     }
  void setMonaN1G5Time(double v)        { fMonaN1G5Time = v;         }
  void setMonaN1G5Edep(double v)        { fMonaN1G5Edep = v;         }
  void setMonaN1G5light(double v)       { fMonaN1G5light = v;        }
  void setMonaN1G6X(double v)           { fMonaN1G6X = v;	     }
  void setMonaN1G6Y(double v)           { fMonaN1G6Y= v; 	     }
  void setMonaN1G6Z(double v)           { fMonaN1G6Z = v;	     }
  void setMonaN1G6Time(double v)        { fMonaN1G6Time = v;         }
  void setMonaN1G6Edep(double v)        { fMonaN1G6Edep = v;         }
  void setMonaN1G6light(double v)       { fMonaN1G6light = v;        }
  void setMonaN1G7X(double v)           { fMonaN1G7X = v;	     }
  void setMonaN1G7Y(double v)           { fMonaN1G7Y= v; 	     }
  void setMonaN1G7Z(double v)           { fMonaN1G7Z = v;	     }
  void setMonaN1G7Time(double v)        { fMonaN1G7Time = v;         }
  void setMonaN1G7Edep(double v)        { fMonaN1G7Edep = v;         }
  void setMonaN1G7light(double v)       { fMonaN1G7light = v;        }
  void setMonaN1G8X(double v)           { fMonaN1G8X = v;	     }
  void setMonaN1G8Y(double v)           { fMonaN1G8Y= v; 	     }
  void setMonaN1G8Z(double v)           { fMonaN1G8Z = v;	     }
  void setMonaN1G8Time(double v)        { fMonaN1G8Time = v;         }
  void setMonaN1G8Edep(double v)        { fMonaN1G8Edep = v;         }
  void setMonaN1G8light(double v)       { fMonaN1G8light = v;        }
  void setMonaN1G9X(double v)           { fMonaN1G9X = v;	     }
  void setMonaN1G9Y(double v)           { fMonaN1G9Y= v; 	     }
  void setMonaN1G9Z(double v)           { fMonaN1G9Z = v;	     }
  void setMonaN1G9Time(double v)        { fMonaN1G9Time = v;         }
  void setMonaN1G9Edep(double v)        { fMonaN1G9Edep = v;         }
  void setMonaN1G9light(double v)       { fMonaN1G9light = v;        }
  void setMonaN1G10X(double v)          { fMonaN1G10X = v;	     }
  void setMonaN1G10Y(double v)          { fMonaN1G10Y= v; 	     }
  void setMonaN1G10Z(double v)          { fMonaN1G10Z = v;	     }
  void setMonaN1G10Time(double v)       { fMonaN1G10Time = v;	     }
  void setMonaN1G10Edep(double v)       { fMonaN1G10Edep = v;        }
  void setMonaN1G10light(double v)      { fMonaN1G10light = v;       }// End N1

  void setMonaN2GMultiplicity(int v)    { fMonaN2GMultiplicity = v;  }// N2
  void setMonaN2G1X(double v)           { fMonaN2G1X = v;            }
  void setMonaN2G1Y(double v)           { fMonaN2G1Y= v;             }
  void setMonaN2G1Z(double v)           { fMonaN2G1Z = v;            }
  void setMonaN2G1Time(double v)        { fMonaN2G1Time = v;         }
  void setMonaN2G1Edep(double v)        { fMonaN2G1Edep = v;         }
  void setMonaN2G1light(double v)       { fMonaN2G1light = v;        }
  void setMonaN2G2X(double v)           { fMonaN2G2X = v;            }
  void setMonaN2G2Y(double v)           { fMonaN2G2Y= v; 	     }
  void setMonaN2G2Z(double v)           { fMonaN2G2Z = v;	     }
  void setMonaN2G2Time(double v)        { fMonaN2G2Time = v;         }
  void setMonaN2G2Edep(double v)        { fMonaN2G2Edep = v;         }
  void setMonaN2G2light(double v)       { fMonaN2G2light = v;        }
  void setMonaN2G3X(double v)           { fMonaN2G3X = v;	     }
  void setMonaN2G3Y(double v)           { fMonaN2G3Y= v; 	     }
  void setMonaN2G3Z(double v)           { fMonaN2G3Z = v;	     }
  void setMonaN2G3Time(double v)        { fMonaN2G3Time = v;         }
  void setMonaN2G3Edep(double v)        { fMonaN2G3Edep = v;         }
  void setMonaN2G3light(double v)       { fMonaN2G3light = v;        }
  void setMonaN2G4X(double v)           { fMonaN2G4X = v;	     }
  void setMonaN2G4Y(double v)           { fMonaN2G4Y= v; 	     }
  void setMonaN2G4Z(double v)           { fMonaN2G4Z = v;	     }
  void setMonaN2G4Time(double v)        { fMonaN2G4Time = v;         }
  void setMonaN2G4Edep(double v)        { fMonaN2G4Edep = v;         }
  void setMonaN2G4light(double v)       { fMonaN2G4light = v;        }
  void setMonaN2G5X(double v)           { fMonaN2G5X = v;	     }  
  void setMonaN2G5Y(double v)           { fMonaN2G5Y= v; 	     }
  void setMonaN2G5Z(double v)           { fMonaN2G5Z = v;	     }  
  void setMonaN2G5Time(double v)        { fMonaN2G5Time = v;         }
  void setMonaN2G5Edep(double v)        { fMonaN2G5Edep = v;         } 
  void setMonaN2G5light(double v)       { fMonaN2G5light = v;        }
  void setMonaN2G6X(double v)           { fMonaN2G6X = v;	     }  
  void setMonaN2G6Y(double v)           { fMonaN2G6Y= v; 	     }
  void setMonaN2G6Z(double v)           { fMonaN2G6Z = v;	     }
  void setMonaN2G6Time(double v)        { fMonaN2G6Time = v;         } 
  void setMonaN2G6Edep(double v)        { fMonaN2G6Edep = v;         }
  void setMonaN2G6light(double v)       { fMonaN2G6light = v;        }
  void setMonaN2G7X(double v)           { fMonaN2G7X = v;	     }
  void setMonaN2G7Y(double v)           { fMonaN2G7Y= v; 	     }
  void setMonaN2G7Z(double v)           { fMonaN2G7Z = v;	     }
  void setMonaN2G7Time(double v)        { fMonaN2G7Time = v;         }
  void setMonaN2G7Edep(double v)        { fMonaN2G7Edep = v;         }
  void setMonaN2G7light(double v)       { fMonaN2G7light = v;        }
  void setMonaN2G8X(double v)           { fMonaN2G8X = v;	     }
  void setMonaN2G8Y(double v)           { fMonaN2G8Y= v; 	     }
  void setMonaN2G8Z(double v)           { fMonaN2G8Z = v;	     }
  void setMonaN2G8Time(double v)        { fMonaN2G8Time = v;         }
  void setMonaN2G8Edep(double v)        { fMonaN2G8Edep = v;         }
  void setMonaN2G8light(double v)       { fMonaN2G8light = v;        }
  void setMonaN2G9X(double v)           { fMonaN2G9X = v;	     }
  void setMonaN2G9Y(double v)           { fMonaN2G9Y= v; 	     }
  void setMonaN2G9Z(double v)           { fMonaN2G9Z = v;	     }
  void setMonaN2G9Time(double v)        { fMonaN2G9Time = v;         }
  void setMonaN2G9Edep(double v)        { fMonaN2G9Edep = v;         }
  void setMonaN2G9light(double v)       { fMonaN2G9light = v;        }
  void setMonaN2G10X(double v)          { fMonaN2G10X = v;	     }
  void setMonaN2G10Y(double v)          { fMonaN2G10Y= v; 	     }
  void setMonaN2G10Z(double v)          { fMonaN2G10Z = v;	     }
  void setMonaN2G10Time(double v)       { fMonaN2G10Time = v;	     }
  void setMonaN2G10Edep(double v)       { fMonaN2G10Edep = v;        }
  void setMonaN2G10light(double v)      { fMonaN2G10light = v;       }// End N2

  void setMonaN3GMultiplicity(int v)    { fMonaN3GMultiplicity = v;  }// N3
  void setMonaN3G1X(double v)           { fMonaN3G1X = v;            }
  void setMonaN3G1Y(double v)           { fMonaN3G1Y= v;             }
  void setMonaN3G1Z(double v)           { fMonaN3G1Z = v;            }
  void setMonaN3G1Time(double v)        { fMonaN3G1Time = v;         }
  void setMonaN3G1Edep(double v)        { fMonaN3G1Edep = v;         }
  void setMonaN3G1light(double v)       { fMonaN3G1light = v;        }
  void setMonaN3G2X(double v)           { fMonaN3G2X = v;            }
  void setMonaN3G2Y(double v)           { fMonaN3G2Y= v; 	     }
  void setMonaN3G2Z(double v)           { fMonaN3G2Z = v;	     }
  void setMonaN3G2Time(double v)        { fMonaN3G2Time = v;         }
  void setMonaN3G2Edep(double v)        { fMonaN3G2Edep = v;         }
  void setMonaN3G2light(double v)       { fMonaN3G2light = v;        }
  void setMonaN3G3X(double v)           { fMonaN3G3X = v;	     }
  void setMonaN3G3Y(double v)           { fMonaN3G3Y= v; 	     }
  void setMonaN3G3Z(double v)           { fMonaN3G3Z = v;	     }
  void setMonaN3G3Time(double v)        { fMonaN3G3Time = v;         }
  void setMonaN3G3Edep(double v)        { fMonaN3G3Edep = v;         }
  void setMonaN3G3light(double v)       { fMonaN3G3light = v;        }
  void setMonaN3G4X(double v)           { fMonaN3G4X = v;	     }
  void setMonaN3G4Y(double v)           { fMonaN3G4Y= v; 	     }
  void setMonaN3G4Z(double v)           { fMonaN3G4Z = v;	     }
  void setMonaN3G4Time(double v)        { fMonaN3G4Time = v;         }
  void setMonaN3G4Edep(double v)        { fMonaN3G4Edep = v;         }
  void setMonaN3G4light(double v)       { fMonaN3G4light = v;        }
  void setMonaN3G5X(double v)           { fMonaN3G5X = v;	     }  
  void setMonaN3G5Y(double v)           { fMonaN3G5Y= v; 	     }
  void setMonaN3G5Z(double v)           { fMonaN3G5Z = v;	     }  
  void setMonaN3G5Time(double v)        { fMonaN3G5Time = v;         }
  void setMonaN3G5Edep(double v)        { fMonaN3G5Edep = v;         } 
  void setMonaN3G5light(double v)       { fMonaN3G5light = v;        }
  void setMonaN3G6X(double v)           { fMonaN3G6X = v;	     }  
  void setMonaN3G6Y(double v)           { fMonaN3G6Y= v; 	     }
  void setMonaN3G6Z(double v)           { fMonaN3G6Z = v;	     }
  void setMonaN3G6Time(double v)        { fMonaN3G6Time = v;         } 
  void setMonaN3G6Edep(double v)        { fMonaN3G6Edep = v;         }
  void setMonaN3G6light(double v)       { fMonaN3G6light = v;        }
  void setMonaN3G7X(double v)           { fMonaN3G7X = v;	     }
  void setMonaN3G7Y(double v)           { fMonaN3G7Y= v; 	     }
  void setMonaN3G7Z(double v)           { fMonaN3G7Z = v;	     }
  void setMonaN3G7Time(double v)        { fMonaN3G7Time = v;         }
  void setMonaN3G7Edep(double v)        { fMonaN3G7Edep = v;         }
  void setMonaN3G7light(double v)       { fMonaN3G7light = v;        }
  void setMonaN3G8X(double v)           { fMonaN3G8X = v;	     }
  void setMonaN3G8Y(double v)           { fMonaN3G8Y= v; 	     }
  void setMonaN3G8Z(double v)           { fMonaN3G8Z = v;	     }
  void setMonaN3G8Time(double v)        { fMonaN3G8Time = v;         }
  void setMonaN3G8Edep(double v)        { fMonaN3G8Edep = v;         }
  void setMonaN3G8light(double v)       { fMonaN3G8light = v;        }
  void setMonaN3G9X(double v)           { fMonaN3G9X = v;	     }
  void setMonaN3G9Y(double v)           { fMonaN3G9Y= v; 	     }
  void setMonaN3G9Z(double v)           { fMonaN3G9Z = v;	     }
  void setMonaN3G9Time(double v)        { fMonaN3G9Time = v;         }
  void setMonaN3G9Edep(double v)        { fMonaN3G9Edep = v;         }
  void setMonaN3G9light(double v)       { fMonaN3G9light = v;        }
  void setMonaN3G10X(double v)          { fMonaN3G10X = v;	     }
  void setMonaN3G10Y(double v)          { fMonaN3G10Y= v; 	     }
  void setMonaN3G10Z(double v)          { fMonaN3G10Z = v;	     }
  void setMonaN3G10Time(double v)       { fMonaN3G10Time = v;	     }
  void setMonaN3G10Edep(double v)       { fMonaN3G10Edep = v;        }
  void setMonaN3G10light(double v)      { fMonaN3G10light = v;       }// End N3

  void setMonaN4GMultiplicity(int v)    { fMonaN4GMultiplicity = v;  }// N4
  void setMonaN4G1X(double v)           { fMonaN4G1X = v;            }
  void setMonaN4G1Y(double v)           { fMonaN4G1Y= v;             }
  void setMonaN4G1Z(double v)           { fMonaN4G1Z = v;            }
  void setMonaN4G1Time(double v)        { fMonaN4G1Time = v;         }
  void setMonaN4G1Edep(double v)        { fMonaN4G1Edep = v;         }
  void setMonaN4G1light(double v)       { fMonaN4G1light = v;        }
  void setMonaN4G2X(double v)           { fMonaN4G2X = v;            }
  void setMonaN4G2Y(double v)           { fMonaN4G2Y= v; 	     }
  void setMonaN4G2Z(double v)           { fMonaN4G2Z = v;	     }
  void setMonaN4G2Time(double v)        { fMonaN4G2Time = v;         }
  void setMonaN4G2Edep(double v)        { fMonaN4G2Edep = v;         }
  void setMonaN4G2light(double v)       { fMonaN4G2light = v;        }
  void setMonaN4G3X(double v)           { fMonaN4G3X = v;	     }
  void setMonaN4G3Y(double v)           { fMonaN4G3Y= v; 	     }
  void setMonaN4G3Z(double v)           { fMonaN4G3Z = v;	     }
  void setMonaN4G3Time(double v)        { fMonaN4G3Time = v;         }
  void setMonaN4G3Edep(double v)        { fMonaN4G3Edep = v;         }
  void setMonaN4G3light(double v)       { fMonaN4G3light = v;        }
  void setMonaN4G4X(double v)           { fMonaN4G4X = v;	     }
  void setMonaN4G4Y(double v)           { fMonaN4G4Y= v; 	     }
  void setMonaN4G4Z(double v)           { fMonaN4G4Z = v;	     }
  void setMonaN4G4Time(double v)        { fMonaN4G4Time = v;         }
  void setMonaN4G4Edep(double v)        { fMonaN4G4Edep = v;         }
  void setMonaN4G4light(double v)       { fMonaN4G4light = v;        }
  void setMonaN4G5X(double v)           { fMonaN4G5X = v;	     }  
  void setMonaN4G5Y(double v)           { fMonaN4G5Y= v; 	     }
  void setMonaN4G5Z(double v)           { fMonaN4G5Z = v;	     }  
  void setMonaN4G5Time(double v)        { fMonaN4G5Time = v;         }
  void setMonaN4G5Edep(double v)        { fMonaN4G5Edep = v;         } 
  void setMonaN4G5light(double v)       { fMonaN4G5light = v;        }
  void setMonaN4G6X(double v)           { fMonaN4G6X = v;	     }  
  void setMonaN4G6Y(double v)           { fMonaN4G6Y= v; 	     }
  void setMonaN4G6Z(double v)           { fMonaN4G6Z = v;	     }
  void setMonaN4G6Time(double v)        { fMonaN4G6Time = v;         } 
  void setMonaN4G6Edep(double v)        { fMonaN4G6Edep = v;         }
  void setMonaN4G6light(double v)       { fMonaN4G6light = v;        }
  void setMonaN4G7X(double v)           { fMonaN4G7X = v;	     }
  void setMonaN4G7Y(double v)           { fMonaN4G7Y= v; 	     }
  void setMonaN4G7Z(double v)           { fMonaN4G7Z = v;	     }
  void setMonaN4G7Time(double v)        { fMonaN4G7Time = v;         }
  void setMonaN4G7Edep(double v)        { fMonaN4G7Edep = v;         }
  void setMonaN4G7light(double v)       { fMonaN4G7light = v;        }
  void setMonaN4G8X(double v)           { fMonaN4G8X = v;	     }
  void setMonaN4G8Y(double v)           { fMonaN4G8Y= v; 	     }
  void setMonaN4G8Z(double v)           { fMonaN4G8Z = v;	     }
  void setMonaN4G8Time(double v)        { fMonaN4G8Time = v;         }
  void setMonaN4G8Edep(double v)        { fMonaN4G8Edep = v;         }
  void setMonaN4G8light(double v)       { fMonaN4G8light = v;        }
  void setMonaN4G9X(double v)           { fMonaN4G9X = v;	     }
  void setMonaN4G9Y(double v)           { fMonaN4G9Y= v; 	     }
  void setMonaN4G9Z(double v)           { fMonaN4G9Z = v;	     }
  void setMonaN4G9Time(double v)        { fMonaN4G9Time = v;         }
  void setMonaN4G9Edep(double v)        { fMonaN4G9Edep = v;         }
  void setMonaN4G9light(double v)       { fMonaN4G9light = v;        }
  void setMonaN4G10X(double v)          { fMonaN4G10X = v;	     }
  void setMonaN4G10Y(double v)          { fMonaN4G10Y= v; 	     }
  void setMonaN4G10Z(double v)          { fMonaN4G10Z = v;	     }
  void setMonaN4G10Time(double v)       { fMonaN4G10Time = v;	     }
  void setMonaN4G10Edep(double v)       { fMonaN4G10Edep = v;        }
  void setMonaN4G10light(double v)      { fMonaN4G10light = v;       }// End N4



  void setMonaG1X(double v)             { fMonaG1X = v;            }
  void setMonaG1Y(double v)             { fMonaG1Y= v;             }
  void setMonaG1Z(double v)             { fMonaG1Z = v;            }
  void setMonaG1Time(double v)          { fMonaG1Time = v;         }
  void setMonaG1Edep(double v)          { fMonaG1Edep = v;         }
  void setMonaG1light(double v)         { fMonaG1light = v;        }
  void setMonaG2X(double v)             { fMonaG2X = v;            }
  void setMonaG2Y(double v)             { fMonaG2Y= v; 	     	   }
  void setMonaG2Z(double v)             { fMonaG2Z = v;	           }
  void setMonaG2Time(double v)          { fMonaG2Time = v;         }
  void setMonaG2Edep(double v)          { fMonaG2Edep = v;         }
  void setMonaG2light(double v)         { fMonaG2light = v;        }
  void setMonaG3X(double v)             { fMonaG3X = v;	  	   }
  void setMonaG3Y(double v)             { fMonaG3Y= v; 	  	   }
  void setMonaG3Z(double v)             { fMonaG3Z = v;	 	   }
  void setMonaG3Time(double v)          { fMonaG3Time = v;         }
  void setMonaG3Edep(double v)          { fMonaG3Edep = v;         }
  void setMonaG3light(double v)         { fMonaG3light = v;        }
  void setMonaG4X(double v)             { fMonaG4X = v;	  	   }
  void setMonaG4Y(double v)             { fMonaG4Y= v; 	 	   }
  void setMonaG4Z(double v)             { fMonaG4Z = v;	  	   }
  void setMonaG4Time(double v)          { fMonaG4Time = v;         }
  void setMonaG4Edep(double v)          { fMonaG4Edep = v;         }
  void setMonaG4light(double v)         { fMonaG4light = v;        }
  void setMonaG5X(double v)             { fMonaG5X = v;	  	   }
  void setMonaG5Y(double v)             { fMonaG5Y= v; 	  	   }
  void setMonaG5Z(double v)             { fMonaG5Z = v;	  	   }
  void setMonaG5Time(double v)          { fMonaG5Time = v;         }
  void setMonaG5Edep(double v)          { fMonaG5Edep = v;         }
  void setMonaG5light(double v)         { fMonaG5light = v;        }
  void setMonaG6X(double v)             { fMonaG6X = v;	  	   }
  void setMonaG6Y(double v)             { fMonaG6Y= v; 	  	   }
  void setMonaG6Z(double v)             { fMonaG6Z = v;	  	   }
  void setMonaG6Time(double v)          { fMonaG6Time = v;         }
  void setMonaG6Edep(double v)          { fMonaG6Edep = v;         }
  void setMonaG6light(double v)         { fMonaG6light = v;        }
  void setMonaG7X(double v)             { fMonaG7X = v;	  	   }
  void setMonaG7Y(double v)             { fMonaG7Y= v; 	  	   }
  void setMonaG7Z(double v)             { fMonaG7Z = v;	   	   }
  void setMonaG7Time(double v)          { fMonaG7Time = v;         }
  void setMonaG7Edep(double v)          { fMonaG7Edep = v;         }
  void setMonaG7light(double v)         { fMonaG7light = v;        }
  void setMonaG8X(double v)             { fMonaG8X = v;	  	   }
  void setMonaG8Y(double v)             { fMonaG8Y= v; 	 	   }
  void setMonaG8Z(double v)             { fMonaG8Z = v;	  	   }
  void setMonaG8Time(double v)          { fMonaG8Time = v;         }
  void setMonaG8Edep(double v)          { fMonaG8Edep = v;         }
  void setMonaG8light(double v)         { fMonaG8light = v;        }
  void setMonaG9X(double v)             { fMonaG9X = v;	 	   }
  void setMonaG9Y(double v)             { fMonaG9Y= v; 	  	   }
  void setMonaG9Z(double v)             { fMonaG9Z = v;	  	   }
  void setMonaG9Time(double v)          { fMonaG9Time = v;         }
  void setMonaG9Edep(double v)          { fMonaG9Edep = v;         }
  void setMonaG9light(double v)         { fMonaG9light = v;        }
  void setMonaG10X(double v)            { fMonaG10X = v;	     }
  void setMonaG10Y(double v)            { fMonaG10Y= v; 	     }
  void setMonaG10Z(double v)            { fMonaG10Z = v;	     }
  void setMonaG10Time(double v)         { fMonaG10Time = v;	     }
  void setMonaG10Edep(double v)         { fMonaG10Edep = v;        }
  void setMonaG10light(double v)        { fMonaG10light = v;       }
  void setMonaG11X(double v)            { fMonaG11X = v;            }
  void setMonaG11Y(double v)            { fMonaG11Y= v;             }
  void setMonaG11Z(double v)            { fMonaG11Z = v;            }
  void setMonaG11Time(double v)         { fMonaG11Time = v;         }
  void setMonaG11Edep(double v)         { fMonaG11Edep = v;         }
  void setMonaG11light(double v)        { fMonaG11light = v;        }
  void setMonaG12X(double v)            { fMonaG12X = v;            }
  void setMonaG12Y(double v)            { fMonaG12Y= v; 	     }
  void setMonaG12Z(double v)            { fMonaG12Z = v;	     }
  void setMonaG12Time(double v)         { fMonaG12Time = v;         }
  void setMonaG12Edep(double v)         { fMonaG12Edep = v;         }
  void setMonaG12light(double v)        { fMonaG12light = v;        }
  void setMonaG13X(double v)            { fMonaG13X = v;	     }
  void setMonaG13Y(double v)            { fMonaG13Y= v; 	     }
  void setMonaG13Z(double v)            { fMonaG13Z = v;	     }
  void setMonaG13Time(double v)         { fMonaG13Time = v;         }
  void setMonaG13Edep(double v)         { fMonaG13Edep = v;         }
  void setMonaG13light(double v)        { fMonaG13light = v;        }
  void setMonaG14X(double v)            { fMonaG14X = v;	     }
  void setMonaG14Y(double v)            { fMonaG14Y= v; 	     }
  void setMonaG14Z(double v)            { fMonaG14Z = v;	     }
  void setMonaG14Time(double v)         { fMonaG14Time = v;         }
  void setMonaG14Edep(double v)         { fMonaG14Edep = v;         }
  void setMonaG14light(double v)        { fMonaG14light = v;        }
  void setMonaG15X(double v)            { fMonaG15X = v;	     }  
  void setMonaG15Y(double v)            { fMonaG15Y= v; 	     }
  void setMonaG15Z(double v)            { fMonaG15Z = v;	     }  
  void setMonaG15Time(double v)         { fMonaG15Time = v;         }
  void setMonaG15Edep(double v)         { fMonaG15Edep = v;         } 
  void setMonaG15light(double v)        { fMonaG15light = v;        }
  void setMonaG16X(double v)            { fMonaG16X = v;	     }  
  void setMonaG16Y(double v)            { fMonaG16Y= v; 	     }
  void setMonaG16Z(double v)            { fMonaG16Z = v;	     }
  void setMonaG16Time(double v)         { fMonaG16Time = v;         } 
  void setMonaG16Edep(double v)         { fMonaG16Edep = v;         }
  void setMonaG16light(double v)        { fMonaG16light = v;        }
  void setMonaG17X(double v)            { fMonaG17X = v;	     }
  void setMonaG17Y(double v)            { fMonaG17Y= v; 	     }
  void setMonaG17Z(double v)            { fMonaG17Z = v;	     }
  void setMonaG17Time(double v)         { fMonaG17Time = v;         }
  void setMonaG17Edep(double v)         { fMonaG17Edep = v;         }
  void setMonaG17light(double v)        { fMonaG17light = v;        }
  void setMonaG18X(double v)            { fMonaG18X = v;	     }
  void setMonaG18Y(double v)            { fMonaG18Y= v; 	     }
  void setMonaG18Z(double v)            { fMonaG18Z = v;	     }
  void setMonaG18Time(double v)         { fMonaG18Time = v;         }
  void setMonaG18Edep(double v)         { fMonaG18Edep = v;         }
  void setMonaG18light(double v)        { fMonaG18light = v;        }
  void setMonaG19X(double v)            { fMonaG19X = v;	     }
  void setMonaG19Y(double v)            { fMonaG19Y= v; 	     }
  void setMonaG19Z(double v)            { fMonaG19Z = v;	     }
  void setMonaG19Time(double v)         { fMonaG19Time = v;         }
  void setMonaG19Edep(double v)         { fMonaG19Edep = v;         }
  void setMonaG19light(double v)        { fMonaG19light = v;        }
  void setMonaG20X(double v)            { fMonaG20X = v;	     }
  void setMonaG20Y(double v)            { fMonaG20Y= v; 	     }
  void setMonaG20Z(double v)            { fMonaG20Z = v;	     }
  void setMonaG20Time(double v)         { fMonaG20Time = v;	     }
  void setMonaG20Edep(double v)         { fMonaG20Edep = v;        }
  void setMonaG20light(double v)        { fMonaG20light = v;       }

  void setFragX(double v)    { fFragX = v;    }
  void setFragTX(double v)   { fFragTX = v;   }
  void setFragY(double v)    { fFragY = v;    } 
  void setFragTY(double v)   { fFragTY = v;   }
  void setFragTime(double v) { fFragTime = v; }

  void setTargetX(double v)  { fTargetX = v;  }
  void setTargetY(double v)  { fTargetY = v;  } 
  
public:
  CalMonaEvent() {} 
  virtual ~CalMonaEvent() {};
  /** fill next event into data structure
   * \return 0 if there are no more events
   */
  virtual int getEvent(long n = -1) = 0; 

  //@{
  /** write calibrated MoNA/Sweeper event (header) to filtered file
   */
  void writeFilterHeader(CXdrOutputStream* o);
  void writeFilter(CXdrOutputStream* o);
  //@}
};




/** Implementation of CalMonaEvent class using simulated data as input.
 *
 */
class CalMonaEventSim : public CalMonaEvent {
public:
  CalMonaEventSim(TTree* t=0) : fT(t), fEventNum(0) {if (fT) init();}

  /// set the tree pointer \todo this should be a chain!!!
  void setTree(TTree* t) {   
    fT = t;
    init();
  }

  /// get next event
  int getEvent(long n = -1);

  /// get current event number
  long getEventNum() { return fEventNum - 1; }

private:
  void init(); ///< initialize (needed) branches of the tree

private:
  TTree* fT;                     ///< tree pointer \todo this should be a chain
  Long64_t fEventNum;            ///< event number
  double mx,my,mz,mt,mEkin;      ///< mona (neutron) data  (Ekin is needed to check for valid events)
  double m2x,m2y,m2z,m2t,m2Ekin; ///< mona (neutron) data  (Ekin is needed to check for valid events)
  double m3x,m3y,m3z,m3t,m3Ekin;
  double m4x,m4y,m4z,m4t,m4Ekin;

  int    mN1GM;
  double mN1G1x,mN1G1y,mN1G1z,mN1G1t,mN1G1Edep,mN1G1light;    ///< mona (neutron) data  First 6 time ordered hits found using Geant4
  double mN1G2x,mN1G2y,mN1G2z,mN1G2t,mN1G2Edep,mN1G2light;    ///< mona (neutron) data  First 6 time ordered hits found using Geant4
  double mN1G3x,mN1G3y,mN1G3z,mN1G3t,mN1G3Edep,mN1G3light;    ///< mona (neutron) data  First 6 time ordered hits found using Geant4
  double mN1G4x,mN1G4y,mN1G4z,mN1G4t,mN1G4Edep,mN1G4light;    ///< mona (neutron) data  First 6 time ordered hits found using Geant4
  double mN1G5x,mN1G5y,mN1G5z,mN1G5t,mN1G5Edep,mN1G5light;    ///< mona (neutron) data  First 6 time ordered hits found using Geant4
  double mN1G6x,mN1G6y,mN1G6z,mN1G6t,mN1G6Edep,mN1G6light;    ///< mona (neutron) data  First 6 time ordered hits found using Geant4
  double mN1G7x,mN1G7y,mN1G7z,mN1G7t,mN1G7Edep,mN1G7light;    ///< mona (neutron) data  First 6 time ordered hits found using Geant4
  double mN1G8x,mN1G8y,mN1G8z,mN1G8t,mN1G8Edep,mN1G8light;    ///< mona (neutron) data  First 6 time ordered hits found using Geant4
  double mN1G9x,mN1G9y,mN1G9z,mN1G9t,mN1G9Edep,mN1G9light;    ///< mona (neutron) data  First 6 time ordered hits found using Geant4
  double mN1G10x,mN1G10y,mN1G10z,mN1G10t,mN1G10Edep,mN1G10light;    ///< mona (neutron) data  First 6 time ordered hits found using Geant4

  int    mN2GM;
  double mN2G1x,mN2G1y,mN2G1z,mN2G1t,mN2G1Edep,mN2G1light;    ///< mona (neutron) data  First 6 time ordered hits found using Geant4
  double mN2G2x,mN2G2y,mN2G2z,mN2G2t,mN2G2Edep,mN2G2light;    ///< mona (neutron) data  First 6 time ordered hits found using Geant4
  double mN2G3x,mN2G3y,mN2G3z,mN2G3t,mN2G3Edep,mN2G3light;    ///< mona (neutron) data  First 6 time ordered hits found using Geant4
  double mN2G4x,mN2G4y,mN2G4z,mN2G4t,mN2G4Edep,mN2G4light;    ///< mona (neutron) data  First 6 time ordered hits found using Geant4
  double mN2G5x,mN2G5y,mN2G5z,mN2G5t,mN2G5Edep,mN2G5light;    ///< mona (neutron) data  First 6 time ordered hits found using Geant4
  double mN2G6x,mN2G6y,mN2G6z,mN2G6t,mN2G6Edep,mN2G6light;    ///< mona (neutron) data  First 6 time ordered hits found using Geant4
  double mN2G7x,mN2G7y,mN2G7z,mN2G7t,mN2G7Edep,mN2G7light;    ///< mona (neutron) data  First 6 time ordered hits found using Geant4
  double mN2G8x,mN2G8y,mN2G8z,mN2G8t,mN2G8Edep,mN2G8light;    ///< mona (neutron) data  First 6 time ordered hits found using Geant4
  double mN2G9x,mN2G9y,mN2G9z,mN2G9t,mN2G9Edep,mN2G9light;    ///< mona (neutron) data  First 6 time ordered hits found using Geant4
  double mN2G10x,mN2G10y,mN2G10z,mN2G10t,mN2G10Edep,mN2G10light;    ///< mona (neutron) data  First 6 time ordered hits found using Geant4

  int    mN3GM;
  double mN3G1x,mN3G1y,mN3G1z,mN3G1t,mN3G1Edep,mN3G1light;    ///< mona (neutron) data  First 6 time ordered hits found using Geant4
  double mN3G2x,mN3G2y,mN3G2z,mN3G2t,mN3G2Edep,mN3G2light;    ///< mona (neutron) data  First 6 time ordered hits found using Geant4
  double mN3G3x,mN3G3y,mN3G3z,mN3G3t,mN3G3Edep,mN3G3light;    ///< mona (neutron) data  First 6 time ordered hits found using Geant4
  double mN3G4x,mN3G4y,mN3G4z,mN3G4t,mN3G4Edep,mN3G4light;    ///< mona (neutron) data  First 6 time ordered hits found using Geant4
  double mN3G5x,mN3G5y,mN3G5z,mN3G5t,mN3G5Edep,mN3G5light;    ///< mona (neutron) data  First 6 time ordered hits found using Geant4
  double mN3G6x,mN3G6y,mN3G6z,mN3G6t,mN3G6Edep,mN3G6light;    ///< mona (neutron) data  First 6 time ordered hits found using Geant4
  double mN3G7x,mN3G7y,mN3G7z,mN3G7t,mN3G7Edep,mN3G7light;    ///< mona (neutron) data  First 6 time ordered hits found using Geant4
  double mN3G8x,mN3G8y,mN3G8z,mN3G8t,mN3G8Edep,mN3G8light;    ///< mona (neutron) data  First 6 time ordered hits found using Geant4
  double mN3G9x,mN3G9y,mN3G9z,mN3G9t,mN3G9Edep,mN3G9light;    ///< mona (neutron) data  First 6 time ordered hits found using Geant4
  double mN3G10x,mN3G10y,mN3G10z,mN3G10t,mN3G10Edep,mN3G10light;    ///< mona (neutron) data  First 6 time ordered hits found using Geant4

  int    mN4GM;
  double mN4G1x,mN4G1y,mN4G1z,mN4G1t,mN4G1Edep,mN4G1light;    ///< mona (neutron) data  First 6 time ordered hits found using Geant4
  double mN4G2x,mN4G2y,mN4G2z,mN4G2t,mN4G2Edep,mN4G2light;    ///< mona (neutron) data  First 6 time ordered hits found using Geant4
  double mN4G3x,mN4G3y,mN4G3z,mN4G3t,mN4G3Edep,mN4G3light;    ///< mona (neutron) data  First 6 time ordered hits found using Geant4
  double mN4G4x,mN4G4y,mN4G4z,mN4G4t,mN4G4Edep,mN4G4light;    ///< mona (neutron) data  First 6 time ordered hits found using Geant4
  double mN4G5x,mN4G5y,mN4G5z,mN4G5t,mN4G5Edep,mN4G5light;    ///< mona (neutron) data  First 6 time ordered hits found using Geant4
  double mN4G6x,mN4G6y,mN4G6z,mN4G6t,mN4G6Edep,mN4G6light;    ///< mona (neutron) data  First 6 time ordered hits found using Geant4
  double mN4G7x,mN4G7y,mN4G7z,mN4G7t,mN4G7Edep,mN4G7light;    ///< mona (neutron) data  First 6 time ordered hits found using Geant4
  double mN4G8x,mN4G8y,mN4G8z,mN4G8t,mN4G8Edep,mN4G8light;    ///< mona (neutron) data  First 6 time ordered hits found using Geant4
  double mN4G9x,mN4G9y,mN4G9z,mN4G9t,mN4G9Edep,mN4G9light;    ///< mona (neutron) data  First 6 time ordered hits found using Geant4
  double mN4G10x,mN4G10y,mN4G10z,mN4G10t,mN4G10Edep,mN4G10light;    ///< mona (neutron) data  First 6 time ordered hits found using Geant4

  double mG1x,mG1y,mG1z,mG1t,mG1Edep,mG1light;    ///< mona (neutron) data  First 6 time ordered hits found using Geant4
  double mG2x,mG2y,mG2z,mG2t,mG2Edep,mG2light;    ///< mona (neutron) data  First 6 time ordered hits found using Geant4
  double mG3x,mG3y,mG3z,mG3t,mG3Edep,mG3light;    ///< mona (neutron) data  First 6 time ordered hits found using Geant4
  double mG4x,mG4y,mG4z,mG4t,mG4Edep,mG4light;    ///< mona (neutron) data  First 6 time ordered hits found using Geant4
  double mG5x,mG5y,mG5z,mG5t,mG5Edep,mG5light;    ///< mona (neutron) data  First 6 time ordered hits found using Geant4
  double mG6x,mG6y,mG6z,mG6t,mG6Edep,mG6light;    ///< mona (neutron) data  First 6 time ordered hits found using Geant4
  double mG7x,mG7y,mG7z,mG7t,mG7Edep,mG7light;    ///< mona (neutron) data  First 6 time ordered hits found using Geant4
  double mG8x,mG8y,mG8z,mG8t,mG8Edep,mG8light;    ///< mona (neutron) data  First 6 time ordered hits found using Geant4
  double mG9x,mG9y,mG9z,mG9t,mG9Edep,mG9light;    ///< mona (neutron) data  First 6 time ordered hits found using Geant4
  double mG10x,mG10y,mG10z,mG10t,mG10Edep,mG10light;    ///< mona (neutron) data  First 6 time ordered hits found using Geant4
  double mG11x,mG11y,mG11z,mG11t,mG11Edep,mG11light;    ///< mona (neutron) data  First 6 time ordered hits found using Geant4
  double mG12x,mG12y,mG12z,mG12t,mG12Edep,mG12light;    ///< mona (neutron) data  First 6 time ordered hits found using Geant4
  double mG13x,mG13y,mG13z,mG13t,mG13Edep,mG13light;    ///< mona (neutron) data  First 6 time ordered hits found using Geant4
  double mG14x,mG14y,mG14z,mG14t,mG14Edep,mG14light;    ///< mona (neutron) data  First 6 time ordered hits found using Geant4
  double mG15x,mG15y,mG15z,mG15t,mG15Edep,mG15light;    ///< mona (neutron) data  First 6 time ordered hits found using Geant4
  double mG16x,mG16y,mG16z,mG16t,mG16Edep,mG16light;    ///< mona (neutron) data  First 6 time ordered hits found using Geant4
  double mG17x,mG17y,mG17z,mG17t,mG17Edep,mG17light;    ///< mona (neutron) data  First 6 time ordered hits found using Geant4
  double mG18x,mG18y,mG18z,mG18t,mG18Edep,mG18light;    ///< mona (neutron) data  First 6 time ordered hits found using Geant4
  double mG19x,mG19y,mG19z,mG19t,mG19Edep,mG19light;    ///< mona (neutron) data  First 6 time ordered hits found using Geant4
  double mG20x,mG20y,mG20z,mG20t,mG20Edep,mG20light;    ///< mona (neutron) data  First 6 time ordered hits found using Geant4

  
  double fx,ftx,fy,fty,ft,fEkin; ///< fragment data
  double tx,ty;                  ///< target data
};


class ParticleCoordinates {
public:
  double xt,txt,yt,tyt,d;     ///< target coordinates x,tx,... target + delta
  double xd,txd,yd,tyd,l,t;   ///< detector coordinates x,tx... detector + length + time
};

#endif // MONA_EVENT_HH

