# Geant4_PET_With_Polarization

## Installation
1. Install GEANT4 first (works with version 11.2). Open a terminal window, locate and run the script geant4make.sh in your GEANT4 installation.
2. Navigate to the directory containing the files in this repo, make a directory named "build" and navigate into it
3. Run `cmake ..`. If it does not work, run `ccmake ..` and set the missing paths to directories in the Geant4 Installation
4. Run `make` and `./sim`. You should see a GUI if the installation worked correctly.

## Description
Contains GEANT4 code to perform PET simulations while also taking into consideration the fact that the annihilation photons during electron-positron decay are also correlated in terms of polarization. The project structure is based upon Dr. Mustafa Schmidt's excellent set of tutorials on GEANT4: https://www.youtube.com/playlist?list=PLLybgCU6QCGWgzNYOV0SKen9vqg4KXeVL

## Notes on Geant4
Here are some notes on a minimal setup in GEANT4 to get a basic grasp of its concepts.
https://nickel-badger-e1b.notion.site/Base-Project-No-Detector-04c14d013dcd4894825664c54ccf32c1
