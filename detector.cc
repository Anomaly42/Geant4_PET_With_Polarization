#include "detector.hh"

MySensitiveDetector::MySensitiveDetector(G4String name) : G4VSensitiveDetector(name){
    quEff = new G4PhysicsFreeVector();

    std::ifstream datafile;
    datafile.open("eff.dat");

    while (1) {
        G4double wlen, eff;
        datafile >> wlen >> eff;

        if (datafile.eof()) break;

        G4cout << wlen << " " << eff << G4endl;
        quEff->InsertValues(wlen*nm, eff/100.); //effDciency is in %
    }

    datafile.close();
    // quEff.SetSpline(false); //use only linear interpolation
}
MySensitiveDetector::~MySensitiveDetector(){}

G4bool MySensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist){
    G4Track *track = aStep->GetTrack();

    track->SetTrackStatus(fStopAndKill);

    G4StepPoint *preStepPoint = aStep->GetPreStepPoint(); //WARNING, be careful about pre and post step for hadrons; photons are fine
    G4StepPoint *postStepPoint = aStep->GetPostStepPoint();

    G4ThreeVector posPhoton = preStepPoint->GetPosition();
    G4ThreeVector momPhoton = postStepPoint->GetMomentum();
    G4double wlen = (1.239814939*eV/momPhoton.mag())*1E+03; //wavelength in nm
    G4double energykeV = (1.239814939/wlen);

    G4double time = preStepPoint->GetGlobalTime();
    // G4cout << "TIMEEEEEEEEEEEEEEEEEEE" << G4endl;
    // G4cout << time/s << G4endl;

    // G4cout << "WLEMNNNNNNNNNNNNNNNNNN" << G4endl;
    // G4cout << wlen << G4endl;


    // G4cout << "Energyyyyyyyy (keV)" << G4endl;
    // G4cout << energykeV << G4endl;

    G4ThreeVector polarization = preStepPoint->GetPolarization();
    // G4cout << "Polarization: " << polarization << "At: " << preStepPoint->GetPosition() << G4endl;
    // G4cout << "Time: " << time << G4endl;


    const G4VTouchable *touchable = preStepPoint->GetTouchable();

    G4int copyNo = touchable->GetVolume()->GetCopyNo();

    //G4cout << "Copy Number: " << copyNo << G4endl;

    G4VPhysicalVolume *physVol = touchable->GetVolume();
    G4ThreeVector posDetector = physVol->GetTranslation();

    // G4cout << "Detector Position: " << posDetector << G4endl;

    G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();

    G4AnalysisManager *man = G4AnalysisManager::Instance();

    man->FillNtupleDColumn(0, 0, (G4double)evt);
    man->FillNtupleDColumn(0, 1, (G4double)posPhoton[0]);
    man->FillNtupleDColumn(0, 2, (G4double)posPhoton[1]);
    man->FillNtupleDColumn(0, 3, (G4double)posPhoton[2]);
    man->FillNtupleDColumn(0, 4, (G4double)energykeV);
    man->FillNtupleDColumn(0, 5, (G4double)time/ns);
    man->FillNtupleDColumn(0, 6, (G4double)polarization[0]);
    man->FillNtupleDColumn(0, 7, (G4double)polarization[1]);
    man->FillNtupleDColumn(0, 8, (G4double)polarization[2]);


    man->AddNtupleRow(0);

    // G4cout << "Wavelength: " << wlen << G4endl;
    // G4cout << "Quantum Efficiency: " << quEff->Value(wlen*nm) << G4endl;


    // if (G4UniformRand() < quEff->Value(wlen*nm)){
        man->FillNtupleDColumn(1, 0, evt);
        man->FillNtupleDColumn(1, 1, posDetector[0]);
        man->FillNtupleDColumn(1, 2, posDetector[1]);
        man->FillNtupleDColumn(1, 3, posDetector[2]);
        man->AddNtupleRow(1);
    // }
}