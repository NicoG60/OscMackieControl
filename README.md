

# OscMackieControl
The missing utility that maps Mackie Control to OSC. It allows you to control any DAW with any OSC capable hardware.

| Some badges |                                                              |
| ----------: | ------------------------------------------------------------ |
|     Version | ![version](https://img.shields.io/badge/version-v2.0--dev-blue) |
|       Build | ![C/C++ CI](https://github.com/NicoG60/OscMackieControl/workflows/C/C++%20CI/badge.svg?branch=v2-dev) |
|     License | ![license](https://img.shields.io/github/license/NicoG60/OscMackieControl) |
|    Platform | ![macOS](https://img.shields.io/badge/macOS-yes-green?logo=apple&logoColor=white)<br />![win10](https://img.shields.io/badge/Win 10-yes-green?logo=windows) |

## Roadmap

The second version of the software is currently being developed. Here is a kinda roadmap of things to do and things that needs to be done.

### :white_check_mark: Phase 1: Better OSC

The previous software used [osc-cpp-qt](https://github.com/MugenSAS/osc-cpp-qt) to read and write OSC messages. It turned out that library has a couple of flows and is not maintained. Since there does not seem to be an OSC implementation based on Qt, I'll do my own.

- [x] Basic OSC types (int32, float32, strings blobs)

- [x] Extended OSC types (color, midi, int64, float64, etc...)

- [x] ~~OSC arrays~~ (Removed for v1)

- [x] OSC Messages implementation

- [x] OSC Bundle implementation

- [x] OSC Interface to send and receive packets to and from the network

- [x] Tests

- [x] Release as an independent library

  **The library is now available! Go check out [QOsc](https://github.com/NicoG60/QOsc)**

### Phase 2: Better MIDI

The previous software relied on [RtMidi](https://www.music.mcgill.ca/~gary/rtmidi/) to provide MIDI communication. The question is wether keeping it or moving to [PortMidi](https://sourceforge.net/projects/portmedia/files/) which is also a good library. RtMidi is still maintained and the v4 has been release less than a yeah ago. PortMidi does not seem active anymore. I'll probably keep going with RtMidi.

- [x] Choose a library to use
- [ ] Implement an interface on top of it to make it easily usable with Qt
- [ ] **Find a way to create virtual midi ports and avoid the use of a third party software**
- [ ] Test
- [ ] Release as an independent library

### Phase 3: Better feature set

Here is a randomly ordered list of features that is worth considering and implementing

- [ ] Mapping editor, being able to change the Mackie Control bit is sent to what OSC address.
- [ ] Mapping options, being able to choose different way of interpreting OSC information. For example TouchOSC heavily relies on numbers even for button press that could be represented as a boolean value. Another example is the LCD display emulation. In the TouchOSC layout, each god damn character is a single element with its own OSC address. you may want to change that to a single string label (even though it will probably not align correctly).
- [ ] Mapping import/export
- [ ] Overlay editor, let the user customise the labels on the touchOSC layout the same way Mackie Control Units have that kinda plastic overlay for different DAWs.
- [ ] Provide an extended set of overlays.
- [ ] Overlay import/export
- [ ] Update the TouchOSC layout if needed
- [ ] Provide an Open Stage Control layout

### Phase 4: Better translator

The current translation component is a beast and doesn't feel right, nor it feels efficient. There is a lot of room here for better implementation and clever design that allow the above features to be easily integrated.

That's also the occasion to implement the few missing feature I've not been able to lift out the first time. (Assignment display for example)

### Phase 5: Better Interface

We are in 2020, that'd be nice to have an interface that does not looks like a port of a 90's software. Random considerations:

- [ ] Use QML?
- [ ] Simpler workflow
- [ ] Hide complexity
- [ ] Provide sensible default values for non tech savvy
- [ ] System tray icon, option to close the window
- [ ] User feedback, show if information is sent, received, translated

### Phase 6: Better documentation

- [ ] Rewrite the documentation in a proper English.
- [ ] Clarify the implementation chart
- [ ] Get rid of that silly PDF thing, use markdown, pages, wiki, etc.

## Downloading

You can head to the [Release section](https://github.com/NicoG60/OscMackieControl/releases) to find the latest download links.

## Building

The project is Qt based.

- Qt 5.15.1
- MSVC 2019 for Windows
- Xcode 12 for macOS

There is submodules to check out

```console
$ git submodule init && git submodule update
```

Then you can build the project

```console
$ mkdir build && cd build
$ qmake .. && make
```

