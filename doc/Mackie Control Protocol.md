# Mackie Control Protocol

A deep dive into the DAW remote control beast.

[TOC]

## Foreword

Everyone knows it. Mackie Control is a very wide spread remote control protocol based on a clever use of not less wide spread MIDI. Unfortunately the specification is proprietary and licensed to whom wants to interface their software or controller. Furthermore, the controllers themselves are quite pricey so I decided to dig it and use my iPad as a controller. This is the results of a few months of researching and reverse engineering.

Before you start reading, please note that since it is based on MIDI, you'll to know MIDI very very well to understand.

## The Controller

![mcu](assets/mcu.png)

This is what I focused on, that controller is meant to work with the Mackie Control Protocol and features pretty much everything the protocol can do.

- In Red is the display zone
- In Green is the mixing zone
- In Blue is the function zone
- in Yellow is the transport zone

We will dig into each zone with as much details as I could find.

## General Behaviour

### Buttons, Leds and Bangs

There are more details about each buttons and each functions down that document but there is a common behaviour to all buttons and leds. They send what I call a MIDI *note bang*. This is a MIDI "Note On" message immediately followed by the corresponding "Note Off". The *bang* uses the velocity of the "Note On" message as a mean of transferring the button / led state. Maximum velocity (127) is a button pressed or a led turned on. Minimum velocity is a button released and or a led turned off.

As a side note, this is to avoid too many notes being triggered and considered as "On" by the DAW. Remember, it's MIDI and we want to use existing technology with exiting limitations.

In the rest of the document I will use the term *Note Bang* to describe that behaviour.

### Channel

Unless specified, all the messages are sent on **midi channel 0**.

## Display Zone

![display_zone](assets/display_zone.png)

### LCD Screen

The LCD screen is a 2x56 characters LCD, 112 characters in total. It is controlled by *System Exclusive* messages from the DAW.

The content of the message is the following (in *hexadecimal*):

```
F0 00 00 66 14 12 @@ ## ## .. ## F7
```

- `F0` at the start, `F7` at the end: These are the MIDI *System Exclusive* message boundaries. `F0` being the MIDI *status* of *SystemExclusive*
- `00 00 66` is the Manufacturer ID (Mackie in that case)
- `14` is the Product ID (Mackie Control Universal Pro)
- `12` is the LCD identifier any other values would refer to other components (as I understand it).
- `@@` is the location of the first character sent. This value range between `00` and `6F` (0 to 111 in decimal). 
- `## ## .. ##` is a list of characters. the first one replace the character position `@@`, the second one replacing `@@+1` etc...

Character codes are basic ASCII character. There is room for a character set extension here with some special symbols being used but I've not been able to reference them yet.

### Assignment display

