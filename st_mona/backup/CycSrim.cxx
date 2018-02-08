//  $Id: CycSrim.cxx,v 1.15 2011/11/03 16:15:23 hagel Exp $
//
#include "CycSrim.h" 
#ifdef __CINT__
#endif
//#include "RooMath.h"
//TEST
#include "TGraph.h"
//END TEST

#include <fstream>
#include <iostream>
using std::ios;
using namespace std;

ClassImp(CycSrim)

   struct stoppingStruct {
      Double_t eOverA;
      Double_t dEdxElectronic;
      Double_t dEdxNuclear;
      Double_t range;
      Double_t stragglingLongitudinal;
      Double_t stragglingLateral;
      };

//When adding a new file, don't forget to modify Makefile.am to have it
//installed.
  const Char_t * srimfile[] = {
     "SiliconStopping.dat", 
     "MylarStopping.dat", 
     "IsobutaneStopping.dat",
     "PentaneStopping.dat",
     "AluminumStopping.dat",
     "NickelStopping.dat",
     "BC420Stopping.dat",
     "CsIStopping.dat",
     "CarbonStopping.dat",
     "AuStopping.dat",
     "PolypropyleneStopping.dat",
     "LowDensityPolyethyleneStopping.dat",
     "ThStopping.dat",
     "BerylliumStopping.dat"
     };


CycSrim::CycSrim(Int_t imat, Double_t thickness, srimDensityUnits units) {
   //Normal constructor for si detector, but could be used for crystal
   //as well
   
   fMaterial  = imat;
   Init();
   fMaterialFactor = 1.0;
   if(units == kUnitsMgCm2) {
      //This is the easiest.
      fMaterialFactor = 1.0;
      }
   if(units == kUnitsMicron) {
      //Convert thickness in um to mg/cm^2
      //1.e-4 factor is to convert um to cm
      //1000. is to convert g/cm^3 of density to mg/cm^3
      fMaterialFactor = 1.e-4 * fMaterialDensity * 1000.;
      }
   else if(units == kUnitsCm) {
      //Convert thickness in cm to mg/cm^2
      //1000. is to convert g/cm^3 of density to mg/cm^3
      fMaterialFactor = fMaterialDensity * 1000.;
      }
   else if(units == kUnitsTorr || units == kUnitsMbar) {
      Error("CycSrim","Cannot specify torr in this constructor!");
      }
   fThickness = thickness * fMaterialFactor;
}

CycSrim::CycSrim(Int_t imat, Double_t thickness, Double_t pressure, srimDensityUnits units) {
   //Normal constructor for gas detector
   
   fMaterial  = imat;
   Init();
   fMaterialFactor = 1.0;
   if(units == kUnitsMicron) {
      Warning("CycSrim","Cannot specify micron in gas detector constructor");
      }
   else if(units == kUnitsCm) {
      Warning("CycSrim","Cannot specify cm in gas detector constructor");
      }
   else if(units == kUnitsTorr) {
      //convert from cm at pressure in torr go mg/cm^2
      //thickness units are assumed to be in cm
      //Factor of 273./298. is to go from STP to room temperature 25C
      //pressure/760 is to go from STP to pressure of gas chamber
      //1000. is to convert from g/cm^2 to mg/cm^2
      fMaterialFactor = fMaterialDensity * 1000.* (273./298.)*pressure/760.;
      }
   else if(units == kUnitsMbar) {
      //convert from cm at pressure in mbar go mg/cm^2
      //thickness units are assumed to be in cm
      //Factor of 273./298. is to go from STP to room temperature 25C
      //pressure/1013 is to go from STP to pressure of gas chamber
      //1000. is to convert from g/cm^2 to mg/cm^2
      fMaterialFactor = fMaterialDensity*1000.*(273./298.)*pressure/1013.25;
      }
   fThickness = thickness * fMaterialFactor;
}

