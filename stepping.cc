#include "stepping.hh"

MySteppingAction::MySteppingAction(MyEventAction *eventAction)
{
    fEventAction = eventAction;
}

MySteppingAction::~MySteppingAction()
{}

void MySteppingAction::UserSteppingAction(const G4Step* step)
{
    //get the total energy deposit only in the radiator
    G4LogicalVolume *volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();
    const MyDetectorConstruction *detectorConstruction = 
    static_cast<const MyDetectorConstruction*>(G4RunManager::GetRunManager()->GetUserDetectorConstruction());

    G4LogicalVolume *fScoringVolume = detectorConstruction->GetScoringVolume();

    if (volume != fScoringVolume) return;
    // if (volume->GetName() != "Radiator") return;

    // G4LogicalVolume *radiator = G4LogicalVolumeStore::GetInstance()->GetVolume("Radiator");

    G4double edep = step->GetTotalEnergyDeposit();
    fEventAction->AddEdep(edep);
}