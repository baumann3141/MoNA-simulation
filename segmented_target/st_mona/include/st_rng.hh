/** \file
 * Random number generator stuff...
 */
#ifndef ST_RNG_HH
#define ST_RNG_HH

#include <fstream>
#include <iostream>
#include <string>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_histogram.h>
#include <gsl/gsl_histogram2d.h>
#include "TF1.h"
#include "TH1.h"

#include "deb_err.h"

/// pure virutal random number generator
class StRNG {
public:
  virtual double value() = 0; ///< return random value
  virtual double value(double l, double u); ///< return random value between l and u
  virtual void value2d(double &En, double &Erel) {En=0; Erel=0;}; //used for volya + asymbw
  virtual double GetEi() {return 0;}  //used for volya's 2n prescription since need initial energy to determine neutron energies.
  virtual ~StRNG() {}  ///< destructor
};
//-----##########################################################-----//

/// Gauss RNG -----------///
class StRNGGauss : public StRNG {
public:

  void setSigma(double v)    { fSigma    = v; }   /// set sigma
  void setCentroid(double v) { fCentroid = v; }   /// set centroid

protected:

  double fCentroid;   /// centroid of gauss
  double fSigma;   /// sigma of gauss

};
//-----##########################################################-----//

///--------- Exp RNG -----------///
class StRNGExp : public StRNG {
public:

  void setCentroid(double v) { fCentroid = v; }   /// set centroid

protected:

  double fCentroid;   /// centroid of gauss

};
//-----##########################################################-----//

/// Custom BW RNG -----------///
class StRNGCustombw : public StRNG {
public:

  /// set sigma and centroid
  void setSigma(double v)    { fSigma    = v; }
  void setCentroid(double v) { fCentroid = v; }

protected:

  double fCentroid;   /// centroid of gauss
  double fSigma;   /// sigma of gauss
  const gsl_histogram_pdf* fP;

};
//-----##########################################################-----//

/// Laplace RNG -----------///
class StRNGLaplace : public StRNG {
public:

  /// set sigma and centroid
  void setSigma(double v)    { fSigma    = v; }
  void setCentroid(double v) { fCentroid = v; }

protected:

  double fCentroid;   /// centroid of gauss
  double fSigma;  /// sigma of gauss
};
//-----##########################################################-----//

/// ####DOUBLE LAPLACIAN RNG FOR MONA RESOLUTION  -----------///
class StRNGDoubleLaplace : public StRNG {
public:

  void setSigma1(double v)    { fSigma1    = v; }   /// set sigmas
  void setCentroid1(double v) { fCentroid1 = v; }   /// set centroid
  void setSigma2(double v)    { fSigma2    = v; }   /// set sigmas
  void setCentroid2(double v) { fCentroid2 = v; }   /// set centroid
  void setPrcnt(double v)     {Prcnt = v;}   /// set weighting percentage:

protected:

  double fCentroid1;   /// centroid of gauss
  double fSigma1;   /// sigma of gauss
  double fCentroid2;   /// centroid of gauss
  double fSigma2;   /// sigma of gauss
  double Prcnt;   /// percentage of the total area that is covered by Laplacian 1.

};
//-----##########################################################-----//

/// Cauchy RNG
class StRNGCauchy : public StRNG {
public:

  /// set sigma and centroid
  void setSigma(double v)    { fbwSigma    = v; }
  void setCentroid(double v) { fbwCentroid = v; }

protected:

  double fbwCentroid;   /// centroid of gauss
  double fbwSigma;   /// sigma of gauss
};
//-----##########################################################-----//

/// Gamma RNG
class StRNGGamma : public StRNG {
public:

  /// set A and B
  void setA(double v) { fA = v; }
  void setB(double v) { fB = v; }

protected:

  double fA;  /// parameter A (see gsl)
  double fB;  /// parameter B (see gsl)

};
//-----##########################################################-----//

/// uniform distribution
class StRNGUniform : public StRNG {
public:

protected:

  double fLower;  /// lower bound 
  double fUpper;  /// upper bound

};
//-----##########################################################-----//

/// Angular Distribution given by legendre polynomials
class StRNGLegendre : public StRNG {
public:
 
  gsl_histogram* fH;
  gsl_histogram_pdf * fP;
  
protected:

  double lvalue;

};

//-----##########################################################-----//

/// Cos_twoneutron distribution
class StRNGCos_twoneutron : public StRNG {
public:

protected:

  double fLower;  /// lower bound 
  double fUpper;  /// upper bound

};
//-----##########################################################-----//

/// custom distribution
class StRNGCustom_e1 : public StRNG {
public:

protected:

  double fLower;   /// lower bound 
  double fUpper;  /// upper bound

};
//-----##########################################################-----//

