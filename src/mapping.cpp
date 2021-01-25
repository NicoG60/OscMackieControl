#include "mapping.h"
#include <QMidi>
#include <QRect>
#include <QLineF>
#include <QtMath>
#include <QBuffer>

#include <sstream>

#include <zip_file.hpp>

Mapping::Mapping(QObject* parent) :
    QObject(parent)
{
    loadDefault();
}

void Mapping::loadDefault()
{
    REC        = createButton(QMidi::C_m1,      "REC");
    SOLO       = createButton(QMidi::GSharp_m1, "SOLO");
    MUTE       = createButton(QMidi::E_0,       "MUTE");
    SEL        = createButton(QMidi::C_1,       "SEL");
    VPOTSelect = createButton(QMidi::GSharp_1,  "VPOTSelect", " ");
    Functions  = createButton(QMidi::FSharp_3,  "F");
    Functions.ledAddr.clear();

    otherButtons.clear();

    registerButtonLed(QMidi::E_2,      "Track");
    registerButtonLed(QMidi::F_2,      "Send");
    registerButtonLed(QMidi::FSharp_2, "Pan");
    registerButtonLed(QMidi::G_2,      "Plugin");
    registerButtonLed(QMidi::GSharp_2, "EQ");
    registerButtonLed(QMidi::A_2,      "Instrument");

    registerButton(QMidi::ASharp_2,    "BankL",    "< BANK >");
    registerButton(QMidi::B_2,         "BankR",    " ");
    registerButton(QMidi::C_3,         "ChannelL", "< CHANNEL >");
    registerButton(QMidi::CSharp_3,    "ChannelR", " ");
    registerButtonLed(QMidi::D_3,      "Flip");
    registerButtonLed(QMidi::DSharp_3, "Global");

    registerButton(QMidi::E_3, "NameValue", "Name", "Value");
    registerButton(QMidi::F_3, "SmpteBeats", "SMPTE", "BEATS");

    registerButton(QMidi::D_4,      "MidiTracks", "MIDI");
    registerButton(QMidi::DSharp_4, "Inputs");
    registerButton(QMidi::E_4,      "AudioTracks", "Audio");
    registerButton(QMidi::F_4,      "AudioInstrument", "Intr.");
    registerButton(QMidi::FSharp_4, "Aux");
    registerButton(QMidi::G_4,      "Busses");
    registerButton(QMidi::GSharp_4, "Outputs");
    registerButton(QMidi::A_4,      "User");

    registerButton(QMidi::ASharp_4, "Shift");
    registerButton(QMidi::B_4,      "Option");
    registerButton(QMidi::C_5,      "Control");
    registerButton(QMidi::CSharp_5, "Alt");

    registerButtonLed(QMidi::D_5,      "ReadOff", "Read/Off");
    registerButtonLed(QMidi::DSharp_5, "Write");
    registerButtonLed(QMidi::E_5,      "Trim");
    registerButtonLed(QMidi::F_5,      "Touch");
    registerButtonLed(QMidi::FSharp_5, "Latch");
    registerButtonLed(QMidi::G_5,      "Group");

    registerButtonLed(QMidi::GSharp_5, "Save");
    registerButtonLed(QMidi::A_5,      "Undo");
    registerButton(   QMidi::ASharp_5, "Cancel");
    registerButton(   QMidi::B_5,      "Enter");

    registerButtonLed(QMidi::C_6,      "Markers");
    registerButtonLed(QMidi::CSharp_6, "Nudge");
    registerButtonLed(QMidi::D_6,      "Cycle");
    registerButtonLed(QMidi::DSharp_6, "Drop");
    registerButtonLed(QMidi::E_6,      "Replace");
    registerButtonLed(QMidi::F_6,      "Click");
    registerButtonLed(QMidi::FSharp_6, "Solo");

    registerButtonLed(QMidi::G_6,      "Rewind", "<<");
    registerButtonLed(QMidi::GSharp_6, "Forward", ">>");
    registerButtonLed(QMidi::A_6,      "Stop");
    registerButtonLed(QMidi::ASharp_6, "Play");
    registerButtonLed(QMidi::B_6,      "Rec");

    registerButton(   QMidi::C_7,      "Up",    " ");
    registerButton(   QMidi::CSharp_7, "Down",  " ");
    registerButtonLed(QMidi::D_7,      "Scrub");
    registerButtonLed(QMidi::DSharp_7, "Zoom");
    registerButton(   QMidi::E_7,      "Left",  " ");
    registerButton(   QMidi::F_7,      "Right", " ");

    registerButtonLed(QMidi::F_8,      "SMPTE");
    registerButtonLed(QMidi::FSharp_8, "BEATS");
    registerButtonLed(QMidi::G_8,      "RUDESOLO", "RUDE SOLO");

    VPOTBaseAddr     = "/VPOT";
    VPOTLedBaseAddr  = "/VPOTRing";
    timecodeBaseAddr = "/Timecode";
    vuMeterBaseAddr  = "/VuMeter";
    faderBaseAddr    = "/Fader";
    jogBaseAddr      = "/Jog";
    assignmentBaseAddr = "/Assignment";
    lcdCharBaseAddr  = "/LcdChar";
    lcdTrackBaseAddr = "/LcdTrack";
    lcdBaseAddr      = "/Lcd";
    lcdLineBaseAddr  = "/LcdLine";

    lbTrackNum   = { "/labelTrack",       "0" };
    lbVPOTAssign = { "/labelVPOTAssign",  "VPOT Assign" };
    lbDisplay    = { "/labelDisplay",     "Display" };
    lbAssigment  = { "/labelAssignement", "Assignement" };
    lbFaderBanks = { "/labelFaderBanks",  "Fader Banks" };
    lbGlobalView = { "/labelGlobalView",  "Global View"};
    lbFunctions  = { "/labelFunctions",   "Functions"};
    lbModifiers  = { "/labelModifiers",   "Modifiers"};
    lbUtilities  = { "/labelUtilities",   "Utilities" };
    lbAutomation = { "/labelAutomation",  "Automation"};
    lbTransport  = { "/labelTransport",   "Transport" };
    lbTimecode   = { "/labelTimecode",    "Timecode" };
}