Int_t CycSrim::Init() {
   //cout << "Reading the stopping file " << endl;
   //Read the stopping file
   ifstream stoppingFile;
   const Char_t *srimFile;
   srimFile = Form("%s/srim/%s",GetSrimDataDir(),srimfile[fMaterial]); //cout << "DataDir:" << srimFile << endl;
   stoppingFile.open(srimFile,ios::in);

   TString stoppingLine;

   stoppingLine.ReadLine(stoppingFile);
   Char_t material[64];
   sscanf(stoppingLine.Data(),"Material: %s , density = %f g/cm^3",material,&fMaterialDensity);
   fMaterialName = material;

   stoppingLine.ReadLine(stoppingFile);
   sscanf(stoppingLine.Data(),"Absorber is made of %d elements",&fNumMaterials);

   Int_t z;
   for(Int_t i=0;i<fNumMaterials;i++) {
      stoppingLine.ReadLine(stoppingFile);
      Float_t percent,a;
      sscanf(stoppingLine.Data(),"%s %f%% z = %d, a = %f",material,&percent,&z,&a);
      fMaterialComposition[i][0] = z;
      fMaterialComposition[i][1] = a;
      fMaterialComposition[i][2] = percent;
      //printf("z = %d, a = %f, percent = %f\n",z,a,percent);
      }

   for(z=1;z<93;z++) {
      Int_t num;
      stoppingLine.ReadLine(stoppingFile);
      sscanf(stoppingLine.Data(),"Z = %d, num = %d",&z,&num);
      fNumStopping[z-1] = num; //cout << "fNumStopping[z-1]: " << fNumStopping[z-1] << "num" << num << endl;
      for(Int_t ie = 0;ie<num;ie++) {
	 stoppingLine.ReadLine(stoppingFile);
	 Double_t eOverA;
	 Double_t dEdxElectronic;
	 Double_t dEdxNuclear;
	 Double_t range;
	 Double_t stragglingLongitudinal;
	 Double_t stragglingLateral;
	 sscanf(stoppingLine.Data(),"%lf %lf %lf %lf %lf %lf",&eOverA,&dEdxElectronic,&dEdxNuclear,&range,&stragglingLongitudinal,&stragglingLateral);
	 stoppingStruct *stop_p = (stoppingStruct*)fStopping[z-1][ie];
	 //printf("z = %d, eOverA = %f, range = %f, lateral strag = %f\n",z,eOverA,range,stragglingLateral);
	 stop_p->eOverA                 = eOverA;
	 stop_p->dEdxElectronic         = dEdxElectronic;
	 stop_p->dEdxNuclear            = dEdxNuclear;
	 stop_p->range                  = range;
	 stop_p->stragglingLongitudinal = stragglingLongitudinal;
	 stop_p->stragglingLateral      = stragglingLateral;
         }
      }

   stoppingFile.close();
}


Double_t CycSrim::GetEnergyLoss(Int_t z, Double_t a, Double_t e) {
   //Return energy loss in absorber.
   //Input is z, a, total energy of ion into absorber
   Double_t eres = GetResidualEnergy(z,a,e);
   Double_t eloss = e - eres;
   cout << " e " << e << " eres " << eres << endl;
   return eloss;
}

Double_t CycSrim::GetEnergyLossFromResidual(Int_t z, Double_t a, Double_t e) {
   //Return energy loss in absorber.
   //Input is z, a, residual energy of ion after passing through absorber

   Double_t e_a = e / a;

   const Int_t numE = fNumStopping[z-1];
   Double_t ranges[numE];
   Double_t evals[numE];

   for(Int_t ie=0;ie<numE;ie++) {
      stoppingStruct *stop_p = (stoppingStruct*)fStopping[z-1][ie];
      evals[ie] = stop_p->eOverA;
      ranges[ie] = stop_p->range;
      }
   
   //Get the residual range in this medium
   //This gives us R (total range) - t (thickness of absorber)
   Double_t rangeRes = Interp(evals,ranges,numE,2,e_a);
   //Double_t rangeRes = RooMath::interpolate(evals,ranges,numE,e_a);

   //Now, add the thickness of the absorber.
   Double_t range = rangeRes + fThickness;
   Double_t etotOverA = Interp(ranges,evals,numE,2,range);

   Double_t eloss = (etotOverA - e_a)*a;

   return eloss;
}

