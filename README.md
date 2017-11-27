# How to install Geant4 on Ubuntu 16.04

Geant4 is a toolkit for the simulation of the passage of particles through matter. Its areas of application include high energy, nuclear and accelerator physics, as well as studies in medical and space science. 
The GEANT4 official website is at https://geant4.web.cern.ch/geant4/. 
This note is a step by step instructions to install GEANT4 and its dependencies (Qt5 as the visualization driver, cmake, ..) in a newly-install 32-bit Ubuntu 16.04. 
The Geant4 installation guide is very helpful, if you are using a different operating system: http://geant4.web.cern.ch/geant4/UserDocumentation/UsersGuides/InstallationGuide/html/ch02.html 


## 1. Before installing Geant4, some tools are needed

Update & Install essential: `sudo apt-get install build-essential`

### A - Install CMAKE

- Download cmake at: https://cmake.org/download/
- Installation guide: https://cmake.org/install/  OR  `sudo apt-get install cmake`

N.B: To check if cmake is (already) installed and to know its version: `cmake --version`

