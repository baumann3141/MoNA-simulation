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
  if ( argc != 6 ) // argc should be 4 for correct execution
    // We print argv[0] assuming it is the program name
    std::cout<<"usage: "<< argv[0] <<" <inputfilename> <nneuts> <tgtID> <uniqueID> <DataDir>\n";
  else 
    {
      //now get unique ID
      TString uniqueID = argv[4];
      TString dirdata = argv[5];

      std::cout << "args are " << argv[1] << " " << argv[2] << std::endl;
      // First open up the root file and the temporary pythia file needed for Geant
      TFile *inFile = new TFile(argv[1]); assert(inFile);
      TTree *t = (TTree*)inFile->Get("t"); assert(t);

      TString ofile = dirdata + "/temp"+uniqueID+".indat";
      TString mfile = dirdata + "/temp"+uniqueID+".mac";
      ofstream outFile(ofile.Data()); // pythia input file
      ofstream macroFile(mfile.Data()); // macro file to run geant in batch mode

      Int_t nneuts = atoi(argv[2]);
      Int_t slice  = atoi(argv[3]);

      cout << "Be " << slice << " was active." << endl;

       if (nneuts == 4)
        {
		cout << "ENTERED THE 4N IF STATEMENT!" << endl;
	  TString nfile2 = dirdata + "/neutron2temp"+uniqueID+".indat";
	  TString nfile3 = dirdata + "/neutron3temp"+uniqueID+".indat";
	  TString nfile4 = dirdata + "/neutron4temp"+uniqueID+".indat";

	  ofstream outN2File(nfile2.Data());
	  ofstream outN3File(nfile3.Data());
	  ofstream outN4File(nfile4.Data());
	  // Now we find the relevant branches to look at
      cout << "Before setting branch address" << endl;
	  Double_t ekin1,ekin2,ekin3,ekin4,x1,x2,x3,x4,y1,y2,y3,y4,z1,z2,z3,z4;
	  TBranch *bekin1 = t->GetBranch("b9p1Ekin");
	  TBranch *bekin2 = t->GetBranch("b9p2Ekin");
	  TBranch *bekin3 = t->GetBranch("b9p3Ekin");
	  TBranch *bekin4 = t->GetBranch("b9p4Ekin");
	
	  TBranch *bx1 = t->GetBranch("b9p1x");
	  TBranch *bx2 = t->GetBranch("b9p2x");
	  TBranch *bx3 = t->GetBranch("b9p3x");
	  TBranch *bx4 = t->GetBranch("b9p4x");

	  TBranch *by1 = t->GetBranch("b9p1y");
	  TBranch *by2 = t->GetBranch("b9p2y");
	  TBranch *by3 = t->GetBranch("b9p3y");
	  TBranch *by4 = t->GetBranch("b9p4y");

	  TBranch *bz1 = t->GetBranch("b9p1z");
	  TBranch *bz2 = t->GetBranch("b9p2z");
	  TBranch *bz3 = t->GetBranch("b9p3z");
	  TBranch *bz4 = t->GetBranch("b9p4z");

      cout << "Got branches" << endl;

	  // Set addresses
	  bekin1->SetAddress(&ekin1);
	  bekin2->SetAddress(&ekin2);
	  bekin3->SetAddress(&ekin3);
	  bekin4->SetAddress(&ekin4);

      cout << "Ekin branches" << endl;

	  bx1->SetAddress(&x1);
	  bx2->SetAddress(&x2);
	  bx3->SetAddress(&x3);
	  bx4->SetAddress(&x4);

	  by1->SetAddress(&y1);
	  by2->SetAddress(&y2);
   	  by3->SetAddress(&y3);
	  by4->SetAddress(&y4);


	  bz1->SetAddress(&z1);
	  bz2->SetAddress(&z2);
	  bz3->SetAddress(&z3);
	  bz4->SetAddress(&z4);

      cout << "Set Addresses " << endl;

	  // Get number of entries in the file
	  Int_t nevent = t->GetEntries();
	  cout << "Events to be passed to Geant4 = " << nevent << endl;
	  // Tell the geant macro how many events it will need to run
	  //macroFile << "/tracking/verbose 2" << std::endl;
	  macroFile << "/run/beamOn " << nevent << std::endl;
	  macroFile.close();

	  // Finally we loop over the file
	  const double mass_n = 939.565560; // mass of neutron in MeV/c^2
	  for (Int_t i = 0; i < nevent; i++)
            {
	      TLorentzVector* neut1 = new TLorentzVector();
	      TLorentzVector* neut2 = new TLorentzVector();
	      TLorentzVector* neut3 = new TLorentzVector();
	      TLorentzVector* neut4 = new TLorentzVector();

	      Double_t energy1,energy2,energy3,energy4,momentum_mag1,momentum_mag2,momentum_mag3,momentum_mag4;

	      t->GetEntry(i);
	      //cout <<i<<" "<<x1<<" "<<y1<<" "<<z1<<" "<<ekin1<<" "<<x2<<" "<<y2<<" "<<z2<<" "<<ekin2<<"\n";
	      if (isnan(x1) || isnan(y1) || isnan(z1) || isnan(ekin1) || isnan(x2) || isnan(y2) || isnan(z2) || isnan(ekin2) || isnan(x3) || isnan(y3) || isnan(z3) || isnan(ekin3) || isnan(x4) || isnan(y4) || isnan(z4) || isnan(ekin4) ) { cout << "NAN!" << endl;
		outFile << "1\n";
		outFile << "1  2112  0  0 " << 0.00000001 << " " << 0.00000001 << " " << -0.00000001 << " " << mass_n/1000. << "\n";
		outN2File << "1\n";
		outN2File << "1  2112  0  0 " << 0.00000001 << " " << 0.00000001 << " " << -0.00000001 << " " << mass_n/1000. << "\n";
		outN3File << "1\n";
		outN3File << "1  2112  0  0 " << 0.00000001 << " " << 0.00000001 << " " << -0.00000001 << " " << mass_n/1000. << "\n";
		outN4File << "1\n";
		outN4File << "1  2112  0  0 " << 0.00000001 << " " << 0.00000001 << " " << -0.00000001 << " " << mass_n/1000. << "\n";


	      } else {
		// total energy = KE + mass
		energy1 = ekin1+mass_n;
		energy2 = ekin2+mass_n;
		energy3 = ekin3+mass_n;
	 	energy4 = ekin4+mass_n;
		// momentum set
		momentum_mag1 = sqrt(energy1*energy1 - mass_n*mass_n);
		momentum_mag2 = sqrt(energy2*energy2 - mass_n*mass_n);
		momentum_mag3 = sqrt(energy3*energy3 - mass_n*mass_n);
	 	momentum_mag4 = sqrt(energy4*energy4 - mass_n*mass_n);

		// Now set these things in the lorentz vector and convert to GeV for pythia's sake
		neut1->SetE(energy1/1000.);
		neut2->SetE(energy2/1000.);
		neut3->SetE(energy3/1000.);
		neut4->SetE(energy4/1000.);

		neut1->SetPz(momentum_mag1/1000.); // Pz because we can then rotate it
		neut2->SetPz(momentum_mag2/1000.); // Pz because we can then rotate it
		neut3->SetPz(momentum_mag3/1000.); // Do you really need a third comment?
		neut4->SetPz(momentum_mag4/1000.); // No but I need a fourth.

		// Now we must calculate theta and phi
		Double_t rho1 = sqrt(x1*x1 + y1*y1);
		Double_t rho2 = sqrt(x2*x2 + y2*y2);
		Double_t rho3 = sqrt(x3*x3 + y3*y3);
		Double_t rho4 = sqrt(x4*x4 + y4*y4);
		//cout << "x4: " << x4 << " y4: " << y4 << " z4: " << z4 << " ekin4:" << ekin4 <<  endl;

		Double_t theta1 = atan(rho1/z1);
		Double_t theta2 = atan(rho2/z2);
		Double_t theta3 = atan(rho3/z3);
		Double_t theta4 = atan(rho4/z3);

		Double_t phi1 = 0.;
		Double_t phi2 = 0.;
		Double_t phi3 = 0.;
		Double_t phi4 = 0.;

		const Double_t pi = 3.141592653589793238462643383279;
		// Phi is trickier because it goes from 0 to 2pi
		// First do first neutron
		if (x1==0. && y1==0.) {
		  phi1 = 0.;
		} else if (y1>=0.) {
		  phi1 = acos(x1/rho1);
		} else if (y1<0.) {
		  phi1 = 2.*pi - acos(x1/rho1);
		}
		// Now do second neutron
		if (x2==0. && y2==0.) {
		  phi2 = 0.;
		} else if (y2>=0.) {
		  phi2 = acos(x2/rho2);
		} else if (y2<0.) {
		  phi2 = 2.*pi - acos(x2/rho2);
		}
		// And the third neutron
		if (x3==0. && y3==0.) {
		  phi3 = 0.;
		} else if (y3>=0.) {
		  phi3 = acos(x3/rho3);
		} else if (y3<0.) {
		  phi3 = 2.*pi - acos(x3/rho3);
		}
		// And the fourth neutron
		if (x4==0. && y4==0.) {
		  phi4 = 0.;
		} else if (y4>=0.) {
		  phi4 = acos(x4/rho4);
		} else if (y4<0.) {
		  phi4 = 2.*pi - acos(x4/rho4);
		}

		neut1->SetTheta(theta1);
		neut2->SetTheta(theta2);
		neut3->SetTheta(theta3);
		neut4->SetTheta(theta4);

		neut1->SetPhi(phi1);
		neut2->SetPhi(phi2);
		neut3->SetPhi(phi3);
		neut4->SetPhi(phi4);


		outFile << "1\n";
		outFile << "1  2112  0  0 " << neut1->Px() << " " << neut1->Py() << " " << neut1->Pz() << " " << mass_n/1000. << "\n";
                outN2File << "1\n";
		outN2File << "1  2112  0  0 " << neut2->Px() << " " << neut2->Py() << " " << neut2->Pz() << " " << mass_n/1000. << "\n";
                outN3File << "1\n";
		outN3File << "1  2112  0  0 " << neut3->Px() << " " << neut3->Py() << " " << neut3->Pz() << " " << mass_n/1000. << "\n";
                outN4File << "1\n";
		outN4File << "1  2112  0  0 " << neut4->Px() << " " << neut4->Py() << " " << neut4->Pz() << " " << mass_n/1000. << "\n";
	         //cout << "Px: " << neut4->Px() << endl;


	      }
            }
	  inFile->Close();
	  outFile.close();
	  outN2File.close();
	  outN3File.close();
	  outN4File.close();

        } // End 4n


      else if (nneuts == 3)
        { //cout << "Entered the 3N if statement!" << endl;
	  TString nfile2 = dirdata + "/neutron2temp"+uniqueID+".indat";
	  TString nfile3 = dirdata + "/neutron3temp"+uniqueID+".indat";
	  ofstream outN2File(nfile2.Data());
	  ofstream outN3File(nfile3.Data());
	  // Now we find the relevant branches to look at
	  Double_t ekin1,ekin2,ekin3,x1,x2,x3,y1,y2,y3,z1,z2,z3;
	  TBranch *bekin1 = t->GetBranch("b9p1Ekin");
	  TBranch *bekin2 = t->GetBranch("b9p2Ekin");
	  TBranch *bekin3 = t->GetBranch("b9p3Ekin");
	
	  TBranch *bx1 = t->GetBranch("b9p1x");
	  TBranch *bx2 = t->GetBranch("b9p2x");
	  TBranch *bx3 = t->GetBranch("b9p3x");
	  
	  TBranch *by1 = t->GetBranch("b9p1y");
	  TBranch *by2 = t->GetBranch("b9p2y");
	  TBranch *by3 = t->GetBranch("b9p3y");
	  
 	  TBranch *bz1 = t->GetBranch("b9p1z");
	  TBranch *bz2 = t->GetBranch("b9p2z");
	  TBranch *bz3 = t->GetBranch("b9p3z");

	  // Set addresses
	  bekin1->SetAddress(&ekin1);
	  bekin2->SetAddress(&ekin2);
	  bekin3->SetAddress(&ekin3);
	  
	  bx1->SetAddress(&x1);
	  bx2->SetAddress(&x2);
	  bx3->SetAddress(&x3);
	  
	  by1->SetAddress(&y1);
	  by2->SetAddress(&y2);
   	  by3->SetAddress(&y3);
	  
	  bz1->SetAddress(&z1);
	  bz2->SetAddress(&z2);
	  bz3->SetAddress(&z3);
	  
	  // Get number of entries in the file
	  Int_t nevent = t->GetEntries();
	  cout << "Events to be passed to Geant4 = " << nevent << endl;
	  // Tell the geant macro how many events it will need to run
	  //macroFile << "/tracking/verbose 2" << std::endl;
	  macroFile << "/run/beamOn " << nevent << std::endl;
	  macroFile.close();

	  // Finally we loop over the file
	  const double mass_n = 939.565560; // mass of neutron in MeV/c^2
	  for (Int_t i = 0; i < nevent; i++)
            {
	      TLorentzVector* neut1 = new TLorentzVector();
	      TLorentzVector* neut2 = new TLorentzVector();
	      TLorentzVector* neut3 = new TLorentzVector();
	      Double_t energy1,energy2,energy3,momentum_mag1,momentum_mag2,momentum_mag3;
	      t->GetEntry(i);
	      //cout <<i<<" "<<x1<<" "<<y1<<" "<<z1<<" "<<ekin1<<" "<<x2<<" "<<y2<<" "<<z2<<" "<<ekin2<<"\n";
	      if (isnan(x1) || isnan(y1) || isnan(z1) || isnan(ekin1) || isnan(x2) || isnan(y2) || isnan(z2) || isnan(ekin2) || isnan(x3) || isnan(y3) || isnan(z3) || isnan(ekin3) ) {
		outFile << "1\n";
		outFile << "1  2112  0  0 " << 0.00000001 << " " << 0.00000001 << " " << -0.00000001 << " " << mass_n/1000. << "\n";
		outN2File << "1\n";
		outN2File << "1  2112  0  0 " << 0.00000001 << " " << 0.00000001 << " " << -0.00000001 << " " << mass_n/1000. << "\n";
		outN3File << "1\n";
		outN3File << "1  2112  0  0 " << 0.00000001 << " " << 0.00000001 << " " << -0.00000001 << " " << mass_n/1000. << "\n";

	      } else {
		// total energy = KE + mass
		energy1 = ekin1+mass_n;
		energy2 = ekin2+mass_n;
		energy3 = ekin3+mass_n;
		// momentum set
		momentum_mag1 = sqrt(energy1*energy1 - mass_n*mass_n);
		momentum_mag2 = sqrt(energy2*energy2 - mass_n*mass_n);
		momentum_mag3 = sqrt(energy3*energy3 - mass_n*mass_n);
		// Now set these things in the lorentz vector and convert to GeV for pythia's sake
		neut1->SetE(energy1/1000.);
		neut2->SetE(energy2/1000.);
		neut3->SetE(energy3/1000.);
		neut1->SetPz(momentum_mag1/1000.); // Pz because we can then rotate it
		neut2->SetPz(momentum_mag2/1000.); // Pz because we can then rotate it
		neut3->SetPz(momentum_mag3/1000.); // Do you really need a third comment?
		// Now we must calculate theta and phi
		Double_t rho1 = sqrt(x1*x1 + y1*y1);
		Double_t rho2 = sqrt(x2*x2 + y2*y2);
		Double_t rho3 = sqrt(x3*x3 + y3*y3);
		Double_t theta1 = atan(rho1/z1);
		Double_t theta2 = atan(rho2/z2);
		Double_t theta3 = atan(rho3/z3);
		Double_t phi1 = 0.;
		Double_t phi2 = 0.;
		Double_t phi3 = 0.;
		const Double_t pi = 3.141592653589793238462643383279;
		// Phi is trickier because it goes from 0 to 2pi
		// First do first neutron
		if (x1==0. && y1==0.) {
		  phi1 = 0.;
		} else if (y1>=0.) {
		  phi1 = acos(x1/rho1);
		} else if (y1<0.) {
		  phi1 = 2.*pi - acos(x1/rho1);
		}
		// Now do second neutron
		if (x2==0. && y2==0.) {
		  phi2 = 0.;
		} else if (y2>=0.) {
		  phi2 = acos(x2/rho2);
		} else if (y2<0.) {
		  phi2 = 2.*pi - acos(x2/rho2);
		}
		// And the third neutron
		if (x3==0. && y3==0.) {
		  phi3 = 0.;
		} else if (y3>=0.) {
		  phi3 = acos(x3/rho3);
		} else if (y3<0.) {
		  phi3 = 2.*pi - acos(x3/rho3);
		}

		neut1->SetTheta(theta1);
		neut2->SetTheta(theta2);
		neut3->SetTheta(theta3);
		neut1->SetPhi(phi1);
		neut2->SetPhi(phi2);
		neut3->SetPhi(phi3);

		outFile << "1\n";
		outFile << "1  2112  0  0 " << neut1->Px() << " " << neut1->Py() << " " << neut1->Pz() << " " << mass_n/1000. << "\n";
                outN2File << "1\n";
		outN2File << "1  2112  0  0 " << neut2->Px() << " " << neut2->Py() << " " << neut2->Pz() << " " << mass_n/1000. << "\n";
                outN3File << "1\n";
		outN3File << "1  2112  0  0 " << neut3->Px() << " " << neut3->Py() << " " << neut3->Pz() << " " << mass_n/1000. << "\n";

	      }
            }
	  inFile->Close();
	  outFile.close();
	  outN2File.close();
	  outN3File.close();
        } // End 3n


      else if (nneuts == 2)
        {
	  Double_t zArb = 1.;// [m] for e15118
	  TString nfile = dirdata + "/neutron2temp"+uniqueID+".indat";
	  ofstream outN2File(nfile.Data());
	  // Now we find the relevant branches to look at
	  Double_t ekin1,ekin2,x1,x2,y1,y2,z1,z2;
	  TBranch *bekin1;
	  TBranch *bekin2;
	  TBranch *bx1;
	  TBranch *bx2;
	  TBranch *by1;
	  TBranch *by2;
	  TBranch *bz1;
	  TBranch *bz2;
	  if(slice==1){
	  bekin1 = t->GetBranch("b3p1Ekin");
	  bekin2 = t->GetBranch("b3p2Ekin");
	  bx1 = t->GetBranch("b3p1tx");
	  bx2 = t->GetBranch("b3p2tx");
	  by1 = t->GetBranch("b3p1ty");
	  by2 = t->GetBranch("b3p2ty");
	  bz1 = t->GetBranch("b3p1z");
	  bz2 = t->GetBranch("b3p2z");
	  
	  }
	  else if(slice==2){
	  bekin1 = t->GetBranch("b5p1Ekin");
	  bekin2 = t->GetBranch("b5p2Ekin");
	  bx1 = t->GetBranch("b5p1tx");
	  bx2 = t->GetBranch("b5p2tx");
	  by1 = t->GetBranch("b5p1ty");
	  by2 = t->GetBranch("b5p2ty");
	  bz1 = t->GetBranch("b5p1z");
	  bz2 = t->GetBranch("b5p2z");
	  }
	  else if(slice==3){
	  bekin1 = t->GetBranch("b7p1Ekin");
	  bekin2 = t->GetBranch("b7p2Ekin");
	  bx1 = t->GetBranch("b7p1tx");
	  bx2 = t->GetBranch("b7p2tx");
	  by1 = t->GetBranch("b7p1ty");
	  by2 = t->GetBranch("b7p2ty");
	  bz1 = t->GetBranch("b7p1z");
	  bz2 = t->GetBranch("b7p2z");
	  }
	  else{
	  cout << "Setting neutron branches to b15, who knows if this is right ..." << endl;
	  bekin1 = t->GetBranch("b15p1Ekin");
	  bekin2 = t->GetBranch("b15p2Ekin");
	  bx1 = t->GetBranch("b15p1x");
	  bx2 = t->GetBranch("b15p2x");
	  by1 = t->GetBranch("b15p1y");
	  by2 = t->GetBranch("b15p2y");
	  bz1 = t->GetBranch("b15p1z");
	  bz2 = t->GetBranch("b15p2z");
	  }


	  // Set addresses
	  bekin1->SetAddress(&ekin1);
	  bekin2->SetAddress(&ekin2);
	  bx1->SetAddress(&x1);
	  bx2->SetAddress(&x2);
	  by1->SetAddress(&y1);
	  by2->SetAddress(&y2);
	  bz1->SetAddress(&z1);
	  bz2->SetAddress(&z2);

	  // Get number of entries in the file
	  Int_t nevent = t->GetEntries();
	  cout << "Events to be passed to Geant4 = " << nevent << endl;
	  // Tell the geant macro how many events it will need to run
	  //macroFile << "/tracking/verbose 2" << std::endl;
	  macroFile << "/run/beamOn " << nevent << std::endl;
	  macroFile.close();

	  // Finally we loop over the file
	  const double mass_n = 939.565560; // mass of neutron in MeV/c^2
	  for (Int_t i = 0; i < nevent; i++)
            {
	      TLorentzVector* neut1 = new TLorentzVector();
	      TLorentzVector* neut2 = new TLorentzVector();
	      Double_t energy1,energy2,momentum_mag1,momentum_mag2;
	      t->GetEntry(i);
	      //cout <<i<<" "<<x1<<" "<<y1<<" "<<z1<<" "<<ekin1<<" "<<x2<<" "<<y2<<" "<<z2<<" "<<ekin2<<"\n";
	      if (isnan(x1) || isnan(y1) || isnan(z1) || isnan(ekin1) || isnan(x2) || isnan(y2) || isnan(z2) || isnan(ekin2)) {
		outFile << "1\n";
		outFile << "1  2112  0  0 " << 0.00000001 << " " << 0.00000001 << " " << -0.00000001 << " " << mass_n/1000. << "\n";
		outN2File << "1\n";
		outN2File << "1  2112  0  0 " << 0.00000001 << " " << 0.00000001 << " " << -0.00000001 << " " << mass_n/1000. << "\n";
	      } else {
		// total energy = KE + mass
		energy1 = ekin1+mass_n;
		energy2 = ekin2+mass_n;
		// momentum set
		momentum_mag1 = sqrt(energy1*energy1 - mass_n*mass_n);
		momentum_mag2 = sqrt(energy2*energy2 - mass_n*mass_n);
		// Now set these things in the lorentz vector and convert to GeV for pythia's sake
		neut1->SetE(energy1/1000.);
		neut2->SetE(energy2/1000.);
		neut1->SetPz(momentum_mag1/1000.); // Pz because we can then rotate it
		neut2->SetPz(momentum_mag2/1000.); // Pz because we can then rotate it
		Double_t xx1 = zArb*tan(x1);
		Double_t xx2 = zArb*tan(x2);
		Double_t yy1 = zArb*tan(y1);
		Double_t yy2 = zArb*tan(y2);
		Double_t rho1= sqrt(xx1*xx1 + yy1*yy1);
		Double_t rho2= sqrt(xx2*xx2 + yy2*yy2);
		Double_t theta1 = atan(rho1/zArb);
		Double_t theta2 = atan(rho2/zArb);
		Double_t phi1 = 0.;
		Double_t phi2 = 0.;
		const Double_t pi = 3.141592653589793238462643383279;
		// Phi is trickier because it goes from 0 to 2pi
		// First do first neutron
		if (xx1==0. && yy1==0.) {
		  phi1 = 0.;
		} else if (yy1>=0.) {
		  phi1 = acos(xx1/rho1);
		} else if (yy1<0.) {
		  phi1 = 2.*pi - acos(xx1/rho1);
		}
		// Now do second neutron
		if (xx2==0. && yy2==0.) {
		  phi2 = 0.;
		} else if (yy2>=0.) {
		  phi2 = acos(xx2/rho2);
		} else if (yy2<0.) {
		  phi2 = 2.*pi - acos(xx2/rho2);
		}
		/* Now we must calculate theta and phi
		Double_t rho1 = sqrt(x1*x1 + y1*y1);
		Double_t rho2 = sqrt(x2*x2 + y2*y2);
		Double_t theta1 = atan(rho1/z1);
		Double_t theta2 = atan(rho2/z2);
		Double_t phi1 = 0.;
		Double_t phi2 = 0.;
		const Double_t pi = 3.141592653589793238462643383279;
		// Phi is trickier because it goes from 0 to 2pi
		// First do first neutron
		if (x1==0. && y1==0.) {
		  phi1 = 0.;
		} else if (y1>=0.) {
		  phi1 = acos(x1/rho1);
		} else if (y1<0.) {
		  phi1 = 2.*pi - acos(x1/rho1);
		}
		// Now do second neutron
		if (x2==0. && y2==0.) {
		  phi2 = 0.;
		} else if (y2>=0.) {
		  phi2 = acos(x2/rho2);
		} else if (y2<0.) {
		  phi2 = 2.*pi - acos(x2/rho2);
		}
		*/
		neut1->SetTheta(theta1);
		neut2->SetTheta(theta2);
		neut1->SetPhi(phi1);
		neut2->SetPhi(phi2);

		outFile << "1\n";
		outFile << "1  2112  0  0 " << neut1->Px() << " " << neut1->Py() << " " << neut1->Pz() << " " << mass_n/1000. << "\n";
                outN2File << "1\n";
		outN2File << "1  2112  0  0 " << neut2->Px() << " " << neut2->Py() << " " << neut2->Pz() << " " << mass_n/1000. << "\n";
	      }
            }
	  inFile->Close();
	  outFile.close();
	  outN2File.close();
        } // End 2n

	else if (nneuts == 1)
        {
	  // Here is where single neutron stuff is done
	  // Now we find the relevant branches to look at
	  Double_t ekin,x,y,z;
	  TBranch *bekin = t->GetBranch("b9p1Ekin");
	  TBranch *bx = t->GetBranch("b9p1x");
	  TBranch *by = t->GetBranch("b9p1y");
	  TBranch *bz = t->GetBranch("b9p1z");

	  // Set addresses
	  bekin->SetAddress(&ekin);
	  bx->SetAddress(&x);
	  by->SetAddress(&y);
	  bz->SetAddress(&z);
	  // Get number of entries in the file
	  Int_t nevent = t->GetEntries();
	  cout << "Events to be passed to Geant4 = " << nevent << endl;

	  // Tell the geant macro how many events it will need to run
	  macroFile << "/run/beamOn " << nevent << std::endl;
	  macroFile.close();

	  // Finally we loop over the file
	  const double mass_n = 939.565560; // mass of neutron in MeV/c^2
	  for (Int_t i = 0; i < nevent; i++)
            {
	      TLorentzVector* neut = new TLorentzVector();
	      Double_t energy,momentum_mag;
	      t->GetEntry(i);
	      if (isnan(x) || isnan(y) || isnan(z) || isnan(ekin) || ekin == 0.) {
		outFile << "1\n";
		outFile << "1  2112  0  0 " << 0.00000001 << " " << 0.00000001 << " " << -0.00000001 << " " << mass_n/1000. << "\n";
	      } else {
		// total energy = KE + mass
		energy = ekin+mass_n;
		// momentum set
		momentum_mag = sqrt(energy*energy - mass_n*mass_n);
		// Now set these things in the lorentz vector and convert to GeV for pythia's sake
		neut->SetE(energy/1000.);
		neut->SetPz(momentum_mag/1000.); // Pz because we can then rotate it
		// Now we must calculate theta and phi
		Double_t rho = sqrt(x*x + y*y);
		Double_t theta = atan(rho/z);
		Double_t phi = 0.;
		const Double_t pi = 3.141592653589793238462643383279;
		// Phi is trickier because it goes from 0 to 2pi
		// First do first neutron
		if (x==0. && y==0.) {
		  phi = 0.;
		} else if (y>=0.) {
		  phi = acos(x/rho);
		} else if (y<0.) {
		  phi = 2.*pi - acos(x/rho);
		}
		neut->SetTheta(theta);
		neut->SetPhi(phi);

		outFile << "1\n";
		outFile << "1  2112  0  0 " << neut->Px() << " " << neut->Py() << " " << neut->Pz() << " " << mass_n/1000. << "\n";
	      }
            }
        } else
        {
	  std::cout << "Bad number of neutrons caught by root2pythia. I am exiting\n";
	  return 1;
        }
      inFile->Close();
      outFile.close();
    }

  return 0;
}