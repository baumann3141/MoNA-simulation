/// \file
#include <TTree.h> 
#include <TFile.h> 
#include <TString.h> 
#include "TObjString.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <math.h>
#include <gsl/gsl_sf_bessel.h>
#include "deb_err.h"
#include "st_ble.hh"
#include "st_propagate.hh"
#include "st_particle.hh"
#include "st_system.hh"
#include "st_part_init.hh"
#include "st_material.hh"
#include "st_par_mat_ia.hh"
#include "st_reaction.hh"
#include "st_histo_tuple.hh"
#include "st_aperture.hh"
#include "st_histo_tuple.hh"
#include "CycSrim.h"


using std::ios;
using namespace std;

void usage() {
  printf(""
	 "Usage: \n"
	 "       \n"
	 " -v     Run verbose.\n"
	 " -n     Set number of events.\n"                                                    // 1
	 " -f     Set output file name.  Extension determines file type.\n"                  // 2
	 "          root   -> ROOT file.\n"
	 "          gsltup -> GSL n-tuple (also a file *.gsltup.dsc is written, describing the columns).\n"
	 " -geant Cause ST MONA to write out neutron energy & angles to an ASCII file.\n"
	 " -reac  Set reaction model.\n"
	 "         glaub    (Normal fragmentation with Glauber kick)\n"
	 "         glaub_noDecay    (Normal fragmentation wth glauber kick) WITHOUT NEUTRON DECAY\n"
	 "         2neutron (2 neutron sequential decay with Glauber kick)\n"
	 "         di_neutron (Dineutron emission and decay with Glauber kick) set -e for decay of the frag-dineutron system and -e1 for decay of the dineutron (n-n)\n"
	 "                                                                    or use the Volya_dineutron decay description (set as -e)"
	 "         3body_decay (3body decay with Glauber kick) set -e for decay of the frag-n-n system\n"
	 "         2body    <Q-value> <Q-value spread> (two-body stripping)\n"
	 "                            n .b. spread is relative, e.g. 0.1 = 10%% spread\n"
	 "                            defaults to 0.4 rad cone in CoM\n"
	 " -reacA  Set what kind of angular distribution (theta) you want for 2body rxn @ target\n"
	 "	  flat <lower-lim (rad)> <upper-lim (rad)>  flat distribution.\n"
	 "	  gauss <width (rad)> <lower-lim (rad)> <upper-lim (rad)>  gaussian distribution.\n"
	 "	  COSgauss <width (rad)> <lower-lim (rad)> <upper-lim (rad)>  gaussian distribution in cosine instead of angle.\n"
	 "         NOTE: in spherical polar coordinates, theta goes from 0 to pi! Set your ranges accordingly\n"
	 " -debug  print to screen out a parameter for debugging purposes. Specified through an argument.\n"
	 "         qValue   spits out the q-value being used for reaction in target for 2body\n"
	 "         exen   spits out the excitatin energy being used for neutron evaporation\n"
	 "         thetaCMP spits out the projectile theta in COM being used for a 2body reaction in target\n"
	 "         phiCMP spits out the projectile phi in COM being used for a 2body reaction in target\n"
	 "         thetaLABP spits out the projectile theta in lab frame being used for reaction in target\n"
	 "         phiLABP spits out the projectile phi in lab frame being used for reaction in target\n"
	 "         NOTE: if you want to use this to generate a histogram from a file, you need to redirect\n"
	 "             the standard output of the program. Do this by putting the following after all flags:\n"
	 "             > filename\n"
	 " -Momentum <para/perp> <filename> set the para/perp momentum distribution of the excited system"
	 " -e      Set decay energy model and parameters for the total decay(MeV)\n"                             // 3
	 "          const           <energy>\n"
	 "          exp             <exp coeffic.>\n"
	 "          therm           <temp>\n"
	 "          Etotal_11Li     <Eb> For documentation see Nuclear Physics A 673 (2000) 143-156\n"
	 "          E_12Li          <Eb> For documentation see Nuclear Physics A 673 (2000) 143-156\n"
	 "          uniform         <e-low> <e-high>\n"
	 "          e1              <e-low> <e-high>\n"
	 "          gauss           <centr> <sigma>\n"
	 "          swave           <Eb> <scatlength>  Eb is neutron separation energy of beam [MeV], scatlenght is the NEGATIVE of the scattering length [fm].\n"
	 "          bw              <centr> <gamma>\n"
	 "          asymbw          <energy> <obsWidth> <angMom> <flagGamma>, flagGamma = 0 -> Observed Width, flagGamma = 1 -> Reduced Width\n"
	 "          TwoNeutron_seq  <TotalDecayCentroid> <TotalWidth> <FirstDecayWidth> <2ndDecayCentoid> <SecondDecayWidth> <angMom>, for documentation see PRL,74 Two-Proton Emission..\n"
	 "          Volya_2nSeq     <EnergyInitialState> <IntermediateStateEnergy> <SF I->V> <SF V->F> <angMom> <Gamma Initial State>\n"
	 "                             **if IntermediateStateEnergy is negative and L=0 then assumes input a scattering length (fm)\n"
	 "          Volya_Dineutron     <EnergyInitialState> <Gamma Initial State> <scattering length (-fm)>\n"
	 " -e1     Set decay energy model and parameters for the first decay in the 2 neutron sequential decay(MeV)\n"                             // 3
	 "          const           <energy>\n"
	 "          exp             <exp coeffic.>\n"
	 "          therm           <temp>\n"
	 "          Etotal_11Li     <Eb> For documentation see Nuclear Physics A 673 (2000) 143-156\n"
	 "          E_12Li          <Eb> For documentation see Nuclear Physics A 673 (2000) 143-156\n"
	 "          uniform         <e-low> <e-high>\n"
	 "          e1              <e-low> <e-high>\n"
	 "          gauss           <centr> <sigma>\n"
	 "          swave           <Eb> <scatlength>\n"
	 "          bw              <centr> <gamma>\n"
	 "          asymbw          <energy> <obsWidth> <angMom> <flagGamma>, flagGamma = 0 -> Observed Width, flagGamma = 1 -> Reduced Width\n"
	 "	        cos_twoneutron  No flags necessary, it picks a random number between 0 and 1 that look an arccos(theta) and multiplies the total by that value.\n"
	 " -e2     Set decay energy model and parameters for the second decay in the 2 neutron sequential decay(MeV)\n"                             // 3
	 "          const           <energy>\n"
	 "          exp             <exp coeffic.>\n"
	 "          therm           <temp>\n"
	 "          Etotal_11Li     <Eb> For documentation see Nuclear Physics A 673 (2000) 143-156\n"
	 "          E_12Li          <Eb> For documentation see Nuclear Physics A 673 (2000) 143-156\n"
	 "          uniform         <e-low> <e-high>\n"
	 "          e1              <e-low> <e-high>\n"
	 "          gauss           <centr> <sigma>\n"
	 "          swave           <Eb> <scatlength>\n"
	 "          bw              <centr> <gamma>\n"
	 "          asymbw          <energy> <obsWidth> <angMom> <flagGamma>, flagGamma = 0 -> Observed Width, flagGamma = 1 -> Reduced Width\n"
	 "	        cos_twoneutron  No flags necessary, it picks a random number between 0 and 1 that look an arccos(theta) and multiplies the total decay by that value.\n"
	 " -be   Set beam energy per A\n"                                                   // 4
	 " -bZ   Set beam Z\n"                                                              // 5
	 " -bA   Set beam A\n"                                                              // 6
	 " -dT   Set target thickness\n"                                                    // 7
	 " -dbe  Set relative beam energy spread (sigma)\n"                                 // 8
	 " -rTx  Set resolution of target-x\n"                                              // 9 
	 " -rt   Set time resolutions (ns)\n"                                               //10
	 " -rx  Set resolution of CRDC1 x (m)\n"                                            //11
	 " -ry  Set resolution of CRDC1 y (m)\n"  
	 " -rthx  Set resolution of CRDC1 theta x (rad)\n"                                  //12
	 " -rthy  Set resolution of CRDC1 theta y (rad)\n"                                  
	 " -rMx1  Set 1st resolution of MoNA x \n"
	 " -rMx2  Set 2nd resolution of MoNA x \n"
	 " -rMxP  Set P-resolution of MoNA x \n"
	 " -rMyz Switch MoNA Y and Z discretization on/off (1/0)\n"                         //14
	 " -strag scale\n"                                                                  //15
	 " -glaub scale\n"                                                                  //16
	 " -np   number of removed protons\n"                                               //17
	 " -nn   number of removed neutrons\n"                                              //18
	 " -md   distance to MoNA (middle of A8)\n"                                         //19
	 " -cmL  position of left  edge of blocker (before CRDC1)\n"                        //21 
	 " -cmR  position of right edge of blocker (before CRDC1)\n"                        //22 
	 " -c2d  distance between CRDC1 and CRDC2 (meters)\n"                               //23
	 " -ctx  centroid of x angle in radians \n"                                         //24 
	 " -cx   centroid of x position in meters \n"                                       //25 
	 " -cty  centroid of y angle in radians \n"                                         //26 
	 " -cy   centroid of y position in meters \n"                                       //27 
	 " -dtx  sigma of x angle (rad) \n"                                                 //28 
	 " -dx   sigma of x posn (m) \n"                                                    //29  
	 " -dty  sigma of y angle (rad) \n"                                                 //30 
	 " -dy   sigma of y posn (m) \n"                                                    //31 
	 " -disTyp type of MoNA X distribution\n"                                           //32
	 " -velocityshift scale \n"                                                         //33
     " -legendre_lvalue L-value of neutron \n"     
     " -phase_space_weight flag for weighting in phase space decays \n"         
	 " -asymMomentum scale\n"                                                           //34
	 " -exp  <exp number>  set default values for experiment <exp number> \n"           //20
	 "          03033 - Nathan's 23O, 22O\n"
	 "          03038 - Kiss 7He\n"
	 "          03038a - 7He no target"
	 "          03048a - 12Be g.s. to 10Be\n"
	 "          03048b - 11Be Coul to 10Be\n"
	 "          05034a - 14B --> 11Li + n\n"
	 "          05034_Be - 14Be --> 11Li + n\n"
	 "          05039  - Hoffman 25O\n"
	 "          05039a - Hoffman 24O\n"
	 "          05039b - Hoffman 23O\n"
	 "          05039c - Hoffman 26F\n"
	 "          05124a - Fragmentation 20O\n"
	 "          05124b - Fragmentation 13B\n"
	 "          05124c - Fragmentation 25Ne\n"
	 "          05124d - Fragmentation 16C\n"
	 "          05124e - Fragmentation 19O\n"
	 "          05124_Li9  - Fragmentation Li9\n"
	 "          05124_O18 - Fragmentation 18O\n"
	 "          05124_O19 - Fragmentation 19O\n"
	 "          05124_O20 - Fragmentation 20O\n"
	 "          05124_O21 - Fragmentation 21O\n"
	 "          05124_O22 - Fragmentation 22O\n"
	 "          06025_B16 - B16, production data\n" 
	 "          06025_Be16 - B17-beam, Be16->Be14+2n\n"

	 //"\n"
         );
  exit(0);
}

