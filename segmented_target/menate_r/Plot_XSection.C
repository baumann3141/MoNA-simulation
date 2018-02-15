#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TAxis.h"
#include <cstring>
#include "TColor.h"
#include "TF1.h"
#include "TCanvas.h"

/*    Cinelastic->Draw("ALP");
    Cnnp->Draw("LP");
    Cnp->Draw("LP");
    Cnng->Draw("LP");
    Cnn3a->Draw("LP");
    Cn2n->Draw("LP");
    Cna->Draw("LP");
    JENDL->Draw("LP");
*/

TGraph* Cnnp, *Cnp, *Cnng, *Cnn3a, *Cn2n, *Cna, *JENDL;

using namespace std;

TGraphErrors *ReadDataErr(string filename){

    ifstream inputFile(filename.c_str());
    //ifstream inputFile("Carbon12_np12B.dat");
    string line;

    vector<double> Evec;
    vector<double> Xvec;
    vector<double> Xvec_err;

    while(getline(inputFile, line)){

    if(!line.length() || line[0] == '#') continue;
    istringstream iss(line);

    double E=0, Xsection = 0;
    double Xsection_err = 0;

  
    iss >> E >> Xsection >> Xsection_err;

    if(E!=0){
        Evec.push_back(E);
        Xvec.push_back(Xsection);
        Xvec_err.push_back(Xsection_err);
    }

    cout << "point: " << E << " \t" << Xsection << endl;

    }

    int points = Evec.size();

    cout << "# of points: " << points << endl;

    const int arr_size = points;


    double Earr[arr_size];
    double Xarr[arr_size];

    double Eerr[arr_size];
    double Xerr[arr_size];

    
    for(int i=0;i<arr_size;i++){
            Earr[i] = Evec[i];
            Xarr[i] = Xvec[i];
            Xerr[i] = Xvec_err[i];
            Eerr[i] = 0;
    }
    
    //
    TGraphErrors *graph = new TGraphErrors(points, Earr, Xarr, Eerr, Xerr);
    graph->GetYaxis()->SetRangeUser(0.001,0.720);
    graph->GetXaxis()->SetRangeUser(1,100);
    graph->SetMarkerSize(0.5);
    graph->SetLineWidth(1);
    graph->SetMarkerStyle(22);
    
    return graph;


}

TGraph* ReadData(string filename){

    ifstream inputFile(filename.c_str());
    //ifstream inputFile("Carbon12_np12B.dat");
    string line;

    vector<double> Evec;
    vector<double> Xvec;
    vector<double> Xvec_err;

    while(getline(inputFile, line)){

    if(!line.length() || line[0] == '#') continue;
    istringstream iss(line);

    double E=0, Xsection = 0;
    double Xsection_err = 0;

  
    iss >> E >> Xsection;

    if(E!=0){
        Evec.push_back(E);
        Xvec.push_back(Xsection);
        //Xvec_err.push_back(Xsection_err);
    }

    cout << "point: " << E << " \t" << Xsection << endl;

    }

    int points = Evec.size();

    cout << "# of points: " << points << endl;

    const int arr_size = points;


    double Earr[arr_size];
    double Xarr[arr_size];

    double Eerr[arr_size];
    double Xerr[arr_size];

    
    for(int i=0;i<arr_size;i++){
            Earr[i] = Evec[i];
            Xarr[i] = Xvec[i];
            //Xerr[i] = Xvec_err[i];
            //Eerr[i] = 0;
    }
    
    //
    TGraph *graph = new TGraph(points, Earr, Xarr);
    graph->GetYaxis()->SetRangeUser(0.001,0.720);
    graph->GetXaxis()->SetRangeUser(1,100);
    graph->SetMarkerSize(0.5);
    graph->SetLineWidth(2);
    graph->SetMarkerStyle(20);
    
    return graph;
    
    


}

double interpolate(double *x, double *par){

    double val = 0;
    double arg = x[0];

     
    val =  par[0]*(  Cnnp->Eval(arg,0,"") ); // linear interp, use "S" option for spline
    
    val += par[1]*(   Cnp->Eval(arg,0,"") );
    val += par[2]*(  Cnng->Eval(arg,0,"") );
    val += par[3]*( Cnn3a->Eval(arg,0,"") );
    val += par[4]*(  Cn2n->Eval(arg,0,"") );
    val += par[5]*(   Cna->Eval(arg,0,"") );    

    //val = val* par[6];
    
    return val;


}

void Scale(TGraph *graph, double scale){

    for(int i=0;i<graph->GetN();i++){
        graph->GetY()[i] *= scale;
    }


};



