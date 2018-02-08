/// \file 
/// System of particles and beam elements

#ifndef ST_SYSTEM_HH
#define ST_SYSTEM_HH

#include <gsl/gsl_ntuple.h>

#include "st_particle.hh"
#include "st_ble.hh"
#include "st_part_init.hh"
#include "st_part_misc.hh"
#include "st_histo_tuple.hh"


/** System of particles and beam elements.
 *  The method run() does all the work.
 */
class StSystem {
public:
  /** Constructor.  Initialize #fRuns and #fTuple.
   */
  StSystem() : fRuns(0), fTuple(0) {}
  
  /** Add a particle to the system.
   * \param p the particle to add
   */
  void addParticle(StParticle p);
  
  /** Add a beam line element (BLE).
   *  \param ss pointer to the beam line element to be added
   */
  void addBLE(StBLE* ss);

  //@{
  /// Get/set ntuple-pointer.
  void     setTupleP(StTuple* p) { fTuple = p; }
  StTuple* getTupleP()           { return fTuple; }
  //@}

  /** Run all particles through the system and apply the beam line elements.
   * \param n do this n times
   * \param f flag.  If set to "print" some things are printed.
   * \return Number of runs performed.
   */
  int run(int n=1, string f="");

  ////////GEANT Output Stuff Added by GAC, 2 Oct 2007///////////////////////////

  //method for running and printing out neutron parameter values to a text file
  int runGEANT(int n=1, string f="", string fle="", double parz=15.385);

  int runGEANTSecondNeutron(int n=1, string f="", string fle="", double parz=15.385);

  //vectors to store neutron energy & angles
  vector<double> EkinVect;
  vector<double> ThetaVect;
  vector<double> PhiVect;
  vector<double> EkinVect2;
  vector<double> ThetaVect2;
  vector<double> PhiVect2;

  //method to append neutron energy & angles to their vectors
  void FillVectors(StParticle p);

  //method to append neutron energy & angles to their vectors
  void FillVectorsSecondNeutron(StParticle pp);

  //method to print vector values to file
  void PrintVectors(string foutname="test123.ascii", double zmona=15.385);

  //method to print vector values to file
  void PrintVectorsSecondNeutron(string foutname="test123.ascii", double zmona=15.385);

  /////////////////////////////////////////////////////////////////////////////

  /// get vector to particle ... \todo fixup
  vector <StParticle>* getPartVectP() {return &fP; }

private:
  /// pointer to beam line elements
  vector <StBLE*> fBle;

  /// particles
  vector <StParticle> fP;

  /// number of runs already performed
  unsigned long  fRuns; 

  /// ntuple pointer
  StTuple* fTuple;

  /// histo pointer  \todo implement histos

};


#endif