void Mapping::loadFromJson(const QJsonObject& obj)
{
    if(obj.isEmpty())
        return;

    REC        = toButton(obj["rec"].toObject());
    MUTE       = toButton(obj["mute"].toObject());
    SOLO       = toButton(obj["solo"].toObject());
    SEL        = toButton(obj["select"].toObject());
    VPOTSelect = toButton(obj["vpot_select"].toObject());
    Functions  = toButton(obj["functions"].toObject());

    auto others = obj["other_buttons"].toObject();

    for(auto it = others.begin(); it != others.end(); it++)
        otherButtons[it.key().toInt()] = toButton(it->toObject());

    VPOTBaseAddr     = obj["vpot"].toString();
    VPOTLedBaseAddr  = obj["vpot_led"].toString();
    timecodeBaseAddr = obj["timecode"].toString();
    vuMeterBaseAddr  = obj["vu"].toString();
    faderBaseAddr    = obj["fader"].toString();
    jogBaseAddr      = obj["jog"].toString();
    assignmentBaseAddr = obj["assignment"].toString();

    lcdCharBaseAddr  = obj["lcd_char"].toString();
    lcdTrackBaseAddr = obj["lcd_track"].toString();
    lcdBaseAddr      = obj["lcd"].toString();
    lcdLineBaseAddr  = obj["lcd_line"].toString();

    lbTrackNum   = toLabel(obj["label_track_number"].toObject());
    lbVPOTAssign = toLabel(obj["label_vpot_assign"].toObject());
    lbDisplay    = toLabel(obj["label_display"].toObject());
    lbAssigment  = toLabel(obj["label_assignment"].toObject());
    lbFaderBanks = toLabel(obj["label_fader_banks"].toObject());
    lbGlobalView = toLabel(obj["label_global_view"].toObject());
    lbFunctions  = toLabel(obj["label_functions"].toObject());
    lbModifiers  = toLabel(obj["label_modifiers"].toObject());
    lbUtilities  = toLabel(obj["label_utilities"].toObject());
    lbAutomation = toLabel(obj["label_automation"].toObject());
    lbTransport  = toLabel(obj["label_transport"].toObject());
    lbTimecode   = toLabel(obj["label_timecode"].toObject());
}

QJsonObject Mapping::dumpJson() const
{
    QJsonObject r{
        { "rec",         toJson(REC) },
        { "mute",        toJson(MUTE) },
        { "solo",        toJson(SOLO) },
        { "select",      toJson(SEL) },
        { "vpot_select", toJson(VPOTSelect) },
        { "functions",   toJson(Functions) },
    };

    QJsonObject other;
    for(auto& btn : otherButtons)
        other.insert(QString::number(btn.note), toJson(btn));

    r.insert("other_buttons", other);

    r.insert("vpot",       VPOTBaseAddr);
    r.insert("vpot_led",   VPOTLedBaseAddr);
    r.insert("timecode",   timecodeBaseAddr);
    r.insert("vu",         vuMeterBaseAddr);
    r.insert("fader",      faderBaseAddr);
    r.insert("jog",        jogBaseAddr);
    r.insert("assignment", assignmentBaseAddr);

    r.insert("lcd_char",  lcdCharBaseAddr);
    r.insert("lcd_track", lcdTrackBaseAddr);
    r.insert("lcd",       lcdBaseAddr);
    r.insert("lcd_line",  lcdLineBaseAddr);

    r.insert("label_track_number", toJson(lbTrackNum));
    r.insert("label_vpot_assign",  toJson(lbVPOTAssign));
    r.insert("label_display",      toJson(lbDisplay));
    r.insert("label_assignment",   toJson(lbAssigment));
    r.insert("label_fader_banks",  toJson(lbFaderBanks));
    r.insert("label_global_view",  toJson(lbGlobalView));
    r.insert("label_functions",    toJson(lbFunctions));
    r.insert("label_modifiers",    toJson(lbModifiers));
    r.insert("label_utilities",    toJson(lbUtilities));
    r.insert("label_automation",   toJson(lbAutomation));
    r.insert("label_transport",    toJson(lbTransport));
    r.insert("label_timecode",     toJson(lbTimecode));

    return r;
}

QVariantMap Mapping::mapping() const
{
    return dumpJson().toVariantMap();
}

void Mapping::setMapping(const QVariantMap& m)
{
    loadFromJson(QJsonObject::fromVariantMap(m));
}

MappingModel* Mapping::createModel()
{
    return new MappingModel(this);
}

QJsonObject Mapping::toJson(const ButtonControl& btn) const
{
    return {
        { "btn_addr", btn.btnAddr },
        { "led_addr", btn.ledAddr },
        { "label1_addr", btn.label1Addr },
        { "label2_addr", btn.label2Addr },
        { "default_label1", btn.defaultLabel1 },
        { "default_label2", btn.defaultLabel2 }
    };
}

QJsonObject Mapping::toJson(const LabelControl& lb) const
{
    return {
        { "addr", lb.addr },
        { "default_string", lb.defaultString }
    };
}

ButtonControl Mapping::toButton(const QJsonObject& obj) const
{
    ButtonControl btn;

    if(obj.contains("btn_addr"))
        btn.btnAddr = obj["btn_addr"].toString();

    if(obj.contains("led_addr"))
        btn.ledAddr = obj["led_addr"].toString();

    if(obj.contains("label1_addr"))
        btn.label1Addr = obj["label1_addr"].toString();

    if(obj.contains("label2_addr"))
        btn.label2Addr = obj["label2_addr"].toString();

    if(obj.contains("default_label1"))
        btn.defaultLabel1 = obj["default_label1"].toString();

    if(obj.contains("default_label2"))
        btn.defaultLabel2 = obj["default_label2"].toString();

    return btn;
}

LabelControl Mapping::toLabel(const QJsonObject& obj) const
{
    LabelControl lb;

    if(obj.contains("addr"))
        lb.addr = obj["addr"].toString();

    if(obj.contains("default_string"))
        lb.defaultString = obj["default_string"].toString();

    return lb;
}

void Mapping::exportTouchOSCLayout(QIODevice* dev, bool in_zip)
{
    if(in_zip)
    {
        miniz_cpp::zip_file f;

        QBuffer buf;
        buf.open(QIODevice::ReadWrite);
        exportTouchOSCXML(&buf);

        f.writestr("index.xml", buf.data().toStdString());

        std::ostringstream stream;
        f.save(stream);

        auto out = stream.str();

        dev->write(out.c_str(), out.length());
    }
    else
        exportTouchOSCXML(dev);
}

ButtonControl Mapping::createButton(quint8 note, const QString& name, const QString& label, const QString& label2)
{
    ButtonControl btn;

    btn.note = note;
    btn.btnAddr = QStringLiteral("/") + name;
    btn.ledAddr = QStringLiteral("/led") + name;
    btn.label1Addr = QStringLiteral("/label") + name;
    btn.defaultLabel1 = label.isEmpty() ? name : label;

    if(!label2.isEmpty())
    {
        btn.label1Addr += "1";
        btn.label2Addr = QStringLiteral("/label") + name + "2";
        btn.defaultLabel2 = label2;
    }

    return btn;
}

void Mapping::registerButton(quint8 note, const QString& name, const QString& label, const QString& label2)
{
    auto btn = createButton(note, name, label, label2);
    btn.ledAddr.clear();
    otherButtons.insert(btn.note, btn);
}

void Mapping::registerButtonLed(quint8 note, const QString& name, const QString& label, const QString& label2)
{
    otherButtons.insert(note, createButton(note, name, label, label2));
}