Double_t CycSrim::GetResidualEnergy(Int_t z, Double_t a, Double_t e) {
   //Return residual energy after loss in absorber.
   //Input is z, a, total energy of ion into absorber
   cout << "Entering CycSrim::GetResidualEnergy" << endl;
   Double_t e_a = e / a;
   //cout << "fNumStopping[z-1]" << fNumStopping[z-1] << endl;
   const Int_t numE = fNumStopping[z-1];
   Double_t ranges[numE];
   Double_t evals[numE];
    //cout << "Segfault before the for loop" << endl;
   for(Int_t ie=0;ie<numE;ie++) {
      stoppingStruct *stop_p = (stoppingStruct*)fStopping[z-1][ie];
      evals[ie] = stop_p->eOverA; //cout << "evals: " << evals[ie] << endl;
      ranges[ie] = stop_p->range; //cout << "ranges: " << ranges[ie] << endl;
      }
   // cout << "Segfault here?" << endl;
   //If no energy in, then no residual energy.
   if(e_a < evals[0]) return 0.;

   //Get total range in this medium
   Double_t range = Interp(evals,ranges,numE,2,e_a);

//    printf("e_a = %f\n",e_a);
//    for(Int_t i=0;i<numE;i++) {
//       printf("evals[%d] = %f, ranges[%d] = %f\n",i,evals[i],i,ranges[i]);
//       }

   //Calculate how far it traveled past the thickness of the detector.
   Double_t gtr = range - fThickness;

    printf("Range = %f, thickness = %f, gtr = %f\n",range,fThickness,gtr);
   //Now, interpolate and see what energy corresponds to that.
   //This will give us the energy out of the detector.
   Double_t eOverAOut = 0.0;
   if(gtr < 0.0) {
      //Means it stopped in the detector
      eOverAOut = 0.0;
      }
   else if(gtr > ranges[0]) {
      eOverAOut = Interp(ranges,evals,numE,2,gtr);
      //eOverAOut = Intero(ranges,evals,numE,2,gtr);

      }
   else {
      eOverAOut = Interp(ranges,evals,numE,2,gtr);
      //eOverAOut = Intero(ranges,evals,numE,2,gtr);

      }

   return eOverAOut * a;
}

Double_t CycSrim::GetIncidentEnergy(Int_t z, Double_t a, Double_t de) {
   //Return incident energy for energy loss 
   //if particle punched through absorber.

   const Int_t numE = fNumStopping[z-1];
   Double_t deltaE[numE];
   Double_t ein[numE];

   Int_t num = 0;
   for(Int_t ie=numE-1;ie>=0;ie--) {
      stoppingStruct *stop_p = (stoppingStruct*)fStopping[z-1][ie];
      Double_t e = stop_p->eOverA;

      ein[num]    = e*a;
      deltaE[num] = ein[num] - GetResidualEnergy(z,a,e*a);
      num++;

      if(deltaE[num-1] > de) break;
      }

   Double_t eIncident;
   if(num > 1) {
      eIncident = Interp(deltaE,ein,num,2,de);
      }
   else {
      //Warning("GetIncidentEnergy","de = %f, deltaE[0] = %f; Problem!!!",de,deltaE[0]);
      eIncident = 1.e8; //give something rediculous; should not happen
      }

   return eIncident;
}

Double_t CycSrim::GetResidualEnergyFromDe(Int_t z, Double_t a, Double_t de) {
   //Return residual energy for a given energy loss 
   //if particle punched through absorber.

   const Int_t numE = fNumStopping[z-1];
   Double_t deltaE[numE];
   Double_t eres[numE];

   //Check to make sure particle punches through this detector
   Double_t deTest = GetEnergyLoss(z,a,de);
   if(deTest >= de) {
      //particle did not punch through
      printf("de = %f, deTest = %f\n",de,deTest);
      return 0.;  //e residual = 0
      }

   Int_t num = 0;
   for(Int_t ie=numE-1;ie>=0;ie--) {
      stoppingStruct *stop_p = (stoppingStruct*)fStopping[z-1][ie];
      Double_t e = stop_p->eOverA;

      Double_t ein = e*a;
      eres[num]    = GetResidualEnergy(z,a,e*a);
      deltaE[num]  = ein - eres[num];
      num++;

      printf("de = %f, ein = %f, deltaE[%d] = %f, eres[%d] = %f\n",de,ein,num-1,deltaE[num-1],num-1,eres[num-1]);
      if(deltaE[num-1] > de) break;
      }

   Double_t eResidual;
   if(num > 1) {
      eResidual = Interp(deltaE,eres,num,2,de);
      }
   else {
      Warning("GetResidualEnergyFromDe","de = %f, deltaE[0] = %f; Problem!!!",de,deltaE[0]);
      eResidual = 1.e8; //give something rediculous; should not happen
      }

   return eResidual;
}

