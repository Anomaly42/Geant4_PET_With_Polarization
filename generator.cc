#include "generator.hh"
#include "construction.hh" //for sample dimensions

MyPrimaryGenerator::MyPrimaryGenerator(){
    particleNumber = 1;
    fParticleGun = new G4ParticleGun(particleNumber); //two particles per event

    G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
    G4String particleName="geantino"; //default particle, previously a proton
    G4ParticleDefinition *particle = particleTable->FindParticle(particleName);

    //set particle position and momentum
    G4ThreeVector pos(0.*m, 0.*m, 0.*m);
    G4ThreeVector mom(0., 0., 1.);

    fParticleGun->SetParticlePosition(pos);
    fParticleGun->SetParticleMomentumDirection(mom);
    fParticleGun->SetParticleMomentum(0.*GeV); 
    fParticleGun->SetParticleDefinition(particle);


}

MyPrimaryGenerator::~MyPrimaryGenerator(){
    delete fParticleGun;
}

void MyPrimaryGenerator::GeneratePrimaries(G4Event* anEvent){
    G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();

    G4ParticleDefinition *particle = fParticleGun->GetParticleDefinition();

    if (particle == G4Geantino::Geantino()){
        G4double energy   = 0.511*GeV;
        G4ParticleDefinition* particle = particleTable->FindParticle("gamma");

        for (G4int i=0; i<particleNumber; i++){
            fParticleGun->SetParticleDefinition(particle);
            fParticleGun->SetParticleEnergy(energy);
        }
    }
    //Bad way to implement
    //Duplicated data
    //TODO: Fix later
    //MyDetectorConstruction *construct = new MyDetectorConstruction();
    G4double sampleX = 10*mm;
    G4double sampleY = 10*mm;
    G4double sampleZ = 5*mm;

    //Random particle time of decay using uniform random
    G4double particleTime = G4UniformRand()*s; //Acquisition time = 1 second
    fParticleGun->SetParticleTime(particleTime);

    //Randomize particle position
    G4double particleX, particleY, particleZ;
    particleX = sampleX*(G4UniformRand()-0.5);
    particleY = sampleY*(G4UniformRand()-0.5);
    particleZ = sampleZ*(G4UniformRand()-0.5);

    fParticleGun->SetParticlePosition(G4ThreeVector(particleX, particleY, particleZ));

    //Randomize particle momentum direction
    G4double theta = 2*CLHEP::pi*G4UniformRand();
    G4double phi = acos(1-2*G4UniformRand());
    G4double px, py, pz;
    px = sin(phi)*cos(theta);
    py = sin(phi)*sin(theta);
    pz = cos(phi);

    G4ThreeVector momentumDirection(px, py, pz);
    //Create a vector that is perpendicular to momentum direction
    G4ThreeVector perpVector = momentumDirection.cross(G4ThreeVector(0, 0, 1));
    //Rotate this perpendicular vector by a random angle around momentum direction
    G4double rotationAngle = 2*CLHEP::pi*G4UniformRand();
    perpVector.rotate(rotationAngle, momentumDirection);

    G4ThreeVector polarizationDirection = perpVector.unit();
    // G4ThreeVector orthogonalDirection = polarizationDirection.rotate(CLHEP::pi/2.0, momentumDirection);

    fParticleGun->SetParticleMomentumDirection(momentumDirection);
    fParticleGun->SetParticlePolarization(polarizationDirection);

    fParticleGun->GeneratePrimaryVertex(anEvent);

    fParticleGun->SetParticleMomentumDirection(-momentumDirection);
    fParticleGun->SetParticlePolarization(polarizationDirection.rotate(CLHEP::pi/2.0, momentumDirection));

    fParticleGun->GeneratePrimaryVertex(anEvent);

}