int main (size_t argc, char** argv) {
  //get location of executable so we can find input files
  string execloc = argv[0];
  short execlength = 7;
  execloc.erase(execloc.length() - execlength, execlength);

  /*--------------*/
  int nevents=1;                                              // number of events to simulate
  string tupleFile = execloc + "../root_files/st_mona.root";  //
  
  string exType ="uniform";                                   // Type of RNG for first (1 neutron decay) or total (2 neutron decay) decay energy 
  double exPar[10];                                           // parameters for RNG for excitation energy
  exPar[0] = 0;                                               //
  exPar[1] = 5;                                               //
  exPar[2] = 0;                                               //
  exPar[3] = 5;                                               //
  exPar[4] = 5;                                               //
  int exParInt[2];                                            // Param for RNG asymbw ang mom or 2 neutron decay
  exParInt[0] = 10;                                           //
  exParInt[1] = 0;                                            //
  
  //-----------------------Parameters for 2 neutron decay JDS 08/2009----------------------//
  string exType1="uniform";                                   // Type of RNG for first neutron decay energy, in 2 neutron decay
  double exPar1[10];                                          // parameters for RNG for excitation energy
  exPar1[0] = 0;                                              //
  exPar1[1] = 5;                                              // 
  int exPar1Int[2];                                           // Param for RNG asymbw ang mom or 2 neutron decay
  exPar1Int[0] = 10;                                          //
  exPar1Int[1] = 0;                                           //
  
  string exType2="uniform";                                   // Type of RNG for second neutron decay energy, in 2 neutron decay
  double exPar2[10];                                          // parameters for RNG for excitation energy
  exPar2[0] = 0;                                              //
  exPar2[1] = 5;                                              //
  int exPar2Int[2];                                           // Param for RNG asymbw ang mom or 2 neutron decay
  exPar2Int[0] = 10;                                          //
  exPar2Int[1] = 0; 
  
  //----------------------Parameters for 3 and 4 neutron decay MDJ ------------------------//

  string exType3="uniform";                                    
  double exPar3[10];					      // Third neutron
  exPar3[0] = 0;
  exPar3[1] = 5;
  int exPar3Int[2];
  exPar3Int[0] = 10;
  exPar3Int[1] = 0;

  string exType4="uniform";
  double exPar4[10];					      // Fourth neutron
  exPar4[0] = 0;
  exPar4[1] = 5;
  int exPar4Int[2];
  exPar4Int[0] = 10;
  exPar4Int[1] = 0;

  int DecayOneFilled = 0;                                     // Flag to show that the first decay energy has been filled
  int DecayTwoFilled = 0;                                     // Flag to show that the second decay energy has been filled
  int DecayThreeFilled = 0;		
  int DecayFourFilled = 0;				                      // Flag to show that the fourth decay energy has been filled
  int DecayTotalFilled = 0;                                   // Flag to show that the total decay energy has been filled
  
  string WhichEnergyIsReconstruct = "ReconstructTotalEnergy"; //
  
  string cust_file;       //added by SMM on 10/16/06 for custom excitation energy lineshape
  string cust_file1;      //added by SMM on 10/16/06 for custom excitation energy lineshape
  string cust_file2;      //added by SMM on 10/16/06 for custom excitation energy lineshape
  string cust_file3;
  string cust_file4;

  //-----------------------End of new parameters for 2-4 neutron decay-----------------------//
  TString momType = "";
  string momFile = "";
  
  bool   verbose = false;                                     //
  bool   geant = false;                                       //
  
  string reacType = "glaub";                                  // 2body
  
  double reacQval=0;                                          // 2body
  double reacQvalSpread=0;                                    // 2body
  
  string aDistType = "flat";                                  // 2body
  string debugSwitch = "none";				                  // for debugging output
  
  double reacASpread=0.;                                      // 2body
  double reacALowLim=0.;                                      // 2body
  double reacAUpLim=0.4;                                      // 2body
  double eBeam=86;                                            //
  double dEbeam=.03;                                          // relative beam energy spread (sigma)                         
  double dTarget=722;                                         // (mg/cm^2)							  
  double resTime=.1;                                          // (ns)
  double resTargetX=.0007;                                    // (m)
  double resCRDC1X=.0013;                                     // (m)
  double resCRDC1Y=.0013;                                     // (m)
  double resCRDC1ThetaX=.001;                                 // (rad)
  double resCRDC1ThetaY=.001;                                 // (rad)
  double resMonaX1=0.162;                                     // For Double Laplace
  double resMonaX2=0.0233;                                    // For Double Laplace
  double resMonaP=0.534;                                      // Fraction for resMonaX1 i.e. 66% is X1 34% X1
  
  int    resMonaBar=1;                                        // flag to discretize y and randomize z
  int    beamA=26;                                            //
  int    beamZ=10;                                            //
  
  double scaleGlaub = 1;                                      //
  double scaleStrag = 1;                                      //
  
  int    nNeutr=0;                                            // neutrons removed in reaction
  int    nProt=2;                                             // protons  removed in reaction
  
  double bSpotDx  = 0.005;                                    //
  double bSpotDy  = 0.012;                                    //
  double bSpotDtx = 0.01;                                     //
  double bSpotDty = 0.0015;                                   //
  double bSpotCtx = 0.0;                                      //
  double bSpotCx = 0.0;                                       //
  double bSpotCty = 0.0;                                      //
  double bSpotCy = 0.0;                                       //
  double bSpotSxtx = 0.0;		         	                  // xtx and yty correlations in beam spot
  double bSpotSyty = 0.0;   
  //ANK: 6-1-2016 added new paramters from JKS to make multiple component beam distribution
  double bSpotDx2  = 0.0;                                    //
  double bSpotDy2  = 0.0;                                    //
  double bSpotDtx2 = 0.0;                                    //
  double bSpotDty2 = 0.00;                                   //
  double bSpotFty2 = 0.00;                                   //
  double bSpotFy2 = 0.00;                                    //
  double bSpotCtx2 = 0.0;                                    //
  double bSpotCx2 = 0.0;                                     //
  double bSpotCty2 = 0.0;                                    //
  double bSpotCy2 = 0.0;                                     //
  double bSpotSxtx2 = 0;                                     //
  double bSpotSyty2 = 0;                                     //
  double bSpotDx3  = 0.0;                                    //
  double bSpotDy3  = 0.0;                                    //
  double bSpotDtx3 = 0.0;                                    //
  double bSpotDty3 = 0.00;                                   //
  double bSpotCtx3 = 0.0;                                    //
  double bSpotCx3 = 0.0;                                     //
  double bSpotCty3 = 0.0;                                    //
  double bSpotCy3 = 0.0;                                     //
  double bSpotSxtx3 = 0;
  double bSpotSyty3 = 0;
  double bSpotSxy3 = 0;
  double ratio = 1;
  double scale1 = 1;
  double scale2 = 0;
  double scale3 = 0;
  double rnn = 0;
  double normscale1 = 1;
  double normscale2 = 1;
  //////////////////////  ANK: end of new parameters                         //
  double crdc2dist = 1.529;                                   // distance between CRDC1 and 2
  double monaDist = 4.55;                                     // distance to middle of A8
  double crdc1MaskLeft  =  0.15;                              //
  double crdc1MaskRight = -0.15;                              // 
  double crdc2MaskLeft  =  0.15;                              //
  double crdc2MaskRight = -0.15;                              // 
  double crdc2MaskTop  =  0.15;                               //
  double crdc2MaskBot = -0.15;                                // 
  double distTypeMx = 0;                                      //
  double velocityshift = 1.0;                                 //
  double asymMomentum = 1.0;                                  //
  double legendre_lvalue = 0.0;                               // L-value for correlated decays, sets ang dist of neutron based off legendre polynomial L
  double maxbulge=0.0;					                      // bulge of D2 target, in [mm]
  double tflatfoil=0.0;					                      // thickness of flat foils
  double phase_space_weight = 0.0;
  
  string exp = "03033";                                       //
  string inputfile;         
  string cosymap;					                          // name of cosymap to use
  string target_material = "Beryllium";                       // name of target material 
    
  printf("\nProgram Simple-Track (st)        Heiko Scheit (scheit@nscl.msu.edu)\n");
  printf("Compiled on " __DATE__ " at " __TIME__ ".\n\n");

  string INPUT_DATA_DIR = "/projects/mona-sim/baumann/single_target/st_mona/input_files/";
  string INPUT_FILE     = "input_file_TB.txt";
  inputfile = INPUT_DATA_DIR + INPUT_FILE; 
  
  // Read in Input File
  //ifstream myfile("/projects/mona-sim/jonesm/st_mona/input_files/input.txt");
  ifstream myfile(inputfile.c_str());  

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

  if (myfile.is_open()){

     while ( getline(myfile,line) ){

         getline(myfile,line,';');

         if(line.find_first_of("#") != line.find_first_not_of(" \t")){// If the first non-whitespace char is NOT a # (comment)  
            delimiter = line.find("=");// cout << " delimiter " << delimiter << endl;
            length = line.length();

         if(line.length() != 0 ) { // Make sure string is not NULL
             name_length =line.copy(var_name,delimiter,0); 
             
             var_name[name_length] = '\0';
             value_length = line.copy(var_value,line.length(),delimiter+1); 
             var_value[value_length] = '\0';
         
	         // Remove whitespace from var_name and var_value
             VAR_NAME  = string(var_name);
             VAR_VALUE = string(var_value);
             
             int strBegin = VAR_NAME.find_first_not_of(" \t"); int strBegin1 = VAR_VALUE.find_first_not_of(" \t");
  	         int strEnd   = VAR_NAME.find_last_not_of(" \t");  int strEnd1   = VAR_VALUE.find_last_not_of(" \t");
  	         int strRange = strEnd - strBegin +1;	           int strRange1 = strEnd1 - strBegin1 + 1;
             
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
                  if( strcmp(var_name,"reacQval")== 0) 	    	reacQval 	    = dvalue;
             else if( strcmp(var_name,"reacQvalSpread") == 0 )  reacQvalSpread	= dvalue;
             else if( strcmp(var_name,"reacASpread")== 0) 	    reacASpread	    = dvalue;
             else if( strcmp(var_name,"reacALowLim") == 0 ) 	reacALowLim	    = dvalue;
             else if( strcmp(var_name,"reacAUpLim")== 0) 	    reacAUpLim	    = dvalue;
             else if( strcmp(var_name,"eBeam") == 0 ) 		    eBeam		    = dvalue;
             else if( strcmp(var_name,"dEbeam")== 0) 		    dEbeam	  	    = dvalue;
             else if( strcmp(var_name,"dTarget") == 0 )   	    dTarget		    = dvalue;
	         else if( strcmp(var_name,"resTime")== 0) 		    resTime	 	    = dvalue;
             else if( strcmp(var_name,"resTargetX") == 0 ) 	    resTargetX	    = dvalue;
             else if( strcmp(var_name,"resCRDC1X") == 0 ) 	    resCRDC1X	    = dvalue;
             else if( strcmp(var_name,"resCRDC1Y")== 0) 	    resCRDC1Y  	    = dvalue;
             else if( strcmp(var_name,"resCRDC1ThetaX") == 0 ) 	resCRDC1ThetaX	= dvalue;
             else if( strcmp(var_name,"resCRDC1ThetaY")== 0) 	resCRDC1ThetaY 	= dvalue;
             else if( strcmp(var_name,"resMonaX1") == 0 ) 	    resMonaX1	    = dvalue;
             else if( strcmp(var_name,"resMonaX2") == 0 ) 	    resMonaX2	    = dvalue;
             else if( strcmp(var_name,"resMonaP")== 0) 		    resMonaP  	    = dvalue;
             else if( strcmp(var_name,"scaleGaub") == 0 ) 	    scaleGlaub	    = dvalue;
             else if( strcmp(var_name,"scaleStrag")== 0) 	    scaleStrag 	    = dvalue;
             else if( strcmp(var_name,"bSpotDx") == 0 ) 	    bSpotDx		    = dvalue;
             else if( strcmp(var_name,"bSpotDy") == 0 ) 	    bSpotDy		    = dvalue;
             else if( strcmp(var_name,"bSpotDtx")== 0) 		    bSpotDtx  	    = dvalue;
             else if( strcmp(var_name,"bSpotDty") == 0 ) 	    bSpotDty	    = dvalue;
             else if( strcmp(var_name,"bSpotCx")== 0) 		    bSpotCx	 	    = dvalue;
             else if( strcmp(var_name,"bSpotCty") == 0 ) 	    bSpotCty	    = dvalue;
	     else if ( strcmp(var_name,"bSpotCy") == 0 )            bSpotCy         = dvalue;
	     else if ( strcmp(var_name,"bSpotCtx") == 0 )           bSpotCtx         = dvalue;
	     else if ( strcmp(var_name,"bSpotSxtx") == 0 )           bSpotSxtx         = dvalue;	  
	     else if ( strcmp(var_name,"bSpotSyty") == 0 )           bSpotSyty         = dvalue;

             else if( strcmp(var_name,"bSpotDx2") == 0 ) 	    bSpotDx2		    = dvalue;
             else if( strcmp(var_name,"bSpotDy2") == 0 ) 	    bSpotDy2		    = dvalue;
             else if( strcmp(var_name,"bSpotDtx2")== 0) 		    bSpotDtx2  	    = dvalue;
             else if( strcmp(var_name,"bSpotDty2") == 0 ) 	    bSpotDty2	    = dvalue;
             else if( strcmp(var_name,"bSpotCx2")== 0) 		    bSpotCx2	 	    = dvalue;
             else if( strcmp(var_name,"bSpotCty2") == 0 ) 	    bSpotCty2	    = dvalue;
	     else if ( strcmp(var_name,"bSpotCy2") == 0 )            bSpotCy2         = dvalue;
	     else if ( strcmp(var_name,"bSpotCtx2") == 0 )           bSpotCtx2         = dvalue;
	     else if ( strcmp(var_name,"bSpotSxtx2") == 0 )           bSpotSxtx2         = dvalue;	  
	     else if ( strcmp(var_name,"bSpotSyty2") == 0 )           bSpotSyty2         = dvalue;
	     else if ( strcmp(var_name,"bSpotFty2") == 0 )           bSpotFty2         = dvalue;	  
	     else if ( strcmp(var_name,"bSpotFy2") == 0 )           bSpotFy2         = dvalue;

	     else if( strcmp(var_name,"bSpotDx3") == 0 ) 	    bSpotDx3		    = dvalue;
             else if( strcmp(var_name,"bSpotDy3") == 0 ) 	    bSpotDy3		    = dvalue;
             else if( strcmp(var_name,"bSpotDtx3")== 0) 		    bSpotDtx3  	    = dvalue;
             else if( strcmp(var_name,"bSpotDty3") == 0 ) 	    bSpotDty3	    = dvalue;
             else if( strcmp(var_name,"bSpotCx3")== 0) 		    bSpotCx3	 	    = dvalue;
             else if( strcmp(var_name,"bSpotCty3") == 0 ) 	    bSpotCty3	    = dvalue;
	     else if ( strcmp(var_name,"bSpotCy3") == 0 )            bSpotCy3         = dvalue;
	     else if ( strcmp(var_name,"bSpotCtx3") == 0 )           bSpotCtx3         = dvalue;
	     else if ( strcmp(var_name,"bSpotSxtx3") == 0 )           bSpotSxtx3         = dvalue;	  
	     else if ( strcmp(var_name,"bSpotSyty3") == 0 )           bSpotSyty3         = dvalue;
	     else if ( strcmp(var_name,"bSpotSxy3") == 0 )           bSpotSxy3         = dvalue;	  

	     else if ( strcmp(var_name,"ratio") == 0 )              ratio         = dvalue;
	     else if ( strcmp(var_name,"scale1") == 0 )             scale1         = dvalue;
	     else if ( strcmp(var_name,"scale2") == 0 )             scale2         = dvalue;
	     else if ( strcmp(var_name,"scale3") == 0 )             scale3         = dvalue;
		  
             else if( strcmp(var_name,"crdc2dist") == 0 ) 	    crdc2dist	    = dvalue;
             else if( strcmp(var_name,"monaDist")== 0) 		    monaDist  	    = dvalue;
             else if( strcmp(var_name,"crdc1MaskLeft") == 0 ) 	crdc1MaskLeft	= dvalue;
             else if( strcmp(var_name,"crdc1MaskRight")== 0) 	crdc1MaskRight 	= dvalue;
             else if( strcmp(var_name,"crdc2MaskLeft") == 0 ) 	crdc2MaskLeft	= dvalue;
             else if( strcmp(var_name,"crdc2MaskTop") == 0 ) 	crdc2MaskTop	= dvalue;
             else if( strcmp(var_name,"crdc2MaskBot")== 0) 	    crdc2MaskBot  	= dvalue;
             else if( strcmp(var_name,"distTypeMx") == 0 ) 	    distTypeMx	    = dvalue;
             else if( strcmp(var_name,"velocityshift")== 0) 	velocityshift 	= dvalue;
             else if( strcmp(var_name,"legendre_lvalue")==0)    legendre_lvalue = ivalue;
             else if( strcmp(var_name,"asymMomentum") == 0 ) 	asymMomentum	= dvalue;
	         else if( strcmp(var_name,"maxbulge") == 0 ) 	    maxbulge 	    = dvalue;
	         else if( strcmp(var_name,"tflatfoil") == 0) 	    tflatfoil 	    = dvalue;



             // List ints and check variable name

             else if( strcmp(var_name,"resMonaBar") == 0 )  	resMonaBar	= ivalue;
             else if( strcmp(var_name,"beamA")== 0) 		    beamA	  	= ivalue;
             else if( strcmp(var_name,"beamZ") == 0 ) 		    beamZ		= ivalue;
             else if( strcmp(var_name,"nNeutr")== 0) 		    nNeutr	 	= ivalue;
             else if( strcmp(var_name,"nProt") == 0 ) 	    	nProt		= ivalue;
               
         } 

         else if (pch != NULL) {// Do not convert to a double
           // Cut off quotation marks and keep variable as a string.
           int valBegin = VAR_VALUE.find_first_of("\"")+1;
           int valEnd   = VAR_VALUE.find_last_of("\"");
           int valRange = valEnd - valBegin;
           
           VAR_VALUE = VAR_VALUE.substr(valBegin,valRange);
           strcpy(var_value,VAR_VALUE.c_str());
             
	     // List strings and check variable name
                  if( strcmp(var_name,"reacType") == 0 )       reacType        = var_value;
             else if( strcmp(var_name,"aDistType") == 0)       aDistType	   = var_value;
             else if( strcmp(var_name,"debugSwitch") == 0)     debugSwitch     = var_value;
             else if( strcmp(var_name,"exp") == 0)	           exp	           = var_value;
	         else if( strcmp(var_name,"cosymap") == 0) 	       cosymap		   = var_value;
             else if( strcmp(var_name,"target_material") ==0)  target_material = var_value;
                
         }
         
        }
    }
      myfile.close();
  }

  else  ERR("Could not find input file!");

	
  // Read in arguments for ST_MoNA, set reaction type and decay energy distributions etc.
  //
  for (size_t i = 1; i < argc; i++) {
    string sw = argv[i];  // Switch
    cout << "sw: " << sw << endl;
    if (0) ;

    else if (sw == "createinputfile") {
      ofstream myfile;
      myfile.open ("input.txt");
      myfile << "Writing this to a file.\n";
      myfile.close();
      exit(0);
    }

    else if (sw == "inputfile") {
      ofstream myfile;
      myfile.open ("input.txt");
      myfile << "Writing this to a file.\n";
      myfile.close();
      exit(0);
    }

    else if (sw == "-v")     verbose    = true;
    else if (sw == "-n")     nevents    = atoi(argv[++i]); // 1
    else if (sw == "-f")     tupleFile  = argv[++i];       // 2
    else if (sw == "-geant") geant      = true;
    else if (sw == "-Momentum"){
      momType = argv[++i];
      momFile = argv[++i];
    }

    else if (sw == "-reac") {
      reacType = argv[++i];
      if (reacType == "2body") {
	    reacQval       = atof(argv[++i]);   //set Q-value
	    reacQvalSpread = atof(argv[++i]);   //set relative Q-value spread
      }
      else if (reacType == "glaub"    || reacType == "glaub_noDecay") {//do nothing
      }
      else if (reacType == "2neutron" || reacType == "di_neutron" || reacType == "3body_decay") {//do nothing
      }
      else if (reacType == "3neutron" || reacType == "4body_decay"){ //do nothing
	  }
      else if (reacType == "4neutron" || reacType == "5body_decay" || reacType == "5body_seq"){ 
	  // do nothing, it doesn't exist (as in the tetraneutron is fictional, this reaction type DOES exist in the code...)
	}
	else {
	ERR("Unkown reaction type %s", reacType.c_str());
      }
    }

    else if (sw == "-reacA") {
      aDistType = argv[++i];

      if (aDistType == "flat") {
	    reacALowLim = atof(argv[++i]);   //set angle low lim
	    reacAUpLim  = atof(argv[++i]);   //set angle upper lim
      } 

      else if (aDistType == "gauss" || aDistType == "COSgauss") {
	    reacASpread = atof(argv[++i]);   //set angle spread (rad)
	    reacALowLim = atof(argv[++i]);   //set angle low lim (rad)
	    reacAUpLim  = atof(argv[++i]);   //set angle upper lim (rad)
      } 

      else {
	    ERR("Unkown distribution type %s", aDistType.c_str());
      }

      //check for proper ranges
      if (reacALowLim < 0.0 || reacAUpLim > 3.1416) ERR("Bad reacA ranges. Biggest possible range is 0.0 to Pi");
    }

    else if (sw == "-debug") {
      debugSwitch = argv[++i];
    }

    // Total Edecay
    // ___________________________________________________________
    else if (sw == "-e") {                                     // 3
      DecayTotalFilled = 1;
      exType = argv[++i];

      if (exType == "const" || exType == "therm" || exType == "exp" || exType == "Etotal_11Li" || exType == "E_12Li"){
          exPar[0] = atof(argv[++i]); // constant decay energy or temperature or exp coeffic.
      }
      else if (exType == "uniform" || exType == "gauss" || exType == "bw" || exType== "e1" || exType=="swave") {
        exPar[0] = atof(argv[++i]); // lower bound or centroid or E1-threshold or energy
        exPar[1] = atof(argv[++i]); // upper bound, sigma, or gamm
      } 

      else if (exType == "asymbw"){
	    exPar[0]    = atof(argv[++i]); // Decay energy
	    exPar[1]    = atof(argv[++i]); // Observed width
	    exParInt[0] = atof(argv[++i]); // ang momentum of state
	    exParInt[1] = atof(argv[++i]); // Gamma Flag
      } 

      else if (exType == "cust"){
	    cust_file = argv[++i];
      } 

      else if (exType == "TwoNeutron_seq"){
	    exPar[0]    = atof(argv[++i]); // Total Decay Energy Centroid
	    exPar[1]    = atof(argv[++i]); // Total Decay Reduced Width
	    exPar[2]    = atof(argv[++i]); // 1st Decay Reduced Width
	    exPar[3]    = atof(argv[++i]); // 2nd Neutron Decay Energy Centroid
	    exPar[4]    = atof(argv[++i]); // 2nd Decay Reduced Width
	    exParInt[0] = atof(argv[++i]); // ang momentum of state
      } 

      else if (exType == "Volya_2nSeq"){
	    exPar[0]    = atof(argv[++i]); // Energy of Initial State
	    exPar[1]    = atof(argv[++i]); // Threshold Energy
	    exPar[2]    = atof(argv[++i]); // Spec. Factor for I->V
	    exPar[3]    = atof(argv[++i]); // Spec. Factor for V->F
	    exParInt[0] = atof(argv[++i]); // ang momentum
	    exPar[4]    = atof(argv[++i]); // Gamma (width) of initial state
      } 

      else if (exType == "Volya_Dineutron"){
	    exPar[0] = atof(argv[++i]);    // Energy of Initial State
	    exPar[1] = atof(argv[++i]);    // Gamma (widht) of inital state
	    exPar[2] = atof(argv[++i]);    // scattering length of n-n, should be negative (-18.7 fm)
      } 

	else 
        ERR("Unknown excitation energy model %s", exType.c_str());
      }
 
    // First Neutron
    // ____________________________________________________________
    else if (sw == "-e1") {                                     // 4
      DecayOneFilled = 1;
      exType1 = argv[++i];

      if (exType1 == "const" || exType1 == "therm" || exType1 == "exp" || exType1 == "Etotal_11Li" || exType1 == "E_12Li"){
          exPar1[0] = atof(argv[++i]); // constant decay energy or temperature or exp coeffic.
      }

      else if (exType1 == "uniform" || exType1 == "gauss" || exType1 == "bw" || exType1 == "e1" || exType1=="swave"){
        exPar1[0] = atof(argv[++i]); // lower bound or centroid or E1-threshold or energy
        exPar1[1] = atof(argv[++i]); // upper bound, sigma, or gamm
      } 

      else if (exType1 == "asymbw"){
	    exPar1[0]    = atof(argv[++i]); // Decay energy
	    exPar1[1]    = atof(argv[++i]); // Observed width
	    exPar1Int[0] = atof(argv[++i]); // ang momentum of state
	    exPar1Int[1] = atof(argv[++i]); // Gamma Flag
      } 

      else if (exType1 == "cust"){
	    cust_file1 = argv[++i];
      } 

      else if (exType1 == "cos_twoneutron"){// why is there nothing here? I guess its purpose is so that cos_twoneutron does not throw an error... MDJ
      } 

      else {
        ERR("Unknown excitation energy model %s", exType1.c_str());
      }
    }//-e1

    // Second Neutron
    // ____________________________________________________________
    else if (sw == "-e2") {                                     // 5
      DecayTwoFilled = 1;
      exType2 = argv[++i];

      if (exType2 == "const" || exType2 == "therm" || exType2 == "exp" || exType2 == "Etotal_11Li" || exType2 == "E_12Li"){   
        exPar2[0] = atof(argv[++i]); // constant decay energy or temperature or exp coeffic.
      }

      else if (exType2 == "uniform" || exType2 == "gauss" || exType2 == "bw" || exType2 == "e1" || exType2=="swave"){
        exPar2[0] = atof(argv[++i]); // lower bound or centroid or E1-threshold or energy
        exPar2[1] = atof(argv[++i]); // upper bound, sigma, or gamm
      } 

      else if (exType2 == "asymbw"){
	    exPar2[0]    = atof(argv[++i]); // Decay energy
	    exPar2[1]    = atof(argv[++i]); // Observed width
	    exPar2Int[0] = atof(argv[++i]); // ang momentum of state
	    exPar2Int[1] = atof(argv[++i]); // Gamma Flag
      } 

      else if (exType2 == "4body_dineutron"){
	    exPar2[2] = atof(argv[++i]);  // Energy of Initial Dineutron state
	    exPar2[3] = atof(argv[++i]);  // Gamma (width) of Dineutron
	    exPar2[4] = atof(argv[++i]);  // scattering length of n-n, should be -18.7 fm
      }
     
      else if (exType2 == "Volya_2nSeq"){
	    exPar2[0]    = atof(argv[++i]); // Energy of Initial State
	    exPar2[1]    = atof(argv[++i]); // Threshold Energy
	    exPar2[2]    = atof(argv[++i]); // Spec. Factor for I->V
	    exPar2[3]    = atof(argv[++i]); // Spec. Factor for V->F
	    exPar2Int[0] = atof(argv[++i]); // ang momentum
	    exPar2[4]    = atof(argv[++i]); // Gamma (width) of initial state
      }
      //Added by ANK 1-28-2014
      else if (exType2 == "4body_Constrained"){
	   exPar2[0]    = atof(argv[++i]); // Decay energy
	   exPar2[1]    = atof(argv[++i]); // Observed width
	   exPar2Int[0] = atof(argv[++i]); // ang momentum of state
	   exPar2Int[1] = atof(argv[++i]); // Gamma Flag
     }
      else if (exType2 == "4body_PS_Seq"){
	exPar2[0]    = atof(argv[++i]); // Decay energy
	exPar2[1]    = atof(argv[++i]); // Observed width
	exPar2Int[0] = atof(argv[++i]); // ang momentum of state
	exPar2Int[1] = atof(argv[++i]); // Gamma Flag
      }
      ///////////////////////
      else if (exType2 == "cust"){
	    cust_file2 = argv[++i];
      } 

      else if (exType2 == "cos_twoneutron"){
      } 

      else {
        ERR("Unknown excitation energy model %s", exType2.c_str());
      }
    }//-e2

    // Third Neutron
    // ____________________________________________________________
    else if (sw == "-e3") {                                     // 6
      DecayThreeFilled = 1;
      exType3 = argv[++i];

     if (exType3 == "const" || exType3 == "therm" || exType3 == "exp" || exType3 == "Etotal_11Li" || exType3 == "E_12Li"){   
        exPar3[0] = atof(argv[++i]); // constant decay energy or temperature or exp coeffic.
     }

     else if (exType3 == "uniform" || exType3 == "gauss" || exType3 == "bw" || exType3 == "e1" || exType3=="swave") {
       exPar3[0] = atof(argv[++i]); // lower bound or centroid or E1-threshold or energy
       exPar3[1] = atof(argv[++i]); // upper bound, sigma, or gamm
     }

     else if (exType3 == "asymbw"){
	   exPar3[0]    = atof(argv[++i]); // Decay energy
	   exPar3[1]    = atof(argv[++i]); // Observed width
	   exPar3Int[0] = atof(argv[++i]); // ang momentum of state
	   exPar3Int[1] = atof(argv[++i]); // Gamma Flag
     } 

     else if (exType3 == "5body_dineutron"){
	   exPar3[2] = atof(argv[++i]);  // Energy of Initial Dineutron state
	   exPar3[3] = atof(argv[++i]);  // Gamma (width) of Dineutron
	   exPar3[4] = atof(argv[++i]);  // scattering length of n-n, should be -18.7 fm
     }
     else if (exType3 == "5body_Grigorenko"){
	   exPar3[2] = atof(argv[++i]);  // Energy of Initial Dineutron state
	   exPar3[3] = atof(argv[++i]);  // Gamma (width) of Dineutron
	   exPar3[4] = atof(argv[++i]);  // scattering length of n-n, should be -18.7 fm
     }
     else if (exType3 == "5body_Constrained"){
	   exPar3[0]    = atof(argv[++i]); // Decay energy
	   exPar3[1]    = atof(argv[++i]); // Observed width
	   exPar3Int[0] = atof(argv[++i]); // ang momentum of state
	   exPar3Int[1] = atof(argv[++i]); // Gamma Flag
     }


     else if (exType3 == "Volya_2nSeq"){
	   exPar3[0]    = atof(argv[++i]); // Energy of Initial State
	   exPar3[1]    = atof(argv[++i]); // Threshold Energy
	   exPar3[2]    = atof(argv[++i]); // Spec. Factor for I->V
	   exPar3[3]    = atof(argv[++i]); // Spec. Factor for V->F
	   exPar3Int[0] = atof(argv[++i]); // ang momentum
	   exPar3[4]    = atof(argv[++i]); // Gamma (width) of initial state
     }
     /*(else if (exType3 = "4N_3body"){
	   exPar3[0]    = atof(argv[++i]); // Decay energy
	   exPar3[1]    = atof(argv[++i]); // Observed width
	   exPar3Int[0] = atof(argv[++i]); // ang momentum of state
	   exPar3Int[1] = atof(argv[++i]); // Gamma Flag
     }*/


	 else if (exType3 == "cust"){
	   cust_file3 = argv[++i];
     }

     else if (exType3 == "cos_twoneutron"){
     } 

     else {
        ERR("Unknown excitation energy model %s", exType3.c_str());
     }
    }//-e3

    // Fourth Neutron
    // ____________________________________________________________
    else if (sw == "-e4") {                                     // 7
      DecayFourFilled = 1;
      exType4 = argv[++i];

      if (exType4 == "const" || exType4 == "therm" || exType4 == "exp" || exType4 == "Etotal_11Li" || exType4 == "E_12Li"){   
        exPar4[0] = atof(argv[++i]); // constant decay energy or temperature or exp coeffic.
      }

      else if (exType4 == "uniform" || exType4 == "gauss" || exType4 == "bw" || exType4 == "e1" || exType4=="swave") {
        exPar4[0] = atof(argv[++i]); // lower bound or centroid or E1-threshold or energy
        exPar4[1] = atof(argv[++i]); // upper bound, sigma, or gamm
      } 

      else if (exType4 == "asymbw"){
	    exPar4[0]    = atof(argv[++i]); // Decay energy
	    exPar4[1]    = atof(argv[++i]); // Observed width
	    exPar4Int[0] = atof(argv[++i]); // ang momentum of state
	    exPar4Int[1] = atof(argv[++i]); // Gamma Flag
      } 

	  else if (exType4 == "cust"){
	    cust_file4 = argv[++i];
      } 

      else if (exType4 == "cos_twoneutron"){
      } 

      else {
        ERR("Unknown excitation energy model %s", exType4.c_str());
      }
    }//-e4

    // Here we set values based on arguemnts given to st_mona.c when it executes, since this is AFTER reading the input file, 
    // arguments to st_mona have priority. MDJ

    else if (sw == "-be")     		        eBeam            = atof(argv[++i]);      // 4
    else if (sw == "-bA")      	    	    beamA            = atoi(argv[++i]);      // 5
    else if (sw == "-bZ")      		        beamZ            = atoi(argv[++i]);      // 6 
    else if (sw == "-dT")      		        dTarget          = atof(argv[++i]);      // 7
    else if (sw == "-dbe")     		        dEbeam           = atof(argv[++i]);      // 8
    else if (sw == "-rt")      		        resTime          = atof(argv[++i]);      //09
    else if (sw == "-rTx")     		        resTargetX       = atof(argv[++i]);      //10
    else if (sw == "-rx")      		        resCRDC1X        = atof(argv[++i]);      //11
    else if (sw == "-ry")      		        resCRDC1Y        = atof(argv[++i]);      //11
    else if (sw == "-rthx")    		        resCRDC1ThetaX   = atof(argv[++i]);      //12
    else if (sw == "-rthy")    		        resCRDC1ThetaY   = atof(argv[++i]);      //12
    else if (sw == "-rMx1")    		        resMonaX1        = atof(argv[++i]);      //13
    else if (sw == "-rMx2")    		        resMonaX2        = atof(argv[++i]);      //13
    else if (sw == "-rMxP")    		        resMonaP         = atof(argv[++i]);      //13
    else if (sw == "-rMyz")    		        resMonaBar       = atoi(argv[++i]);      //14 
    else if (sw == "-strag")   		        scaleStrag       = atof(argv[++i]);      //15
    else if (sw == "-glaub")   		        scaleGlaub       = atof(argv[++i]);      //16 
    else if (sw == "-nn")      		        nNeutr           = atoi(argv[++i]);      //17
    else if (sw == "-np")      		        nProt            = atoi(argv[++i]);      //18
    else if (sw == "-md")      		        monaDist         = atof(argv[++i]);      //19
    else if (sw == "-cmL")     		        crdc1MaskLeft    = atof(argv[++i]);      //21
    else if (sw == "-cmR")     		        crdc1MaskRight   = atof(argv[++i]);      //22
    else if (sw == "-c2d")     		        crdc2dist        = atof(argv[++i]);      //23
    else if (sw == "-ctx")     		        bSpotCtx         = atof(argv[++i]);      //24 
    else if (sw == "-cx")      		        bSpotCx          = atof(argv[++i]);      //25
    else if (sw == "-cty")     		        bSpotCty         = atof(argv[++i]);      //26   
    else if (sw == "-cy")      		        bSpotCy          = atof(argv[++i]);      //27   
    else if (sw == "-dtx")    		        bSpotDtx         = atof(argv[++i]);      //28   
    else if (sw == "-dx")     		        bSpotDx          = atof(argv[++i]);      //29   
    else if (sw == "-dty")     		        bSpotDty         = atof(argv[++i]);      //30   
    else if (sw == "-dy")     		        bSpotDy          = atof(argv[++i]);      //31  
    else if (sw == "-cxm")                      bSpotCx          = atof(argv[++i])/1000.; // 12/8/17 Added by DV to input centroid in mm. 
    else if (sw == "-ctxm")                     bSpotCtx         = atof(argv[++i])/1000.; // 12/8/17 Added by DV to input centroid in mrad.
    else if (sw == "-sxtxm")                    bSpotSxtx        = atof(argv[++i])/10000.;// 12/8/17 Added by DV to input correlation in smaller steps. 
    else if (sw == "-cym")                      bSpotCy          = atof(argv[++i])/1000.; // 12/8/17 Added by DV to input centroid in mm.     
    else if (sw == "-ctym")                     bSpotCty         = atof(argv[++i])/1000.; // 12/8/17 Added by DV to input centroid in mrad.
    else if (sw == "-sytym")                    bSpotSyty        = atof(argv[++i])/10000.;// 12/8/17 Added by DV to input correlation in smaller steps.  
    //ANK: new paramters from JKS for multiple component incoming beam distribution
    
    else if (sw == "-sxtx")  bSpotSxtx        = atof(argv[++i]);
    else if (sw == "-syty")  bSpotSyty        = atof(argv[++i]);
    else if (sw == "-ctx2")     bSpotCtx2         = atof(argv[++i]);      //24   //
    else if (sw == "-cx2")      bSpotCx2          = atof(argv[++i]);      //25   //
    else if (sw == "-cty2")     bSpotCty2         = atof(argv[++i]);      //26   //
    else if (sw == "-cy2")      bSpotCy2          = atof(argv[++i]);      //27   //
    else if (sw == "-dtx2")     bSpotDtx2         = atof(argv[++i]);      //28   //
    else if (sw == "-dx2")      bSpotDx2          = atof(argv[++i]);      //29   //
    else if (sw == "-dty2")     bSpotDty2         = atof(argv[++i]);      //30   //
    else if (sw == "-fty2")     bSpotFty2         = atof(argv[++i]);      //30   //
    else if (sw == "-dy2")      bSpotDy2          = atof(argv[++i]);      //31   //
    else if (sw == "-fy2")     bSpotFy2         = atof(argv[++i]);      //30   //
    else if (sw == "-sxtx2")  bSpotSxtx2        = atof(argv[++i]);
    else if (sw == "-syty2")  bSpotSyty2        = atof(argv[++i]);
    else if (sw == "-ctx3")     bSpotCtx3         = atof(argv[++i]);      //24   //
    else if (sw == "-cx3")      bSpotCx3          = atof(argv[++i]);      //25   //
    else if (sw == "-cty3")     bSpotCty3         = atof(argv[++i]);      //26   //
    else if (sw == "-cy3")      bSpotCy3          = atof(argv[++i]);      //27   //
    else if (sw == "-dtx3")     bSpotDtx3         = atof(argv[++i]);      //28   //
    else if (sw == "-dx3")      bSpotDx3          = atof(argv[++i]);      //29   //
    else if (sw == "-dty3")     bSpotDty3         = atof(argv[++i]);      //30   //
    else if (sw == "-dy3")      bSpotDy3          = atof(argv[++i]);      //31   //
    else if (sw == "-sxtx3")  bSpotSxtx3        = atof(argv[++i]);
    else if (sw == "-syty3")  bSpotSyty3        = atof(argv[++i]);
    else if (sw == "-sxy3")	bSpotSxy3	= atof(argv[++i]);
    else if (sw == "-ratio")    ratio		= atof(argv[++i]);
    else if (sw == "-scale1")    scale1		= atof(argv[++i]);
    else if (sw == "-scale2")    scale2		= atof(argv[++i]);
    else if (sw == "-scale3")    scale3		= atof(argv[++i]);
    
    ///////////////////////////// ANK
    else if (sw == "-disType") 		        distTypeMx       = atof(argv[++i]);      //32
    else if (sw == "-velocityshift") 	    velocityshift    = atof(argv[++i]);      //33
    else if (sw == "-legendre_lvalue")      legendre_lvalue  = atof(argv[++i]);
    else if (sw == "-phase_space_weight")   phase_space_weight = atof(argv[++i]);
    else if (sw == "-asymMomentum")  	    asymMomentum     = atof(argv[++i]);      //34
    else if (sw == "-sxtx")    		        bSpotSxtx        = atof(argv[++i]);      //35
    else if (sw == "-syty")   		        bSpotSyty        = atof(argv[++i]);      //36 
    else if (sw == "-input")   		        inputfile 	     = argv[++i];
    else if (sw == "-exp") {   		        exp              = argv[++i];// set default values for certain experiment   //20
        
   // List experiment names here
     if (0) ;
     else if ( exp == "03033");                 // Nathans exp; nothing todo, corresponds to global default
     else if ( exp == "03038") 		  	        INFO("Using default values for experiment 03038 (Kiss 7He)");// Kiss 7He      
     else if ( exp == "03038a")                 INFO("Using default values for experiment 03038 (7He NO TARGET)");// Kiss 7He No Target        
     else if ( exp == "03048a")      		    INFO("Using default values for experiment 03048a (12Be g.s. to 10Be)"); // 12Be to 10Be       
     else if ( exp == "03048b")   	            INFO("Using default values for experiment 03048b (11Be Coul to 10Be)");// 11Be to 10Be        
     else if ( exp == "03048c")        	 	    INFO("Using default values for experiment 03048c (mimic 11Be singles)");// 12Be to 11Be mimic 11Be singles     
     else if ( exp == "05039_O25") 	            INFO("Using default values for experiment 05039 (Hoffman 25O) - zwk update");// Hoffman 25O  
     else if ( exp == "05039a")  	            INFO("Using default values for experiment 05039 (Hoffman 25O)"); // Hoffman 24O 
     else if ( exp == "05039b")     	        INFO("Using default values for experiment 05039 (Hoffman 25O)");// Hoffman 23O  
     else if ( exp == "05039c") 	            INFO("Using default values for experiment 05039 (Hoffman 25O)"); // Hoffman 25O    
     else if ( exp == "05124_Li9" )  	        INFO("Using default values for 05124-z3n6 (FRAG II)"); // FRAG II        
     else if ( exp == "05124_O18" ) 	        INFO("Using default values for 05124-z8n10 (FRAG II)");// FRAG II     
     else if ( exp == "05124_O19" )             INFO("Using default values for 05124-z8n11 (FRAG II)");// FRAG II  
     else if ( exp == "05124_O20" )   	        INFO("Using default values for 05124-z8n12 (FRAG II)");// FRAG II 
     else if ( exp == "05124_O21" )             INFO("Using default values for 05124-z8n13 (FRAG II)");// FRAG II 
     else if ( exp == "05124_O22" )        	    INFO("Using default values for 05124-z8n14 (FRAG II)");// FRAG II  
     else if ( exp == "05124a" )       		    INFO("Using default values for 05124-z8n12 (FRAG II)");// FRAG II 
     else if ( exp == "05124b" ) 		        INFO("Using default values for 05124-z5n8 (FRAG II)");// FRAG II 
     else if ( exp == "05124c" )    		    INFO("Using default values for 05124-z10n15 (FRAG II)");// FRAG II       
     else if ( exp == "05124d" )		        INFO("Using default values for 05124-z6n10 (FRAG II)"); // FRAG II 
     else if ( exp == "05124e" )		        INFO("Using default values for 05124-z8n11 (FRAG II)");// FRAG II     
     else if ( exp == "05124g" )		        INFO("Using default values for 05124-Beam (FRAG II)");// FRAG II 
     else if ( exp == "05034a" )		        INFO("Using default values for 05034-14B (11Li) 14B->12Li->11Li+n - zwk update"); // 11Li
     else if ( exp == "05034_Be" ) 		        INFO("Using default values for 05034-14Be Beam (11Li+2n)");// 11Li 
     else if ( exp == "06025_B16" )   		    INFO("Using default values for 06025- B15 decay");// C17 beam, create Be15 
     else if ( exp == "06025_Be16" ) 		    INFO("Using default values for 06025-17B Beam (16Be->14Be+2n)");// 16Be->14Be+2n 
     else if ( exp == "99999")           	    INFO("Using default values for experiment 03048a (12Be g.s. to 10Be)");// 15Be to 14Be, propulated by a (d,p) reaction
     else if ( exp == "99999a")     	        INFO("Using default values for experiment 03048a (12Be g.s. to 10Be)");
     else if ( exp == "08026_O26" ) 	        INFO("Using default values for 08026 Beam (O26->O24+2n)");// 27F -> O26 -> O24+2n 
     
     else if ( exp == "05039naNe26") {  // Hoffman 25O
        INFO("Using default values for experiment 05039 incoming Na beam (Hoffman 25O)");
	    INFO(" 28Ne --> 26Ne + 2n " );
	}

     else if( exp == "09067_He10")		        INFO("Exper Jesse 14Be(-2p2n) --> 10He --> 8He + 2n " );
     else if( exp == "09067_Be14_Inelastic") 	INFO("Exper Jesse 14Be+CD2 --> 14Be*(2+) --> 12Be + 2n " );
     else if( exp == "09028_O24")   		    INFO("Exper 09028(Schiller/Hope): Neutron knockout of 24O" );
     else if( exp == "12004_dp")    		    INFO("Using default values for experiment e12004, d,p reaction"); // 25O to 24O, propulated by a (d,p) reaction
    
     else ERR("Unknown Experiment %s.",exp.c_str());
    }
    else if (sw == "-?" || sw == "--help" || sw == "-help") usage();
    else {  cout << "sw: " << sw.c_str() << endl;
            ERR("Unknown option %s",sw.c_str());}
  }

  if (argc < 2) {
    WAR("No command line options given.  Printing usage information.\n");
    usage();
  }

  // Create outfile, parameter tree and assign branches.

  TFile *f = new TFile(tupleFile.c_str(),"recreate");// assert(outFile);
  TTree *pt = new TTree("pt","Parameter Tree");// assert(tOut);

  pt->Branch("beamA",&beamA,"beamA/I");
  pt->Branch("beamZ",&beamZ,"beamZ/I");
  pt->Branch("nNeutr",&nNeutr,"nNeutr/I");
  pt->Branch("nProt",&nProt,"nProt/I");
  pt->Branch("exParInt_0",&exParInt[0],"exParInt_0/I");
  pt->Branch("DecayOneFilled",&DecayOneFilled,"DecayOneFilleed/I");
  pt->Branch("DecayTwoFilled",&DecayTwoFilled,"DecayTwoFilled/I");
  pt->Branch("DecayThreeFilled",&DecayThreeFilled, "DecayThreeFilled/I");
  pt->Branch("DecayTotalFilled",&DecayTotalFilled,"DecayTotalFilled/I");
  pt->Branch("resMonaBar",&resMonaBar,"resMonaBar/I");

  pt->Branch("exPar_0",&exPar[0],"exPar_0/D");
  pt->Branch("exPar_1",&exPar[1],"exPar_1/D");
  pt->Branch("exPar_2",&exPar[2],"exPar_2/D");
  pt->Branch("exPar_3",&exPar[3],"exPar_3/D");
  pt->Branch("exPar_4",&exPar[4],"exPar_4/D");
  pt->Branch("exPar1_0",&exPar1[0],"exPar1_0/D");
  pt->Branch("exPar1_1",&exPar1[1],"exPar1_1/D");
  pt->Branch("exPar2_0",&exPar2[0],"exPar2_0/D");
  pt->Branch("exPar2_1",&exPar2[1],"exPar2_1/D");
  pt->Branch("exPar3_0",&exPar3[0],"exPar3_0/D");
  pt->Branch("exPar3_1",&exPar3[1],"exPar3_1/D");

  pt->Branch("reacQval",&reacQval,"reacQval/D");
  pt->Branch("reacQvalSpread",&reacQvalSpread,"reacQvalSpread/D");
  pt->Branch("reacASpread",&reacASpread,"reacASpread/D");
  pt->Branch("reacALowLim",&reacALowLim,"reacALowLim/D");
  pt->Branch("reacAUpLim",&reacAUpLim,"reacAUpLim/D");
  pt->Branch("eBeam",&eBeam,"eBeam/D");
  pt->Branch("dEbeam",&dEbeam,"dEbeam/D");
  pt->Branch("dTarget",&dTarget,"dTarget/D");
  pt->Branch("resTime",&resTime,"resTime/D");
  pt->Branch("resTargetX",&resTargetX,"resTargetX/D");
  pt->Branch("resCRDC1X",&resCRDC1X,"resCRDC1X/D");
  pt->Branch("resCRDC1Y",&resCRDC1Y,"resCRDC1Y/D");
  pt->Branch("resCRDC1ThetaX",&resCRDC1ThetaX,"resCRDC1ThetaX/D");
  pt->Branch("resCRDC1ThetaY",&resCRDC1ThetaY,"resCRDC1ThetaY/D");
  pt->Branch("resMonaX1",&resMonaX1,"resMonaX1/D");
  pt->Branch("resMonaX2",&resMonaX2,"resMonaX2/D");
  pt->Branch("resMonaP",&resMonaP,"resMonaP/D");
  pt->Branch("scaleGlaub",&scaleGlaub,"scaleGlaub/D");
  pt->Branch("scaleStrag",&scaleStrag,"scaleStrag/D");
  pt->Branch("bSpotDx",&bSpotDx,"bSpotDx/D");
  pt->Branch("bSpotDy",&bSpotDy,"bSpotDy/D");
  pt->Branch("bSpotDtx",&bSpotDtx,"bSpotDtx/D");
  pt->Branch("bSpotDty",&bSpotDty,"bSpotDty/D");
  pt->Branch("bSpotCtx",&bSpotCtx,"bSpotCtx/D");
  pt->Branch("bSpotCx",&bSpotCx,"bSpotCx/D");
  pt->Branch("bSpotCty",&bSpotCty,"bSpotCty/D");
  pt->Branch("bSpotCy",&bSpotCy,"bSpotCy/D");
  pt->Branch("scale3",&scale3,"scale3/D");
  pt->Branch("crdc2dist",&crdc2dist,"crdc2dist/D");
  pt->Branch("monaDist",&monaDist,"monaDist/D");
  pt->Branch("crdc1MaskLeft",&crdc1MaskLeft,"crdc1MaskLeft/D");
  pt->Branch("crdc1MaskRight",&crdc1MaskRight,"crdc1MaskRight/D");
  pt->Branch("distTypeMx",&distTypeMx,"distTypeMx/D");

  // Write branches to file.
  pt->Fill();
  pt->Write();
  f->Close();

  printf("            Number of events:    %d events.\n",nevents);          			          //  1
  printf("             Writing to file:    %s.\n",tupleFile.c_str());      			              
  printf("     Total Excitation energy:    %s %g %g\n",exType.c_str(), exPar[0], exPar[1]);	      
  printf("       1st Excitation energy:    %s %g %g\n",exType1.c_str(), exPar1[0], exPar1[1]); 	  
  printf("       2nd Excitation energy:    %s %g %g\n",exType2.c_str(), exPar2[0], exPar2[1]); 	  
  printf("       3rd Excitation energy:    %s %g %g\n",exType3.c_str(), exPar3[0], exPar3[1]);
  printf("                    beam A-Z:    %d-%-3d\n",beamA,beamZ);       			              
  printf("            target thickness:    %g mg/cm^2\n",dTarget);       			              
  printf("                 beam energy:    %g MeV/u\n",eBeam);           			              
  printf("          beam energy spread:    %g \n",dEbeam);               			              
  printf("         Angular strag scale:    %g \n",scaleStrag);           			        
  printf("          Glauber Kick scale:    %g \n",scaleGlaub);           			  
  printf("   Velocity & AsymMom scales:    %g, %g \n",velocityshift, asymMomentum);          	  
  printf("    protons, neutron removed:    %d,%d \n",nProt, nNeutr);       			  
  printf("         beam spot x,y,tx,ty:    %g, %g, %g, %g\n",bSpotDx, bSpotDy,bSpotDtx,bSpotDty);
  printf("      beam profile x,y,tx,ty:    %g, %g, %g, %g\n",bSpotCx, bSpotCy,bSpotCtx,bSpotCty);
  printf("\n");
  printf("Resolutions are:\n"); 
  printf("                        time:    %g ns \n",resTime);           			   
  printf("                    Target x:    %g m\n",resTargetX);         			  
  printf("                     CRDC1 X:    %g m\n",resCRDC1X);          			  
  printf("                     CRDC1 Y:    %g m\n",resCRDC1Y);          			  
  printf("                CRDC1 ThetaX:    %g rad\n",resCRDC1ThetaX);   			  
  printf("                CRDC1 ThetaY:    %g rad\n",resCRDC1ThetaY);   			  
  printf("                     MoNA X1:    %g m\n",resMonaX1);            			  
  printf("                     MoNA X2:    %g m\n",resMonaX2);           			  
  printf("              MoNA X percent:    %g\n",resMonaP);            				  
  printf("     MoNA bar (0/1 = no/yes):    %d \n",resMonaBar);             			  
  printf("                  crdc2 Dist:    %g\n",crdc2dist);            			  
  printf("                      MoNA z:    %g\n",monaDist);              			  
  printf("          crdc1 left blocker:    %g\n",crdc1MaskLeft);         			  
  printf("         crdc1 right blocker:    %g\n",crdc1MaskRight);       			  
  printf("        Type of Mona X Dist.:    %g\n",distTypeMx);           			  
  printf("CRDC2 Mask(right,left,top,bot):  %g, %g, %g, %g\n",crdc2MaskRight,crdc2MaskLeft,crdc2MaskTop,crdc2MaskBot);
  printf("\n");

  // system
  StSystem sys;

  // a particle (the beam)
  StParticle p;

  // set A, Z and Q
  p.setA(beamA);               // mass number A
  p.setZ(beamZ);               // proton number Z
  p.setQ(p.getZ());            // charge Q
  p.setm(p.getA()*931.494028); // mass m

  // add one particle to the system
  sys.addParticle(p);

  // GSL random number generator
  StGSLrng* r = new StGSLrng();

  // RNGs for x, y, tx, ty and energy
  // StRNG *a = new StRNGUniformGSL(r,0,1)
  // StRNG *theta = new StRNGUniformGSL(r,0,2*3.1415926);
    
  StRNG* rx  =  new StRNGGaussGSL(r, bSpotCx, bSpotDx);  
  StRNG* ry  =  new StRNGGaussGSL(r, bSpotCy, bSpotDy);
             
  StRNG* rtx = new StRNGGaussGSL(r, bSpotCtx, bSpotDtx /*.01*/);                  
  StRNG* rty = new StRNGGaussGSL(r, bSpotCty, bSpotDty /*.0015*/);   
  
 //ANK: 6-1-2016 added to have multiple component distribution for incoming beam
  
  StRNG* rx2  = new StRNGGaussGSL(r, bSpotCx2, bSpotDx2 /*.005*/);                   ///////<<<<<<<<<<<<<<<<<<<<<<
  //StRNG* rx2  = new StRNGExpPowerGSL(r, bSpotCx2, bSpotDx2 /*.005*/);                   ///////<<<<<<<<<<<<<<<<<<<<<<
  StRNG* rty2  = new StRNGExpPowerGSL(r, bSpotDty2, bSpotFty2 ,bSpotCty2 /*.012*/);                   ///////<<<<<<<<<<<<<<<<<<<<<<
  StRNG* ry2  = new StRNGExpPowerGSL(r, bSpotDy2, bSpotFy2, bSpotCy2 /*.012*/);                   ///////<<<<<<<<<<<<<<<<<<<<<<
  StRNG* rtx2 = new StRNGGaussGSL(r, bSpotCtx2, bSpotDtx2 /*.01*/);                  ///////<<<<<<<<<<<<<<<<<<<<<<

  //	StRNG* rty2 = new StRNGGaussGSL(r, bSpotCty2, bSpotDty2 /*.0015*/);                ///////<<<<<<<<<<<<<<<<<<<<<<
 
  StRNG* rx3  = new StRNGGaussGSL(r, bSpotCx3, bSpotDx3 /*.005*/);                   ///////<<<<<<<<<<<<<<<<<<<<<<
  StRNG* ry3  = new StRNGGaussGSL(r, bSpotCy3, bSpotDy3 /*.012*/);                   ///////<<<<<<<<<<<<<<<<<<<<<<
  StRNG* rtx3 = new StRNGGaussGSL(r, bSpotCtx3, bSpotDtx3 /*.01*/);                  ///////<<<<<<<<<<<<<<<<<<<<<<
  StRNG* rty3 = new StRNGGaussGSL(r, bSpotCty3, bSpotDty3 /*.0015*/);


  ////////////////// ANK

  // 11/30/17, DV changed incoming beam energy distribution from Gaussian to uniform.
  //StRNG* re  = new StRNGGaussGSL(r, eBeam*p.getA(), eBeam*p.getA()*dEbeam);
  StRNG* re  = new StRNGUniformGSL(r, eBeam*p.getA()*(1.-dEbeam), eBeam*p.getA()*(1.+dEbeam) );

  StRNGUniform* rUniform = new StRNGUniformGSL(r);
  //added for 2body angular distribution manipulation
  
  // elements needed for straggling target
  // They have no bearing on energy loss
  StElement Au197;
  Au197.setZ(79);
  Au197.setm(197);   /// \todo need exact mass here

  StElement Ni56;
  Ni56.setZ(28);
  Ni56.setm(56);

  StElement D2;
  D2.setZ(1);
  D2.setm(2.014102);
  
  StElement Be9;
  Be9.setZ(4);
  Be9.setm(9.012182);  
                   
  // Here we Set Target Material for energy loss with CycSrim.
  // Alternatively you can define it in the if statements below, as long as it exists before the StRageny instantiation
  // The default is Beryllium. 	   
 
    CycSrim *srimTarget = new CycSrim(CycSrim::SrimMaterialBeryllium,dTarget, CycSrim::kUnitsMgCm2);
    //cout << "srimTarget= " << srimTarget << "dTarget= " << dTarget << endl;
   
  if( strcmp(target_material.c_str(), "Deuterium") ==0){
    
    CycSrim *srimTarget = new CycSrim(CycSrim::SrimMaterialDeuterium,dTarget, CycSrim::kUnitsMgCm2);

  }

  else if( strcmp(target_material.c_str(), "Nickel") ==0){
    CycSrim *srimTarget = new CycSrim(CycSrim::SrimMaterialNickel,dTarget, CycSrim::kUnitsMgCm2);  

  }

  // TODO: Add flag for removing/including foils and edit st_par_mat_ia and st_material.cc appropriately!
  //       As it stands, the foil and bulge are ALWAYS present, but can be made infitesimally small.
  //       (But must be finite). MDJ
  // Kapton Foil
    CycSrim *srimFoil = new CycSrim(CycSrim::SrimMaterialKapton,17,CycSrim::kUnitsMgCm2);
	
	
  // Set target geometry  
    StMaterial* matTarget = new StMaterial();

	matTarget->setRngbSpotx(rx); 
    matTarget->setRngbSpoty(ry);
	matTarget->setBspotx(bSpotDx);//bSpotCx
	matTarget->setBspoty(bSpotDy);
 	matTarget->setBulge(maxbulge);
    matTarget->setTfoil(tflatfoil);

   // The following if statements have no bearing on the energy-loss calculation, but determine straggling
	if (exp == "03048b" || exp == "03048c"){
	  INFO("using gold Target.\n");  // target material set to Gold but still name matTarget
	  matTarget->addElement(Au197);
	  matTarget->setThickness(dTarget);      
    } 

   	else if (exp == "99999" || exp == "09067_He10" || exp == "09067_Be14_Inelastic"){ 
	  INFO("using an effective Be9 Target instead of CD2.\n");
	  matTarget->addElement(Be9);
	  matTarget->setThickness(dTarget*1.115);          
    } 

   	else if(exp == "12004_dp"){
	  INFO("using deuterium target.\n");
      matTarget->addElement(D2);
      matTarget->setThickness(dTarget);          
    } 

   	else if (1){
	  INFO("using Be Target.\n");
	  matTarget->addElement(Be9);
	  matTarget->setThickness(dTarget);      
    }
                  
        // This creates a rageny object which references the CycSrim class, it was easier to just replace do_de with a CycSrim value. MDJ
		StRageny* rageny;
		rageny = new StRageny(1); // always use interpolator!!!

		rageny->setRngUniform(new StRNGUniformGSL(r));
		rageny->setRngGauss(new StRNGGaussGSL(r));
        rageny->setCycSrim(srimTarget); // set CycSrim target
        rageny->setSrimFoil(srimFoil);

		matTarget->setInteraction(rageny);
		matTarget->setStragAngScale(scaleStrag);              


		//--zwk--Momentum distribution--
		StRNG* rMomDist(0);
		bool kUseMomDist = false;

		if(momType.Contains("para") || momType.Contains("perp") ){
		  rMomDist = new StRNGCustomGSL(r, momFile);
		  kUseMomDist = true;
		  if(momType.Contains("perp")) cerr << "Currently NO support for perp mom. distributions!!" << endl;
		}

		// reaction (in Be target)
		StRNG* rReac(0);
		     if (exType == "const")          rReac = new StRNGGaussGSL(r,exPar[0],0);
		else if (exType == "therm")          rReac = new StRNGGammaGSL(r,1.5,exPar[0]);
		else if (exType == "e1")             rReac = new StRNGCustomE1(r,exPar[0],exPar[1]);
		else if (exType == "uniform")        rReac = new StRNGUniformGSL(r,exPar[0],exPar[1]);
		else if (exType == "gauss")          rReac = new StRNGGaussGSL(r,exPar[0],exPar[1]);
		else if (exType == "exp")            rReac = new StRNGExpGSL(r, 1. / exPar[0]);
		else if (exType == "Etotal_11Li")    rReac = new StRNGEtotal_11LiGSL(r, exPar[0]);
		else if (exType == "E_12Li")         rReac = new StRNGE_12LiGSL(r, exPar[0]);
		else if (exType == "asymbw")         rReac = new StRNGAsymmetricBWGSL(r,exPar[0],exPar[1],exParInt[0],beamA - nProt - nNeutr - 1,exParInt[1]);
                                             // The fourth arguement here is the fragment mass
		else if (exType == "swave")          rReac = new StRNGswave(r,exPar[0],exPar[1]); //added by JKS 11 July 2011 for swave lineshape
		else if (exType == "cust")           rReac = new StRNGCustomGSL(r,cust_file);
		else if (exType == "bw") {
		  StRNGCauchyGSL* rReacCauchy = new StRNGCauchyGSL(r,exPar[0],exPar[1]);
		  rReacCauchy -> setGamma(exPar[1]); // set Gamma
		  rReac = rReacCauchy;
		} 
        else if (exType == "TwoNeutron_seq")   rReac = new StRNGTwoNeutron_seqGSL(r,exPar[0],exPar[1],exPar[2],exPar[3],exPar[4],exParInt[0],beamA,nNeutr,nProt); 
		else if (exType == "Volya_2nSeq")      rReac = new StRNGVolya_2nseqGSL(r,exPar[0],exPar[1],exPar[2],exPar[3],exParInt[0], exPar[4],(beamA-nNeutr-nProt));  
		else if (exType == "Volya_Dineutron")  rReac = new StRNGVolya_DineutronGSL(r,exPar[0],exPar[1],exPar[2],(beamA-nNeutr-nProt));
        //else if (exType == "4N_3body")         rReac = new StRNG
		else if (exType == "5body_dineutron")  rReac = new StRNGVolya_DineutronGSL(r,exPar[2],exPar[3],exPar[4],(beamA-nNeutr-2-nProt));
                                               //Make 12He, (nNeutr = 0), dissociate 2n by thermal then di-neutron emission

		// First Neutron
		StRNG* rReac1(0);
		     if (exType1 == "const")          rReac1 = new StRNGGaussGSL(r,exPar1[0],0);
		else if (exType1 == "therm")          rReac1 = new StRNGGammaGSL(r,1.5,exPar1[0]);
		else if (exType1 == "e1")             rReac1 = new StRNGCustomE1(r,exPar1[0],exPar1[1]);
		else if (exType1 == "uniform")        rReac1 = new StRNGUniformGSL(r,exPar1[0],exPar1[1]);
		else if (exType1 == "gauss")          rReac1 = new StRNGGaussGSL(r,exPar1[0],exPar1[1]);
		else if (exType1 == "cos_twoneutron") rReac1 = new StRNGCos_twoneutronGSL(r,0,1);
		else if (exType1 == "exp")            rReac1 = new StRNGExpGSL(r, 1. / exPar1[0]);
		else if (exType1 == "Etotal_11Li")    rReac1 = new StRNGEtotal_11LiGSL(r, exPar[0]);
		else if (exType1 == "E_12Li")         rReac1 = new StRNGE_12LiGSL(r, exPar[0]);
		else if (exType1 == "asymbw")         rReac1 = new StRNGAsymmetricBWGSL(r,exPar1[0],exPar1[1],exPar1Int[0],beamA - nProt - nNeutr - 1,exPar1Int[1]);
		else if (exType1 == "cust")           rReac1 = new StRNGCustomGSL(r,cust_file1);   
		else if (exType1 == "swave")          rReac1 = new StRNGswave(r,exPar1[0],exPar1[1]); //added by JKS 11 July 2011 for swave lineshape  
		else if (exType1 == "bw") {
		  StRNGCauchyGSL* rReacCauchy = new StRNGCauchyGSL(r,exPar1[0],exPar1[1]);
		  rReacCauchy -> setGamma(exPar1[1]); // set Gamma
		  rReac1 = rReacCauchy;
		}

		// Second Neutron
		StRNG* rReac2(0);
		     if (exType2 == "const")          rReac2 = new StRNGGaussGSL(r,exPar2[0],0);
		else if (exType2 == "therm")          rReac2 = new StRNGGammaGSL(r,1.5,exPar2[0]);
		else if (exType2 == "e1")             rReac2 = new StRNGCustomE1(r,exPar2[0],exPar2[1]);
		else if (exType2 == "uniform")        rReac2 = new StRNGUniformGSL(r,exPar2[0],exPar2[1]);
		else if (exType2 == "gauss")          rReac2 = new StRNGGaussGSL(r,exPar2[0],exPar2[1]);
		else if (exType2 == "cos_twoneutron") rReac2 = new StRNGCos_twoneutronGSL(r,0,1);
		else if (exType2 == "exp")            rReac2 = new StRNGExpGSL(r, 1. / exPar2[0]);
		else if (exType2 == "Etotal_11Li")    rReac2 = new StRNGEtotal_11LiGSL(r, exPar[0]);
		else if (exType2 == "E_12Li")         rReac2 = new StRNGE_12LiGSL(r, exPar[0]);
		else if (exType2 == "asymbw")         rReac2 = new StRNGAsymmetricBWGSL(r,exPar2[0],exPar2[1],exPar2Int[0],beamA - nProt - nNeutr - 2,exPar2Int[1]);
                                              // Here we lose 1 nuetron before fragment decays, hence -2
		else if (exType2 == "cust")           rReac2 = new StRNGCustomGSL(r,cust_file2);
		else if (exType2 == "swave")          rReac2 = new StRNGswave(r,exPar2[0],exPar2[1]); //added by JKS 11 July 2011 for swave lineshape 
		else if (exType2 == "bw") {
		  StRNGCauchyGSL* rReacCauchy = new StRNGCauchyGSL(r,exPar2[0],exPar2[1]);
		  rReacCauchy -> setGamma(exPar2[1]); // set Gamma
		  rReac2 = rReacCauchy;
		} 
		else if (exType2 == "4body_dineutron") rReac2 = new StRNGVolya_DineutronGSL(r,exPar2[2],exPar2[3],exPar2[4],(beamA-nNeutr-1-nProt));
        else if (exType2 == "Volya_2nSeq")     rReac2 = new StRNGVolya_2nseqGSL(r,exPar2[0],exPar2[1],exPar2[2],exPar2[3],exPar2Int[0], exPar2[4],(beamA-nNeutr-1-nProt)); 
		     //Added by ANK 1-28-2014
		else if (exType2 == "4body_Constrained") rReac2 = new StRNGAsymmetricBWGSL(r,exPar2[0],exPar2[1],exPar2Int[0],beamA - nProt - nNeutr - 2,exPar2Int[1]);
		        //Added by ANK 11-11-2015
		else if (exType2 == "4body_PS_Seq") rReac2 = new StRNGAsymmetricBWGSL(r,exPar2[0],exPar2[1],exPar2Int[0],beamA - nProt - nNeutr - 3,exPar2Int[1]);
		     /////////////////////////

        // Third Neutron
		StRNG* rReac3(0);
		     if (exType3 == "const")          rReac3 = new StRNGGaussGSL(r,exPar3[0],0);
		else if (exType3 == "therm")          rReac3 = new StRNGGammaGSL(r,1.5,exPar3[0]);
		else if (exType3 == "e1")             rReac3 = new StRNGCustomE1(r,exPar3[0],exPar3[1]);
		else if (exType3 == "uniform")        rReac3 = new StRNGUniformGSL(r,exPar3[0],exPar3[1]);
		else if (exType3 == "gauss")          rReac3 = new StRNGGaussGSL(r,exPar3[0],exPar3[1]);
		else if (exType3 == "cos_twoneutron") rReac3 = new StRNGCos_twoneutronGSL(r,0,1);
		else if (exType3 == "exp")            rReac3 = new StRNGExpGSL(r, 1. / exPar3[0]);
		else if (exType3 == "Etotal_11Li")    rReac3 = new StRNGEtotal_11LiGSL(r, exPar[0]);
		else if (exType3 == "E_12Li")         rReac3 = new StRNGE_12LiGSL(r, exPar[0]);
		else if (exType3 == "asymbw")         rReac3 = new StRNGAsymmetricBWGSL(r,exPar3[0],exPar3[1],exPar3Int[0],beamA - nProt - nNeutr - 3,exPar3Int[1]);
                                              // Here we lose 2 neutrons before fragment decays, hence -3
		else if (exType3 == "cust")           rReac3 = new StRNGCustomGSL(r,cust_file3);
		else if (exType3 == "swave")          rReac3 = new StRNGswave(r,exPar3[0],exPar3[1]); //added by JKS 11 July 2011 for swave lineshape 
		else if (exType3 == "bw") {
		  StRNGCauchyGSL* rReacCauchy = new StRNGCauchyGSL(r,exPar3[0],exPar3[1]);
		  rReacCauchy -> setGamma(exPar3[1]);  // set Gamma
		  rReac3 = rReacCauchy;
		} 
		else if (exType3 == "5body_dineutron")  rReac3 = new StRNGVolya_DineutronGSL(r,exPar3[2],exPar3[3],exPar3[4],(beamA-nNeutr-2-nProt));
		else if (exType3 == "5body_Grigorenko") rReac3 = new StRNGVolya_DineutronGSL(r,exPar3[2],exPar3[3],exPar3[4],(beamA-nNeutr-2-nProt));
        	else if (exType3 == "Volya_2nSeq")      rReac3 = new StRNGVolya_2nseqGSL(r,exPar3[0],exPar3[1],exPar3[2],exPar3[3],exPar3Int[0], exPar3[4],(beamA-nNeutr-2-nProt)); 
 		else if (exType3 == "5body_Constrained") rReac3 = new StRNGAsymmetricBWGSL(r,exPar3[0],exPar3[1],exPar3Int[0],beamA - nProt - nNeutr - 3,exPar3Int[1]);

                                                // Mass here is of the compound nucleus, BEFORE the decay.
 
        // Fourth Neutron
		StRNG* rReac4(0);
		     if (exType4 == "const")          rReac4 = new StRNGGaussGSL(r,exPar4[0],0);
		else if (exType4 == "therm")          rReac4 = new StRNGGammaGSL(r,1.5,exPar4[0]);
		else if (exType4 == "e1")             rReac4 = new StRNGCustomE1(r,exPar4[0],exPar4[1]);
		else if (exType4 == "uniform")        rReac4 = new StRNGUniformGSL(r,exPar4[0],exPar4[1]);
		else if (exType4 == "gauss")          rReac4 = new StRNGGaussGSL(r,exPar4[0],exPar4[1]);
		else if (exType4 == "cos_twoneutron") rReac4 = new StRNGCos_twoneutronGSL(r,0,1);
		else if (exType4 == "exp")            rReac4 = new StRNGExpGSL(r, 1. / exPar4[0]);
		else if (exType4 == "Etotal_11Li")    rReac4 = new StRNGEtotal_11LiGSL(r, exPar[0]);
		else if (exType4 == "E_12Li")         rReac4 = new StRNGE_12LiGSL(r, exPar[0]);
		else if (exType4 == "asymbw")         rReac4 = new StRNGAsymmetricBWGSL(r,exPar4[0],exPar4[1],exPar4Int[0],beamA - nProt - nNeutr - 4,exPar4Int[1]);
                                              // Here we lose 3 neutrons before fragment decays, hence -4
		else if (exType3 == "cust")           rReac4 = new StRNGCustomGSL(r,cust_file4);
		else if (exType4 == "swave")          rReac4 = new StRNGswave(r,exPar4[0],exPar4[1]); //added by JKS 11 July 2011 for swave lineshape 
		else if (exType4 == "bw") {
		  StRNGCauchyGSL* rReacCauchy = new StRNGCauchyGSL(r,exPar4[0],exPar4[1]);
		  rReacCauchy -> setGamma(exPar4[1]); // set Gamma
		  rReac4 = rReacCauchy;
		} 
 


    // Here we set the reaction class to use, there are 1-4n reaction classes (two - five body decays).
    // 
    // ================================= 1 NEUTRON REACTIONS =====================================
    //
	if (reacType == "2body") {  //use two-body reaction class

		  StReacStripEvapTwoBody* reac = new StReacStripEvapTwoBody(0.1, nNeutr, nProt, rReac);

		  reac->setDebugSwitch(debugSwitch);
		  reac->setRngUniform(rUniform);
		  reac->setRngGaussQvalue(new  StRNGGaussGSL(r,0,1));
		  reac->setQval(reacQval);
		  reac->setQspread(reacQvalSpread);
		  reac->setAspread(reacASpread);

		  matTarget->addReaction(reac);
		  reac->setLowLim(reacALowLim);
		  reac->setUpLim(reacAUpLim);

          if (aDistType == "flat"){    
              reac->setDistSwitch(0);
		  }

		  else if (aDistType == "gauss"){
		    reac->setRngGaussGlauber(new StRNGGaussGSL(r,0.,reacASpread));
		    reac->setDistSwitch(1);
		  }

		  else if (aDistType == "COSgauss"){
		    reacASpread = 1-cos(reacASpread);
		    reac->setRngGaussGlauber(new StRNGGaussGSL(r,1.,reacASpread));
		    reac->setDistSwitch(2);
		  }
		} 
		
	else if (reacType == "d,p") {  // use d,p reaction class 8/22/12 MDJ
	  StReacDP* reac       = new StReacDP(0.1, nNeutr, nProt, rReac);
	 
      reac->setDebugSwitch(debugSwitch);
	  reac->setRngUniform(rUniform);
      
      StRNGangular_distribution_dp* rAngDist = new StRNGangular_distribution_dpGSL(r); //angular distribution used for dp reaction JDS
	  reac->setRngAngDist(rAngDist);
	  reac->setRngGaussGlauber(new StRNGGaussGSL(r,0,1));
	  reac->setGlauberKickScale(scaleGlaub);
	 
      matTarget->addReaction(reac);
	} 

    //
    // ================================= 4 NEUTRON REACTIONS =====================================
    //  
	else if(reacType == "5body_decay"){
	  INFO("5body-decay (frag-n-n-n-n) reaction chosen");

	  StReacStripEvap_5bodydecay* reac = new StReacStripEvap_5bodydecay(0.1, nNeutr, nProt, rReac, rReac1, rReac2, rReac3, rReac4, WhichEnergyIsReconstruct);
		    
      reac->setDebugSwitch(debugSwitch);
	  reac->setRngUniform(rUniform);
	  reac->setRngGaussGlauber(new StRNGGaussGSL(r,0,1));
	  reac->setGlauberKickScale(scaleGlaub);
	  reac->setVelocityScale(velocityshift);
	  reac->setAsymMomentum(asymMomentum);
		    
      matTarget->addReaction(reac);
            
	}
	
    else if(reacType == "5body_seq"){
	  
        if( exType3 == "5body_dineutron")	{
          WhichEnergyIsReconstruct = "volyadineutron";
        }

        else if( exType3 == "5body_Grigorenko"){
            WhichEnergyIsReconstruct = "grigorenko";// goes to di-neutron
                }
	else if( exType3 == "5body_Constrained"){
	   WhichEnergyIsReconstruct = "5body_constrained";// defaults to a 3body phase space decay
	}

        else{
          WhichEnergyIsReconstruct = "4N_3body";

        }

	  INFO("5body-decay sequential reaction chosen, Reconstructing: %s", WhichEnergyIsReconstruct.c_str());

	  StReacSequential_5bodydecay* reac = new StReacSequential_5bodydecay(0.1, nNeutr, nProt, rReac, rReac1, rReac2, rReac3, rReac4, WhichEnergyIsReconstruct);

      reac->setDebugSwitch(debugSwitch);
	  reac->setRngUniform(rUniform);
	  reac->setRngGaussGlauber(new StRNGGaussGSL(r,0,1));
	  reac->setGlauberKickScale(scaleGlaub);
	  reac->setVelocityScale(velocityshift);
	  reac->setAsymMomentum(asymMomentum);
		  
      matTarget->addReaction(reac);

    }

	else if(reacType == "4neutron"){
	  
        if(DecayOneFilled == 1 && DecayTwoFilled == 1 && DecayThreeFilled == 1 && DecayFourFilled ==1){ 
          WhichEnergyIsReconstruct = "Reconstruct4thEnergy";
        }

        if(legendre_lvalue > 0)  WhichEnergyIsReconstruct = "CorrelatedSequential";
      
        if( exType3 == "Volya_2nSeq")    WhichEnergyIsReconstruct = "volya2n";
      
      INFO("4 Neutron Decay Selected.");

	  StReacStripEvap4neutron* reac = new StReacStripEvap4neutron(0.1, nNeutr, nProt, rReac, rReac1, rReac2, rReac3, rReac4, WhichEnergyIsReconstruct);
	
      reac->setDebugSwitch(debugSwitch);
	  reac->setRngUniform(rUniform);
	  reac->setRngGaussGlauber(new StRNGGaussGSL(r,0,1));
	  reac->setGlauberKickScale(scaleGlaub);
	  reac->setVelocityScale(velocityshift);
	  reac->setAsymMomentum(asymMomentum);

      StRNGLegendre *rAngDist = new StRNGLegendreGSL(r,legendre_lvalue); // StRNGLegendreGSL(random number, L value)
      reac->setRngLegendre(rAngDist);

	  matTarget->addReaction(reac);
		
	} 

    //
    // ================================= 3 NEUTRON REACTIONS =====================================
    // 

	else if(reacType == "4body_decay"){

      //Added by ANK 1-28-2014
      if( exType2 == "4body_Constrained"){
	   WhichEnergyIsReconstruct = "4body_constrained";
	}
      //Added by ANK 11-11-2015
      if( exType2 == "4body_PS_Seq"){
	   WhichEnergyIsReconstruct = "4body_PS_Seq";
	}

      INFO("4body-decay (frag-n-n-n) reaction chosen");
	
      StReacStripEvap_4bodydecay* reac = new StReacStripEvap_4bodydecay(0.1, nNeutr, nProt, rReac, rReac1, rReac2, rReac3,  WhichEnergyIsReconstruct);
	
      reac->setDebugSwitch(debugSwitch);
	  reac->setRngUniform(rUniform);
      reac->setRngGaussGlauber(new StRNGGaussGSL(r,0,1));
      reac->setGlauberKickScale(scaleGlaub);
	  reac->setVelocityScale(velocityshift);
	  reac->setAsymMomentum(asymMomentum);
	
      matTarget->addReaction(reac);


	  ///////////////////////////
	}


	else if(reacType == "3neutron"){// use 3n class
      
        if(DecayOneFilled == 1 && DecayTwoFilled == 1 && DecayThreeFilled == 1){
          WhichEnergyIsReconstruct = "Reconstruct3rdEnergy";   
        }

        if( exType2 == "Volya_2nSeq" && legendre_lvalue >=0)    WhichEnergyIsReconstruct = "volya2n_corr";
        if( exType2 == "Volya_2nSeq" && legendre_lvalue < 0)    WhichEnergyIsReconstruct = "volya2n";
        if( exType2 == "4body_dineutron") WhichEnergyIsReconstruct = "volyadineutron";
	//if( exType2 == "4body_Constrained") WhichEnergyIsReconstruct = "4body_constrained";
      
      INFO("3 Neutron Decay Selected %s", WhichEnergyIsReconstruct.c_str());

	  StReacStripEvap3neutron* reac = new StReacStripEvap3neutron(0.1, nNeutr, nProt, rReac, rReac1, rReac2, rReac3, WhichEnergyIsReconstruct);
     
      reac->setDebugSwitch(debugSwitch);
	  reac->setRngUniform(rUniform);
	  reac->setRngGaussGlauber(new StRNGGaussGSL(r,0,1));
	  reac->setGlauberKickScale(scaleGlaub);
	  reac->setVelocityScale(velocityshift);
	  reac->setAsymMomentum(asymMomentum);

      StRNGLegendre *rAngDist = new StRNGLegendreGSL(r,legendre_lvalue);// StRNGLegendreGSL(random number, L value)
      reac->setRngLegendre(rAngDist);

	   
      matTarget->addReaction(reac);
		 			
    } 
    //
    // ================================= 2 NEUTRON REACTIONS =====================================
    // 

	else if (reacType == "no reaction") {}      
		 
	else if (reacType == "2neutron" || reacType == "di_neutron" || reacType == "3body_decay"){ //use 2 Neutron decay class

		  if (DecayOneFilled == 1 && DecayTotalFilled == 1){
		      WhichEnergyIsReconstruct = "Reconstruct2ndEnergy";
		      } 

		  else if (DecayTwoFilled == 1 && DecayTotalFilled == 1){
            WhichEnergyIsReconstruct = "Reconstruct1stEnergy";
		      
            if (exType == "TwoNeutron_seq") {
                WhichEnergyIsReconstruct = "TwoNeutron_seq";  
            }    
          }

		  else {
              WhichEnergyIsReconstruct = "ReconstructTotalEnergy";  
          }
	   
		  if( exType1 == "cos_twoneutron") 	WhichEnergyIsReconstruct = "cos_2nd";
		  if( exType2 == "cos_twoneutron") 	WhichEnergyIsReconstruct = "cos_1st";
		  if( exType  == "Volya_2nSeq")		WhichEnergyIsReconstruct = "volya2n";
		  if( exType  == "Volya_Dineutron")	WhichEnergyIsReconstruct = "volyadineutron";

		  	
		  if(reacType == "2neutron"){
              INFO("2 Neutron Decay Selected");
	         
              StReacStripEvap2neutron* reac = new StReacStripEvap2neutron(0.1, nNeutr, nProt, rReac, rReac1, rReac2, WhichEnergyIsReconstruct);
		    
              reac->setDebugSwitch(debugSwitch);
		      reac->setRngUniform(rUniform);
		      reac->setRngGaussGlauber(new StRNGGaussGSL(r,0,1));
		      reac->setGlauberKickScale(scaleGlaub);
		      reac->setVelocityScale(velocityshift);
		      reac->setAsymMomentum(asymMomentum);
                cout << "Using L Value: " << legendre_lvalue << endl;
              StRNGLegendre *rAngDist = new StRNGLegendreGSL(r,legendre_lvalue);// StRNGLegendreGSL(random number, L value)
              reac->setRngLegendre(rAngDist);

              matTarget->addReaction(reac);
		  
          }
		  else if(reacType == "di_neutron"){
		    INFO("Di-neutron Reaction Chosen");

            StReacStripEvap_dineutron* reac = new StReacStripEvap_dineutron(0.1, nNeutr, nProt, rReac, rReac1, rReac2, WhichEnergyIsReconstruct);
		    
            reac->setDebugSwitch(debugSwitch);
		    reac->setRngUniform(rUniform);
		    reac->setRngGaussGlauber(new StRNGGaussGSL(r,0,1));
		    reac->setGlauberKickScale(scaleGlaub);
		    reac->setVelocityScale(velocityshift);
		    reac->setAsymMomentum(asymMomentum);
		    
            matTarget->addReaction(reac);
		  
          }
		  else if(reacType == "3body_decay"){
		    cout << "3body-decay (frag-n-n) reaction chosen" << endl;
		    StReacStripEvap_3bodydecay* reac = new StReacStripEvap_3bodydecay(0.1, nNeutr, nProt, rReac, rReac1, rReac2, WhichEnergyIsReconstruct, phase_space_weight);
		    reac->setDebugSwitch(debugSwitch);
		    reac->setRngUniform(rUniform);
		    //reac->setRngTwoNeutron_seq(rTwoNeutron_seq);
		    reac->setRngGaussGlauber(new StRNGGaussGSL(r,0,1));
		    reac->setGlauberKickScale(scaleGlaub);
		    reac->setVelocityScale(velocityshift);
		    reac->setAsymMomentum(asymMomentum);
		    matTarget->addReaction(reac);
		  }

		} 
    // 
    // ================================= MISCELLANEOUS =====================================
    //
		
    else if (reacType == "glaub_noDecay" ){
	  StReacGlaub_noDecay* reac       = new StReacGlaub_noDecay(0.1, nNeutr, nProt);
	
      reac->setDebugSwitch(debugSwitch);
	  reac->setRngUniform(rUniform);
	  reac->setRngGaussGlauber(new StRNGGaussGSL(r,0,1));
	  reac->setGlauberKickScale(scaleGlaub);
	  reac->setVelocityScale(velocityshift);
	  reac->setAsymMomentum(asymMomentum);
	
      if(kUseMomDist) reac->setRngParaMom(rMomDist);	  
	
      matTarget->addReaction(reac);
	
    }

   else { //use "normal" reaction class
     StReacStripEvap* reac       = new StReacStripEvap(0.1, nNeutr, nProt, rReac);
	
     reac->setDebugSwitch(debugSwitch);
	 reac->setRngUniform(rUniform);
	 reac->setRngGaussGlauber(new StRNGGaussGSL(r,0,1));
	 reac->setGlauberKickScale(scaleGlaub);
	 reac->setVelocityScale(velocityshift);
	 reac->setAsymMomentum(asymMomentum);
	
     if(kUseMomDist) reac->setRngParaMom(rMomDist);	  
	
     matTarget->addReaction(reac);
   }

	 matTarget->setRngUniform(rUniform);
	 matTarget->setRngThickness(rUniform);

  // Needed for CosyMap
  #include "maps.icc"

  // initialization
  // first we save the particle as it is now
  sys.addBLE(new StPartInitSave());

  // then we use randomized x, tx, y, ty and energy
  //StPartInitRandomGauss* init = new StPartInitRandomGauss(rx,rtx,ry,rty,re,bSpotSxtx,bSpotSyty);//ANK: THIS IS THE ORIGINAL randomization of beam, changed to get multiple components 6-1-2016

 
  
  if (ratio != 1 ) {normscale1 = ratio; normscale2 = 1;}
  else
    {
      normscale1 = scale1 / (scale1+scale2+scale3);
      normscale2 = (scale1+scale2) / (scale1+scale2+scale3);
      }
  StGSLrng* rand = new StGSLrng();
  StRNG* r2 = new StRNGUniformGSL(rand);
  StPartInitRandomGauss* init = new StPartInitRandomGauss(rx,rtx,ry,rty,re,bSpotSxtx,bSpotSyty,rx2,rtx2,ry2,rty2,bSpotSxtx2,bSpotSyty2,rx3,rtx3,ry3,rty3,bSpotSxtx3,bSpotSyty3,bSpotSxy3,r2,normscale1,normscale2);
 

  sys.addBLE(init);

  // Add the Be target
  sys.addBLE(matTarget);

  //ANK: added 5/10/16 to add drift between target and dipole for CAESAR target position
  //ANK: hmm, adding this, even 0.0 doesn't seem to work
  //sys.addBLE(new StPropDrift(0.0));
  
  if (!geant){
        // apperture for the neutrons (edge of vacuum chamber)
    //StAperture* aVacCham = new StApertureSquare(-0.5,0.5,-0.065,0.065);//ANK: origininal dimensions
    StAperture* aVacCham = new StApertureSquare(-0.15,0.15,-0.065,0.065);//ANK: test aperture	  
      sys.addBLE(new StPropDrift(1.57));//ANK: 1.28 was the value originally here, using 1.57 now
		sys.addBLE(aVacCham);
		sys.addBLE(new StPropDrift(-1.57));   // need to drift "backwards" to the focal point of the magnet (map)
		//ANK: 1.28 was the value originally here, using 1.57 now
  }

    //StAperture* aVacCham = new StApertureSquare(-0.5,0.5,-0.000002,0.000002);//ANK: test aperture outside !geant
    //sys.addBLE(aVacCham);
  
	// transport through dipole
	TString tstr_reacType = reacType;

	StPropMapCosy* dipole = 0;
	StPropDrift* dmona = 0;
	
	// This method of assigning the map does not make the code any shorter, it is however easier
 	// on the user, and allows one to switch maps without recompiling. If your map is not in the 
	// list below, then I didn't make an else-if statement for it. MDJ
	// 
	// List of cosymaps:
	// m24o_09028,  m23o_09028,   m22o_09028,   m12be_jesse, m8he_jesse
	// m26ne_calem, m11Li_BeBeam, m24o_hope,    m23O,        m22O
	// m6He,        m8Li,         m10Bea,       m10Beb,      m11Bec
	// m24O,        m26f,         m23Oa,        m22Oa,       m18Of
	// m20O,	    m21Of,        m22Of,        m13B,        m25Ne
	// m16C         m9Li,         m19O,         m11Li,       m14Be
	// mJenna,      m15B_06025,   m14Be_B17beam



	// convert cosymap str to char
	const char *COSYmap = cosymap.c_str();

	if( strcmp(COSYmap, "m24o_09028") ==0){
        dipole = new StPropMapCosy(m24o_09028);
		dmona = new StPropDrift(monaDist - m24o_09028.getLen());
		INFO("using 24O map from 09028 Exper: 24O beam no reaction!\n");
    }

    else if( strcmp(COSYmap, "m23o_09028") == 0){
	  dipole = new StPropMapCosy(m23o_09028);
      dmona = new StPropDrift(monaDist - m23o_09028.getLen());
	  INFO("using 23O map from 09028 Exper: 24O(-n) --> 23O\n");		
    }

	else if( strcmp(COSYmap, "m22o_09028") == 0){
      dipole = new StPropMapCosy(m22o_09028);
	  dmona = new StPropDrift(monaDist - m22o_09028.getLen());
	  INFO("using 22O map from 09028 Exper: 24O(-n) --> 23O* --> 22O + n\n");
    }

	else if( strcmp(COSYmap, "m12be_jesse") == 0){
	  dipole = new StPropMapCosy(m12be_jesse);
	  dmona = new StPropDrift(monaDist - m12be_jesse.getLen());
	  INFO("using 12Be map from Jesse Exper: 14Be->14Be*->12Be+2n\n");		
    }

	else if( strcmp(COSYmap, "m8he_jesse") == 0){
	  dipole = new StPropMapCosy(m8he_jesse);
	  dmona = new StPropDrift(monaDist - m8he_jesse.getLen());
	  INFO("using 8He map from Jesse Exper: 14Be->10He->8He+2n\n");		
    }

	else if( strcmp(COSYmap, "m26ne_calem") == 0){
	  dipole = new StPropMapCosy(m26ne_calem);
	  dmona = new StPropDrift(monaDist - m26ne_calem.getLen());
	  INFO("using 26Ne map for Calem Exper: 29Na(-p)->28Ne->26Ne + 2n\n");	
    }

	else if( strcmp(COSYmap, "m11Li_BeBeam") == 0){
	  dipole = new StPropMapCosy(m11Li_BeBeam);
	  dmona = new StPropDrift(monaDist - m11Li_BeBeam.getLen());
	  INFO("using 11Li map for 05034.: 14Be->13Li->11Li + 2n\n");
    }

	else if( strcmp(COSYmap, "m24o_hope") == 0){
	  dipole = new StPropMapCosy(m24o_hope);
	  dmona = new StPropDrift(monaDist - m24o_hope.getLen());
	  INFO("using 24O map from 27F beam for 08026.\n");		
    }

    else if( strcmp(COSYmap, "m23O") == 0){
      dipole = new StPropMapCosy(m23O);
	  dmona = new StPropDrift(monaDist - m23O.getLen());		
    }

	else if( strcmp(COSYmap, "m22O") == 0){
	  dipole = new StPropMapCosy(m22O);
	  dmona = new StPropDrift(monaDist - m22O.getLen());		
    }

	else if( strcmp(COSYmap, "m6He") == 0){
	  dipole = new StPropMapCosy(m6He);
	  dmona = new StPropDrift(monaDist - m6He.getLen());		
    }

	else if( strcmp(COSYmap, "m8Li") == 0){
	  dipole = new StPropMapCosy(m8Li);
	  dmona = new StPropDrift(monaDist - m8Li.getLen());		
    }

	else if( strcmp(COSYmap, "m10Bea") == 0){
   	  dipole = new StPropMapCosy(m10Bea);
	  dmona = new StPropDrift(monaDist - m10Bea.getLen());		
    }

	else if( strcmp(COSYmap, "m10Beb") == 0){
	  dipole = new StPropMapCosy(m10Beb);
	  dmona = new StPropDrift(monaDist - m10Beb.getLen());		
    }

	else if( strcmp(COSYmap, "m11Bec") == 0){
	  dipole = new StPropMapCosy(m11Bec);
	  dmona = new StPropDrift(monaDist - m11Bec.getLen());	
    }

    else if( strcmp(COSYmap, "m24O") == 0){
   	  dipole = new StPropMapCosy(m24O);
	  dmona = new StPropDrift(monaDist - m24O.getLen());
	  INFO("WARNING: using 24O map for 05039 - 26F -> 25O* -> 24O+n (Calem Experiment) for 12004_dp");		
    } 

	else if( strcmp(COSYmap, "m26f") ==0){
	  dipole = new StPropMapCosy(m26f);
	  dmona = new StPropDrift(monaDist - m26f.getLen());
	  INFO("using 26F map for 05039");		
    }

	else if( strcmp(COSYmap, "m23Oa") == 0){
	  dipole = new StPropMapCosy(m23Oa);
	  dmona = new StPropDrift(monaDist - m23Oa.getLen());
	  INFO("using 23o map for 05039 --> zwk 26F->24O->23O+n");		
    }

	else if( strcmp(COSYmap, "m22Oa") == 0){
 	  dipole = new StPropMapCosy(m22Oa);
	  dmona = new StPropDrift(monaDist - m22Oa.getLen());
	  INFO("using 22o map for 05039, 2neutron test bullshit");// Courtesy of Jesse		
    }

	else if( strcmp(COSYmap, "m18Of") == 0){
   	  dipole = new StPropMapCosy(m18Of);
	  dmona = new StPropDrift(monaDist - m18Of.getLen());
	  INFO("using 18O map for 05124.\n");		
    }

	else if( strcmp(COSYmap, "m20O") == 0){
	  dipole = new StPropMapCosy(m20O);
	  dmona = new StPropDrift(monaDist - m20O.getLen());
	  INFO("using 20O map for 05124.\n");		
    }	

	else if( strcmp(COSYmap, "m210f") == 0){
	  dipole = new StPropMapCosy(m21Of);
	  dmona = new StPropDrift(monaDist - m21Of.getLen());
	  INFO("using 21O map for 05124.\n");		
    }

	else if( strcmp(COSYmap, "m22Of") == 0){
 	  dipole = new StPropMapCosy(m22Of);
	  dmona = new StPropDrift(monaDist - m22Of.getLen());
	  INFO("using 22O map for 05124.\n");		
    }

	else if( strcmp(COSYmap, "m13B") == 0){
	  dipole = new StPropMapCosy(m13B);
      dmona = new StPropDrift(monaDist - m13B.getLen());
      INFO("using 13B map for 05124.\n");		
    }

	else if( strcmp(COSYmap, "m25Ne") == 0){
 	  dipole = new StPropMapCosy(m25Ne);
	  dmona = new StPropDrift(monaDist - m25Ne.getLen());
	  INFO("using 25Ne map for 05124.\n");		
    }

    else if( strcmp(COSYmap, "m16C") == 0){
	  dipole = new StPropMapCosy(m16C);
	  dmona = new StPropDrift(monaDist - m16C.getLen());
	  INFO("using 16C map for 05124.\n");		
    }

	else if( strcmp(COSYmap, "m9Li") == 0){
 	  dipole = new StPropMapCosy(m9Li);
	  dmona = new StPropDrift(monaDist - m9Li.getLen());
	  INFO("using 9Li map for 05124.\n");		
    }

	else if( strcmp(COSYmap, "m19O") == 0){
	  dipole = new StPropMapCosy(m19O);
	  dmona = new StPropDrift(monaDist - m19O.getLen());
	  INFO("using 48Ca map for 05124.\n");		
    }

	else if( strcmp(COSYmap, "m11Li") == 0){
	  dipole = new StPropMapCosy(m11Li);
	  dmona = new StPropDrift(monaDist - m11Li.getLen());
	  INFO("using 11Li map for 05034. 14B->12Li->11Li+n - zwk\n");		
    }

	else if( strcmp(COSYmap, "m14Be") == 0){
	  dipole = new StPropMapCosy(m14Be);
	  dmona = new StPropDrift(monaDist - m14Be.getLen());
	  INFO("Using.\n");		
    }

	else if( strcmp(COSYmap, "mJenna") == 0){
	  dipole = new StPropMapCosy(mJenna);
	  dmona = new StPropDrift(monaDist - mJenna.getLen());
	  INFO("Using 6He map, for 9Li.\n");		
    }

	else if( strcmp(COSYmap, "m15B_06025") == 0){
 	  dipole = new StPropMapCosy(m15B_06025);
	  dmona = new StPropDrift(monaDist - m15B_06025.getLen());
	  INFO("using 15B map for 06025.\n");
    }

	else if( strcmp(COSYmap, "m14Be_B17beam") == 0){
 	  dipole = new StPropMapCosy(m14Be_B17beam);
	  dmona = new StPropDrift(monaDist - m14Be_B17beam.getLen());
	  INFO("using 14Be map from 17B beam for 06025.\n");		
    }	
	//ANK
	else if( strcmp(COSYmap, "m12Be_06025") == 0){
///	  dipole = new StPropMapCosy(m12Be_06025);
///	  dmona = new StPropDrift(monaDist - m12Be_06025.getLen());
///	  INFO("using 12Be map from 06025. \n");

          /// DV 10/31/2016 Commented this out so it would compile.	
          cout << endl << endl << endl << "CHECK ST_MONA.CC LINE 1952" << endl << endl << endl;	
	}

	else if( strcmp(COSYmap, "m23O_10023") == 0){
	  dipole = new StPropMapCosy(m23O_10023);
	  dmona = new StPropDrift(monaDist - m23O_10023.getLen());
	  INFO("using 23O map from 10023. \n");
	}
	else if( strcmp(COSYmap, "m14Be_06025") == 0){
///	  dipole = new StPropMapCosy(m14Be_06025);
///	  dmona = new StPropDrift(monaDist - m14Be_06025.getLen());
///	  INFO("using 14Be map from 06025. \n");

          /// DV 10/31/2016 Commented this out so it would compile.	
          cout << endl << endl << endl << "CHECK ST_MONA.CC LINE 1966" << endl << endl << endl;	
	}
	//ANK: 10/7/2015
	else if( strcmp(COSYmap, "m7Li_e11028") == 0){
	  dipole = new StPropMapCosy(m7Li_e11028);
	  dmona = new StPropDrift(monaDist - m7Li_e11028.getLen());
	  INFO("using 7Li map from e11028. \n");
	}	
	else if( strcmp(COSYmap, "m6He_e11028") == 0){
	  dipole = new StPropMapCosy(m6He_e11028);
	  dmona = new StPropDrift(monaDist - m6He_e11028.getLen());
	  INFO("using 6He map from e11028. \n");
	  cout << "ANK: length= " << m6He_e11028.getLen() << endl;
	}
	else if( strcmp(COSYmap, "m11Be_e15091") == 0){ // DV 2017.
 	  dipole = new StPropMapCosy(m11Be_e15091);
	  dmona = new StPropDrift(monaDist - m11Be_e15091.getLen());
	  INFO("using 11Be map from 11Be for e15091.\n");
        }
	else if( strcmp(COSYmap, "m12B_e15091") == 0){ // DV 2017.
 	  dipole = new StPropMapCosy(m12B_e15091);
	  dmona = new StPropDrift(monaDist - m12B_e15091.getLen());
	  INFO("using 12B map from 12B for e15091.\n");
        }	
	else if( strcmp(COSYmap, "m8He_11Be_e15091") == 0){ // DV 2017.
 	  dipole = new StPropMapCosy(m8He_11Be_e15091);
	  dmona = new StPropDrift(monaDist - m8He_11Be_e15091.getLen());
	  INFO("using 8He map from 11Be for e15091.\n");
        }
	else if( strcmp(COSYmap, "m8He_12B_e15091") == 0){ // DV 2017.
 	  dipole = new StPropMapCosy(m8He_12B_e15091);
	  dmona = new StPropDrift(monaDist - m8He_12B_e15091.getLen());
	  INFO("using 8He map from 12B for e15091.\n");
        }	

	//ANK: lets try adding drift here
	//sys.addBLE(new StPropDrift(0.0));

	sys.addBLE(dipole);
		
	// CRDC1 aperture (act only on charged particles)
	StAperture* aCRDC1 = new StApertureSquare(crdc1MaskRight, crdc1MaskLeft, -0.15, 0.15, 1);
		
	sys.addBLE(aCRDC1);
                           
	// apperture for the fragments (CRDC2 acceptance) (act only on charged particles)
	StAperture* aCRDC2ap = new StApertureSquare(crdc2MaskRight,crdc2MaskLeft,crdc2MaskBot,crdc2MaskTop, 1);		
	aCRDC2ap->setName("crdc2_ble");
	
	sys.addBLE(new StPropDrift(crdc2dist));
	sys.addBLE(aCRDC2ap);
	sys.addBLE(new StPropDrift(-crdc2dist));// need to drift "backwards" to crdc1 position for dmona

	// drift to Mona
	sys.addBLE(dmona);

	if (!geant){
        // aperture of Mona
		if ( exp == "05124a" || exp == "05124b" || exp == "05124c" || exp == "05124d" || exp == "05124e" || exp == "05124_Li9" || exp == "05124_O18" || exp == "05124_O19" || exp == "05124_O20" || exp == "05124_O21" || exp == "05124_O22" ) {
            
            INFO("using smaller MoNA acceptance for 05124.\n");
		    
            // MoNA aperture set to smaller opening 
		    StAperture* aMona_frag = new StApertureSquare(-0.4,0.6,-.30,.25); 
		    sys.addBLE(aMona_frag);  
        }

        else {
		    INFO("using standard MoNA aperture size.\n");

		    StAperture* aMona = new StApertureSquare(-1,1,-0.85,0.75); //(-1,1,-.85,.75);
		    sys.addBLE(aMona);
		  }	
    }

	StPropDrift* d = new StPropDrift(0);
	sys.addBLE(d);

	// end of Beam Line Elements

  // start setting up n-tuple or root tree
  StTuple* tup = 0;
  
  // setup GSL n-tuple for writing
  if (tupleFile.find(".gsltup") != string::npos) {
      INFO("Writing data to GSL n-tuple in file %s",tupleFile.c_str());
	  tup = new StGSLTuple(tupleFile);		
  } 

  else{ 
      if (tupleFile.find(".root") != string::npos)  {
          INFO("Writing data to ROOT tree in file %s",tupleFile.c_str());
          tup = new StROOTTuple(tupleFile);	  
      }

          else {
		    WAR("Data will not be written to file!");
		  }
  }

  sys.setTupleP(tup);

  // initial parameters
  init->addNtupFields("x");
  init->addNtupFields("tx");
  init->addNtupFields("y");
  init->addNtupFields("ty");
  init->addNtupFields("Ekin");

  // x-target resolution
  StRNG*            rngTargetX = new StRNGGaussGSL(r,0,resTargetX);
  StDetRespRNG* detRespTargetX = new StDetRespRNG(rngTargetX);

  // after target target
  matTarget->addNtupFields("x",0,0);// detRespTargetX
  matTarget->addNtupFields("tx",0);
  matTarget->addNtupFields("y",0);
  matTarget->addNtupFields("ty",0);
  matTarget->addNtupFields("t",0);
  matTarget->addNtupFields("z",0);
  matTarget->addNtupFields("TP",0);
  matTarget->addNtupFields("dE",0);
  matTarget->addNtupFields("Ekin",0);
  matTarget->addNtupFields("R_exen",0);
  matTarget->addNtupFields("R_ph",0);
  matTarget->addNtupFields("R_th",0);
  matTarget->addNtupFields("R_pz",0);
		
  // Add energy, theta and phi branches for each neutron depending on type of reaction
  if (reacType == "2neutron" || reacType == "di_neutron" || reacType == "3body_decay"){
      matTarget->addNtupFields("R_exen2",0);
      matTarget->addNtupFields("R_ph2",0);
      matTarget->addNtupFields("R_th2",0);		
  }
  
  if( reacType == "3neutron" || reacType == "4body_decay"){
      matTarget->addNtupFields("R_exen2",0);
      matTarget->addNtupFields("R_ph2",0);
      matTarget->addNtupFields("R_th2",0);
      matTarget->addNtupFields("R_exen3",0);
      matTarget->addNtupFields("R_ph3",0);
      matTarget->addNtupFields("R_th3",0);	
  }

  if( reacType == "4neutron" ||  reacType == "5body_decay" || reacType == "5body_seq"){
		  matTarget->addNtupFields("R_exen2",0);
		  matTarget->addNtupFields("R_ph2",0);
		  matTarget->addNtupFields("R_th2",0);
		  matTarget->addNtupFields("R_exen3",0);
		  matTarget->addNtupFields("R_ph3",0);
		  matTarget->addNtupFields("R_th3",0);
		  matTarget->addNtupFields("R_exen4",0);
		  matTarget->addNtupFields("R_ph4",0);
		  matTarget->addNtupFields("R_th4",0);
  }

  // time resolution
  StRNG*            rngTime = new StRNGGaussGSL(r,0,resTime);
  StDetRespRNG* detRespTime = new StDetRespRNG(rngTime);

  // CRDC1 X response
  StRNG*            rngCRDC1X = new StRNGGaussGSL(r,0,resCRDC1X);
  StDetRespRNG* detRespCRDC1X = new StDetRespRNG(rngCRDC1X);

  // CRDC1 Y Response
  StRNG*            rngCRDC1Y = new StRNGGaussGSL(r,0,resCRDC1Y);
  StDetRespRNG* detRespCRDC1Y = new StDetRespRNG(rngCRDC1Y);

  // CRDC1 theta X response
  StRNG*            rngCRDC1ThetaX = new StRNGGaussGSL(r,0,resCRDC1ThetaX);
  StDetRespRNG* detRespCRDC1ThetaX = new StDetRespRNG(rngCRDC1ThetaX);

  // CRDC1 theta Y response
  StRNG*            rngCRDC1ThetaY = new StRNGGaussGSL(r,0,resCRDC1ThetaY);
  StDetRespRNG* detRespCRDC1ThetaY = new StDetRespRNG(rngCRDC1ThetaY);

  // after dipole (at CRDC1)
  aCRDC1->addNtupFields("x"   ,0,detRespCRDC1X);
  aCRDC1->addNtupFields("tx"  ,0,detRespCRDC1ThetaX);
  aCRDC1->addNtupFields("y"   ,0,detRespCRDC1Y);
  aCRDC1->addNtupFields("ty"  ,0,detRespCRDC1ThetaY);
  aCRDC1->addNtupFields("t"   ,0,detRespTime);
  aCRDC1->addNtupFields("Ekin",0);

  aCRDC2ap->addNtupFields("Ekin",0);
  aCRDC2ap->addNtupFields("x",   0,detRespCRDC1X);
  aCRDC2ap->addNtupFields("tx"  ,0,detRespCRDC1ThetaX);
  aCRDC2ap->addNtupFields("y"   ,0,detRespCRDC1Y);
  aCRDC2ap->addNtupFields("ty"  ,0,detRespCRDC1ThetaY);

		// Remove Detector Resolutions
		/*aCRDC1->addNtupFields("x"   ,0,0);
		aCRDC1->addNtupFields("tx"  ,0,0);
		aCRDC1->addNtupFields("y"   ,0,0);
		aCRDC1->addNtupFields("ty"  ,0,0);
		aCRDC1->addNtupFields("t"   ,0,0);
		aCRDC1->addNtupFields("Ekin",0);

		aCRDC2ap->addNtupFields("Ekin",0);
		aCRDC2ap->addNtupFields("x",   0,0);
		aCRDC2ap->addNtupFields("tx"  ,0,0);
		aCRDC2ap->addNtupFields("y"   ,0,0);
		aCRDC2ap->addNtupFields("ty"  ,0,0);*/



  // New Double Laplace Implimented by GC
  StRNG* rngMonaX            = new StRNGDoubleLaplaceGSL(r,0,resMonaX1,0,resMonaX2,resMonaP);
  StDetRespRNG* detRespMonaX = new StDetRespRNG(rngMonaX);
 
  // Mona resp Y
  StDetRespBin* detRespMonaY = new StDetRespBin(-.85,.75,16);

  // Mona resp Z
  StRNG* rngMonaZ            = new StRNGUniformGSL(r,-.05,.05);
  StDetRespRNG* detRespMonaZ = new StDetRespRNG(rngMonaZ);

  INFO("Finished setting up Beam Line Elements.");

  if (!resMonaBar) {
      detRespMonaZ = 0;
      detRespMonaY = 0;
  }
 		
  if (geant){
      // Saves the data without resolutions with the geant flag
      // MoNA first neutron
      d->addNtupFields("t"   ,1,0);
	  d->addNtupFields("x"   ,1,0);
	  d->addNtupFields("y"   ,1,0);
	  d->addNtupFields("z"   ,1,0);
	  d->addNtupFields("Ekin",1);
		  
      //MoNA second neutron	  
      if (reacType == "2neutron" || reacType == "di_neutron" || reacType == "3body_decay"){
          d->addNtupFields("t"   ,2,0);
		  d->addNtupFields("x"   ,2,0);
		  d->addNtupFields("y"   ,2,0);
		  d->addNtupFields("z"   ,2,0);
		  d->addNtupFields("Ekin",2);	  
      }
      // Third Neutron
      else if (reacType == "3neutron" || reacType == "4body_decay"){
        d->addNtupFields("t"   ,2,0);
		d->addNtupFields("x"   ,2,0);
		d->addNtupFields("y"   ,2,0);
		d->addNtupFields("z"   ,2,0);
		d->addNtupFields("Ekin",2);

		d->addNtupFields("t"   ,3,0);
		d->addNtupFields("x"   ,3,0);
		d->addNtupFields("y"   ,3,0);
		d->addNtupFields("z"   ,3,0);
		d->addNtupFields("Ekin",3);	  
      }
      // Fourth Neutron
      else if (reacType == "4neutron" || reacType == "5body_decay" || reacType == "5body_seq"){
		d->addNtupFields("t"   ,2,0);
		d->addNtupFields("x"   ,2,0);
		d->addNtupFields("y"   ,2,0);
		d->addNtupFields("z"   ,2,0);
		d->addNtupFields("Ekin",2);

		d->addNtupFields("t"   ,3,0);
		d->addNtupFields("x"   ,3,0);
		d->addNtupFields("y"   ,3,0);
		d->addNtupFields("z"   ,3,0);
		d->addNtupFields("Ekin",3);

		d->addNtupFields("t"   ,4,0);
		d->addNtupFields("x"   ,4,0);
		d->addNtupFields("y"   ,4,0);
		d->addNtupFields("z"   ,4,0);
		d->addNtupFields("Ekin",4);
      }	
  }

  else {  // Saves the data with the resolutions when no geant flag is present
      // MoNA first neutron
	  d->addNtupFields("t"   ,1,detRespTime);
	  d->addNtupFields("x"   ,1,detRespMonaX);
	  d->addNtupFields("y"   ,1,detRespMonaY);
	  d->addNtupFields("z"   ,1,detRespMonaZ);
	  d->addNtupFields("Ekin",1);
		  
      //MoNA second neutron
	  if (reacType == "2neutron" || reacType == "di_neutron" || reacType == "3body_decay"){
          d->addNtupFields("t"   ,2,detRespTime);
		  d->addNtupFields("x"   ,2,detRespMonaX);
		  d->addNtupFields("y"   ,2,detRespMonaY);
		  d->addNtupFields("z"   ,2,detRespMonaZ);
		  d->addNtupFields("Ekin",2);	  
      }
      // Third Neutron
	  else if (reacType == "3neutron" || reacType == "4body_decay"){
          d->addNtupFields("t"   ,2,detRespTime);
          d->addNtupFields("x"   ,2,detRespMonaX);
		  d->addNtupFields("y"   ,2,detRespMonaY);
		  d->addNtupFields("z"   ,2,detRespMonaZ);
		  d->addNtupFields("Ekin",2);
		  
          d->addNtupFields("t"   ,3,detRespTime);
		  d->addNtupFields("x"   ,3,detRespMonaX);
		  d->addNtupFields("y"   ,3,detRespMonaY);
		  d->addNtupFields("z"   ,3,detRespMonaZ);
		  d->addNtupFields("Ekin",3);
      }
      // Fourth Neutron
      else if (reacType == "4neutron" || reacType == "5body_decay" || reacType == "5body_seq"){
          d->addNtupFields("t"   ,2,detRespTime);
		  d->addNtupFields("x"   ,2,detRespMonaX);
		  d->addNtupFields("y"   ,2,detRespMonaY);
		  d->addNtupFields("z"   ,2,detRespMonaZ);
		  d->addNtupFields("Ekin",2);
		  
          d->addNtupFields("t"   ,3,detRespTime);
		  d->addNtupFields("x"   ,3,detRespMonaX);
		  d->addNtupFields("y"   ,3,detRespMonaY);
		  d->addNtupFields("z"   ,3,detRespMonaZ);
		  d->addNtupFields("Ekin",3);
		  
          d->addNtupFields("t"   ,4,detRespTime);
		  d->addNtupFields("x"   ,4,detRespMonaX);
		  d->addNtupFields("y"   ,4,detRespMonaY);
		  d->addNtupFields("z"   ,4,detRespMonaZ);
		  d->addNtupFields("Ekin",4);
      }		
  }  

  cout << "Running Events " << endl;	 

  if (verbose) sys.run(nevents,"print");
  else         sys.run(nevents,""); //"crdc2_cut" removes events not passing crdc2
  //else         sys.run(nevents,"crdc2_cut"); //"crdc2_cut" removes events not passing crdc2
  //Note that when using "crdc2_cut", the input number of events represents the number of events to be written!

  delete tup;  // needed to be sure that data is written to file

  return 0;

}