void Mapping::exportTouchOSCXML(QIODevice* dev)
{
    QXmlStreamWriter writer(dev);
    writer.setAutoFormatting(false);
    writer.writeStartDocument();

    writer.writeStartElement("layout");
    writer.writeAttribute("version", "17");
    writer.writeAttribute("mode", "1");
    writer.writeAttribute("orientation", "vertical");

    writeStartPage(&writer, "1");

    exportTouchOSCLCD(&writer);

    for(quint8 t = 0; t < 8; t++)
        exportTouchOSCTrack(&writer, t);

    exportTouchOSCTimecode(&writer);
    exportTouchOSCAssign(&writer);
    exportTouchOSCFaderBank(&writer);
    exportTouchOSCMaster(&writer);

    writer.writeEndElement();

    writeStartPage(&writer, "2");

    exportTouchOSCGlobalView(&writer);
    exportTouchOSCFunctions(&writer);
    exportTouchOSCModifiers(&writer);
    exportTouchOSCAutomation(&writer);
    exportTouchOSCUtilities(&writer);
    exportTouchOSCTransport(&writer);
    exportTouchOSCTimecodeAssign(&writer);
    exportTouchOSCMovingPart(&writer);

    writer.writeEndElement();

    writer.writeEndElement();

    writer.writeEndDocument();
}

void Mapping::exportTouchOSCTrack(QXmlStreamWriter* writer, quint8 track)
{
    int baseX = 100*track;
    QString ntrack = QString::number(track+1);

    // Make VPOT
    TouchOscButtonOption vpotopt;
    vpotopt.ledRect = QRect(13, 43, 20, 20);

    auto VPOTTrack = VPOTSelect;
    VPOTTrack.btnAddr += ntrack;
    VPOTTrack.ledAddr += ntrack;
    VPOTTrack.label1Addr += ntrack;


    QRectF vPotRect(baseX, 64, 100, 100);
    exportTouchOSCEncoder(writer, vPotRect.toRect(), "gray", VPOTBaseAddr + ntrack);
    exportTouchOSCMackieControlButton(writer, {baseX+27, 64+35, 46, 30}, VPOTTrack, vpotopt);

    QRect vPotLedRect(0, 0, 16, 16);
    QString vPotLedAddr = VPOTLedBaseAddr + ntrack;
    qreal angle = 210;
    qreal step = 24;
    for(int i = 0; i < 11; i++, angle -= step)
    {
        auto l = QLineF::fromPolar(40, angle);
        l.translate(vPotRect.center());
        vPotLedRect.moveCenter(l.p2().toPoint());
        exportTouchOSCLed(writer, vPotLedRect, "red", vPotLedAddr + QString::number(i+1));
    }

    TouchOscButtonOption btnopt;
    btnopt.fontSize = 14;
    btnopt.ledColor = "red";
    btnopt.ledRect = QRect(13, 26, 20, 20);
    btnopt.btnColor = "red";

    // Make REC
    auto RECTrack = REC;
    RECTrack.btnAddr += ntrack;
    RECTrack.ledAddr += ntrack;
    RECTrack.label1Addr += ntrack;

    exportTouchOSCMackieControlButton(writer, {baseX+27, 184, 46, 46}, RECTrack, btnopt);

    // Make SOLO
    btnopt.ledRect = QRect(50, 7, 20, 20);
    btnopt.label1Rect = QRect(0, 7, 50, 20);
    btnopt.ledColor = "orange";
    btnopt.btnColor = "orange";

    auto SOLOTrack = SOLO;
    SOLOTrack.btnAddr += ntrack;
    SOLOTrack.ledAddr += ntrack;
    SOLOTrack.label1Addr += ntrack;

    exportTouchOSCMackieControlButton(writer, {baseX+15, 236, 70, 34}, SOLOTrack, btnopt);

    // Make MUTE
    btnopt.ledColor = "red";
    btnopt.btnColor = "red";

    auto MUTETrack = MUTE;
    MUTETrack.btnAddr += ntrack;
    MUTETrack.ledAddr += ntrack;
    MUTETrack.label1Addr += ntrack;

    exportTouchOSCMackieControlButton(writer, {baseX+15, 272,   70, 34}, MUTETrack, btnopt);

    // Make SEL
    btnopt.ledColor = "blue";
    btnopt.btnColor = "blue";

    auto SELTrack = SEL;
    SELTrack.btnAddr += ntrack;
    SELTrack.ledAddr += ntrack;
    SELTrack.label1Addr += ntrack;

    exportTouchOSCMackieControlButton(writer, {baseX+15, 308, 70, 34}, SELTrack, btnopt);

    // track num
    exportTouchOSCLabel(writer, {baseX, 344, 100, 20}, "gray", ntrack, 14, lbTrackNum.addr + ntrack, true, true);

    // track fader
    exportTouchOSCFader(writer, {baseX+20, 364, 60, 344}, "gray", faderBaseAddr + ntrack);

    // vu meter
    QString ledAddr = vuMeterBaseAddr + ntrack;
    exportTouchOSCLed(writer, {baseX+80, 528, 14, 14}, "red",    ledAddr + QString::number(12));
    exportTouchOSCLed(writer, {baseX+80, 543, 14, 14}, "yellow", ledAddr + QString::number(11));
    exportTouchOSCLed(writer, {baseX+80, 558, 14, 14}, "yellow", ledAddr + QString::number(10));
    exportTouchOSCLed(writer, {baseX+80, 573, 14, 14}, "yellow", ledAddr + QString::number(9));
    exportTouchOSCLed(writer, {baseX+80, 588, 14, 14}, "green",  ledAddr + QString::number(8));
    exportTouchOSCLed(writer, {baseX+80, 603, 14, 14}, "green",  ledAddr + QString::number(7));
    exportTouchOSCLed(writer, {baseX+80, 618, 14, 14}, "green",  ledAddr + QString::number(6));
    exportTouchOSCLed(writer, {baseX+80, 633, 14, 14}, "green",  ledAddr + QString::number(5));
    exportTouchOSCLed(writer, {baseX+80, 648, 14, 14}, "green",  ledAddr + QString::number(4));
    exportTouchOSCLed(writer, {baseX+80, 663, 14, 14}, "green",  ledAddr + QString::number(3));
    exportTouchOSCLed(writer, {baseX+80, 678, 14, 14}, "green",  ledAddr + QString::number(2));
    exportTouchOSCLed(writer, {baseX+80, 693, 14, 14}, "green",  ledAddr + QString::number(1));
}

