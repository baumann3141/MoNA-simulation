// C++ library includes
#include <sstream>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
//#include <vector>
// Geant4 library includes
#include "Randomize.hh"
// My file includes
#include "Detector.hh"

////////////////////////////////////
// Begin General Function Section //
////////////////////////////////////
//TODO: define constructor / destructor behavior
Detector::Detector()
{
    ;
}

Detector::~Detector()
{
    ;
}

// Just a simple function set... the intention is to overload
std::vector<G4Element*> Detector::makeElements(std::vector<G4Element*> madeElements)
{
    return madeElements;
}
std::vector<G4Material*> Detector::makeMaterials(std::vector<G4Material*> madeMaterials,std::vector<G4Element*> madeElements)
{
    return madeMaterials;
}
std::vector<G4LogicalVolume*> Detector::makeLogVols(std::vector<G4LogicalVolume*> madeLogVols,std::vector<G4Material*> madeMaterials)
{
    return madeLogVols;
}
std::vector<NamedG4AssemblyVolume*> Detector::makeAssemblies(std::vector<NamedG4AssemblyVolume*> madeAssemblies)
{
    return madeAssemblies;
}
// Another overloaded function
std::vector<G4double> Detector::addResolutions(G4Step* aStep)
{
    std::vector<G4double> temp;
    temp.push_back(0.);
    return temp;
}

// Another overloaded function
std::vector<G4double> Detector::processLight(G4Step* aStep)
{
    std::vector<G4double> temp;
    temp.push_back(0.);
    return temp;
}
// this is supposed to be overloaded... creates the physical volumes needed
void Detector::addPhysVol(G4LogicalVolume* mother)
{
    ;
}
//TODO: make the functions which depend on this know that a null vector is bad... more error checking
// This searches for and returns a logical volume pointer to the made volume database
G4LogicalVolume* Detector::getLogVol()
{
    G4LogicalVolume* neededLogVol = NULL;
    std::vector<G4LogicalVolume*> logVolDatabase = interface->getMadeLogVols();
    for (G4int i = 0; i < logVolDatabase.size(); i++)
    {
        if (logVolDatabase[i]->GetName() == getType().append(".log"))
        {
            neededLogVol = logVolDatabase[i];
        }
    }
    return neededLogVol;
}

// This searches for and returns a logical volume pointer to the made volume database
NamedG4AssemblyVolume* Detector::getAssembly()
{
    NamedG4AssemblyVolume* neededAssembly = NULL;
    std::vector<NamedG4AssemblyVolume*> assemblyDatabase = interface->getMadeAssemblies();
    for (G4int i = 0; i < assemblyDatabase.size(); i++)
    {
        if (assemblyDatabase[i]->GetName() == getType().append(".assembly"))
        {
            //G4cout << "I REALLY FOUND MY ASSEMBLY\n";
            neededAssembly = assemblyDatabase[i];
            return neededAssembly;
        }
    }
    G4cout << "I failed to find the assembly I needed\n";
    return neededAssembly;
}

// This checks a string against the name of a vector of elements, and tells you whether or not the vector has it
G4bool Detector::hasElement(G4String name,std::vector<G4Element*> existing)
{
    // Now look through the existing element vector
    for (G4int j = 0;j < existing.size(); j++)
    {
        // Look for duplicate named elements
        if (name == existing[j]->GetName())
        {
            return true;
        }
    }
    return false;
}
// This checks a string against the name of a vector of Materials, and tells you whether or not the vector has it
G4bool Detector::hasMaterial(G4String name,std::vector<G4Material*> existing)
{
    // Now look through the existing element vector
    for (G4int j = 0;j < existing.size(); j++)
    {
        // Look for duplicate named elements
        if (name == existing[j]->GetName())
        {
            return true;
        }
    }
    return false;
}
// This checks a string against the name of a vector of Log Vols, and tells you whether or not the vector has it
G4bool Detector::hasLogVol(G4String name,std::vector<G4LogicalVolume*> existing)
{
    // Now look through the existing LogVol vector
    for (G4int j = 0;j < existing.size(); j++)
    {
        // Look for duplicate named LogVols
        if (name == existing[j]->GetName())
        {
            return true;
        }
    }
    return false;
}

