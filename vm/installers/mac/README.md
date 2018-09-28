Generating DMG for Mac
1. set CMAKE_INSTALL_PREFIX to /Users/mikalaiturankou/Work/Compilation/RadJavPrivate/vm/build-xcode/install
2. enable RadJavVM_CREATE_INSTALL_PACKAGES
3. set RadJavVM_PACKAGER_TYPE to DragNDrop
4. set MAC_CODESIGN_IDENTITY to Mac Developer (can be determined by "security find-identity -v -p codesigning")
5. hit configure
6. hit generate
7. build project
8. switch build scheme to install
9. build project
you will get errors
copy Ogre frameworks into /RadJavPrivate/vm/build-xcode/Debug/RadJavVMLauncher.app/Contents/Frameworks
copy Ogre plugins into /RadJavPrivate/vm/build-xcode/Debug/RadJavVMLauncher.app/Contents/Frameworks
for Mac OS here is the list of plugins and frameworks used:
Ogre.framework
OgreBites.framework
OgreHLMS.framework
OgreMeshLodGenerator.framework
OgreOverlay.framework
OgrePaging.framework
OgreProperty.framework
OgreRTShaderSystem.framework
OgreTerrain.framework
OgreVolume.framework
Plugin_BSPSceneManager.framework
Plugin_OctreeSceneManager.framework
Plugin_ParticleFX.framework
RenderSystem_GL.framework
10. build project again
11. select scheme to package
12. build project
In build folder there will be new file called RadJavVMLauncher-0.1.dmg which can be installed on the system
Notes:
Configuration files for Ogre usually located in: /Users/<user>/Library/Preferences/RadJav/3D Engine were we can tweak plugins.cfg and ogre.cfg files be sure that you will create them before running any .xrj 3D programs(files). Depending on build type files can be postfixed with _d(for debug build), example plugins_d.cfg
Examples of recent files for MacOS:
ogre.cfg:
Render System=OpenGL Rendering Subsystem
[OpenGL Rendering Subsystem]
Colour Depth=32
Content Scaling Factor=1.0
FSAA=0
Full Screen=No
RTT Preferred Mode=FBO
Video Mode=1024 x 768
hidden=No
macAPI=cocoa
sRGB Gamma Conversion=No
vsync=Yes
plugins.cfg:
# Defines plugins to load
# Define plugin folder
PluginFolder=
# Define plugins
# Plugin=RenderSystem_Direct3D9
# Plugin=RenderSystem_Direct3D11
 Plugin=RenderSystem_GL
# Plugin=RenderSystem_GL3Plus
# Plugin=RenderSystem_GLES
# Plugin=RenderSystem_GLES2
 Plugin=Plugin_ParticleFX
 Plugin=Plugin_BSPSceneManager
# Plugin=Plugin_CgProgramManager
# Plugin=Plugin_EXRCodec
# Plugin=Plugin_PCZSceneManager
# Plugin=Plugin_OctreeZone
 Plugin=Plugin_OctreeSceneManager