/// AsymmetricBW distribution
class StRNGAsymmetricBW : public StRNG {
public:
  
  gsl_histogram* fH;
  gsl_histogram_pdf * fP;
  int angMom;
  int fragMass;
  int flagGamma;
  int n;
  double Ezero;
  double Width;

  
protected:

  double fLower;  /// lower bound 
  double fUpper;  /// upper bound

};
//-----##########################################################-----//

/// Etotal_11Li distribution
class StRNGEtotal_11Li : public StRNG {
public:
  
  gsl_histogram* fH;
  gsl_histogram_pdf * fP;
  int n;
  void setEb(double v) { Eb = v; }   /// set centroid

  
protected:

  double Eb;  /// lower bound 

};
//-----##########################################################-----//

/// E_12Li distribution
class StRNGE_12Li : public StRNG {
public:
  
  gsl_histogram* fH;
  gsl_histogram_pdf * fP;
  int n;
  void setEb(double v) { Eb = v; }   /// set centroid

  
protected:

  double Eb;  /// lower bound 

};
//-----##########################################################-----//

/// Angular Distribution for (d,p) reaction
class StRNGangular_distribution_dp : public StRNG {
public:
  
  gsl_histogram* fH;
  gsl_histogram_pdf * fP;
//  int n;
  
protected:

};

//-----##########################################################-----//

/// Two Neutron_seq distribution
class StRNGTwoNeutron_seq : public StRNG {
public:
  
  gsl_histogram* fH;
  gsl_histogram_pdf * fP;
  int n;
  int AngMom;
  int nlines;
  double Q2N;
  double WidthTotal;
  double Width1;
  double Q1N;
  double Width2;
  int fragMass;
  double SF1;
  double SF2;
  double hBarC; 
  double Nmass;
  double amu; 
  double nucRad; 
  double redMass1; 
  double redMass2; 
  double k1;
  double k2;
  double Radius1;
  double Radius2;
  double x[10001];

  
protected:

  double fLower;  /// lower bound 
  double fUpper;  /// upper bound

};
//-----##########################################################-----//
//-----##########################################################-----//
//   Two neutron decay following Volya's perscription
class StRNGVolya_2nseq : public StRNG {
public:
  double fInitialStateEnergy;
  gsl_histogram* fH;
  gsl_histogram_pdf * fP;

  //used volya + bw
  gsl_histogram2d* fH2d;
  gsl_histogram2d_pdf *fP2d;

  double GetEi() {return fInitialStateEnergy;}
  
protected:

  double fLower;  /// lower bound 
  double fUpper;  /// upper bound

};
//-----##########################################################-----//
//-----##########################################################-----//
//  Dineutron decay following Volya's perscription
class StRNGVolya_Dineutron : public StRNG {
public:
  gsl_histogram* fH;
  gsl_histogram_pdf * fP;

  //used volya + bw
  gsl_histogram2d* fH2d;
  gsl_histogram2d_pdf *fP2d;
  
protected:

  double fLower;  /// lower bound 
  double fUpper;  /// upper bound

};
//-----##########################################################-----//

//--------------############################################----------//
///----------------------- aux class for GSL rng --------------------///
//--------------############################################----------//

class StGSLrng {
public:
  /// constructor, takes everything from the environment
  /// \todo rng with given seed...
  StGSLrng();

  /// destructor 
  ~StGSLrng();

  /// get gsl_rng pointer
  gsl_rng* getr() {return fR;}

private:
  /// rng type
  const gsl_rng_type* fT;
  /// rng
  gsl_rng* fR;
  gsl_rng* fR2;

};
//-----##########################################################-----//

/// Gauss RNG using GSL
class StRNGGaussGSL : public StRNGGauss {
public:
  /// constructor
  StRNGGaussGSL(StGSLrng* rr, double c=0, double s=1);
  
  /// destructor
  ~StRNGGaussGSL() {}

  /// return value
  double value ();

private:
  /// GSL random number generator
  StGSLrng* fR;
};
//-----##########################################################-----//

/// Exp RNG using GSL
class StRNGExpGSL : public StRNGExp {
public:
  /// constructor
  StRNGExpGSL(StGSLrng* rr, double c=0);
  
  /// destructor
  ~StRNGExpGSL() {}

  /// return value
  double value ();

private:
  /// GSL random number generator
  StGSLrng* fR;
};
//-----##########################################################-----//

/// asybw RNG using GSL
class StRNGCustombwGSL : public StRNGCustombw {
public:
  /// constructor

  StRNGCustombwGSL(gsl_histogram_pdf* hh, StGSLrng* rr, double c=0, double s=1);
 
  /// destructor
  ~StRNGCustombwGSL() {}

