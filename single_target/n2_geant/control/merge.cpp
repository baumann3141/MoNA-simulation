// C++ library includes
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <assert.h>
#include <iostream>

// ROOT library includes
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TObject.h>
#include <TLorentzVector.h>

using namespace std;

int main(int argc, char *argv[])
{
    if(argc != 6) // argc should be 2 for correct execution.
        // We print argv[0] assuming it is the program name.
        std::cout << "Usage: " << argv[0] << " <filename> <filename> <nneuts>\n";
    else 
    {

      //new unique id
      TString uniqueID = argv[4];
      TString dirdata = argv[5];

        // First open the original st-mona .root file
        TFile *inFile = new TFile(argv[1]); assert(inFile);
        TTree *t = (TTree*)inFile->Get("t"); assert(t);
        Int_t nneuts = atoi(argv[3]);
	
	if (nneuts == 4)// Begin 4n
        {
            // Now we find the relevant branches to look at
	  Double_t b1p0x,b1p0tx,b1p0y,b1p0ty,b1p0Ekin,
		   b2p0x,b2p0tx,b2p0y,b2p0ty,b2p0t,b2p0z,b2p0TP,b2p0dE,b2p0Ekin,
		   b2p0R_pz,b2p0R_exen,b2p0R_exen2,b2p0R_exen3,b2p0R_exen4,b2p0R_ph,b2p0R_ph2,b2p0R_ph3,b2p0R_ph4,b2p0R_th,b2p0R_th2,b2p0R_th3,b2p0R_th4,
		   b4p0x,b4p0tx,b4p0y,b4p0ty,b4p0t,b4p0Ekin,b6p0Ekin,b6p0x,b6p0y,b6p0tx,b6p0ty,
	           b9p1t,b9p1x,b9p1y,b9p1z,b9p1Ekin,
		   b9p2t,b9p2x,b9p2y,b9p2z,b9p2Ekin,
	           b9p3t,b9p3x,b9p3y,b9p3z,b9p3Ekin,
		   b9p4t,b9p4x,b9p4y,b9p4z,b9p4Ekin;



            string bn_old[] = {"b1p0x", "b1p0tx", "b1p0y", "b1p0ty", "b1p0Ekin",
                "b2p0x", "b2p0tx", "b2p0y", "b2p0ty", "b2p0t", "b2p0z", "b2p0TP", "b2p0dE", "b2p0Ekin",
		"b2p0R_pz","b2p0R_exen", "b2p0R_exen2", "b2p0R_exen3", "b2p0R_exen4", "b2p0R_ph", "b2p0R_ph2", "b2p0R_ph3", "b2p0R_ph4", "b2p0R_th", "b2p0R_th2", "b2p0R_th3", "b2p0R_th4",
                "b4p0x", "b4p0tx", "b4p0y", "b4p0ty", "b4p0t", "b4p0Ekin",
                "b6p0x", "b6p0tx", "b6p0y", "b6p0ty", "b6p0Ekin",
                "b9p1t", "b9p1x", "b9p1y", "b9p1z", "b9p1Ekin",
                "b9p2t", "b9p2x", "b9p2y", "b9p2z", "b9p2Ekin",
  	        "b9p3t", "b9p3x", "b9p3y", "b9p3z", "b9p3Ekin",
                "b9p4t", "b9p4x", "b9p4y", "b9p4z", "b9p4Ekin"
                };
        
            double* ps_old[] = {&b1p0x,&b1p0tx,&b1p0y,&b1p0ty,&b1p0Ekin,
                &b2p0x,&b2p0tx,&b2p0y,&b2p0ty,&b2p0t,&b2p0z,&b2p0TP,&b2p0dE,
		&b2p0Ekin,&b2p0R_pz,&b2p0R_exen,&b2p0R_exen2,&b2p0R_exen3, &b2p0R_exen4, &b2p0R_ph,&b2p0R_ph2, &b2p0R_ph3, &b2p0R_ph4, &b2p0R_th, &b2p0R_th2, &b2p0R_th3, &b2p0R_th4,
                &b4p0x,&b4p0tx,&b4p0y,&b4p0ty,&b4p0t,&b4p0Ekin,
                &b6p0x,&b6p0tx,&b6p0y,&b6p0ty,&b6p0Ekin,
                &b9p1t,&b9p1x,&b9p1y,&b9p1z,&b9p1Ekin,
                &b9p2t,&b9p2x,&b9p2y,&b9p2z,&b9p2Ekin,
                &b9p3t,&b9p3x,&b9p3y,&b9p3z,&b9p3Ekin,
                &b9p4t,&b9p4x,&b9p4y,&b9p4z,&b9p4Ekin
                };   

            for (size_t i(0); i<sizeof(ps_old)/sizeof(double*); i++) {
                t->SetBranchAddress(bn_old[i].c_str(),ps_old[i]);  
            }

            // Get number of entries in the file
            Int_t input_nevent = t->GetEntries();

            // Then open the GEANT output for the 1st neutron root file
	    TString n1name = dirdata+"/neutron1"+uniqueID+".root";
		cout << "Found 1st file: " << n1name << endl;
            TFile *tempFile = new TFile(n1name); assert(tempFile);
            TTree *tn1Temp = (TTree*)tempFile->Get("t"); assert(tn1Temp);

            Double_t mh1n1Edep,mh1n1light,mh1n1t,mh1n1x,mh1n1y,mh1n1z,mh1n2Edep,mh1n2light,mh1n2t,mh1n2x,mh1n2y,mh1n2z,mh1n3Edep,mh1n3light,mh1n3t,mh1n3x,mh1n3y,mh1n3z,mh1n4Edep,mh1n4light,mh1n4t,mh1n4x,mh1n4y,mh1n4z,mh1n5Edep,mh1n5light,mh1n5t,mh1n5x,mh1n5y,mh1n5z,mh1n6Edep,mh1n6light,mh1n6t,mh1n6x,mh1n6y,mh1n6z,mh1n7Edep,mh1n7light,mh1n7t,mh1n7x,mh1n7y,mh1n7z,mh1n8Edep,mh1n8light,mh1n8t,mh1n8x,mh1n8y,mh1n8z,mh1n9Edep,mh1n9light,mh1n9t,mh1n9x,mh1n9y,mh1n9z,mh1n10Edep,mh1n10light,mh1n10t,mh1n10x,mh1n10y,mh1n10z;

            Int_t mh1nmultiplicity;
            Int_t mh1_z1; //zwk particle multiplicities
            Int_t mh1_z2;
            Int_t mh1_zgt2;
            Int_t mh1_g;

            string bn_n1[] = {"MoNA.Hit.1.e","MoNA.Hit.1.light","MoNA.Hit.1.t","MoNA.Hit.1.x","MoNA.Hit.1.y","MoNA.Hit.1.z",
                "MoNA.Hit.2.e","MoNA.Hit.2.light","MoNA.Hit.2.t","MoNA.Hit.2.x","MoNA.Hit.2.y","MoNA.Hit.2.z",
                "MoNA.Hit.3.e","MoNA.Hit.3.light","MoNA.Hit.3.t","MoNA.Hit.3.x","MoNA.Hit.3.y","MoNA.Hit.3.z",
                "MoNA.Hit.4.e","MoNA.Hit.4.light","MoNA.Hit.4.t","MoNA.Hit.4.x","MoNA.Hit.4.y","MoNA.Hit.4.z",
                "MoNA.Hit.5.e","MoNA.Hit.5.light","MoNA.Hit.5.t","MoNA.Hit.5.x","MoNA.Hit.5.y","MoNA.Hit.5.z",
                "MoNA.Hit.6.e","MoNA.Hit.6.light","MoNA.Hit.6.t","MoNA.Hit.6.x","MoNA.Hit.6.y","MoNA.Hit.6.z",
                "MoNA.Hit.7.e","MoNA.Hit.7.light","MoNA.Hit.7.t","MoNA.Hit.7.x","MoNA.Hit.7.y","MoNA.Hit.7.z",
                "MoNA.Hit.8.e","MoNA.Hit.8.light","MoNA.Hit.8.t","MoNA.Hit.8.x","MoNA.Hit.8.y","MoNA.Hit.8.z",
                "MoNA.Hit.9.e","MoNA.Hit.9.light","MoNA.Hit.9.t","MoNA.Hit.9.x","MoNA.Hit.9.y","MoNA.Hit.9.z",
                "MoNA.Hit.10.e","MoNA.Hit.10.light","MoNA.Hit.10.t","MoNA.Hit.10.x","MoNA.Hit.10.y","MoNA.Hit.10.z",
                };

            double* ps_n1[] = {&mh1n1Edep,&mh1n1light,&mh1n1t,&mh1n1x,&mh1n1y,&mh1n1z,
                &mh1n2Edep,&mh1n2light,&mh1n2t,&mh1n2x,&mh1n2y,&mh1n2z,
                &mh1n3Edep,&mh1n3light,&mh1n3t,&mh1n3x,&mh1n3y,&mh1n3z,
                &mh1n4Edep,&mh1n4light,&mh1n4t,&mh1n4x,&mh1n4y,&mh1n4z,
                &mh1n5Edep,&mh1n5light,&mh1n5t,&mh1n5x,&mh1n5y,&mh1n5z,
                &mh1n6Edep,&mh1n6light,&mh1n6t,&mh1n6x,&mh1n6y,&mh1n6z,
                &mh1n7Edep,&mh1n7light,&mh1n7t,&mh1n7x,&mh1n7y,&mh1n7z,
                &mh1n8Edep,&mh1n8light,&mh1n8t,&mh1n8x,&mh1n8y,&mh1n8z,
                &mh1n9Edep,&mh1n9light,&mh1n9t,&mh1n9x,&mh1n9y,&mh1n9z,
                &mh1n10Edep,&mh1n10light,&mh1n10t,&mh1n10x,&mh1n10y,&mh1n10z,
                };

            for (size_t i(0); i<sizeof(ps_n1)/sizeof(double*); i++) {
                tn1Temp->SetBranchAddress(bn_n1[i].c_str(),ps_n1[i]);  
            }

            tn1Temp->SetBranchAddress("MoNA.multiplicity",&mh1nmultiplicity);  
            tn1Temp->SetBranchAddress("MoNA.Z1mult",&mh1_z1);  
            tn1Temp->SetBranchAddress("MoNA.Z2mult",&mh1_z2);  
            tn1Temp->SetBranchAddress("MoNA.ZGt2mult",&mh1_zgt2);  
            tn1Temp->SetBranchAddress("MoNA.Gmult",&mh1_g);  

            // Get number of entries in the file
            Int_t outputn1_nevents = tn1Temp->GetEntries();

            // Then open the GEANT output for the 2nd neutron root file
	    TString n2name = dirdata+"/neutron2"+uniqueID+".root";
		cout << "Found 2nd File: " << n2name << endl;
            TFile *temp2File = new TFile(n2name); assert(temp2File);
            TTree *tn2Temp = (TTree*)temp2File->Get("t"); assert(tn2Temp);	

            Double_t mh2n1Edep,mh2n1light,mh2n1t,mh2n1x,mh2n1y,mh2n1z,mh2n2Edep,mh2n2light,mh2n2t,mh2n2x,mh2n2y,mh2n2z,mh2n3Edep,mh2n3light,mh2n3t,mh2n3x,mh2n3y,mh2n3z,mh2n4Edep,mh2n4light,mh2n4t,mh2n4x,mh2n4y,mh2n4z,mh2n5Edep,mh2n5light,mh2n5t,mh2n5x,mh2n5y,mh2n5z,mh2n6Edep,mh2n6light,mh2n6t,mh2n6x,mh2n6y,mh2n6z,mh2n7Edep,mh2n7light,mh2n7t,mh2n7x,mh2n7y,mh2n7z,mh2n8Edep,mh2n8light,mh2n8t,mh2n8x,mh2n8y,mh2n8z,mh2n9Edep,mh2n9light,mh2n9t,mh2n9x,mh2n9y,mh2n9z,mh2n10Edep,mh2n10light,mh2n10t,mh2n10x,mh2n10y,mh2n10z;

            Int_t mh2nmultiplicity;
            Int_t mh2_z1; //zwk particle multiplicities
            Int_t mh2_z2;
            Int_t mh2_zgt2;
            Int_t mh2_g;

            string bn_n2[] = {"MoNA.Hit.1.e","MoNA.Hit.1.light","MoNA.Hit.1.t","MoNA.Hit.1.x","MoNA.Hit.1.y","MoNA.Hit.1.z",
                "MoNA.Hit.2.e","MoNA.Hit.2.light","MoNA.Hit.2.t","MoNA.Hit.2.x","MoNA.Hit.2.y","MoNA.Hit.2.z",
                "MoNA.Hit.3.e","MoNA.Hit.3.light","MoNA.Hit.3.t","MoNA.Hit.3.x","MoNA.Hit.3.y","MoNA.Hit.3.z",
                "MoNA.Hit.4.e","MoNA.Hit.4.light","MoNA.Hit.4.t","MoNA.Hit.4.x","MoNA.Hit.4.y","MoNA.Hit.4.z",
                "MoNA.Hit.5.e","MoNA.Hit.5.light","MoNA.Hit.5.t","MoNA.Hit.5.x","MoNA.Hit.5.y","MoNA.Hit.5.z",
                "MoNA.Hit.6.e","MoNA.Hit.6.light","MoNA.Hit.6.t","MoNA.Hit.6.x","MoNA.Hit.6.y","MoNA.Hit.6.z",
                "MoNA.Hit.7.e","MoNA.Hit.7.light","MoNA.Hit.7.t","MoNA.Hit.7.x","MoNA.Hit.7.y","MoNA.Hit.7.z",
                "MoNA.Hit.8.e","MoNA.Hit.8.light","MoNA.Hit.8.t","MoNA.Hit.8.x","MoNA.Hit.8.y","MoNA.Hit.8.z",
                "MoNA.Hit.9.e","MoNA.Hit.9.light","MoNA.Hit.9.t","MoNA.Hit.9.x","MoNA.Hit.9.y","MoNA.Hit.9.z",
                "MoNA.Hit.10.e","MoNA.Hit.10.light","MoNA.Hit.10.t","MoNA.Hit.10.x","MoNA.Hit.10.y","MoNA.Hit.10.z",
                };

            double* ps_n2[] = {&mh2n1Edep,&mh2n1light,&mh2n1t,&mh2n1x,&mh2n1y,&mh2n1z,
                &mh2n2Edep,&mh2n2light,&mh2n2t,&mh2n2x,&mh2n2y,&mh2n2z,
                &mh2n3Edep,&mh2n3light,&mh2n3t,&mh2n3x,&mh2n3y,&mh2n3z,
                &mh2n4Edep,&mh2n4light,&mh2n4t,&mh2n4x,&mh2n4y,&mh2n4z,
                &mh2n5Edep,&mh2n5light,&mh2n5t,&mh2n5x,&mh2n5y,&mh2n5z,
                &mh2n6Edep,&mh2n6light,&mh2n6t,&mh2n6x,&mh2n6y,&mh2n6z,
                &mh2n7Edep,&mh2n7light,&mh2n7t,&mh2n7x,&mh2n7y,&mh2n7z,
                &mh2n8Edep,&mh2n8light,&mh2n8t,&mh2n8x,&mh2n8y,&mh2n8z,
                &mh2n9Edep,&mh2n9light,&mh2n9t,&mh2n9x,&mh2n9y,&mh2n9z,
                &mh2n10Edep,&mh2n10light,&mh2n10t,&mh2n10x,&mh2n10y,&mh2n10z,
                };

            for (size_t i(0); i<sizeof(ps_n2)/sizeof(double*); i++) {
                tn2Temp->SetBranchAddress(bn_n2[i].c_str(),ps_n2[i]);  
            }

            tn2Temp->SetBranchAddress("MoNA.multiplicity",&mh2nmultiplicity); 
            tn2Temp->SetBranchAddress("MoNA.Z1mult",&mh2_z1);  
            tn2Temp->SetBranchAddress("MoNA.Z2mult",&mh2_z2);  
            tn2Temp->SetBranchAddress("MoNA.ZGt2mult",&mh2_zgt2);  
            tn2Temp->SetBranchAddress("MoNA.Gmult",&mh2_g);  

            // Get number of entries in the file
            Int_t outputn2_nevents = tn2Temp->GetEntries();


	// Then open the GEANT output for the 3rd neutron root file
	    TString n3name = dirdata+"/neutron3"+uniqueID+".root";
		cout << "Found 3rd file: " << n3name << endl;
            TFile *temp3File = new TFile(n3name); assert(temp3File);
            TTree *tn3Temp = (TTree*)temp3File->Get("t"); assert(tn3Temp);

            Double_t mh3n1Edep,mh3n1light,mh3n1t,mh3n1x,mh3n1y,mh3n1z,mh3n2Edep,mh3n2light,mh3n2t,mh3n2x,mh3n2y,mh3n2z,mh3n3Edep,mh3n3light,mh3n3t,mh3n3x,mh3n3y,mh3n3z,mh3n4Edep,mh3n4light,mh3n4t,mh3n4x,mh3n4y,mh3n4z,mh3n5Edep,mh3n5light,mh3n5t,mh3n5x,mh3n5y,mh3n5z,mh3n6Edep,mh3n6light,mh3n6t,mh3n6x,mh3n6y,mh3n6z,mh3n7Edep,mh3n7light,mh3n7t,mh3n7x,mh3n7y,mh3n7z,mh3n8Edep,mh3n8light,mh3n8t,mh3n8x,mh3n8y,mh3n8z,mh3n9Edep,mh3n9light,mh3n9t,mh3n9x,mh3n9y,mh3n9z,mh3n10Edep,mh3n10light,mh3n10t,mh3n10x,mh3n10y,mh3n10z;

            Int_t mh3nmultiplicity;
            Int_t mh3_z1; //zwk particle multiplicities
            Int_t mh3_z2;
            Int_t mh3_zgt2;
            Int_t mh3_g;

            string bn_n3[] = {"MoNA.Hit.1.e","MoNA.Hit.1.light","MoNA.Hit.1.t","MoNA.Hit.1.x","MoNA.Hit.1.y","MoNA.Hit.1.z",
                "MoNA.Hit.2.e","MoNA.Hit.2.light","MoNA.Hit.2.t","MoNA.Hit.2.x","MoNA.Hit.2.y","MoNA.Hit.2.z",
                "MoNA.Hit.3.e","MoNA.Hit.3.light","MoNA.Hit.3.t","MoNA.Hit.3.x","MoNA.Hit.3.y","MoNA.Hit.3.z",
                "MoNA.Hit.4.e","MoNA.Hit.4.light","MoNA.Hit.4.t","MoNA.Hit.4.x","MoNA.Hit.4.y","MoNA.Hit.4.z",
                "MoNA.Hit.5.e","MoNA.Hit.5.light","MoNA.Hit.5.t","MoNA.Hit.5.x","MoNA.Hit.5.y","MoNA.Hit.5.z",
                "MoNA.Hit.6.e","MoNA.Hit.6.light","MoNA.Hit.6.t","MoNA.Hit.6.x","MoNA.Hit.6.y","MoNA.Hit.6.z",
                "MoNA.Hit.7.e","MoNA.Hit.7.light","MoNA.Hit.7.t","MoNA.Hit.7.x","MoNA.Hit.7.y","MoNA.Hit.7.z",
                "MoNA.Hit.8.e","MoNA.Hit.8.light","MoNA.Hit.8.t","MoNA.Hit.8.x","MoNA.Hit.8.y","MoNA.Hit.8.z",
                "MoNA.Hit.9.e","MoNA.Hit.9.light","MoNA.Hit.9.t","MoNA.Hit.9.x","MoNA.Hit.9.y","MoNA.Hit.9.z",
                "MoNA.Hit.10.e","MoNA.Hit.10.light","MoNA.Hit.10.t","MoNA.Hit.10.x","MoNA.Hit.10.y","MoNA.Hit.10.z",
                };

            double* ps_n3[] = {&mh3n1Edep,&mh3n1light,&mh3n1t,&mh3n1x,&mh3n1y,&mh3n1z,
                &mh3n2Edep,&mh3n2light,&mh3n2t,&mh3n2x,&mh3n2y,&mh3n2z,
                &mh3n3Edep,&mh3n3light,&mh3n3t,&mh3n3x,&mh3n3y,&mh3n3z,
                &mh3n4Edep,&mh3n4light,&mh3n4t,&mh3n4x,&mh3n4y,&mh3n4z,
                &mh3n5Edep,&mh3n5light,&mh3n5t,&mh3n5x,&mh3n5y,&mh3n5z,
                &mh3n6Edep,&mh3n6light,&mh3n6t,&mh3n6x,&mh3n6y,&mh3n6z,
                &mh3n7Edep,&mh3n7light,&mh3n7t,&mh3n7x,&mh3n7y,&mh3n7z,
                &mh3n8Edep,&mh3n8light,&mh3n8t,&mh3n8x,&mh3n8y,&mh3n8z,
                &mh3n9Edep,&mh3n9light,&mh3n9t,&mh3n9x,&mh3n9y,&mh3n9z,
                &mh3n10Edep,&mh3n10light,&mh3n10t,&mh3n10x,&mh3n10y,&mh3n10z,
                };

            for (size_t i(0); i<sizeof(ps_n3)/sizeof(double*); i++) {
                tn3Temp->SetBranchAddress(bn_n3[i].c_str(),ps_n3[i]);  
            }

            tn3Temp->SetBranchAddress("MoNA.multiplicity",&mh3nmultiplicity); 
            tn3Temp->SetBranchAddress("MoNA.Z1mult",&mh3_z1);  
            tn3Temp->SetBranchAddress("MoNA.Z2mult",&mh3_z2);  
            tn3Temp->SetBranchAddress("MoNA.ZGt2mult",&mh3_zgt2);  
            tn3Temp->SetBranchAddress("MoNA.Gmult",&mh3_g);  

            // Get number of entries in the file
            Int_t outputn3_nevents = tn3Temp->GetEntries();


	// Then open the GEANT output for the 4th neutron root file
	    TString n4name = dirdata+"/neutron4"+uniqueID+".root";
		cout << "Found 4th file: " << n4name << endl;
            TFile *temp4File = new TFile(n4name); assert(temp4File);
            TTree *tn4Temp = (TTree*)temp4File->Get("t"); assert(tn4Temp);

            Double_t mh4n1Edep,mh4n1light,mh4n1t,mh4n1x,mh4n1y,mh4n1z,mh4n2Edep,mh4n2light,mh4n2t,mh4n2x,mh4n2y,mh4n2z,mh4n3Edep,mh4n3light,mh4n3t,mh4n3x,mh4n3y,mh4n3z,mh4n4Edep,mh4n4light,mh4n4t,mh4n4x,mh4n4y,mh4n4z,mh4n5Edep,mh4n5light,mh4n5t,mh4n5x,mh4n5y,mh4n5z,mh4n6Edep,mh4n6light,mh4n6t,mh4n6x,mh4n6y,mh4n6z,mh4n7Edep,mh4n7light,mh4n7t,mh4n7x,mh4n7y,mh4n7z,mh4n8Edep,mh4n8light,mh4n8t,mh4n8x,mh4n8y,mh4n8z,mh4n9Edep,mh4n9light,mh4n9t,mh4n9x,mh4n9y,mh4n9z,mh4n10Edep,mh4n10light,mh4n10t,mh4n10x,mh4n10y,mh4n10z;

            Int_t mh4nmultiplicity;
            Int_t mh4_z1; //zwk particle multiplicities
            Int_t mh4_z2;
            Int_t mh4_zgt2;
            Int_t mh4_g;

            string bn_n4[] = {"MoNA.Hit.1.e","MoNA.Hit.1.light","MoNA.Hit.1.t","MoNA.Hit.1.x","MoNA.Hit.1.y","MoNA.Hit.1.z",
                "MoNA.Hit.2.e","MoNA.Hit.2.light","MoNA.Hit.2.t","MoNA.Hit.2.x","MoNA.Hit.2.y","MoNA.Hit.2.z",
                "MoNA.Hit.3.e","MoNA.Hit.3.light","MoNA.Hit.3.t","MoNA.Hit.3.x","MoNA.Hit.3.y","MoNA.Hit.3.z",
                "MoNA.Hit.4.e","MoNA.Hit.4.light","MoNA.Hit.4.t","MoNA.Hit.4.x","MoNA.Hit.4.y","MoNA.Hit.4.z",
                "MoNA.Hit.5.e","MoNA.Hit.5.light","MoNA.Hit.5.t","MoNA.Hit.5.x","MoNA.Hit.5.y","MoNA.Hit.5.z",
                "MoNA.Hit.6.e","MoNA.Hit.6.light","MoNA.Hit.6.t","MoNA.Hit.6.x","MoNA.Hit.6.y","MoNA.Hit.6.z",
                "MoNA.Hit.7.e","MoNA.Hit.7.light","MoNA.Hit.7.t","MoNA.Hit.7.x","MoNA.Hit.7.y","MoNA.Hit.7.z",
                "MoNA.Hit.8.e","MoNA.Hit.8.light","MoNA.Hit.8.t","MoNA.Hit.8.x","MoNA.Hit.8.y","MoNA.Hit.8.z",
                "MoNA.Hit.9.e","MoNA.Hit.9.light","MoNA.Hit.9.t","MoNA.Hit.9.x","MoNA.Hit.9.y","MoNA.Hit.9.z",
                "MoNA.Hit.10.e","MoNA.Hit.10.light","MoNA.Hit.10.t","MoNA.Hit.10.x","MoNA.Hit.10.y","MoNA.Hit.10.z",
                };

            double* ps_n4[] = {&mh4n1Edep,&mh4n1light,&mh4n1t,&mh4n1x,&mh4n1y,&mh4n1z,
                &mh4n2Edep,&mh4n2light,&mh4n2t,&mh4n2x,&mh4n2y,&mh4n2z,
                &mh4n3Edep,&mh4n3light,&mh4n3t,&mh4n3x,&mh4n3y,&mh4n3z,
                &mh4n4Edep,&mh4n4light,&mh4n4t,&mh4n4x,&mh4n4y,&mh4n4z,
                &mh4n5Edep,&mh4n5light,&mh4n5t,&mh4n5x,&mh4n5y,&mh4n5z,
                &mh4n6Edep,&mh4n6light,&mh4n6t,&mh4n6x,&mh4n6y,&mh4n6z,
                &mh4n7Edep,&mh4n7light,&mh4n7t,&mh4n7x,&mh4n7y,&mh4n7z,
                &mh4n8Edep,&mh4n8light,&mh4n8t,&mh4n8x,&mh4n8y,&mh4n8z,
                &mh4n9Edep,&mh4n9light,&mh4n9t,&mh4n9x,&mh4n9y,&mh4n9z,
                &mh4n10Edep,&mh4n10light,&mh4n10t,&mh4n10x,&mh4n10y,&mh4n10z,
                };

            for (size_t i(0); i<sizeof(ps_n4)/sizeof(double*); i++) {
                tn4Temp->SetBranchAddress(bn_n4[i].c_str(),ps_n4[i]);  
            }

            tn4Temp->SetBranchAddress("MoNA.multiplicity",&mh4nmultiplicity); 
            tn4Temp->SetBranchAddress("MoNA.Z1mult",&mh4_z1);  
            tn4Temp->SetBranchAddress("MoNA.Z2mult",&mh4_z2);  
            tn4Temp->SetBranchAddress("MoNA.ZGt2mult",&mh4_zgt2);  
            tn4Temp->SetBranchAddress("MoNA.Gmult",&mh4_g);  

            // Get number of entries in the file
            Int_t outputn4_nevents = tn4Temp->GetEntries();


	   

            // Finally open the output .root file
            TFile *outFile = new TFile(argv[2],"recreate"); assert(outFile);
            TTree *tOut = new TTree("t","Simulation Output"); assert(tOut);

            // We want to preserve the original st-mona parameters
            TBranch *obb1p0x = tOut->Branch("b1p0x",&b1p0x,"b1p0x/D");
            TBranch *obb1p0tx = tOut->Branch("b1p0tx",&b1p0tx,"b1p0tx/D");
            TBranch *obb1p0y = tOut->Branch("b1p0y",&b1p0y,"b1p0y/D");
            TBranch *obb1p0ty = tOut->Branch("b1p0ty",&b1p0ty,"b1p0ty/D");
            TBranch *obb1p0Ekin = tOut->Branch("b1p0Ekin",&b1p0Ekin,"b1p0Ekin/D");
            TBranch *obb2p0x = tOut->Branch("b2p0x",&b2p0x,"b2p0x/D");
            TBranch *obb2p0tx = tOut->Branch("b2p0tx",&b2p0tx,"b2p0tx/D");
            TBranch *obb2p0y = tOut->Branch("b2p0y",&b2p0y,"b2p0y/D");
            TBranch *obb2p0ty = tOut->Branch("b2p0ty",&b2p0ty,"b2p0ty/D");
            TBranch *obb2p0t = tOut->Branch("b2p0t",&b2p0t,"b2p0t/D");
            TBranch *obb2p0z = tOut->Branch("b2p0z",&b2p0z,"b2p0z/D");
            TBranch *obb2p0TP = tOut->Branch("b2p0TP",&b2p0TP,"b2p0TP/D");
            TBranch *obb2p0dE = tOut->Branch("b2p0dE",&b2p0dE,"b2p0dE/D");
            TBranch *obb2p0Ekin = tOut->Branch("b2p0Ekin",&b2p0Ekin,"b2p0Ekin/D");
            TBranch *obb2p0R_pz = tOut->Branch("b2p0R_pz",&b2p0R_pz,"b2p0R_pz/D");
            TBranch *obb2p0R_exen = tOut->Branch("b2p0R_exen",&b2p0R_exen,"b2p0R_exen/D");
            TBranch *obb2p0R_exen2 = tOut->Branch("b2p0R_exen2",&b2p0R_exen2,"b2p0R_exen2/D");
	    TBranch *obb2p0R_exen3 = tOut->Branch("b2p0R_exen3",&b2p0R_exen3,"b2p0R_exen3/D");
	    TBranch *obb2p0R_exen4 = tOut->Branch("b2p0R_exen4",&b2p0R_exen4,"b2p0R_exen4/D");

            TBranch *obb2p0R_ph = tOut->Branch("b2p0R_ph",&b2p0R_ph,"b2p0R_ph/D");
            TBranch *obb2p0R_ph2 = tOut->Branch("b2p0R_ph2",&b2p0R_ph2,"b2p0R_ph2/D");
            TBranch *obb2p0R_ph3 = tOut->Branch("b2p0R_ph3",&b2p0R_ph3,"b2p0R_ph3/D");
	    TBranch *obb2p0R_ph4 = tOut->Branch("b2p0R_ph4",&b2p0R_ph4,"b2p0R_ph4/D");

            TBranch *obb2p0R_th = tOut->Branch("b2p0R_th",&b2p0R_th,"b2p0R_th/D");
            TBranch *obb2p0R_th2 = tOut->Branch("b2p0R_th2",&b2p0R_th2,"b2p0R_th2/D");
            TBranch *obb2p0R_th3 = tOut->Branch("b2p0R_th3",&b2p0R_th3,"b2p0R_th3/D");
	    TBranch *obb2p0R_th4 = tOut->Branch("b2p0R_th4",&b2p0R_th4,"b2p0R_th4/D");

            TBranch *obb7p0x = tOut->Branch("b7p0x",&b4p0x,"b7p0x/D");
            TBranch *obb7p0tx = tOut->Branch("b7p0tx",&b4p0tx,"b7p0tx/D");
            TBranch *obb7p0y = tOut->Branch("b7p0y",&b4p0y,"b7p0y/D");
            TBranch *obb7p0ty = tOut->Branch("b7p0ty",&b4p0ty,"b7p0ty/D");
            TBranch *obb7p0t = tOut->Branch("b7p0t",&b4p0t,"b7p0t/D");
            TBranch *obb7p0Ekin = tOut->Branch("b7p0Ekin",&b4p0Ekin,"b7p0Ekin/D");
            TBranch *obb9p0x = tOut->Branch("b9p0x",&b6p0x,"b9p0x/D");
            TBranch *obb9p0tx = tOut->Branch("b9p0tx",&b6p0tx,"b9p0tx/D");
            TBranch *obb9p0y = tOut->Branch("b9p0y",&b6p0y,"b9p0y/D");
            TBranch *obb9p0ty = tOut->Branch("b9p0ty",&b6p0ty,"b9p0ty/D");
            TBranch *obb9p0Ekin = tOut->Branch("b9p0Ekin",&b6p0Ekin,"b9p0Ekin/D");
            
  	    TBranch *obb13p1t = tOut->Branch("b13p1t",&b9p1t,"b13p1t/D");
            TBranch *obb13p1x = tOut->Branch("b13p1x",&b9p1x,"b13p1x/D");
            TBranch *obb13p1y = tOut->Branch("b13p1y",&b9p1y,"b13p1y/D");
            TBranch *obb13p1z = tOut->Branch("b13p1z",&b9p1z,"b13p1z/D");
            TBranch *obb13p1Ekin = tOut->Branch("b13p1Ekin",&b9p1Ekin,"b13p1Ekin/D");
            TBranch *obb13p2t = tOut->Branch("b13p2t",&b9p2t,"b13p2t/D");
            TBranch *obb13p2x = tOut->Branch("b13p2x",&b9p2x,"b13p2x/D");
            TBranch *obb13p2y = tOut->Branch("b13p2y",&b9p2y,"b13p2y/D");
            TBranch *obb13p2z = tOut->Branch("b13p2z",&b9p2z,"b13p2z/D");
            TBranch *obb13p2Ekin = tOut->Branch("b13p2Ekin",&b9p2Ekin,"b13p2Ekin/D");
	    TBranch *obb13p3t = tOut->Branch("b13p3t",&b9p3t,"b13p3t/D");
            TBranch *obb13p3x = tOut->Branch("b13p3x",&b9p3x,"b13p3x/D");
            TBranch *obb13p3y = tOut->Branch("b13p3y",&b9p3y,"b13p3y/D");
            TBranch *obb13p3z = tOut->Branch("b13p3z",&b9p3z,"b13p3z/D");
            TBranch *obb13p3Ekin = tOut->Branch("b13p3Ekin",&b9p3Ekin,"b13p3Ekin/D");
            TBranch *obb13p4t = tOut->Branch("b13p4t",&b9p4t,"b13p4t/D");
            TBranch *obb13p4x = tOut->Branch("b13p4x",&b9p4x,"b13p4x/D");
            TBranch *obb13p4y = tOut->Branch("b13p4y",&b9p4y,"b13p4y/D");
            TBranch *obb13p4z = tOut->Branch("b13p4z",&b9p4z,"b13p4z/D");
            TBranch *obb13p4Ekin = tOut->Branch("b13p4Ekin",&b9p4Ekin,"b13p4Ekin/D");


            // And add in the new GEANT parameters... note that they are simply pg instead of p
	    // 1st Neutron
            tOut->Branch("b13pn1gmultiplicity",&mh1nmultiplicity,"b13pn1gmultiplicity/I");
            tOut->Branch("b13pn1gZ1Mult",&mh1_z1,"b13pn1gZ1Mult/I");//zwk particle mults
            tOut->Branch("b13pn1gZ2Mult",&mh1_z2,"b13pn1gZ2Mult/I");
            tOut->Branch("b13pn1gZGt2Mult",&mh1_zgt2,"b13pn1gZGt2Mult/I");
            tOut->Branch("b13pn1gGammaMult",&mh1_g,"b13pn1gGammaMult/I");

            tOut->Branch("b13pn1g1Edep",&mh1n1Edep,"b13pn1g1Edep/D");
            tOut->Branch("b13pn1g1light",&mh1n1light,"b13pn1g1light/D");
            tOut->Branch("b13pn1g1t",&mh1n1t,"b13pn1g1t/D");
            tOut->Branch("b13pn1g1x",&mh1n1x,"b13pn1g1x/D");
            tOut->Branch("b13pn1g1y",&mh1n1y,"b13pn1g1y/D");
            tOut->Branch("b13pn1g1z",&mh1n1z,"b13pn1g1z/D");
            tOut->Branch("b13pn1g2Edep",&mh1n2Edep,"b13pn1g2Edep/D");
            tOut->Branch("b13pn1g2light",&mh1n2light,"b13pn1g2light/D");
            tOut->Branch("b13pn1g2t",&mh1n2t,"b13pn1g2t/D");
            tOut->Branch("b13pn1g2x",&mh1n2x,"b13pn1g2x/D");
            tOut->Branch("b13pn1g2y",&mh1n2y,"b13pn1g2y/D");
            tOut->Branch("b13pn1g2z",&mh1n2z,"b13pn1g2z/D");
            tOut->Branch("b13pn1g3Edep",&mh1n3Edep,"b13pn1g3Edep/D");
            tOut->Branch("b13pn1g3light",&mh1n3light,"b13pn1g3light/D");
            tOut->Branch("b13pn1g3t",&mh1n3t,"b13pn1g3t/D");
            tOut->Branch("b13pn1g3x",&mh1n3x,"b13pn1g3x/D");
            tOut->Branch("b13pn1g3y",&mh1n3y,"b13pn1g3y/D");
            tOut->Branch("b13pn1g3z",&mh1n3z,"b13pn1g3z/D");
            tOut->Branch("b13pn1g4Edep",&mh1n4Edep,"b13pn1g4Edep/D");
            tOut->Branch("b13pn1g4light",&mh1n4light,"b13pn1g4light/D");
            tOut->Branch("b13pn1g4t",&mh1n4t,"b13pn1g4t/D");
            tOut->Branch("b13pn1g4x",&mh1n4x,"b13pn1g4x/D");
            tOut->Branch("b13pn1g4y",&mh1n4y,"b13pn1g4y/D");
            tOut->Branch("b13pn1g4z",&mh1n4z,"b13pn1g4z/D");
            tOut->Branch("b13pn1g5Edep",&mh1n5Edep,"b13pn1g5Edep/D");
            tOut->Branch("b13pn1g5light",&mh1n5light,"b13pn1g5light/D");
            tOut->Branch("b13pn1g5t",&mh1n5t,"b13pn1g5t/D");
            tOut->Branch("b13pn1g5x",&mh1n5x,"b13pn1g5x/D");
            tOut->Branch("b13pn1g5y",&mh1n5y,"b13pn1g5y/D");
            tOut->Branch("b13pn1g5z",&mh1n5z,"b13pn1g5z/D");
            tOut->Branch("b13pn1g6Edep",&mh1n6Edep,"b13pn1g6Edep/D");
            tOut->Branch("b13pn1g6light",&mh1n6light,"b13pn1g6light/D");
            tOut->Branch("b13pn1g6t",&mh1n6t,"b13pn1g6t/D");
            tOut->Branch("b13pn1g6x",&mh1n6x,"b13pn1g6x/D");
            tOut->Branch("b13pn1g6y",&mh1n6y,"b13pn1g6y/D");
            tOut->Branch("b13pn1g6z",&mh1n6z,"b13pn1g6z/D");
            tOut->Branch("b13pn1g7Edep",&mh1n7Edep,"b13pn1g7Edep/D");
            tOut->Branch("b13pn1g7light",&mh1n7light,"b13pn1g7light/D");
            tOut->Branch("b13pn1g7t",&mh1n7t,"b13pn1g7t/D");
            tOut->Branch("b13pn1g7x",&mh1n7x,"b13pn1g7x/D");
            tOut->Branch("b13pn1g7y",&mh1n7y,"b13pn1g7y/D");
            tOut->Branch("b13pn1g7z",&mh1n7z,"b13pn1g7z/D");
            tOut->Branch("b13pn1g8Edep",&mh1n8Edep,"b13pn1g8Edep/D");
            tOut->Branch("b13pn1g8light",&mh1n8light,"b13pn1g8light/D");
            tOut->Branch("b13pn1g8t",&mh1n8t,"b13pn1g8t/D");
            tOut->Branch("b13pn1g8x",&mh1n8x,"b13pn1g8x/D");
            tOut->Branch("b13pn1g8y",&mh1n8y,"b13pn1g8y/D");
            tOut->Branch("b13pn1g8z",&mh1n8z,"b13pn1g8z/D");
            tOut->Branch("b13pn1g9Edep",&mh1n9Edep,"b13pn1g9Edep/D");
            tOut->Branch("b13pn1g9light",&mh1n9light,"b13pn1g9light/D");
            tOut->Branch("b13pn1g9t",&mh1n9t,"b13pn1g9t/D");
            tOut->Branch("b13pn1g9x",&mh1n9x,"b13pn1g9x/D");
            tOut->Branch("b13pn1g9y",&mh1n9y,"b13pn1g9y/D");
            tOut->Branch("b13pn1g9z",&mh1n9z,"b13pn1g9z/D");
            tOut->Branch("b13pn1g10Edep",&mh1n10Edep,"b13pn1g10Edep/D");
            tOut->Branch("b13pn1g10light",&mh1n10light,"b13pn1g10light/D");
            tOut->Branch("b13pn1g10t",&mh1n10t,"b13pn1g10t/D");
            tOut->Branch("b13pn1g10x",&mh1n10x,"b13pn1g10x/D");
            tOut->Branch("b13pn1g10y",&mh1n10y,"b13pn1g10y/D");
            tOut->Branch("b13pn1g10z",&mh1n10z,"b13pn1g10z/D");

            // And add in the new GEANT parameters... note that they are simply pg instead of p
	    // 2nd Neutron
            tOut->Branch("b13pn2gmultiplicity",&mh2nmultiplicity,"b13pn2gmultiplicity/I");
		//(All four had b13pn1gZ%Mult/I)
            tOut->Branch("b13pn2gZ1Mult",&mh2_z1,"b13pn2gZ1Mult/I");//zwk particle mults
            tOut->Branch("b13pn2gZ2Mult",&mh2_z2,"b13pn2gZ2Mult/I");
            tOut->Branch("b13pn2gZGt2Mult",&mh2_zgt2,"b13pn2gZGt2Mult/I");// Is this a mistake? MDJ
            tOut->Branch("b13pn2gGammaMult",&mh2_g,"b13pn2gGammaMult/I");
		//
            tOut->Branch("b13pn2g1Edep",&mh2n1Edep,"b13pn2g1Edep/D");
            tOut->Branch("b13pn2g1light",&mh2n1light,"b13pn2g1light/D");
            tOut->Branch("b13pn2g1t",&mh2n1t,"b13pn2g1t/D");
            tOut->Branch("b13pn2g1x",&mh2n1x,"b13pn2g1x/D");
            tOut->Branch("b13pn2g1y",&mh2n1y,"b13pn2g1y/D");
            tOut->Branch("b13pn2g1z",&mh2n1z,"b13pn2g1z/D");
            tOut->Branch("b13pn2g2Edep",&mh2n2Edep,"b13pn2g2Edep/D");
            tOut->Branch("b13pn2g2light",&mh2n2light,"b13pn2g2light/D");
            tOut->Branch("b13pn2g2t",&mh2n2t,"b13pn2g2t/D");
            tOut->Branch("b13pn2g2x",&mh2n2x,"b13pn2g2x/D");
            tOut->Branch("b13pn2g2y",&mh2n2y,"b13pn2g2y/D");
            tOut->Branch("b13pn2g2z",&mh2n2z,"b13pn2g2z/D");
            tOut->Branch("b13pn2g3Edep",&mh2n3Edep,"b13pn2g3Edep/D");
            tOut->Branch("b13pn2g3light",&mh2n3light,"b13pn2g3light/D");
            tOut->Branch("b13pn2g3t",&mh2n3t,"b13pn2g3t/D");
            tOut->Branch("b13pn2g3x",&mh2n3x,"b13pn2g3x/D");
            tOut->Branch("b13pn2g3y",&mh2n3y,"b13pn2g3y/D");
            tOut->Branch("b13pn2g3z",&mh2n3z,"b13pn2g3z/D");
            tOut->Branch("b13pn2g4Edep",&mh2n4Edep,"b13pn2g4Edep/D");
            tOut->Branch("b13pn2g4light",&mh2n4light,"b13pn2g4light/D");
            tOut->Branch("b13pn2g4t",&mh2n4t,"b13pn2g4t/D");
            tOut->Branch("b13pn2g4x",&mh2n4x,"b13pn2g4x/D");
            tOut->Branch("b13pn2g4y",&mh2n4y,"b13pn2g4y/D");
            tOut->Branch("b13pn2g4z",&mh2n4z,"b13pn2g4z/D");
            tOut->Branch("b13pn2g5Edep",&mh2n5Edep,"b13pn2g5Edep/D");
            tOut->Branch("b13pn2g5light",&mh2n5light,"b13pn2g5light/D");
            tOut->Branch("b13pn2g5t",&mh2n5t,"b13pn2g5t/D");
            tOut->Branch("b13pn2g5x",&mh2n5x,"b13pn2g5x/D");
            tOut->Branch("b13pn2g5y",&mh2n5y,"b13pn2g5y/D");
            tOut->Branch("b13pn2g5z",&mh2n5z,"b13pn2g5z/D");
            tOut->Branch("b13pn2g6Edep",&mh2n6Edep,"b13pn2g6Edep/D");
            tOut->Branch("b13pn2g6light",&mh2n6light,"b13pn2g6light/D");
            tOut->Branch("b13pn2g6t",&mh2n6t,"b13pn2g6t/D");
            tOut->Branch("b13pn2g6x",&mh2n6x,"b13pn2g6x/D");
            tOut->Branch("b13pn2g6y",&mh2n6y,"b13pn2g6y/D");
            tOut->Branch("b13pn2g6z",&mh2n6z,"b13pn2g6z/D");
            tOut->Branch("b13pn2g7Edep",&mh2n7Edep,"b13pn2g7Edep/D");
            tOut->Branch("b13pn2g7light",&mh2n7light,"b13pn2g7light/D");
            tOut->Branch("b13pn2g7t",&mh2n7t,"b13pn2g7t/D");
            tOut->Branch("b13pn2g7x",&mh2n7x,"b13pn2g7x/D");
            tOut->Branch("b13pn2g7y",&mh2n7y,"b13pn2g7y/D");
            tOut->Branch("b13pn2g7z",&mh2n7z,"b13pn2g7z/D");
            tOut->Branch("b13pn2g8Edep",&mh2n8Edep,"b13pn2g8Edep/D");
            tOut->Branch("b13pn2g8light",&mh2n8light,"b13pn2g8light/D");
            tOut->Branch("b13pn2g8t",&mh2n8t,"b13pn2g8t/D");
            tOut->Branch("b13pn2g8x",&mh2n8x,"b13pn2g8x/D");
            tOut->Branch("b13pn2g8y",&mh2n8y,"b13pn2g8y/D");
            tOut->Branch("b13pn2g8z",&mh2n8z,"b13pn2g8z/D");
            tOut->Branch("b13pn2g9Edep",&mh2n9Edep,"b13pn2g9Edep/D");
            tOut->Branch("b13pn2g9light",&mh2n9light,"b13pn2g9light/D");
            tOut->Branch("b13pn2g9t",&mh2n9t,"b13pn2g9t/D");
            tOut->Branch("b13pn2g9x",&mh2n9x,"b13pn2g9x/D");
            tOut->Branch("b13pn2g9y",&mh2n9y,"b13pn2g9y/D");
            tOut->Branch("b13pn2g9z",&mh2n9z,"b13pn2g9z/D");
            tOut->Branch("b13pn2g10Edep",&mh2n10Edep,"b13pn2g10Edep/D");
            tOut->Branch("b13pn2g10light",&mh2n10light,"b13pn2g10light/D");
            tOut->Branch("b13pn2g10t",&mh2n10t,"b13pn2g10t/D");
            tOut->Branch("b13pn2g10x",&mh2n10x,"b13pn2g10x/D");
            tOut->Branch("b13pn2g10y",&mh2n10y,"b13pn2g10y/D");
            tOut->Branch("b13pn2g10z",&mh2n10z,"b13pn2g10z/D");

	    // And add in the new GEANT parameters... note that they are simply pg instead of p
	    // 3rd Neutron
            tOut->Branch("b13pn3gmultiplicity",&mh3nmultiplicity,"b13pn3gmultiplicity/I");

            tOut->Branch("b13pn3gZ1Mult",&mh3_z1,"b13pn3gZ1Mult/I");//zwk particle mults
            tOut->Branch("b13pn3gZ2Mult",&mh3_z2,"b13pn3gZ2Mult/I");
            tOut->Branch("b13pn3gZGt2Mult",&mh3_zgt2,"b13pn3gZGt2Mult/I");
            tOut->Branch("b13pn3gGammaMult",&mh3_g,"b13pn3gGammaMult/I");

            tOut->Branch("b13pn3g1Edep",&mh3n1Edep,"b13pn3g1Edep/D");
            tOut->Branch("b13pn3g1light",&mh3n1light,"b13pn3g1light/D");
            tOut->Branch("b13pn3g1t",&mh3n1t,"b13pn3g1t/D");
            tOut->Branch("b13pn3g1x",&mh3n1x,"b13pn3g1x/D");
            tOut->Branch("b13pn3g1y",&mh3n1y,"b13pn3g1y/D");
            tOut->Branch("b13pn3g1z",&mh3n1z,"b13pn3g1z/D");
            tOut->Branch("b13pn3g2Edep",&mh3n2Edep,"b13pn3g2Edep/D");
            tOut->Branch("b13pn3g2light",&mh3n2light,"b13pn3g2light/D");
            tOut->Branch("b13pn3g2t",&mh3n2t,"b13pn3g2t/D");
            tOut->Branch("b13pn3g2x",&mh3n2x,"b13pn3g2x/D");
            tOut->Branch("b13pn3g2y",&mh3n2y,"b13pn3g2y/D");
            tOut->Branch("b13pn3g2z",&mh3n2z,"b13pn3g2z/D");
            tOut->Branch("b13pn3g3Edep",&mh3n3Edep,"b13pn3g3Edep/D");
            tOut->Branch("b13pn3g3light",&mh3n3light,"b13pn3g3light/D");
            tOut->Branch("b13pn3g3t",&mh3n3t,"b13pn3g3t/D");
            tOut->Branch("b13pn3g3x",&mh3n3x,"b13pn3g3x/D");
            tOut->Branch("b13pn3g3y",&mh3n3y,"b13pn3g3y/D");
            tOut->Branch("b13pn3g3z",&mh3n3z,"b13pn3g3z/D");
            tOut->Branch("b13pn3g4Edep",&mh3n4Edep,"b13pn3g4Edep/D");
            tOut->Branch("b13pn3g4light",&mh3n4light,"b13pn3g4light/D");
            tOut->Branch("b13pn3g4t",&mh3n4t,"b13pn3g4t/D");
            tOut->Branch("b13pn3g4x",&mh3n4x,"b13pn3g4x/D");
            tOut->Branch("b13pn3g4y",&mh3n4y,"b13pn3g4y/D");
            tOut->Branch("b13pn3g4z",&mh3n4z,"b13pn3g4z/D");
            tOut->Branch("b13pn3g5Edep",&mh3n5Edep,"b13pn3g5Edep/D");
            tOut->Branch("b13pn3g5light",&mh3n5light,"b13pn3g5light/D");
            tOut->Branch("b13pn3g5t",&mh3n5t,"b13pn3g5t/D");
            tOut->Branch("b13pn3g5x",&mh3n5x,"b13pn3g5x/D");
            tOut->Branch("b13pn3g5y",&mh3n5y,"b13pn3g5y/D");
            tOut->Branch("b13pn3g5z",&mh3n5z,"b13pn3g5z/D");
            tOut->Branch("b13pn3g6Edep",&mh3n6Edep,"b13pn3g6Edep/D");
            tOut->Branch("b13pn3g6light",&mh3n6light,"b13pn3g6light/D");
            tOut->Branch("b13pn3g6t",&mh3n6t,"b13pn3g6t/D");
            tOut->Branch("b13pn3g6x",&mh3n6x,"b13pn3g6x/D");
            tOut->Branch("b13pn3g6y",&mh3n6y,"b13pn3g6y/D");
            tOut->Branch("b13pn3g6z",&mh3n6z,"b13pn3g6z/D");
            tOut->Branch("b13pn3g7Edep",&mh3n7Edep,"b13pn3g7Edep/D");
            tOut->Branch("b13pn3g7light",&mh3n7light,"b13pn3g7light/D");
            tOut->Branch("b13pn3g7t",&mh3n7t,"b13pn3g7t/D");
            tOut->Branch("b13pn3g7x",&mh3n7x,"b13pn3g7x/D");
            tOut->Branch("b13pn3g7y",&mh3n7y,"b13pn3g7y/D");
            tOut->Branch("b13pn3g7z",&mh3n7z,"b13pn3g7z/D");
            tOut->Branch("b13pn3g8Edep",&mh3n8Edep,"b13pn3g8Edep/D");
            tOut->Branch("b13pn3g8light",&mh3n8light,"b13pn3g8light/D");
            tOut->Branch("b13pn3g8t",&mh3n8t,"b13pn3g8t/D");
            tOut->Branch("b13pn3g8x",&mh3n8x,"b13pn3g8x/D");
            tOut->Branch("b13pn3g8y",&mh3n8y,"b13pn3g8y/D");
            tOut->Branch("b13pn3g8z",&mh3n8z,"b13pn3g8z/D");
            tOut->Branch("b13pn3g9Edep",&mh3n9Edep,"b13pn3g9Edep/D");
            tOut->Branch("b13pn3g9light",&mh3n9light,"b13pn3g9light/D");
            tOut->Branch("b13pn3g9t",&mh3n9t,"b13pn3g9t/D");
            tOut->Branch("b13pn3g9x",&mh3n9x,"b13pn3g9x/D");
            tOut->Branch("b13pn3g9y",&mh3n9y,"b13pn3g9y/D");
            tOut->Branch("b13pn3g9z",&mh3n9z,"b13pn3g9z/D");
            tOut->Branch("b13pn3g10Edep",&mh3n10Edep,"b13pn3g10Edep/D");
            tOut->Branch("b13pn3g10light",&mh3n10light,"b13pn3g10light/D");
            tOut->Branch("b13pn3g10t",&mh3n10t,"b13pn3g10t/D");
            tOut->Branch("b13pn3g10x",&mh3n10x,"b13pn3g10x/D");
            tOut->Branch("b13pn3g10y",&mh3n10y,"b13pn3g10y/D");
            tOut->Branch("b13pn3g10z",&mh3n10z,"b13pn3g10z/D");

	    // And add in the new GEANT parameters... note that they are simply pg instead of p
	    // 4th Neutron
            tOut->Branch("b13pn4gmultiplicity",&mh4nmultiplicity,"b13pn4gmultiplicity/I");

            tOut->Branch("b13pn4gZ1Mult",&mh4_z1,"b13pn4gZ1Mult/I");//zwk particle mults
            tOut->Branch("b13pn4gZ2Mult",&mh4_z2,"b13pn3gZ2Mult/I");
            tOut->Branch("b13pn4gZGt2Mult",&mh4_zgt2,"b13pn4gZGt2Mult/I");
            tOut->Branch("b13pn4gGammaMult",&mh4_g,"b13pn4gGammaMult/I");

            tOut->Branch("b13pn4g1Edep",&mh4n1Edep,"b13pn4g1Edep/D");
            tOut->Branch("b13pn4g1light",&mh4n1light,"b13pn4g1light/D");
            tOut->Branch("b13pn4g1t",&mh4n1t,"b13pn4g1t/D");
            tOut->Branch("b13pn4g1x",&mh4n1x,"b13pn4g1x/D");
            tOut->Branch("b13pn4g1y",&mh4n1y,"b13pn4g1y/D");
            tOut->Branch("b13pn4g1z",&mh4n1z,"b13pn4g1z/D");
            tOut->Branch("b13pn4g2Edep",&mh4n2Edep,"b13pn4g2Edep/D");
            tOut->Branch("b13pn4g2light",&mh4n2light,"b13pn4g2light/D");
            tOut->Branch("b13pn4g2t",&mh4n2t,"b13pn4g2t/D");
            tOut->Branch("b13pn4g2x",&mh4n2x,"b13pn4g2x/D");
            tOut->Branch("b13pn4g2y",&mh4n2y,"b13pn4g2y/D");
            tOut->Branch("b13pn4g2z",&mh4n2z,"b13pn4g2z/D");
            tOut->Branch("b13pn4g3Edep",&mh4n3Edep,"b13pn4g3Edep/D");
            tOut->Branch("b13pn4g3light",&mh4n3light,"b13pn4g3light/D");
            tOut->Branch("b13pn4g3t",&mh4n3t,"b13pn4g3t/D");
            tOut->Branch("b13pn4g3x",&mh4n3x,"b13pn4g3x/D");
            tOut->Branch("b13pn4g3y",&mh4n3y,"b13pn4g3y/D");
            tOut->Branch("b13pn4g3z",&mh4n3z,"b13pn4g3z/D");
            tOut->Branch("b13pn4g4Edep",&mh4n4Edep,"b13pn4g4Edep/D");
            tOut->Branch("b13pn4g4light",&mh4n4light,"b13pn4g4light/D");
            tOut->Branch("b13pn4g4t",&mh4n4t,"b13pn4g4t/D");
            tOut->Branch("b13pn4g4x",&mh4n4x,"b13pn4g4x/D");
            tOut->Branch("b13pn4g4y",&mh4n4y,"b13pn4g4y/D");
            tOut->Branch("b13pn4g4z",&mh4n4z,"b13pn4g4z/D");
            tOut->Branch("b13pn4g5Edep",&mh4n5Edep,"b13pn4g5Edep/D");
            tOut->Branch("b13pn4g5light",&mh4n5light,"b13pn4g5light/D");
            tOut->Branch("b13pn4g5t",&mh4n5t,"b13pn4g5t/D");
            tOut->Branch("b13pn4g5x",&mh4n5x,"b13pn4g5x/D");
            tOut->Branch("b13pn4g5y",&mh4n5y,"b13pn4g5y/D");
            tOut->Branch("b13pn4g5z",&mh4n5z,"b13pn4g5z/D");
            tOut->Branch("b13pn4g6Edep",&mh4n6Edep,"b13pn4g6Edep/D");
            tOut->Branch("b13pn4g6light",&mh4n6light,"b13pn4g6light/D");
            tOut->Branch("b13pn4g6t",&mh4n6t,"b13pn4g6t/D");
            tOut->Branch("b13pn4g6x",&mh4n6x,"b13pn4g6x/D");
            tOut->Branch("b13pn4g6y",&mh4n6y,"b13pn4g6y/D");
            tOut->Branch("b13pn4g6z",&mh4n6z,"b13pn4g6z/D");
            tOut->Branch("b13pn4g7Edep",&mh4n7Edep,"b13pn4g7Edep/D");
            tOut->Branch("b13pn4g7light",&mh4n7light,"b13pn4g7light/D");
            tOut->Branch("b13pn4g7t",&mh4n7t,"b13pn4g7t/D");
            tOut->Branch("b13pn4g7x",&mh4n7x,"b13pn4g7x/D");
            tOut->Branch("b13pn4g7y",&mh4n7y,"b13pn4g7y/D");
            tOut->Branch("b13pn4g7z",&mh4n7z,"b13pn4g7z/D");
            tOut->Branch("b13pn4g8Edep",&mh4n8Edep,"b13pn4g8Edep/D");
            tOut->Branch("b13pn4g8light",&mh4n8light,"b13pn4g8light/D");
            tOut->Branch("b13pn4g8t",&mh4n8t,"b13pn4g8t/D");
            tOut->Branch("b13pn4g8x",&mh4n8x,"b13pn4g8x/D");
            tOut->Branch("b13pn4g8y",&mh4n8y,"b13pn4g8y/D");
            tOut->Branch("b13pn4g8z",&mh4n8z,"b13pn4g8z/D");
            tOut->Branch("b13pn4g9Edep",&mh4n9Edep,"b13pn4g9Edep/D");
            tOut->Branch("b13pn4g9light",&mh4n9light,"b13pn4g9light/D");
            tOut->Branch("b13pn4g9t",&mh4n9t,"b13pn4g9t/D");
            tOut->Branch("b13pn4g9x",&mh4n9x,"b13pn4g9x/D");
            tOut->Branch("b13pn4g9y",&mh4n9y,"b13pn4g9y/D");
            tOut->Branch("b13pn4g9z",&mh4n9z,"b13pn4g9z/D");
            tOut->Branch("b13pn4g10Edep",&mh4n10Edep,"b13pn4g10Edep/D");
            tOut->Branch("b13pn4g10light",&mh4n10light,"b13pn4g10light/D");
            tOut->Branch("b13pn4g10t",&mh4n10t,"b13pn4g10t/D");
            tOut->Branch("b13pn4g10x",&mh4n10x,"b13pn4g10x/D");
            tOut->Branch("b13pn4g10y",&mh4n10y,"b13pn4g10y/D");
            tOut->Branch("b13pn4g10z",&mh4n10z,"b13pn4g10z/D");


	double g1light,g2light,g3light,g4light,g5light,g6light,g7light,g8light,g9light,g10light,
	       g11light,g12light,g13light,g14light,g15light,g16light,g17light,g18light,g19light,g20light,
	       g21light,g22light,g23light,g24light,g25light,g26light,g27light,g28light,g29light,g30light,
	       g31light,g32light,g33light,g34light,g35light,g36light,g37light,g38light,g39light,g40light;


	double g1Edep,g2Edep,g3Edep,g4Edep,g5Edep,g6Edep,g7Edep,g8Edep,g9Edep,g10Edep,
	       g11Edep,g12Edep,g13Edep,g14Edep,g15Edep,g16Edep,g17Edep,g18Edep,g19Edep,g20Edep,
	       g21Edep,g22Edep,g23Edep,g24Edep,g25Edep,g26Edep,g27Edep,g28Edep,g29Edep,g30Edep,
	       g31Edep,g32Edep,g33Edep,g34Edep,g35Edep,g36Edep,g37Edep,g38Edep,g39Edep,g40Edep;

	double g1t,g2t,g3t,g4t,g5t,g6t,g7t,g8t,g9t,g10t,g11t,g12t,g13t,g14t,g15t,g16t,g17t,g18t,g19t,g20t,
	       g21t,g22t,g23t,g24t,g25t,g26t,g27t,g28t,g29t,g30t,g31t,g32t,g33t,g34t,g35t,g36t,g37t,g38t,g39t,g40t;

	double g1x,g2x,g3x,g4x,g5x,g6x,g7x,g8x,g9x,g10x,g11x,g12x,g13x,g14x,g15x,g16x,g17x,g18x,g19x,g20x,
	       g21x,g22x,g23x,g24x,g25x,g26x,g27x,g28x,g29x,g30x,g31x,g32x,g33x,g34x,g35x,g36x,g37x,g38x,g39x,g40x;

	double g1y,g2y,g3y,g4y,g5y,g6y,g7y,g8y,g9y,g10y,g11y,g12y,g13y,g14y,g15y,g16y,g17y,g18y,g19y,g20y,
	       g21y,g22y,g23y,g24y,g25y,g26y,g27y,g28y,g29y,g30y,g31y,g32y,g33y,g34y,g35y,g36y,g37y,g38y,g39y,g40y;

	double g1z,g2z,g3z,g4z,g5z,g6z,g7z,g8z,g9z,g10z,g11z,g12z,g13z,g14z,g15z,g16z,g17z,g18z,g19z,g20z,
	       g21z,g22z,g23z,g24z,g25z,g26z,g27z,g28z,g29z,g30z,g31z,g32z,g33z,g34z,g35z,g36z,g37z,g38z,g39z,g40z;


	string name[] = {"b13pg1","b13pg2","b13pg3","b13pg4","b13pg5","b13pg6","b13pg7","b13pg8","b13pg9","b13pg10",
			 "b13pg11","b13pg12","b13pg13","b13pg14","b13pg15","b13pg16","b13pg17","b13pg18","b13pg19","b13pg20",
			 "b13pg21","b13pg22","b13pg23","b13pg24","b13pg25","b13pg26","b13pg27","b13pg28","b13pg29","b13pg30",		
			 "b13pg31","b13pg32","b13pg33","b13pg34","b13pg35","b13pg36","b13pg37","b13pg38","b13pg39","b13pg40"};
	
	double* ps_light[] = {&g1light,&g2light,&g3light,&g4light,&g5light,&g6light,&g7light,&g8light,&g9light,&g10light,
			      &g11light,&g12light,&g13light,&g14light,&g15light,&g16light,&g17light,&g18light,&g19light,&g20light,
			      &g21light,&g22light,&g23light,&g24light,&g25light,&g26light,&g27light,&g28light,&g29light,&g30light,
			      &g31light,&g32light,&g33light,&g34light,&g35light,&g36light,&g37light,&g38light,&g39light,&g40light};


	double* ps_Edep[] = {&g1Edep,&g2Edep,&g3Edep,&g4Edep,&g5Edep,&g6Edep,&g7Edep,&g8Edep,&g9Edep,&g10Edep,
			     &g11Edep,&g12Edep,&g13Edep,&g14Edep,&g15Edep,&g16Edep,&g17Edep,&g18Edep,&g19Edep,&g20Edep,
			     &g21Edep,&g22Edep,&g23Edep,&g24Edep,&g25Edep,&g26Edep,&g27Edep,&g28Edep,&g29Edep,&g30Edep,
			     &g31Edep,&g32Edep,&g33Edep,&g34Edep,&g35Edep,&g36Edep,&g37Edep,&g38Edep,&g39Edep,&g40Edep};


	double* ps_t[] = {&g1t,&g2t,&g3t,&g4t,&g5t,&g6t,&g7t,&g8t,&g9t,&g10t,&g11t,&g12t,&g13t,&g14t,&g15t,&g16t,&g17t,&g18t,&g19t,&g20t,
			  &g21t,&g22t,&g23t,&g24t,&g25t,&g26t,&g27t,&g28t,&g29t,&g30t,&g31t,&g32t,&g33t,&g34t,&g35t,&g36t,&g37t,&g38t,&g39t,&g40t};

	double* ps_x[] = {&g1x,&g2x,&g3x,&g4x,&g5x,&g6x,&g7x,&g8x,&g9x,&g10x,&g11x,&g12x,&g13x,&g14x,&g15x,&g16x,&g17x,&g18x,&g19x,&g20x,
			  &g21x,&g22x,&g23x,&g24x,&g25x,&g26x,&g27x,&g28x,&g29x,&g30x,&g31x,&g32x,&g33x,&g34x,&g35x,&g36x,&g37x,&g38x,&g39x,&g40x};

	double* ps_y[] = {&g1y,&g2y,&g3y,&g4y,&g5y,&g6y,&g7y,&g8y,&g9y,&g10y,&g11y,&g12y,&g13y,&g14y,&g15y,&g16y,&g17y,&g18y,&g19y,&g20y,
			  &g21y,&g22y,&g23y,&g24y,&g25y,&g26y,&g27y,&g28y,&g29y,&g30y,&g31y,&g32y,&g33y,&g34y,&g35y,&g36y,&g37y,&g38y,&g39y,&g40y};

	double* ps_z[] = {&g1z,&g2z,&g3z,&g4z,&g5z,&g6z,&g7z,&g8z,&g9z,&g10z,&g11z,&g12z,&g13z,&g14z,&g15z,&g16z,&g17z,&g18z,&g19z,&g20z,
			  &g21z,&g22z,&g23z,&g24z,&g25z,&g26z,&g27z,&g28z,&g29z,&g30z,&g31z,&g32z,&g33z,&g34z,&g35z,&g36z,&g37z,&g38z,&g39z,&g40z};


	for(int l = 0; l < 20; l++) {
	string first_Edep = name[l]+"Edep";
	string first_light = name[l]+"light";
	string first_t = name[l]+"t";
	string first_x = name[l]+"x";
	string first_y = name[l]+"y";
	string first_z = name[l]+"z";	
	string second_Edep = name[l]+"Edep/D";
	string second_light = name[l]+"light/D";
	string second_t = name[l]+"t/D";
	string second_x = name[l]+"x/D";
	string second_y = name[l]+"y/D";
	string second_z = name[l]+"z/D";

	tOut->Branch(first_Edep.c_str(),ps_Edep[l],second_Edep.c_str());
        tOut->Branch(first_light.c_str(),ps_light[l],second_light.c_str());
        tOut->Branch(first_t.c_str(),ps_t[l],second_t.c_str());
        tOut->Branch(first_x.c_str(),ps_x[l],second_x.c_str());
        tOut->Branch(first_y.c_str(),ps_y[l],second_y.c_str());
        tOut->Branch(first_z.c_str(),ps_z[l],second_z.c_str());
	}

            // Now loop over the files and merge them into the output file we want
            // but first check for length
            if (outputn1_nevents != outputn2_nevents || outputn1_nevents != outputn3_nevents || outputn1_nevents != outputn4_nevents )
            {
                    std::cout << "The Three GEANT output files have different for the number of neutrons! This is disallowed and I am exiting\n";
                    return 1;
            
            } else if (input_nevent > outputn1_nevents)
            {
                std::cout << "ST MONA output had more events than GEANT output (" << input_nevent << " vs. " << outputn1_nevents << ") so I am only merging until I hit the end of the GEANT output.\n";
                for (Int_t i = 0; i < outputn1_nevents; i++)
                {
                    t->GetEntry(i);
                    tn1Temp->GetEntry(i);
                    tn2Temp->GetEntry(i);
		    tn3Temp->GetEntry(i);
		    tn4Temp->GetEntry(i);
                    tOut->Fill();
                }
            } else if (input_nevent < outputn1_nevents )
            {
                std::cout << "GEANT output had MORE events than ST MONA output! This is disallowed and I am exiting\n";
                return 1;
            } else
            {
                std::cout << "ST MONA and GEANT outputs have same number of events. I am happy and am merging all the data.\n";
                for (Int_t i = 0; i < outputn1_nevents; i++)
                {
                t->GetEntry(i);
                tn1Temp->GetEntry(i);
                tn2Temp->GetEntry(i);
 		tn3Temp->GetEntry(i);
		tn4Temp->GetEntry(i);
		

		double total_light[] = {mh1n1light,mh1n2light,mh1n3light,mh1n4light,mh1n5light,mh1n6light,mh1n7light,mh1n8light,mh1n9light,mh1n10light,
					mh2n1light,mh2n2light,mh2n3light,mh2n4light,mh2n5light,mh2n6light,mh2n7light,mh2n8light,mh2n9light,mh2n10light,
					mh3n1light,mh3n2light,mh3n3light,mh3n4light,mh3n5light,mh3n6light,mh3n7light,mh3n8light,mh3n9light,mh3n10light,
					mh4n1light,mh4n2light,mh4n3light,mh4n4light,mh4n5light,mh4n6light,mh4n7light,mh4n8light,mh4n9light,mh4n10light};

	 	double total_Edep[] = {mh1n1Edep,mh1n2Edep,mh1n3Edep,mh1n4Edep,mh1n5Edep,mh1n6Edep,mh1n7Edep,mh1n8Edep,mh1n9Edep,mh1n10Edep,
				       mh2n1Edep,mh2n2Edep,mh2n3Edep,mh2n4Edep,mh2n5Edep,mh2n6Edep,mh2n7Edep,mh2n8Edep,mh2n9Edep,mh2n10Edep,
				       mh3n1Edep,mh3n2Edep,mh3n3Edep,mh3n4Edep,mh3n5Edep,mh3n6Edep,mh3n7Edep,mh3n8Edep,mh3n9Edep,mh3n10Edep,
				       mh4n1Edep,mh4n2Edep,mh4n3Edep,mh4n4Edep,mh4n5Edep,mh4n6Edep,mh4n7Edep,mh4n8Edep,mh4n9Edep,mh4n10Edep};

		double total_t[] = {mh1n1t,mh1n2t,mh1n3t,mh1n4t,mh1n5t,mh1n6t,mh1n7t,mh1n8t,mh1n9t,mh1n10t,
				    mh2n1t,mh2n2t,mh2n3t,mh2n4t,mh2n5t,mh2n6t,mh2n7t,mh2n8t,mh2n9t,mh2n10t,
				    mh3n1t,mh3n2t,mh3n3t,mh3n4t,mh3n5t,mh3n6t,mh3n7t,mh3n8t,mh3n9t,mh3n10t,
				    mh4n1t,mh4n2t,mh4n3t,mh4n4t,mh4n5t,mh4n6t,mh4n7t,mh4n8t,mh4n9t,mh4n10t};

		double total_x[] = {mh1n1x,mh1n2x,mh1n3x,mh1n4x,mh1n5x,mh1n6x,mh1n7x,mh1n8x,mh1n9x,mh1n10x,
				    mh2n1x,mh2n2x,mh2n3x,mh2n4x,mh2n5x,mh2n6x,mh2n7x,mh2n8x,mh2n9x,mh2n10x,
				    mh3n1x,mh3n2x,mh3n3x,mh3n4x,mh3n5x,mh3n6x,mh3n7x,mh3n8x,mh3n9x,mh3n10x,
				    mh4n1x,mh4n2x,mh4n3x,mh4n4x,mh4n5x,mh4n6x,mh4n7x,mh4n8x,mh4n9x,mh4n10x};

		double total_y[] = {mh1n1y,mh1n2y,mh1n3y,mh1n4y,mh1n5y,mh1n6y,mh1n7y,mh1n8y,mh1n9y,mh1n10y,
				    mh2n1y,mh2n2y,mh2n3y,mh2n4y,mh2n5y,mh2n6y,mh2n7y,mh2n8y,mh2n9y,mh2n10y,
				    mh3n1y,mh3n2y,mh3n3y,mh3n4y,mh3n5y,mh3n6y,mh3n7y,mh3n8y,mh3n9y,mh3n10y,
				    mh4n1y,mh4n2y,mh4n3y,mh4n4y,mh4n5y,mh4n6y,mh4n7y,mh4n8y,mh4n9y,mh4n10y};

		double total_z[] = {mh1n1z,mh1n2z,mh1n3z,mh1n4z,mh1n5z,mh1n6z,mh1n7z,mh1n8z,mh1n9z,mh1n10z,
				    mh2n1z,mh2n2z,mh2n3z,mh2n4z,mh2n5z,mh2n6z,mh2n7z,mh2n8z,mh2n9z,mh2n10z,
				    mh3n1z,mh3n2z,mh3n3z,mh3n4z,mh3n5z,mh3n6z,mh3n7z,mh3n8z,mh3n9z,mh3n10z,
				    mh4n1z,mh4n2z,mh4n3z,mh4n4z,mh4n5z,mh4n6z,mh4n7z,mh4n8z,mh4n9z,mh4n10z};
		    


		double final_light[40];
		double final_Edep[40];
		double final_t[40];
		double final_x[40];
		double final_y[40];
		double final_z[40];

		for(int m = 0; m < 40; m++) {

		double temp_t = 10000;
		int temp_n = -1;

		for(int n = 0; n < 40; n++) {
    		if (total_t[n] > 0 && total_t[n] < temp_t)	{
			temp_t = total_t[n];
			temp_n = n;
			}
		}

		if (temp_n == -1 ) {
		for(int o = 0; o < 40; o++) {
    		if (total_Edep[o]==0 && total_light[o]==0 && total_t[o]==0 ) {
			temp_n = o;
			}
		}
		}
		//for(int i=0;i<40;i++){
		//	cout << "total_t[" << i << "]: " << total_t[i] << endl;

		//}
		final_Edep[m] = total_Edep[temp_n];
		final_light[m] = total_light[temp_n];
		final_t[m] = total_t[temp_n];
		final_x[m] = total_x[temp_n];
		final_y[m] = total_y[temp_n];
		final_z[m] = total_z[temp_n];

		total_Edep[temp_n] = 0;
		total_light[temp_n] = 0;
		total_t[temp_n] = 0;
		total_x[temp_n] = 0;
		total_y[temp_n] = 0;
		total_z[temp_n] = 0;

		}

		//for(int i=0; i<40;i++){
		//	if(final_t[i]!=0) cout << "final_t[" << i << "]: " << final_t[i] << endl;
		//	}

		// You seriously couldn't put this in a loop? 
		g1light = final_light[0]; g2light = final_light[1]; g3light = final_light[2]; g4light = final_light[3]; g5light = final_light[4];
		g1Edep = final_Edep[0];   g2Edep  = final_Edep[1];  g3Edep  = final_Edep[2];  g4Edep  = final_Edep[3];	g5Edep  = final_Edep[4];
		g1t = final_t[0];	  g2t     = final_t[1];     g3t	    = final_t[2];     g4t     = final_t[3];     g5t     = final_t[4];
		g1x = final_x[0];	  g2x     = final_x[1];     g3x     = final_x[2];     g4x     = final_x[3];     g5x     = final_x[4];
		g1y = final_y[0];	  g2y     = final_y[1];     g3y     = final_y[2];     g4y     = final_y[3];     g5y     = final_y[4];
		g1z = final_z[0];	  g2z     = final_z[1];     g3z     = final_z[2];     g4z     = final_z[3];     g5z     = final_z[4];
		g6light = final_light[5]; g7light = final_light[6]; g8light = final_light[7]; g9light = final_light[8]; g10light = final_light[9];
		g6Edep = final_Edep[5];   g7Edep  = final_Edep[6];  g8Edep  = final_Edep[7];  g9Edep  = final_Edep[8];	g10Edep  = final_Edep[9];
		g6t = final_t[5];	  g7t     = final_t[6];     g8t	    = final_t[7];     g9t     = final_t[8];     g10t     = final_t[9];
		g6x = final_x[5];	  g7x     = final_x[6];     g8x     = final_x[7];     g9x     = final_x[8];     g10x     = final_x[9];
		g6y = final_y[5];	  g7y     = final_y[6];     g8y     = final_y[7];     g9y     = final_y[8];     g10y     = final_y[9];
		g6z = final_z[5];	  g7z     = final_z[6];     g8z     = final_z[7];     g9z     = final_z[8];     g10z     = final_z[9];

		g11light = final_light[10]; g12light = final_light[11]; g13light = final_light[12]; g14light = final_light[13]; g15light = final_light[14];
		g11Edep = final_Edep[10];   g12Edep  = final_Edep[11];  g13Edep  = final_Edep[12];  g14Edep  = final_Edep[13];  g15Edep  = final_Edep[14];
		g11t = final_t[10];	    g12t     = final_t[11];     g13t     = final_t[12];     g14t     = final_t[13];     g15t     = final_t[14];
		g11x = final_x[10];	    g12x     = final_x[11];     g13x     = final_x[12];     g14x     = final_x[13];     g15x     = final_x[14];
		g11y = final_y[10];	    g12y     = final_y[11];     g13y     = final_y[12];     g14y     = final_y[13];     g15y     = final_y[14];
		g11z = final_z[10];	    g12z     = final_z[11];     g13z     = final_z[12];     g14z     = final_z[13];     g15z     = final_z[14];
		g16light = final_light[15]; g17light = final_light[16]; g18light = final_light[17]; g19light = final_light[18]; g20light = final_light[19];
		g16Edep = final_Edep[15];   g17Edep  = final_Edep[16];  g18Edep  = final_Edep[17];  g19Edep  = final_Edep[18];  g20Edep  = final_Edep[19];
		g16t = final_t[15];	    g17t     = final_t[16];     g18t     = final_t[17];     g19t     = final_t[18];     g20t     = final_t[19];
		g16x = final_x[15];	    g17x     = final_x[16];     g18x     = final_x[17];     g19x     = final_x[18];     g20x     = final_x[19];
		g16y = final_y[15];	    g17y     = final_y[16];     g18y     = final_y[17];     g19y     = final_y[18];     g20y     = final_y[19];
		g16z = final_z[15];	    g17z     = final_z[16];     g18z     = final_z[17];     g19z     = final_z[18];     g20z     = final_z[19];
	
		g21light = final_light[20]; g22light = final_light[21]; g23light = final_light[22]; g24light = final_light[23]; g25light = final_light[24];
		g21Edep = final_Edep[20];   g22Edep  = final_Edep[21];  g23Edep  = final_Edep[22];  g24Edep  = final_Edep[23];  g25Edep  = final_Edep[24];
		g21t = final_t[20];	    g22t     = final_t[21];     g23t     = final_t[22];     g24t     = final_t[23];     g25t     = final_t[24];
		g21x = final_x[20];	    g22x     = final_x[21];     g23x     = final_x[22];     g24x     = final_x[23];     g25x     = final_x[24];
		g21y = final_y[20];	    g22y     = final_y[21];     g23y     = final_y[22];     g24y     = final_y[23];     g25y     = final_y[24];
		g21z = final_z[20];	    g22z     = final_z[21];     g23z     = final_z[22];     g24z     = final_z[23];     g25z     = final_z[24];
		g26light = final_light[25]; g27light = final_light[26]; g28light = final_light[27]; g29light = final_light[28]; g30light = final_light[29];
		g26Edep = final_Edep[25];   g27Edep  = final_Edep[26];  g28Edep  = final_Edep[27];  g29Edep  = final_Edep[28];  g30Edep  = final_Edep[29];
		g26t = final_t[25];	    g27t     = final_t[26];     g28t     = final_t[27];     g29t     = final_t[28];     g30t     = final_t[29];
		g26x = final_x[25];	    g27x     = final_x[26];     g28x     = final_x[27];     g29x     = final_x[28];     g30x     = final_x[29];
		g26y = final_y[25];	    g27y     = final_y[26];     g28y     = final_y[27];     g29y     = final_y[28];     g30y     = final_y[29];
		g26z = final_z[25];	    g27z     = final_z[26];     g28z     = final_z[27];     g29z     = final_z[28];     g30z     = final_z[29];
	
		g31light = final_light[30]; g32light = final_light[31]; g33light = final_light[32]; g34light = final_light[33]; g35light = final_light[34];
		g31Edep = final_Edep[30];   g32Edep  = final_Edep[31];  g33Edep  = final_Edep[32];  g34Edep  = final_Edep[33];  g35Edep  = final_Edep[34];
		g31t = final_t[30];	    g32t     = final_t[31];     g33t     = final_t[32];     g34t     = final_t[33];     g35t     = final_t[34];
		g31x = final_x[30];	    g32x     = final_x[31];     g33x     = final_x[32];     g34x     = final_x[33];     g35x     = final_x[34];
		g31y = final_y[30];	    g32y     = final_y[31];     g33y     = final_y[32];     g34y     = final_y[33];     g35y     = final_y[34];
		g31z = final_z[30];	    g32z     = final_z[31];     g33z     = final_z[32];     g34z     = final_z[33];     g35z     = final_z[34];
		g36light = final_light[35]; g37light = final_light[36]; g38light = final_light[37]; g39light = final_light[38]; g40light = final_light[39];
		g36Edep = final_Edep[35];   g37Edep  = final_Edep[36];  g38Edep  = final_Edep[37];  g39Edep  = final_Edep[38];  g40Edep  = final_Edep[39];
		g36t = final_t[35];	    g37t     = final_t[36];     g38t     = final_t[37];     g39t     = final_t[38];     g40t     = final_t[39];
		g36x = final_x[35];	    g37x     = final_x[36];     g38x     = final_x[37];     g39x     = final_x[38];     g40x     = final_x[39];
		g36y = final_y[35];	    g37y     = final_y[36];     g38y     = final_y[37];     g39y     = final_y[38];     g40y     = final_y[39];
		g36z = final_z[35];	    g37z     = final_z[36];     g38z     = final_z[37];     g39z     = final_z[38];     g40z     = final_z[39];
	





  		/*g2light = final_light[1];
		g2Edep = final_Edep[1];
		g2t = final_t[1];
		g2x = final_x[1];
		g2y = final_y[1];
		g2z = final_z[1];
		g3light = final_light[2];
		g3Edep = final_Edep[2];
		g3t = final_t[2];
		g3x = final_x[2];
		g3y = final_y[2];
		g3z = final_z[2];
		g4light = final_light[3];
		g4Edep = final_Edep[3];
		g4t = final_t[3];
		g4x = final_x[3];
		g4y = final_y[3];
		g4z = final_z[3];
		g5light = final_light[4];
		g5Edep = final_Edep[4];
		g5t = final_t[4];
		g5x = final_x[4];
		g5y = final_y[4];
		g5z = final_z[4];
		g6light = final_light[5];
		g6Edep = final_Edep[5];
		g6t = final_t[5];
		g6x = final_x[5];
		g6y = final_y[5];
		g6z = final_z[5];
		g7light = final_light[6];
		g7Edep = final_Edep[6];
		g7t = final_t[6];
		g7x = final_x[6];
		g7y = final_y[6];
		g7z = final_z[6];
		g8light = final_light[7];
		g8Edep = final_Edep[7];
		g8t = final_t[7];
		g8x = final_x[7];
		g8y = final_y[7];
		g8z = final_z[7];
		g9light = final_light[8];
		g9Edep = final_Edep[8];
		g9t = final_t[8];
		g9x = final_x[8];
		g9y = final_y[8];
		g9z = final_z[8];
		g10light = final_light[9];
		g10Edep = final_Edep[9];
		g10t = final_t[9];
		g10x = final_x[9];
		g10y = final_y[9];
		g10z = final_z[9];
		g11light = final_light[10];
		g11Edep = final_Edep[10];
		g11t = final_t[10];
		g11x = final_x[10];
		g11y = final_y[10];
		g11z = final_z[10];
		g12light = final_light[11];
		g12Edep = final_Edep[11];
		g12t = final_t[11];
		g12x = final_x[11];
		g12y = final_y[11];
		g12z = final_z[11];
		g13light = final_light[12];
		g13Edep = final_Edep[12];
		g13t = final_t[12];
		g13x = final_x[12];
		g13y = final_y[12];
		g13z = final_z[12];
		g14light = final_light[13];
		g14Edep = final_Edep[13];
		g14t = final_t[13];
		g14x = final_x[13];
		g14y = final_y[13];
		g14z = final_z[13];
		g15light = final_light[14];
		g15Edep = final_Edep[14];
		g15t = final_t[14];
		g15x = final_x[14];
		g15y = final_y[14];
		g15z = final_z[14];
		g16light = final_light[15];
		g16Edep = final_Edep[15];
		g16t = final_t[15];
		g16x = final_x[15];
		g16y = final_y[15];
		g16z = final_z[15];
		g17light = final_light[16];
		g17Edep = final_Edep[16];
		g17t = final_t[16];
		g17x = final_x[16];
		g17y = final_y[16];
		g17z = final_z[16];
		g18light = final_light[17];
		g18Edep = final_Edep[17];
		g18t = final_t[17];
		g18x = final_x[17];
		g18y = final_y[17];
		g18z = final_z[17];
		g19light = final_light[18];
		g19Edep = final_Edep[18];
		g19t = final_t[18];
		g19x = final_x[18];
		g19y = final_y[18];
		g19z = final_z[18];
		g20light = final_light[19];
		g20Edep = final_Edep[19];
		g20t = final_t[19];
		g20x = final_x[19];
		g20y = final_y[19];
		g20z = final_z[19];*/

                tOut->Fill();
                }
            }

            tOut->Write();
            inFile->Close();
            tempFile->Close();
            temp2File->Close();
	    temp3File->Close();
	    temp4File->Close();
            outFile->Close();
        }// End 4n

 	else if (nneuts == 3)// Begin 3n
        {
            // Now we find the relevant branches to look at
	  Double_t b1p0x,b1p0tx,b1p0y,b1p0ty,b1p0Ekin,
		   b2p0x,b2p0tx,b2p0y,b2p0ty,b2p0t,b2p0z,b2p0TP,b2p0dE,b2p0Ekin,
		   b2p0R_pz,b2p0R_exen,b2p0R_exen2,b2p0R_exen3,b2p0R_ph,b2p0R_ph2,b2p0R_ph3,b2p0R_th,b2p0R_th2, b2p0R_th3,
		   b4p0x,b4p0tx,b4p0y,b4p0ty,b4p0t,b4p0Ekin,
		   b6p0x,b6p0y,b6p0tx,b6p0ty,b6p0Ekin,
		   b9p1t,b9p1x,b9p1y,b9p1z,b9p1Ekin,
		   b9p2t,b9p2x,b9p2y,b9p2z,b9p2Ekin,
		   b9p3t,b9p3x,b9p3y,b9p3z,b9p3Ekin;

            string bn_old[] = {"b1p0x", "b1p0tx", "b1p0y", "b1p0ty", "b1p0Ekin",
                "b2p0x", "b2p0tx", "b2p0y", "b2p0ty", "b2p0t", "b2p0z", "b2p0TP", "b2p0dE", "b2p0Ekin",
		"b2p0R_pz","b2p0R_exen", "b2p0R_exen2", "b2p0R_exen3", "b2p0R_ph", "b2p0R_ph2", "b2p0R_ph3", "b2p0R_th", "b2p0R_th2", "b2p0R_th3",
                "b4p0x", "b4p0tx", "b4p0y", "b4p0ty", "b4p0t", "b4p0Ekin",
                "b6p0x", "b6p0tx", "b6p0y", "b6p0ty", "b6p0Ekin",
                "b9p1t", "b9p1x", "b9p1y", "b9p1z", "b9p1Ekin",
                "b9p2t", "b9p2x", "b9p2y", "b9p2z", "b9p2Ekin",
		"b9p3t", "b9p3x", "b9p3y", "b9p3z", "b9p3Ekin"
                };
        
            double* ps_old[] = {&b1p0x,&b1p0tx,&b1p0y,&b1p0ty,&b1p0Ekin,
                &b2p0x,&b2p0tx,&b2p0y,&b2p0ty,&b2p0t,&b2p0z,&b2p0TP,&b2p0dE,
		&b2p0Ekin,&b2p0R_pz,&b2p0R_exen,&b2p0R_exen2,&b2p0R_exen3, &b2p0R_ph,&b2p0R_ph2, &b2p0R_ph3, &b2p0R_th, &b2p0R_th2, &b2p0R_th3,
                &b4p0x,&b4p0tx,&b4p0y,&b4p0ty,&b4p0t,&b4p0Ekin,
                &b6p0x,&b6p0tx,&b6p0y,&b6p0ty,&b6p0Ekin,
                &b9p1t,&b9p1x,&b9p1y,&b9p1z,&b9p1Ekin,
                &b9p2t,&b9p2x,&b9p2y,&b9p2z,&b9p2Ekin,
		&b9p3t,&b9p3x,&b9p3y,&b9p3z,&b9p3Ekin
                };   

            for (size_t i(0); i<sizeof(ps_old)/sizeof(double*); i++) {
                t->SetBranchAddress(bn_old[i].c_str(),ps_old[i]);  
            }

            // Get number of entries in the file
            Int_t input_nevent = t->GetEntries();

            // Then open the GEANT output for the 1st neutron root file
	    TString n1name = dirdata+"/neutron1"+uniqueID+".root";
		cout << "Found 1st file: " << n1name << endl;
            TFile *tempFile = new TFile(n1name); assert(tempFile);
            TTree *tn1Temp = (TTree*)tempFile->Get("t"); assert(tn1Temp);

            Double_t mh1n1Edep,mh1n1light,mh1n1t,mh1n1x,mh1n1y,mh1n1z,
		     mh1n2Edep,mh1n2light,mh1n2t,mh1n2x,mh1n2y,mh1n2z,
		     mh1n3Edep,mh1n3light,mh1n3t,mh1n3x,mh1n3y,mh1n3z,
		     mh1n4Edep,mh1n4light,mh1n4t,mh1n4x,mh1n4y,mh1n4z,
		     mh1n5Edep,mh1n5light,mh1n5t,mh1n5x,mh1n5y,mh1n5z,
		     mh1n6Edep,mh1n6light,mh1n6t,mh1n6x,mh1n6y,mh1n6z,
		     mh1n7Edep,mh1n7light,mh1n7t,mh1n7x,mh1n7y,mh1n7z,
		     mh1n8Edep,mh1n8light,mh1n8t,mh1n8x,mh1n8y,mh1n8z,
	 	     mh1n9Edep,mh1n9light,mh1n9t,mh1n9x,mh1n9y,mh1n9z,
		     mh1n10Edep,mh1n10light,mh1n10t,mh1n10x,mh1n10y,mh1n10z;

            Int_t mh1nmultiplicity;
            Int_t mh1_z1; //zwk particle multiplicities
            Int_t mh1_z2;
            Int_t mh1_zgt2;
            Int_t mh1_g;

            string bn_n1[] = {"MoNA.Hit.1.e","MoNA.Hit.1.light","MoNA.Hit.1.t","MoNA.Hit.1.x","MoNA.Hit.1.y","MoNA.Hit.1.z",
                    "MoNA.Hit.2.e","MoNA.Hit.2.light","MoNA.Hit.2.t","MoNA.Hit.2.x","MoNA.Hit.2.y","MoNA.Hit.2.z",
                "MoNA.Hit.3.e","MoNA.Hit.3.light","MoNA.Hit.3.t","MoNA.Hit.3.x","MoNA.Hit.3.y","MoNA.Hit.3.z",
                "MoNA.Hit.4.e","MoNA.Hit.4.light","MoNA.Hit.4.t","MoNA.Hit.4.x","MoNA.Hit.4.y","MoNA.Hit.4.z",
                "MoNA.Hit.5.e","MoNA.Hit.5.light","MoNA.Hit.5.t","MoNA.Hit.5.x","MoNA.Hit.5.y","MoNA.Hit.5.z",
                "MoNA.Hit.6.e","MoNA.Hit.6.light","MoNA.Hit.6.t","MoNA.Hit.6.x","MoNA.Hit.6.y","MoNA.Hit.6.z",
                "MoNA.Hit.7.e","MoNA.Hit.7.light","MoNA.Hit.7.t","MoNA.Hit.7.x","MoNA.Hit.7.y","MoNA.Hit.7.z",
                "MoNA.Hit.8.e","MoNA.Hit.8.light","MoNA.Hit.8.t","MoNA.Hit.8.x","MoNA.Hit.8.y","MoNA.Hit.8.z",
                "MoNA.Hit.9.e","MoNA.Hit.9.light","MoNA.Hit.9.t","MoNA.Hit.9.x","MoNA.Hit.9.y","MoNA.Hit.9.z",
                "MoNA.Hit.10.e","MoNA.Hit.10.light","MoNA.Hit.10.t","MoNA.Hit.10.x","MoNA.Hit.10.y","MoNA.Hit.10.z",
                };

            double* ps_n1[] = {&mh1n1Edep,&mh1n1light,&mh1n1t,&mh1n1x,&mh1n1y,&mh1n1z,
                	       &mh1n2Edep,&mh1n2light,&mh1n2t,&mh1n2x,&mh1n2y,&mh1n2z,
                               &mh1n3Edep,&mh1n3light,&mh1n3t,&mh1n3x,&mh1n3y,&mh1n3z,
                               &mh1n4Edep,&mh1n4light,&mh1n4t,&mh1n4x,&mh1n4y,&mh1n4z,
                               &mh1n5Edep,&mh1n5light,&mh1n5t,&mh1n5x,&mh1n5y,&mh1n5z,
                               &mh1n6Edep,&mh1n6light,&mh1n6t,&mh1n6x,&mh1n6y,&mh1n6z,
                               &mh1n7Edep,&mh1n7light,&mh1n7t,&mh1n7x,&mh1n7y,&mh1n7z,
                               &mh1n8Edep,&mh1n8light,&mh1n8t,&mh1n8x,&mh1n8y,&mh1n8z,
                               &mh1n9Edep,&mh1n9light,&mh1n9t,&mh1n9x,&mh1n9y,&mh1n9z,
                               &mh1n10Edep,&mh1n10light,&mh1n10t,&mh1n10x,&mh1n10y,&mh1n10z,
                };

            for (size_t i(0); i<sizeof(ps_n1)/sizeof(double*); i++) {
                tn1Temp->SetBranchAddress(bn_n1[i].c_str(),ps_n1[i]);  
            }

            tn1Temp->SetBranchAddress("MoNA.multiplicity",&mh1nmultiplicity);  
            tn1Temp->SetBranchAddress("MoNA.Z1mult",&mh1_z1);  
            tn1Temp->SetBranchAddress("MoNA.Z2mult",&mh1_z2);  
            tn1Temp->SetBranchAddress("MoNA.ZGt2mult",&mh1_zgt2);  
            tn1Temp->SetBranchAddress("MoNA.Gmult",&mh1_g);  

            // Get number of entries in the file
            Int_t outputn1_nevents = tn1Temp->GetEntries();

            // Then open the GEANT output for the 2nd neutron root file
	    TString n2name = dirdata+"/neutron2"+uniqueID+".root";
		cout << "Found 2nd File: " << n2name << endl;
            TFile *temp2File = new TFile(n2name); assert(temp2File);
            TTree *tn2Temp = (TTree*)temp2File->Get("t"); assert(tn2Temp);	

            Double_t mh2n1Edep,mh2n1light,mh2n1t,mh2n1x,mh2n1y,mh2n1z,
		     mh2n2Edep,mh2n2light,mh2n2t,mh2n2x,mh2n2y,mh2n2z,
		     mh2n3Edep,mh2n3light,mh2n3t,mh2n3x,mh2n3y,mh2n3z,
		     mh2n4Edep,mh2n4light,mh2n4t,mh2n4x,mh2n4y,mh2n4z,
	             mh2n5Edep,mh2n5light,mh2n5t,mh2n5x,mh2n5y,mh2n5z,
                     mh2n6Edep,mh2n6light,mh2n6t,mh2n6x,mh2n6y,mh2n6z,
                     mh2n7Edep,mh2n7light,mh2n7t,mh2n7x,mh2n7y,mh2n7z,
                     mh2n8Edep,mh2n8light,mh2n8t,mh2n8x,mh2n8y,mh2n8z,
                     mh2n9Edep,mh2n9light,mh2n9t,mh2n9x,mh2n9y,mh2n9z,
                     mh2n10Edep,mh2n10light,mh2n10t,mh2n10x,mh2n10y,mh2n10z;

            Int_t mh2nmultiplicity;
            Int_t mh2_z1; //zwk particle multiplicities
            Int_t mh2_z2;
            Int_t mh2_zgt2;
            Int_t mh2_g;

            string bn_n2[] = {"MoNA.Hit.1.e","MoNA.Hit.1.light","MoNA.Hit.1.t","MoNA.Hit.1.x","MoNA.Hit.1.y","MoNA.Hit.1.z",
                "MoNA.Hit.2.e","MoNA.Hit.2.light","MoNA.Hit.2.t","MoNA.Hit.2.x","MoNA.Hit.2.y","MoNA.Hit.2.z",
                "MoNA.Hit.3.e","MoNA.Hit.3.light","MoNA.Hit.3.t","MoNA.Hit.3.x","MoNA.Hit.3.y","MoNA.Hit.3.z",
                "MoNA.Hit.4.e","MoNA.Hit.4.light","MoNA.Hit.4.t","MoNA.Hit.4.x","MoNA.Hit.4.y","MoNA.Hit.4.z",
                "MoNA.Hit.5.e","MoNA.Hit.5.light","MoNA.Hit.5.t","MoNA.Hit.5.x","MoNA.Hit.5.y","MoNA.Hit.5.z",
                "MoNA.Hit.6.e","MoNA.Hit.6.light","MoNA.Hit.6.t","MoNA.Hit.6.x","MoNA.Hit.6.y","MoNA.Hit.6.z",
                "MoNA.Hit.7.e","MoNA.Hit.7.light","MoNA.Hit.7.t","MoNA.Hit.7.x","MoNA.Hit.7.y","MoNA.Hit.7.z",
                "MoNA.Hit.8.e","MoNA.Hit.8.light","MoNA.Hit.8.t","MoNA.Hit.8.x","MoNA.Hit.8.y","MoNA.Hit.8.z",
                "MoNA.Hit.9.e","MoNA.Hit.9.light","MoNA.Hit.9.t","MoNA.Hit.9.x","MoNA.Hit.9.y","MoNA.Hit.9.z",
                "MoNA.Hit.10.e","MoNA.Hit.10.light","MoNA.Hit.10.t","MoNA.Hit.10.x","MoNA.Hit.10.y","MoNA.Hit.10.z",
                };

            double* ps_n2[] = {&mh2n1Edep,&mh2n1light,&mh2n1t,&mh2n1x,&mh2n1y,&mh2n1z,
                               &mh2n2Edep,&mh2n2light,&mh2n2t,&mh2n2x,&mh2n2y,&mh2n2z,
                               &mh2n3Edep,&mh2n3light,&mh2n3t,&mh2n3x,&mh2n3y,&mh2n3z,
                               &mh2n4Edep,&mh2n4light,&mh2n4t,&mh2n4x,&mh2n4y,&mh2n4z,
                               &mh2n5Edep,&mh2n5light,&mh2n5t,&mh2n5x,&mh2n5y,&mh2n5z,
                               &mh2n6Edep,&mh2n6light,&mh2n6t,&mh2n6x,&mh2n6y,&mh2n6z,
                               &mh2n7Edep,&mh2n7light,&mh2n7t,&mh2n7x,&mh2n7y,&mh2n7z,
                               &mh2n8Edep,&mh2n8light,&mh2n8t,&mh2n8x,&mh2n8y,&mh2n8z,
                               &mh2n9Edep,&mh2n9light,&mh2n9t,&mh2n9x,&mh2n9y,&mh2n9z,
                               &mh2n10Edep,&mh2n10light,&mh2n10t,&mh2n10x,&mh2n10y,&mh2n10z,
                };

            for (size_t i(0); i<sizeof(ps_n2)/sizeof(double*); i++) {
                tn2Temp->SetBranchAddress(bn_n2[i].c_str(),ps_n2[i]);  
            }

            tn2Temp->SetBranchAddress("MoNA.multiplicity",&mh2nmultiplicity); 
            tn2Temp->SetBranchAddress("MoNA.Z1mult",&mh2_z1);  
            tn2Temp->SetBranchAddress("MoNA.Z2mult",&mh2_z2);  
            tn2Temp->SetBranchAddress("MoNA.ZGt2mult",&mh2_zgt2);  
            tn2Temp->SetBranchAddress("MoNA.Gmult",&mh2_g);  

            // Get number of entries in the file
            Int_t outputn2_nevents = tn2Temp->GetEntries();


	// Then open the GEANT output for the 3rd neutron root file
	    TString n3name = dirdata+"/neutron3"+uniqueID+".root";
		cout << "Found 3rd file: " << n3name << endl;
            TFile *temp3File = new TFile(n3name); assert(temp3File);
            TTree *tn3Temp = (TTree*)temp3File->Get("t"); assert(tn3Temp);

            Double_t mh3n1Edep,mh3n1light,mh3n1t,mh3n1x,mh3n1y,mh3n1z,
		     mh3n2Edep,mh3n2light,mh3n2t,mh3n2x,mh3n2y,mh3n2z,
		     mh3n3Edep,mh3n3light,mh3n3t,mh3n3x,mh3n3y,mh3n3z,
		     mh3n4Edep,mh3n4light,mh3n4t,mh3n4x,mh3n4y,mh3n4z,
		     mh3n5Edep,mh3n5light,mh3n5t,mh3n5x,mh3n5y,mh3n5z,
		     mh3n6Edep,mh3n6light,mh3n6t,mh3n6x,mh3n6y,mh3n6z,
		     mh3n7Edep,mh3n7light,mh3n7t,mh3n7x,mh3n7y,mh3n7z,
	             mh3n8Edep,mh3n8light,mh3n8t,mh3n8x,mh3n8y,mh3n8z,
		     mh3n9Edep,mh3n9light,mh3n9t,mh3n9x,mh3n9y,mh3n9z,
	      	     mh3n10Edep,mh3n10light,mh3n10t,mh3n10x,mh3n10y,mh3n10z;

            Int_t mh3nmultiplicity;
            Int_t mh3_z1; //zwk particle multiplicities
            Int_t mh3_z2;
            Int_t mh3_zgt2;
            Int_t mh3_g;

            string bn_n3[] = {"MoNA.Hit.1.e","MoNA.Hit.1.light","MoNA.Hit.1.t","MoNA.Hit.1.x","MoNA.Hit.1.y","MoNA.Hit.1.z",
                "MoNA.Hit.2.e","MoNA.Hit.2.light","MoNA.Hit.2.t","MoNA.Hit.2.x","MoNA.Hit.2.y","MoNA.Hit.2.z",
                "MoNA.Hit.3.e","MoNA.Hit.3.light","MoNA.Hit.3.t","MoNA.Hit.3.x","MoNA.Hit.3.y","MoNA.Hit.3.z",
                "MoNA.Hit.4.e","MoNA.Hit.4.light","MoNA.Hit.4.t","MoNA.Hit.4.x","MoNA.Hit.4.y","MoNA.Hit.4.z",
                "MoNA.Hit.5.e","MoNA.Hit.5.light","MoNA.Hit.5.t","MoNA.Hit.5.x","MoNA.Hit.5.y","MoNA.Hit.5.z",
                "MoNA.Hit.6.e","MoNA.Hit.6.light","MoNA.Hit.6.t","MoNA.Hit.6.x","MoNA.Hit.6.y","MoNA.Hit.6.z",
                "MoNA.Hit.7.e","MoNA.Hit.7.light","MoNA.Hit.7.t","MoNA.Hit.7.x","MoNA.Hit.7.y","MoNA.Hit.7.z",
                "MoNA.Hit.8.e","MoNA.Hit.8.light","MoNA.Hit.8.t","MoNA.Hit.8.x","MoNA.Hit.8.y","MoNA.Hit.8.z",
                "MoNA.Hit.9.e","MoNA.Hit.9.light","MoNA.Hit.9.t","MoNA.Hit.9.x","MoNA.Hit.9.y","MoNA.Hit.9.z",
                "MoNA.Hit.10.e","MoNA.Hit.10.light","MoNA.Hit.10.t","MoNA.Hit.10.x","MoNA.Hit.10.y","MoNA.Hit.10.z",
                };

            double* ps_n3[] = {&mh3n1Edep,&mh3n1light,&mh3n1t,&mh3n1x,&mh3n1y,&mh3n1z,
               		       &mh3n2Edep,&mh3n2light,&mh3n2t,&mh3n2x,&mh3n2y,&mh3n2z,
               		       &mh3n3Edep,&mh3n3light,&mh3n3t,&mh3n3x,&mh3n3y,&mh3n3z,
                	       &mh3n4Edep,&mh3n4light,&mh3n4t,&mh3n4x,&mh3n4y,&mh3n4z,
               		       &mh3n5Edep,&mh3n5light,&mh3n5t,&mh3n5x,&mh3n5y,&mh3n5z,
                               &mh3n6Edep,&mh3n6light,&mh3n6t,&mh3n6x,&mh3n6y,&mh3n6z,
                               &mh3n7Edep,&mh3n7light,&mh3n7t,&mh3n7x,&mh3n7y,&mh3n7z,
                               &mh3n8Edep,&mh3n8light,&mh3n8t,&mh3n8x,&mh3n8y,&mh3n8z,
                               &mh3n9Edep,&mh3n9light,&mh3n9t,&mh3n9x,&mh3n9y,&mh3n9z,
                               &mh3n10Edep,&mh3n10light,&mh3n10t,&mh3n10x,&mh3n10y,&mh3n10z,
                };

            for (size_t i(0); i<sizeof(ps_n3)/sizeof(double*); i++) {
                tn3Temp->SetBranchAddress(bn_n3[i].c_str(),ps_n3[i]);  
            }

            tn3Temp->SetBranchAddress("MoNA.multiplicity",&mh3nmultiplicity); 
            tn3Temp->SetBranchAddress("MoNA.Z1mult",&mh3_z1);  
            tn3Temp->SetBranchAddress("MoNA.Z2mult",&mh3_z2);  
            tn3Temp->SetBranchAddress("MoNA.ZGt2mult",&mh3_zgt2);  
            tn3Temp->SetBranchAddress("MoNA.Gmult",&mh3_g);  

            // Get number of entries in the file
            Int_t outputn3_nevents = tn3Temp->GetEntries();


            // Finally open the output .root file
            TFile *outFile = new TFile(argv[2],"recreate"); assert(outFile);
            TTree *tOut = new TTree("t","Simulation Output"); assert(tOut);

            // We want to preserve the original st-mona parameters
            TBranch *obb1p0x = tOut->Branch("b1p0x",&b1p0x,"b1p0x/D");
            TBranch *obb1p0tx = tOut->Branch("b1p0tx",&b1p0tx,"b1p0tx/D");
            TBranch *obb1p0y = tOut->Branch("b1p0y",&b1p0y,"b1p0y/D");
            TBranch *obb1p0ty = tOut->Branch("b1p0ty",&b1p0ty,"b1p0ty/D");
            TBranch *obb1p0Ekin = tOut->Branch("b1p0Ekin",&b1p0Ekin,"b1p0Ekin/D");
            TBranch *obb2p0x = tOut->Branch("b2p0x",&b2p0x,"b2p0x/D");
            TBranch *obb2p0tx = tOut->Branch("b2p0tx",&b2p0tx,"b2p0tx/D");
            TBranch *obb2p0y = tOut->Branch("b2p0y",&b2p0y,"b2p0y/D");
            TBranch *obb2p0ty = tOut->Branch("b2p0ty",&b2p0ty,"b2p0ty/D");
            TBranch *obb2p0t = tOut->Branch("b2p0t",&b2p0t,"b2p0t/D");
            TBranch *obb2p0z = tOut->Branch("b2p0z",&b2p0z,"b2p0z/D");
            TBranch *obb2p0TP = tOut->Branch("b2p0TP",&b2p0TP,"b2p0TP/D");
            TBranch *obb2p0dE = tOut->Branch("b2p0dE",&b2p0dE,"b2p0dE/D");
            TBranch *obb2p0Ekin = tOut->Branch("b2p0Ekin",&b2p0Ekin,"b2p0Ekin/D");
            TBranch *obb2p0R_pz = tOut->Branch("b2p0R_pz",&b2p0R_pz,"b2p0R_pz/D");
            TBranch *obb2p0R_exen = tOut->Branch("b2p0R_exen",&b2p0R_exen,"b2p0R_exen/D");
            TBranch *obb2p0R_exen2 = tOut->Branch("b2p0R_exen2",&b2p0R_exen2,"b2p0R_exen2/D");
	    TBranch *obb2p0R_exen3 = tOut->Branch("b2p0R_exen3",&b2p0R_exen3,"b2p0R_exen3/D");

            TBranch *obb2p0R_ph = tOut->Branch("b2p0R_ph",&b2p0R_ph,"b2p0R_ph/D");
            TBranch *obb2p0R_ph2 = tOut->Branch("b2p0R_ph2",&b2p0R_ph2,"b2p0R_ph2/D");
            TBranch *obb2p0R_ph3 = tOut->Branch("b2p0R_ph3",&b2p0R_ph3,"b2p0R_ph3/D");

            TBranch *obb2p0R_th = tOut->Branch("b2p0R_th",&b2p0R_th,"b2p0R_th/D");
            TBranch *obb2p0R_th2 = tOut->Branch("b2p0R_th2",&b2p0R_th2,"b2p0R_th2/D");
            TBranch *obb2p0R_th3 = tOut->Branch("b2p0R_th3",&b2p0R_th3,"b2p0R_th3/D");

            TBranch *obb7p0x = tOut->Branch("b7p0x",&b4p0x,"b7p0x/D");
            TBranch *obb7p0tx = tOut->Branch("b7p0tx",&b4p0tx,"b7p0tx/D");
            TBranch *obb7p0y = tOut->Branch("b7p0y",&b4p0y,"b7p0y/D");
            TBranch *obb7p0ty = tOut->Branch("b7p0ty",&b4p0ty,"b7p0ty/D");
            TBranch *obb7p0t = tOut->Branch("b7p0t",&b4p0t,"b7p0t/D");
            TBranch *obb7p0Ekin = tOut->Branch("b7p0Ekin",&b4p0Ekin,"b7p0Ekin/D");
            TBranch *obb9p0x = tOut->Branch("b9p0x",&b6p0x,"b9p0x/D");
            TBranch *obb9p0tx = tOut->Branch("b9p0tx",&b6p0tx,"b9p0tx/D");
            TBranch *obb9p0y = tOut->Branch("b9p0y",&b6p0y,"b9p0y/D");
            TBranch *obb9p0ty = tOut->Branch("b9p0ty",&b6p0ty,"b9p0ty/D");
            TBranch *obb9p0Ekin = tOut->Branch("b9p0Ekin",&b6p0Ekin,"b9p0Ekin/D");
            TBranch *obb13p1t = tOut->Branch("b13p1t",&b9p1t,"b13p1t/D");
            TBranch *obb13p1x = tOut->Branch("b13p1x",&b9p1x,"b13p1x/D");
            TBranch *obb13p1y = tOut->Branch("b13p1y",&b9p1y,"b13p1y/D");
            TBranch *obb13p1z = tOut->Branch("b13p1z",&b9p1z,"b13p1z/D");
            TBranch *obb13p1Ekin = tOut->Branch("b13p1Ekin",&b9p1Ekin,"b13p1Ekin/D");
            TBranch *obb13p2t = tOut->Branch("b13p2t",&b9p2t,"b13p2t/D");
            TBranch *obb13p2x = tOut->Branch("b13p2x",&b9p2x,"b13p2x/D");
            TBranch *obb13p2y = tOut->Branch("b13p2y",&b9p2y,"b13p2y/D");
            TBranch *obb13p2z = tOut->Branch("b13p2z",&b9p2z,"b13p2z/D");
            TBranch *obb13p2Ekin = tOut->Branch("b13p2Ekin",&b9p2Ekin,"b13p2Ekin/D");
            TBranch *obb13p3t = tOut->Branch("b13p3t",&b9p3t,"b13p3t/D");
            TBranch *obb13p3x = tOut->Branch("b13p3x",&b9p3x,"b13p3x/D");
            TBranch *obb13p3y = tOut->Branch("b13p3y",&b9p3y,"b13p3y/D");
            TBranch *obb13p3z = tOut->Branch("b13p3z",&b9p3z,"b13p3z/D");
            TBranch *obb13p3Ekin = tOut->Branch("b13p3Ekin",&b9p3Ekin,"b13p3Ekin/D");


            // And add in the new GEANT parameters... note that they are simply pg instead of p
	    // 1st Neutron
            tOut->Branch("b13pn1gmultiplicity",&mh1nmultiplicity,"b13pn1gmultiplicity/I");
            tOut->Branch("b13pn1gZ1Mult",&mh1_z1,"b13pn1gZ1Mult/I");//zwk particle mults
            tOut->Branch("b13pn1gZ2Mult",&mh1_z2,"b13pn1gZ2Mult/I");
            tOut->Branch("b13pn1gZGt2Mult",&mh1_zgt2,"b13pn1gZGt2Mult/I");
            tOut->Branch("b13pn1gGammaMult",&mh1_g,"b13pn1gGammaMult/I");

            tOut->Branch("b13pn1g1Edep",&mh1n1Edep,"b13pn1g1Edep/D");
            tOut->Branch("b13pn1g1light",&mh1n1light,"b13pn1g1light/D");
            tOut->Branch("b13pn1g1t",&mh1n1t,"b13pn1g1t/D");
            tOut->Branch("b13pn1g1x",&mh1n1x,"b13pn1g1x/D");
            tOut->Branch("b13pn1g1y",&mh1n1y,"b13pn1g1y/D");
            tOut->Branch("b13pn1g1z",&mh1n1z,"b13pn1g1z/D");
            tOut->Branch("b13pn1g2Edep",&mh1n2Edep,"b13pn1g2Edep/D");
            tOut->Branch("b13pn1g2light",&mh1n2light,"b13pn1g2light/D");
            tOut->Branch("b13pn1g2t",&mh1n2t,"b13pn1g2t/D");
            tOut->Branch("b13pn1g2x",&mh1n2x,"b13pn1g2x/D");
            tOut->Branch("b13pn1g2y",&mh1n2y,"b13pn1g2y/D");
            tOut->Branch("b13pn1g2z",&mh1n2z,"b13pn1g2z/D");
            tOut->Branch("b13pn1g3Edep",&mh1n3Edep,"b13pn1g3Edep/D");
            tOut->Branch("b13pn1g3light",&mh1n3light,"b13pn1g3light/D");
            tOut->Branch("b13pn1g3t",&mh1n3t,"b13pn1g3t/D");
            tOut->Branch("b13pn1g3x",&mh1n3x,"b13pn1g3x/D");
            tOut->Branch("b13pn1g3y",&mh1n3y,"b13pn1g3y/D");
            tOut->Branch("b13pn1g3z",&mh1n3z,"b13pn1g3z/D");
            tOut->Branch("b13pn1g4Edep",&mh1n4Edep,"b13pn1g4Edep/D");
            tOut->Branch("b13pn1g4light",&mh1n4light,"b13pn1g4light/D");
            tOut->Branch("b13pn1g4t",&mh1n4t,"b13pn1g4t/D");
            tOut->Branch("b13pn1g4x",&mh1n4x,"b13pn1g4x/D");
            tOut->Branch("b13pn1g4y",&mh1n4y,"b13pn1g4y/D");
            tOut->Branch("b13pn1g4z",&mh1n4z,"b13pn1g4z/D");
            tOut->Branch("b13pn1g5Edep",&mh1n5Edep,"b13pn1g5Edep/D");
            tOut->Branch("b13pn1g5light",&mh1n5light,"b13pn1g5light/D");
            tOut->Branch("b13pn1g5t",&mh1n5t,"b13pn1g5t/D");
            tOut->Branch("b13pn1g5x",&mh1n5x,"b13pn1g5x/D");
            tOut->Branch("b13pn1g5y",&mh1n5y,"b13pn1g5y/D");
            tOut->Branch("b13pn1g5z",&mh1n5z,"b13pn1g5z/D");
            tOut->Branch("b13pn1g6Edep",&mh1n6Edep,"b13pn1g6Edep/D");
            tOut->Branch("b13pn1g6light",&mh1n6light,"b13pn1g6light/D");
            tOut->Branch("b13pn1g6t",&mh1n6t,"b13pn1g6t/D");
            tOut->Branch("b13pn1g6x",&mh1n6x,"b13pn1g6x/D");
            tOut->Branch("b13pn1g6y",&mh1n6y,"b13pn1g6y/D");
            tOut->Branch("b13pn1g6z",&mh1n6z,"b13pn1g6z/D");
            tOut->Branch("b13pn1g7Edep",&mh1n7Edep,"b13pn1g7Edep/D");
            tOut->Branch("b13pn1g7light",&mh1n7light,"b13pn1g7light/D");
            tOut->Branch("b13pn1g7t",&mh1n7t,"b13pn1g7t/D");
            tOut->Branch("b13pn1g7x",&mh1n7x,"b13pn1g7x/D");
            tOut->Branch("b13pn1g7y",&mh1n7y,"b13pn1g7y/D");
            tOut->Branch("b13pn1g7z",&mh1n7z,"b13pn1g7z/D");
            tOut->Branch("b13pn1g8Edep",&mh1n8Edep,"b13pn1g8Edep/D");
            tOut->Branch("b13pn1g8light",&mh1n8light,"b13pn1g8light/D");
            tOut->Branch("b13pn1g8t",&mh1n8t,"b13pn1g8t/D");
            tOut->Branch("b13pn1g8x",&mh1n8x,"b13pn1g8x/D");
            tOut->Branch("b13pn1g8y",&mh1n8y,"b13pn1g8y/D");
            tOut->Branch("b13pn1g8z",&mh1n8z,"b13pn1g8z/D");
            tOut->Branch("b13pn1g9Edep",&mh1n9Edep,"b13pn1g9Edep/D");
            tOut->Branch("b13pn1g9light",&mh1n9light,"b13pn1g9light/D");
            tOut->Branch("b13pn1g9t",&mh1n9t,"b13pn1g9t/D");
            tOut->Branch("b13pn1g9x",&mh1n9x,"b13pn1g9x/D");
            tOut->Branch("b13pn1g9y",&mh1n9y,"b13pn1g9y/D");
            tOut->Branch("b13pn1g9z",&mh1n9z,"b13pn1g9z/D");
            tOut->Branch("b13pn1g10Edep",&mh1n10Edep,"b13pn1g10Edep/D");
            tOut->Branch("b13pn1g10light",&mh1n10light,"b13pn1g10light/D");
            tOut->Branch("b13pn1g10t",&mh1n10t,"b13pn1g10t/D");
            tOut->Branch("b13pn1g10x",&mh1n10x,"b13pn1g10x/D");
            tOut->Branch("b13pn1g10y",&mh1n10y,"b13pn1g10y/D");
            tOut->Branch("b13pn1g10z",&mh1n10z,"b13pn1g10z/D");

            // And add in the new GEANT parameters... note that they are simply pg instead of p
	    // 2nd Neutron
            tOut->Branch("b13pn2gmultiplicity",&mh2nmultiplicity,"b13pn2gmultiplicity/I");
		//(All four had b13pn1gZ%Mult/I)
            tOut->Branch("b13pn2gZ1Mult",&mh2_z1,"b13pn2gZ1Mult/I");//zwk particle mults
            tOut->Branch("b13pn2gZ2Mult",&mh2_z2,"b13pn2gZ2Mult/I");
            tOut->Branch("b13pn2gZGt2Mult",&mh2_zgt2,"b13pn2gZGt2Mult/I");// Is this a mistake? MDJ
            tOut->Branch("b13pn2gGammaMult",&mh2_g,"b13pn2gGammaMult/I");
		//
            tOut->Branch("b13pn2g1Edep",&mh2n1Edep,"b13pn2g1Edep/D");
            tOut->Branch("b13pn2g1light",&mh2n1light,"b13pn2g1light/D");
            tOut->Branch("b13pn2g1t",&mh2n1t,"b13pn2g1t/D");
            tOut->Branch("b13pn2g1x",&mh2n1x,"b13pn2g1x/D");
            tOut->Branch("b13pn2g1y",&mh2n1y,"b13pn2g1y/D");
            tOut->Branch("b13pn2g1z",&mh2n1z,"b13pn2g1z/D");
            tOut->Branch("b13pn2g2Edep",&mh2n2Edep,"b13pn2g2Edep/D");
            tOut->Branch("b13pn2g2light",&mh2n2light,"b13pn2g2light/D");
            tOut->Branch("b13pn2g2t",&mh2n2t,"b13pn2g2t/D");
            tOut->Branch("b13pn2g2x",&mh2n2x,"b13pn2g2x/D");
            tOut->Branch("b13pn2g2y",&mh2n2y,"b13pn2g2y/D");
            tOut->Branch("b13pn2g2z",&mh2n2z,"b13pn2g2z/D");
            tOut->Branch("b13pn2g3Edep",&mh2n3Edep,"b13pn2g3Edep/D");
            tOut->Branch("b13pn2g3light",&mh2n3light,"b13pn2g3light/D");
            tOut->Branch("b13pn2g3t",&mh2n3t,"b13pn2g3t/D");
            tOut->Branch("b13pn2g3x",&mh2n3x,"b13pn2g3x/D");
            tOut->Branch("b13pn2g3y",&mh2n3y,"b13pn2g3y/D");
            tOut->Branch("b13pn2g3z",&mh2n3z,"b13pn2g3z/D");
            tOut->Branch("b13pn2g4Edep",&mh2n4Edep,"b13pn2g4Edep/D");
            tOut->Branch("b13pn2g4light",&mh2n4light,"b13pn2g4light/D");
            tOut->Branch("b13pn2g4t",&mh2n4t,"b13pn2g4t/D");
            tOut->Branch("b13pn2g4x",&mh2n4x,"b13pn2g4x/D");
            tOut->Branch("b13pn2g4y",&mh2n4y,"b13pn2g4y/D");
            tOut->Branch("b13pn2g4z",&mh2n4z,"b13pn2g4z/D");
            tOut->Branch("b13pn2g5Edep",&mh2n5Edep,"b13pn2g5Edep/D");
            tOut->Branch("b13pn2g5light",&mh2n5light,"b13pn2g5light/D");
            tOut->Branch("b13pn2g5t",&mh2n5t,"b13pn2g5t/D");
            tOut->Branch("b13pn2g5x",&mh2n5x,"b13pn2g5x/D");
            tOut->Branch("b13pn2g5y",&mh2n5y,"b13pn2g5y/D");
            tOut->Branch("b13pn2g5z",&mh2n5z,"b13pn2g5z/D");
            tOut->Branch("b13pn2g6Edep",&mh2n6Edep,"b13pn2g6Edep/D");
            tOut->Branch("b13pn2g6light",&mh2n6light,"b13pn2g6light/D");
            tOut->Branch("b13pn2g6t",&mh2n6t,"b13pn2g6t/D");
            tOut->Branch("b13pn2g6x",&mh2n6x,"b13pn2g6x/D");
            tOut->Branch("b13pn2g6y",&mh2n6y,"b13pn2g6y/D");
            tOut->Branch("b13pn2g6z",&mh2n6z,"b13pn2g6z/D");
            tOut->Branch("b13pn2g7Edep",&mh2n7Edep,"b13pn2g7Edep/D");
            tOut->Branch("b13pn2g7light",&mh2n7light,"b13pn2g7light/D");
            tOut->Branch("b13pn2g7t",&mh2n7t,"b13pn2g7t/D");
            tOut->Branch("b13pn2g7x",&mh2n7x,"b13pn2g7x/D");
            tOut->Branch("b13pn2g7y",&mh2n7y,"b13pn2g7y/D");
            tOut->Branch("b13pn2g7z",&mh2n7z,"b13pn2g7z/D");
            tOut->Branch("b13pn2g8Edep",&mh2n8Edep,"b13pn2g8Edep/D");
            tOut->Branch("b13pn2g8light",&mh2n8light,"b13pn2g8light/D");
            tOut->Branch("b13pn2g8t",&mh2n8t,"b13pn2g8t/D");
            tOut->Branch("b13pn2g8x",&mh2n8x,"b13pn2g8x/D");
            tOut->Branch("b13pn2g8y",&mh2n8y,"b13pn2g8y/D");
            tOut->Branch("b13pn2g8z",&mh2n8z,"b13pn2g8z/D");
            tOut->Branch("b13pn2g9Edep",&mh2n9Edep,"b13pn2g9Edep/D");
            tOut->Branch("b13pn2g9light",&mh2n9light,"b13pn2g9light/D");
            tOut->Branch("b13pn2g9t",&mh2n9t,"b13pn2g9t/D");
            tOut->Branch("b13pn2g9x",&mh2n9x,"b13pn2g9x/D");
            tOut->Branch("b13pn2g9y",&mh2n9y,"b13pn2g9y/D");
            tOut->Branch("b13pn2g9z",&mh2n9z,"b13pn2g9z/D");
            tOut->Branch("b13pn2g10Edep",&mh2n10Edep,"b13pn2g10Edep/D");
            tOut->Branch("b13pn2g10light",&mh2n10light,"b13pn2g10light/D");
            tOut->Branch("b13pn2g10t",&mh2n10t,"b13pn2g10t/D");
            tOut->Branch("b13pn2g10x",&mh2n10x,"b13pn2g10x/D");
            tOut->Branch("b13pn2g10y",&mh2n10y,"b13pn2g10y/D");
            tOut->Branch("b13pn2g10z",&mh2n10z,"b13pn2g10z/D");

	    // And add in the new GEANT parameters... note that they are simply pg instead of p
	    // 3rd Neutron
            tOut->Branch("b13pn3gmultiplicity",&mh3nmultiplicity,"b13pn3gmultiplicity/I");

            tOut->Branch("b13pn3gZ1Mult",&mh3_z1,"b13pn3gZ1Mult/I");//zwk particle mults
            tOut->Branch("b13pn3gZ2Mult",&mh3_z2,"b13pn3gZ2Mult/I");
            tOut->Branch("b13pn3gZGt2Mult",&mh3_zgt2,"b13pn3gZGt2Mult/I");
            tOut->Branch("b13pn3gGammaMult",&mh3_g,"b13pn3gGammaMult/I");

            tOut->Branch("b13pn3g1Edep",&mh3n1Edep,"b13pn3g1Edep/D");
            tOut->Branch("b13pn3g1light",&mh3n1light,"b13pn3g1light/D");
            tOut->Branch("b13pn3g1t",&mh3n1t,"b13pn3g1t/D");
            tOut->Branch("b13pn3g1x",&mh3n1x,"b13pn3g1x/D");
            tOut->Branch("b13pn3g1y",&mh3n1y,"b13pn3g1y/D");
            tOut->Branch("b13pn3g1z",&mh3n1z,"b13pn3g1z/D");
            tOut->Branch("b13pn3g2Edep",&mh3n2Edep,"b13pn3g2Edep/D");
            tOut->Branch("b13pn3g2light",&mh3n2light,"b13pn3g2light/D");
            tOut->Branch("b13pn3g2t",&mh3n2t,"b13pn3g2t/D");
            tOut->Branch("b13pn3g2x",&mh3n2x,"b13pn3g2x/D");
            tOut->Branch("b13pn3g2y",&mh3n2y,"b13pn3g2y/D");
            tOut->Branch("b13pn3g2z",&mh3n2z,"b13pn3g2z/D");
            tOut->Branch("b13pn3g3Edep",&mh3n3Edep,"b13pn3g3Edep/D");
            tOut->Branch("b13pn3g3light",&mh3n3light,"b13pn3g3light/D");
            tOut->Branch("b13pn3g3t",&mh3n3t,"b13pn3g3t/D");
            tOut->Branch("b13pn3g3x",&mh3n3x,"b13pn3g3x/D");
            tOut->Branch("b13pn3g3y",&mh3n3y,"b13pn3g3y/D");
            tOut->Branch("b13pn3g3z",&mh3n3z,"b13pn3g3z/D");
            tOut->Branch("b13pn3g4Edep",&mh3n4Edep,"b13pn3g4Edep/D");
            tOut->Branch("b13pn3g4light",&mh3n4light,"b13pn3g4light/D");
            tOut->Branch("b13pn3g4t",&mh3n4t,"b13pn3g4t/D");
            tOut->Branch("b13pn3g4x",&mh3n4x,"b13pn3g4x/D");
            tOut->Branch("b13pn3g4y",&mh3n4y,"b13pn3g4y/D");
            tOut->Branch("b13pn3g4z",&mh3n4z,"b13pn3g4z/D");
            tOut->Branch("b13pn3g5Edep",&mh3n5Edep,"b13pn3g5Edep/D");
            tOut->Branch("b13pn3g5light",&mh3n5light,"b13pn3g5light/D");
            tOut->Branch("b13pn3g5t",&mh3n5t,"b13pn3g5t/D");
            tOut->Branch("b13pn3g5x",&mh3n5x,"b13pn3g5x/D");
            tOut->Branch("b13pn3g5y",&mh3n5y,"b13pn3g5y/D");
            tOut->Branch("b13pn3g5z",&mh3n5z,"b13pn3g5z/D");
            tOut->Branch("b13pn3g6Edep",&mh3n6Edep,"b13pn3g6Edep/D");
            tOut->Branch("b13pn3g6light",&mh3n6light,"b13pn3g6light/D");
            tOut->Branch("b13pn3g6t",&mh3n6t,"b13pn3g6t/D");
            tOut->Branch("b13pn3g6x",&mh3n6x,"b13pn3g6x/D");
            tOut->Branch("b13pn3g6y",&mh3n6y,"b13pn3g6y/D");
            tOut->Branch("b13pn3g6z",&mh3n6z,"b13pn3g6z/D");
            tOut->Branch("b13pn3g7Edep",&mh3n7Edep,"b13pn3g7Edep/D");
            tOut->Branch("b13pn3g7light",&mh3n7light,"b13pn3g7light/D");
            tOut->Branch("b13pn3g7t",&mh3n7t,"b13pn3g7t/D");
            tOut->Branch("b13pn3g7x",&mh3n7x,"b13pn3g7x/D");
            tOut->Branch("b13pn3g7y",&mh3n7y,"b13pn3g7y/D");
            tOut->Branch("b13pn3g7z",&mh3n7z,"b13pn3g7z/D");
            tOut->Branch("b13pn3g8Edep",&mh3n8Edep,"b13pn3g8Edep/D");
            tOut->Branch("b13pn3g8light",&mh3n8light,"b13pn3g8light/D");
            tOut->Branch("b13pn3g8t",&mh3n8t,"b13pn3g8t/D");
            tOut->Branch("b13pn3g8x",&mh3n8x,"b13pn3g8x/D");
            tOut->Branch("b13pn3g8y",&mh3n8y,"b13pn3g8y/D");
            tOut->Branch("b13pn3g8z",&mh3n8z,"b13pn2g8z/D");
            tOut->Branch("b13pn3g9Edep",&mh3n9Edep,"b13pn3g9Edep/D");
            tOut->Branch("b13pn3g9light",&mh3n9light,"b13pn3g9light/D");
            tOut->Branch("b13pn3g9t",&mh3n9t,"b13pn3g9t/D");
            tOut->Branch("b13pn3g9x",&mh3n9x,"b13pn3g9x/D");
            tOut->Branch("b13pn3g9y",&mh3n9y,"b13pn3g9y/D");
            tOut->Branch("b13pn3g9z",&mh3n9z,"b13pn3g9z/D");
            tOut->Branch("b13pn3g10Edep",&mh3n10Edep,"b13pn3g10Edep/D");
            tOut->Branch("b13pn3g10light",&mh3n10light,"b13pn3g10light/D");
            tOut->Branch("b13pn3g10t",&mh3n10t,"b13pn3g10t/D");
            tOut->Branch("b13pn3g10x",&mh3n10x,"b13pn3g10x/D");
            tOut->Branch("b13pn3g10y",&mh3n10y,"b13pn3g10y/D");
            tOut->Branch("b13pn3g10z",&mh3n10z,"b13pn3g10z/D");



	double g1light,g2light,g3light,g4light,g5light,g6light,g7light,g8light,g9light,g10light,
	       g11light,g12light,g13light,g14light,g15light,g16light,g17light,g18light,g19light,g20light,
	       g21light,g22light,g23light,g24light,g25light,g26light,g27light,g28light,g29light,g30light;


	double g1Edep,g2Edep,g3Edep,g4Edep,g5Edep,g6Edep,g7Edep,g8Edep,g9Edep,g10Edep,
	       g11Edep,g12Edep,g13Edep,g14Edep,g15Edep,g16Edep,g17Edep,g18Edep,g19Edep,g20Edep,
	       g21Edep,g22Edep,g23Edep,g24Edep,g25Edep,g26Edep,g27Edep,g28Edep,g29Edep,g30Edep;

	double g1t,g2t,g3t,g4t,g5t,g6t,g7t,g8t,g9t,g10t,g11t,g12t,g13t,g14t,g15t,g16t,g17t,g18t,g19t,g20t,
	       g21t,g22t,g23t,g24t,g25t,g26t,g27t,g28t,g29t,g30t;

	double g1x,g2x,g3x,g4x,g5x,g6x,g7x,g8x,g9x,g10x,g11x,g12x,g13x,g14x,g15x,g16x,g17x,g18x,g19x,g20x,
	       g21x,g22x,g23x,g24x,g25x,g26x,g27x,g28x,g29x,g30x;

	double g1y,g2y,g3y,g4y,g5y,g6y,g7y,g8y,g9y,g10y,g11y,g12y,g13y,g14y,g15y,g16y,g17y,g18y,g19y,g20y,
	       g21y,g22y,g23y,g24y,g25y,g26y,g27y,g28y,g29y,g30y;

	double g1z,g2z,g3z,g4z,g5z,g6z,g7z,g8z,g9z,g10z,g11z,g12z,g13z,g14z,g15z,g16z,g17z,g18z,g19z,g20z,
	       g21z,g22z,g23z,g24z,g25z,g26z,g27z,g28z,g29z,g30z;


	string name[] = {"b13pg1","b13pg2","b13pg3","b13pg4","b13pg5","b13pg6","b13pg7","b13pg8","b13pg9","b13pg10",
			 "b13pg11","b13pg12","b13pg13","b13pg14","b13pg15","b13pg16","b13pg17","b13pg18","b13pg19","b13pg20",
			 "b13pg21","b13pg22","b13pg23","b13pg24","b13pg25","b13pg26","b13pg27","b13pg28","b13pg29","b13pg30"};
	
	double* ps_light[] = {&g1light,&g2light,&g3light,&g4light,&g5light,&g6light,&g7light,&g8light,&g9light,&g10light,
			      &g11light,&g12light,&g13light,&g14light,&g15light,&g16light,&g17light,&g18light,&g19light,&g20light,
			      &g21light,&g22light,&g23light,&g24light,&g25light,&g26light,&g27light,&g28light,&g29light,&g30light};


	double* ps_Edep[] = {&g1Edep,&g2Edep,&g3Edep,&g4Edep,&g5Edep,&g6Edep,&g7Edep,&g8Edep,&g9Edep,&g10Edep,
			     &g11Edep,&g12Edep,&g13Edep,&g14Edep,&g15Edep,&g16Edep,&g17Edep,&g18Edep,&g19Edep,&g20Edep,
			     &g21Edep,&g22Edep,&g23Edep,&g24Edep,&g25Edep,&g26Edep,&g27Edep,&g28Edep,&g29Edep,&g30Edep};


	double* ps_t[] = {&g1t,&g2t,&g3t,&g4t,&g5t,&g6t,&g7t,&g8t,&g9t,&g10t,&g11t,&g12t,&g13t,&g14t,&g15t,&g16t,&g17t,&g18t,&g19t,&g20t,
			  &g21t,&g22t,&g23t,&g24t,&g25t,&g26t,&g27t,&g28t,&g29t,&g30t};

	double* ps_x[] = {&g1x,&g2x,&g3x,&g4x,&g5x,&g6x,&g7x,&g8x,&g9x,&g10x,&g11x,&g12x,&g13x,&g14x,&g15x,&g16x,&g17x,&g18x,&g19x,&g20x,
			  &g21x,&g22x,&g23x,&g24x,&g25x,&g26x,&g27x,&g28x,&g29x,&g30x};

	double* ps_y[] = {&g1y,&g2y,&g3y,&g4y,&g5y,&g6y,&g7y,&g8y,&g9y,&g10y,&g11y,&g12y,&g13y,&g14y,&g15y,&g16y,&g17y,&g18y,&g19y,&g20y,
			  &g21y,&g22y,&g23y,&g24y,&g25y,&g26y,&g27y,&g28y,&g29y,&g30y};


	double* ps_z[] = {&g1z,&g2z,&g3z,&g4z,&g5z,&g6z,&g7z,&g8z,&g9z,&g10z,&g11z,&g12z,&g13z,&g14z,&g15z,&g16z,&g17z,&g18z,&g19z,&g20z,
			  &g21z,&g22z,&g23z,&g24z,&g25z,&g26z,&g27z,&g28z,&g29z,&g30z};



	for(int l = 0; l < 30; l++) {
	string first_Edep = name[l]+"Edep";
	string first_light = name[l]+"light";
	string first_t = name[l]+"t";
	string first_x = name[l]+"x";
	string first_y = name[l]+"y";
	string first_z = name[l]+"z";	
	string second_Edep = name[l]+"Edep/D";
	string second_light = name[l]+"light/D";
	string second_t = name[l]+"t/D";
	string second_x = name[l]+"x/D";
	string second_y = name[l]+"y/D";
	string second_z = name[l]+"z/D";

	tOut->Branch(first_Edep.c_str(),ps_Edep[l],second_Edep.c_str());
        tOut->Branch(first_light.c_str(),ps_light[l],second_light.c_str());
        tOut->Branch(first_t.c_str(),ps_t[l],second_t.c_str());
        tOut->Branch(first_x.c_str(),ps_x[l],second_x.c_str());
        tOut->Branch(first_y.c_str(),ps_y[l],second_y.c_str());
        tOut->Branch(first_z.c_str(),ps_z[l],second_z.c_str());
		
	}

            // Now loop over the files and merge them into the output file we want
            // but first check for length
            if (outputn1_nevents != outputn2_nevents || outputn1_nevents != outputn3_nevents )
            {
                    std::cout << "The Three GEANT output files have different for the number of neutrons! This is disallowed and I am exiting\n";
                    return 1;
            
            } else if (input_nevent > outputn1_nevents)
            {
                std::cout << "ST MONA output had more events than GEANT output (" << input_nevent << " vs. " << outputn1_nevents << ") so I am only merging until I hit the end of the GEANT output.\n";
                for (Int_t i = 0; i < outputn1_nevents; i++)
                {
                    t->GetEntry(i);
                    tn1Temp->GetEntry(i);
                    tn2Temp->GetEntry(i);
		    tn3Temp->GetEntry(i);
                    tOut->Fill();
                }
            } else if (input_nevent < outputn1_nevents )
            {
                std::cout << "GEANT output had MORE events than ST MONA output! This is disallowed and I am exiting\n";
                return 1;
            } else
            {
                std::cout << "ST MONA and GEANT outputs have same number of events. I am satisfied and am merging all the data.\n";
                for (Int_t i = 0; i < outputn1_nevents; i++)
                {
                t->GetEntry(i);
                tn1Temp->GetEntry(i);
                tn2Temp->GetEntry(i);
 		tn3Temp->GetEntry(i);
		    
		double total_light[] = {mh1n1light,mh1n2light,mh1n3light,mh1n4light,mh1n5light,mh1n6light,mh1n7light,mh1n8light,mh1n9light,mh1n10light,
					mh2n1light,mh2n2light,mh2n3light,mh2n4light,mh2n5light,mh2n6light,mh2n7light,mh2n8light,mh2n9light,mh2n10light,
					mh3n1light,mh3n2light,mh3n3light,mh3n4light,mh3n5light,mh3n6light,mh3n7light,mh3n8light,mh3n9light,mh3n10light};

	 	double total_Edep[] = {mh1n1Edep,mh1n2Edep,mh1n3Edep,mh1n4Edep,mh1n5Edep,mh1n6Edep,mh1n7Edep,mh1n8Edep,mh1n9Edep,mh1n10Edep,
				       mh2n1Edep,mh2n2Edep,mh2n3Edep,mh2n4Edep,mh2n5Edep,mh2n6Edep,mh2n7Edep,mh2n8Edep,mh2n9Edep,mh2n10Edep,
				       mh3n1Edep,mh3n2Edep,mh3n3Edep,mh3n4Edep,mh3n5Edep,mh3n6Edep,mh3n7Edep,mh3n8Edep,mh3n9Edep,mh3n10Edep};

		double total_t[] = {mh1n1t,mh1n2t,mh1n3t,mh1n4t,mh1n5t,mh1n6t,mh1n7t,mh1n8t,mh1n9t,mh1n10t,
				    mh2n1t,mh2n2t,mh2n3t,mh2n4t,mh2n5t,mh2n6t,mh2n7t,mh2n8t,mh2n9t,mh2n10t,
				    mh3n1t,mh3n2t,mh3n3t,mh3n4t,mh3n5t,mh3n6t,mh3n7t,mh3n8t,mh3n9t,mh3n10t};

		double total_x[] = {mh1n1x,mh1n2x,mh1n3x,mh1n4x,mh1n5x,mh1n6x,mh1n7x,mh1n8x,mh1n9x,mh1n10x,
				    mh2n1x,mh2n2x,mh2n3x,mh2n4x,mh2n5x,mh2n6x,mh2n7x,mh2n8x,mh2n9x,mh2n10x,
				    mh3n1x,mh3n2x,mh3n3x,mh3n4x,mh3n5x,mh3n6x,mh3n7x,mh3n8x,mh3n9x,mh3n10x};

		double total_y[] = {mh1n1y,mh1n2y,mh1n3y,mh1n4y,mh1n5y,mh1n6y,mh1n7y,mh1n8y,mh1n9y,mh1n10y,
				    mh2n1y,mh2n2y,mh2n3y,mh2n4y,mh2n5y,mh2n6y,mh2n7y,mh2n8y,mh2n9y,mh2n10y,
				    mh3n1y,mh3n2y,mh3n3y,mh3n4y,mh3n5y,mh3n6y,mh3n7y,mh3n8y,mh3n9y,mh3n10y};

		double total_z[] = {mh1n1z,mh1n2z,mh1n3z,mh1n4z,mh1n5z,mh1n6z,mh1n7z,mh1n8z,mh1n9z,mh1n10z,
				    mh2n1z,mh2n2z,mh2n3z,mh2n4z,mh2n5z,mh2n6z,mh2n7z,mh2n8z,mh2n9z,mh2n10z,
				    mh3n1z,mh3n2z,mh3n3z,mh3n4z,mh3n5z,mh3n6z,mh3n7z,mh3n8z,mh3n9z,mh3n10z};

		double final_light[30];
		double final_Edep[30];
		double final_t[30];
		double final_x[30];
		double final_y[30];
		double final_z[30];

		for(int m = 0; m < 30; m++) {

		double temp_t = 10000;
		int temp_n = -1;

		for(int n = 0; n < 30; n++) {
    		if (total_t[n]>0 && total_t[n]<temp_t)	{
			temp_t = total_t[n];
			temp_n = n;
			}
		}

		if (temp_n == -1 ) {
		for(int o = 0; o < 30; o++) {
    		if (total_Edep[o]==0 && total_light[o]==0 && total_t[o]==0 ) {
			temp_n = o;
			}
		}
		}
		final_Edep[m] = total_Edep[temp_n];
		final_light[m] = total_light[temp_n];
		final_t[m] = total_t[temp_n];
		final_x[m] = total_x[temp_n];
		final_y[m] = total_y[temp_n];
		final_z[m] = total_z[temp_n];

		total_Edep[temp_n] = 0;
		total_light[temp_n] = 0;
		total_t[temp_n] = 0;
		total_x[temp_n] = 0;
		total_y[temp_n] = 0;
		total_z[temp_n] = 0;

		}


		g1light = final_light[0]; g2light = final_light[1]; g3light = final_light[2]; g4light = final_light[3]; g5light = final_light[4];
		g1Edep = final_Edep[0];   g2Edep  = final_Edep[1];  g3Edep  = final_Edep[2];  g4Edep  = final_Edep[3];	g5Edep  = final_Edep[4];
		g1t = final_t[0];	  g2t     = final_t[1];     g3t	    = final_t[2];     g4t     = final_t[3];     g5t     = final_t[4];
		g1x = final_x[0];	  g2x     = final_x[1];     g3x     = final_x[2];     g4x     = final_x[3];     g5x     = final_x[4];
		g1y = final_y[0];	  g2y     = final_y[1];     g3y     = final_y[2];     g4y     = final_y[3];     g5y     = final_y[4];
		g1z = final_z[0];	  g2z     = final_z[1];     g3z     = final_z[2];     g4z     = final_z[3];     g5z     = final_z[4];
		g6light = final_light[5]; g7light = final_light[6]; g8light = final_light[7]; g9light = final_light[8]; g10light = final_light[9];
		g6Edep = final_Edep[5];   g7Edep  = final_Edep[6];  g8Edep  = final_Edep[7];  g9Edep  = final_Edep[8];	g10Edep  = final_Edep[9];
		g6t = final_t[5];	  g7t     = final_t[6];     g8t	    = final_t[7];     g9t     = final_t[8];     g10t     = final_t[9];
		g6x = final_x[5];	  g7x     = final_x[6];     g8x     = final_x[7];     g9x     = final_x[8];     g10x     = final_x[9];
		g6y = final_y[5];	  g7y     = final_y[6];     g8y     = final_y[7];     g9y     = final_y[8];     g10y     = final_y[9];
		g6z = final_z[5];	  g7z     = final_z[6];     g8z     = final_z[7];     g9z     = final_z[8];     g10z     = final_z[9];

		g11light = final_light[10]; g12light = final_light[11]; g13light = final_light[12]; g14light = final_light[13]; g15light = final_light[14];
		g11Edep = final_Edep[10];   g12Edep  = final_Edep[11];  g13Edep  = final_Edep[12];  g14Edep  = final_Edep[13];  g15Edep  = final_Edep[14];
		g11t = final_t[10];	    g12t     = final_t[11];     g13t     = final_t[12];     g14t     = final_t[13];     g15t     = final_t[14];
		g11x = final_x[10];	    g12x     = final_x[11];     g13x     = final_x[12];     g14x     = final_x[13];     g15x     = final_x[14];
		g11y = final_y[10];	    g12y     = final_y[11];     g13y     = final_y[12];     g14y     = final_y[13];     g15y     = final_y[14];
		g11z = final_z[10];	    g12z     = final_z[11];     g13z     = final_z[12];     g14z     = final_z[13];     g15z     = final_z[14];
		g16light = final_light[15]; g17light = final_light[16]; g18light = final_light[17]; g19light = final_light[18]; g20light = final_light[19];
		g16Edep = final_Edep[15];   g17Edep  = final_Edep[16];  g18Edep  = final_Edep[17];  g19Edep  = final_Edep[18];  g20Edep  = final_Edep[19];
		g16t = final_t[15];	    g17t     = final_t[16];     g18t     = final_t[17];     g19t     = final_t[18];     g20t     = final_t[19];
		g16x = final_x[15];	    g17x     = final_x[16];     g18x     = final_x[17];     g19x     = final_x[18];     g20x     = final_x[19];
		g16y = final_y[15];	    g17y     = final_y[16];     g18y     = final_y[17];     g19y     = final_y[18];     g20y     = final_y[19];
		g16z = final_z[15];	    g17z     = final_z[16];     g18z     = final_z[17];     g19z     = final_z[18];     g20z     = final_z[19];
	
		g21light = final_light[20]; g22light = final_light[21]; g23light = final_light[22]; g24light = final_light[23]; g25light = final_light[24];
		g21Edep = final_Edep[20];   g22Edep  = final_Edep[21];  g23Edep  = final_Edep[22];  g24Edep  = final_Edep[23];  g25Edep  = final_Edep[24];
		g21t = final_t[20];	    g22t     = final_t[21];     g23t     = final_t[22];     g24t     = final_t[23];     g25t     = final_t[24];
		g21x = final_x[20];	    g22x     = final_x[21];     g23x     = final_x[22];     g24x     = final_x[23];     g25x     = final_x[24];
		g21y = final_y[20];	    g22y     = final_y[21];     g23y     = final_y[22];     g24y     = final_y[23];     g25y     = final_y[24];
		g21z = final_z[20];	    g22z     = final_z[21];     g23z     = final_z[22];     g24z     = final_z[23];     g25z     = final_z[24];
		g26light = final_light[25]; g27light = final_light[26]; g28light = final_light[27]; g29light = final_light[28]; g30light = final_light[29];
		g26Edep = final_Edep[25];   g27Edep  = final_Edep[26];  g28Edep  = final_Edep[27];  g29Edep  = final_Edep[28];  g30Edep  = final_Edep[29];
		g26t = final_t[25];	    g27t     = final_t[26];     g28t     = final_t[27];     g29t     = final_t[28];     g30t     = final_t[29];
		g26x = final_x[25];	    g27x     = final_x[26];     g28x     = final_x[27];     g29x     = final_x[28];     g30x     = final_x[29];
		g26y = final_y[25];	    g27y     = final_y[26];     g28y     = final_y[27];     g29y     = final_y[28];     g30y     = final_y[29];
		g26z = final_z[25];	    g27z     = final_z[26];     g28z     = final_z[27];     g29z     = final_z[28];     g30z     = final_z[29];

	
                tOut->Fill();
                }
            }

            tOut->Write();
            inFile->Close();
            tempFile->Close();
            temp2File->Close();
	    temp3File->Close();
            outFile->Close();
        }// End 3n


        else if (nneuts == 2) // Begin 2n
        {	cout << "2 NEUTRON!" << endl;
            // Now we find the relevant branches to look at
	  Double_t b1p0x,b1p0tx,b1p0y,b1p0ty,b1p0Ekin,b2p0x,b2p0tx,b2p0y,b2p0ty,b2p0t,b2p0z,b2p0TP,b2p0dE,b2p0Ekin,b2p0R_pz,b2p0R_exen,b2p0R_exen2,b2p0R_ph,b2p0R_ph2,b2p0R_th,b2p0R_th2,b4p0x,b4p0tx,b4p0y,b4p0ty,b4p0t,b4p0Ekin,b6p0Ekin,b6p0x,b6p0y,b6p0tx,b6p0ty,b9p1t,b9p1x,b9p1y,b9p1z,b9p1Ekin,b9p2t,b9p2x,b9p2y,b9p2z,b9p2Ekin;

            string bn_old[] = {"b1p0x", "b1p0tx", "b1p0y", "b1p0ty", "b1p0Ekin",
                "b2p0x", "b2p0tx", "b2p0y", "b2p0ty", "b2p0t", "b2p0z", "b2p0TP", "b2p0dE", "b2p0Ekin",
		"b2p0R_pz","b2p0R_exen", "b2p0R_exen2", "b2p0R_ph", "b2p0R_ph2", "b2p0R_th", "b2p0R_th2",
                "b4p0x", "b4p0tx", "b4p0y", "b4p0ty", "b4p0t", "b4p0Ekin",
                "b6p0x", "b6p0tx", "b6p0y", "b6p0ty", "b6p0Ekin",
                "b9p1t", "b9p1x", "b9p1y", "b9p1z", "b9p1Ekin",
                "b9p2t", "b9p2x", "b9p2y", "b9p2z", "b9p2Ekin",
                };
        
            double* ps_old[] = {&b1p0x,&b1p0tx,&b1p0y,&b1p0ty,&b1p0Ekin,
                &b2p0x,&b2p0tx,&b2p0y,&b2p0ty,&b2p0t,&b2p0z,&b2p0TP,&b2p0dE,
		&b2p0Ekin,&b2p0R_pz,&b2p0R_exen,&b2p0R_exen2,&b2p0R_ph,&b2p0R_ph2,&b2p0R_th,&b2p0R_th2,
                &b4p0x,&b4p0tx,&b4p0y,&b4p0ty,&b4p0t,&b4p0Ekin,
                &b6p0x,&b6p0tx,&b6p0y,&b6p0ty,&b6p0Ekin,
                &b9p1t,&b9p1x,&b9p1y,&b9p1z,&b9p1Ekin,
                &b9p2t,&b9p2x,&b9p2y,&b9p2z,&b9p2Ekin
                };   

            for (size_t i(0); i<sizeof(ps_old)/sizeof(double*); i++) {
                t->SetBranchAddress(bn_old[i].c_str(),ps_old[i]);  
            }

            // Get number of entries in the file
            Int_t input_nevent = t->GetEntries();

            // Then open the GEANT output for the 1st neutron root file
	    TString n1name = dirdata+"/neutron1"+uniqueID+".root";//neutron1
            TFile *tempFile = new TFile(n1name); assert(tempFile);
            TTree *tn1Temp = (TTree*)tempFile->Get("t"); assert(tn1Temp);

            Double_t mh1n1Edep,mh1n1light,mh1n1t,mh1n1x,mh1n1y,mh1n1z,mh1n2Edep,mh1n2light,mh1n2t,mh1n2x,mh1n2y,mh1n2z,mh1n3Edep,mh1n3light,mh1n3t,mh1n3x,mh1n3y,mh1n3z,mh1n4Edep,mh1n4light,mh1n4t,mh1n4x,mh1n4y,mh1n4z,mh1n5Edep,mh1n5light,mh1n5t,mh1n5x,mh1n5y,mh1n5z,mh1n6Edep,mh1n6light,mh1n6t,mh1n6x,mh1n6y,mh1n6z,mh1n7Edep,mh1n7light,mh1n7t,mh1n7x,mh1n7y,mh1n7z,mh1n8Edep,mh1n8light,mh1n8t,mh1n8x,mh1n8y,mh1n8z,mh1n9Edep,mh1n9light,mh1n9t,mh1n9x,mh1n9y,mh1n9z,mh1n10Edep,mh1n10light,mh1n10t,mh1n10x,mh1n10y,mh1n10z;

            Int_t mh1nmultiplicity;
            Int_t mh1_z1; //zwk particle multiplicities
            Int_t mh1_z2;
            Int_t mh1_zgt2;
            Int_t mh1_g;

            string bn_n1[] = {"MoNA.Hit.1.e","MoNA.Hit.1.light","MoNA.Hit.1.t","MoNA.Hit.1.x","MoNA.Hit.1.y","MoNA.Hit.1.z",
                    "MoNA.Hit.2.e","MoNA.Hit.2.light","MoNA.Hit.2.t","MoNA.Hit.2.x","MoNA.Hit.2.y","MoNA.Hit.2.z",
                "MoNA.Hit.3.e","MoNA.Hit.3.light","MoNA.Hit.3.t","MoNA.Hit.3.x","MoNA.Hit.3.y","MoNA.Hit.3.z",
                "MoNA.Hit.4.e","MoNA.Hit.4.light","MoNA.Hit.4.t","MoNA.Hit.4.x","MoNA.Hit.4.y","MoNA.Hit.4.z",
                "MoNA.Hit.5.e","MoNA.Hit.5.light","MoNA.Hit.5.t","MoNA.Hit.5.x","MoNA.Hit.5.y","MoNA.Hit.5.z",
                "MoNA.Hit.6.e","MoNA.Hit.6.light","MoNA.Hit.6.t","MoNA.Hit.6.x","MoNA.Hit.6.y","MoNA.Hit.6.z",
                "MoNA.Hit.7.e","MoNA.Hit.7.light","MoNA.Hit.7.t","MoNA.Hit.7.x","MoNA.Hit.7.y","MoNA.Hit.7.z",
                "MoNA.Hit.8.e","MoNA.Hit.8.light","MoNA.Hit.8.t","MoNA.Hit.8.x","MoNA.Hit.8.y","MoNA.Hit.8.z",
                "MoNA.Hit.9.e","MoNA.Hit.9.light","MoNA.Hit.9.t","MoNA.Hit.9.x","MoNA.Hit.9.y","MoNA.Hit.9.z",
                "MoNA.Hit.10.e","MoNA.Hit.10.light","MoNA.Hit.10.t","MoNA.Hit.10.x","MoNA.Hit.10.y","MoNA.Hit.10.z",
                };

            double* ps_n1[] = {&mh1n1Edep,&mh1n1light,&mh1n1t,&mh1n1x,&mh1n1y,&mh1n1z,
                &mh1n2Edep,&mh1n2light,&mh1n2t,&mh1n2x,&mh1n2y,&mh1n2z,
                &mh1n3Edep,&mh1n3light,&mh1n3t,&mh1n3x,&mh1n3y,&mh1n3z,
                &mh1n4Edep,&mh1n4light,&mh1n4t,&mh1n4x,&mh1n4y,&mh1n4z,
                &mh1n5Edep,&mh1n5light,&mh1n5t,&mh1n5x,&mh1n5y,&mh1n5z,
                &mh1n6Edep,&mh1n6light,&mh1n6t,&mh1n6x,&mh1n6y,&mh1n6z,
                &mh1n7Edep,&mh1n7light,&mh1n7t,&mh1n7x,&mh1n7y,&mh1n7z,
                &mh1n8Edep,&mh1n8light,&mh1n8t,&mh1n8x,&mh1n8y,&mh1n8z,
                &mh1n9Edep,&mh1n9light,&mh1n9t,&mh1n9x,&mh1n9y,&mh1n9z,
                &mh1n10Edep,&mh1n10light,&mh1n10t,&mh1n10x,&mh1n10y,&mh1n10z,
                };

            for (size_t i(0); i<sizeof(ps_n1)/sizeof(double*); i++) {
                tn1Temp->SetBranchAddress(bn_n1[i].c_str(),ps_n1[i]);  
            }

            tn1Temp->SetBranchAddress("MoNA.multiplicity",&mh1nmultiplicity);  
            tn1Temp->SetBranchAddress("MoNA.Z1mult",&mh1_z1);  
            tn1Temp->SetBranchAddress("MoNA.Z2mult",&mh1_z2);  
            tn1Temp->SetBranchAddress("MoNA.ZGt2mult",&mh1_zgt2);  
            tn1Temp->SetBranchAddress("MoNA.Gmult",&mh1_g);  

            // Get number of entries in the file
            Int_t outputn1_nevents = tn1Temp->GetEntries();

            // Then open the GEANT output for the 2nd neutron root file
	    TString n2name = dirdata+"/neutron2"+uniqueID+".root";//neutron2
            TFile *temp2File = new TFile(n2name); assert(temp2File);
            TTree *tn2Temp = (TTree*)temp2File->Get("t"); assert(tn2Temp);

            Double_t mh2n1Edep,mh2n1light,mh2n1t,mh2n1x,mh2n1y,mh2n1z,mh2n2Edep,mh2n2light,mh2n2t,mh2n2x,mh2n2y,mh2n2z,mh2n3Edep,mh2n3light,mh2n3t,mh2n3x,mh2n3y,mh2n3z,mh2n4Edep,mh2n4light,mh2n4t,mh2n4x,mh2n4y,mh2n4z,mh2n5Edep,mh2n5light,mh2n5t,mh2n5x,mh2n5y,mh2n5z,mh2n6Edep,mh2n6light,mh2n6t,mh2n6x,mh2n6y,mh2n6z,mh2n7Edep,mh2n7light,mh2n7t,mh2n7x,mh2n7y,mh2n7z,mh2n8Edep,mh2n8light,mh2n8t,mh2n8x,mh2n8y,mh2n8z,mh2n9Edep,mh2n9light,mh2n9t,mh2n9x,mh2n9y,mh2n9z,mh2n10Edep,mh2n10light,mh2n10t,mh2n10x,mh2n10y,mh2n10z;

            Int_t mh2nmultiplicity;
            Int_t mh2_z1; //zwk particle multiplicities
            Int_t mh2_z2;
            Int_t mh2_zgt2;
            Int_t mh2_g;

            string bn_n2[] = {"MoNA.Hit.1.e","MoNA.Hit.1.light","MoNA.Hit.1.t","MoNA.Hit.1.x","MoNA.Hit.1.y","MoNA.Hit.1.z",
                "MoNA.Hit.2.e","MoNA.Hit.2.light","MoNA.Hit.2.t","MoNA.Hit.2.x","MoNA.Hit.2.y","MoNA.Hit.2.z",
                "MoNA.Hit.3.e","MoNA.Hit.3.light","MoNA.Hit.3.t","MoNA.Hit.3.x","MoNA.Hit.3.y","MoNA.Hit.3.z",
                "MoNA.Hit.4.e","MoNA.Hit.4.light","MoNA.Hit.4.t","MoNA.Hit.4.x","MoNA.Hit.4.y","MoNA.Hit.4.z",
                "MoNA.Hit.5.e","MoNA.Hit.5.light","MoNA.Hit.5.t","MoNA.Hit.5.x","MoNA.Hit.5.y","MoNA.Hit.5.z",
                "MoNA.Hit.6.e","MoNA.Hit.6.light","MoNA.Hit.6.t","MoNA.Hit.6.x","MoNA.Hit.6.y","MoNA.Hit.6.z",
                "MoNA.Hit.7.e","MoNA.Hit.7.light","MoNA.Hit.7.t","MoNA.Hit.7.x","MoNA.Hit.7.y","MoNA.Hit.7.z",
                "MoNA.Hit.8.e","MoNA.Hit.8.light","MoNA.Hit.8.t","MoNA.Hit.8.x","MoNA.Hit.8.y","MoNA.Hit.8.z",
                "MoNA.Hit.9.e","MoNA.Hit.9.light","MoNA.Hit.9.t","MoNA.Hit.9.x","MoNA.Hit.9.y","MoNA.Hit.9.z",
                "MoNA.Hit.10.e","MoNA.Hit.10.light","MoNA.Hit.10.t","MoNA.Hit.10.x","MoNA.Hit.10.y","MoNA.Hit.10.z",
                };

            double* ps_n2[] = {&mh2n1Edep,&mh2n1light,&mh2n1t,&mh2n1x,&mh2n1y,&mh2n1z,
                &mh2n2Edep,&mh2n2light,&mh2n2t,&mh2n2x,&mh2n2y,&mh2n2z,
                &mh2n3Edep,&mh2n3light,&mh2n3t,&mh2n3x,&mh2n3y,&mh2n3z,
                &mh2n4Edep,&mh2n4light,&mh2n4t,&mh2n4x,&mh2n4y,&mh2n4z,
                &mh2n5Edep,&mh2n5light,&mh2n5t,&mh2n5x,&mh2n5y,&mh2n5z,
                &mh2n6Edep,&mh2n6light,&mh2n6t,&mh2n6x,&mh2n6y,&mh2n6z,
                &mh2n7Edep,&mh2n7light,&mh2n7t,&mh2n7x,&mh2n7y,&mh2n7z,
                &mh2n8Edep,&mh2n8light,&mh2n8t,&mh2n8x,&mh2n8y,&mh2n8z,
                &mh2n9Edep,&mh2n9light,&mh2n9t,&mh2n9x,&mh2n9y,&mh2n9z,
                &mh2n10Edep,&mh2n10light,&mh2n10t,&mh2n10x,&mh2n10y,&mh2n10z,
                };

            for (size_t i(0); i<sizeof(ps_n2)/sizeof(double*); i++) {
                tn2Temp->SetBranchAddress(bn_n2[i].c_str(),ps_n2[i]);  
            }

            tn2Temp->SetBranchAddress("MoNA.multiplicity",&mh2nmultiplicity); 
            tn2Temp->SetBranchAddress("MoNA.Z1mult",&mh2_z1);  
            tn2Temp->SetBranchAddress("MoNA.Z2mult",&mh2_z2);  
            tn2Temp->SetBranchAddress("MoNA.ZGt2mult",&mh2_zgt2);  
            tn2Temp->SetBranchAddress("MoNA.Gmult",&mh2_g);  

            // Get number of entries in the file
            Int_t outputn2_nevents = tn2Temp->GetEntries();

            // Finally open the output .root file
            TFile *outFile = new TFile(argv[2],"recreate"); assert(outFile);
            TTree *tOut = new TTree("t","Simulation Output"); assert(tOut);

            // We want to preserve the original st-mona parameters
            TBranch *obb1p0x = tOut->Branch("b1p0x",&b1p0x,"b1p0x/D");
            TBranch *obb1p0tx = tOut->Branch("b1p0tx",&b1p0tx,"b1p0tx/D");
            TBranch *obb1p0y = tOut->Branch("b1p0y",&b1p0y,"b1p0y/D");
            TBranch *obb1p0ty = tOut->Branch("b1p0ty",&b1p0ty,"b1p0ty/D");
            TBranch *obb1p0Ekin = tOut->Branch("b1p0Ekin",&b1p0Ekin,"b1p0Ekin/D");
            TBranch *obb2p0x = tOut->Branch("b2p0x",&b2p0x,"b2p0x/D");
            TBranch *obb2p0tx = tOut->Branch("b2p0tx",&b2p0tx,"b2p0tx/D");
            TBranch *obb2p0y = tOut->Branch("b2p0y",&b2p0y,"b2p0y/D");
            TBranch *obb2p0ty = tOut->Branch("b2p0ty",&b2p0ty,"b2p0ty/D");
            TBranch *obb2p0t = tOut->Branch("b2p0t",&b2p0t,"b2p0t/D");
            TBranch *obb2p0z = tOut->Branch("b2p0z",&b2p0z,"b2p0z/D");
            TBranch *obb2p0TP = tOut->Branch("b2p0TP",&b2p0TP,"b2p0TP/D");
            TBranch *obb2p0dE = tOut->Branch("b2p0dE",&b2p0dE,"b2p0dE/D");
            TBranch *obb2p0Ekin = tOut->Branch("b2p0Ekin",&b2p0Ekin,"b2p0Ekin/D");
            TBranch *obb2p0R_pz = tOut->Branch("b2p0R_pz",&b2p0R_pz,"b2p0R_pz/D");
            TBranch *obb2p0R_exen = tOut->Branch("b2p0R_exen",&b2p0R_exen,"b2p0R_exen/D");
            TBranch *obb2p0R_exen2 = tOut->Branch("b2p0R_exen2",&b2p0R_exen2,"b2p0R_exen2/D");
            TBranch *obb2p0R_ph = tOut->Branch("b2p0R_ph",&b2p0R_ph,"b2p0R_ph/D");
            TBranch *obb2p0R_ph2 = tOut->Branch("b2p0R_ph2",&b2p0R_ph2,"b2p0R_ph2/D");
            TBranch *obb2p0R_th = tOut->Branch("b2p0R_th",&b2p0R_th,"b2p0R_th/D");
            TBranch *obb2p0R_th2 = tOut->Branch("b2p0R_th2",&b2p0R_th2,"b2p0R_th2/D");
            TBranch *obb7p0x = tOut->Branch("b7p0x",&b4p0x,"b7p0x/D");
            TBranch *obb7p0tx = tOut->Branch("b7p0tx",&b4p0tx,"b7p0tx/D");
            TBranch *obb7p0y = tOut->Branch("b7p0y",&b4p0y,"b7p0y/D");
            TBranch *obb7p0ty = tOut->Branch("b7p0ty",&b4p0ty,"b7p0ty/D");
            TBranch *obb7p0t = tOut->Branch("b7p0t",&b4p0t,"b7p0t/D");
            TBranch *obb7p0Ekin = tOut->Branch("b7p0Ekin",&b4p0Ekin,"b7p0Ekin/D");
            TBranch *obb9p0x = tOut->Branch("b9p0x",&b6p0x,"b9p0x/D");
            TBranch *obb9p0tx = tOut->Branch("b9p0tx",&b6p0tx,"b9p0tx/D");
            TBranch *obb9p0y = tOut->Branch("b9p0y",&b6p0y,"b9p0y/D");
            TBranch *obb9p0ty = tOut->Branch("b9p0ty",&b6p0ty,"b9p0ty/D");
            TBranch *obb9p0Ekin = tOut->Branch("b9p0Ekin",&b6p0Ekin,"b9p0Ekin/D");
            TBranch *obb13p1t = tOut->Branch("b13p1t",&b9p1t,"b13p1t/D");
            TBranch *obb13p1x = tOut->Branch("b13p1x",&b9p1x,"b13p1x/D");
            TBranch *obb13p1y = tOut->Branch("b13p1y",&b9p1y,"b13p1y/D");
            TBranch *obb13p1z = tOut->Branch("b13p1z",&b9p1z,"b13p1z/D");
            TBranch *obb13p1Ekin = tOut->Branch("b13p1Ekin",&b9p1Ekin,"b13p1Ekin/D");
            TBranch *obb13p2t = tOut->Branch("b13p2t",&b9p2t,"b13p2t/D");
            TBranch *obb13p2x = tOut->Branch("b13p2x",&b9p2x,"b13p2x/D");
            TBranch *obb13p2y = tOut->Branch("b13p2y",&b9p2y,"b13p2y/D");
            TBranch *obb13p2z = tOut->Branch("b13p2z",&b9p2z,"b13p2z/D");
            TBranch *obb13p2Ekin = tOut->Branch("b13p2Ekin",&b9p2Ekin,"b13p2Ekin/D");

            // And add in the new GEANT parameters... note that they are simply pg instead of p
            tOut->Branch("b13pn1gmultiplicity",&mh1nmultiplicity,"b13pn1gmultiplicity/I");
            tOut->Branch("b13pn1gZ1Mult",&mh1_z1,"b13pn1gZ1Mult/I");//zwk particle mults
            tOut->Branch("b13pn1gZ2Mult",&mh1_z2,"b13pn1gZ2Mult/I");
            tOut->Branch("b13pn1gZGt2Mult",&mh1_zgt2,"b13pn1gZGt2Mult/I");
            tOut->Branch("b13pn1gGammaMult",&mh1_g,"b13pn1gGammaMult/I");
            tOut->Branch("b13pn1g1Edep",&mh1n1Edep,"b13pn1g1Edep/D");
            tOut->Branch("b13pn1g1light",&mh1n1light,"b13pn1g1light/D");
            tOut->Branch("b13pn1g1t",&mh1n1t,"b13pn1g1t/D");
            tOut->Branch("b13pn1g1x",&mh1n1x,"b13pn1g1x/D");
            tOut->Branch("b13pn1g1y",&mh1n1y,"b13pn1g1y/D");
            tOut->Branch("b13pn1g1z",&mh1n1z,"b13pn1g1z/D");
            tOut->Branch("b13pn1g2Edep",&mh1n2Edep,"b13pn1g2Edep/D");
            tOut->Branch("b13pn1g2light",&mh1n2light,"b13pn1g2light/D");
            tOut->Branch("b13pn1g2t",&mh1n2t,"b13pn1g2t/D");
            tOut->Branch("b13pn1g2x",&mh1n2x,"b13pn1g2x/D");
            tOut->Branch("b13pn1g2y",&mh1n2y,"b13pn1g2y/D");
            tOut->Branch("b13pn1g2z",&mh1n2z,"b13pn1g2z/D");
            tOut->Branch("b13pn1g3Edep",&mh1n3Edep,"b13pn1g3Edep/D");
            tOut->Branch("b13pn1g3light",&mh1n3light,"b13pn1g3light/D");
            tOut->Branch("b13pn1g3t",&mh1n3t,"b13pn1g3t/D");
            tOut->Branch("b13pn1g3x",&mh1n3x,"b13pn1g3x/D");
            tOut->Branch("b13pn1g3y",&mh1n3y,"b13pn1g3y/D");
            tOut->Branch("b13pn1g3z",&mh1n3z,"b13pn1g3z/D");
            tOut->Branch("b13pn1g4Edep",&mh1n4Edep,"b13pn1g4Edep/D");
            tOut->Branch("b13pn1g4light",&mh1n4light,"b13pn1g4light/D");
            tOut->Branch("b13pn1g4t",&mh1n4t,"b13pn1g4t/D");
            tOut->Branch("b13pn1g4x",&mh1n4x,"b13pn1g4x/D");
            tOut->Branch("b13pn1g4y",&mh1n4y,"b13pn1g4y/D");
            tOut->Branch("b13pn1g4z",&mh1n4z,"b13pn1g4z/D");
            tOut->Branch("b13pn1g5Edep",&mh1n5Edep,"b13pn1g5Edep/D");
            tOut->Branch("b13pn1g5light",&mh1n5light,"b13pn1g5light/D");
            tOut->Branch("b13pn1g5t",&mh1n5t,"b13pn1g5t/D");
            tOut->Branch("b13pn1g5x",&mh1n5x,"b13pn1g5x/D");
            tOut->Branch("b13pn1g5y",&mh1n5y,"b13pn1g5y/D");
            tOut->Branch("b13pn1g5z",&mh1n5z,"b13pn1g5z/D");
            tOut->Branch("b13pn1g6Edep",&mh1n6Edep,"b13pn1g6Edep/D");
            tOut->Branch("b13pn1g6light",&mh1n6light,"b13pn1g6light/D");
            tOut->Branch("b13pn1g6t",&mh1n6t,"b13pn1g6t/D");
            tOut->Branch("b13pn1g6x",&mh1n6x,"b13pn1g6x/D");
            tOut->Branch("b13pn1g6y",&mh1n6y,"b13pn1g6y/D");
            tOut->Branch("b13pn1g6z",&mh1n6z,"b13pn1g6z/D");
            tOut->Branch("b13pn1g7Edep",&mh1n7Edep,"b13pn1g7Edep/D");
            tOut->Branch("b13pn1g7light",&mh1n7light,"b13pn1g7light/D");
            tOut->Branch("b13pn1g7t",&mh1n7t,"b13pn1g7t/D");
            tOut->Branch("b13pn1g7x",&mh1n7x,"b13pn1g7x/D");
            tOut->Branch("b13pn1g7y",&mh1n7y,"b13pn1g7y/D");
            tOut->Branch("b13pn1g7z",&mh1n7z,"b13pn1g7z/D");
            tOut->Branch("b13pn1g8Edep",&mh1n8Edep,"b13pn1g8Edep/D");
            tOut->Branch("b13pn1g8light",&mh1n8light,"b13pn1g8light/D");
            tOut->Branch("b13pn1g8t",&mh1n8t,"b13pn1g8t/D");
            tOut->Branch("b13pn1g8x",&mh1n8x,"b13pn1g8x/D");
            tOut->Branch("b13pn1g8y",&mh1n8y,"b13pn1g8y/D");
            tOut->Branch("b13pn1g8z",&mh1n8z,"b13pn1g8z/D");
            tOut->Branch("b13pn1g9Edep",&mh1n9Edep,"b13pn1g9Edep/D");
            tOut->Branch("b13pn1g9light",&mh1n9light,"b13pn1g9light/D");
            tOut->Branch("b13pn1g9t",&mh1n9t,"b13pn1g9t/D");
            tOut->Branch("b13pn1g9x",&mh1n9x,"b13pn1g9x/D");
            tOut->Branch("b13pn1g9y",&mh1n9y,"b13pn1g9y/D");
            tOut->Branch("b13pn1g9z",&mh1n9z,"b13pn1g9z/D");
            tOut->Branch("b13pn1g10Edep",&mh1n10Edep,"b13pn1g10Edep/D");
            tOut->Branch("b13pn1g10light",&mh1n10light,"b13pn1g10light/D");
            tOut->Branch("b13pn1g10t",&mh1n10t,"b13pn1g10t/D");
            tOut->Branch("b13pn1g10x",&mh1n10x,"b13pn1g10x/D");
            tOut->Branch("b13pn1g10y",&mh1n10y,"b13pn1g10y/D");
            tOut->Branch("b13pn1g10z",&mh1n10z,"b13pn1g10z/D");

            // And add in the new GEANT parameters... note that they are simply pg instead of p
            tOut->Branch("b13pn2gmultiplicity",&mh2nmultiplicity,"b13pn2gmultiplicity/I");
            tOut->Branch("b13pn2gZ1Mult",&mh2_z1,"b13pn1gZ1Mult/I");//zwk particle mults
            tOut->Branch("b13pn2gZ2Mult",&mh2_z2,"b13pn1gZ2Mult/I");
            tOut->Branch("b13pn2gZGt2Mult",&mh2_zgt2,"b13pn1gZGt2Mult/I");
            tOut->Branch("b13pn2gGammaMult",&mh2_g,"b13pn1gGammaMult/I");
            tOut->Branch("b13pn2g1Edep",&mh2n1Edep,"b13pn2g1Edep/D");
            tOut->Branch("b13pn2g1light",&mh2n1light,"b13pn2g1light/D");
            tOut->Branch("b13pn2g1t",&mh2n1t,"b13pn2g1t/D");
            tOut->Branch("b13pn2g1x",&mh2n1x,"b13pn2g1x/D");
            tOut->Branch("b13pn2g1y",&mh2n1y,"b13pn2g1y/D");
            tOut->Branch("b13pn2g1z",&mh2n1z,"b13pn2g1z/D");
            tOut->Branch("b13pn2g2Edep",&mh2n2Edep,"b13pn2g2Edep/D");
            tOut->Branch("b13pn2g2light",&mh2n2light,"b13pn2g2light/D");
            tOut->Branch("b13pn2g2t",&mh2n2t,"b13pn2g2t/D");
            tOut->Branch("b13pn2g2x",&mh2n2x,"b13pn2g2x/D");
            tOut->Branch("b13pn2g2y",&mh2n2y,"b13pn2g2y/D");
            tOut->Branch("b13pn2g2z",&mh2n2z,"b13pn2g2z/D");
            tOut->Branch("b13pn2g3Edep",&mh2n3Edep,"b13pn2g3Edep/D");
            tOut->Branch("b13pn2g3light",&mh2n3light,"b13pn2g3light/D");
            tOut->Branch("b13pn2g3t",&mh2n3t,"b13pn2g3t/D");
            tOut->Branch("b13pn2g3x",&mh2n3x,"b13pn2g3x/D");
            tOut->Branch("b13pn2g3y",&mh2n3y,"b13pn2g3y/D");
            tOut->Branch("b13pn2g3z",&mh2n3z,"b13pn2g3z/D");
            tOut->Branch("b13pn2g4Edep",&mh2n4Edep,"b13pn2g4Edep/D");
            tOut->Branch("b13pn2g4light",&mh2n4light,"b13pn2g4light/D");
            tOut->Branch("b13pn2g4t",&mh2n4t,"b13pn2g4t/D");
            tOut->Branch("b13pn2g4x",&mh2n4x,"b13pn2g4x/D");
            tOut->Branch("b13pn2g4y",&mh2n4y,"b13pn2g4y/D");
            tOut->Branch("b13pn2g4z",&mh2n4z,"b13pn2g4z/D");
            tOut->Branch("b13pn2g5Edep",&mh2n5Edep,"b13pn2g5Edep/D");
            tOut->Branch("b13pn2g5light",&mh2n5light,"b13pn2g5light/D");
            tOut->Branch("b13pn2g5t",&mh2n5t,"b13pn2g5t/D");
            tOut->Branch("b13pn2g5x",&mh2n5x,"b13pn2g5x/D");
            tOut->Branch("b13pn2g5y",&mh2n5y,"b13pn2g5y/D");
            tOut->Branch("b13pn2g5z",&mh2n5z,"b13pn2g5z/D");
            tOut->Branch("b13pn2g6Edep",&mh2n6Edep,"b13pn2g6Edep/D");
            tOut->Branch("b13pn2g6light",&mh2n6light,"b13pn2g6light/D");
            tOut->Branch("b13pn2g6t",&mh2n6t,"b13pn2g6t/D");
            tOut->Branch("b13pn2g6x",&mh2n6x,"b13pn2g6x/D");
            tOut->Branch("b13pn2g6y",&mh2n6y,"b13pn2g6y/D");
            tOut->Branch("b13pn2g6z",&mh2n6z,"b13pn2g6z/D");
            tOut->Branch("b13pn2g7Edep",&mh2n7Edep,"b13pn2g7Edep/D");
            tOut->Branch("b13pn2g7light",&mh2n7light,"b13pn2g7light/D");
            tOut->Branch("b13pn2g7t",&mh2n7t,"b13pn2g7t/D");
            tOut->Branch("b13pn2g7x",&mh2n7x,"b13pn2g7x/D");
            tOut->Branch("b13pn2g7y",&mh2n7y,"b13pn2g7y/D");
            tOut->Branch("b13pn2g7z",&mh2n7z,"b13pn2g7z/D");
            tOut->Branch("b13pn2g8Edep",&mh2n8Edep,"b13pn2g8Edep/D");
            tOut->Branch("b13pn2g8light",&mh2n8light,"b13pn2g8light/D");
            tOut->Branch("b13pn2g8t",&mh2n8t,"b13pn2g8t/D");
            tOut->Branch("b13pn2g8x",&mh2n8x,"b13pn2g8x/D");
            tOut->Branch("b13pn2g8y",&mh2n8y,"b13pn2g8y/D");
            tOut->Branch("b13pn2g8z",&mh2n8z,"b13pn2g8z/D");
            tOut->Branch("b13pn2g9Edep",&mh2n9Edep,"b13pn2g9Edep/D");
            tOut->Branch("b13pn2g9light",&mh2n9light,"b13pn2g9light/D");
            tOut->Branch("b13pn2g9t",&mh2n9t,"b13pn2g9t/D");
            tOut->Branch("b13pn2g9x",&mh2n9x,"b13pn2g9x/D");
            tOut->Branch("b13pn2g9y",&mh2n9y,"b13pn2g9y/D");
            tOut->Branch("b13pn2g9z",&mh2n9z,"b13pn2g9z/D");
            tOut->Branch("b13pn2g10Edep",&mh2n10Edep,"b13pn2g10Edep/D");
            tOut->Branch("b13pn2g10light",&mh2n10light,"b13pn2g10light/D");
            tOut->Branch("b13pn2g10t",&mh2n10t,"b13pn2g10t/D");
            tOut->Branch("b13pn2g10x",&mh2n10x,"b13pn2g10x/D");
            tOut->Branch("b13pn2g10y",&mh2n10y,"b13pn2g10y/D");
            tOut->Branch("b13pn2g10z",&mh2n10z,"b13pn2g10z/D");

	double g1light,g2light,g3light,g4light,g5light,g6light,g7light,g8light,g9light,g10light,g11light,g12light,g13light,g14light,g15light,g16light,g17light,g18light,g19light,g20light;
	double g1Edep,g2Edep,g3Edep,g4Edep,g5Edep,g6Edep,g7Edep,g8Edep,g9Edep,g10Edep,g11Edep,g12Edep,g13Edep,g14Edep,g15Edep,g16Edep,g17Edep,g18Edep,g19Edep,g20Edep;
	double g1t,g2t,g3t,g4t,g5t,g6t,g7t,g8t,g9t,g10t,g11t,g12t,g13t,g14t,g15t,g16t,g17t,g18t,g19t,g20t;
	double g1x,g2x,g3x,g4x,g5x,g6x,g7x,g8x,g9x,g10x,g11x,g12x,g13x,g14x,g15x,g16x,g17x,g18x,g19x,g20x;
	double g1y,g2y,g3y,g4y,g5y,g6y,g7y,g8y,g9y,g10y,g11y,g12y,g13y,g14y,g15y,g16y,g17y,g18y,g19y,g20y;
	double g1z,g2z,g3z,g4z,g5z,g6z,g7z,g8z,g9z,g10z,g11z,g12z,g13z,g14z,g15z,g16z,g17z,g18z,g19z,g20z;

	string name[] = {"b13pg1","b13pg2","b13pg3","b13pg4","b13pg5","b13pg6","b13pg7","b13pg8","b13pg9","b13pg10","b13pg11","b13pg12","b13pg13","b13pg14","b13pg15","b13pg16","b13pg17","b13pg18","b13pg19","b13pg20"};	
	double* ps_light[] = {&g1light,&g2light,&g3light,&g4light,&g5light,&g6light,&g7light,&g8light,&g9light,&g10light,&g11light,&g12light,&g13light,&g14light,&g15light,&g16light,&g17light,&g18light,&g19light,&g20light};
	double* ps_Edep[] = {&g1Edep,&g2Edep,&g3Edep,&g4Edep,&g5Edep,&g6Edep,&g7Edep,&g8Edep,&g9Edep,&g10Edep,&g11Edep,&g12Edep,&g13Edep,&g14Edep,&g15Edep,&g16Edep,&g17Edep,&g18Edep,&g19Edep,&g20Edep};
	double* ps_t[] = {&g1t,&g2t,&g3t,&g4t,&g5t,&g6t,&g7t,&g8t,&g9t,&g10t,&g11t,&g12t,&g13t,&g14t,&g15t,&g16t,&g17t,&g18t,&g19t,&g20t};
	double* ps_x[] = {&g1x,&g2x,&g3x,&g4x,&g5x,&g6x,&g7x,&g8x,&g9x,&g10x,&g11x,&g12x,&g13x,&g14x,&g15x,&g16x,&g17x,&g18x,&g19x,&g20x};
	double* ps_y[] = {&g1y,&g2y,&g3y,&g4y,&g5y,&g6y,&g7y,&g8y,&g9y,&g10y,&g11y,&g12y,&g13y,&g14y,&g15y,&g16y,&g17y,&g18y,&g19y,&g20y};
	double* ps_z[] = {&g1z,&g2z,&g3z,&g4z,&g5z,&g6z,&g7z,&g8z,&g9z,&g10z,&g11z,&g12z,&g13z,&g14z,&g15z,&g16z,&g17z,&g18z,&g19z,&g20z};

	for(int l = 0; l < 20; l++) {
	string first_Edep = name[l]+"Edep";
	string first_light = name[l]+"light";
	string first_t = name[l]+"t";
	string first_x = name[l]+"x";
	string first_y = name[l]+"y";
	string first_z = name[l]+"z";	
	string second_Edep = name[l]+"Edep/D";
	string second_light = name[l]+"light/D";
	string second_t = name[l]+"t/D";
	string second_x = name[l]+"x/D";
	string second_y = name[l]+"y/D";
	string second_z = name[l]+"z/D";

	tOut->Branch(first_Edep.c_str(),ps_Edep[l],second_Edep.c_str());
        tOut->Branch(first_light.c_str(),ps_light[l],second_light.c_str());
        tOut->Branch(first_t.c_str(),ps_t[l],second_t.c_str());
        tOut->Branch(first_x.c_str(),ps_x[l],second_x.c_str());
        tOut->Branch(first_y.c_str(),ps_y[l],second_y.c_str());
        tOut->Branch(first_z.c_str(),ps_z[l],second_z.c_str());
	}

            // Now loop over the files and merge them into the output file we want
            // but first check for length
            if (outputn1_nevents != outputn2_nevents)
            {
                    std::cout << "GEANT first neutron has different number of events from the second neutron! This is disallowed and I am exiting\n";
                    return 1;
            
            } else if (input_nevent > outputn1_nevents)
            {
                std::cout << "ST MONA output had more events than GEANT output (" << input_nevent << " vs. " << outputn1_nevents << ") so I am only merging until I hit the end of the GEANT output.\n";
                for (Int_t i = 0; i < outputn1_nevents; i++)
                {
                    t->GetEntry(i);
                    tn1Temp->GetEntry(i);
                    tn2Temp->GetEntry(i);
                    tOut->Fill();
                }
            } else if (input_nevent < outputn1_nevents )
            {
                std::cout << "GEANT output had MORE events than ST MONA output! This is disallowed and I am exiting\n";
                return 1;
            } else
            {
                std::cout << "ST MONA and GEANT outputs have same number of events. I am ecstatic and am merging all the data.\n";
                for (Int_t i = 0; i < outputn1_nevents; i++)
                {
                t->GetEntry(i);
                tn1Temp->GetEntry(i);
                tn2Temp->GetEntry(i);
		    
		double total_light[] = {mh1n1light,mh1n2light,mh1n3light,mh1n4light,mh1n5light,mh1n6light,mh1n7light,mh1n8light,mh1n9light,mh1n10light,mh2n1light,mh2n2light,mh2n3light,mh2n4light,mh2n5light,mh2n6light,mh2n7light,mh2n8light,mh2n9light,mh2n10light};
	 	double total_Edep[] = {mh1n1Edep,mh1n2Edep,mh1n3Edep,mh1n4Edep,mh1n5Edep,mh1n6Edep,mh1n7Edep,mh1n8Edep,mh1n9Edep,mh1n10Edep,mh2n1Edep,mh2n2Edep,mh2n3Edep,mh2n4Edep,mh2n5Edep,mh2n6Edep,mh2n7Edep,mh2n8Edep,mh2n9Edep,mh2n10Edep};
		double total_t[] = {mh1n1t,mh1n2t,mh1n3t,mh1n4t,mh1n5t,mh1n6t,mh1n7t,mh1n8t,mh1n9t,mh1n10t,mh2n1t,mh2n2t,mh2n3t,mh2n4t,mh2n5t,mh2n6t,mh2n7t,mh2n8t,mh2n9t,mh2n10t};
		double total_x[] = {mh1n1x,mh1n2x,mh1n3x,mh1n4x,mh1n5x,mh1n6x,mh1n7x,mh1n8x,mh1n9x,mh1n10x,mh2n1x,mh2n2x,mh2n3x,mh2n4x,mh2n5x,mh2n6x,mh2n7x,mh2n8x,mh2n9x,mh2n10x};
		double total_y[] = {mh1n1y,mh1n2y,mh1n3y,mh1n4y,mh1n5y,mh1n6y,mh1n7y,mh1n8y,mh1n9y,mh1n10y,mh2n1y,mh2n2y,mh2n3y,mh2n4y,mh2n5y,mh2n6y,mh2n7y,mh2n8y,mh2n9y,mh2n10y};
		double total_z[] = {mh1n1z,mh1n2z,mh1n3z,mh1n4z,mh1n5z,mh1n6z,mh1n7z,mh1n8z,mh1n9z,mh1n10z,mh2n1z,mh2n2z,mh2n3z,mh2n4z,mh2n5z,mh2n6z,mh2n7z,mh2n8z,mh2n9z,mh2n10z};

		double final_light[20];
		double final_Edep[20];
		double final_t[20];
		double final_x[20];
		double final_y[20];
		double final_z[20];

		for(int m = 0; m < 20; m++) {

		double temp_t = 10000;
		int temp_n = -1;

		for(int n = 0; n < 20; n++) {
    		if (total_t[n]>0 && total_t[n]<temp_t)	{
			temp_t = total_t[n];
			temp_n = n;
			}
		}

		if (temp_n == -1 ) {
		for(int o = 0; o < 20; o++) {
    		if (total_Edep[o]==0 && total_light[o]==0 && total_t[o]==0 ) {
			temp_n = o;
			}
		}
		}
		final_Edep[m] = total_Edep[temp_n];
		final_light[m] = total_light[temp_n];
		final_t[m] = total_t[temp_n];
		final_x[m] = total_x[temp_n];
		final_y[m] = total_y[temp_n];
		final_z[m] = total_z[temp_n];

		total_Edep[temp_n] = 0;
		total_light[temp_n] = 0;
		total_t[temp_n] = 0;
		total_x[temp_n] = 0;
		total_y[temp_n] = 0;
		total_z[temp_n] = 0;

		}
		g1light = final_light[0];
		g1Edep = final_Edep[0];
		g1t = final_t[0];
		g1x = final_x[0];
		g1y = final_y[0];
		g1z = final_z[0];
		g2light = final_light[1];
		g2Edep = final_Edep[1];
		g2t = final_t[1];
		g2x = final_x[1];
		g2y = final_y[1];
		g2z = final_z[1];
		g3light = final_light[2];
		g3Edep = final_Edep[2];
		g3t = final_t[2];
		g3x = final_x[2];
		g3y = final_y[2];
		g3z = final_z[2];
		g4light = final_light[3];
		g4Edep = final_Edep[3];
		g4t = final_t[3];
		g4x = final_x[3];
		g4y = final_y[3];
		g4z = final_z[3];
		g5light = final_light[4];
		g5Edep = final_Edep[4];
		g5t = final_t[4];
		g5x = final_x[4];
		g5y = final_y[4];
		g5z = final_z[4];
		g6light = final_light[5];
		g6Edep = final_Edep[5];
		g6t = final_t[5];
		g6x = final_x[5];
		g6y = final_y[5];
		g6z = final_z[5];
		g7light = final_light[6];
		g7Edep = final_Edep[6];
		g7t = final_t[6];
		g7x = final_x[6];
		g7y = final_y[6];
		g7z = final_z[6];
		g8light = final_light[7];
		g8Edep = final_Edep[7];
		g8t = final_t[7];
		g8x = final_x[7];
		g8y = final_y[7];
		g8z = final_z[7];
		g9light = final_light[8];
		g9Edep = final_Edep[8];
		g9t = final_t[8];
		g9x = final_x[8];
		g9y = final_y[8];
		g9z = final_z[8];
		g10light = final_light[9];
		g10Edep = final_Edep[9];
		g10t = final_t[9];
		g10x = final_x[9];
		g10y = final_y[9];
		g10z = final_z[9];
		g11light = final_light[10];
		g11Edep = final_Edep[10];
		g11t = final_t[10];
		g11x = final_x[10];
		g11y = final_y[10];
		g11z = final_z[10];
		g12light = final_light[11];
		g12Edep = final_Edep[11];
		g12t = final_t[11];
		g12x = final_x[11];
		g12y = final_y[11];
		g12z = final_z[11];
		g13light = final_light[12];
		g13Edep = final_Edep[12];
		g13t = final_t[12];
		g13x = final_x[12];
		g13y = final_y[12];
		g13z = final_z[12];
		g14light = final_light[13];
		g14Edep = final_Edep[13];
		g14t = final_t[13];
		g14x = final_x[13];
		g14y = final_y[13];
		g14z = final_z[13];
		g15light = final_light[14];
		g15Edep = final_Edep[14];
		g15t = final_t[14];
		g15x = final_x[14];
		g15y = final_y[14];
		g15z = final_z[14];
		g16light = final_light[15];
		g16Edep = final_Edep[15];
		g16t = final_t[15];
		g16x = final_x[15];
		g16y = final_y[15];
		g16z = final_z[15];
		g17light = final_light[16];
		g17Edep = final_Edep[16];
		g17t = final_t[16];
		g17x = final_x[16];
		g17y = final_y[16];
		g17z = final_z[16];
		g18light = final_light[17];
		g18Edep = final_Edep[17];
		g18t = final_t[17];
		g18x = final_x[17];
		g18y = final_y[17];
		g18z = final_z[17];
		g19light = final_light[18];
		g19Edep = final_Edep[18];
		g19t = final_t[18];
		g19x = final_x[18];
		g19y = final_y[18];
		g19z = final_z[18];
		g20light = final_light[19];
		g20Edep = final_Edep[19];
		g20t = final_t[19];
		g20x = final_x[19];
		g20y = final_y[19];
		g20z = final_z[19];

                tOut->Fill();
                }
            }

            tOut->Write();
            inFile->Close();
            tempFile->Close();
            temp2File->Close();
            outFile->Close();
        }// End 2n 
	else if (nneuts == 1)
        {
            // Here is where 1 neutron stuff is done
            // Now we find the relevant branches to look at
	  Double_t b1p0x,b1p0tx,b1p0y,b1p0ty,b1p0Ekin,b2p0x,b2p0tx,b2p0y,b2p0ty,b2p0t,b2p0z,b2p0TP,b2p0dE,b2p0Ekin,b2p0R_pz,b2p0R_exen,b2p0R_ph,b2p0R_th,b4p0x,b4p0tx,b4p0y,b4p0ty,b4p0t,b4p0Ekin,b6p0Ekin,b6p0x,b6p0y,b6p0tx,b6p0ty,b9p1t,b9p1x,b9p1y,b9p1z,b9p1Ekin;
            // Make branches and set addresses
            string bn_old[] = {"b1p0x", "b1p0tx", "b1p0y", "b1p0ty", "b1p0Ekin",
                "b2p0x", "b2p0tx", "b2p0y", "b2p0ty", "b2p0t", "b2p0z", "b2p0TP", "b2p0dE", "b2p0Ekin",
		"b2p0R_pz","b2p0R_exen", "b2p0R_ph", "b2p0R_th",
                "b4p0x", "b4p0tx", "b4p0y", "b4p0ty", "b4p0t", "b4p0Ekin",
                "b6p0x", "b6p0tx", "b6p0y", "b6p0ty", "b6p0Ekin",
                "b9p1t", "b9p1x", "b9p1y", "b9p1z", "b9p1Ekin",
                };
            double* ps_old[] = {&b1p0x,&b1p0tx,&b1p0y,&b1p0ty,&b1p0Ekin,
                &b2p0x,&b2p0tx,&b2p0y,&b2p0ty,&b2p0t,&b2p0z,&b2p0TP,&b2p0dE,
	        &b2p0Ekin,&b2p0R_pz,&b2p0R_exen,&b2p0R_ph,&b2p0R_th,
                &b4p0x,&b4p0tx,&b4p0y,&b4p0ty,&b4p0t,&b4p0Ekin,
       	        &b6p0x,&b6p0tx,&b6p0y,&b6p0ty,&b6p0Ekin,
                &b9p1t,&b9p1x,&b9p1y,&b9p1z,&b9p1Ekin,
                };   
            for (size_t i(0); i < sizeof(ps_old)/sizeof(double*); i++) {
                t->SetBranchAddress(bn_old[i].c_str(),ps_old[i]);  
            }

            // Get number of entries in the file
            Int_t input_nevent = t->GetEntries();

            // Then open the GEANT output file
	    TString dname = dirdata+"/default"+uniqueID+".root";
            TFile *tempFile = new TFile(dname); assert(tempFile);
            TTree *tTemp = (TTree*)tempFile->Get("t"); assert(tTemp);

            Double_t n1Edep,n1light,n1t,n1x,n1y,n1z,n2Edep,n2light,n2t,n2x,n2y,n2z,n3Edep,n3light,n3t,n3x,n3y,n3z,n4Edep,n4light,n4t,n4x,n4y,n4z,n5Edep,n5light,n5t,n5x,n5y,n5z;
	    Double_t n6Edep,n6light,n6t,n6x,n6y,n6z,n7Edep,n7light,n7t,n7x,n7y,n7z,n8Edep,n8light,n8t,n8x,n8y,n8z,n9Edep,n9light,n9t,n9x,n9y,n9z,n10Edep,n10light,n10t,n10x,n10y,n10z;
            Int_t multiplicity;
            Int_t m_z1; //zwk particle multiplicities
            Int_t m_z2;
            Int_t m_zgt2;
            Int_t m_g;

            // Make branches and set addresses
            string bn_n1[] = {"MoNA.Hit.1.e","MoNA.Hit.1.light","MoNA.Hit.1.t","MoNA.Hit.1.x","MoNA.Hit.1.y","MoNA.Hit.1.z",
                "MoNA.Hit.2.e","MoNA.Hit.2.light","MoNA.Hit.2.t","MoNA.Hit.2.x","MoNA.Hit.2.y","MoNA.Hit.2.z",
                "MoNA.Hit.3.e","MoNA.Hit.3.light","MoNA.Hit.3.t","MoNA.Hit.3.x","MoNA.Hit.3.y","MoNA.Hit.3.z",
                "MoNA.Hit.4.e","MoNA.Hit.4.light","MoNA.Hit.4.t","MoNA.Hit.4.x","MoNA.Hit.4.y","MoNA.Hit.4.z",
                "MoNA.Hit.5.e","MoNA.Hit.5.light","MoNA.Hit.5.t","MoNA.Hit.5.x","MoNA.Hit.5.y","MoNA.Hit.5.z",
                "MoNA.Hit.6.e","MoNA.Hit.6.light","MoNA.Hit.6.t","MoNA.Hit.6.x","MoNA.Hit.6.y","MoNA.Hit.6.z",
                "MoNA.Hit.7.e","MoNA.Hit.7.light","MoNA.Hit.7.t","MoNA.Hit.7.x","MoNA.Hit.7.y","MoNA.Hit.7.z",
                "MoNA.Hit.8.e","MoNA.Hit.8.light","MoNA.Hit.8.t","MoNA.Hit.8.x","MoNA.Hit.8.y","MoNA.Hit.8.z",
                "MoNA.Hit.9.e","MoNA.Hit.9.light","MoNA.Hit.9.t","MoNA.Hit.9.x","MoNA.Hit.9.y","MoNA.Hit.9.z",
                "MoNA.Hit.10.e","MoNA.Hit.10.light","MoNA.Hit.10.t","MoNA.Hit.10.x","MoNA.Hit.10.y","MoNA.Hit.10.z",
                };

            double* ps_n1[] = {&n1Edep,&n1light,&n1t,&n1x,&n1y,&n1z,
                &n2Edep,&n2light,&n2t,&n2x,&n2y,&n2z,
                &n3Edep,&n3light,&n3t,&n3x,&n3y,&n3z,
                &n4Edep,&n4light,&n4t,&n4x,&n4y,&n4z,
                &n5Edep,&n5light,&n5t,&n5x,&n5y,&n5z,
                &n6Edep,&n6light,&n6t,&n6x,&n6y,&n6z,
                &n7Edep,&n7light,&n7t,&n7x,&n7y,&n7z,
                &n8Edep,&n8light,&n8t,&n8x,&n8y,&n8z,
                &n9Edep,&n9light,&n9t,&n9x,&n9y,&n9z,
                &n10Edep,&n10light,&n10t,&n10x,&n10y,&n10z,
                };

            for (size_t i(0); i < sizeof(ps_n1)/sizeof(double*); i++) {
                tTemp->SetBranchAddress(bn_n1[i].c_str(),ps_n1[i]);  
            }

            tTemp->SetBranchAddress("MoNA.multiplicity",&multiplicity);  
            tTemp->SetBranchAddress("MoNA.Z1mult",&m_z1);  
            tTemp->SetBranchAddress("MoNA.Z2mult",&m_z2);  
            tTemp->SetBranchAddress("MoNA.ZGt2mult",&m_zgt2);  
            tTemp->SetBranchAddress("MoNA.Gmult",&m_g);  


            // Get number of entries in the file
            Int_t outputn1_nevents = tTemp->GetEntries();

            // Finally open the output .root file
            TFile *outFile = new TFile(argv[2],"recreate"); assert(outFile);
            TTree *tOut = new TTree("t","Simulation Output"); assert(tOut);

            // We want to preserve the original st-mona parameters
            TBranch *obb1p0x = tOut->Branch("b1p0x",&b1p0x,"b1p0x/D");
            TBranch *obb1p0tx = tOut->Branch("b1p0tx",&b1p0tx,"b1p0tx/D");
            TBranch *obb1p0y = tOut->Branch("b1p0y",&b1p0y,"b1p0y/D");
            TBranch *obb1p0ty = tOut->Branch("b1p0ty",&b1p0ty,"b1p0ty/D");
            TBranch *obb1p0Ekin = tOut->Branch("b1p0Ekin",&b1p0Ekin,"b1p0Ekin/D");
            TBranch *obb2p0x = tOut->Branch("b2p0x",&b2p0x,"b2p0x/D");
            TBranch *obb2p0tx = tOut->Branch("b2p0tx",&b2p0tx,"b2p0tx/D");
            TBranch *obb2p0y = tOut->Branch("b2p0y",&b2p0y,"b2p0y/D");
            TBranch *obb2p0ty = tOut->Branch("b2p0ty",&b2p0ty,"b2p0ty/D");
            TBranch *obb2p0t = tOut->Branch("b2p0t",&b2p0t,"b2p0t/D");
            TBranch *obb2p0z = tOut->Branch("b2p0z",&b2p0z,"b2p0z/D");
            TBranch *obb2p0TP = tOut->Branch("b2p0TP",&b2p0TP,"b2p0TP/D");
            TBranch *obb2p0dE = tOut->Branch("b2p0dE",&b2p0dE,"b2p0dE/D");
            TBranch *obb2p0Ekin = tOut->Branch("b2p0Ekin",&b2p0Ekin,"b2p0Ekin/D");
            TBranch *obb2p0R_pz = tOut->Branch("b2p0R_pz",&b2p0R_pz,"b2p0R_pz/D");
            TBranch *obb2p0R_exen = tOut->Branch("b2p0R_exen",&b2p0R_exen,"b2p0R_exen/D");
            TBranch *obb2p0R_ph = tOut->Branch("b2p0R_ph",&b2p0R_ph,"b2p0R_ph/D");
            TBranch *obb2p0R_th = tOut->Branch("b2p0R_th",&b2p0R_th,"b2p0R_th/D");
            TBranch *obb7p0x = tOut->Branch("b7p0x",&b4p0x,"b7p0x/D");
            TBranch *obb7p0tx = tOut->Branch("b7p0tx",&b4p0tx,"b7p0tx/D");
            TBranch *obb7p0y = tOut->Branch("b7p0y",&b4p0y,"b7p0y/D");
            TBranch *obb7p0ty = tOut->Branch("b7p0ty",&b4p0ty,"b7p0ty/D");
            TBranch *obb7p0t = tOut->Branch("b7p0t",&b4p0t,"b7p0t/D");
            TBranch *obb7p0Ekin = tOut->Branch("b7p0Ekin",&b4p0Ekin,"b7p0Ekin/D");
	    TBranch *obb9p0x = tOut->Branch("b9p0x",&b6p0x,"b9p0x/D");
            TBranch *obb9p0tx = tOut->Branch("b9p0tx",&b6p0tx,"b9p0tx/D");
            TBranch *obb9p0y = tOut->Branch("b9p0y",&b6p0y,"b9p0y/D");
            TBranch *obb9p0ty = tOut->Branch("b9p0ty",&b6p0ty,"b9p0ty/D");
            TBranch *obb9p0Ekin = tOut->Branch("b9p0Ekin",&b6p0Ekin,"b9p0Ekin/D");
            TBranch *obb13p1t = tOut->Branch("b13p1t",&b9p1t,"b13p1t/D");
            TBranch *obb13p1x = tOut->Branch("b13p1x",&b9p1x,"b13p1x/D");
            TBranch *obb13p1y = tOut->Branch("b13p1y",&b9p1y,"b13p1y/D");
            TBranch *obb13p1z = tOut->Branch("b13p1z",&b9p1z,"b13p1z/D");
            TBranch *obb13p1Ekin = tOut->Branch("b13p1Ekin",&b9p1Ekin,"b13p1Ekin/D");

            // And add in the new GEANT parameters... note that they are simply pg instead of p
            tOut->Branch("b13pgmultiplicity",&multiplicity,"b13pgmultiplicity/I");
            tOut->Branch("b13pg1gZ1Mult",&m_z1,"b13pg1gZ1Mult/I");//zwk particle mults
            tOut->Branch("b13pg1gZ2Mult",&m_z2,"b13pg1gZ2Mult/I");
            tOut->Branch("b13pg1gZGt2Mult",&m_zgt2,"b13pg1gZGt2Mult/I");
            tOut->Branch("b13pg1gGammaMult",&m_g,"b13pg1gGammaMult/I");
            tOut->Branch("b13pg1Edep",&n1Edep,"b13pg1Edep/D");
            tOut->Branch("b13pg1light",&n1light,"b13pg1light/D");
            tOut->Branch("b13pg1t",&n1t,"b13pg1t/D");
            tOut->Branch("b13pg1x",&n1x,"b13pg1x/D");
            tOut->Branch("b13pg1y",&n1y,"b13pg1y/D");
            tOut->Branch("b13pg1z",&n1z,"b13pg1z/D");
            tOut->Branch("b13pg2Edep",&n2Edep,"b13pg2Edep/D");
            tOut->Branch("b13pg2light",&n2light,"b13pg2light/D");
            tOut->Branch("b13pg2t",&n2t,"b13pg2t/D");
            tOut->Branch("b13pg2x",&n2x,"b13pg2x/D");
            tOut->Branch("b13pg2y",&n2y,"b13pg2y/D");
            tOut->Branch("b13pg2z",&n2z,"b13pg2z/D");
            tOut->Branch("b13pg3Edep",&n3Edep,"b13pg3Edep/D");
            tOut->Branch("b13pg3light",&n3light,"b13pg3light/D");
            tOut->Branch("b13pg3t",&n3t,"b13pg3t/D");
            tOut->Branch("b13pg3x",&n3x,"b13pg3x/D");
            tOut->Branch("b13pg3y",&n3y,"b13pg3y/D");
            tOut->Branch("b13pg3z",&n3z,"b13pg3z/D");
            tOut->Branch("b13pg4Edep",&n4Edep,"b13pg4Edep/D");
            tOut->Branch("b13pg4light",&n4light,"b13pg4light/D");
            tOut->Branch("b13pg4t",&n4t,"b13pg4t/D");
            tOut->Branch("b13pg4x",&n4x,"b13pg4x/D");
            tOut->Branch("b13pg4y",&n4y,"b13pg4y/D");
            tOut->Branch("b13pg4z",&n4z,"b13pg4z/D");
            tOut->Branch("b13pg5Edep",&n5Edep,"b13pg5Edep/D");
            tOut->Branch("b13pg5light",&n5light,"b13pg5light/D");
            tOut->Branch("b13pg5t",&n5t,"b13pg5t/D");
            tOut->Branch("b13pg5x",&n5x,"b13pg5x/D");
            tOut->Branch("b13pg5y",&n5y,"b13pg5y/D");
            tOut->Branch("b13pg5z",&n5z,"b13pg5z/D");
            tOut->Branch("b13pg6Edep",&n6Edep,"b13pg6Edep/D");
            tOut->Branch("b13pg6light",&n6light,"b13pg6light/D");
            tOut->Branch("b13pg6t",&n6t,"b13pg6t/D");
            tOut->Branch("b13pg6x",&n6x,"b13pg6x/D");
            tOut->Branch("b13pg6y",&n6y,"b13pg6y/D");
            tOut->Branch("b13pg6z",&n6z,"b13pg6z/D");
            tOut->Branch("b13pg7Edep",&n7Edep,"b13pg7Edep/D");
            tOut->Branch("b13pg7light",&n7light,"b13pg7light/D");
            tOut->Branch("b13pg7t",&n7t,"b13pg7t/D");
            tOut->Branch("b13pg7x",&n7x,"b13pg7x/D");
            tOut->Branch("b13pg7y",&n7y,"b13pg7y/D");
            tOut->Branch("b13pg7z",&n7z,"b13pg7z/D");
            tOut->Branch("b13pg8Edep",&n8Edep,"b13pg8Edep/D");
            tOut->Branch("b13pg8light",&n8light,"b13pg8light/D");
            tOut->Branch("b13pg8t",&n8t,"b13pg8t/D");
            tOut->Branch("b13pg8x",&n8x,"b13pg8x/D");
            tOut->Branch("b13pg8y",&n8y,"b13pg8y/D");
            tOut->Branch("b13pg8z",&n8z,"b13pg8z/D");
            tOut->Branch("b13pg9Edep",&n9Edep,"b13pg9Edep/D");
            tOut->Branch("b13pg9light",&n9light,"b13pg9light/D");
            tOut->Branch("b13pg9t",&n9t,"b13pg9t/D");
            tOut->Branch("b13pg9x",&n9x,"b13pg9x/D");
            tOut->Branch("b13pg9y",&n9y,"b13pg9y/D");
            tOut->Branch("b13pg9z",&n9z,"b13pg9z/D");
            tOut->Branch("b13pg10Edep",&n10Edep,"b13pg10Edep/D");
            tOut->Branch("b13pg10light",&n10light,"b13pg10light/D");
            tOut->Branch("b13pg10t",&n10t,"b13pg10t/D");
            tOut->Branch("b13pg10x",&n10x,"b13pg10x/D");
            tOut->Branch("b13pg10y",&n10y,"b13pg10y/D");
            tOut->Branch("b13pg10z",&n10z,"b13pg10z/D");

            // Now loop over the files and merge them into the output file we want
            // but first check for length
            if (input_nevent > outputn1_nevents)
            {
                std::cout << "ST MONA output had more events than GEANT output (" << input_nevent << " vs. " << outputn1_nevents << ") so I am only merging until I hit the end of the GEANT output.\n";
                for (Int_t i = 0; i < outputn1_nevents; i++)
                {
                    t->GetEntry(i);
                    tTemp->GetEntry(i);
                    tOut->Fill();
                }
            } else if (input_nevent < outputn1_nevents )
            {
                std::cout << "GEANT output had MORE events than ST MONA output! This is disallowed and I am exiting\n";
                return 1;
            } else
            {
                std::cout << "ST MONA and GEANT outputs have same number of events. I am pleased and am merging all the data.\n";
                for (Int_t i = 0; i < outputn1_nevents; i++)
                {
                    t->GetEntry(i);
                    tTemp->GetEntry(i);
                    tOut->Fill();
                }
            }

            tOut->Write();
            inFile->Close();
            tempFile->Close();
            outFile->Close();
        } else
        {
            std::cout << "Bad number of neutrons caught by merge. I am exiting\n";
            return 1;
        }
    }

    std::cout << "Merging Complete!\n";
    return 0;
}
