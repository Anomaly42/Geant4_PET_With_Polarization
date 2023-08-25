#ifndef CONSTRUCTION_HH
#define CONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4GenericMessenger.hh"
#include "G4Tubs.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4RotationMatrix.hh"
#include "Randomize.hh"

#include<string>

class MyDetectorConstruction : public G4VUserDetectorConstruction
{
public:
    MyDetectorConstruction();
    ~MyDetectorConstruction();

    G4LogicalVolume *GetScoringVolume() const {return fScoringVolume;}
    G4double GetSampleX() {return sampleX;}
    G4double GetSampleY() {return sampleY;}
    G4double GetSampleZ() {return sampleZ;}

    void ConstructCherenkov();
    void ConstructScintillator();
    void ConstructPET();
    
    virtual G4VPhysicalVolume *Construct();
private:
    
    virtual void ConstructSDandField();
    void DefineMaterials();

    G4int nCols, nRows;
    G4GenericMessenger *fMessenger;

    G4Box *solidWorld, *solidRadiator, *solidDetector;
    G4LogicalVolume *logicWorld, *logicRadiator, *logicDetector;
    G4VPhysicalVolume *physWorld, *physRadiator, *physDetector;

    G4Box *solidScintillator;
    G4LogicalVolume *logicScintillator;
    G4VPhysicalVolume *physScintillator;

    G4Box *solidPETBox1;
    G4LogicalVolume *logicPETBox1;
    G4VPhysicalVolume *physPETBox1;

    G4Box *solidPETBox2;
    G4LogicalVolume *logicPETBox2;
    G4VPhysicalVolume *physPETBox2;

    G4Box *solidPETSample;
    G4LogicalVolume *logicPETSample;
    G4VPhysicalVolume *physPETSample;

    G4double sampleX, sampleY, sampleZ;

    G4LogicalVolume *fScoringVolume;

    G4Material *SiO2, *H2O, *Aerogel, *worldMat, *NaI, *fLXe;
    G4Element *C, *Na, *I;

    G4double xWorld, yWorld, zWorld;
    G4bool isCherenkov, isScintillator, isPET;
    G4OpticalSurface *mirrorSurface;
};

#endif