// This checks a string against the name of a vector of Assemblies, and tells you whether or not the vector has it
G4bool Detector::hasAssembly(G4String name,std::vector<NamedG4AssemblyVolume*> existing)
{
    // Now look through the existing Assembly vector
    for (G4int j = 0;j < existing.size(); j++)
    {
        // Look for duplicate named Assembly
        if (name == existing[j]->GetName())
        {
            return true;
        }
    }
    return false;
}

// This function is used to obtain elements from the database.
G4Element* Detector::retrieveElement(std::vector<G4Element*> database, G4String search)
{
    for (G4int i = 0 ; i < database.size() ; i++)
    {
        if (database[i]->GetName() == search) //search
        {
            return database[i];
        }
    }
    return NULL;
}
// This function is used to retrieve a material from the database
G4Material* Detector::retrieveMaterial(std::vector<G4Material*> database, G4String search)
{
    for (G4int i = 0 ; i < database.size() ; i++)
    {
        if (database[i]->GetName() == search) //search
        {
            return database[i];
        }
    }
    return NULL;
}
// This function is used to retrieve a logical volume from the database
G4LogicalVolume* Detector::retrieveLogVol(std::vector<G4LogicalVolume*> database, G4String search)
{
    for (G4int i = 0 ; i < database.size() ; i++)
    {
        if (database[i]->GetName() == search) //search
        {
            return database[i];
        }
    }
    return NULL;
}
// This function is used to retrieve an assembly volume from the database
NamedG4AssemblyVolume* Detector::retrieveAssembly(std::vector<NamedG4AssemblyVolume*> database, G4String search)
{
    for (G4int i = 0 ; i < database.size() ; i++)
    {
        if (database[i]->GetName() == search) //search
        {
            return database[i];
        }
    }
    return NULL;
}
//////////////////////////////
// End General Fct. Section //
//////////////////////////////

//////////////////////
// MoNA Bar Section //
//////////////////////
// This documents the basic properties of a MoNA bar
MoNABar::MoNABar()
{
    activeSize.set(200.*cm,10.*cm,10.*cm); // size of the MoNA scintillators
    type = "MoNA.bar";
    //TODO: I have a number of duplicate numbers here, and scattered around the whole mona bar class
    // .... there is no reason that these cannot be part of the class itself to streamline things.
    G4double vinylthickness = 0.127*cm;
    G4ThreeVector spacing(240.0*cm,10.0*cm + 2.0 * vinylthickness,10.0*cm + 2.0 * vinylthickness);
    setSpacingSize(spacing);
}

