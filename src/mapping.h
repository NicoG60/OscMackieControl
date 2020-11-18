#ifndef MAPPING_H
#define MAPPING_H

#include <QString>
#include <QList>
#include <QJsonObject>

class Mapping
{
public:
    Mapping();

    struct ButtonControl
    {
        ButtonControl() = default;

        ButtonControl(quint8 note, QString addrIn, QString addrOut) :
            note(note), addrIn(addrIn), addrOut(addrOut) {}

        ButtonControl(quint8 note, QString addrOut) :
            note(note), addrOut(addrOut) {}

        quint8  note;
        QString addrIn;
        QString addrOut;
    };

    ButtonControl REC;
    ButtonControl SOLO;
    ButtonControl MUTE;
    ButtonControl SEL;
    ButtonControl VPOTSelect;
    ButtonControl Function;

    QList<ButtonControl> otherButtons;

    QString vPotLedBaseAddr;
    QString timecodeBaseAddr;
    QString vuMeterBaseAddr;
    QString faderBaseAddr;
    QString jogBaseAddr;

    QString charBaseAddr;
    QString trackDisplayBaseAddr;
    QString lcdBaseAddr;
    QString lcdLineBaseAddr;

    void loadDefault();
    void loadFromJson(const QJsonObject& obj);

    QJsonObject dumpJson();
};

#endif // MAPPING_H
