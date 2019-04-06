# Building the installers for Linux
Open CMake

Check RadJavVM_CREATE_INSTALL_PACKAGES.

Make sure CPACK_BINARY_DEB is checked.
Make sure CPACK_BINARY_RPM is checked.

Navigate to your build directory and enter:

    make -j4
    make package