void Mapping::exportTouchOSCLCD(QXmlStreamWriter* writer)
{
    /*
    exportTouchOSCLabel(writer, {0, 4, 800, 44}, "blue", " ", 14, "/backgroundLCD", true, true);

    for(int t = 0; t < 8; t++)
    {
        int charIdx = 1+7*t;
        int baseX = 1+100*t;

        for(int c = 0; c < 7; c++)
        {
            auto idx1 = QString::number(charIdx);
            auto idx2 = QString::number(charIdx+56);

            exportTouchOSCLabel(writer, {baseX, 5,  20, 30}, "blue", idx1, 14, charBaseAddr + idx1, false, false);
            exportTouchOSCLabel(writer, {baseX, 23, 20, 30}, "blue", idx2, 14, charBaseAddr + idx2, false, false);

            charIdx++;
            baseX += 14;
        }
    }
    /*/
    for(int t = 0; t < 8; t++)
    {
        int x = 100*t;
        auto idx = QString::number(t+1);

        exportTouchOSCLabel(writer, {x, 4,  100, 60}, "blue", " ", 18, "/backgroundLCD" + idx, true, true);
        exportTouchOSCLabel(writer, {x, 8,  100, 30}, "blue", "       ", 18, lcdTrackBaseAddr + idx + "1", false, false);
        exportTouchOSCLabel(writer, {x, 30, 100, 30}, "blue", "       ", 18, lcdTrackBaseAddr + idx + "2", false, false);
    }
    //*/
}

void Mapping::exportTouchOSCTimecode(QXmlStreamWriter* writer)
{
    exportTouchOSCLabel(writer, {810, 4, 214, 60}, "red", " ", 14, "/backgroundTimecode", true, true);

    exportTouchOSCLabel(writer, {812, 4, 24, 60}, "red", "0",  30, timecodeBaseAddr + QString::number(1), false, false);
    exportTouchOSCLabel(writer, {830, 4, 24, 60}, "red", "0",  30, timecodeBaseAddr + QString::number(2), false, false);
    exportTouchOSCLabel(writer, {848, 4, 30, 60}, "red", "0.", 30, timecodeBaseAddr + QString::number(3), false, false);

    exportTouchOSCLabel(writer, {874, 4, 24, 60}, "red", "0",  30, timecodeBaseAddr + QString::number(4), false, false);
    exportTouchOSCLabel(writer, {892, 4, 30, 60}, "red", "0.", 30, timecodeBaseAddr + QString::number(5), false, false);

    exportTouchOSCLabel(writer, {918, 4, 24, 60}, "red", "0",  30, timecodeBaseAddr + QString::number(6), false, false);
    exportTouchOSCLabel(writer, {936, 4, 30, 60}, "red", "0.", 30, timecodeBaseAddr + QString::number(7), false, false);

    exportTouchOSCLabel(writer, {962, 4, 24, 60}, "red", "0",  30, timecodeBaseAddr + QString::number(8), false, false);
    exportTouchOSCLabel(writer, {980, 4, 24, 60}, "red", "0",  30, timecodeBaseAddr + QString::number(9), false, false);
    exportTouchOSCLabel(writer, {998, 4, 24, 60}, "red", "0", 30, timecodeBaseAddr + QString::number(10), false, false);

    auto& smpte = otherButtons[QMidi::F_8];
    exportTouchOSCLabel(writer, {810, 64, 50, 20}, "gray", smpte.defaultLabel1, 12, smpte.label1Addr, false, false);
    exportTouchOSCLed(writer, {860, 67, 14, 14}, "red", smpte.ledAddr);

    auto& beats = otherButtons[QMidi::FSharp_8];
    exportTouchOSCLabel(writer, {874, 64, 50, 20}, "gray", beats.defaultLabel1, 12, beats.label1Addr, false, false);
    exportTouchOSCLed(writer, {924, 67, 14, 14}, "red", beats.ledAddr);

    auto& rude = otherButtons[QMidi::G_8];
    exportTouchOSCLabel(writer, {938, 64, 72, 20}, "gray", rude.defaultLabel1, 12, rude.label1Addr, false, false);
    exportTouchOSCLed(writer, {1010, 67, 14, 14}, "red", rude.ledAddr);
}

void Mapping::exportTouchOSCAssign(QXmlStreamWriter* writer)
{
    exportTouchOSCLabel(writer, {810, 106, 104, 20}, "gray", lbVPOTAssign.defaultString, 14, lbVPOTAssign.addr, true, true);

    TouchOscButtonOption opt;
    opt.ledColor = "green";
    opt.label1Rect = QRect(0, -20, 46, 20);

    // Track
    exportTouchOSCMackieControlButton(writer, {810, 146, 46, 30}, otherButtons[QMidi::E_2], opt);

    // Send
    exportTouchOSCMackieControlButton(writer, {868, 146, 46, 30}, otherButtons[QMidi::F_2], opt);

    // Pan
    exportTouchOSCMackieControlButton(writer, {810, 196, 46, 30}, otherButtons[QMidi::FSharp_2], opt);

    // Plugin
    exportTouchOSCMackieControlButton(writer, {868, 196, 46, 30}, otherButtons[QMidi::G_2], opt);

    // EQ
    exportTouchOSCMackieControlButton(writer, {810, 246, 46, 30}, otherButtons[QMidi::GSharp_2], opt);

    // Instru
    exportTouchOSCMackieControlButton(writer, {868, 246, 46, 30}, otherButtons[QMidi::A_2], opt);


    exportTouchOSCLabel(writer, {924, 106, 100, 20}, "gray", lbDisplay.defaultString, 14, lbDisplay.addr, true, true);

    // Name Value
    exportTouchOSCMackieControlButton(writer, {924, 126, 50, 40}, otherButtons[QMidi::E_3]);

    // SMPTE Beats
    exportTouchOSCMackieControlButton(writer, {974, 126, 50, 40}, otherButtons[QMidi::F_3]);

    exportTouchOSCLabel(writer, {924, 246, 100, 20}, "gray", lbAssigment.defaultString, 14, lbAssigment.addr, true, true);
    exportTouchOSCLabel(writer, {924, 266, 100, 60}, "red", "AA", 45, assignmentBaseAddr, true, true);
}

void Mapping::exportTouchOSCFaderBank(QXmlStreamWriter* writer)
{
    exportTouchOSCLabel(writer, {810, 344, 104, 20}, "gray", lbFaderBanks.defaultString, 14, lbFaderBanks.addr, true, true);

    TouchOscButtonOption optLed;
    optLed.ledColor = "green";
    optLed.label1Rect = QRect(0, -20, 46, 20);

    TouchOscButtonOption optRect;
    optRect.label1Rect = QRect(0, -20, 104, 20);

    // Banks
    exportTouchOSCMackieControlButton(writer, {810, 384, 46, 30}, otherButtons[QMidi::ASharp_2], optRect);
    exportTouchOSCMackieControlButton(writer, {868, 384, 46, 30}, otherButtons[QMidi::B_2]);

    // Channels
    exportTouchOSCMackieControlButton(writer, {810, 434, 46, 30}, otherButtons[QMidi::C_3], optRect);
    exportTouchOSCMackieControlButton(writer, {868, 434, 46, 30}, otherButtons[QMidi::CSharp_3]);

    // Flip
    exportTouchOSCMackieControlButton(writer, {810, 484, 46, 30}, otherButtons[QMidi::D_3], optLed);

    // Global
    exportTouchOSCMackieControlButton(writer, {868, 484, 46, 30}, otherButtons[QMidi::DSharp_3], optLed);
}

