// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME CycSrimDict

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
#include "../include/CycSrim.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void *new_CycSrim(void *p = 0);
   static void *newArray_CycSrim(Long_t size, void *p);
   static void delete_CycSrim(void *p);
   static void deleteArray_CycSrim(void *p);
   static void destruct_CycSrim(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CycSrim*)
   {
      ::CycSrim *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CycSrim >(0);
      static ::ROOT::TGenericClassInfo 
         instance("CycSrim", ::CycSrim::Class_Version(), "../include/CycSrim.h", 87,
                  typeid(::CycSrim), DefineBehavior(ptr, ptr),
                  &::CycSrim::Dictionary, isa_proxy, 4,
                  sizeof(::CycSrim) );
      instance.SetNew(&new_CycSrim);
      instance.SetNewArray(&newArray_CycSrim);
      instance.SetDelete(&delete_CycSrim);
      instance.SetDeleteArray(&deleteArray_CycSrim);
      instance.SetDestructor(&destruct_CycSrim);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CycSrim*)
   {
      return GenerateInitInstanceLocal((::CycSrim*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::CycSrim*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr CycSrim::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *CycSrim::Class_Name()
{
   return "CycSrim";
}

//______________________________________________________________________________
const char *CycSrim::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CycSrim*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int CycSrim::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CycSrim*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CycSrim::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CycSrim*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CycSrim::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CycSrim*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void CycSrim::Streamer(TBuffer &R__b)
{
   // Stream an object of class CycSrim.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CycSrim::Class(),this);
   } else {
      R__b.WriteClassBuffer(CycSrim::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CycSrim(void *p) {
      return  p ? new(p) ::CycSrim : new ::CycSrim;
   }
   static void *newArray_CycSrim(Long_t nElements, void *p) {
      return p ? new(p) ::CycSrim[nElements] : new ::CycSrim[nElements];
   }
   // Wrapper around operator delete
   static void delete_CycSrim(void *p) {
      delete ((::CycSrim*)p);
   }
   static void deleteArray_CycSrim(void *p) {
      delete [] ((::CycSrim*)p);
   }
   static void destruct_CycSrim(void *p) {
      typedef ::CycSrim current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CycSrim

namespace {
  void TriggerDictionaryInitialization_CycSrimDict_Impl() {
    static const char* headers[] = {
"../include/CycSrim.h",
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
class __attribute__((annotate(R"ATTRDUMP(SRIM)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$../include/CycSrim.h")))  CycSrim;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "../include/CycSrim.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"CycSrim", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("CycSrimDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_CycSrimDict_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_CycSrimDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_CycSrimDict() {
  TriggerDictionaryInitialization_CycSrimDict_Impl();
}
