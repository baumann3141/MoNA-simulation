// C++ library includes
// Geant4 library includes
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
// My file includes
#include "TrackerHit.hh"

G4Allocator<TrackerHit> TrackerHitAllocator;

// Default constructor ... set to invalid hit
TrackerHit::TrackerHit()
{
    trackID   = 0;
    chamberNb = 0;
    edep      = 0.;
    pos       = G4ThreeVector(0.,0.,0.);
    time      = 0.;
    x         = 0.;
    y         = 0.;
    z         = 0.;
    lightL    = 0.;
    lightR    = 0.;
    light    = 0.;
}


TrackerHit::~TrackerHit() {}

TrackerHit::TrackerHit(const TrackerHit& right) : G4VHit()
{
    trackID   = right.trackID;
    chamberNb = right.chamberNb;
    edep      = right.edep;
    lightL    = right.lightL;
    lightR    = right.lightR;
    time      = right.time;
    pos       = right.pos;
    x         = pos.getX();
    y         = pos.getY();
    z         = pos.getZ();
}

const TrackerHit& TrackerHit::operator=(const TrackerHit& right)
{
    trackID   = right.trackID;
    chamberNb = right.chamberNb;
    edep      = right.edep;
    lightL    = right.lightL;
    lightR    = right.lightR;
    time      = right.time;
    pos       = right.pos;
    x         = pos.getX();
    y         = pos.getY();
    z         = pos.getZ();
    return *this;
}

G4int TrackerHit::operator==(const TrackerHit& right) const
{
    return (this==&right) ? 1 : 0;
}

// Draws the step on screen
void TrackerHit::Draw()
{
    G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
    if(pVVisManager)
    {
        G4Circle circle(pos);
        circle.SetScreenSize(2.);
        circle.SetFillStyle(G4Circle::filled);
        G4Colour colour(1.,0.,0.);
        G4VisAttributes attribs(colour);
        circle.SetVisAttributes(attribs);
        pVVisManager->Draw(circle);
    }
}

void TrackerHit::SetPos(G4ThreeVector xyz)
{
    pos = xyz;
    x = pos.getX()/m;
    y = pos.getY()/m;
    z = pos.getZ()/m;
}

// Prints the information about the step to screen
void TrackerHit::Print()
{
	/*
    //G4cout << "  trackID: " << trackID << "  chamberNb: " << chamberNb
            << "  energy deposit: " << G4BestUnit(edep,"Energy")
        << "  position: " << G4BestUnit(pos,"Length") << "name: " << name << "Time: " << time <<  G4endl;
	*/
}
