#include "construction.hh"
#include "detector.hh"
#include<math.h>

MyDetectorConstruction::MyDetectorConstruction(){

    fMessenger = new G4GenericMessenger(this, "/detector/", "Detector Construction");

    fMessenger->DeclareProperty("nCols", nCols, "Number of columns in the detector");
    fMessenger->DeclareProperty("nRows", nRows, "Number of rows in the detector");
    // fMessenger->DeclareProperty("isCherenkov", isCherenkov, "Toggle Cherenkov setup");
    // fMessenger->DeclareProperty("isScintillator", isScintillator, "Toggle Scintillator setup");

    nCols = 100;
    nRows = 100;

    xWorld = 0.5*m;
    yWorld = 0.5*m;
    zWorld = 0.5*m;

    isCherenkov = false;
    isScintillator = false;
    isPET = true;

    //Sample Volume for PET
    sampleX = 10*mm;
    sampleY = 10*mm;
    sampleZ = 10*mm;

    DefineMaterials();
}
MyDetectorConstruction::~MyDetectorConstruction(){}

void MyDetectorConstruction::DefineMaterials(){
    G4NistManager *nist = G4NistManager::Instance();
    worldMat = nist->FindOrBuildMaterial("G4_AIR");

    //World Material Properties
    G4double energyWorld[2] = {1.239841939*eV/0.9, 1.239841939*eV/0.2};
    G4double rindexWorld[2] = {1.0, 1.0};
    G4MaterialPropertiesTable *mptWorld = new G4MaterialPropertiesTable();
    mptWorld->AddProperty("RINDEX", energyWorld, rindexWorld, 2);

    worldMat->SetMaterialPropertiesTable(mptWorld);

    SiO2 = new G4Material("SiO2", 2.201*g/cm3, 2);
    SiO2->AddElement(nist->FindOrBuildElement("Si"), 1);
    SiO2->AddElement(nist->FindOrBuildElement("O"), 2);

    H2O = new G4Material("H2O", 1.000*g/cm3, 2);
    H2O->AddElement(nist->FindOrBuildElement("H"), 2);
    H2O->AddElement(nist->FindOrBuildElement("O"), 1);

    C = nist->FindOrBuildElement("C");

    //Aerogel
    Aerogel = new G4Material("Aerogel", 0.200*g/cm3, 3);
    Aerogel->AddMaterial(SiO2, 62.5*perCent);
    Aerogel->AddMaterial(H2O, 37.4*perCent);
    Aerogel->AddElement(C, 0.1*perCent);

    //Material Properties of Aerogel
    G4double energy[2] = {1.239841939*eV/0.9, 1.239841939*eV/0.2};
    G4double rindexAerogel[2] = {1.1, 1.1};

    G4MaterialPropertiesTable *mptAerogel = new G4MaterialPropertiesTable();
    mptAerogel->AddProperty("RINDEX", energy, rindexAerogel, 2); //DOUBT: momentum and energy of massless particles have a factor of c. where is it accounted for?

    Aerogel->SetMaterialPropertiesTable(mptAerogel);

    //NaI
    Na = nist->FindOrBuildElement("Na");
    I = nist->FindOrBuildElement("I");
    NaI = new G4Material("NaI", 3.67*g/cm3, 2);
    NaI->AddElement(Na, 1);
    NaI->AddElement(I, 1);

    G4MaterialPropertiesTable *mptNaI = new G4MaterialPropertiesTable();
    G4double energyNaI[2] = {1.239841939*eV/0.9, 1.239841939*eV/0.2};
    G4double rindexNaI[2] = {1.78, 1.78};
    G4double fraction[2] = {1.0, 1.0};
    // mptNaI->AddProperty("RINDEX", energyNaI, rindexNaI, 2);
    // mptNaI->AddProperty("SCINTILLATIONCOMPONENT1", energyNaI, fraction, 2);
    // mptNaI->AddConstProperty("SCINTILLATIONYIELD1", 38000./MeV); 
    // mptNaI->AddConstProperty("RESOLUTIONSCALE", 1.0);
    // mptNaI->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 0.25*ns);
    // mptNaI->AddConstProperty("ELECTRONSCINTILLATIONYIELD1", 1.0);
    // mptNaI->AddConstProperty("PROTONSCINTILLATIONYIELD1", 1.0);

    // NaI->SetMaterialPropertiesTable(mptNaI);

    //Liquid Xenon
    fLXe = new G4Material("LXe", 3.020*g/cm3, 1);
    fLXe->AddElement(nist->FindOrBuildElement("Xe"), 1);

    std::vector<G4double> lxe_Energy = {7.0*eV, 7.07*eV, 7.14*eV};

    std::vector<G4double> lxe_SCINT = {0.1, 1.0, 0.1};
    std::vector<G4double> lxe_RIND  = {1.59, 1.57, 1.54};
    std::vector<G4double> lxe_ABSL  = {35.*cm, 35.*cm, 35.*cm};
    G4MaterialPropertiesTable *fLXe_mt = new G4MaterialPropertiesTable();
    fLXe_mt->AddProperty("SCINTILLATIONCOMPONENT1", lxe_Energy, lxe_SCINT);
    fLXe_mt->AddProperty("SCINTILLATIONCOMPONENT2", lxe_Energy, lxe_SCINT);
    fLXe_mt->AddProperty("RINDEX",        lxe_Energy, lxe_RIND);
    fLXe_mt->AddProperty("ABSLENGTH",     lxe_Energy, lxe_ABSL);
    fLXe_mt->AddConstProperty("SCINTILLATIONYIELD", 12000./MeV);
    fLXe_mt->AddConstProperty("RESOLUTIONSCALE", 1.0);
    fLXe_mt->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 20.*ns);
    fLXe_mt->AddConstProperty("SCINTILLATIONTIMECONSTANT2", 45.*ns);
    fLXe_mt->AddConstProperty("SCINTILLATIONYIELD1", 1.0);
    fLXe_mt->AddConstProperty("SCINTILLATIONYIELD2", 0.0);
    fLXe->SetMaterialPropertiesTable(fLXe_mt);

    // Set the Birks Constant for the LXe scintillator
    fLXe->GetIonisation()->SetBirksConstant(0.126*mm/MeV);

    G4double reflectivity[2] = {1.0, 1.0};
    mirrorSurface = new G4OpticalSurface("mirrorSurface");

    mirrorSurface->SetType(dielectric_metal);
    mirrorSurface->SetFinish(ground);
    mirrorSurface->SetModel(unified);

    G4MaterialPropertiesTable *mptMirror = new G4MaterialPropertiesTable();
    mptMirror->AddProperty("REFLECTIVITY", energy, reflectivity, 2);

    mirrorSurface->SetMaterialPropertiesTable(mptMirror);

}

