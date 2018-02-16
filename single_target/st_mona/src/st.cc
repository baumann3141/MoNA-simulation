/** \file 
 * Program mainly for testing
 */

#include <iostream>
#include <vector>

#include <stdlib.h>

#include "st_ble.hh"
#include "st_propagate.hh"

#include "st_particle.hh"
#include "st_system.hh"
#include "st_part_init.hh"
#include "st_material.hh"
#include "st_par_mat_ia.hh"
#include "st_reaction.hh"
#include "st_histo_tuple.hh"

#include "st_interp_invers.hh"

#include "deb_err.h"

using namespace std;

/// something test
/// \param argc aaa
/// \param argv bbb
/// \return something
int main (int argc, char** argv) {
  int nevents;  // number of events to simulate

  printf("\nProgram Simple-Track (st)     Heiko Scheit (scheit@nscl.msu.edu)\n");
  printf("Compiled on " __DATE__ " at " __TIME__ ".\n");

  nevents = 10;
  if (argc >1) {
    nevents = atoi(argv[1]);
  }

  // system
  StSystem sys;

#if 0
  // a particle 
  StParticle p;
  // set A, Z and Q
  p.setA(26);             // mass number A
  p.setZ(10);             // proton number Z
  p.setQ(10);             // charge Q
  p.setm(p.getA()*931.494028);  // mass m

  // add one particle to the system 
  sys.addParticle(p);  

  /*
  p.setZ(5);
  p.setQ(5);
  sys.addParticle(p);  
  */

  // GSL random number generator
  StGSLrng* r = new StGSLrng();

  // RNGs for x, y, tx, ty and energy
  StRNG* rx  = new StRNGGaussGSL(r,0,.005);
  StRNG* ry  = new StRNGGaussGSL(r,0,.012);
  StRNG* rtx = new StRNGGaussGSL(r,0,.01);
  StRNG* rty = new StRNGGaussGSL(r,0,.0015);
  StRNG* re  = new StRNGGaussGSL(r,86*26,86*26*0.03);
  StRNGUniform* rUniform = new StRNGUniformGSL(r);
  
  // elements needed
  StElement H1,Be9,Cnat;
  H1.setZ(1);
  H1.setm(1); /// \todo need exact mass here

  Be9.setZ(4);
  Be9.setm(9); /// \todo need exact mass here
  
  Cnat.setZ(6);
  Cnat.setm(12); /// \todo need exact mass here
  
  // some matter
  StMaterial* matBe = new StMaterial();
  matBe->addElement(Be9);
  matBe->setThickness(722);
  matBe->setInteraction(new StRageny);   // interaction for energy loss

  // reaction (in Be target)
  StReacStripEvap* reac = new StReacStripEvap(0.1, 0, 2, 1, -1);
  matBe->addReaction(reac);
  matBe->setRngUniform(rUniform);
  matBe->setRngReac(rUniform);

  StMaterial* matScint    = new StMaterial();
  H1.seta(11);  // abundance
  Cnat.seta(10);
  matScint->addElement(H1);
  matScint->addElement(Cnat);
  matScint->setThickness(60);
  matScint->setInteraction(new StRageny);

  CosyMap m;
  m.setLen(1.5741);
  m.setBrho(3.52319);
  m.read(
"  0.3382809E-01 -1.092335      0.000000      0.000000    -0.3394279     100000\n"
"  0.9135178     0.6304389E-01  0.000000      0.000000    -0.2512429     010000\n"
"   0.000000      0.000000      1.562218     0.7540065      0.000000     001000\n"
"   0.000000      0.000000      2.091716      1.649684      0.000000     000100\n"
"   0.000000      0.000000      0.000000      0.000000      1.000000     000010\n"
"  0.3015744     0.2958402      0.000000      0.000000     0.3632250     000001\n"
"   11.62345      10.34985      0.000000      0.000000     -1.220002     200000\n"
"   15.46044      16.43572      0.000000      0.000000    -0.8632414     110000\n"
"   6.189317      7.656672      0.000000      0.000000    -0.4719861     020000\n"
"   0.000000      0.000000     -39.42424     -44.35280      0.000000     101000\n"
"   0.000000      0.000000     -26.34414     -32.83324      0.000000     011000\n"
"  -17.53898     -18.41260      0.000000      0.000000     0.8923045     002000\n"
"   0.000000      0.000000     -22.23371     -28.48529      0.000000     100100\n"
"   0.000000      0.000000     -17.39411     -24.53789      0.000000     010100\n"
"  -24.48413     -29.29538      0.000000      0.000000     0.5040620     001100\n"
"   1.606873      1.474865      0.000000      0.000000     0.2258942     100001\n"
"   1.073981      2.153976      0.000000      0.000000     0.1528296     010001\n"
"   0.000000      0.000000     -1.982317     -2.565170      0.000000     001001\n"
"  -10.07905     -13.08258      0.000000      0.000000    -0.4418688     000200\n"
"   0.000000      0.000000     -2.752931     -2.670008      0.000000     000101\n"
" -0.1201824     0.8969398E-01  0.000000      0.000000    -0.2962241     000002\n"
"   308.8896      213.3511      0.000000      0.000000     -36.33477     300000\n"
"   411.6606      430.2701      0.000000      0.000000     -31.32121     210000\n"
"   277.9111      403.7419      0.000000      0.000000     -10.73108     120000\n"
"   92.15278      157.8605      0.000000      0.000000     -2.188718     030000\n"
"   0.000000      0.000000     -1316.003     -1229.825      0.000000     201000\n"
"   0.000000      0.000000     -947.8196     -1103.522      0.000000     111000\n"
"   0.000000      0.000000     -324.8601     -522.4812      0.000000     021000\n"
"  -774.2144     -540.8255      0.000000      0.000000      117.9448     102000\n"
"  -501.1311     -704.4570      0.000000      0.000000      42.74263     012000\n"
"   0.000000      0.000000      646.5661      785.1504      0.000000     003000\n"
"   0.000000      0.000000     -503.3401     -587.5367      0.000000     200100\n"
"   0.000000      0.000000     -587.9247     -898.8989      0.000000     110100\n"
"   0.000000      0.000000     -305.6831     -552.2771      0.000000     020100\n"
"  -675.1998     -828.8056      0.000000      0.000000      86.65609     101100\n"
"  -772.4359     -1368.611      0.000000      0.000000      47.31684     011100\n"
"   0.000000      0.000000      985.8357      1500.675      0.000000     002100\n"
"   23.08388      48.55653      0.000000      0.000000     0.4977516E-01 200001\n"
"   46.56644      103.0957      0.000000      0.000000    -0.5435750     110001\n"
"   29.68692      64.86272      0.000000      0.000000    -0.3399501     020001\n"
"   0.000000      0.000000     -28.92266     -93.74801      0.000000     101001\n"
"   0.000000      0.000000     -55.05525     -142.0185      0.000000     011001\n"
"  -55.86764     -123.6402      0.000000      0.000000      2.420899     002001\n"
"  -263.5852     -448.9683      0.000000      0.000000      21.13840     100200\n"
"  -383.4880     -730.6940      0.000000      0.000000      17.85924     010200\n"
"   0.000000      0.000000      713.9965      1256.724      0.000000     001200\n"
"   0.000000      0.000000     -39.05193     -102.6891      0.000000     100101\n"
"   0.000000      0.000000     -64.95447     -153.1087      0.000000     010101\n"
"  -103.1344     -234.2238      0.000000      0.000000      5.048772     001101\n"
"   2.695544      7.307717      0.000000      0.000000    -0.3834659     100002\n"
"   3.952016      9.213578      0.000000      0.000000    -0.3286819     010002\n"
"   0.000000      0.000000     -4.910782     -12.36092      0.000000     001002\n"
"   0.000000      0.000000      215.9479      403.1077      0.000000     000300\n"
"  -50.85031     -116.9805      0.000000      0.000000      3.515450     000201\n"
"   0.000000      0.000000     -4.070939     -12.19732      0.000000     000102\n"
"  0.3354481     0.4711240      0.000000      0.000000     0.2514623     000003\n");


  // initialization
  // first we save the particle as it is now
  sys.addBLE(new StPartInitSave());    

  // then we use randomized x, tx, y, ty and energy
  StPartInitRandomGauss* ini = new StPartInitRandomGauss(rx,rtx,ry,rty,re);
  sys.addBLE(ini);

  // the Be
  sys.addBLE(matBe);
  
  StPropMapCosy* dipole = new StPropMapCosy(m);
  sys.addBLE(dipole);
  StPropDrift* d = new StPropDrift(8.17 - m.getLen());
  sys.addBLE(d);

  // setup GSL n-tuple for writing
  //  StGSLTuple tup("tuple_file.dat");
  StROOTTuple tup("tuple_file.root");
  sys.setTupleP(&tup);

  // init
  ini->addNtupFields("x");
  ini->addNtupFields("tx");
  ini->addNtupFields("y");
  ini->addNtupFields("ty");
  ini->addNtupFields("GammaMinOneU");

  // after target target
  matBe->addNtupFields("x",0);
  matBe->addNtupFields("tx",0);
  matBe->addNtupFields("y",0);
  matBe->addNtupFields("ty",0);
  matBe->addNtupFields("t",0);
  matBe->addNtupFields("z",0);
  /// \todo check why crash with this line uncommented
  /// \todo think some more about how to do this!!!
  //  matBe->addNtupFields("dE",1);  
  matBe->addNtupFields("dE",0);  
  matBe->addNtupFields("GammaMinOneU",0);
  matBe->addNtupFields("TP",0);  
  //  matBe->addNtupFields("TP",1);  

  // after dipole (at CRDC1)
  dipole->addNtupFields("Brho",0);
  dipole->addNtupFields("x",0);
  dipole->addNtupFields("tx",0);
  dipole->addNtupFields("y",0);
  dipole->addNtupFields("ty",0);
  dipole->addNtupFields("t",0);

  // MoNA
  d->addNtupFields("t",1);
  d->addNtupFields("x",1);
  d->addNtupFields("y",1);
  d->addNtupFields("z",1);
  
#endif
  printf("\n\nNow two simulations are run with information printed on screen:\n");
  sys.run(2, "print");

  printf("Now running simulation  %d times.\n", nevents);
  sys.run(nevents);

  return 0;
}

