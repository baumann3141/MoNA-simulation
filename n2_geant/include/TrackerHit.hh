#ifndef TrackerHit_h
#define TrackerHit_h 1
// C++ library includes
// Geant4 library includes
#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
// My file includes

// This is my hit class. It stores all the information I want for each hit
class TrackerHit : public G4VHit
{
    public:
        TrackerHit();
        ~TrackerHit();
        TrackerHit(const TrackerHit&);
        const TrackerHit& operator=(const TrackerHit&);
        G4int operator==(const TrackerHit&) const;

        inline void* operator new(size_t);
        inline void  operator delete(void*);

        // Tells the computer how to draw the step
        void Draw();
        // Puts the information about the step on screen
        void Print();

    public:
        // Set functions
        void SetTrackID(G4int track) {trackID = track;}
        void SetChamberNb(G4int chamb) {chamberNb = chamb;} 
        void SetEdep(G4double de) {edep = de;}
        void SetLightL(G4double de) {lightL = de;}
        void SetLightR(G4double de) {lightR = de;}
        void SetLight(G4double de) {light = de;}
        void SetPos(G4ThreeVector xyz);//{pos = xyz; SetX(xyz.X); SetY(xyz.Y); SetZ(xyz.Z);}
        void SetName(G4String inName){name = inName;}
        void SetTime(G4double inTime){time = inTime;}
        void SetX(G4double value){x = value;}
        void SetY(G4double value){y = value;}
        void SetZ(G4double value){z = value;}
        
        // Get functions
        G4int GetTrackID() {return trackID;}
        G4int GetChamberNb() {return chamberNb;}
        G4double GetEdep() {return edep;}   
        G4double* GetEdepA() {return &edep;}   
        G4double GetLightL() {return lightL;}   
        G4double GetLightR() {return lightR;}   
        G4double GetLight() {return light;}   
        G4double* GetLightA() {return &light;}   
        G4ThreeVector GetPos() {return pos;}
        G4ThreeVector* GetPosA() {return &pos;}
        G4double GetX() {return x;}
        G4double* GetXA() {return &x;}
        G4double GetY() {return y;}
        G4double* GetYA() {return &y;}
        G4double GetZ() {return z;}
        G4double* GetZA() {return &z;}
        G4String GetName() {return name;}
        G4String* GetNameA() {return &name;}
        G4double GetTime() {return time;}
        G4double* GetTimeA() {return &time;}
        
    private:
        // The information I want to keep track of
        G4int         trackID;      // Which particle track
        G4int         chamberNb;    // Detector ID number
        G4double      edep;         // Energy deposited in this step
        G4double      lightL;         // attenuated light going to left bar
        G4double      lightR;         // attenuated light going to right bar
        G4double      light;         // attenuated light
        G4ThreeVector pos;          // Position of the step
        G4double      x;            // x because i don't know how to write vector to root file yet
        G4double      y;
        G4double      z;
        G4String      name;         // The name of the struck detector
        G4double      time;         // The time of the strike
};


typedef G4THitsCollection<TrackerHit> TrackerHitsCollection;

// TODO: Does this really have to be done this way?
extern G4Allocator<TrackerHit> TrackerHitAllocator;

inline void* TrackerHit::operator new(size_t)
{
    void *aHit;
    aHit = (void *) TrackerHitAllocator.MallocSingle();
    return aHit;
}


inline void TrackerHit::operator delete(void *aHit)
{
    TrackerHitAllocator.FreeSingle((TrackerHit*) aHit);
}
#endif
