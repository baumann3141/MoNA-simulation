/** \file
 * Implementation of particle system methods
 */

#include "st_system.hh"
#include "deb_err.h"

// add beam line element
void StSystem::addBLE(StBLE* ss) {
  if (! ss) ERR("zero pointer not valid");
  if (fRuns) ERR("The simulation was already started.  You cannot add BLEs (or?)!  Why not?");

  // BLEs need to have access to the system, e.g. to create new particles 
  ss->setSystemP(this);
  ss->setID(fBle.size());  // tell BEL, which BLE in the system it is

  fBle.push_back(ss);      // add BLE
}

void StSystem::addParticle(StParticle p) { 
  p.setID(fP.size()); // set particle ID
  fP.push_back(p);    // add particle 
}


/// \todo Print time needed for the run.
/// \todo Add option to print progress.
int StSystem::run(int n, string f) {
  if (n<=0) return 0;

  int killed_runs = 0;
  int written_runs = 0;
  int required_good_runs = -1;

  //When using crdc2_cut, input the number of good events you want...
  if(f.find("crdc2_cut")!=string::npos){
    required_good_runs = n;
    n = 9999999999;
  }

  for (size_t i(0); i < (size_t) n; i++){     // loop over number of simulation runs
    if (f.find("print")!=string::npos) {
      printf("\n\nRun number %lu:\n",fRuns);
    }

    bool write_events = true;

    //zwk addition to track number of events
    if( (fRuns%25000)==0 ) printf(" --Run Number: %d\n",fRuns);
    if(f.find("crdc2_cut")!=string::npos && (fRuns%25000)==0) printf("   Killed Runs: %d   Written Runs: %d  (CRDC2_cut is applied)\n",killed_runs, written_runs);
    //printf("\n --Run Number: %d\n",fRuns);

    for (size_t j(0); j < fBle.size(); j++) {  // loop over beam line elements
//    for (size_t j(0); j < 3; j++) {  // loop over beam line elements
      //cout << fBle.at(j)->getName() << " " << fBle.at(j)->getID() << " " << fBle.at(j)->getName() <<  endl;            // print name of BLE MDJ
      fBle.at(j)->act(&fP);                    // act on all particles with current BLE
            if (f.find("print")!=string::npos) {    // print info if wanted
        if ( i==0 && j==0 ) fP.at(0).printHeader();  // print header for first particle and first run
        cout<<fBle.at(j)->getName()<< " " << fBle.at(j)->getID() << " " << fBle.at(j)->getName() << endl;            // print name of BLE
	for (size_t k=0; k<fP.size(); k++) fP.at(k).print();  // loop over particles
      }

      //----option to kill events in which frag doesn't make it to CRDC2 (used initally for 8He from 14Be experiment)
      if(f.find("crdc2_cut")!=string::npos){	
	if(fBle.at(j)->getName().find("crdc2_ble")!=string::npos  && fP.at(0).getEkin() <= 0){ write_events = false; killed_runs++;} //cout << "*^*^*^^*^*^*^KILLED EVENT" << endl;}
      }
      else write_events = true;
      //---------------------------

    }//j

    fRuns++;  // increment run number
    if (fTuple && write_events){ fTuple->fill(); written_runs++;}// fill (write) ntuple

    if(f.find("crdc2_cut")!=string::npos && written_runs == required_good_runs) break;

  }//i
  return n;
}

//////////GEANT Output Stuff Added by GAC, 2 Oct 2007////////////////////////////////////////////////////

void StSystem::FillVectors(StParticle p) {
  EkinVect.push_back( p.getEkin() );
  ThetaVect.push_back( p.getTheta() );
  PhiVect.push_back ( p.getPhi() );
}

void StSystem::FillVectorsSecondNeutron(StParticle pp) {
  EkinVect2.push_back( pp.getEkin() );
  ThetaVect2.push_back( pp.getTheta() );
  PhiVect2.push_back ( pp.getPhi() );
}

