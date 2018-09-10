

#include "SteppingAction.hh"
#include "EventAction.hh"
#include "Geometry.hh"

#include "G4Step.hh"
#include "G4Track.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

SteppingAction::SteppingAction(EventAction* eventAction)
: G4UserSteppingAction(),
  fEventAction(eventAction),
  fScoringVol(0)
{}
SteppingAction::~SteppingAction()
{}

void SteppingAction::UserSteppingAction(const G4Step* step)
{
    if (!fScoringVol) {
        const Geometry* geometry
             = static_cast<const Geometry*>
            (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
         fScoringVol = geometry->GetScoringVol();
    }
// Get PreStepPoint and TouchableHandle objects
    G4StepPoint* preStepPoint=step->GetPreStepPoint();
    G4TouchableHandle preTouchable = preStepPoint->GetTouchableHandle();
//  Get the name of logical volume 
    G4LogicalVolume* preVolume = preTouchable->GetVolume()->GetLogicalVolume();
    G4String preVolName = preVolume->GetName();

//if step is within the scoring volume
   if(preVolume==fScoringVol){
       G4double edepStep = step->GetTotalEnergyDeposit();
       G4int copyNo = preTouchable->GetCopyNumber();
       fEventAction->AddeDep(copyNo-1000, edepStep);      // #0 ~ #19 crystal
   }else{
//PreStep reached boundary and is just in the logical World
  if (preStepPoint->GetStepStatus()== fGeomBoundary && preVolName =="LogVol_World"){
        G4Track* aTrack = step->GetTrack(); 
	G4double  eScape = aTrack->GetKineticEnergy();
        fEventAction->AddeDep(noBGO, eScape);
	aTrack->SetTrackStatus(fStopAndKill);  // no further tracking
   }
  }
}


