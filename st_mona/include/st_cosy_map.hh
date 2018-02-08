/** \file 
 *  Cosy stuff
 */

#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>

using namespace std;

/// corresponds to one line of a map file
class CosyMapLine {
public:
  int read(string l);
  int print();

public: /// \todo make these private
  double coef[5]; ///< coefficients for x, tx, y, ty, l
  int power[6];   ///< power for        x, tx, y, ty, length and delta
};


/// a full cosy map is just a vector of all CosyMapLines
class CosyMap {
public:
  CosyMap() : fLen(0), fBrho(0), fA(0), fQ(0) {}
  //@{
  /// add a line
  void add(CosyMapLine l)    { fM.push_back(l); }
  void add(string l)         { CosyMapLine m; m.read(l); add(m); }
  //@}

  /// print map
  void print();

  /// read whole map file in string ll
  void read(string ll);

  //@{
  /// get/set Brho 
  void   setBrho(double d) {        fBrho = d; }
  double getBrho()         { return fBrho;     }
  //@}
  
  //@{
  /// get/set central track length
  void   setLen(double l ) {        fLen  = l; }
  double getLen()          { return fLen;      }
  //@}

  //@{
  /// set/get A and Q
  void setA(int v) {        fA = v;    }
  int  getA()      { return fA;        }
  void setQ(int v) {        fQ = v;    }
  int  getQ()      { return fQ;        }
  //@}
  
  /// number lines
  size_t size()         { return fM.size(); }
  
  /// return certain map line
  CosyMapLine getLine(int i) { return fM.at(i); }

private:
  vector<CosyMapLine> fM;  ///< vector of map lines
  double fLen;            ///< nominal path length (central track path length)
  double fBrho;           ///< nominal brho
  int fA;              ///< nominal A
  int fQ;              ///< nominal Q
};


