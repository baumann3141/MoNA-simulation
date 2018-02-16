// -*- mode: c++ -*-
//
// $Id: CycSrim.h,v 1.14 2011/11/03 16:15:23 hagel Exp $
// $Author: hagel $
// $Date: 2011/11/03 16:15:23 $
//
#ifndef CYC_CycSrim
#define CYC_CycSrim

#ifndef ROOT_TString
#include "TString.h"
#endif

#ifndef CYC_SRIM_paths
#include "paths.h"
#endif

#ifndef ROOT_TError
#include "TError.h"
#endif

class TGraph;

//densities
#define SRIM_MYLAR_DENSITY			1.397
#define SRIM_TH_DENSITY		        	11.658
#define SRIM_U_DENSITY				19.043
#define SRIM_AU_DENSITY				19.311 
#define SRIM_C_DENSITY				2.267
#define SRIM_BUTANE_DENSITY		(0.002594*(273.15/298.15))	//g/cm^3 at 25C and 1 atm
#define SRIM_PENTANE_DENSITY		(0.003220*(273.15/298.15))	//g/cm^3 at 25C and 1 atm
#define SRIM_AL_DENSITY				2.702//p=n rho T
#define SRIM_SI_DENSITY				2.3212
#define SRIM_CSI_DENSITY                        4.51  //g/cm^3
#define SRIM_C2F6_DENSITY		(0.005654*(273.15/298.15))	//g/cm^3 at 25C and 1 atm
#define SRIM_C4F10_DENSITY		(0.009751*(273.15/298.15))	//g/cm^3 at 25C and 1 atm
#define SRIM_YAP_DENSITY		
#define SRIM_TI_DENSITY				4.519
#define SRIM_ZR_DENSITY				40
#define SRIM_ZN_DENSITY				30
#define SRIM_FE_DENSITY				7.866
#define SRIM_NI_DENSITY				8.896
#define SRIM_GE_DENSITY				5.35
#define SRIM_MO_DENSITY				10.206
#define SRIM_SN_DENSITY				7.2816 
#define SRIM_PT_DENSITY				7.2816
#define SRIM_PB_DENSITY				11.344
#define SRIM_BI_DENSITY				9.800
#define SRIM_YB_DENSITY				6.960
#define SRIM_POLYPROPYLENE_DENSITY              0.855
#define SRIM_LOWDENSITYPOLYPROPYLENE_DENSITY    0.910

//Zmedium
#define SRIM_ZMEDIUM_MYLAR			5
#define SRIM_ZMEDIUM_TH				90
#define SRIM_ZMEDIUM_U				92
#define SRIM_ZMEDIUM_AU				79
#define SRIM_ZMEDIUM_C			        6
#define SRIM_ZMEDIUM_BUTANE			2.42
#define SRIM_ZMEDIUM_PENTANE			2.47
#define SRIM_ZMEDIUM_AL				13
#define SRIM_ZMEDIUM_SI				14
#define SRIM_ZMEDIUM_C2F6			8.25
#define SRIM_ZMEDIUM_C4F10			8.14
#define SRIM_ZMEDIUM_YAP			14
#define SRIM_ZMEDIUM_TI				22
#define SRIM_ZMEDIUM_ZR				40
#define SRIM_ZMEDIUM_ZN				30
#define SRIM_ZMEDIUM_FE				26
#define SRIM_ZMEDIUM_NI				28
#define SRIM_ZMEDIUM_GE				32
#define SRIM_ZMEDIUM_MO				42
#define SRIM_ZMEDIUM_SN				50
#define SRIM_ZMEDIUM_PT				78
#define SRIM_ZMEDIUM_PB				82
#define SRIM_ZMEDIUM_BI				83
#define SRIM_ZMEDIUM_YB				70

class CycSrim {
public:
   enum srimDensityUnits {
      kUnitsMgCm2  = 1,
      kUnitsCm     = 2,
      kUnitsMicron = 3,
      kUnitsTorr   = 4,
      kUnitsMbar   = 5
      };