// This creates the elements needed by a MoNA bar
std::vector<G4Element*> MoNABar::makeElements(std::vector<G4Element*> madeElements)
{
    G4String name = "";
    // First let's make Carbon
    name = "Carbon";
    // Need to check if we already have made this element
    if (!hasElement(name,madeElements))
    {
                        // Give a String name, string symbol, integer z, molar mass
        madeElements.push_back(new G4Element(name," C" ,6., 12.011*g/mole));
        //G4cout << "Added element " << name << "\n";
    }
    //Now let's make Hydrogen
    name = "Hydrogen";
    if (!hasElement(name,madeElements))
    {
                        // Give a String name, string symbol, integer z, molar mass
        madeElements.push_back(new G4Element(name," H" ,1., 1.008*g/mole));
        //G4cout << "Added element " << name << "\n";
    }

    //Now let's make Chlorine
    name = "Chlorine";
    if (!hasElement(name,madeElements))
    {
                        // Give a String name, string symbol, integer z, molar mass
        madeElements.push_back(new G4Element(name,"Cl" ,17., 35.453*g/mole));
        //G4cout << "Added element " << name << "\n";
    }

    //Now let's make Oxygen
    name = "Oxygen";
    if (!hasElement(name,madeElements))
    {
                        // Give a String name, string symbol, integer z, molar mass
        madeElements.push_back(new G4Element(name,"O" ,8., 15.999*g/mole));
        //G4cout << "Added element " << name << "\n";
    }

    //Now let's make Aluminum
    name = "Aluminum";
    if (!hasElement(name,madeElements))
    {
                        // Give a String name, string symbol, integer z, molar mass
        madeElements.push_back(new G4Element(name,"Al" ,13., 26.9815386*g/mole));
        //G4cout << "Added element " << name << "\n";
    }

    // Finally we return all the elements that now exist
    return madeElements;
}
// This creates the materials needed by a MoNA bar
std::vector<G4Material*> MoNABar::makeMaterials(std::vector<G4Material*> madeMaterials,std::vector<G4Element*> madeElements)
{
    // TODO: turn this into a text file input? Think about it anyway
    G4String name = "";

    // First we have to make the material... here it is the scintillator of MoNA
    name = "BC408";
    if (!hasMaterial(name,madeMaterials))
      {
                        // Give a String name, density, integer number of components

	//1.032 g/cm3 is the nominal density from the BC-408 datasheet.
	G4Material *Scintillator = new G4Material(name,1.032*g/cm3,2);
                                // want arguments of element, mass fraction (%)
	//91.5% C, 8.5%H gives the nominal ratio of C:H from datasheet.

        Scintillator->AddElement(retrieveElement(madeElements,"Carbon"), 91.5*perCent);   //BC408 , "Carbon" madeElements[]0
        Scintillator->AddElement(retrieveElement(madeElements,"Hydrogen"), 8.5*perCent);   //BC408, "Hydrogen"

        //Scintillator->AddElement(retrieveElement(madeElements,"Carbon"), 100.0*perCent);   //BC408 , "Carbon" madeElements[]0
        //Scintillator->AddElement(retrieveElement(madeElements,"Hydrogen"), 100.0*perCent);   //BC408, "Hydrogen"

        // Now add it to the material vector
        madeMaterials.push_back(Scintillator);
        //G4cout << "Added material " << name << "\n";
    }

    // Vinyl
    name = "Vinyl";
    if (!hasMaterial(name,madeMaterials))
    {
                        // Give a String name, density, integer number of components
        G4Material* Vinyl = new G4Material(name, 1.385*g/cm3, 3);	  
                                // want arguments of element, number of atoms
        Vinyl->AddElement(retrieveElement(madeElements,"Carbon"), 2);   
        Vinyl->AddElement(retrieveElement(madeElements,"Hydrogen"), 3);
        Vinyl->AddElement(retrieveElement(madeElements,"Chlorine"), 1);

        // Now add it to the material vector
        madeMaterials.push_back(Vinyl);
        //G4cout << "Added material " << name << "\n";
    }

    // Light Guide Material
    name = "BC800";
    if (!hasMaterial(name,madeMaterials))
    {
                        // Give a String name, density, integer number of components
        G4Material* BC800 = new G4Material(name, 1.19*g/cm3, 3);	  
                                // want arguments of element, number of atoms
        BC800->AddElement(retrieveElement(madeElements,"Hydrogen"), 8);
        BC800->AddElement(retrieveElement(madeElements,"Carbon"), 5);   
        BC800->AddElement(retrieveElement(madeElements,"Oxygen"), 2);

        // Light refraction properties
        G4MaterialPropertiesTable* BC800_MPT = new G4MaterialPropertiesTable();
        BC800_MPT->AddConstProperty("REFRACTIVEINDEX", 1.49);

        BC800->SetMaterialPropertiesTable(BC800_MPT);

        // Now add it to the material vector
        madeMaterials.push_back(BC800);
        //G4cout << "Added material " << name << "\n";
    }

    // Flange Material
    name = "Flange";
    if (!hasMaterial(name,madeMaterials))
    {
                        // Give a String name, density, integer number of components
        G4Material* Flange = new G4Material(name, 2.70*g/cm3, 1);	  
                                // want arguments of element, number of atoms
        Flange->AddElement(retrieveElement(madeElements,"Aluminum"), 1);

        // Now add it to the material vector
        madeMaterials.push_back(Flange);
        //G4cout << "Added material " << name << "\n";
    }

    return madeMaterials;
}
// This creates the logical volumes needed by a MoNA bar
std::vector<G4LogicalVolume*> MoNABar::makeLogVols(std::vector<G4LogicalVolume*> madeLogVols,std::vector<G4Material*> madeMaterials)
{
    // TODO: turn this into a text file input? Think about it anyway
    G4String name = "";

    // let's make a mona scintillator
    name = getType().append(".log");
    //first need a shape ... we need it out here because it is needed a few times
                            // args string name, size
    G4Box *scinBar = new G4Box("Scinbar1", getActiveSize().getX()/2, getActiveSize().getY()/2., getActiveSize().getZ()/2.);
    // make the volume if it hasn't already been made
    if (!hasLogVol(name,madeLogVols))
    {
                                // args shape, materials, string name
        G4LogicalVolume *Scin_log1 = new G4LogicalVolume(scinBar,retrieveMaterial(madeMaterials,"BC408"),name,0,0,0);
        // necessary for making the scintillators sensitive detectors
        Scin_log1->SetSensitiveDetector(getDetectorInterface()->getTrackerSD());

        //visualization attributes
        G4VisAttributes* ScinbarAttr = new G4VisAttributes(G4Colour(1., 0., 0.));   //Red
        ScinbarAttr->SetVisibility(true);
        //  ScinbarAttr->SetForceWireframe(true);
        //ScinbarAttr->SetForceSolid(true);
        Scin_log1->SetVisAttributes(ScinbarAttr);

        madeLogVols.push_back(Scin_log1);
        //G4cout << "Added logical volume " << name << "\n";
    }
    // let's make a mona vinyl cover
    name = getType().append("_vinyl.log");
    G4double vinylthickness = 0.127*cm;
    //G4double vinylthickness = 0.1*cm;
    // make the volume if it hasn't already been made
    if (!hasLogVol(name,madeLogVols))
    {
        G4double vinylXlength = 200.0*cm;
        G4double vinylYlength = 10.0*cm + 2.0 * vinylthickness;
        G4double vinylZlength = 10.0*cm + 2.0 * vinylthickness;

        G4double halfvinylXlength = 0.5 * vinylXlength;
        G4double halfvinylYlength = 0.5 * vinylYlength;
        G4double halfvinylZlength = 0.5 * vinylZlength;

        G4Box* Vinyl_box = new G4Box("Vinyl_box", halfvinylXlength, halfvinylYlength, halfvinylZlength);

        G4SubtractionSolid* Vinyl_wrap =  new G4SubtractionSolid("Vinyl_wrap", Vinyl_box, scinBar);
                                // args shape, materials, string name
        G4LogicalVolume* Vinyl_log = new G4LogicalVolume(Vinyl_wrap, retrieveMaterial(madeMaterials,"Vinyl"),name,0,0,0);

        
        //visualization attributes
        //G4VisAttributes* ScinbarAttr = new G4VisAttributes(G4Colour(1., 0., 0.));   //Red
        //ScinbarAttr->SetVisibility(true);
        //  ScinbarAttr->SetForceWireframe(true);
        //ScinbarAttr->SetForceSolid(true);
        //Scin_log1->SetVisAttributes(ScinbarAttr);
        

        madeLogVols.push_back(Vinyl_log);
        //G4cout << "Added logical volume " << name << "\n";
    } 
    // let's make a mona light guide
    name = getType().append("_LightGuide.log");
    // TODO: some of these sizes can be related to the ActiveSize... fix to make it so
    // TODO: this whole section is basically copied from the original MoNA simulation...
    //       ... it is pretty messy and really ought to be cleaned up
    G4double lgx1 = 0.5 * 127.0*mm;
    G4double lgx2 = 0.5 * 38.1*mm;
    G4double fLGDiameter = 50.8*mm;
    G4double fWorldLength= 5*m;
    G4double lgz1 = 0.5 * 10.0*cm;
    G4double lgy = 0.5 * 10.0*cm;
    G4double lgz2 = 0.5 * fLGDiameter;
    // make the volume if it hasn't already been made
    if (!hasLogVol(name,madeLogVols))
    {

        G4Trd* solidLGpartA = new G4Trd("LGA", lgz1, lgz2, lgy, lgz2, lgx1);
        G4Cons* solidLGpartB = new G4Cons("LGB", 0., sqrt(lgz1 * lgz1 + lgy * lgy), 0., lgz2, lgx1, 0., 2 * pi);
        G4Tubs* solidLGpartC = new G4Tubs("LGC", 0., lgz2, lgx2, 0., 2 * pi);

        G4RotationMatrix rm0;
        G4RotationMatrix rmr;
        rmr.rotateY(0.5 * pi);
        G4RotationMatrix rml;
        rml.rotateY(-0.5 * pi);

        G4IntersectionSolid* solidLGAB =  new G4IntersectionSolid("LGAB", solidLGpartA, solidLGpartB);
        
        G4UnionSolid* solidLGABC = new G4UnionSolid("LGABC", solidLGAB, solidLGpartC,
            G4Transform3D(rm0, G4ThreeVector(0, 0, (lgx1 + lgx2))));
        G4LogicalVolume* LG_log = new G4LogicalVolume(solidLGABC, retrieveMaterial(madeMaterials,"BC800"),name,0,0,0);
        madeLogVols.push_back(LG_log);
        //G4cout << "Added logical volume " << name << "\n";
    }
    // let's make a mona light guide cover
    name = getType().append("_LightGuideCover.log");
    // make the volume if it hasn't already been made
    if (!hasLogVol(name,madeLogVols))
    {
        G4RotationMatrix rm0;
        G4double vinylZ1length = lgz1 + 2 * vinylthickness;
        G4double vinylZ2length = lgz2 + 2 * vinylthickness;
        G4double vinylY1length = 5*cm + 2 * vinylthickness;

        G4Trd* solidLGpartAvinyl = new G4Trd("LGAvinyl", vinylZ1length, vinylZ2length, vinylY1length, vinylZ2length, lgx1);
        G4Cons* solidLGpartBvinyl = new G4Cons("LGBvinyl", 0., sqrt(vinylZ1length * vinylZ1length + vinylY1length * vinylY1length),
		  0., vinylZ2length, lgx1, 0., 2 * pi);
        G4Tubs* solidLGpartCvinyl = new G4Tubs("LGCvinyl", 0., vinylZ2length, lgx2, 0., 2 * pi);


        G4IntersectionSolid* solidLGABvinyl =  new G4IntersectionSolid("LGABvinyl", solidLGpartAvinyl, solidLGpartBvinyl);

        G4UnionSolid* solidLGABCvinyl = new G4UnionSolid("LGABCvinyl", solidLGABvinyl, solidLGpartCvinyl,
            G4Transform3D(rm0, G4ThreeVector(0, 0, (lgx1 + lgx2))));
  

        G4Trd* solidLGpartA = new G4Trd("LGA", lgz1, lgz2, lgy, lgz2, lgx1);
        G4Cons* solidLGpartB = new G4Cons("LGB", 0., sqrt(lgz1 * lgz1 + lgy * lgy), 0., lgz2, lgx1, 0., 2 * pi);
        G4Tubs* solidLGpartC = new G4Tubs("LGC", 0., lgz2, lgx2, 0., 2 * pi);

        G4RotationMatrix rmr;
        rmr.rotateY(0.5 * pi);
        G4RotationMatrix rml;
        rml.rotateY(-0.5 * pi);

        G4IntersectionSolid* solidLGAB =  new G4IntersectionSolid("LGAB", solidLGpartA, solidLGpartB);
        
        G4UnionSolid* solidLGABC = new G4UnionSolid("LGABC", solidLGAB, solidLGpartC,
            G4Transform3D(rm0, G4ThreeVector(0, 0, (lgx1 + lgx2))));
        G4SubtractionSolid* VinylLG_wrap =  new G4SubtractionSolid("VinylLG_wrap", solidLGABCvinyl, solidLGABC);
        G4LogicalVolume* VinylLG_log = new G4LogicalVolume(VinylLG_wrap, retrieveMaterial(madeMaterials,"Vinyl"),name,0,0,0);
        madeLogVols.push_back(VinylLG_log);
        //G4cout << "Added logical volume " << name << "\n";
    }

    return madeLogVols;
}

