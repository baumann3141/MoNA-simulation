// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME monaAnaDict

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "RConfig.h"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Since CINT ignores the std namespace, we need to do so in this file.
namespace std {} using namespace std;

// Header files passed as explicit arguments
#include "../include/mona_analysis.hh"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void *new_MonaAnalyzedEvent(void *p = 0);
   static void *newArray_MonaAnalyzedEvent(Long_t size, void *p);
   static void delete_MonaAnalyzedEvent(void *p);
   static void deleteArray_MonaAnalyzedEvent(void *p);
   static void destruct_MonaAnalyzedEvent(void *p);
   static void streamer_MonaAnalyzedEvent(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::MonaAnalyzedEvent*)
   {
      ::MonaAnalyzedEvent *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::MonaAnalyzedEvent >(0);
      static ::ROOT::TGenericClassInfo 
         instance("MonaAnalyzedEvent", ::MonaAnalyzedEvent::Class_Version(), "../include/mona_analysis.hh", 22,
                  typeid(::MonaAnalyzedEvent), DefineBehavior(ptr, ptr),
                  &::MonaAnalyzedEvent::Dictionary, isa_proxy, 16,
                  sizeof(::MonaAnalyzedEvent) );
      instance.SetNew(&new_MonaAnalyzedEvent);
      instance.SetNewArray(&newArray_MonaAnalyzedEvent);
      instance.SetDelete(&delete_MonaAnalyzedEvent);
      instance.SetDeleteArray(&deleteArray_MonaAnalyzedEvent);
      instance.SetDestructor(&destruct_MonaAnalyzedEvent);
      instance.SetStreamerFunc(&streamer_MonaAnalyzedEvent);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::MonaAnalyzedEvent*)
   {
      return GenerateInitInstanceLocal((::MonaAnalyzedEvent*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::MonaAnalyzedEvent*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr MonaAnalyzedEvent::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *MonaAnalyzedEvent::Class_Name()
{
   return "MonaAnalyzedEvent";
}

//______________________________________________________________________________
const char *MonaAnalyzedEvent::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::MonaAnalyzedEvent*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int MonaAnalyzedEvent::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::MonaAnalyzedEvent*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *MonaAnalyzedEvent::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::MonaAnalyzedEvent*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *MonaAnalyzedEvent::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::MonaAnalyzedEvent*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void MonaAnalyzedEvent::Streamer(TBuffer &R__b)
{
   // Stream an object of class MonaAnalyzedEvent.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      R__b >> manDE;
      R__b >> manDESwitch;
      vTPos.Streamer(R__b);
      vNPos.Streamer(R__b);
      vN2Pos.Streamer(R__b);
      vN3Pos.Streamer(R__b);
      vN4Pos.Streamer(R__b);
      vG1NPos.Streamer(R__b);
      vG2NPos.Streamer(R__b);
      vG3NPos.Streamer(R__b);
      vG4NPos.Streamer(R__b);
      vG5NPos.Streamer(R__b);
      vG6NPos.Streamer(R__b);
      lvN.Streamer(R__b);
      lvN2.Streamer(R__b);
      lvN3.Streamer(R__b);
      lvN4.Streamer(R__b);
      lvG1N.Streamer(R__b);
      lvG2N.Streamer(R__b);
      lvG3N.Streamer(R__b);
      lvG4N.Streamer(R__b);
      lvG5N.Streamer(R__b);
      lvG6N.Streamer(R__b);
      lvFCosy.Streamer(R__b);
      lvFSchill.Streamer(R__b);
      lvFFit.Streamer(R__b);
      R__b.ReadStaticArray((double*)deltaE);
      R__b >> EDecay1;
      R__b >> EDecay2;
      R__b >> EDecay3;
      R__b >> EDecayTotal;
      R__b >> EDecayTotalG1and2;
      R__b >> EDecayTotalG3and4;
      R__b >> EDecayG1;
      R__b >> EDecayG2;
      R__b >> EDecayG3;
      R__b >> EDecayG4;
      R__b >> EDecayG5;
      R__b >> EDecayG6;
      R__b >> EDecayGHigh;
      R__b >> EDecayGLow;
      R__b >> EDecayG_2neutron;
      R__b >> EDecayG_3neutron;
      R__b >> EDecayTotalG12and3;
      R__b >> EDecay4body;
      R__b >> EDecay4body_True;
      R__b >> EDecay5body;
      R__b >> EDecay5body_True;
      R__b.ReadStaticArray((double*)altExenCosy);
      R__b.ReadStaticArray((double*)altExenSchill);
      R__b.ReadStaticArray((double*)altExenFit);
      R__b >> tLab;
      R__b >> tLab2;
      R__b >> vRel;
      R__b >> vRelG1;
      R__b >> vRelG2;
      R__b >> vRel2;
      R__b >> nVel;
      R__b >> nVelG1;
      R__b >> nVelG2;
      R__b >> nVel2;
      R__b >> fVel;
      R__b >> fragKinE;
      R__b >> neutronKinE;
      R__b >> nTheta;
      R__b >> nPhi;
      R__b >> neutronKinE2;
      R__b >> nTheta2;
      R__b >> nPhi2;
      R__b >> fTheta;
      R__b >> fPhi;
      R__b >> fAngleX;
      R__b >> fAngleY;
      R__b >> nAngleX;
      R__b >> nAngleY;
      R__b >> nAngleX2;
      R__b >> nAngleY2;
      R__b >> fpATA;
      R__b >> fpBTA;
      R__b >> fpYTA;
      R__b >> delta;
      R__b >> fYfp;
      R__b >> fBfp;
      R__b >> fXfp;
      R__b >> fAfp;
      R__b >> Tsys_CosTheta1;
      R__b >> Tsys_CosTheta2;
      R__b >> Ysys_CosTheta1;
      R__b >> Ysys_CosTheta2;
      R__b >> Tsys_Ex_Et1;
      R__b >> Tsys_Ex_Et2;
      R__b >> Ysys_Ex_Et1;
      R__b >> Ysys_Ex_Et2;
      R__b >> Pz_FragN;
      R__b >> Pz_Frag;
      R__b >> Ppar_Frag;
      R__b >> Pperp_Frag;
      R__b >> Ptot_Frag;
      R__b >> nnCM_ppar1;
      R__b >> nnCM_ppar2;
      R__b >> nnCM_pperp1;
      R__b >> nnCM_pperp2;
      R__b >> Theta_3body;
      R__b >> Phi_3body;
      R__b >> n1n2costhetaCM;
      R__b >> tflatfoil;
      R__b >> bulge;
      R__b >> test;
      R__b >> srimTarget;
      R__b >> srimFoil;
      R__b.CheckByteCount(R__s, R__c, MonaAnalyzedEvent::IsA());
   } else {
      R__c = R__b.WriteVersion(MonaAnalyzedEvent::IsA(), kTRUE);
      TObject::Streamer(R__b);
      R__b << manDE;
      R__b << manDESwitch;
      vTPos.Streamer(R__b);
      vNPos.Streamer(R__b);
      vN2Pos.Streamer(R__b);
      vN3Pos.Streamer(R__b);
      vN4Pos.Streamer(R__b);
      vG1NPos.Streamer(R__b);
      vG2NPos.Streamer(R__b);
      vG3NPos.Streamer(R__b);
      vG4NPos.Streamer(R__b);
      vG5NPos.Streamer(R__b);
      vG6NPos.Streamer(R__b);
      lvN.Streamer(R__b);
      lvN2.Streamer(R__b);
      lvN3.Streamer(R__b);
      lvN4.Streamer(R__b);
      lvG1N.Streamer(R__b);
      lvG2N.Streamer(R__b);
      lvG3N.Streamer(R__b);
      lvG4N.Streamer(R__b);
      lvG5N.Streamer(R__b);
      lvG6N.Streamer(R__b);
      lvFCosy.Streamer(R__b);
      lvFSchill.Streamer(R__b);
      lvFFit.Streamer(R__b);
      R__b.WriteArray(deltaE, 3);
      R__b << EDecay1;
      R__b << EDecay2;
      R__b << EDecay3;
      R__b << EDecayTotal;
      R__b << EDecayTotalG1and2;
      R__b << EDecayTotalG3and4;
      R__b << EDecayG1;
      R__b << EDecayG2;
      R__b << EDecayG3;
      R__b << EDecayG4;
      R__b << EDecayG5;
      R__b << EDecayG6;
      R__b << EDecayGHigh;
      R__b << EDecayGLow;
      R__b << EDecayG_2neutron;
      R__b << EDecayG_3neutron;
      R__b << EDecayTotalG12and3;
      R__b << EDecay4body;
      R__b << EDecay4body_True;
      R__b << EDecay5body;
      R__b << EDecay5body_True;
      R__b.WriteArray(altExenCosy, 3);
      R__b.WriteArray(altExenSchill, 3);
      R__b.WriteArray(altExenFit, 3);
      R__b << tLab;
      R__b << tLab2;
      R__b << vRel;
      R__b << vRelG1;
      R__b << vRelG2;
      R__b << vRel2;
      R__b << nVel;
      R__b << nVelG1;
      R__b << nVelG2;
      R__b << nVel2;
      R__b << fVel;
      R__b << fragKinE;
      R__b << neutronKinE;
      R__b << nTheta;
      R__b << nPhi;
      R__b << neutronKinE2;
      R__b << nTheta2;
      R__b << nPhi2;
      R__b << fTheta;
      R__b << fPhi;
      R__b << fAngleX;
      R__b << fAngleY;
      R__b << nAngleX;
      R__b << nAngleY;
      R__b << nAngleX2;
      R__b << nAngleY2;
      R__b << fpATA;
      R__b << fpBTA;
      R__b << fpYTA;
      R__b << delta;
      R__b << fYfp;
      R__b << fBfp;
      R__b << fXfp;
      R__b << fAfp;
      R__b << Tsys_CosTheta1;
      R__b << Tsys_CosTheta2;
      R__b << Ysys_CosTheta1;
      R__b << Ysys_CosTheta2;
      R__b << Tsys_Ex_Et1;
      R__b << Tsys_Ex_Et2;
      R__b << Ysys_Ex_Et1;
      R__b << Ysys_Ex_Et2;
      R__b << Pz_FragN;
      R__b << Pz_Frag;
      R__b << Ppar_Frag;
      R__b << Pperp_Frag;
      R__b << Ptot_Frag;
      R__b << nnCM_ppar1;
      R__b << nnCM_ppar2;
      R__b << nnCM_pperp1;
      R__b << nnCM_pperp2;
      R__b << Theta_3body;
      R__b << Phi_3body;
      R__b << n1n2costhetaCM;
      R__b << tflatfoil;
      R__b << bulge;
      R__b << test;
      R__b << srimTarget;
      R__b << srimFoil;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_MonaAnalyzedEvent(void *p) {
      return  p ? new(p) ::MonaAnalyzedEvent : new ::MonaAnalyzedEvent;
   }
   static void *newArray_MonaAnalyzedEvent(Long_t nElements, void *p) {
      return p ? new(p) ::MonaAnalyzedEvent[nElements] : new ::MonaAnalyzedEvent[nElements];
   }
   // Wrapper around operator delete
   static void delete_MonaAnalyzedEvent(void *p) {
      delete ((::MonaAnalyzedEvent*)p);
   }
   static void deleteArray_MonaAnalyzedEvent(void *p) {
      delete [] ((::MonaAnalyzedEvent*)p);
   }
   static void destruct_MonaAnalyzedEvent(void *p) {
      typedef ::MonaAnalyzedEvent current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_MonaAnalyzedEvent(TBuffer &buf, void *obj) {
      ((::MonaAnalyzedEvent*)obj)->::MonaAnalyzedEvent::Streamer(buf);
   }
} // end of namespace ROOT for class ::MonaAnalyzedEvent

namespace {
  void TriggerDictionaryInitialization_monaAnaDict_Impl() {
    static const char* headers[] = {
"../include/mona_analysis.hh",
0
    };
    static const char* includePaths[] = {
"/mnt/misc/sw/x86_64/Debian/8/root/gnu/6.04.02/include/root",
"/projects/mona-sim/baumann/single_target/st_mona/src/",
0
    };
    static const char* fwdDeclCode = 
R"DICTFWDDCLS(
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate("$clingAutoload$../include/mona_analysis.hh")))  MonaAnalyzedEvent;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "../include/mona_analysis.hh"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"MonaAnalyzedEvent", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("monaAnaDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_monaAnaDict_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_monaAnaDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_monaAnaDict() {
  TriggerDictionaryInitialization_monaAnaDict_Impl();
}
