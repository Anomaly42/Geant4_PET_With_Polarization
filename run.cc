#include "run.hh"

MyRunAction::MyRunAction()
{
    G4AnalysisManager* man = G4AnalysisManager::Instance();

    man->CreateNtuple("Photons", "Photons");
    man->CreateNtupleDColumn("fEvent");
    man->CreateNtupleDColumn("fX");
    man->CreateNtupleDColumn("fY");
    man->CreateNtupleDColumn("fZ");
    man->CreateNtupleDColumn("fEnergy");
    man->CreateNtupleDColumn("fTime");
    man->CreateNtupleDColumn("fPolX");
    man->CreateNtupleDColumn("fPolY");
    man->CreateNtupleDColumn("fPolZ");
    man->CreateNtupleSColumn("sName");


    man->FinishNtuple(0);

    man->CreateNtuple("Hits", "Hits");
    man->CreateNtupleDColumn("fEvent");
    man->CreateNtupleDColumn("fX");
    man->CreateNtupleDColumn("fY");
    man->CreateNtupleDColumn("fZ");
    man->FinishNtuple(1);

    man->CreateNtuple("Scoring", "Scoring");
    man->CreateNtupleDColumn("fEdep");
    man->FinishNtuple(2);
}

MyRunAction::~MyRunAction()
{}

void MyRunAction::BeginOfRunAction(const G4Run* run)
{
    G4AnalysisManager* man = G4AnalysisManager::Instance();
    G4int runID = run->GetRunID();

    std::stringstream strRunID;
    strRunID << runID;

    man->OpenFile("hello" + strRunID.str() + ".root");
    //Let's use random numbers for the output file name

    // std::random_device rd;

    // std::uniform_int_distribution<int> dist(0, 1000000);

    // man->OpenFile("output" + std::to_string(dist(rd)) + ".root");


}

void MyRunAction::EndOfRunAction(const G4Run*)
{
    G4AnalysisManager* man = G4AnalysisManager::Instance();

    man->Write();
    man->CloseFile();
}