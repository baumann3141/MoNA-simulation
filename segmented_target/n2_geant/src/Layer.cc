#include "Layer.hh"

Layer::Layer() //DetectorInterface* detectorInterface
{
    position.set(0.*cm,0.*cm,0.*cm);
    // by default the layer faces along the z axis
    facing.set(0.,0.,1.);
    name = "";
    type = "";
    //interface = detectorInterface;
}


Layer::~Layer()
{
    ;
}

// Adds detectors to the layer
// TODO: layer style file implementation + detector rotation possibilities
// TODO: clean this function up... right now it works but is messy
void Layer::addDetectors(G4int nBars,G4ThreeVector pos,DetectorInterface* detectorInterface,G4String type,G4String arrayName,G4int layerNum, G4double facingRotation)
{
    // TODO: i'm pretty sure I don't need this many vectors... see about cleaning it up
    // This is used to represent the plane we are rotating in
    G4ThreeVector* planeVec = new G4ThreeVector;
    // This represents the facing of the detector as we move through the rotations
    G4ThreeVector* currentFacing = new G4ThreeVector;
    // This represents the direction about which we are rotating
    G4ThreeVector* yRotationDir = new G4ThreeVector;
    G4ThreeVector* xRotationDir = new G4ThreeVector;
    // This is for the positioning of the detector in the internal coords of the layer
    G4ThreeVector* layerCoordSysDetPos = new G4ThreeVector;
    G4ThreeVector* labCoordSysDetPos = new G4ThreeVector; // this is for the lab system
    // loop over all the bars
    for (G4int i = 0; i < nBars; i++)
    {
        // Note: this currently simply stacks the detectors in the y-direction.
        Detector* newDetector = detectorInterface->makeDetector(i,nBars,type,arrayName,layerNum);
        newDetector->setFacing(getFacing());

        // We need these to do the rotations so the detector actually aims where we told it to
        // We start by rotating about the y-axis
        ///////////////////////////
        // Begin Y-axis rotation //
        ///////////////////////////
        //TODO: make sure all the correct vectors are normalized
        currentFacing->set(0.,0.,1.);
        yRotationDir->set(0.,1.,0.);

        G4RotationMatrix* rotation = new G4RotationMatrix();
        G4RotationMatrix* xRotation = new G4RotationMatrix();
        G4RotationMatrix* yRotation = new G4RotationMatrix();
        G4RotationMatrix* faceRotation = new G4RotationMatrix();
        // First up, we wish to rotate about y... need to look at x/z plane
        planeVec->set(newDetector->getFacing().getX(),0.,newDetector->getFacing().getZ());
        //G4cout << "The vector i am rotating to is " << *planeVec << "\n";
        G4double angle = planeVec->azimAngle(*currentFacing,*yRotationDir); //use azimangle because it happens to do what we want
        //G4cout << "The angle is " << angle << "\n";
        yRotation->rotateY(angle*rad);
        currentFacing->rotateY(-angle*rad); // a -sign because of how vectors do it relative to matrices?
        //currentFacing->transform(xRotation->inverse());
        //G4cout << "My current facing is now " << *currentFacing << "\n";
        /////////////////////////
        // End Y-axis rotation //
        /////////////////////////

        ///////////////////////////
        // Begin X-axis rotation //
        ///////////////////////////
        // It is actually about the "new" x-axis defined by the new direction
        xRotationDir->set(1.,0.,0.); // Set to x-axis, then we rotate..
        xRotationDir->rotateY(-angle*rad); // - sign because of weird? TODO: figure out how to avoid this silly hard-coded thing
        //xRotationDir->transform(yRotation->inverse());
        //G4cout << "My new rotation direction is " << *xRotationDir << "\n";
        planeVec->set(newDetector->getFacing().getX(),newDetector->getFacing().getY(),newDetector->getFacing().getZ());
        angle = planeVec->azimAngle(*currentFacing,*xRotationDir);
        //G4cout << "My x angle of rotation is " << angle << "\n";
        xRotation->rotate(angle*rad,*xRotationDir); // Now add this rotation

        /////////////////////////
        // End X-axis rotation //
        /////////////////////////

        ////////////////////////////////////
        // Begin Facing Rotation rotation //
        ////////////////////////////////////
        faceRotation->rotate(facingRotation,getFacing());
        *rotation = (*yRotation)*(*xRotation)*(*faceRotation); // The order is relevant... it makes it so the y comes 1st
        //////////////////////////////////
        // End Facing Rotation rotation //
        //////////////////////////////////

        ////////////////////////////////
        // Begin Coordinate Transform //
        ////////////////////////////////
        // This is to put the detectors in the right place relative to the internal frame of the layer
        // First we set everything to 0 to initialize
        layerCoordSysDetPos->set(0.*cm,0.*cm,0.*cm);
        labCoordSysDetPos->set(0.*cm,0.*cm,0.*cm);

        // Now we stack things
        G4double ySize = newDetector->getSpacingSize().getY();
        //TODO:make sure you explicitly type cast everything so there is no doubt about the math
        layerCoordSysDetPos->setY(-ySize*(nBars-1)/2. + i*ySize);

        // Now we have to apply the rotation
        layerCoordSysDetPos->transform(*rotation);
        //G4cout << "I am making detector at internal pos " << *layerCoordSysDetPos << "\n";

        // Now we move to the lab frame
        *labCoordSysDetPos = *layerCoordSysDetPos + getPosition();
        //G4cout << "I am making detector at lab pos " << *labCoordSysDetPos << "\n";
        newDetector->setPosition(*labCoordSysDetPos);
        //////////////////////////////
        // End Coordinate Transform //
        //////////////////////////////

        // Finally, we can set the end rotation and add the detector to the layer
        newDetector->setRotation(rotation);
        detectors.push_back(newDetector);
    }
    
}
