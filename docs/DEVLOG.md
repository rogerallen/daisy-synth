# Notes
* Based on https://github.com/floooh/cimgui-sokol-starterkit 
* Requires DaisySP libary path specified in CMakeLists.txt DAISYSP_DIR:
  * by default it is expected in `..`
  * `git clone https://github.com/electro-smith/DaisySP` and build 
  * On Windows DaisySP file `adsr.h` needed a `#define _USE_MATH_DEFINES` before include math.h
  * On Windows CMAKE_BUILD_TYPE will need to match the DaisySP build.
* Optional libraries in `..` if running `update_deps.sh`.  Not all repos need to do this.
  * `git clone git@github.com:floooh/sokol.git`
  * `git clone --recursive https://github.com/cimgui/cimgui.git`
# To Do
* [x] get simplest synth example done
* [ ] refactor/encapsulate keyboard
* [ ] add a "real" synth and stereo
# Log
Most recent on top.
## Friday June 23, 2023

Added waveform control.

Rename synth c++ code for clarity.

## Thursday June 22, 2023

Figured out depth issue was really a culling issue.  Fixed & cleaned up code.

Weirdness on Windows.  
On linux z ranges from -1(far) to 1(near).   white=0, black=0.5 works
On windows z ranges from -1(far) to 0(near)  white=-0.5, black=0.0 works
FIXME? adjust ortho call to match?

## Wednesday June 21, 2023

Add very basic note control and keyboard drawing.

Moved basic code into demo directory.  Different synths can be in different directories.

Added nice keyboard but can't get depth enabled.  FIXME

## Tuesday June 20, 2023

Noticed this "I'm not planning to do frequent updates to newer versions of those 
files", so let's see about running the `update_deps.sh` script.  We'll use linux for this.
```
cd ..
git clone git@github.com:floooh/sokol.git
git clone --recursive https://github.com/cimgui/cimgui.git
cd daisy-synth
./update_deps.sh
```

Updates dear imgui, v1.89.5 -> v1.89.6

Updates sokol_gfx.h

Works on Windows 11 & Linux

Wanted to add some basic graphics.  Did that with `sokol_gl.h`

## Monday June 19, 2023

Looking for a easy-to-use application framework to support a Daisy SP synth.  Found https://github.com/floooh/sokol and https://github.com/floooh/cimgui-sokol-starterkit

Works on Linux (GCC 9.3.0 x86_64), Windows 10 & Windows 11 (VS 2019 Release - amd64)

Cloned DaisySP  `git clone https://github.com/electro-smith/DaisySP` into `../DaisySP`
Did standard cmake build and got a static library in `build/libDaisySP.a`
Adjusted `CMakeLists.txt` to build.  
Needed to add a `demo.cpp` for libDaisySP since it is C++ based.

Got basic DaisySP osc working.

On Windows DaisySP file `adsr.h` needed a `#define _USE_MATH_DEFINES` before include math.h

I'm putting windows library in `DaisySP\build\RelWithDebInfo\DaisySP.lib`

But it works!