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

N.B: To check if ccmake is *already* installed and to know its version: `ccmake --version`


### C - Install OpenGL:
OpenGL is useful for the visualisation:
```
sudo apt-get install libxaw7-dev libxaw7`

sudo apt-get install freeglut3 freeglut3-dev mesa-utils mesa-common-dev
sudo apt-get install libglu1-mesa-dev -y 
sudo apt-get install libxerces-c-dev
```

### D - Install X11 
`sudo apt-get install libxaw7-dev libxaw7`

There is a second step with the Qt installation, see below the **section E**.

N.B: To check if X11 is *already* installed and to know its version: `xdpyinfo|grep -i version`


### E - Install Qt:
The Qt is not necessary for the Geant4 installation, but it is a very good visualization driver. Many Geant4 users love it. Ask also for Qt4 not only Qt5.

- Download X11 at: http://doc.qt.io/qt-4.8/install-x11.html
- Installation guide: http://doc.qt.io/qt-4.8/requirements-x11.html
- Additionnal info to install Qt5: https://wiki.qt.io/Install_Qt_5_on_Ubuntu

N.B: For Qt5 or latter it is much easier to use online installer, for which you just need to do `chmod +x qt-xxx.run; ./qt-xxx.run`, and then it pops up a gui to help you install Qt5.

`sudo apt-get install qt-sdk`

```
wget http://download.qt.io/official_releases/qt/5.7/5.7.0/qt-opensource-linux-x64-5.7.0.run
chmod +x qt-opensource-linux-x64-5.7.0.run
./qt-opensource-linux-x64-5.7.0.run
```

Installation of additional librairies:

`sudo apt-get install libfontconfig1-dev libfreetype6-dev libx11-dev libxcursor-dev libxext-dev libxfixes-dev libxft-dev libxi-dev libxrandr-dev libxrender-dev`

### F - Install Expat
Expat is a XML parsing C library. Geant4 default installation will search for Expat.

`sudo apt-get install expat`

N.B: To check if Qt is *already* installed and to know its version: `sudo apt-get install qt-sdk`
