#include "RunAction.hh"
#include "Analysis.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"


RunAction::RunAction()
 : G4UserRunAction()
{ 
// Create analysis manager
  
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  G4cout << "Using " << analysisManager->GetType() << G4endl;

  analysisManager->SetVerboseLevel(1);
  analysisManager->SetFirstHistoId(1);        // Histo Id starts from 1 not from0

  // Book histograms, ntuple
  //
  
  // Creating histograms
    G4String Hist_Name[20]=
    {"BGO1", "BGO2", "BGO3", "BGO4", "BGO5", "BGO6", "BGO7", "BGO8", "BGO9", "BGO10",
        "BGO11", "BGO12", "BGO13", "BGO14", "BGO15", "BGO16", "BGO17", "BGO18", "BGO19", "BGO20"};

    for(G4int i=0; i<20; i++){
        analysisManager->CreateH1(Hist_Name[i],"Edep in BGO", 100, 0., 100*MeV);
    }
    analysisManager->CreateH1("Sum","Edep Sum", 100, 0., 10000*MeV);
    analysisManager->CreateH1("Escape","Escape Sum", 100, 0., 10000*MeV);

  // Creating ntuple
}

RunAction::~RunAction()
{
  delete G4AnalysisManager::Instance();  
}

void RunAction::BeginOfRunAction(const G4Run* /*run*/)
{
    // Get analysis manager and open an output file
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  G4String fileName = "P06";
  analysisManager->OpenFile(fileName);
}

void RunAction::EndOfRunAction(const G4Run* /*run*/)
{
  //save histograms & ntuple
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  G4cout << "Run Summary" << G4endl;
  G4double sum = 0.;
      for(G4int i=1; i<=20; i++){
        G4double depo = analysisManager->GetH1(i)->mean();
	G4cout << " mean depo in  "<<  i << " layer =  "<< G4BestUnit(depo, "Energy") <<G4endl;
	sum =sum + depo;
	}
	G4cout << " sum =  "<< G4BestUnit(sum, "Energy") << G4endl;
  analysisManager->Write();
  analysisManager->CloseFile();

}

