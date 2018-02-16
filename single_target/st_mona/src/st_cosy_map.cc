/** \file 
 * Implementation of cosy stuff
 *
 */
#include <stdio.h>
#include <string.h>
#include "st_cosy_map.hh"

int CosyMapLine::read(string l) {
    int i;
    memset(coef,0,sizeof(coef));
    memset(power,0,sizeof(power));
#define FFF "%14lf "
#define DDD "%1d"
    i = sscanf(l.c_str(),FFF FFF FFF FFF FFF DDD DDD DDD DDD DDD DDD,
           &coef[0], &coef[1], &coef[2], &coef[3], &coef[4],
           &power[0], &power[1], &power[2], &power[3], &power[4], &power[5]); 
    /// \todo check that i == 11
#undef FFF
#undef DDD
    return 0;
  }


int CosyMapLine::print() { 
    for (int i(0); i<5; i++) printf("%14.7g ",coef[i]);
    for (int i(0); i<6; i++) printf("%1d"    ,power[i]);
    printf("\n");
    return 0;
  }


void CosyMap::print() {  /// \todo also print brho and len
    cout << "Size: " << size() << endl;
    for (size_t i(0); i<size(); i++) {
      getLine(i).print();
    }
  }

void CosyMap::read(string ll) {
    string l;
    size_t s1,s2;
    s1=0;
    do {
      s2=ll.find('\n',s1);
      if (s2 == string::npos) break;
      l = ll.substr(s1,s2-s1);
      add(l);
      s1=s2+1;
    } while (1);
  }

