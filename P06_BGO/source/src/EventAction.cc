
#include "EventAction.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"

#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "Analysis.hh"
#include "Randomize.hh"
#include <iomanip>


EventAction::EventAction()
 : G4UserEventAction() 
{}


EventAction::~EventAction()
{}

void EventAction::BeginOfEventAction(const G4Event* /*event*/)
{
    for(G4int i=0; i<=noBGO;i++){
        sum_eDep[i] = 0.;
    }


}

void EventAction::EndOfEventAction(const G4Event* /*event*/)
{
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    G4double sum_BGO = 0.;
    for(G4int i=0; i<noBGO; i++){
        GetSum(i);
        analysisManager->FillH1(i+1, GetSum(i));   //H1 id = 1 ~ 20
        sum_BGO = sum_BGO + GetSum(i);
    }
    G4double eLeak = GetSum(noBGO);
    G4cout << "sum_BGO "<<G4BestUnit(sum_BGO, "Energy") << " escape "<<G4BestUnit(eLeak, "Energy") << "  and total energy "<< G4BestUnit(sum_BGO + eLeak, "Energy") << G4endl;
    analysisManager->FillH1(21, sum_BGO);
    analysisManager->FillH1(22, eLeak);  // event sum of escape
}