void MyDetectorConstruction::ConstructCherenkov(){
    //Radiator
    solidRadiator = new G4Box("solidRadiator", 0.4*m, 0.4*m, 0.01*m);
    logicRadiator = new G4LogicalVolume(solidRadiator, Aerogel, "logicRadiator");
    physRadiator = new G4PVPlacement(0,
    G4ThreeVector(0., 0., 0.25*m), logicRadiator, "physRadiator", logicWorld, false, 0, true);

    //Detector Array
    solidDetector = new G4Box("solidDetector", xWorld/nRows, yWorld/nCols, 0.01*m);
    logicDetector = new G4LogicalVolume(solidDetector, worldMat, "logicDetector");

    //Scoring Volume
    fScoringVolume = logicRadiator;

    for (G4int i = 0; i < nRows; ++i){
        for (G4int j = 0; j < nCols; ++j){
            physDetector = new G4PVPlacement(0,
            G4ThreeVector(-0.5*m + (i + 0.5)*m/nRows, -0.5*m + (j + 0.5)*m/nCols, 0.49*m),
            logicDetector, "physDetector", logicWorld, false, i + j*nCols, true);
        }
    }
}

void MyDetectorConstruction::ConstructScintillator(){
    solidScintillator = new G4Box("solidScintillator", 5*cm, 5*cm, 6*cm);
    logicScintillator = new G4LogicalVolume(solidScintillator, fLXe, "logicScintillator"); //scintillator material

    G4LogicalSkinSurface *skin = new G4LogicalSkinSurface("skin", logicWorld, mirrorSurface);

    solidDetector = new G4Box("solidDetector", 1*cm, 5*cm,6*cm);
    logicDetector = new G4LogicalVolume(solidDetector, worldMat, "logicDetector");

    fScoringVolume = logicScintillator;

    for (G4int i = 0; i < 6; ++i){
        for (G4int j = 0; j < 16; ++j){
            //building the PET detectors
            G4Rotate3D rotz = G4Rotate3D(j*22.5*deg, G4ThreeVector(0., 0., 1.));
            G4Translate3D transXScint(G4ThreeVector(5./tan(22.5/2*deg)*cm + 5.*cm, 0.*cm, -40*cm + i*15*cm));
            G4Translate3D transXDet(G4ThreeVector(5./tan(22.5/2*deg)*cm + 6.*cm + 5.*cm, 0.*cm, -40*cm + i*15*cm));

            G4Transform3D transformScint = (rotz)*(transXScint);
            G4Transform3D transformDet = (rotz)*(transXDet);

            physScintillator = new G4PVPlacement(transformScint, logicScintillator, "physScintillator", logicWorld, false, 0, true);
            physDetector = new G4PVPlacement(transformDet, logicDetector, "physDetector", logicWorld, false, 0, true);
        }
    }

}