  /// return value
  double value ();

private:
  /// GSL random number generator
  StGSLrng* fR;
  //
  
};
//-----##########################################################-----//

/// Laplace RNG using GSL
class StRNGLaplaceGSL : public StRNGLaplace {
public:

  StRNGLaplaceGSL(StGSLrng* rr, double c=0, double s=1);
  
  /// destructor
  ~StRNGLaplaceGSL() {}

  /// return value
  double value ();

private:
  /// GSL random number generator
  StGSLrng* fR;
};
//-----##########################################################-----//

/// ####DOUBLE LAPLACIAN RNG FOR MONA RESOLUTION##########
class StRNGDoubleLaplaceGSL : public StRNGDoubleLaplace {
public:

  StRNGDoubleLaplaceGSL(StGSLrng* rr, double c1=0, double s1=1, double c2=0, double s2=1,double p=0.66);
  
  /// destructor
  ~StRNGDoubleLaplaceGSL() {}

  /// return value
  double value ();

private:
  /// GSL random number generator
  StGSLrng* fR;
};
//-----##########################################################-----//

/// Cauchy RNG using GSL
class StRNGCauchyGSL : public StRNGCauchy {
public:
  /// constructor
  StRNGCauchyGSL(StGSLrng* rr, double c=0, double s=1);
  
  /// destructor
  ~StRNGCauchyGSL() {}

  /// return value
  double value ();

  void setGamma(double v) { fbwSigma = v/2; }    ///< set gamma

private:
  /// GSL random number generator
  StGSLrng* fR;
};
//-----##########################################################-----//

/// Gamma RNG using GSL
class StRNGGammaGSL : public StRNGGamma {
public:
  StRNGGammaGSL(StGSLrng* rr, double a=0, double b=1);  /// constructor
  ~StRNGGammaGSL() {}  /// destructor

  double value ();  /// return value
private:
  StGSLrng* fR;  /// GSL random number generator
};
//-----##########################################################-----//

/// uniform RNG using GSL
class StRNGUniformGSL : public StRNGUniform {
public:
  StRNGUniformGSL(StGSLrng* rr, double l=0, double u=1);  /// constructor
  ~StRNGUniformGSL() {}  /// destructor

  double value ();  /// return value between fLower and fUpper
  double value (double l, double u);  /// return value between l und u
private:
  StGSLrng* fR;  /// GSL random number generator
};
//-----##########################################################-----//
///Angular Distribution for legendre polynomials///
class StRNGLegendreGSL : public StRNGLegendre {
public:
  StRNGLegendreGSL(StGSLrng* rr, double lvalue); /// constructor
  ~StRNGLegendreGSL() {}  /// destructor

  double value ();  /// return value
 // double value (double l, double u); // return value between l and u

private:
  StGSLrng* fR;  /// GSL random number generator
};

//-----##########################################################-----//

/// Cos_twoneutron RNG using GSL
class StRNGCos_twoneutronGSL : public StRNGUniform {
public:
  StRNGCos_twoneutronGSL(StGSLrng* rr, double l=0, double u=1);  /// constructor
  ~StRNGCos_twoneutronGSL() {}  /// destructor

  double value ();  /// return value between fLower and fUpper
  double value (double l, double u);  /// return value between l und u
private:
  StGSLrng* fR;  /// GSL random number generator
};
//-----##########################################################-----//

/// Custom RNG for E1 function in array
class StRNGCustomE1 : public StRNGCustom_e1 {
public:
  StRNGCustomE1(StGSLrng* rr, double l=0, double u=1);  /// constructor
  ~StRNGCustomE1() {}  /// destructor

  double value ();  /// return value between fLower and fUpper
  double value (double l, double u);  /// return value between l und u
private:
  double x,xe,y,v;
  int xi;
  double fx[101];  // array to contain function in steps

  StGSLrng* fR;  /// GSL random number generator
};
//-----##########################################################-----//

///------- AsymmetricBW RNG using GSL -------////
class StRNGAsymmetricBWGSL : public StRNGAsymmetricBW {
public:
  StRNGAsymmetricBWGSL(StGSLrng* rr, double par1=0, double par2=0,int par3=0,int par4=0,int par5=0); /// constructor
  ~StRNGAsymmetricBWGSL() {}  /// destructor

  double value ();  /// return value between fLower and fUpper
  //double value (double l, double u);  /// return value between l und u
private:
  StGSLrng* fR;  /// GSL random number generator
};
//-----##########################################################-----//

///------- Etotal_11Li RNG using GSL -------////
class StRNGEtotal_11LiGSL : public StRNGEtotal_11Li {
public:
  StRNGEtotal_11LiGSL(StGSLrng* rr, double par1=0); /// constructor
  ~StRNGEtotal_11LiGSL() {}  /// destructor

