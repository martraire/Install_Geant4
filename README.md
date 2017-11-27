# How to install Geant4 on Ubuntu 16.04

Geant4 is a toolkit for the simulation of the passage of particles through matter. Its areas of application include high energy, nuclear and accelerator physics, as well as studies in medical and space science.

This note is a step by step instructions to install GEANT4 and its dependencies (Qt5 as the visualization driver, cmake, ..) in a newly-install 32-bit Ubuntu 16.04. 

For more information, the GEANT4 official website is at https://geant4.web.cern.ch/geant4/. The Geant4 installation guide is very helpful, especially if you are using a different operating system: http://geant4.web.cern.ch/geant4/UserDocumentation/UsersGuides/InstallationGuide/html/ch02.html 



## 1. Before installing Geant4, some tools are needed

Update & Install essential: `sudo apt-get install build-essential`


### A - Install CMAKE

**cmake** is not only needed to install Geant4, it is also very convenient to compile the examples (B1, B2, ... etc). 

- Download cmake at: https://cmake.org/download/
- Installation guide: https://cmake.org/install/  OR  `sudo apt-get install cmake`

N.B: To check if cmake is *already* installed and to know its version: `cmake --version`


### B - Install CCMAKE

**ccmake** is a useful tool to configure the build options. When we use cmake to configure the building script, we type in cmake `-DGEANT4_VAR="xxx"`, i.e use `-D` to define options. It is not really convenient to have many options to enter. This is where ccmake comes in, it helps us to control these build options.

To install ccmake: `sudo apt install cmake-curses-gui`

N.B: To check if cmake is *already* installed and to know its version: `ccmake --version`
