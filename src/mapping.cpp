#include "mapping.h"
#include <QMidi>

Mapping::Mapping()
{
    loadDefault();
}

void Mapping::loadDefault()
{
    REC  = { QMidi::C_m1,      "/REC",  "/ledREC"  };
    SOLO = { QMidi::GSharp_m1, "/SOLO", "/ledSOLO" };
    MUTE = { QMidi::E_0,       "/MUTE", "/ledMUTE" };
    SEL  = { QMidi::C_1,       "/SEL",  "/ledSEL" };
    VPOTSelect = { QMidi::GSharp_1, "/VPOT", "/ledPOT" };
    Function   = { QMidi::FSharp_3, "/F", {}  };

    otherButtons.clear();

    otherButtons.append({ QMidi::E_2,      "/Track",      "/ledTrack"  });
    otherButtons.append({ QMidi::F_2,      "/Send",       "/ledSend"   });
    otherButtons.append({ QMidi::FSharp_2, "/Pan",        "/ledPan"    });
    otherButtons.append({ QMidi::G_2,      "/Plugin",     "/ledPlugin" });
    otherButtons.append({ QMidi::GSharp_2, "/EQ",         "/ledEQ"     });
    otherButtons.append({ QMidi::A_2,      "/Instrument", "/ledInstrument" });

    otherButtons.append({ QMidi::ASharp_2, "/BankL",    {} });
    otherButtons.append({ QMidi::B_2,      "/BankR",    {} });
    otherButtons.append({ QMidi::C_3,      "/ChannelL", {} });
    otherButtons.append({ QMidi::CSharp_3, "/ChannelR", {} });
    otherButtons.append({ QMidi::D_3,      "/Flip",     "/ledFlip"   });
    otherButtons.append({ QMidi::DSharp_3, "/Global",   "/ledGlobal" });

    otherButtons.append({ QMidi::E_3, "/NameValue",  {} });
    otherButtons.append({ QMidi::F_3, "/SmpteBeats", {} });

    otherButtons.append({ QMidi::D_4,      "/MidiTracks",      {} });
    otherButtons.append({ QMidi::DSharp_4, "/Inputs",          {} });
    otherButtons.append({ QMidi::E_4,      "/AudioTracks",     {} });
    otherButtons.append({ QMidi::F_4,      "/AudioInstrument", {} });
    otherButtons.append({ QMidi::FSharp_4, "/Aux",             {} });
    otherButtons.append({ QMidi::G_4,      "/Busses",          {} });
    otherButtons.append({ QMidi::GSharp_4, "/Outputs",         {} });
    otherButtons.append({ QMidi::A_4,      "/User",            {} });

    otherButtons.append({ QMidi::ASharp_4, "/Shift",   {} });
    otherButtons.append({ QMidi::B_4,      "/Option",  {} });
    otherButtons.append({ QMidi::C_5,      "/Control", {} });
    otherButtons.append({ QMidi::CSharp_5, "/Alt",     {} });

    otherButtons.append({ QMidi::D_5,      "/ReadOff", "/ledReadOff" });
    otherButtons.append({ QMidi::DSharp_5, "/Write",   "/ledWrite"   });
    otherButtons.append({ QMidi::E_5,      "/Trim",    "/ledTrim"    });
    otherButtons.append({ QMidi::F_5,      "/Touch",   "/ledTouch"   });
    otherButtons.append({ QMidi::FSharp_5, "/Latch",   "/ledLatch"   });
    otherButtons.append({ QMidi::G_5,      "/Group",   "/ledGroup"   });

    otherButtons.append({ QMidi::GSharp_5, "/Save",   "/ledSave" });
    otherButtons.append({ QMidi::A_5,      "/Undo",   "/ledUndo" });
    otherButtons.append({ QMidi::ASharp_5, "/Cancel", {} });
    otherButtons.append({ QMidi::B_5,      "/Enter",  {} });

    otherButtons.append({ QMidi::C_6,      "/Markers", "/ledMarkers" });
    otherButtons.append({ QMidi::CSharp_6, "/Nudge",   "/ledNudge"   });
    otherButtons.append({ QMidi::D_6,      "/Cycle",   "/ledCycle"   });
    otherButtons.append({ QMidi::DSharp_6, "/Drop",    "/ledDrop"    });
    otherButtons.append({ QMidi::E_6,      "/Replace", "/ledReplace" });
    otherButtons.append({ QMidi::F_6,      "/Click",   "/ledClick"   });
    otherButtons.append({ QMidi::FSharp_6, "/Solo",    "/ledSolo"    });

    otherButtons.append({ QMidi::G_6,      "/Rewind",  "/ledRewind"  });
    otherButtons.append({ QMidi::GSharp_6, "/Forward", "/ledForward" });
    otherButtons.append({ QMidi::A_6,      "/Stop",    "/ledStop"    });
    otherButtons.append({ QMidi::ASharp_6, "/Play",    "/ledPlay"    });
    otherButtons.append({ QMidi::B_6,      "/Rec",     "/ledRec"     });

    otherButtons.append({ QMidi::C_7,      "/Up",    {} });
    otherButtons.append({ QMidi::CSharp_7, "/Down",  {} });
    otherButtons.append({ QMidi::D_7,      "/Scrub", "/ledScrub" });
    otherButtons.append({ QMidi::DSharp_7, "/Zoom",  "/ledZoom"  });
    otherButtons.append({ QMidi::E_7,      "/Left",  {} });
    otherButtons.append({ QMidi::F_7,      "/Right", {} });

    otherButtons.append({ QMidi::F_8,      "/ledSMPTE" });
    otherButtons.append({ QMidi::FSharp_8, "/ledBEATS" });

    vPotLedBaseAddr  = "/VPOTRing";
    timecodeBaseAddr = "/Timecode";
    vuMeterBaseAddr  = "/led";
    faderBaseAddr    = "/Fader";
    jogBaseAddr      = "/Jog";
    charBaseAddr     = "/char";
    trackDisplayBaseAddr = "/TrackDisplay";
    lcdBaseAddr      = "/LCD";
    lcdLineBaseAddr  = "/LCDLine";
}

void Mapping::loadFromJson(const QJsonObject& obj)
{
    Q_UNUSED(obj);
}

QJsonObject Mapping::dumpJson()
{
    return {};
}