void Mapping::exportTouchOSCMaster(QXmlStreamWriter* writer)
{
    exportTouchOSCLabel(writer, {924, 344, 100, 20}, "red", "Master", 14, lbTrackNum.addr+"9", true, true);
    exportTouchOSCFader(writer, {944, 364, 60, 344}, "red", faderBaseAddr + "9");
}

void Mapping::exportTouchOSCGlobalView(QXmlStreamWriter* writer)
{
    exportTouchOSCLabel(writer, {0, 4, 660, 20}, "gray", lbGlobalView.defaultString, 14, lbGlobalView.addr, true, true);

    for(int n = QMidi::D_4, x = 20; n <= QMidi::A_4; n++, x+=80)
        exportTouchOSCMackieControlButton(writer, {x, 44, 60, 40}, otherButtons[n]);
}

void Mapping::exportTouchOSCFunctions(QXmlStreamWriter* writer)
{
    exportTouchOSCLabel(writer, {0, 124, 660, 20}, "gray", lbFunctions.defaultString, 14, lbFunctions.addr, true, true);

    for(int i = 1, x = 20; i <= 8; i++, x+=80)
    {
        auto n = QString::number(i);
        auto btn = Functions;
        btn.btnAddr += n;
        btn.label1Addr += n;
        btn.defaultLabel1 += n;

        exportTouchOSCMackieControlButton(writer, {x, 164, 60, 40}, btn);
    }
}

void Mapping::exportTouchOSCModifiers(QXmlStreamWriter* writer)
{
    exportTouchOSCLabel(writer, {0, 244, 180, 20}, "gray", lbModifiers.defaultString, 14, lbModifiers.addr, true, true);

    exportTouchOSCMackieControlButton(writer, {20, 284, 60, 40}, otherButtons[QMidi::ASharp_4]);
    exportTouchOSCMackieControlButton(writer, {100, 284, 60, 40}, otherButtons[QMidi::B_4]);
    exportTouchOSCMackieControlButton(writer, {20, 364, 60, 40}, otherButtons[QMidi::C_5]);
    exportTouchOSCMackieControlButton(writer, {100, 364, 60, 40}, otherButtons[QMidi::CSharp_5]);
}

void Mapping::exportTouchOSCUtilities(QXmlStreamWriter* writer)
{
    exportTouchOSCLabel(writer, {480, 244, 180, 20}, "gray", lbUtilities.defaultString, 14, lbUtilities.addr, true, true);

    TouchOscButtonOption opt;
    opt.ledColor = "green";

    exportTouchOSCMackieControlButton(writer, {500, 284, 60, 40}, otherButtons[QMidi::GSharp_5]);
    exportTouchOSCMackieControlButton(writer, {580, 284, 60, 40}, otherButtons[QMidi::A_5], opt);
    exportTouchOSCMackieControlButton(writer, {500, 364, 60, 40}, otherButtons[QMidi::ASharp_5]);
    exportTouchOSCMackieControlButton(writer, {580, 364, 60, 40}, otherButtons[QMidi::B_5]);
}

void Mapping::exportTouchOSCAutomation(QXmlStreamWriter* writer)
{
    exportTouchOSCLabel(writer, {200, 244, 260, 20}, "gray", lbAutomation.defaultString, 14, lbAssigment.addr, true, true);

    TouchOscButtonOption opt;
    opt.ledColor = "green";

    exportTouchOSCMackieControlButton(writer, {220, 284, 60, 40}, otherButtons[QMidi::D_5], opt);
    exportTouchOSCMackieControlButton(writer, {300, 284, 60, 40}, otherButtons[QMidi::DSharp_5]);
    exportTouchOSCMackieControlButton(writer, {380, 284, 60, 40}, otherButtons[QMidi::E_5]);
    exportTouchOSCMackieControlButton(writer, {220, 364, 60, 40}, otherButtons[QMidi::F_5], opt);
    exportTouchOSCMackieControlButton(writer, {300, 364, 60, 40}, otherButtons[QMidi::FSharp_5]);
    exportTouchOSCMackieControlButton(writer, {380, 364, 60, 40}, otherButtons[QMidi::G_5]);
}

void Mapping::exportTouchOSCTransport(QXmlStreamWriter* writer)
{
    exportTouchOSCLabel(writer, {0, 444, 660, 20}, "gray", lbTransport.defaultString, 14, lbTransport.addr, true, true);

    TouchOscButtonOption opt;
    opt.ledColor = "green";

    for(int n = QMidi::C_6, x = 20; n <= QMidi::FSharp_6; n++, x += 80)
    {
        if(n == QMidi::D_6 || n == QMidi::F_6)
            x += 40;

        QRect r(x, 484, 60, 40);

        if(n <= QMidi::D_6 || n == QMidi::F_6)
            exportTouchOSCMackieControlButton(writer, r, otherButtons[n], opt);
        else
            exportTouchOSCMackieControlButton(writer, r, otherButtons[n]);
    }

    TouchOscButtonOption trOpt;
    trOpt.ledColor = "yellow";
    trOpt.ledRect = QRect(40, -20, 20, 20);
    trOpt.label1Rect = QRect(0, 0, 100, 80);
    trOpt.fontSize = 32;

    exportTouchOSCMackieControlButton(writer, {20, 628, 100, 80}, otherButtons[QMidi::G_6], trOpt);
    exportTouchOSCMackieControlButton(writer, {140, 628, 100, 80}, otherButtons[QMidi::GSharp_6], trOpt);
    exportTouchOSCMackieControlButton(writer, {260, 628, 100, 80}, otherButtons[QMidi::A_6], trOpt);

    trOpt.ledColor = "green";
    exportTouchOSCMackieControlButton(writer, {380, 628, 100, 80}, otherButtons[QMidi::ASharp_6], trOpt);

    trOpt.ledColor = "red";
    trOpt.btnColor = "red";
    exportTouchOSCMackieControlButton(writer, {540, 628, 100, 80}, otherButtons[QMidi::B_6], trOpt);
}

void Mapping::exportTouchOSCMovingPart(QXmlStreamWriter* writer)
{
    exportTouchOSCEncoder(writer, {710, 444, 264, 264}, "gray", jogBaseAddr);

    exportTouchOSCMackieControlButton(writer, {944, 444, 60, 40}, otherButtons[QMidi::D_7]);

    exportTouchOSCMackieControlButton(writer, {810, 244, 60, 40}, otherButtons[QMidi::C_7]);
    exportTouchOSCMackieControlButton(writer, {810, 364, 60, 40}, otherButtons[QMidi::CSharp_7]);

    exportTouchOSCMackieControlButton(writer, {730, 304, 60, 40}, otherButtons[QMidi::E_7]);
    exportTouchOSCMackieControlButton(writer, {890, 304, 60, 40}, otherButtons[QMidi::F_7]);

    exportTouchOSCMackieControlButton(writer, {810, 304, 60, 40}, otherButtons[QMidi::DSharp_7]);
}

