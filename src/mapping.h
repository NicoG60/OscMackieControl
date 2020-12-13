#ifndef MAPPING_H
#define MAPPING_H

#include <QObject>
#include <QString>
#include <QList>
#include <QJsonObject>
#include <QXmlStreamWriter>
#include <QRect>
#include <QAbstractListModel>

struct TouchOscButtonOption
{
    QString ledColor;
    QString btnColor;
    QRect ledRect;
    QRect label1Rect;
    QRect label2Rect;
    int fontSize = 12;
};

struct LabelControl
{
    LabelControl() = default;
    LabelControl(const QString& addr, const QString& str) :
        addr(addr), defaultString(str)
    {}
    QString addr;
    QString defaultString;
};

struct ButtonControl
{
    ButtonControl() = default;

    quint8  note;
    QString btnAddr;
    QString ledAddr;
    QString label1Addr;
    QString label2Addr;
    QString defaultLabel1;
    QString defaultLabel2;
};

class MappingModel;
class Mapping : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QVariantMap mapping READ mapping WRITE setMapping NOTIFY mappingChanged)

public:
    explicit Mapping(QObject* parent = nullptr);

    ButtonControl REC;
    ButtonControl SOLO;
    ButtonControl MUTE;
    ButtonControl SEL;
    ButtonControl VPOTSelect;
    ButtonControl Functions;

    QMap<quint8, ButtonControl> otherButtons;

    QString VPOTBaseAddr;
    QString VPOTLedBaseAddr;
    QString timecodeBaseAddr;
    QString vuMeterBaseAddr;
    QString faderBaseAddr;
    QString jogBaseAddr;
    QString assignmentAddr;

    QString lcdCharBaseAddr;
    QString lcdTrackBaseAddr;
    QString lcdBaseAddr;
    QString lcdLineBaseAddr;

    LabelControl lbTrackNum;
    LabelControl lbVPOTAssign;
    LabelControl lbDisplay;
    LabelControl lbAssigment;
    LabelControl lbFaderBanks;
    LabelControl lbGlobalView;
    LabelControl lbFunctions;
    LabelControl lbModifiers;
    LabelControl lbUtilities;
    LabelControl lbAutomation;
    LabelControl lbTransport;
    LabelControl lbTimecode;

    void loadDefault();
    void loadFromJson(const QJsonObject& obj);

    QJsonObject dumpJson() const;

    QVariantMap mapping() const;
    void setMapping(const QVariantMap& m);

    Q_INVOKABLE MappingModel* createModel();

signals:
    void mappingChanged();

public:
    QJsonObject toJson(const ButtonControl& btn) const;
    QJsonObject toJson(const LabelControl& lb) const;
    ButtonControl toButton(const QJsonObject& obj) const;
    LabelControl toLabel(const QJsonObject& obj) const;
    void exportTouchOSCLayout(QIODevice* dev, bool in_zip = false);

private:
    ButtonControl createButton(quint8 note, const QString& name, const QString& label = {}, const QString& label2 = {});
    void registerButton(quint8 note, const QString& name, const QString& label = {}, const QString& label2 = {});
    void registerButtonLed(quint8 note, const QString& name, const QString& label = {}, const QString& label2 = {});

private:
    void exportTouchOSCTrack(QXmlStreamWriter* writer, quint8 track);
    void exportTouchOSCLCD(QXmlStreamWriter* writer);
    void exportTouchOSCTimecode(QXmlStreamWriter* writer);
    void exportTouchOSCAssign(QXmlStreamWriter* writer);
    void exportTouchOSCFaderBank(QXmlStreamWriter* writer);
    void exportTouchOSCMaster(QXmlStreamWriter* writer);
    void exportTouchOSCGlobalView(QXmlStreamWriter* writer);
    void exportTouchOSCFunctions(QXmlStreamWriter* writer);
    void exportTouchOSCModifiers(QXmlStreamWriter* writer);
    void exportTouchOSCUtilities(QXmlStreamWriter* writer);
    void exportTouchOSCAutomation(QXmlStreamWriter* writer);
    void exportTouchOSCTransport(QXmlStreamWriter* writer);
    void exportTouchOSCMovingPart(QXmlStreamWriter* writer);
    void exportTouchOSCTimecodeAssign(QXmlStreamWriter* writer);

    void exportTouchOSCMackieControlButton(QXmlStreamWriter* writer, const QRect& rect, const ButtonControl& btn, const TouchOscButtonOption& opt = {});
    void exportTouchOSCButton(QXmlStreamWriter* writer, const QRect& rect, const QString& color, const QString& addr);
    void exportTouchOSCLabel(QXmlStreamWriter* writer, const QRect& rect, const QString& color, const QString& text, int size, const QString& addr, bool background, bool outlined);
    void exportTouchOSCLed(QXmlStreamWriter* writer, const QRect& rect, const QString& color, const QString& addr);
    void exportTouchOSCEncoder(QXmlStreamWriter* writer, const QRect& rect, const QString& color, const QString& addr);
    void exportTouchOSCFader(QXmlStreamWriter* writer, const QRect& rect, const QString& color, const QString& addr);
    void exportTouchOSCGenericControl(QXmlStreamWriter* writer, const QString& type, const QRect& rect, const QString& color, const QString& addr);
    void writeStartPage(QXmlStreamWriter* writer, const QString& name);
    void writeBase64(QXmlStreamWriter* writer, const QString& name, const QString& value);
};

class MappingModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum MappingRole {
        TypeRole = Qt::UserRole+1,
        NameRole,
        HintRole,
        BaseRole,
        ButtonAddressRole,
        LedAddressRole,
        LabelAddressRole,
        Label2AddressRole,
        DefaultLabelRole,
        DefaultLabel2Role
    };
    Q_ENUM(MappingRole)

public:
    MappingModel(Mapping* m);

    QHash<int, QByteArray> roleNames() const override;

    int rowCount(const QModelIndex& index = {}) const override;

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index = {}) const override;

public slots:
    void loadLCD();
    void loadDisplays();
    void loadvPot();
    void loadButtons();
    void loadFaders();
    void loadAssignment();
    void loadGlobal();
    void loadAutomation();
    void loadTransport();

private:
    void load(const QString& type, const QString& name, const QString& hint, QString* base);
    void load(const QString& type, const QString& name, const QString& hint, ButtonControl* btn);
    void load(const QString& type, const QString& name, const QString& hint, LabelControl* lb);

private:
    struct Data {
        QString type;
        QString name;
        QString hint;

        QString*       _base   = nullptr;
        ButtonControl* _button = nullptr;
        LabelControl*  _label  = nullptr;
    };

    Mapping* _mapping;
    QList<Data> _data;
};

#endif // MAPPING_H
