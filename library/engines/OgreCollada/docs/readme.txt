OgreColladaLoader compilation steps

Dependencies:
Before compiling OgreColladaLoader shared library we need to deal with dependencies.
Current dependencies for COLLADA support are:
* OpenCOLLADA: https://github.com/KhronosGroup/OpenCOLLADA
    * Link to clone: git@github.com:KhronosGroup/OpenCOLLADA.git
    We are using version: v1.6.62, so please switch to this tag
* ColladaOgreImporter: https://github.com/jefftrull/ColladaOgreImporter
    * Link to clone: git@github.com:jefftrull/ColladaOgreImporter.git 
    We are using commit 55c3e5e8ecec527d02bf3412c1aa9d9bb92a5bb1 from 25 of October 2017

Patches
OpenCOLLADA - check patches folder for OpenCOLLADA-MacOS.diff patch, you donât need it if you compile it on other platforms
ColladaOgreImporter - check patches folder for ColladaOgreImporter-MacOS.diff this patch is also if you compile for other platforms

Compiling OpenCollada
First of all apply patch OpenCOLLADA-MacOS.diff if you compile it for Mac OS
Follow steps provided on the main page https://github.com/KhronosGroup/OpenCOLLADA for your platform
Verify that USE_LIBXML and USE_STATIC marked for build the the rest of options must be unchecked
Specify CMAKE_INSTALL_PREFIX with the full path were you want it to be installed after compilation usually <project_path>/build/install
Compile and install it using OS specific IDE

Compiling ColladaOgreImporter
First of all apply patch ColladaOgreImporter-MacOS.diff it also necessary for other platforms, hopefully no extra fixes needed
Set CMAKE_INSTALL_PREFIX to <project-path>/build/install
Create new BOOST_ROOT path variable and set to folder where you install Boost after its compilation process i.e. <full-path>/boost_1_67_0/stage/install
Create new COLLADA_ROOT path variable and set it to the root folder of OpenCOLLADA project (not a build folder) i.e. <full-path>/OpenCOLLADA
Create new OGRE_HOME path variable and specify it with path to Ogre3D installation folder i.e. <full-path>/ogre-1.10.11/<build-dir>/sdk
Specific for Mac OS: set CMAKE_FRAMEWORK_PATH to <full-path>/ogre-1.10.11/<build-dir>/sdk/lib/macosx/Release
By default(at least on Mac OS) Ogre3D will not install FindOGRE.cmake, so copy it from <full-path>/ogre-1.10.11/CMake/Packages into <full-path>/ogre-1.10.11/<build-dir>/sdk/CMake

Compiling OgreColladaLoader shared library (our wrapper)
Library located at <full-path>/<RadJav-root>/library/engines/OgreCollada
Run CMake
Create new BOOST_ROOT path variable and set to folder where you install Boost after its compilation process i.e. <full-path>/boost_1_67_0/stage/install
Set CMAKE_INSTALL_PREFIX to point to <project-path>/build/install
Set OGRE_HOME to point to Ogre3D installation folder i.e. <full-path>/ogre-1.10.11/<build-dir>/sdk
Mac OS specific: set OGRE_FRAMEWORK_PATH to <full-path>/ogre-1.10.11/<build-dir>/sdk/lib/macosx/Release
Set COLLADA_ROOT to point to <full-path>/OpenCOLLADA/build/install
Set OGRECOLLADA_LIBRARIES_PATH to point to ColladaOgreImporter library build folder i.e. <full-path>/ColladaOgreImporter/build/Debug
Set OGRECOLLADA_HEADERS_PATH to point to ColladaOgreImporter library project folder i.e. <full-path>/ColladaOgreImporter