void Mapping::exportTouchOSCTimecodeAssign(QXmlStreamWriter* writer)
{
    // Timecode
    exportTouchOSCLabel(writer, {680, 4, 324, 20}, "gray", lbTimecode.defaultString, 14, lbTimecode.addr, true, true);

    exportTouchOSCLabel(writer, {790, 24, 214, 60}, "red", " ", 14, "/backgroundTimecode", true, true);

    exportTouchOSCLabel(writer, {790, 24, 24, 60}, "red", "0",  30, timecodeBaseAddr + QString::number(1), false, false);
    exportTouchOSCLabel(writer, {810, 24, 24, 60}, "red", "0",  30, timecodeBaseAddr + QString::number(2), false, false);
    exportTouchOSCLabel(writer, {828, 24, 30, 60}, "red", "0.", 30, timecodeBaseAddr + QString::number(3), false, false);

    exportTouchOSCLabel(writer, {854, 24, 24, 60}, "red", "0",  30, timecodeBaseAddr + QString::number(4), false, false);
    exportTouchOSCLabel(writer, {872, 24, 30, 60}, "red", "0.", 30, timecodeBaseAddr + QString::number(5), false, false);

    exportTouchOSCLabel(writer, {898, 24, 24, 60}, "red", "0",  30, timecodeBaseAddr + QString::number(6), false, false);
    exportTouchOSCLabel(writer, {916, 24, 30, 60}, "red", "0.", 30, timecodeBaseAddr + QString::number(7), false, false);

    exportTouchOSCLabel(writer, {942, 24, 24, 60}, "red", "0",  30, timecodeBaseAddr + QString::number(8), false, false);
    exportTouchOSCLabel(writer, {960, 24, 24, 60}, "red", "0",  30, timecodeBaseAddr + QString::number(9), false, false);
    exportTouchOSCLabel(writer, {978, 24, 24, 60}, "red", "0", 30, timecodeBaseAddr + QString::number(10), false, false);

    auto& smpte = otherButtons[QMidi::F_8];
    exportTouchOSCLabel(writer, {680, 24, 90, 20}, "gray", smpte.defaultLabel1, 12, smpte.label1Addr, false, false);
    exportTouchOSCLed(writer, {770, 27, 14, 14}, "red", smpte.ledAddr);

    auto& beats = otherButtons[QMidi::FSharp_8];
    exportTouchOSCLabel(writer, {680, 44, 90, 20}, "gray", beats.defaultLabel1, 12, beats.label1Addr, false, false);
    exportTouchOSCLed(writer, {770, 47, 14, 14}, "red", beats.ledAddr);

    auto& rude = otherButtons[QMidi::G_8];
    exportTouchOSCLabel(writer, {680, 64, 90, 20}, "gray", rude.defaultLabel1, 12, rude.label1Addr, false, false);
    exportTouchOSCLed(writer, {770, 67, 14, 14}, "red", rude.ledAddr);

    // Display
    exportTouchOSCLabel(writer, {680, 124, 180, 20}, "gray", lbDisplay.defaultString, 14, lbDisplay.addr, true, true);

    // Name Value
    exportTouchOSCMackieControlButton(writer, {700, 164, 60, 40}, otherButtons[QMidi::E_3]);

    // SMPTE Beats
    exportTouchOSCMackieControlButton(writer, {780, 164, 60, 40}, otherButtons[QMidi::F_3]);

    // Assign
    exportTouchOSCLabel(writer, {904, 124, 100, 20}, "gray", lbAssigment.defaultString, 14, lbAssigment.addr, true, true);
    exportTouchOSCLabel(writer, {904, 144, 100, 60}, "red", "AA", 45, assignmentBaseAddr, true, true);
}

void Mapping::exportTouchOSCMackieControlButton(QXmlStreamWriter* writer, const QRect& rect, const ButtonControl& btn, const TouchOscButtonOption &opt)
{
    QString ledColor = opt.ledColor.isEmpty() ? "red" :  opt.ledColor;
    QString btnColor = opt.btnColor.isEmpty() ? "gray" : opt.btnColor;

    QRect ledRect;
    if(!opt.ledRect.isValid())
    {
        int s = rect.height() / 2;

        QPoint tl(rect.center().x()-(s/2), rect.bottom()-s);

        ledRect = QRect(tl, QSize(s, s));
    }
    else
        ledRect = opt.ledRect.translated(rect.topLeft());

    QRect label1Rect;
    if(!opt.label1Rect.isValid())
        label1Rect = QRect(rect.x(), rect.y()+1, rect.width(), rect.height()/2);
    else
        label1Rect = opt.label1Rect.translated(rect.topLeft());

    QRect label2Rect;
    if(!opt.label2Rect.isValid())
        label2Rect = QRect(rect.x(), rect.bottom()-(rect.height()/2+1), rect.width(), rect.height()/2);
    else
        label2Rect = opt.label2Rect.translated(rect.topLeft());

    exportTouchOSCButton(writer, rect, btnColor, btn.btnAddr);
    exportTouchOSCLabel(writer, label1Rect, btnColor, btn.defaultLabel1, opt.fontSize, btn.label1Addr, false, false);

    if(!btn.label2Addr.isEmpty())
        exportTouchOSCLabel(writer, label2Rect, btnColor, btn.defaultLabel2, opt.fontSize, btn.label2Addr, false, false);

    if(!btn.ledAddr.isEmpty())
        exportTouchOSCLed(writer, ledRect, ledColor, btn.ledAddr);
}

void Mapping::exportTouchOSCButton(QXmlStreamWriter* writer, const QRect& rect, const QString& color, const QString& addr)
{
    exportTouchOSCGenericControl(writer, "push", rect, color, addr);

    writer->writeAttribute("local_off", "false");
    writer->writeAttribute("sp", "true");
    writer->writeAttribute("sr", "false");

    writer->writeEndElement();
}

void Mapping::exportTouchOSCLabel(QXmlStreamWriter* writer, const QRect& rect, const QString& color, const QString& text, int size, const QString& addr, bool background, bool outlined)
{
    exportTouchOSCGenericControl(writer, "labelv", rect, color, addr);

    writer->writeAttribute("size", QString::number(size));
    writer->writeAttribute("background", background ? "true" : "false");
    writer->writeAttribute("outline", outlined ? "true" : "false");
    writeBase64(writer, "text", text);

    writer->writeEndElement();
}

void Mapping::exportTouchOSCLed(QXmlStreamWriter* writer, const QRect& rect, const QString& color, const QString& addr)
{
    exportTouchOSCGenericControl(writer, "led", rect, color, addr);
    writer->writeEndElement();
}

void Mapping::exportTouchOSCEncoder(QXmlStreamWriter* writer, const QRect& rect, const QString& color, const QString& addr)
{
    exportTouchOSCGenericControl(writer, "encoder", rect, color, addr);
    writer->writeEndElement();
}