void MyDetectorConstruction::ConstructPET(){
    G4double radiusPET = 182.;
    
    solidPETBox1 = new G4Box("solidPETBox1", 10*mm, 17.75*mm, 17.75*mm);
    logicPETBox1 = new G4LogicalVolume(solidPETBox1, worldMat, "logicPETBox1");

    solidPETBox2 = new G4Box("solidPETBox2", 10*mm, 2*mm, 2*mm);
    logicPETBox2 = new G4LogicalVolume(solidPETBox2, worldMat, "logicPETBox2");
    // physPETBox2 = new G4PVPlacement(0, G4ThreeVector(0,0,0), logicPETBox2, "physPETBox2", logicPETBox1, false, 0, true);

    G4int repeatX = 30, repeatY = 8, repeatZ = 8;
    for (G4int j = -(G4int)(repeatY/2); j < (G4int)(repeatY/2); ++j){
        for (G4int k = - (G4int)(repeatZ/2); k < (G4int)(repeatZ/2); ++k){
            physPETBox2 = new G4PVPlacement(0, G4ThreeVector(0, 2.25 +  j*4.5*mm, 2.25 + k*4.5*mm), logicPETBox2, "physPETBox" + to_string(repeatY*j + k), logicPETBox1, false, 0, true);
        }
    }

    for (G4int i = 0; i < repeatX; ++i){
        double angle_rad = i*(360.0/(double)repeatX)*3.1415926535/180.0;
        
        G4RotationMatrix *detectorRot = new G4RotationMatrix();
        detectorRot->rotateZ(angle_rad*rad);

        physPETBox1 = new G4PVPlacement(detectorRot, G4ThreeVector(radiusPET*mm*cos(-angle_rad), radiusPET*mm*sin(-angle_rad), 0), logicPETBox1, "physPETBox1", logicWorld, false, 0, true);
    }

    solidPETSample = new G4Box("solidPETSample", sampleX, sampleY, sampleZ);
    logicPETSample = new G4LogicalVolume(solidPETSample, H2O, "logicPETSample");
    physPETSample = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), logicPETSample, "physPETSample", logicWorld, false, 0, true);

}

G4VPhysicalVolume *MyDetectorConstruction::Construct(){
    //World
    solidWorld = new G4Box("solidWorld", xWorld, yWorld, zWorld);
    logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld"); 
    physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, true);

    if (isCherenkov){
        ConstructCherenkov();
    }
    if (isScintillator){
        ConstructScintillator();
    }
    if (isPET){
        ConstructPET();
    }

    return physWorld;
}

void MyDetectorConstruction::ConstructSDandField(){
    MySensitiveDetector *sensDet = new MySensitiveDetector("sensDet");

    if (isCherenkov){
        logicRadiator->SetSensitiveDetector(sensDet);
    }

    if (isPET){
        logicPETBox2->SetSensitiveDetector(sensDet);
    }
}