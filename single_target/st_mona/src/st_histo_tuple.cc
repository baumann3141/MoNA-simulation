/** \file
 * implementation of histo and tuple stuff
 *
 */
 
#include "st_histo_tuple.hh"

#include "deb_err.h"


void StGSLTuple::addField(string name) {
  if (fillCalled) ERR("N-tuple filling started already.  New fields cannot be added");

  if (! idd[name]) {
    idd[name] = &dataRow[idata];
    dataRow[idata++]=0;
    fieldNames.push_back(name);
  } else {
    ERR("field with that name exists already");
  }
}


void StGSLTuple::fillFieldAfterResponse(string name, double v) {
  double* p;
  p=idd[name];

  if (!p) ERR("Field with name %s does not exist.",name.c_str());

  *p = v;
}


void StGSLTuple::fill() {
  if ( ! fillCalled ) {
    ntuple = gsl_ntuple_create ((char*) fileName.c_str(), dataRow, sizeof(double)*idata);
    if (!ntuple) ERR("gsl-ntuple allocation faild");
    fillCalled = 1;
    // file with description of ntuple
    string f = fileName + ".dsc";
    FILE* fp = fopen(f.c_str(),"w");
    if (!fp) SERR("Could not open file %s", f.c_str());
    int n = 0;
    for (vector<string>::const_iterator i=fieldNames.begin(); i!=fieldNames.end(); i++) {
      fprintf(fp, "%d: %s\n",n++,i->c_str());
    }
    fclose(fp);
  }

  // fill n-tuple
  gsl_ntuple_write(ntuple);

  // reset data array
  memset(dataRow,0,sizeof(double)*idata); 
}


StGSLTuple::~StGSLTuple() {
  if (ntuple) gsl_ntuple_close (ntuple);
}





#ifdef HAVE_ROOT

StROOTTuple::StROOTTuple(string name)  : StTuple(name), idata(0), t(0), f(0) {
  f = new TFile(name.c_str(),"update");  
  f->cd();
  t = new TTree("t","simple-track tree");
}

void StROOTTuple::addField(string name) {
  if (fillCalled) ERR("Tree filling started already.  New fields cannot be added");

  if (! idd[name]) {
    idd[name] = &dataRow[idata];
    string rname = "b";
    rname += name.substr(0,name.find(":"));
    rname += "p" + name.substr(name.find(":")+1,name.find("-")-name.find(":")-1);
    rname += name.substr(name.find("-")+1);
    DSV(rname);
    string cname = rname + "/D";
    t->Branch(rname.c_str(),idd[name],cname.c_str());
    dataRow[idata++]=0;
    fieldNames.push_back(name);
  } else {
    
    cout << "Instead of a uniformative error, here is the freaking name: " << name << endl;
 
    ERR("field with that name exists already");
 }
}


void StROOTTuple::fillFieldAfterResponse(string name, double v) {
  double* p;
  DSV(name);
  p=idd[name];

  if (!p) ERR("Field with name \"%s\" does not exist.",name.c_str());

  *p = v;
}


void StROOTTuple::fill() {
  if ( ! fillCalled ) {
    fillCalled = 1;
  }

  // fill tree
  t->Fill();
  
  // reset data array  
  memset(dataRow,0,sizeof(double)*idata); 
}


StROOTTuple::~StROOTTuple() {
  DEB("Closing");
  //  t->Write();
  f->Write();
  f->Close();
}

#endif // HAVE_ROOT