void Mapping::exportTouchOSCFader(QXmlStreamWriter* writer, const QRect& rect, const QString& color, const QString& addr)
{
    exportTouchOSCGenericControl(writer, "faderh", rect, color, addr);
    writer->writeAttribute("response", "absolute");
    writer->writeAttribute("inverted", "false");
    writer->writeAttribute("centered", "false");
    writer->writeEndElement();
}

void Mapping::exportTouchOSCGenericControl(QXmlStreamWriter* writer, const QString& type, const QRect& rect, const QString& color, const QString& addr)
{
    writer->writeStartElement("control");
    writer->writeAttribute("type", type);
    writer->writeAttribute("y", QString::number(rect.x()));
    writer->writeAttribute("x", QString::number(728-(rect.y() + rect.height())));
    writer->writeAttribute("h", QString::number(rect.width()));
    writer->writeAttribute("w", QString::number(rect.height()));
    writer->writeAttribute("color", color);

    if(!type.startsWith("label"))
    {
        writer->writeAttribute("scalef", "0.0");
        writer->writeAttribute("scalet", "1.0");
    }

    writeBase64(writer, "name", addr.mid(1));
    writeBase64(writer, "osc_cs", addr);
}

void Mapping::writeStartPage(QXmlStreamWriter* writer, const QString& name)
{
    writer->writeStartElement("tabpage");
    writeBase64(writer, "name", name);
    writer->writeAttribute("scalef", "0.0");
    writer->writeAttribute("scalet", "1.0");
    writer->writeAttribute("li_t", "");
    writer->writeAttribute("li_c", "gray");
    writer->writeAttribute("li_s", "14");
    writer->writeAttribute("li_o", "false");
    writer->writeAttribute("li_b", "false");
    writer->writeAttribute("la_t", "");
    writer->writeAttribute("la_c", "gray");
    writer->writeAttribute("la_s", "14");
    writer->writeAttribute("la_o", "false");
    writer->writeAttribute("la_b", "false");
}

void Mapping::writeBase64(QXmlStreamWriter* writer, const QString& name, const QString& value)
{
    auto data = value.toUtf8();
    writer->writeAttribute(name, data.toBase64());
}



// =============================================================================



MappingModel::MappingModel(Mapping* m) :
    QAbstractListModel(m),
    _mapping(m)
{

}

QHash<int, QByteArray> MappingModel::roleNames() const
{
    return {
        { TypeRole,          "type"     },
        { NameRole,          "name"     },
        { BaseRole,          "base"     },
        { ButtonAddressRole, "button"   },
        { LedAddressRole,    "led"      },
        { LabelAddressRole,  "label"    },
        { Label2AddressRole, "label2"   },
        { DefaultLabelRole,  "default"  },
        { DefaultLabel2Role, "default2" }
    };
}

int MappingModel::rowCount(const QModelIndex& index) const
{
    Q_UNUSED(index)
    return _data.size();
}

QVariant MappingModel::data(const QModelIndex& index, int role) const
{
    if(!index.isValid())
        return QString{};

    int r = index.row();

    if(r < 0 || r >= _data.size())
        return QString{};

    auto& d = _data[r];

    switch(role)
    {
    case TypeRole:
        return d.type;

    case NameRole:
        return d.name;

    case BaseRole:
        return d._base ? *d._base : QString();

    case ButtonAddressRole:
        return d._button ? d._button->btnAddr : QString();

    case LedAddressRole:
        return d._button ? d._button->ledAddr : QString();

    case LabelAddressRole:
        if(d._button)
            return d._button->label1Addr;
        else if(d._label)
            return d._label->addr;
        else
            return QString{};

    case Label2AddressRole:
        return d._button ? d._button->label2Addr : QString();

    case DefaultLabelRole:
        if(d._button)
            return d._button->defaultLabel1;
        else if(d._label)
            return d._label->defaultString;
        else
            return QString{};

    case DefaultLabel2Role:
        return d._button ? d._button->defaultLabel2 : QString();

    default:
        return QString{};
    }
}

bool MappingModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if(!index.isValid())
        return false;

    int r = index.row();

    if(r < 0 || r >= _data.size())
        return false;

    auto& d = _data[r];

    switch(role)
    {
    case BaseRole:
        if(d._base)
        {
            *d._base = value.toString();
            emit dataChanged(index, index, {role});
            return true;
        }
        else
            return false;

    case ButtonAddressRole:
        if(d._button)
        {
            d._button->btnAddr = value.toString();
            emit dataChanged(index, index, {role});
            return true;
        }
        else
            return false;

    case LedAddressRole:
        if(d._button)
        {
            d._button->ledAddr = value.toString();
            emit dataChanged(index, index, {role});
            return true;
        }
        else
            return false;

    case LabelAddressRole:
        if(d._button)
        {
            d._button->label1Addr = value.toString();
            emit dataChanged(index, index, {role});
            return true;
        }
        else if(d._label)
        {
            d._label->addr = value.toString();
            emit dataChanged(index, index, {role});
            return true;
        }
        else
            return false;

    case Label2AddressRole:
        if(d._button)
        {
            d._button->label2Addr = value.toString();
            emit dataChanged(index, index, {role});
            return true;
        }
        else
            return false;

    case DefaultLabelRole:
        if(d._button)
        {
            d._button->defaultLabel1 = value.toString();
            emit dataChanged(index, index, {role});
            return true;
        }
        else if(d._label)
        {
            d._label->defaultString = value.toString();
            emit dataChanged(index, index, {role});
            return true;
        }
        else
            return false;

    case DefaultLabel2Role:
        if(d._button)
        {
            d._button->defaultLabel2 = value.toString();
            emit dataChanged(index, index, {role});
            return true;
        }
        else
            return false;

    default:
        return false;
    }
}

Qt::ItemFlags MappingModel::flags(const QModelIndex& index) const
{
    Q_UNUSED(index)
    return Qt::ItemIsEnabled | Qt::ItemIsEditable;
}

void MappingModel::loadLCD()
{
    beginResetModel();
    _data.clear();

    load("LCD address", &_mapping->lcdBaseAddr);
    load("LCD lines base address", &_mapping->lcdLineBaseAddr);
    load("LCD tracks base address", &_mapping->lcdTrackBaseAddr);
    load("LCD character base address", &_mapping->lcdCharBaseAddr);

    endResetModel();
}

void MappingModel::loadDisplays()
{
    beginResetModel();
    _data.clear();

    load("Assignment Label address", &_mapping->lbAssigment);
    load("Assignment base address", &_mapping->assignmentBaseAddr);
    load("Timecode Label address", &_mapping->lbTimecode);
    load("Timecode base address", &_mapping->timecodeBaseAddr);
    load("SMPTE Led", &_mapping->otherButtons[QMidi::F_8].ledAddr);
    load("BEATS Led", &_mapping->otherButtons[QMidi::FSharp_8].ledAddr);
    load("RUDE SOLO Led", &_mapping->otherButtons[QMidi::G_8].ledAddr);

    endResetModel();
}

