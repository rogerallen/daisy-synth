# Daisy Synth

Simulate Daisy SP synth on Win/Mac/Linux computers to develop & check sound before putting it on Daisy.  Project is meant for Daisy synth developers.

Sokol header files are nearly ideal for ease of use & cross-platform control.

Work In Progress.  See `docs/DEVLOG.md` for progress.  Tested on Windows 10, 11 and Linux via VS Code.  Emscripten is not tested, not expected to work.

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

```
 ____ ____ ____ ____ ____ ____ ____ ____ ____ ____
||1 |||2 |||3 |||4 |||5 |||6 |||7 |||8 |||9 |||0 ||
||__|||c#|||d#|||__|||f#|||g#|||a#|||__|||c#|||d#||
|/__\|/__\|/__\|/__\|/__\|/__\|/__\|/__\|/__\|/__\|
   ____ ____ ____ ____ ____ ____ ____ ____ ____ ____
  ||Q |||W |||E |||R |||T |||Y |||U |||I |||O |||P ||
  ||c4|||d4|||e4|||f4|||g4|||a4|||b4|||c5|||d5|||e5||
  |/__\|/__\|/__\|/__\|/__\|/__\|/__\|/__\|/__\|/__\|
     ____ ____ ____ ____ ____ ____ ____ ____ ____ ____
    ||A |||S |||D |||F |||G |||H |||J |||K |||L |||; ||
    ||__|||c#|||d#|||__|||f#|||g#|||a#|||__|||c#|||d#||
    |/__\|/__\|/__\|/__\|/__\|/__\|/__\|/__\|/__\|/__\|
       ____ ____ ____ ____ ____ ____ ____ ____ ____ ____
      ||Z |||X |||C |||V |||B |||N |||M |||, |||. |||/ ||
      ||c3|||d3|||e3|||f3|||g3|||a3|||b3|||c4|||d4|||e4||
      |/__\|/__\|/__\|/__\|/__\|/__\|/__\|/__\|/__\|/__\|
```

# s1

"real" synth. (Work-in-progress)

Run `build\demo`, keyboard piano & pressure_pad turns notes on/bends/off, ESC to quit.

# License

Code is MIT licensed, just as the Sokol code is.