   CycSrim() {}
   CycSrim(Int_t imat, Double_t thickness, srimDensityUnits units=kUnitsMgCm2);
   CycSrim(Int_t imat, Double_t thickness, Double_t pressure, srimDensityUnits units=kUnitsTorr);
   Double_t GetEnergyLoss(Int_t z, Double_t m, Double_t etot);
   Double_t GetEnergyLossFromResidual(Int_t z, Double_t a, Double_t eres);
   Double_t GetResidualEnergy(Int_t z, Double_t m, Double_t etot);
   Double_t GetResidualEnergyFromDe(Int_t z, Double_t a, Double_t de);
   Double_t GetIncidentEnergy(Int_t z, Double_t m, Double_t de);
   Double_t GetRange(Int_t z, Double_t m, Double_t etot, Bool_t lengthUnits=kTRUE);
   Double_t GetDeDx(Int_t z, Double_t m, Double_t etot);
   Double_t GetDeDxElectronic(Int_t z, Double_t m, Double_t etot);
   Double_t GetDeDxNuclear(Int_t z, Double_t m, Double_t etot);
   void     TestInterp(Int_t z, TGraph *&grData, TGraph *&grInterp);
   Double_t GetThickness() const {return fThickness;} //return in mg/cm^2
   Double_t SetThickness(Double_t thick) {fThickness = thick * fMaterialFactor;}
   Double_t GetPunchthroughEnergy(Int_t z, Double_t m);

  enum EMaterials {
     //materials
     SrimMaterialSi                     =   0,
     SrimMaterialMylar	                =   1,
     SrimMaterialIsobutane              =   2,
     SrimMaterialPentane                =   3,
     SrimMaterialAluminum               =   4,
     SrimMaterialNickel                 =   5,
     SrimMaterialBC420                  =   6,
     SrimMaterialCsI                    =   7,
     SrimMaterialCarbon                 =   8,
     SrimMaterialAu                     =   9,
     SrimMaterialPolypropylene          =  10,
     SrimMaterialLowDensityPolyethylene =  11,
     SrimMaterialTh                     =  12,
     SrimNumberOfMaterials              =  13, //numb material (largest indx+1)
     SrimMaxNumberOfEnergies            = 200	//upper_limit
     };

private:

   Double_t fStopping[92][SrimMaxNumberOfEnergies][6];
   Int_t    fNumStopping[92];
   TString  fMaterialName;
   Float_t  fMaterialDensity;
   Float_t  fMaterialFactor; //factor to go from thickness in units specified to mg/cm^2

   Int_t fNumMaterials;
   //[0] = Z
   //[1] = A
   //[2] = percent
   Float_t fMaterialComposition[10][3];

   //private methods
   int Init();
   const Char_t* GetSrimDataDir() const;
   Double_t Interp(const Double_t *x, const Double_t *y, Int_t ntps, Int_t nterms, Double_t xin);
   Double_t Intero(const Double_t *x, const Double_t *y, Int_t ntps, Int_t nterms, Double_t xin);

   Int_t    fMaterial;    //Material number
   Double_t fThickness;   //Thickness in mg/cm2

public:
  ClassDef(CycSrim,0) //SRIM
};

#endif

//  $Log: CycSrim.h,v $
//  Revision 1.14  2011/11/03 16:15:23  hagel
//  Added Carbon, Au, Polyproplyene, low density polyethylene
//
//  Revision 1.13  2011/06/28 22:17:43  hagel
//  Remove diagnostic print statements
//
//  Revision 1.12  2011/05/11 22:48:37  hagel
//  Add CsI absorber
//
//  Revision 1.11  2011/05/02 14:41:29  hagel
//  Implement SetThickness method
//
//  Revision 1.10  2010/10/19 17:29:57  hagel
//  Implement BC420 absorber
//
//  Revision 1.9  2010/10/07 19:19:01  hagel
//  Implement Nickel as a stopper
//
//  Revision 1.8  2009/12/15 22:24:49  hagel
//  Latest updates
//
//  Revision 1.7  2009/09/25 20:35:31  hagel
//  Implement GetPunchthroughEnergy
//
//  Revision 1.6  2009/04/10 17:30:11  hagel
//  Latest updates
//
//  Revision 1.5  2009/03/09 19:01:05  hagel
//  Minor improvements
//
//  Revision 1.4  2009/03/04 16:41:37  hagel
//  First revision after extensively modifiying to use ranges (ie get away from T. Materna method
//
//  Revision 1.3  2008/10/08 18:26:47  hagel
//  Changes to make work with rootcint AND on RH7.3
//
//  Revision 1.2  2008/09/01 14:47:09  hagel
//  Add extra methods for ease of use
//
//  Revision 1.1  2008/01/16 20:48:59  hagel
//  Initial compilable revision after copying from Thomas and changing to class
//
