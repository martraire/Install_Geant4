# How to install, build and run Geant4 on Ubuntu 16.04

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



## 2. Geant4 installation

The whole installation of Geant4 will be in the directory **/geant4**:
```
mkdir geant4
cd geant4
```

### A - CLHEP Library 
CLHEP is a Class Library for High Energy Physics providing additional classes such as random generators, physics vectors, geometry and linear algebra. 

- Download CLHEP source file: http://proj-clhep.web.cern.ch/proj-clhep/clhep23.html
- In this example, I choose the version 2.3.4.5.tgz
- Copy the file in **/geant4**
- Uncompress the source: `tar -xzvf clhep-2.3.4.5.tgz`
- Install & compile in **/build**:
```
mkdir build
cd build
cmake ../2.3.4.5/CLHEP
make
sudo make all install
```


### B - Install & compile Geant4 source

- Download Geant4 source file: http://geant4.web.cern.ch/geant4/support/download.shtml
- In this example, I choose the GNU or Linux tar format, version **geant4.10.02.p01.gzip**

N.B: As instruction mentioned, I also recommend to download the data files during the installation process, since many built-in examples need these data files. Hence you don't need to manually download data files unzip them, recompile the sources.
- Uncompress the source: `tar -xzvf geant4.10.02.p03.tar.gz`
- Install & compile in **/geant4.10.02.p03-build**:
```
mkdir geant4.10.02.p03-build
cd geant4.10.02.p03-build
cmake ../geant4.10.02.p03
cmake -DCMAKE_INSTALL_PREFIX=/home/martraire/Soft/geant4/geant4.10.02.p03-install -DGEANT4_INSTALL_DATA=ON -DGEANT4_USE_OPENGL_X11=ON -DGEANT4_USE_QT=ON -DGEANT4_INSTALL_EXAMPLES=ON -DBUILD_SHARED_LIBS=ON -DGEANT4_BUILD_MULTITHREADED=ON /home/martraire/Soft/geant4/geant4.10.02.p03
make -j4    (4 is for 4 cores, cf. details below)
make install
```
N.B: Change value by number of cores. Here *-j**4***, 4 is for 4 cores. To know how many cores your CPU has: `cat /proc/cpuinfo | grep processor | wc -l`

*Additional information about cmake:*

If you don't use cmake before, it is important to understand the basic cmake installation concepts of source, build, and install directories.
1. **source directory** "geant4.10.02.p03"is a folder to store source file (.h .cpp etc...). You download the Geant4 source files from the Internet. You unzip the files and get a folder "geant4.10.02.p01". That is your source folder.
2. **build directory** "geant4.10.02.p03-build" is a folder to store cmake scripts and the build scripts generated from cmake programs. In the Unix system, like Ubuntu, cmake program will check your compilers and create the correct Unix Makefile for you.
3. **install directory** "geant4.10.02.p03-install" stores the final products -- like share libraries or header files which are created according to the build scripts from cmake that configured by you. If you think you will not reconfigure the Geant4 anymore, you can remove "source" and "build" folder. 


###  C - Setup the environment and activate Geant4

The last part is to activate the Geant4 environment:
`source ~/Soft/geant4/geant4.10.02.p03-install/bin/geant4.sh`

In order to avoid using this command each time we need Geant4, you can add it in your **.bashrc** :
`source ~/Soft/geant4/geant4.10.02.p03-install/bin/geant4.sh`



## 3. Run a Geant4 example

If you didn't activate the Geant4 environment, please refer to the previous step **2.C** and run the *geant.sh* script.

1. Copy an example from  **~/Soft/geant4/geant4.10.02.p03-install/share/Geant4-10.2.3/examples/**. There are different level of complexity. For this example, let's start with a basic one, the B1 in the directory **/basic** and copy it in your **/Work** directory:

`cp -r ~/Soft/geant4/geant4.10.02.p03-install/share/Geant4-10.2.3/examples/basic/B1 ~/Work/.`

2. Compile and build the executable of B1 example:
```
cd Work
mkdir B1-build
cd B1-build
cmake -DGeant4_DIR=~/Soft/geant4/geant4.10.02.p03-install/lib/Geant4-10.2.3/ ~/Work/Geant4/example/B1
cmake -DCMAKE_PREFIX_PATH=~/Soft/geant4/geant4.10.02.p03-install ~Work/Geant4/example/B1
make -j4   (once again 4 is for 4 cores, change with the config of your machine)
make
```
Now, you should see the executable *exampleB1*.

3. Execute the example: `./exampleB1`
