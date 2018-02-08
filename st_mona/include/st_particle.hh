/*! \file
 *  \brief Definition of a particle.
 *
 *  Everything belonging to a particle is defined and declared here.
 *
*/

#ifndef ST_PARTICLE_HH
#define ST_PARTICLE_HH

#include <string>
#include <math.h>

#include "deb_err.h"

using namespace std;

/** Particle data structure.
 * This struct with the particle data is needed for the filling of
 * ntuples, so we can just fill the whole structure.
 */
typedef struct {
  int Z;     ///< charge number (used for particle ID); 
             ///  use negative values for special particles, if needed
  int A;     ///< mass number   (used for particle ID)
             ///  use negative values for special particles, if needed
  
  int Q;     ///< particle charge in units of e
  double m;  ///< particle mass in MeV/c^2
  
  double x;  ///< x in meter
  double tx; ///< \f$\theta_x\f$ in rad
  double y;  ///< y in meter
  double ty; ///< \f$\theta_y\f$ in rad
  double z;  ///< z in meter
  
  double Ekin;  ///< total kinetic energy in MeV
  double path;  ///< path length in meter
  double t;     ///< time in ns
} StParticleData;

/** This is the class.
 *  With it all relevant parameters for particles can be set or unset.
 */
class StParticle {
public:
  StParticle();   ///< constructor; sets all values of fData to zero

public:
  /// Macro to define standard setters and getters.
  /// This Macro is used below to define standart getters and setters for StParticle,
  /// i.e. the members of StParticleData.
#define SETGET(vt, X) void set ## X(vt arg) {fData.X=arg;} \
                      vt   get ## X()       {return fData.X;}
  //@{
  /// Standard GETters and SETters for members of StParticleData. 
  SETGET(int,Z);
  SETGET(int,A);

  SETGET(int,Q);
  SETGET(double,m);

  SETGET(double,x);
  SETGET(double,tx);
  SETGET(double,y);
  SETGET(double,ty);
  SETGET(double,z);

  SETGET(double,Ekin);
  SETGET(double,path);
  SETGET(double,t);
#undef SETGET
  //@}

  //@{
  /// setget E/A
  void setEperA(double v) { /// \todo check that fData.A was set already
    fData.Ekin = v*fData.A;
  }
  /// \todo add constants file!!!
  double getEperA() {return fData.Ekin/fData.A;}
  //@}

  //@{
  /// setget (gamma-1)*u  (what is usually meant when saying MeV/u)
  void setGammaMinOneU(double v) {setGamma(v/931.494028+1);}
  /// \todo add constants file!!!
  double getGammaMinOneU() {return 931.494028*(getGamma()-1);}
  //@}

  //@{
  /// setget gamma
  void setGamma(double v) {if (getm()<=0) ERR("Mass not set"); setEkin((v-1)*getm());}  /// \todo make sure mass is set right
  double getGamma() {if (getm()<=0) ERR("Mass not set"); return 1 + getEkin()/getm(); }
  //@}

  //@{
  /// setget beta
  void setBeta(double v) {setGamma(1/sqrt(1-v*v));}  /// \todo check that v < 1
  double getBeta() {return sqrt(1-1/(getGamma()*getGamma()));}
  //@}

  //@{
  /// setget velocity in m/ns
  void setVelo(double v) {setBeta(v/0.299792458);}  /// \todo replace by defined constant!!!
                                            /// \todo check first that v < c
  double getVelo() {return getBeta()*0.299792458;}
  //@}

  //@{
  /// set/get Etotal  (relativistic total energy)
  void setEtotal(double v) {fData.Ekin = v - fData.m;}  
  /// \todo check that mass is set already
  /// \todo check that v is larger than m!!!
  double getEtotal() {return fData.m + fData.Ekin;}
  //@}

  //@{
  /// set/get momentum P and components
  void setP(double v) {setEtotal(sqrt(v*v + getm()*getm()));}
  double getP()  { return getBeta()*getGamma()*getm();}
  double getPx() { return getP()*getUnitX();}
  double getPy() { return getP()*getUnitY();}
  double getPz() { return getP()*getUnitZ();}
  //@}

  //@{
  /// get components of unit vector
  double getUnitX() { return cos(getPhi()) * sin(getTheta()); }
  double getUnitY() { return sin(getPhi()) * sin(getTheta()); }
  double getUnitZ() { return                 cos(getTheta()); }
  //@}

  //@{  \todo replace constants !!!
  /// setget Brho  
  void   setBrho(double v) {        setP(v*getQ()*299.792458); }
  double getBrho()         { return getP()/getQ()/299.792458;  }
  //@}

  /// \todo setget Isotope Name (format e.g. 24O)

  /// \todo setMass from mass table

  /// Print all data in one row 
  /// \param flag if set to one, also a description (table header) is printed
  void print(int flag=0);  

  /// print table header
  void printHeader();

  /// get data pointer
  StParticleData* getDataP() {return &fData;}
  
  //@{
  /// set/get particle ID number
  void   setID(size_t i) {       fID=i; }
  size_t getID()         {return fID;   }
  //@}

  //@{
  /// set/get slope
  double getSlopeX()         { return tan(fData.tx); }
  double getSlopeY()         { return tan(fData.ty); }
  void   setSlopeX(double s) { fData.tx = atan(s); }
  void   setSlopeY(double s) { fData.ty = atan(s); }
  //@}

  //@{
  /// set/get \f$\theta\f$ and \f$\phi\f$ (in rad) 
  void   setThetaPhi(double theta, double phi);
  double getTheta();
  double getPhi();
  //@}

  //@{ 
  /// "add" theta and phi to existing vector
  void addThetaPhi(double theta, double phi);
  //@}

private:
  StParticleData fData;  ///< particle data
  size_t fID;            ///< ID given by the system
};

#endif

