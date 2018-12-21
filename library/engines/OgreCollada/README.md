# OgreColladaLoader

Simple shared library to load [\.dae](https://en.wikipedia.org/wiki/COLLADA) files into [Ogre3D](https://www.ogre3d.org) engine's scene.

## Getting Started

The library was created to minimize complexity of projects build files.

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. See deployment for notes on how include this library into your project.

### Prerequisites

The main build tool we depends on is [CMake](https://cmake.org), the version we are currently using is 3.11, so please make sure that you have it. Also to make a copy of dependency sources we advise you to use [Git](https://git-scm.com).

Library has several dependencies which needs to be build first:
- [OpenCOLLADA](https://github.com/KhronosGroup/OpenCOLLADA) - a stream based reader and writer library for COLLADA(.dae) files.
- [ColladaOgreImporter](https://github.com/jefftrull/ColladaOgreImporter) - a small library for importing COLLADA data into Ogre3D engine. 

Also there are several fixes we made for these dependencies, but we explain that later on.

### Installing dependencies

#### OpenCOLLADA

To build that set of libraries we need to clone them from the official repository using the following command:

```
git clone git@github.com:KhronosGroup/OpenCOLLADA.git
```

The version we are currently using is `v1.6.62`. To check it out please use the following command:

```
git checkout v1.6.62
```

If you are building for macOS X then you need to apply patch [OpenCOLLADA-MacOS.diff](./docs/patches/OpenCOLLADA-MacOS.diff) to the main folder of OpenCollada project, that is needed to fix compilation issues for that platform.

To finally build it follow steps on the main page of [OpenCOLLADA](https://github.com/KhronosGroup/OpenCOLLADA) for your platform.

Be sure that `USE_LIBXML` and `USE_STATIC` were set to true(marked), the rest of options must be unchecked. Also specify `CMAKE_INSTALL_PREFIX` with the full path were you want it to be installed after successfull compilation, usually it is `<path-to-OpenCOLLADA>/build/install`. 
Compile and install it using OS specific IDE.

```
Currently we verified it on macOS X, but it should work on other operating systems as well
```

#### ColladaOgreImporter

To build that library we need to clone it from the official repository using the following command:

```
git clone git@github.com:jefftrull/ColladaOgreImporter.git
```

The version we are currently using is a commit with SHA: `55c3e5e8ecec527d02bf3412c1aa9d9bb92a5bb1` from October 25 2017. So we need to checkout this commit using:

```
git checkout 55c3e5e8ecec527d02bf3412c1aa9d9bb92a5bb1
```

We have a patch for this library as well which tune Ogre3D version, fix material issues and unique_ptr usage. So please apply patch [ColladaOgreImporter-MacOS.diff](./docs/patches/ColladaOgreImporter-MacOS.diff) to the root ColladaOgreImporter project folder.

During library configuration we need to specify several variables in CMake utility:
- CMAKE_INSTALL_PREFIX needs to be set to a full path were we need that library to be installed, usually this is `<path-to-OgreColladaImporter>/build/install`.

- BOOST_ROOT path variable must be created and point to path where compiled Boost reside (with include and libs folders), usually it is `<absolute-path>/boost_1_68_0/stage/install`. For more information how to compile Boost libraries please see [this document]()**(Need Boost steps here)**.

- COLLADA_ROOT path variable must be created and point to the root folder of OpenCOLLADA project (not a build folder), usually it is `<absolute-path>/OpenCOLLADA`.

- OGRE_HOME path variable must be created and point to Ogre3D installation folder, usually it is `<absolute-path>/ogre-1.10.11/build/sdk`.

Specifically for macOS X:

- CMAKE_FRAMEWORK_PATH path variable must point to Ogre3D Frameworks installation path, usually it is `<absolute-path>/ogre-1.10.11/build/sdk/lib/macosx/Release`.

- By default Ogre3D will not install FindOGRE.cmake, so copy it from `<absolute-path>/ogre-1.10.11/CMake/Packages` into `<absolute-path>/ogre-1.10.11/build/sdk/CMake`.

For more information on how to build Ogre3D please follow [this document]()**(Need Ogre3D steps here)**.

Compile library using OS specific IDE.

### Building OgreColladaLoader

First of all we need to configure build using CMake utility.
Here is a list of variables which needs to be set:

- CMAKE_INSTALL_PREFIX needs to be set to a full path were we need that library to be installed, usually this is `<path-to-OgreCollada>/build/install`.

- BOOST_ROOT path variable must point to path where compiled Boost reside (with include and libs folders), usually it is `<absolute-path>/boost_1_68_0/stage/install`. For more information on how to compile Boost libraries please see [this document]()**(Need Boost steps here)**.

- OGRE_HOME path variable must point to Ogre3D installation folder, usually it is `<absolute-path>/ogre-1.10.11/build/sdk`.

- COLLADA_ROOT path variable must point to the **installation** folder of OpenCOLLADA project, usually it is `<absolute-path>/OpenCOLLADA/build/install`.

- OGRECOLLADA_LIBRARIES_PATH path variable must point to ColladaOgreImporter library **build** folder, usually it is `<absolute-path>/ColladaOgreImporter/build/Debug`.

- OGRECOLLADA_HEADERS_PATH path variable must point to ColladaOgreImporter library **project** folder i.e. `<absolute-path>/ColladaOgreImporter`

Specifically for macOS X:

- CMAKE_FRAMEWORK_PATH path variable must point to Ogre3D Frameworks installation path, usually it is `<absolute-path>/ogre-1.10.11/build/sdk/lib/macosx/Release`.

Compile and install library using OS specific IDE.

## Include in your project

To be able to use OgreColladaLoader in your project you need to add it to your CMakeLists.txt file as follows
```
include (FindColladaLoader)
```

FindColladaLoader cmake script can be found [here](../../CMake/lib/FindColladaLoader.cmake).

To help CMake find library you can specify `COLLADALOADER_HOME` environment variable or `COLLADA_SOURCE` path variable with link to library install directory. When FindColladaLoader script successfully found the library it will set `COLLADA_INCLUDE` and `COLLADA_LIBRARIES` variables accordingly which in turn can be used in `target_link_libraries` and `include_directories` CMake routines.

## Authors

* **Mikalai Turankou** - *Initial work* - [jefftrull](https://github.com/jefftrull)

See also the list of [contributors](https://github.com/FogChainInc/RadJavPrivate/graphs/contributors) who participated in this project.

## License

This project is licensed under the MIT License - see the [LICENSE](../../../LICENSE) file for details.

## Acknowledgments

Many thanks for a hard work of guys worked on:

- [Boost](https://www.boost.org)
- [Ogre3D](https://www.ogre3d.org)
- [OpenCOLLADA](https://github.com/KhronosGroup/OpenCOLLADA)
- [ColladaOgreImporter](https://github.com/jefftrull/ColladaOgreImporter)

