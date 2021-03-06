void ShowSWAVE(){


  Double_t par[2] = {0.87, -18.7};
  //Double_t par[2] = {1.847, -2.7};
  
  TGraph *gswave = new TGraph();
  int p = 0;
  for(double e=0.01; e<11; e+=0.01){
    // see SMM logbook fri 2/25/11 to see where this formula comes from
    double hbar = 6.5821; //hbar in fun units with powers pre-cancelled
    double mu = 1.0432;   // mass of neutron in MeV/c^2 with c division and powers pre-done
    double k = sqrt(2.0*mu*e)/hbar; // wave number

    //zwk changed calculation of gamma such that the input is now the neutron seperation energy from the projectile in MeV
    double gamma = sqrt(2*par[0]*939.565)/197.0;
    //cout << gamma << endl;
    double scatlen = par[1];
    double val = k*pow(1.0/(gamma*gamma + k*k),2)*pow(cos(k*scatlen) - (gamma/k)*sin(k*scatlen),2);

    gswave->SetPoint(p, e, val/1.2);
    cout << e << " " << val << endl;
    p++;
  }


  gswave->SetLineWidth(3);
  //gswave->Draw("al");



  TGraph *gfull = new TGraph("/projects/mona-sim/kohley/swave_fullcalc/Li11+n/cs/phase_ang_4.75.dat.cross");

  gfull->SetLineWidth(3);
  gfull->SetLineColor(3);
  //gfull->Draw("l");

  TGraph *gs = new TGraph();
  NormGraph(gswave,gs);
  TGraph *gfulls = new TGraph();
  NormGraph(gfull, gfulls);

  gfulls->Draw("al");
  gs->Draw("l");

  gfulls->SetLineColor(3);
}


void DrawFullSWave(){

  TGraph *g1t = new TGraph("/projects/mona-sim/kohley/swave_fullcalc/Li11+n/cs_anc1.31/phase_ang_2.0.dat.cross");
  TGraph *g1 = new TGraph();
  NormGraph(g1t,g1);

  TGraph *g2t = new TGraph("/projects/mona-sim/kohley/swave_fullcalc/Li11+n/cs/phase_ang_2.0.dat.cross");
  TGraph *g2 = new TGraph();
  NormGraph(g2t,g2);

  TGraph *g3t = new TGraph("/projects/mona-sim/kohley/swave_fullcalc/Li11+n/cs/phase_ang_4.75.dat.cross");
  TGraph *g3 = new TGraph();
  NormGraph(g3t,g3);
  


  g1->SetLineWidth(3);
  g1->SetLineColor(1);
  g1->Draw("al");

  g2->SetLineWidth(3);
  g2->SetLineColor(2);
  g2->Draw("l");
  
  //g3->SetLineWidth(3);
  //g3->SetLineColor(3);
  //g3->Draw("l");
  
  

}


void NormGraph(TGraph *g1t, TGraph *g1){

  Double_t max = -1;

  for(int i=0; i<g1t->GetN(); i++){

    Double_t x, y,;
    g1t->GetPoint(i, x, y);

    if(y>max) max = y;
  }


  for(int i=0; i<g1t->GetN(); i++){

    Double_t x, y,;
    g1t->GetPoint(i, x, y);
    g1->SetPoint(i, x, y/max);

  }


}
