/** \file
 * Things for creating and filling of historgrams and n-tuples or trees.
 *
 * \author Heiko Scheit
 *
 */
 
#ifndef ST_HISTO_TUPLE_HH
#define ST_HISTO_TUPLE_HH

#include <map>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

#include <gsl/gsl_ntuple.h>

#include "st_det_resp.hh"

/// histo base class
class StHisto {
};


/** pure virtual class to handle 1D histograms 
 * \todo check in root what else is/might be needed 
 */
class StHisto1D : public StHisto {
public:
  virtual void increment(double) = 0; ///< increment  \todo use double here???
  //  virtual int getBin(double) = 0;    ///< get bin content
  /* 
     getOverFl() = 0 ;      ///< get number of over flows
     getUnderFl() = 0 ;     ///< get number of under flows
  */
};


/** pure virtual class to handle 2D histograms
 * \todo check in root what else is/might be needed 
 */
class StHisto2D : public StHisto {
public:
  virtual void increment(double,double) = 0;        ///< increment  \todo use double here???
  //  getBinContent(double,double) = 0;    ///< get bin content
  /*
  getOverFlX() = 0 ;      ///< get number of over flows in X
  getUnderFlX() = 0 ;     ///< get number of under flows in X
  getOverFlY() = 0 ;      ///< get number of over flows in Y
  getUnderFlY() = 0 ;     ///< get number of under flows in Y
  */
};


/** manager for histograms */
class StHistoManager {
public:
  virtual void add() = 0;       ///< add new histo field

  virtual ~StHistoManager() = 0; ///< destructor; should delete all histos

private:
  map<string , StHisto*> id;  ///< unique identifier
};




/* #####################################  n-tuple stuff ###  */

/** Pure virtual n-tuple class.
 *
 */
class StTuple {
public:
  /// Constructor 
  StTuple(string name) : fileName(name), fillCalled(0) {}

  /// add field with (unique) name, and set possible detector response
  void addField(string name, StDetResp* r) {
    DSV(r);
    fDetResp[name] = r;
    addField(name);
  }

private:
  /// add field with (unique) name, and set possible detector response
  virtual void addField(string name)  = 0; 

public:
  /// fill value 'v' into field with name 'name' after applying detector response
  void fillField(string name, double v) {
    StDetResp* r = fDetResp[name];
    DSV(name);
    DSV(v);
    DSV(r);
    if (! r) return fillFieldAfterResponse(name,v);
    double vv;
    vv = (*r)(v);
    DSV(vv);
    DSV(vv-v);
    return fillFieldAfterResponse(name,vv);
  }

private:
  ///< fill value 'v' into field with name 'name'
  virtual void fillFieldAfterResponse(string name, double v) = 0; 

public:
  virtual void fill() = 0; ///< write n-tuple row; first call to this will disable addField()

  /// Destructor
  virtual ~StTuple() {}

protected:
  string fileName;  ///< file name for n-tuple data
  int fillCalled;   ///< flag, set to one once filling started
private:
  map<string, StDetResp*> fDetResp;  ///< map between name and detector response
};





/// maximum size of data-arrary for GSL n-tuple
#define MAXDATA 1000  /// \todo fix this: make any length possible



/** GSL ntuple (derived from StTuple)
 *
 */
class StGSLTuple : public StTuple {
public:

  /// Constructor.  Calls base class constructor StTuple
  StGSLTuple(string name) : StTuple(name), idata(0), ntuple(0) {}


  /** add named field 
      \param name Name of the field.
  */
  void addField(string name);         
  void fillFieldAfterResponse(string name, double v);  ///< fill name field with value

  void fill();                            ///< write n-tuple row to file

  /// Destructor.
  ~StGSLTuple ();

private:
  map<string, double*> idd;    ///< map between fieldnames and pointer to n-tuple fields
  vector <string> fieldNames;  ///< string with the field names
  double dataRow[MAXDATA];     ///< data array correspondong to n-tuple row
  size_t idata;                ///< used as index for dataRow
  gsl_ntuple* ntuple;          ///< ntuple pointer
};




#ifdef HAVE_ROOT

#include "TFile.h"
#include "TTree.h"

/** ROOT tree (derived from StTuple)
 *
 */
class StROOTTuple : public StTuple {
public:

  /// Constructor.  Calls base class constructor StTuple
  StROOTTuple(string name);


  /** add named field 
      \param name Name of the field.
  */
  void addField(string name);         
  void fillFieldAfterResponse(string name, double v);  ///< fill name field with value

  void fill();                            ///< write n-tuple row to file

  /// Destructor.
  ~StROOTTuple ();

private:
  map<string, double*> idd;    ///< map between fieldnames and pointer to n-tuple fields
  vector <string> fieldNames;  ///< string with the field names
  double dataRow[MAXDATA];     ///< data array correspondong to n-tuple row
  size_t idata;                ///< used as index for dataRow
  TTree* t;       ///< tree
  TFile* f;       ///< file for tree
};
#endif // HAVE_ROOT

#endif // ST_HISTO_TUPLE_HH

