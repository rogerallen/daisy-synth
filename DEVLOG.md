# Notes
* Based on https://github.com/floooh/cimgui-sokol-starterkit
# Log
Most recent on top.
## Monday June 19, 2023
Looking for a easy-to-use application framework to support a Daisy SP synth.  Found https://github.com/floooh/sokol and https://github.com/floooh/cimgui-sokol-starterkit

Works on Linux (GCC 9.3.0 x86_64), Windows 10 & Windows 11 (VS 2019 Release - amd64)

Cloned DaisySP  git clone https://github.com/electro-smith/DaisySP into ../DaisySP
Did standard cmake build and got a static library in build/libDaisySP.a
Adjusted CMakeLists.txt to build.  
Needed to add a demo.cpp for libDaisySP since it is C++ based.

Got basic DaisySP osc working.

On Windows DaisySP file adsr.h needed a _USE_MATH_DEFINES before include math.h

I'm putting library in DaisySP\build\RelWithDebInfo\DaisySP.lib

But it works!