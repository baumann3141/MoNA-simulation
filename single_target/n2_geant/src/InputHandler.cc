#include "InputHandler.hh"
/*
InputHandler::InputHandler()
{
    ;
}
*/
// Check that the input line to describe the new layer has the right number of entries
bool InputHandler::validLayerInput(G4String input,int columns)
{
    int spaceCount = 0;
    // We check to see that there are the correct number of columns
    // TODO: improve this so it checks whitespace instead of simply spaces
    for (unsigned int i = 0; i <= input.length(); i++)
    {
        if (!input.compare(i,1," "))
            spaceCount += 1;
    }

    //G4cout << "i counted " << spaceCount << "spaces\n";
    if (spaceCount == columns)
    {
        return true;
    }
    else
    {
        return false;
    }
}
