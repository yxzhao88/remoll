#include "remollSteppingAction.hh"
//#include "remollSteppingActionMessenger.hh"

#include "G4VVisManager.hh"
#include "G4Polyline.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4SteppingManager.hh"

remollSteppingAction::remollSteppingAction()
:drawFlag(false)
{
///  new remollSteppingActionMessenger(this);

    fEnableKryptonite = true;
}

void remollSteppingAction::UserSteppingAction(const G4Step *aStep) {
    // G4Track* fTrack = aStep->GetTrack();
    // G4Material* material = fTrack->GetMaterial();

    // G4double stepLen = aStep->GetStepLength();

    // // Don't continue in these materials
    // if( (  material->GetName()=="Tungsten" )
    //     // ||  material->GetName()=="Pb"
    // 	// ||  material->GetName()=="Copper" )
    // 	//	    && fEnableKryptonite
    // 	){
    //   G4double edep = aStep->GetTotalEnergyDeposit();
    //   G4cout << "UserSteppingAction:: Executing KillTrackAndSecondaries ....; edep:: "<< edep<< "\t etot:: "<< fTrack->GetTotalEnergy() << "\tStepLen:: "<< stepLen << G4endl;
    //   //fTrack->SetTrackStatus(fKillTrackAndSecondaries);

    // }

}


