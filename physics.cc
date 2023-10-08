#include "physics.hh"

MyPhysicsList::MyPhysicsList() : G4VModularPhysicsList()
{
    RegisterPhysics(new G4EmStandardPhysics());
    RegisterPhysics(new G4OpticalPhysics());
}

MyPhysicsList::~MyPhysicsList(){}

    // RegisterPhysics(new G4DecayPhysics());
    // RegisterPhysics(new G4RadioactiveDecayPhysics());