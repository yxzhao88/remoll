#include "remollVertex.hh"

#include "CLHEP/Units/SystemOfUnits.h"

#include "G4Material.hh"

remollVertex::remollVertex(){
    // Some default material
    fMaterial = NULL;
    fBeamE = 0.0*CLHEP::GeV;
    fRadLen = 0.0;
}

remollVertex::~remollVertex(){
}