void StSystem::PrintVectors(string foutname, double zmona) {
  int lll = int(EkinVect.size());
  FILE * fout;
  fout = fopen(foutname.c_str(),"w");
  fprintf(fout,"%-12s\n","MoNA Zpos (m)");
  fprintf(fout,"%-12.3f\n",zmona);
  fprintf(fout,"\n");
  fprintf(fout,"%-12s %-12s %-12s\n","Energy(MeV)","Theta(rad)","Phi(rad)");
  fprintf(fout,"\n");
  for (int jj=0; jj<lll ; ++jj) {
    fprintf(fout,"%-12f %-12f %-12f\n",EkinVect.at(jj),ThetaVect.at(jj),PhiVect.at(jj));
  }
  fclose(fout);
}

void StSystem::PrintVectorsSecondNeutron(string foutname, double zmona) {
  int lll = int(EkinVect.size());
  FILE * fout;
  fout = fopen(foutname.c_str(),"w");
  fprintf(fout,"%-12s\n","MoNA Zpos (m)");
  fprintf(fout,"%-12.3f\n",zmona);
  fprintf(fout,"\n");
  fprintf(fout,"%-12s %-12s %-12s\n","Energy(MeV)","Theta(rad)","Phi(rad)");
  fprintf(fout,"\n");
  for (int jj=0; jj<lll ; ++jj) {
    fprintf(fout,"%-12f %-12f %-12f\n",EkinVect2.at(jj),ThetaVect2.at(jj),PhiVect2.at(jj));
  }
  fclose(fout);
}



int StSystem::runGEANT(int n, string f, string fle ,double parz) {
  if (n<=0) return 0;
  //  fp.at(1).initializeVectors();
  for (size_t i(0); i < (size_t) n; i++){     // loop over number of simulation runs
    if (f.find("print")!=string::npos) {
      printf("\n\nRun number %lu:\n",fRuns);
    }

    //zwk addition to track number of events
    if( (fRuns%25000)==0 ) printf(" ---Run Number: %d\n",fRuns);

    for (size_t j(0); j < fBle.size(); j++) {  // loop over beam line elements
      fBle.at(j)->act(&fP);                    // act on all particles with current BLE

      if(j==2) FillVectors(fP.at(1));    //Fill vectors of neutron energy & angles

      if (f.find("print")!=string::npos) {    // print info if wanted
        if ( i==0 && j==0 ) fP.at(0).printHeader();  // print header for first particle and first run
        cout<<fBle.at(j)->getName()<<endl;            // print name of BLE
        /// \todo print detailed description of BLE here
        for (size_t k=0; k<fP.size(); k++) fP.at(k).print();  // loop over particles
      }
    }

    fRuns++;  // increment run number
    if (fTuple) fTuple->fill(); // fill (write) ntuple
  }
  PrintVectors(fle, parz);  //Write out neutron energies & angles to file for all events
  return n;
}

int StSystem::runGEANTSecondNeutron(int n, string f, string fle ,double parz) {
  if (n<=0) return 0;
  //  fp.at(1).initializeVectors();
  for (size_t i(0); i < (size_t) n; i++){     // loop over number of simulation runs
    if (f.find("print")!=string::npos) {
      printf("\n\nRun number %lu:\n",fRuns);
    }

    //zwk addition to track number of events
    if( (fRuns%25000)==0 ) printf(" ----Run Number: %d\n",fRuns);

    for (size_t j(0); j < fBle.size(); j++) {  // loop over beam line elements
      fBle.at(j)->act(&fP);                    // act on all particles with current BLE

      if(j==2) FillVectorsSecondNeutron(fP.at(2));    //Fill vectors of neutron energy & angles

      if (f.find("print")!=string::npos) {    // print info if wanted
        if ( i==0 && j==0 ) fP.at(0).printHeader();  // print header for first particle and first run
        cout<<fBle.at(j)->getName()<<endl;            // print name of BLE
        /// \todo print detailed description of BLE here
        for (size_t k=0; k<fP.size(); k++) fP.at(k).print();  // loop over particles
      }
    }

    fRuns++;  // increment run number
    if (fTuple) fTuple->fill(); // fill (write) ntuple
  }
  PrintVectorsSecondNeutron(fle, parz);  //Write out neutron energies & angles to file for all events
  return n;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
