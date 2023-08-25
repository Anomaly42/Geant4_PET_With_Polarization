#include "event.hh"

MyEventAction::MyEventAction(){
    fEdep = 0;
}

MyEventAction::~MyEventAction()
{}

void MyEventAction::BeginOfEventAction(const G4Event* event)
{
    fEdep = 0;
}

void MyEventAction::EndOfEventAction(const G4Event* event)
{
    // G4cout << "Energy Deposition: " << fEdep/keV << " keV" << G4endl;

    // G4AnalysisManager* man = G4AnalysisManager::Instance();
    // man->FillNtupleDColumn(2, 0, fEdep);
    // man->AddNtupleRow(2);
}