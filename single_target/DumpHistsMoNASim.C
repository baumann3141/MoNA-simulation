// Copies only user-selected active branches to a new file "fileout"
// This saves considerable space. Structure is preserved, so previous 
// load macros can stay the same.

void DumpHistsMoNASim(const char* filein, const char* gfilein, const char* fileout, double mona_r, double mona_t){

  // Create input and output files
  TFile* fraw = new TFile(filein);
  TTree* traw = (TTree*)fraw->Get("t");

  TFile *gfraw = new TFile(gfilein);
  TTree *gtraw = (TTree*)gfraw->Get("at");
  // Output
  TFile* outf = new TFile(fileout, "RECREATE");

  // Set Branch Status of -geant_UNIQUE_ID filein
  traw->SetBranchStatus("*",0); // Dissables ALL branches for copying (inactive)
  traw->SetBranchStatus("b13pg1x",1);// Here I select the branches I want to copy
  traw->SetBranchStatus("b13pg1y",1);
  traw->SetBranchStatus("b13pg1z",1);
  traw->SetBranchStatus("b13pgmultiplicity",1);
  traw->SetBranchStatus("b2p0R_exen",1);
  traw->SetBranchStatus("b9p0Ekin",1);
 
  // Set Branch Status of -geant_anal_UNIQU_ID gfilein
  gtraw->SetBranchStatus("*",0);
  gtraw->SetBranchStatus("EDecayG1",1);   

  // Clone Active Branches, but do not fill.
  TTree *outt  = (TTree*)traw->CloneTree(0);
  TTree *goutt = (TTree*)gtraw->CloneTree(0);
 
  // Copy Entries of Active Branches
  outt->CopyEntries(traw);
  goutt->CopyEntries(gtraw);

  // Create mona_r parameter and add to outt
  TBranch *MoNA_R = outt->Branch("mona_r",&mona_r,"mona_r/D");
  TBranch *MoNA_T = outt->Branch("mona_t",&mona_t,"mona_t/D");
  int n = traw->GetEntries();
  //outt->SetBranchStatus("*",0);
  //outt->SetBranchStatus("mona_r",1);
  for(int i = 0; i < n; i++){
     MoNA_R->Fill();
     MoNA_T->Fill();
  }
  outt->SetEntries(MoNA_R->GetEntries());
  outt->SetEntries(MoNA_T->GetEntries());

  outt->AddFriend(goutt);
  // Write Trees to file
  outf->Write();
    
  // Clean Up
  fraw->Close();
  gfraw->Close();
  outf->Close();
 
}
