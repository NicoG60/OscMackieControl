# OscMackieControl
The small software that map the midi implementation of Mackie Control to OSC protocol. It allows you to control any DAW with any OSC capable hardware.
Compatible Mac/Win

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