Double_t CycSrim::GetRange(Int_t z, Double_t a, Double_t e, Bool_t lengthUnits) {
   //Return Range of an ion having energy e
   //Input is z, a, total energy of ion
   //if lengthUnits = kTRUE (default), returns in units of length 
   //specified in constructor; otherwise returns range in mg/cm^2

   Double_t e_a = e / a;

   const Int_t numE = fNumStopping[z-1];
   Double_t ranges[numE];
   Double_t evals[numE];

   for(Int_t ie=0;ie<numE;ie++) {
      stoppingStruct *stop_p = (stoppingStruct*)fStopping[z-1][ie];
      evals[ie] = stop_p->eOverA; //cout << "evals: " << evals[ie] << endl;
      ranges[ie] = stop_p->range; //cout << "ranges: " << ranges[ie] << endl;
      }
   
   Double_t range = Interp(evals,ranges,numE,2,e_a); //cout << "Range: " << range << endl;
   //Double_t range = Intero(evals,ranges,numE,2,e_a);
   if(lengthUnits) range /= fMaterialFactor;
   return range;
}

Double_t CycSrim::GetDeDx(Int_t z, Double_t a, Double_t etot) {
   Double_t dedxElec = GetDeDxElectronic(z,a,etot);
   Double_t dedxNucl = GetDeDxNuclear(z,a,etot);
   Double_t dedx = dedxElec + dedxNucl;
   return dedx;
}

Double_t CycSrim::GetDeDxElectronic(Int_t z, Double_t a, Double_t e) {
   //Get electronic dE/dx

   Double_t e_a = e / a;

   const Int_t numE = fNumStopping[z-1];
   Double_t dedxElec[numE];
   Double_t evals[numE];

   for(Int_t ie=0;ie<numE;ie++) {
      stoppingStruct *stop_p = (stoppingStruct*)fStopping[z-1][ie];
      evals[ie] = stop_p->eOverA;
      dedxElec[ie] = stop_p->dEdxElectronic;
      }

   //Get dedx at this energy
   Double_t dedx = Interp(evals,dedxElec,numE,2,e_a);
   return dedx;
}

Double_t CycSrim::GetDeDxNuclear(Int_t z, Double_t a, Double_t e) {
   //Get electronic dE/dx

   Double_t e_a = e / a;

   const Int_t numE = fNumStopping[z-1];
   Double_t dedxNucl[numE];
   Double_t evals[numE];

   for(Int_t ie=0;ie<numE;ie++) {
      stoppingStruct *stop_p = (stoppingStruct*)fStopping[z-1][ie];
      evals[ie] = stop_p->eOverA;
      dedxNucl[ie] = stop_p->dEdxNuclear;
      }

   //Get dedx at this energy
   Double_t dedx = Interp(evals,dedxNucl,numE,2,e_a);
   return dedx;
}

Double_t CycSrim::GetPunchthroughEnergy(Int_t z, Double_t a) {
   //Return energy necessary to just punch through this absorber.
   //Input is z, a

   const Int_t numE = fNumStopping[z-1];
   Double_t ranges[numE];
   Double_t evals[numE];

   for(Int_t ie=0;ie<numE;ie++) {
      stoppingStruct *stop_p = (stoppingStruct*)fStopping[z-1][ie];
      evals[ie] = stop_p->eOverA;
      ranges[ie] = stop_p->range;
      }
   Double_t eOverA = Interp(ranges,evals,numE,2,fThickness);
   return a*eOverA;
}

