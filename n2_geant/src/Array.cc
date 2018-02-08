#include "Array.hh"

Array::Array(G4String name)
{
    tgtPos.set(0.,0.,0.);
    arrayName = name;
    layer.clear();
}

// This is used to add a layer to the array
void Array::addLayer(G4String info,DetectorInterface* detectorInterface)
{
    //TODO: add proper exception handling
    //TODO: speed things up by doing it all with pointers
    Layer* newLayer = new Layer();

    G4String xIn,yIn,zIn,nBarsIn,xFIn,yFIn,zFIn,rotateIn,layerType,layerName;
    G4ThreeVector layerFacingIn,layerPositionIn;
    G4double facingRotation;
    std::istringstream breaker(info);
    breaker >> xIn >> yIn >> zIn >> nBarsIn >> xFIn >> yFIn >> zFIn >> rotateIn >> layerType >> layerName;

    // Determine the positioning of the center of the array
    // For now you simply use absolute coords
    // TODO: implement relative and auto-gap spacing methods
    layerPositionIn.setX(strtod(xIn,NULL)*cm); // we set the layer position vector
    layerPositionIn.setY(strtod(yIn,NULL)*cm);
    layerPositionIn.setZ(strtod(zIn,NULL)*cm);
    newLayer->setPosition(layerPositionIn);

    // Input the number of detectors in the layer
    // TODO: add the ability to set a layer style as opposed to the number
    newLayer->setNBars(atoi(nBarsIn));

    // Input the facing vector
    layerFacingIn.setX(strtod(xFIn,NULL)); // we set the layer facing vector
    layerFacingIn.setY(strtod(yFIn,NULL));
    layerFacingIn.setZ(strtod(zFIn,NULL));
    layerFacingIn.setMag(1.0);

    // Input the facing rotation
    facingRotation = strtod(rotateIn,NULL)*deg;


    if (true) //TODO: make this check if its valid to manually override
    {
        newLayer->setFacing(layerFacingIn); // manually settings to the vector always override auto settings
    }

    newLayer->setName(layerName); // set the layer name
    newLayer->setType(layerType); // set the layer type
    // Add detectors to the array ... needs number of detectors, position of layer, interface, type, name of array, and which layer
    newLayer->addDetectors(newLayer->getNBars(),newLayer->getPosition(),detectorInterface,newLayer->getType(), getArrayName(), layer.size(),facingRotation);

    // Add the layer to the array
    layer.push_back(newLayer);
}

