![C/C++ CI](https://github.com/NicoG60/OscMackieControl/workflows/C/C++%20CI/badge.svg?branch=master)

# OscMackieControl
The small software that map the midi implementation of Mackie Control to OSC protocol. It allows you to control any DAW with any OSC capable hardware.
Compatible Mac/Win

# :warning: DEPRECATION WARNING :warning:

As TouchOSC just release its second version, this software as become obsolete.
Indeed, TouchOSC mkII features more flexible control and a scripting area.
These new features allowed me to implement the Mackie Control Protocol directly within TouchOSC.

You can find the project page here: [TouchMCU](https://github.com/NicoG60/TouchMCU)

---

# Version 2

:tada: Version 2 is on its way! Check it out [here](https://github.com/NicoG60/OscMackieControl/tree/v2-dev)!



## Cloning & Building
It is a Qt5 project.
You can clone the repo and then :
```
cd src
qmake OscMackieControl.pro
make -j 4
```
The result wil be in `build` dir.
If you have the `bin` directory of Qt in your PATH then `xxx_deployqt` is executed to copy every dynamic lib and other file properly in the build dir.

## Installation
This program is not able to create its own virtual midi port to communicate with the DAW so you need to create them by yourself. See the wiki to know how to do that on Windows and Mac OS.

## Note
This app is developped to work with touchOSC app, the layout I made is on the repository. Actually, any OSC capable software or hardware can work, the implementation charts is on the wiki.