void CycSrim::TestInterp(Int_t z, TGraph *&grData, TGraph *&grInterp) {

   grData = new TGraph();
   grInterp = new TGraph();

   const Int_t numE = fNumStopping[z-1];
   Double_t ranges[numE];
   Double_t evals[numE];

   for(Int_t ie=0;ie<numE;ie++) {
      stoppingStruct *stop_p = (stoppingStruct*)fStopping[z-1][ie];
      Double_t eOverA = stop_p->eOverA;
      Double_t range = stop_p->range;
      grData->SetPoint(ie,eOverA,range);

      //keep for the interpolation test
      evals[ie] = eOverA;
      ranges[ie] = range;
      }

   for(Double_t eOverA=1.;eOverA<150.;eOverA+=0.5) {
      Double_t rInterp = Interp(evals,ranges,numE,2,eOverA);
      //printf("eOverA = %f, rInterp = %f\n",eOverA,rInterp);
      grInterp->SetPoint(grInterp->GetN(),eOverA,rInterp);
      }

}

//____________________________________________________________________
const Char_t*  CycSrim::GetSrimDataDir() const {
  // Return the data directory.  
  TString CYC_SRIM_DATA_DIR="/projects/mona-sim/jonesm/st_mona/bin";
  return CYC_SRIM_DATA_DIR;
}
  
Double_t CycSrim::Intero(const Double_t *x, const Double_t *y, Int_t npts, Int_t nterms, Double_t xin) {
   //Do a simple interpolation from 0 using the first elements.
   Double_t x2 = x[0];
   Double_t y2 = y[0];
   Double_t x1 = 0.0;
   Double_t y1 = 0.0;
   Double_t yout = y1 + (y2-y1)*(xin-x1)/(x2-x1);
   return yout;

   //Original FORTRAN from IONGIVSOM
// 	SUBROUTINE INTERO(X,Y,XIN,YOUT)
// 	DIMENSION X(1),Y(1)
// 	X2=X(1)
// 	Y2=Y(1)
// 	X1=0.0
// 	Y1=0.0
// 	YOUT=Y1+(Y2-Y1)*(XIN-X1)/(X2-X1)
// 	RETURN
// 	END
      }

