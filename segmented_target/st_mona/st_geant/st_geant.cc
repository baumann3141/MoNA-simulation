/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * A program to convert neutron data from GEANT simulation into a ROOT file that can be  *
 * analyzed using mona_analysis.  To compile, type at the command line:                  *
 *       g++ -o st_geant st_geant.cc `root-config --cflags --libs`                       *
 *                                                                                       *
 * In order to run this program your PATH must include                                   *
 * /projects/proj6/mona-sim/soft/root/root_v5.12_00/include/                             *
 * and your LD_LIBRARY_PATH must include                                                 *
 * /projects/proj6/mona-sim/soft/root/root_v5.12_00/lib/                                 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */



//Normal C++ includes
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
using namespace std;

//ROOT-specific includes
#include "TFile.h"
#include "TTree.h"
#include "TString.h"

void MyDirections() {

  printf("\n"
	 "This program merges an existing st_mona ROOT file with an ASCII file containing\n"
         "neutron data from GEANT simulation; the output is a new ROOT file with the b13p1*\n"
	 "parameter values replaced by those from the GEANT simulation. This ROOT file can then\n"
	 "be used as an input file for the mona_analysis program or be converted into a filter\n"
	 "file for analysis using SpecTcl.\n"
	 "\n"
         "Program options:\n"
	 "\n"
         "-rootf filename.root -- set the input ROOT file name (default is st_mona.root).\n"
	 "\n"
	 "-geantf filename.* -- set the input ASCII file name (default is st_geant.out).\n"
	 "      --Note that your ASCII file must contain only five collums of numbers\n"
         "        ordered in the following way (this should have been the default output from GEANT):\n"
	 "        Kinetic Energy (MeV)  TOF(ns)  X-Position (mm)  Y-position (mm)  Z-position (mm)\n"
	 "\n"
	 "-outf filename.root -- set the output ROOT file name (default is st_geant.root).\n"
	 "\n"
	 "--help or -? -- Print this help menu (but you already figured that out...)\n"
	 "\n");
  exit(0);
}

int main(size_t argc, char** argv) {

  string  rootfile  = "st_mona.root"  ;
  string  geantfile = "st_geant.out"     ;
  string  outfile   = "st_geant.root" ;

  for (size_t i=1; i < argc; i++) {

    string sw=argv[i];
    if (0);
    else if (sw == "-rootf")   rootfile  = argv[++i];
    else if (sw == "-geantf")  geantfile = argv[++i];
    else if (sw == "-outf")    outfile   = argv[++i];
    else if (sw == "-?" || sw == "--help") MyDirections();
    else cerr << "Unknown option " << sw.c_str() << endl;
 
 }


  //read old ROOT file into memory and set a tree pointer
  TFile * oldfile = new TFile(rootfile.c_str());
  TTree * oldt = (TTree *)oldfile->Get("t");

  //Make it so we don't copy neutron parameters.
  oldt->SetBranchStatus("b13p1*",0);

  //Create new ROOT file and tree that is a clone of the old one.
  TFile * newfile = new TFile(outfile.c_str(),"RECREATE");
  TTree *t = oldt->CloneTree();

  oldt->SetBranchStatus("b13p1*",1);


  //Read in neutron params from ascii file

  //ifstream for reading in data from ASCII file
   ifstream instrm;
   instrm.open(geantfile.c_str());

   double b13p1Ekin, b13p1t, b13p1x, b13p1y, b13p1z;
   double b13p1EkinOLD, b13p1tOLD, b13p1xOLD, b13p1yOLD, b13p1zOLD;

   int nlines = 0;

   //Make new branches for neutron params
   TBranch *Tbr  = t->Branch("b13p1t",&b13p1t,"b13p1t/D");
   TBranch *Xbr  = t->Branch("b13p1x",&b13p1x,"b13p1x/D");
   TBranch *Ybr  = t->Branch("b13p1y",&b13p1y,"b13p1y/D");
   TBranch *Zbr  = t->Branch("b13p1z",&b13p1z,"b13p1z/D");
   TBranch *KEbr = t->Branch("b13p1Ekin",&b13p1Ekin,"b13p1Ekin/D");

   //remake the old ones with a different name
   TBranch *TbrOLD  = t->Branch("b13p1t_st",&b13p1tOLD,"b13p1t_st/D");
   TBranch *XbrOLD  = t->Branch("b13p1x_st",&b13p1xOLD,"b13p1x_st/D");
   TBranch *YbrOLD  = t->Branch("b13p1y_st",&b13p1yOLD,"b13p1y_st/D");
   TBranch *ZbrOLD  = t->Branch("b13p1z_st",&b13p1zOLD,"b13p1z_st/D");
   TBranch *KEbrOLD = t->Branch("b13p1Ekin_st",&b13p1EkinOLD,"b13p1Ekin_st/D");

   //Get neutron values out of the ASCII file and put them into the ROOT tree
   while (1) {

     //read in param values from ASCII file
     instrm >> b13p1Ekin >> b13p1t >> b13p1x >> b13p1y >> b13p1z;

     //convert millimeters to meters
     b13p1x*=0.001;
     b13p1y*=0.001;
     b13p1z*=0.001;

      if (!instrm.good()) break;

      //Fill the branches
      Tbr->Fill();
      Xbr->Fill();
      Ybr->Fill();
      Zbr->Fill();
      KEbr->Fill();

      nlines++;
   }

   //close out the ifstream
   instrm.close();

   //Get the old neutron param values and put them into the new tree w/ different names
   oldt->SetBranchAddress("b13p1t",&b13p1tOLD);
   oldt->SetBranchAddress("b13p1x",&b13p1xOLD);
   oldt->SetBranchAddress("b13p1y",&b13p1yOLD);
   oldt->SetBranchAddress("b13p1z",&b13p1zOLD);
   oldt->SetBranchAddress("b13p1Ekin",&b13p1EkinOLD);

   for (int i=0; i < oldt->GetEntries(); i++) {
     oldt->GetEntry(i);
     TbrOLD->Fill();
     XbrOLD->Fill();
     YbrOLD->Fill();
     ZbrOLD->Fill();
     KEbrOLD->Fill();
   }

   //Write & close the new ROOT file.
   newfile->Write();
   newfile->Close();


   //Check that # of events in ROOT file = # of evenst in ASCII file
   int nROOT = oldt->GetEntries();
   if (nlines != nROOT) {
     cerr << "WARNING: Your ROOT file and ASCII file have a different number of events!!!" << endl;
     cerr << "The ROOT file has "<<nROOT<<" events; the ASCII file has "<<nlines<< "events..."<<endl;
   } else {
     printf("Found %d events\n",nlines);
   }

   //Close the old ROOT file.
   oldfile->Close();

   return 0;

}
