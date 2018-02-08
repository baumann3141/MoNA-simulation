/** \file 
 * Beam line elements.
 * Beam line elements have a name StBLE::name and some config StBLE::config descriptions and
 * the element can act on all the particles, either via act() or
 * via the operator().
 */
#ifndef ST_BLE_HH
#define ST_BLE_HH

#include <string>
#include <iostream>
#include <vector>

using namespace std;

#include "st_particle.hh"
#include "st_det_resp.hh"
#include "CycSrim.h"

// It should know that there is a class StSystem.
// We cannot include st_system.hh because st_system.hh includes this file...
class StSystem;

/** Pure virtual class for beam line elements
 * 
 */
class StBLE {
public:
  /// Constructor
  StBLE(string n) : fName(n),             fSys(0) {}    

  /** do the action for one particle
   * \param p pointer to particle
   */
  virtual int act(StParticle* p) = 0;

  /** do the action for all particles
   * \param pp pointer to vector of particles
   */
  virtual int act(vector <StParticle> *pp) ;

  //@{
  /// See act()
  inline int operator() (StParticle* p)           { return act(p);  }
  inline int operator() (vector <StParticle> *pp) { return act(pp); }
  //@}

  /// virtual destructor
  inline virtual ~StBLE() {}

  /** add general ntuple fields,
      i.e. ones that a present for all BLEs such as 
      all particle properties (x,tx,y....)  
      \param name     Name of item to add; has to be known to the corresponding BLE
      \param particle Particle number of concerned particle
      \param detResp  Detector response
  */
  void addNtupFields(string name, int particle=0, StDetResp* detResp=0);  

private:
  // do some clean-up if there is the need
  virtual int cleanUp() { return 0; }

public:
  /// Fill values defined by addNtupFields()
  /// into the n-tuple row.
  /// \param pp pointer to vector of particles
  void fillNtupFields(vector <StParticle> * pp); /// \todo use interator here!!!

private:
  /** fill special ntuple fields,
      i.e. ones that are specific for a certain BLE, such
      as energy loss for StMaterial.  
      This is private and should be called by addNtupFields().
      It should be re-implemented by the daughter class, where appropriate.
      \param name Name of item to add; has to be known to the corresponding BLE
      \param particle_number Particle Number
      \todo implement this!!!
  */
  virtual int fillSpecialNtupFields(string name, int particle_number) {
    name = name;
    particle_number = particle_number;
    return 1;
  } 

public:
  //@{
  /// Set/Get name and config information
  void   setName(string s)   {        fName=s;   }
  string getName()           { return fName;     }
  void   setConfig(string s) {        fConfig=s; }
  string getConfig()         { return fConfig;   }
  //@}

  //@{
  /// Set/Get system pointer
  void setSystemP(StSystem* p) {fSys=p; setSubSystemP();}
  virtual void setSubSystemP() {}  /// sometime needed to set the subsystem pointers
  StSystem* getSystemP() {return fSys;}
  //@}

  //@{
  /// Set/Get ID given by the system
  void   setID(size_t i)  { fID=i;      }
  size_t getID()          { return fID; }
  //@}

private:
  //@{
  /// Name and config information.
  string fName;
  string fConfig;
  //@}

  size_t fID;       ///< ID number given by the system (so we know who (which BLE) we are)
  StSystem* fSys;   ///< Pointer to StSystem this BLE belongs to (needed to create particles e.g.)

  vector <string> fNtupFields;     ///< stores the field to be filled; e.g. Brho, x, tx
  vector <int>    fNtupParticles;  ///< stores the particles to be considered for values of ntupFields
};


#endif