std::vector<NamedG4AssemblyVolume*> MoNABar::makeAssemblies(std::vector<NamedG4AssemblyVolume*> madeAssemblies,std::vector<G4LogicalVolume*> madeLogVols)
{
    G4String name = "";
    // let's make a mona scintillator
    name = getType().append(".assembly");
    // make the volume if it hasn't already been made
    if (!hasAssembly(name,madeAssemblies))
    {
        NamedG4AssemblyVolume* MoNAAssembly = new NamedG4AssemblyVolume();
        MoNAAssembly->SetName(name);
        ////////////////////
        // Begin crazy crap
        ///////////////////
        // Rotation and translation of a plate inside the assembly
        G4RotationMatrix Ra;
        G4ThreeVector Ta; 

        // Rotation of the assembly inside the world
        G4RotationMatrix Rm;

        // Fill the assembly with each needed logical volume
        
        Ta.setX(0.); Ta.setY(0.); Ta.setZ(0.);
        G4Transform3D Vinyl_final = G4Transform3D(Ra, Ta);
        MoNAAssembly->AddPlacedVolume(retrieveLogVol(madeLogVols,getType().append("_vinyl.log")), Vinyl_final);
        
        Ta.setX( 0. ); Ta.setY( 0. ); Ta.setZ( 0. );
        G4Transform3D Scin_final = G4Transform3D(Ra, Ta);
        MoNAAssembly->AddPlacedVolume(retrieveLogVol(madeLogVols,getType().append(".log")), Scin_final);

        G4double lgx1 = 0.5 * 127.0*mm;
        G4double lgx2 = 0.5 * 38.1*mm;
        G4double fLGDiameter = 50.8*mm;
        G4double fWorldLength= 5*m;
        G4double lgz1 = 0.5 * 10.0*cm;
        G4double lgy = 0.5 * 10.0*cm;
        G4double lgz2 = 0.5 * fLGDiameter;
        G4RotationMatrix rmr;
        rmr.rotateY(0.5 * pi);
        G4RotationMatrix rml;
        rml.rotateY(-0.5 * pi);
	      
        Ta.setX( lgx1 + 100.0*cm ); Ta.setY( 0. ); Ta.setZ( 0. );
        G4Transform3D LG_right = G4Transform3D(rmr, Ta);
        MoNAAssembly->AddPlacedVolume(retrieveLogVol(madeLogVols,getType().append("_LightGuide.log")), LG_right);

        Ta.setX( -1 * (lgx1 + 100.0*cm) ); Ta.setY( 0. ); Ta.setZ( 0. );
        G4Transform3D LG_left = G4Transform3D(rml, Ta);
        MoNAAssembly->AddPlacedVolume(retrieveLogVol(madeLogVols,getType().append("_LightGuide.log")), LG_left);

        Ta.setX( lgx1 + 100.0*cm ); Ta.setY( 0. ); Ta.setZ( 0. );
        G4Transform3D VinylLG_right = G4Transform3D(rmr, Ta);
        MoNAAssembly->AddPlacedVolume(retrieveLogVol(madeLogVols,getType().append("_LightGuideCover.log")), VinylLG_right);

        Ta.setX( -1 * (lgx1 + 100.0*cm) ); Ta.setY( 0. ); Ta.setZ( 0. );
        G4Transform3D VinylLG_left = G4Transform3D(rml, Ta);
        MoNAAssembly->AddPlacedVolume(retrieveLogVol(madeLogVols,getType().append("_LightGuideCover.log")), VinylLG_left);
	
        ///////////////////////
        //End crazy crap
        ////////////////////////
        // Set the spacing size so that the detectors know how close they can be
        madeAssemblies.push_back(MoNAAssembly);
        //G4cout << "Added Assembly " << name << "\n";
        //TODO: everything in this region really needs to be cleaned up
    }
    return madeAssemblies;
}
// This is used to create the physical assemblies of MoNA bars
void MoNABar::addPhysVol(G4LogicalVolume* mother)
{
    G4RotationMatrix Rm = *(getRotation());
    G4ThreeVector Tm(getPosition().getX(), getPosition().getY(), getPosition().getZ());
    G4Transform3D Assembly_placement = G4Transform3D(Rm, Tm);
    getAssembly()->MakeImprint(mother, Assembly_placement);

    // Now going to loop over the made physical volumes to find the ones we want to be sensitive detectors, etc
    unsigned int limit = getAssembly()->TotalImprintedVolumes();
    std::vector<G4VPhysicalVolume*>::iterator iterator = getAssembly()->GetVolumesIterator();
    for (unsigned int i = 0; i < limit; i++)
    {
        G4VPhysicalVolume* currentDet = *iterator;
        G4String defaultName = currentDet->GetName();
        // the default name is av_WWW_impr_XXX_YYY_ZZZ where
        // WWW: assembly volume instance number
        // XXX: assembly volume imprint number
        // YYY: name of the placed logical volume
        // ZZZ: the logical volume index inside the assembly volume
        // we want to look for those of the "active" type... delimited by
        // getType() + ".log"
        // First we're gonna strip out the stuff we don't care about
        G4String trash,logName,detType;
        std::stringstream stream(defaultName);
        for (unsigned int i = 0; i < 4; i++)
        {
            std::getline(stream,trash,'_');
        }
        // Now we look at the name of the logical volume
        // and compare to the active logical volume name
        // give by "type.log"
        std::getline(stream,logName,'_');
        detType = getType() + ".log";
        //TODO: this currently only allows for a single active volume per assembly.
        // make it so that multiple active areas can be defined
        if (logName == detType)
        {
            // Now we rename the detector properly
            currentDet->SetName(getName());
            break;
        }
        iterator++;
    }
}
std::vector<G4double> MoNABar::processLight(G4Step* aStep)
{
    std::vector<G4double> lightInfo;
    G4double edep = aStep->GetTotalEnergyDeposit();
    // Do light output
    // TODO: is this a little primitive? Look into how this could be done better
    G4double delta = (1./1.032)*edep/(aStep->GetStepLength()/cm);
    G4double BirkFactor;
    G4Track *aTrack = aStep->GetTrack();
    if (aTrack->GetDefinition()->GetPDGCharge()<2)
    {
        BirkFactor = 1. + 0.013*delta + (9.6e-6)*delta*delta;
    }
        else
    {
        BirkFactor = 1. + (7.2/12.6)*0.013*delta + (9.6e-6)*delta*delta;
    }
    //Factor of 2.72 is to calibrate to muon in the center of a bar.
    G4double stepLight = 2.72*edep/BirkFactor; // This is the light made by the energy deposition
    // Now do the attenuation itself
    //Attenuation length is 4.3 meters.
    //Adds light detected to individual counters for right and left detectors.
    // TODO: is there any reason to favor post vs pre step position?
    G4ThreeVector loc = aStep->GetPostStepPoint()->GetPosition() - getPosition(); // First apply the translation vector of the detector
    G4RotationMatrix* rotate = getRotation();
    loc.transform(rotate->inverse()); // transform with the inverse of the matrix to return to internal coordinate system
    G4double xpos = loc.x();
    //G4double leftLight = stepLight*exp(-(xpos+100.*cm)/(4.3*m))/2.;
    //G4double rightLight= stepLight*exp(-(100.*cm-xpos)/(4.3*m))/2.;
    G4double leftLight = stepLight*(0.528 + 0.472*exp(-(xpos+100.*cm)/(1.34*m)))/2;
    G4double rightLight = stepLight*(0.528 + 0.472*exp(-(100.*cm-xpos)/(1.34*m)))/2;

    lightInfo.push_back(leftLight);
    lightInfo.push_back(rightLight);

    return lightInfo;
}
std::vector<G4double> MoNABar::addResolutions(G4Step* aStep)
{    
/**
const gsl_rng_type * T;
gsl_rng * r;
     
int i, n = 10;
double mu = 3.0;
double k;     
     
gsl_rng_env_setup();
     
T = gsl_rng_default;
r = gsl_rng_alloc (T);
     
G4cout << "Starting new rng now: "<< G4endl;
for (i = 0; i < n; i++) 
{
//k = gsl_ran_gaussian (r, mu);
G4cout << "The new rng is " << gsl_ran_flat(r, 0.,1.) << G4endl;
}    
gsl_rng_free (r);
**/
    std::vector<G4double> AfterResolutions;
    // TODO: is there any reason to favor post vs pre step position?
    G4ThreeVector barloc = getPosition(); // First apply the translation vector of the detector
    //G4ThreeVector hitloc = aStep->GetPostStepPoint()->GetPosition(); // First apply the translation vector of the detector

    G4ThreeVector loc = aStep->GetPostStepPoint()->GetPosition() - getPosition(); // First apply the translation vector of the detector
    G4RotationMatrix* rotate = getRotation();
    loc.transform(rotate->inverse()); // transform with the inverse of the matrix to return to internal coordinate system
    //G4cout << "The bar z loc is " << barloc.z() << G4endl;
    //G4cout << "The old x loc is " << loc.x() << G4endl;
    //loc.setX(loc.x() + gsl_ran_gaussian_pdf(0,3.));
    //G4cout << "The new x loc is " << loc.x() << G4endl;
    //G4cout << "The new rng is " << getXRNG() << G4endl;

/* remove resolutions */

    loc.setZ(0.);
    loc.setY(0.);


    //X and T resolutions should be handled after TrackerHits are summed.
    //Moving these to OutputHandler.cc
    //loc.setX(G4RandGauss::shoot(loc.getX(),3*cm));
    loc.transform(*rotate); // transform with the inverse of the matrix to return to internal coordinate system
    G4ThreeVector newhitloc = loc + getPosition();; // First apply the translation vector of the detector

   
    G4double xpos = newhitloc.x();
    G4double ypos = newhitloc.y();
    G4double zpos = newhitloc.z();

    //G4double tof = aStep->GetTrack()->GetGlobalTime();
    //tof = G4RandGauss::shoot(tof,0.18*ns);

    //AfterResolutions.push_back(xpos);
    AfterResolutions.push_back(ypos);
    AfterResolutions.push_back(zpos);
    //AfterResolutions.push_back(tof);

    return AfterResolutions;
}
//////////////////////////
// End MoNA Bar Section //
//////////////////////////


// This documents the basic properties of a CMDA bar
CMDABar::CMDABar(G4ThreeVector barPos,G4ThreeVector barFace,G4String barName)
{
    activeSize.set(100.*cm,6.25*cm,6.25*cm); // size of the MoNA scintillators
    type = "CMDA";
    position = barPos;
    facing = barFace;
    name = barName;
}

// This documents the basic properties of a PHOBOS time of flight scintillator
PHOBOSToFBar::PHOBOSToFBar()
{
    activeSize.set(20.0*cm,0.8*cm,0.8*cm); // Size of each bar... 4 per set of PMTs
    type = "PHOBOSToF";
}

// This documents the basic properties of a long CAESAR scintillator
LongCAESARScint::LongCAESARScint()
{
    activeSize.set(10.16*cm,5.4*cm,5.4*cm);
    type = "LongCAESAR";
}

// This documents the basic properties of a short CAESAR scintillator
ShortCAESARScint::ShortCAESARScint()
{
    activeSize.set(7.62*cm,7.95*cm,7.95*cm);
    type = "ShortCAESAR";
}

