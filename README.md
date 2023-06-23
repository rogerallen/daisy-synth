# Daisy Synth

Simulate Daisy SP synth on Win/Mac/Linux computers to develop & check sound before putting it on Daisy.

Sokol header files are nearly ideal for ease of use & cross-platform control.

Work In Progress.  See `docs/DEVLOG.md` for progress.  Tested on Windows 10, 11 and Linux via VS Code.  Emscripten is not tested.

# Building/Usage

* Based on https://github.com/floooh/cimgui-sokol-starterkit (MIT License (c) Andre Weissflog)
* Requires DaisySP library at path specified in CMakeLists.txt DAISYSP_DIR:
  * by default it is expected in `..`
  * `git clone https://github.com/electro-smith/DaisySP` and build 
  * On Windows DaisySP file `adsr.h`, I needed a `#define _USE_MATH_DEFINES` before `#include <math.h>`
  * On Windows use RelWithDebInfo target. (FIXME--see CMakeLists.txt)
* Optional libraries in `..` if you want to run `update_deps.sh` to keep things up-to-date.  Not all repos need to do this.  I'll just do this on Linux.
  * `git clone git@github.com:floooh/sokol.git`
  * `git clone --recursive https://github.com/cimgui/cimgui.git`

# Demo

Very Simple Synth with some GUI controls. Run `build\demo`, keyboard piano turns notes on/off, ESC to quit.

# License

Code is MIT licensed, just as the Sokol code is.