  double value ();  /// return value between fLower and fUpper
  //double value (double l, double u);  /// return value between l und u
private:
  StGSLrng* fR;  /// GSL random number generator
};
//-----##########################################################-----//

///------- E_12Li RNG using GSL -------////
class StRNGE_12LiGSL : public StRNGE_12Li {
public:
  StRNGE_12LiGSL(StGSLrng* rr, double par1=0); /// constructor
  ~StRNGE_12LiGSL() {}  /// destructor

  double value ();  /// return value between fLower and fUpper
  //double value (double l, double u);  /// return value between l und u
private:
  StGSLrng* fR;  /// GSL random number generator
};
//-----##########################################################-----//
///Angular Distribution for d,p reaction///
class StRNGangular_distribution_dpGSL : public StRNGangular_distribution_dp {
public:
  StRNGangular_distribution_dpGSL(StGSLrng* rr); /// constructor
  ~StRNGangular_distribution_dpGSL() {}  /// destructor

  double value ();  /// return value
private:
  StGSLrng* fR;  /// GSL random number generator
};
//-----##########################################################-----//

///------- TwoNeutron_seq RNG using GSL -------////
class StRNGTwoNeutron_seqGSL : public StRNGTwoNeutron_seq {
public:
  StRNGTwoNeutron_seqGSL(StGSLrng* rr, double par1=0, double par2=0,double par3=0,double par4=0,double par5=0,int par6=1,int beamA=20,int nNeut=0, int nPro=1); /// constructor
  ~StRNGTwoNeutron_seqGSL() {}  /// destructor

  double value ();  /// return value between fLower and fUpper
  double value (double l, double u);  /// return value between l und u
private:
  StGSLrng* fR;  /// GSL random number generator
};
//-----##########################################################-----//

///------- Volya 2nseq using GSL -------////
class StRNGVolya_2nseqGSL : public StRNGVolya_2nseq {
public:
  StRNGVolya_2nseqGSL(StGSLrng* rr, double par1=0, double par2=0,double par3=0,double par4=0,int par6=0, double par7=0, int fragA=20); /// constructor
  ~StRNGVolya_2nseqGSL() {}  /// destructor

  double SPDW(double ee, int L, double fragA, int add);
  double Gamma(double ee, int L, double fragA, int add, double S);
  double GetDecayRate(int L, double Ei, double S, double Ev, double Er, double e, double ep, double sI, double sV, double fragA);
  double value ();  /// return value between fLower and fUpper
  double value (double l, double u);  /// return value between l und u
  void value2d(double &En, double &Erel);

private:
  StGSLrng* fR;  /// GSL random number generator
  StGSLrng* fR2;  /// GSL random number generator
};
//-----##########################################################-----//

///------- Volya 2nseq using GSL -------////
class StRNGVolya_DineutronGSL : public StRNGVolya_Dineutron {
public:
  StRNGVolya_DineutronGSL(StGSLrng* rr, double par1=0, double par2=0,double par3=0, int fragA=20); /// constructor
  ~StRNGVolya_DineutronGSL() {}  /// destructor

  double GetDineutronDecayRate(Double_t FragA, Double_t Ebw, Double_t di_ei, Double_t r0, Double_t RR, Double_t as);

  double value ();  /// return value between fLower and fUpper
  double value (double l, double u);  /// return value between l und u
  void value2d(double &di_ei, double &di_ek);

private:
  StGSLrng* fR;  /// GSL random number generator
  StGSLrng* fR2;  /// GSL random number generator
};
//-----##########################################################-----//


//added by SMM on 10/16/06 for custom excitation energy lineshape
///// custom distro from dat file
class StRNGCustom : public StRNG {
public:

protected:
  // dat file name
  std::string fname;
  TH1F *customLinePdf;
};

//added by SMM on 10/16/06 for custom excitation energy lineshape
/// Custom from dat file
class StRNGCustomGSL : public StRNGCustom {
public:
  /// constructor
  StRNGCustomGSL(StGSLrng* rr, std::string fnme);
  
  /// destructor
  ~StRNGCustomGSL() {}

  /// return value of custom distribution
  double value ();

private:

  /// GSL random number generator
  StGSLrng* fR;

};


//---------#############################################
//----------SWAVE LINESHAPE-------------------

//added by JKS from SMM code on 11 July 2011 for swave lineshape
class swavefct
{
    public:
        double operator() (double *x, double *p);
};

// tophat function for beam distribution
class StRNGswave : public StRNG {

 public:
   StRNGswave(StGSLrng* rr, double gamma, double scatlen);
   //double swave(double *x,double *par);
   double value ();
 protected:
   TF1* swavePdf;
     
};


#endif