Double_t CycSrim::Interp(const Double_t *x, const Double_t *y, Int_t npts, Int_t nterms, Double_t xin) {
//-------PURPOSE
//	 INTERPOLATE BETWEEN DATA POINTS TO EVALUATE A FUNCTION
//
//-------USAGE
//	 CALL INTERP (X, Y, NPTS, NTERMS, XIN, YOUT)
//
//-------DESCRIPTION OF PARAMETERS
//	 X	- ARRAY OF DATA POINTS FOR INDENPENDENT VARIABLE
//	 Y	- ARRAY OF DATA POINTS FOR DENPENDENT VARIABLE
//	 NPTS   - NUMBER OF PAIRS OF DATA POINTS
//	 NTERMS - NUMBER OF TERMS IN FITTING POLYNOMIAL
//	 XIN    - INPUT VALUE OF X
//	 YOUT   - INTERPOLATED VALUE OF Y
//
//-------SUBROUTINES AND FUNCTION SUBPROGRAMS REQUIRED
//	 NONE
//
//-------COMMENTS  DIMENSION STATEMENT VALID FOR NTERMS UP TO 10
//	 MODIFIED SO THAT NTERMS IN CALLING STATEMENT IS NOT CHANGED
//
//-------FROM  PHILIP R. BEVINGTON, DATA REDUCTION AND ERROR ANALYSIS,
//-------MC GRAW-HILL, NEW YORK, 1969,P.266-7
//-------MAKE AN INTERNAL VARIABLE NTERMS
//
//	  SEARCH FOR APPROPRIATE VALUE OF X(1)
//
//	DIMENSION X(1),Y(1)
//	DIMENSION DELTA(10),A(10)
   Int_t i;
   Double_t delta[10],a[10];

   Double_t yout = 0.0;
   Int_t i1 = npts - nterms;
   printf("Inside interp, ntps = %d, nterms = %d, i1 = %d, xin = %f\n",npts,nterms,i1,xin);
   for(i=0;i<npts;i++) {
      if(xin < x[i]) {
	 i1 = i - nterms/2;
	 if(i1 < 0) i1 = 0;
	 break;
         }
      else if(xin == x[i]) {
	 yout = y[i];
	 return yout;
         }
      }
   Int_t i2 = i1 + nterms - 1;

   if(i2 >= npts) {
      i2 = npts - 1;
      i1 = i2 - nterms + 1;
      if(i1 < 0) i1 = 0;
      nterms = i2 - i1 + 1;  //redefine just in case i1 set to 0
      }

   //Evaluate deviations data

   Float_t denom = x[i1+1] - x[i1]; cout << "Denom: " << denom << endl;
   //denom += 0.0001; // Why is this zero? 
   Float_t deltax = (xin - x[i1]) / denom;
  
//printf("i1 = %d, i2 = %d, xin = %f, x[%d] = %f, x[%d] = %f, x[%d] = %f\n",i1,i2,xin,i1,x[i1],i1+1,x[i1+1],i2,x[i2]);

   for(i=0;i<nterms;i++) {
      Int_t ix = i1 + i;
      delta[i] = (x[ix] - x[i1]) / denom;
      printf("delta[%d] = %f, x[%d] = %f, x[%d] = %f\n",i,delta[i],ix,x[ix],i1,x[i1]);
      }

   //Accumulate coefficients a

   a[0] = y[i1];
   for(Int_t k=1;k<nterms;k++) {
      Double_t prod = 1.;
      Double_t sum = 0.;
      Int_t imax = k;
      Int_t ixmax = i1 + imax;
      printf("k = %d, imax = %d, ixmax = %d\n",k,imax,ixmax);
      for(i=0; i<imax;i++) {
	 Int_t j = k - i - 1;
	 printf("delta[%d] = %f, delta[%d] = %f\n",k,delta[k],j,delta[j]);
	 prod *= (delta[k] - delta[j]);
	 sum -= a[j] / prod;
         }
      a[k] = sum + y[ixmax] / prod; //cout << "a[k]: " << a[k] << endl;
      //printf("a[%d] = %f,sum = %f, prod = %f\n",k,a[k],sum,prod);
      }

   //Accumulate sum of expansion

   Double_t sum = a[0];
   for(Int_t j=1;j<nterms;j++) {
      Double_t prod = 1.;
      Int_t imax = j;
      for(i=0;i<imax;i++) prod *= (deltax - delta[i]);
      sum += a[j] * prod; cout << "a[j]: " << a[j] << endl;
      }
   yout = sum;
   cout << "Returning Interpoloated yout: " << yout << endl;
   return yout;

//Original FORTRAN from IONGIVSOM
//    11 DO 19 I=1,NPTS
// 	IF (XIN-X(I)) 13,17,19
//    13 I1 = I-NTERMS/2
// 	IF (I1) 15,15,21
//    15 I1 = 1
// 	GO TO 21
//    17 YOUT = Y(I)
//    18 GO TO 61
//    19 CONTINUE
// 	I1 = NPTS - NTERMS + 1
//    21 I2 = I1 + NTERMS - 1
// 	IF (NPTS - I2) 23,31,31
//    23 I2 = NPTS
// 	I1 = I2 - NTERMS + 1
//    25 IF (I1) 26,26,31
//    26 I1 = 1
//    27 NTERMS = I2 - I1 + 1
// C
// C	  EVALUATE DEVIATIONS DELTA
// C
//    31 DENOM = X(I1+1) - X(I1)
// 	DELTAX = (XIN - X(I1))/DENOM
// 	DO 35 I=1,NTERMS
// 	IX = I1 + I - 1
//    35 DELTA(I) = (X(IX)-X(I1))/DENOM
// C
// C	  ACCUMULATE COEFFICIENTS A
// C
//    40 A(1)=Y(I1)
//    41 DO 50 K=2,NTERMS
// 	PROD = 1.
// 	SUM = 0.
// 	IMAX = K-1
// 	IXMAX = I1 + IMAX
// 	DO 49 I=1,IMAX
// 	J=K-I
// 	PROD = PROD*(DELTA(K)-DELTA(J))
//    49 SUM = SUM-A(J)/PROD
// C
// C	  ACCUMULATE SUM OF EXPANSION
// C
//    50 A(K) = SUM + Y(IXMAX)/PROD
//    51 SUM = A(1)
// 	DO 57 J=2,NTERMS
// 	PROD = 1.
// 	IMAX = J-1
// 	DO 56 I=1,IMAX
//    56 PROD = PROD*(DELTAX-DELTA(I))
//    57 SUM = SUM +A(J)*PROD
//    60 YOUT = SUM
//    61 RETURN
// 	END
}