These are controlled with *CC* number 74 and 75 (hex: `4A`, `4B`), for more info on controlling these see [Timecode Display](#timecode-display).

### SMPTE, BEATS, Rude Solo

Those 3 leds are controlled by *Note Bangs* sent by the DAW.

| LED       | Note | Decimal | Hex  |
| --------- | ---- | ------- | ---- |
| SMPTE     | F8   | 113     | 71   |
| BEATS     | F#8  | 114     | 72   |
| Rude Solo | G8   | 115     | 73   |

### Timecode Display

It si composed of 10 7-segments displays. They are controlled by *Control Change* message sent by the DAW. **Channel 15 (the last one) is used.** From right to left, *CC* number 64 (hex: `40`) to 73 (hex: `49`) as such:

![timecode](assets/timecode.png)

The *Value* of the CC control what to display, the 4 most significant bits are *Control* data, the 4 least significant bits are *Value* data.

```
| Control           | Value             |
| b7 | b6 | b5 | b4 | b3 | b2 | b1 | b0 |
```

- Bit 7 is always 0
- Bit 6 is the *Dot* on/off toggle. (each 7-seg display has a dot)
- Bit 5 is always 1
- Bit 4 is the General on/off toggle.
- Bit 3 to 0 represents the ASCII character to display. The characters from `33` to `96` are supported. However if the character code is between `64` and `96`, subtract `64` from the from the character code to display it properly.

## Mixing zone

![mixing](assets/mixing.png)

This where the cool stuff happens and this is probably what you are here for.

### Virtual Pot

Each track has a virtual pot. vPot are composed of:

- A clickable rotary encoder
- 11 surrounding leds, called ring
- 1 small led underneath the encoder

#### Rotating vPots

The MCU sends *CC*s messages to notify the DAW about an encoder being rotated. from *CC* number 16 (hex: 10) for the first channel to 23 (hex: 17) for the eighth channel.

The direction of rotation, and potentially the speed, are represented using the *Value* component of the *CC* as a *signed 7 bits integer*.

- Sending +1 (`0000001` in binary) means rotating clockwise
- Sending -1 (`1000001` in binary) means rotating counter-clockwise

There is a good chance that using greater values than +1 and -1 encodes the speed of rotation but I'm not sure by how much.

#### Clicking vPots

Using good ol' *Note Bang* from G#1 (32 | 20) for the first channel to D#2 (39 | 27) for the eighth channel.

#### vPots Led Ring

the DAW sends *CC*s messages to control the led ring. from *CC* number 48 (hex: 30) for the first channel to *CC* number 55 (hex: 37) for the eighth channel.

Again, the *Value* component is divided in a 4-bits control/value pair.

- Bit 7 is always 0
- Bit 6 is the on/off toggle of the little led underneath the encoder.
- Bits 5 and 4 represent a *Mode* value
- Bits 3 to 0 represent the *Value*

The following table shows the correspondance between *Mode*, *Value* and the led ring state.

|      | Mode `00`     | Mode `01`     | Mode `10`     | Mode `11`     |
| ---- | ------------- | ------------- | ------------- | ------------- |
| `0`  | `-----------` | `-----------` | `-----------` | `-----------` |
| `1`  | `O----------` | `OOOOOO-----` | `O----------` | `-----O-----` |
| `2`  | `-O---------` | `-OOOOO-----` | `OO---------` | `----OOO----` |
| `3`  | `--O--------` | `--OOOO-----` | `OOO--------` | `---OOOOO---` |
| `4`  | `---O-------` | `---OOO-----` | `OOOO-------` | `--OOOOOOO--` |
| `5`  | `----O------` | `----OO-----` | `OOOOO------` | `-OOOOOOOOO-` |
| `6`  | `-----O-----` | `-----O-----` | `OOOOOO-----` | `OOOOOOOOOO`  |
| `7`  | `------O----` | `-----OO----` | `OOOOOOO----` | `OOOOOOOOOO`  |
| `8`  | `-------O---` | `-----OOO---` | `OOOOOOOO---` | `OOOOOOOOOO`  |
| `9`  | `--------O--` | `-----OOOO--` | `OOOOOOOOO--` | `OOOOOOOOOO`  |
| `A`  | `---------O-` | `-----OOOOO-` | `OOOOOOOOOO-` | `OOOOOOOOOO`  |
| `B`  | `----------O` | `-----OOOOOO` | `OOOOOOOOOOO` | `OOOOOOOOOO`  |

### Rec, Solo, Mute, Select

All those buttons follow the *Note Bang* behaviour. Here is a summary of what note they are bound to

| Button | 1st channel           | 8th channel           |
| ------ | --------------------- | --------------------- |
| REC    | C-1 (0 | 0)  |
| SOLO   | G#-1 (8 | 8) |
| MUTE   | E0 (16 | 10) |
| SEL    | C1 (24 | 18) |

### VU Meter

This one is note shown on the illustration but exists in the standard. You can have one VU Meter per channel. They are composed of 12 leds red to green. They are controller by the DAW using *Channel Pressure* MIDI message.

Again the *Value* component of the *Channel Pressure* is divided in a pair a 4-bits data. The most significant 4 bits represents the channel ID from 0 to 7. The least significant 4 bits represents the state of the vu meter as such:

| Value | Signal    | Leds         |
| ----- | --------- | ------------ |
| C     | \>= 0dB   | Red (clip)   |
| B     | \>= -2dB  | Yellow       |
| A     | \>= -4dB  | Yellow       |
| 9     | \>= -6dB  | Yellow       |
| 8     | \>= -8dB  | Green        |
| 7     | \>= -10dB | Green        |
| 6     | \>= -14dB | Green        |
| 5     | \>= -20dB | Green        |
| 4     | \>= -30dB | Green        |
| 3     | \>= -40dB | Green        |
| 2     | \>= -50dB | Green        |
| 1     | \>= -60dB | Green        |
| 0     | < -60dB   | All leds Off |

### Fader

Faders uses the greater resolution of *Pitch Bend* messages for position and *Note Bangs* for fader touch notification.

Fader touch behaves exactly as buttons. when the fader is touched the "button" is pressed. Notes range from G#7 (104 | 68) for the first fader to E8 (112 | 70) for the Master fader (9th fader)

For the position, it uses the full 14 bits of the *Pitch Bend Value* component. 0 being all the way down, and 16383 being to the roof.

**Channel 0 to 8 are used to drive the 9 faders**

Either if the controller wants to mode the faders on the DAW or the DAW wants to move the controller fader, the sequence of messages is the same:

- Fader touched
- Fader new position
- Fader new position
- etc...
- Fader released

## Function zone

![function_zone](assets/function_zone.png)

This part is full of buttons and leds that follows the *Note Bang* behaviour. There is a massive summary table down there where all individual buttons are detailed.

## Transport zone

![transport_zone](assets/transport_zone.png)

### Transport Buttons

They all follow the *Note Bang* behaviour and are detailed in the massive table down there.

### Scroll encoder

This works exactly as the vPot encoders works. It uses *CC* number 60.

- +1: clockwise
- -1: coutner-clockwise

# Massive MIDI Note Mapping Summary

All note mapping is on Channel 0 (the first one).

|     Button / Led     | Note | Dec  | Hex  |
| :------------------: | :--: | :--: | :--: |
|        Rec 1         | C-1  |  0   |  0   |
|        Rec 2         | C#-1 |  1   |  1   |
|        Rec 3         | D-1  |  2   |  2   |
|        Rec 4         | D#-1 |  3   |  3   |
|        Rec 5         | E-1  |  4   |  4   |
|        Rec 6         | F-1  |  5   |  5   |
|        Rec 7         | F#-1 |  6   |  6   |
|        Rec 8         | G-1  |  7   |  7   |
|                      |      |      |      |
|        Solo 1        | G#-1 |  8   |  8   |
|        Solo 2        | A-1  |  9   |  9   |
|        Solo 3        | A#-1 |  10  |  A   |
|        Solo 4        | B-1  |  11  |  B   |
|        Solo 5        |  C0  |  12  |  C   |
|        Solo 6        | C#0  |  13  |  D   |
|        Solo 7        |  D0  |  14  |  E   |
|        Solo 8        | D#0  |  15  |  F   |
|                      |      |      |      |
|        Mute 1        |  E0  |  16  |  10  |
|        Mute 2        |  F0  |  17  |  11  |
|        Mute 3        | F#0  |  18  |  12  |
|        Mute 4        |  G0  |  19  |  13  |
|        Mute 5        | G#0  |  20  |  14  |
|        Mute 6        |  A0  |  21  |  15  |
|        Mute 7        | A#0  |  22  |  16  |
|        Mute 8        |  B0  |  23  |  17  |
|                      |      |      |      |
|        Sel 1         |  C1  |  24  |  18  |
|        Sel 2         | C#1  |  25  |  19  |
|        Sel 3         |  D1  |  26  |  1A  |
|        Sel 4         | D#1  |  27  |  1B  |
|        Sel 5         |  E1  |  28  |  1C  |
|        Sel 6         |  F1  |  29  |  1D  |
|        Sel 7         | F#1  |  30  |  1E  |
|        Sel 8         |  G1  |  31  |  1F  |
|                      |      |      |      |
|    Vpot switch 1     | G#1  |  32  |  20  |
|    Vpot switch 2     |  A1  |  33  |  21  |
|    Vpot switch 3     | A#1  |  34  |  22  |
|    Vpot switch 4     |  B1  |  35  |  23  |
|    Vpot switch 5     |  C2  |  36  |  24  |
|    Vpot switch 6     | C#2  |  37  |  25  |
|    Vpot switch 7     |  D2  |  38  |  26  |
|    Vpot switch 8     | D#2  |  39  |  27  |
|                      |      |      |      |
|     Assign Track     |  E2  |  40  |  28  |
|     Assign Send      |  F2  |  41  |  29  |
| Assign Pan/Surround  | F#2  |  42  |  2A  |
|    Assign Plug-in    |  G2  |  43  |  2B  |
|      Assign EQ       | G#2  |  44  |  2C  |
|  Assign Instrument   |  A2  |  45  |  2D  |
|                      |      |      |      |
|      Bank Left       | A#2  |  46  |  2E  |
|      Bank Right      |  B2  |  47  |  2F  |
|     Channel Left     |  C3  |  48  |  30  |
|    Channel Right     | C#3  |  49  |  31  |
|         Flip         |  D3  |  50  |  32  |
|        Global        | D#3  |  51  |  33  |
|                      |      |      |      |
| Name / Value Button  |  E3  |  52  |  34  |
| SMPTE / BEATS Button |  F3  |  53  |  35  |
|                      |      |      |      |
|          F1          | F#3  |  54  |  36  |
|          F2          |  G3  |  55  |  37  |
|          F3          | G#3  |  56  |  38  |
|          F4          |  A3  |  57  |  39  |
|          F5          | A#3  |  58  |  3A  |
|          F6          |  B3  |  59  |  3B  |
|          F7          |  C4  |  60  |  3C  |
|          F8          | C#4  |  61  |  3D  |
|                      |      |      |      |
|     MIDI Tracks      |  D4  |  62  |  3E  |
|        Inputs        | D#4  |  63  |  3F  |
|     Audio Tracks     |  E4  |  64  |  40  |
|   Audio Intruments   |  F4  |  65  |  41  |
|         Aux          | F#4  |  66  |  42  |
|        Busses        |  G4  |  67  |  43  |
|       Outputs        | G#4  |  68  |  44  |
|         User         |  A4  |  69  |  45  |
|                      |      |      |      |
|        Shift         | A#4  |  70  |  46  |
|        Option        |  B4  |  71  |  47  |
|       Control        |  C5  |  72  |  48  |
|         Alt          | C#5  |  73  |  49  |
|                      |      |      |      |
|       Read/Off        |  D5  |  74  |  4A  |
|        Write         | D#5  |  75  |  4B  |
|         Trim         |  E5  |  76  |  4C  |
|        Touch         |  F5  |  77  |  4D  |
|        Latch         | F#5  |  78  |  4E  |
|        Group         |  G5  |  79  |  4F  |
|         Save         | G#5  |  80  |  50  |
|         Undo         |  A5  |  81  |  51  |
|        Cancel        | A#5  |  82  |  52  |
|        Enter         |  B5  |  83  |  53  |
|                      |      |      |      |
|       Markers        |  C6  |  84  |  54  |
|        Nudge         | C#6  |  85  |  55  |
|        Cycle         |  D6  |  86  |  56  |
|         Drop         | D#6  |  87  |  57  |
|       Replace        |  E6  |  88  |  58  |
|        Click         |  F6  |  89  |  59  |
|         Solo         | F#6  |  90  |  5A  |
|                      |      |      |      |
|        Rewind        |  G6  |  91  |  5B  |
|       Forward        | G#6  |  92  |  5C  |
|         Stop         |  A6  |  93  |  5D  |
|         Play         | A#6  |  94  |  5E  |
|        Record        |  B6  |  95  |  5F  |
|                      |      |      |      |
|          Up          |  C7  |  96  |  60  |
|         Down         | C#7  |  97  |  61  |
|        Scrub         |  D7  |  98  |  62  |
|         Zoom         | D#7  |  99  |  63  |
|         Left         |  E7  | 100  |  64  |
|        Right         |  F7  | 101  |  65  |
|                      |      |      |      |
|                      | F#7  | 102  |  66  |
|                      |  G7  | 103  |  67  |
|                      |      |      |      |
|   Fader 1 Touched    | G#7  | 104  |  68  |
|   Fader 2 Touched    |  A7  | 105  |  69  |
|   Fader 3 Touched    | A#7  | 106  |  6A  |
|   Fader 4 Touched    |  B7  | 107  |  6B  |
|   Fader 5 Touched    |  C8  | 108  |  6C  |
|   Fader 6 Touched    | C#8  | 109  |  6D  |
|   Fader 7 Touched    |  D8  | 110  |  6E  |
|   Fader 8 Touched    | D#8  | 111  |  6F  |
| Master Fader Touched |  E8  | 112  |  70  |
|                      |      |      |      |
|      SMPTE Led       |  F8  | 113  |  71  |
|      BEATS Led       | F#8  | 114  |  72  |
|    RUDE SOLO Led     |  G8  | 115  |  73  |
|                      |      |      |      |
|                      | G#8  | 116  |  74  |
|                      |  A8  | 117  |  75  |
|     Relay Click      | A#8  | 118  |  76  |
|                      |  B8  | 119  |  77  |

# Control Change Mapping Summary

| Control                      | MIDI Channel | CC # | Hex  |
| ---------------------------- | ------------ | ---- | ---- |
| vPot 1 scroll                | 0            | 16   | 10   |
| vPot 2 scroll                | 0            | 17   | 11   |
| vPot 3 scroll                | 0            | 18   | 12   |
| vPot 4 scroll                | 0            | 19   | 13   |
| vPot 5 scroll                | 0            | 20   | 14   |
| vPot 6 scroll                | 0            | 21   | 18   |
| vPot 7 scroll                | 0            | 22   | 16   |
| vPot 8 scroll                | 0            | 23   | 17   |
|                              |              |      |      |
| vPot 1 led ring              | 0            | 48   | 30   |
| vPot 2 led ring              | 0            | 49   | 31   |
| vPot 3 led ring              | 0            | 50   | 32   |
| vPot 4 led ring              | 0            | 51   | 33   |
| vPot 5 led ring              | 0            | 52   | 34   |
| vPot 6 led ring              | 0            | 53   | 35   |
| vPot 7 led ring              | 0            | 54   | 36   |
| vPot 8 led ring              | 0            | 55   | 37   |
|                              |              |      |      |
| Jog wheel                    | 0            | 60   | 3C   |
|                              |              |      |      |
| Timecode digit 1             | 15           | 64   | 40   |
| Timecode digit 2             | 15           | 65   | 41   |
| Timecode digit 3             | 15           | 66   | 42   |
| Timecode digit 4             | 15           | 67   | 43   |
| Timecode digit 5             | 15           | 68   | 44   |
| Timecode digit 6             | 15           | 69   | 45   |
| Timecode digit 7             | 15           | 70   | 46   |
| Timecode digit 8             | 15           | 71   | 47   |
| Timecode digit 9             | 15           | 72   | 48   |
| Timecode digit 10            | 15           | 73   | 49   |
|                              |              |      |      |
| Suspected Assignment digit 1 | 0            | 74   | 4A   |
| Suspected Assignment digit 2 | 0            | 75   | 4B   |

# Pitch Bend Mapping Summary

| Control               | MIDI Channel |
| --------------------- | ------------ |
| Fader 1 position      | 0            |
| Fader 2 position      | 1            |
| Fader 3 position      | 2            |
| Fader 4 position      | 3            |
| Fader 5 position      | 4            |
| Fader 6 position      | 5            |
| Fader 7 position      | 6            |
| Fader 8 position      | 7            |
| Master Fader position | 8            |

# Channel Pressure (After Touch) Mapping Summary

| Control  | MIDI Channel |
| -------- | ------------ |
| Vu Meter | 0            |

# Reminder of MIDI Messages Format

| Message                       | Status Byte<br />D7..D0 | Data1 Byte<br />D7..D0 | Data2 Byte<br />D7..D0 |                                            |
| ----------------------------- | ----------------------- | ---------------------- | ---------------------- | ------------------------------------------ |
| Note Off                      | `1000cccc`              | `0nnnnnnn`             | `0vvvvvvv`             | C: Channel<br />N: Note<br />V: Velocity   |
| Note On                       | `1001cccc`              | `0nnnnnnn`             | `0vvvvvvv`             | C: Channel<br />N: Note<br />V: Velocity   |
| AfterTouch<br />Key pressure  | `1010cccc`              | `0nnnnnnn`             | `0vvvvvvv`             | C: Channel<br />N: Note<br />V: Velocity   |
| Control Change                | `1011cccc`              | `0nnnnnnn`             | `0vvvvvvv`             | C: Channel<br />N: CC number<br />V: Value |
| Program Change                | `1100cccc`              | `0ppppppp`             |                        | C: Channel<br />P: Program                 |
| AfterTouch<br />Chan Pressure | `1101cccc`              | `0vvvvvvv`             |                        | C: Channel<br />V: Value                   |
| Pitch Bend                    | `1110cccc`              | `0lllllll`             | `0mmmmmmm`             | C: Channel<br />L: LSB<br />M: MSB         |
| Sysex start                   | `11110000`              |                        |                        |                                            |
| Sysex end                     | `11110111`              |                        |                        |                                            |