void MappingModel::loadvPot()
{
    beginResetModel();
    _data.clear();

    load("VPot encoder base address", &_mapping->VPOTBaseAddr);
    load("VPot led ring base address", &_mapping->VPOTLedBaseAddr);
    load("VPot button click base address", &_mapping->VPOTSelect.btnAddr);

    endResetModel();
}

void MappingModel::loadButtons()
{
    beginResetModel();
    _data.clear();

    load("REC buttons base addresses", &_mapping->REC);
    load("MUTE buttons base addresses", &_mapping->MUTE);
    load("SOLO buttons base addresses", &_mapping->SOLO);
    load("SELECT buttons base addresses", &_mapping->SEL);

    endResetModel();
}

void MappingModel::loadFaders()
{
    beginResetModel();
    _data.clear();

    load("Track label base address", &_mapping->lbTrackNum);
    load("Faders base address", &_mapping->faderBaseAddr);
    load("Vu meter base address", &_mapping->vuMeterBaseAddr);

    endResetModel();
}

void MappingModel::loadAssignment()
{
    beginResetModel();
    _data.clear();

    load("VPot Assign Label", &_mapping->lbVPOTAssign);

    load("Track", &_mapping->otherButtons[QMidi::E_2]);
    load("Send", &_mapping->otherButtons[QMidi::F_2]);
    load("Pan", &_mapping->otherButtons[QMidi::FSharp_2]);
    load("Plugin", &_mapping->otherButtons[QMidi::G_2]);
    load("EQ", &_mapping->otherButtons[QMidi::GSharp_2]);
    load("Instrument", &_mapping->otherButtons[QMidi::A_2]);

    load("Fader Banks Label", &_mapping->lbFaderBanks);

    load("Bank Left", &_mapping->otherButtons[QMidi::ASharp_2]);
    load("Bank Right", &_mapping->otherButtons[QMidi::B_2]);
    load("Channel Left", &_mapping->otherButtons[QMidi::C_3]);
    load("Channel Right", &_mapping->otherButtons[QMidi::CSharp_3]);
    load("Flip", &_mapping->otherButtons[QMidi::D_3]);
    load("Global", &_mapping->otherButtons[QMidi::DSharp_3]);

    endResetModel();
}

void MappingModel::loadGlobal()
{
    beginResetModel();
    _data.clear();

    load("Display Label", &_mapping->lbDisplay);

    load("Name / Value", &_mapping->otherButtons[QMidi::E_3]);
    load("SMPTE / BEATS", &_mapping->otherButtons[QMidi::F_3]);

    load("Function Label", &_mapping->lbFunctions);
    load("Function button base address", &_mapping->Functions);

    load("Global View Label", &_mapping->lbGlobalView);
    load("Midi Tracks", &_mapping->otherButtons[QMidi::D_4]);
    load("Input Tracks", &_mapping->otherButtons[QMidi::DSharp_4]);
    load("Audio Tracks", &_mapping->otherButtons[QMidi::E_4]);
    load("Audio Inst Tracks", &_mapping->otherButtons[QMidi::F_4]);
    load("Aux Tracks", &_mapping->otherButtons[QMidi::FSharp_4]);
    load("Bus Tracks", &_mapping->otherButtons[QMidi::G_4]);
    load("Output Tracks", &_mapping->otherButtons[QMidi::GSharp_4]);
    load("User", &_mapping->otherButtons[QMidi::A_4]);

    endResetModel();
}

void MappingModel::loadAutomation()
{
    beginResetModel();
    _data.clear();

    load("Modifiers Label", &_mapping->lbModifiers);

    load("Shift", &_mapping->otherButtons[QMidi::ASharp_4]);
    load("Option", &_mapping->otherButtons[QMidi::B_4]);
    load("Control", &_mapping->otherButtons[QMidi::C_5]);
    load("Alt", &_mapping->otherButtons[QMidi::CSharp_5]);

    load("Automation Label", &_mapping->lbAutomation);

    load("Read/Off", &_mapping->otherButtons[QMidi::D_5]);
    load("Write", &_mapping->otherButtons[QMidi::DSharp_5]);
    load("Trim", &_mapping->otherButtons[QMidi::E_5]);
    load("Touch", &_mapping->otherButtons[QMidi::F_5]);
    load("Latch", &_mapping->otherButtons[QMidi::FSharp_5]);
    load("Groups", &_mapping->otherButtons[QMidi::G_5]);

    load("Utilities Label", &_mapping->lbUtilities);

    load("Save", &_mapping->otherButtons[QMidi::GSharp_5]);
    load("Undo", &_mapping->otherButtons[QMidi::A_5]);
    load("Cancel", &_mapping->otherButtons[QMidi::ASharp_5]);
    load("Enter", &_mapping->otherButtons[QMidi::B_5]);

    endResetModel();
}

void MappingModel::loadTransport()
{
    beginResetModel();
    _data.clear();

    load("Transport Label", &_mapping->lbTransport);

    load("Markers", &_mapping->otherButtons[QMidi::C_6]);
    load("Nudge", &_mapping->otherButtons[QMidi::CSharp_6]);
    load("Cycle", &_mapping->otherButtons[QMidi::D_6]);
    load("Drop", &_mapping->otherButtons[QMidi::DSharp_6]);
    load("Replace", &_mapping->otherButtons[QMidi::E_6]);
    load("Click", &_mapping->otherButtons[QMidi::F_6]);
    load("Solo", &_mapping->otherButtons[QMidi::FSharp_6]);

    load("Rewind", &_mapping->otherButtons[QMidi::G_6]);
    load("Forward", &_mapping->otherButtons[QMidi::GSharp_6]);
    load("Stop", &_mapping->otherButtons[QMidi::A_6]);
    load("Play", &_mapping->otherButtons[QMidi::ASharp_6]);
    load("Rec", &_mapping->otherButtons[QMidi::B_6]);

    load("Up", &_mapping->otherButtons[QMidi::C_7]);
    load("Down", &_mapping->otherButtons[QMidi::CSharp_7]);
    load("Scrub", &_mapping->otherButtons[QMidi::D_7]);
    load("Zoom", &_mapping->otherButtons[QMidi::DSharp_7]);
    load("Left", &_mapping->otherButtons[QMidi::E_7]);
    load("Right", &_mapping->otherButtons[QMidi::F_7]);

    load("Jog", &_mapping->jogBaseAddr);

    endResetModel();
}

void MappingModel::load(const QString& name, QString* base)
{
    Data d;
    d.type = "base";
    d.name = name;
    d._base = base;

    _data << d;
}

void MappingModel::load(const QString& name, ButtonControl* btn)
{
    Data d;
    d.type = "button";
    d.name = name;
    d._button = btn;

    _data << d;
}

void MappingModel::load(const QString& name, LabelControl* lb)
{
    Data d;
    d.type = "label";
    d.name = name;
    d._label = lb;

    _data << d;
}