void Plot_XSection(){

    TGraph *Celastic = new TGraph();
    Celastic = ReadData("Carbon12_el.dat");
    
 
    TGraphErrors *Cinelastic = new TGraphErrors();
    Cinelastic = ReadDataErr("Carbon12_inel.dat");


    //TGraph *Cnnp = new TGraph();
    Cnnp = ReadData("Carbon12_nnp11B.dat");
    Cnnp->SetMarkerStyle(21);
    Cnnp->SetLineColor(kOrange+1);


    //TGraph *Cnp = new TGraph();
    Cnp = ReadData("Carbon12_np12B.dat");
    Cnp->SetMarkerStyle(21);
    Cnp->SetLineColor(4);
    Cnp->SetLineStyle(2);

    //TGraph *Cnng = new TGraph();
    Cnng = ReadData("Carbon12_nng4_4.dat");
    Cnng->SetLineColor(2);

    //TGraph *Cnn3a = new TGraph();
    Cnn3a = ReadData("Carbon12_nn3a.dat");
    Cnn3a->SetLineColor(8);

    //TGraph *Cn2n = new TGraph();
    Cn2n = ReadData("Carbon12_2n11C.dat");
    Cn2n->SetLineColor(kMagenta);

    //TGraph *Cna = new TGraph();
    Cna = ReadData("Carbon12_na9Be.dat");
    Cna->SetLineColor(kViolet);
    

    TGraph *JENDL = new TGraph();
    JENDL = ReadData("C12_inel_JENDL.dat");
    //JENDL = ReadData("C12_non_elastic.dat");
    JENDL->SetLineWidth(2);
    //JENDL->SetLineColor(4);
    //JENDL->SetLineStyle(2);
    //

    TGraph *non_el = new TGraph();
    non_el = ReadData("C12_non_elastic.dat");
    non_el->SetLineWidth(2);
    

    Cinelastic->SetTitle("12C(n,inl)* ");
    Cinelastic->GetYaxis()->CenterTitle();
    Cinelastic->GetYaxis()->SetTitle("#sigma [b]");
    Cinelastic->GetXaxis()->SetTitle("T_{n} [MeV]");

    TCanvas *c1 = new TCanvas("c1","c1",1400,650);
    c1->Divide(2,1);

    c1->cd(1);

    //Celastic->Draw("ALP");
    Cinelastic->Draw("ALP");
    Cnnp->Draw("LP");
    Cnp->Draw("LP");
    Cnng->Draw("LP");
    Cnn3a->Draw("LP");
    Cn2n->Draw("LP");
    Cna->Draw("LP");
    //JENDL->Draw("LP");

    non_el->Draw("LP");



    TF1 *initial = new TF1("initial",interpolate,0,100,6);
    for(int i=0;i<6;i++){
        initial->SetParameter(i,1);
    }
    initial->SetLineColor(4);
    initial->SetLineWidth(2);

    initial->Draw("SAME");


    TF1 *sum = new TF1("sum",interpolate,0,100,6);
/*
        val =  par[0]*(  Cnnp->Eval(arg,0,"") ); // linear interp, use "S" option for spline
    
    val += par[1]*(   Cnp->Eval(arg,0,"") );
    val += par[2]*(  Cnng->Eval(arg,0,"") );
    val += par[3]*( Cnn3a->Eval(arg,0,"") );
    val += par[4]*(  Cn2n->Eval(arg,0,"") );
    val += par[5]*(   Cna->Eval(arg,0,"") );    */

    // par[0] = Cnnp (ORANGE)
    // par[1] = Cnp (BLUE)
    // par[2] = Cnng (RED)
    // par[3] = Cnn3a (GREEN)
    // par[4] = C2n (PINK)
    // par[5] = Cna (PURPLE)
    
    for(int i=0;i<6;i++){
        sum->SetParameter(i,1);
       // if(i==0) sum->FixParameter(i,0.5);
       sum->SetParLimits(i,1e-2,100);
    
    }
    
    
    sum->FixParameter(0,0);//orange (n,np)
    sum->FixParameter(1,1);//4.93/70//blue (n,p)
    //sum->FixParameter(1,0);// 
    sum->FixParameter(2,1);//red (n,ng)
    sum->FixParameter(3,1);//9.8/14//green (n,n3a)
    sum->FixParameter(4,1);//pink (n,2n)
    sum->FixParameter(5,1);//purple (n,a)

    // fit relative to (n,2n);
    //sum->FixParameter(0,sum->GetParameter(4));
    //sum->FixParameter(1,sum->GetParameter(4));
    //sum->FixParameter(3,sum->GetParameter(4));
    //sum->FixParameter(5,sum->GetParameter(4));

    
    sum->SetLineColor(4);
    initial->Draw("SAME");

    JENDL->Fit("sum","R0");
    //Cinelastic->Fit("sum","R0");
    //non_el->Fit("sum","R0");


    TGraph *cCnnp = (TGraph*)Cnnp->Clone();
    Scale(cCnnp, sum->GetParameter(0));

    TGraph* cCnp = (TGraph*)Cnp->Clone();
    Scale(cCnp, sum->GetParameter(1));

    TGraph *cCnng = (TGraph*)Cnng->Clone();
    Scale(cCnng, sum->GetParameter(2));

    TGraph *cCnn3a = (TGraph*)Cnn3a->Clone();
    Scale(cCnn3a, sum->GetParameter(3));
   
    TGraph *cCn2n = (TGraph*)Cn2n->Clone();
    Scale(cCn2n, sum->GetParameter(4));

    TGraph *cCna = (TGraph*)Cna->Clone();
    Scale(cCna, sum->GetParameter(5));


    
    
    c1->cd(2);
    //Celastic->Draw("ALP");
    Cinelastic->Draw("ALP");
    //non_el->Draw("LP");
    cCnnp->Draw("LP");
    cCnp->Draw("LP");
    cCnng->Draw("LP");
    cCnn3a->Draw("LP");
    cCn2n->Draw("LP");
    cCna->Draw("LP");
    JENDL->Draw("LP");
    sum->Draw("SAME");


  





}
