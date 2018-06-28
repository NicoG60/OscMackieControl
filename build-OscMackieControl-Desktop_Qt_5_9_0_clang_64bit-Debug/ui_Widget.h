/********************************************************************************
** Form generated from reading UI file 'Widget.ui'
**
** Created by: Qt User Interface Compiler version 5.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QHBoxLayout *horizontalLayout;
    QGroupBox *groupBox;
    QFormLayout *formLayout;
    QLabel *label;
    QLineEdit *leHost;
    QLabel *label_2;
    QSpinBox *sbOutPort;
    QLabel *label_3;
    QSpinBox *sbInPort;
    QPushButton *btnConnectOSC;
    QGroupBox *groupBox_2;
    QFormLayout *formLayout_2;
    QLabel *label_5;
    QComboBox *cbInPort;
    QFrame *line;
    QLabel *label_7;
    QComboBox *cbOutPort;
    QPushButton *btnConnectMidi;
    QGroupBox *groupBox_3;
    QVBoxLayout *verticalLayout;
    QComboBox *cb_overlays;
    QLabel *status;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QStringLiteral("Widget"));
        Widget->resize(704, 187);
        horizontalLayout = new QHBoxLayout(Widget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        groupBox = new QGroupBox(Widget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        formLayout = new QFormLayout(groupBox);
        formLayout->setSpacing(6);
        formLayout->setContentsMargins(11, 11, 11, 11);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label);

        leHost = new QLineEdit(groupBox);
        leHost->setObjectName(QStringLiteral("leHost"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(leHost->sizePolicy().hasHeightForWidth());
        leHost->setSizePolicy(sizePolicy);

        formLayout->setWidget(0, QFormLayout::FieldRole, leHost);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QStringLiteral("label_2"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_2);

        sbOutPort = new QSpinBox(groupBox);
        sbOutPort->setObjectName(QStringLiteral("sbOutPort"));
        sbOutPort->setMaximum(65535);
        sbOutPort->setValue(9000);

        formLayout->setWidget(1, QFormLayout::FieldRole, sbOutPort);

        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QStringLiteral("label_3"));

        formLayout->setWidget(2, QFormLayout::LabelRole, label_3);

        sbInPort = new QSpinBox(groupBox);
        sbInPort->setObjectName(QStringLiteral("sbInPort"));
        sbInPort->setMaximum(65535);
        sbInPort->setValue(8000);

        formLayout->setWidget(2, QFormLayout::FieldRole, sbInPort);

        btnConnectOSC = new QPushButton(groupBox);
        btnConnectOSC->setObjectName(QStringLiteral("btnConnectOSC"));

        formLayout->setWidget(3, QFormLayout::FieldRole, btnConnectOSC);


        horizontalLayout->addWidget(groupBox);

        groupBox_2 = new QGroupBox(Widget);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        formLayout_2 = new QFormLayout(groupBox_2);
        formLayout_2->setSpacing(6);
        formLayout_2->setContentsMargins(11, 11, 11, 11);
        formLayout_2->setObjectName(QStringLiteral("formLayout_2"));
        label_5 = new QLabel(groupBox_2);
        label_5->setObjectName(QStringLiteral("label_5"));

        formLayout_2->setWidget(0, QFormLayout::LabelRole, label_5);

        cbInPort = new QComboBox(groupBox_2);
        cbInPort->setObjectName(QStringLiteral("cbInPort"));

        formLayout_2->setWidget(0, QFormLayout::FieldRole, cbInPort);

        line = new QFrame(groupBox_2);
        line->setObjectName(QStringLiteral("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        formLayout_2->setWidget(1, QFormLayout::FieldRole, line);

        label_7 = new QLabel(groupBox_2);
        label_7->setObjectName(QStringLiteral("label_7"));

        formLayout_2->setWidget(2, QFormLayout::LabelRole, label_7);

        cbOutPort = new QComboBox(groupBox_2);
        cbOutPort->setObjectName(QStringLiteral("cbOutPort"));

        formLayout_2->setWidget(2, QFormLayout::FieldRole, cbOutPort);

        btnConnectMidi = new QPushButton(groupBox_2);
        btnConnectMidi->setObjectName(QStringLiteral("btnConnectMidi"));

        formLayout_2->setWidget(3, QFormLayout::FieldRole, btnConnectMidi);


        horizontalLayout->addWidget(groupBox_2);

        groupBox_3 = new QGroupBox(Widget);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        verticalLayout = new QVBoxLayout(groupBox_3);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        cb_overlays = new QComboBox(groupBox_3);
        cb_overlays->setObjectName(QStringLiteral("cb_overlays"));

        verticalLayout->addWidget(cb_overlays);

        status = new QLabel(groupBox_3);
        status->setObjectName(QStringLiteral("status"));

        verticalLayout->addWidget(status);


        horizontalLayout->addWidget(groupBox_3);


        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QApplication::translate("Widget", "Mackie Control to OSC Bridge", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("Widget", "OSC", Q_NULLPTR));
        label->setText(QApplication::translate("Widget", "Host", Q_NULLPTR));
        leHost->setText(QApplication::translate("Widget", "192.168.1.20", Q_NULLPTR));
        label_2->setText(QApplication::translate("Widget", "Port (outgoing)", Q_NULLPTR));
        label_3->setText(QApplication::translate("Widget", "Port (Incoming)", Q_NULLPTR));
        btnConnectOSC->setText(QApplication::translate("Widget", "Connect", Q_NULLPTR));
        groupBox_2->setTitle(QApplication::translate("Widget", "MIDI", Q_NULLPTR));
        label_5->setText(QApplication::translate("Widget", "In Port", Q_NULLPTR));
        label_7->setText(QApplication::translate("Widget", "Out Port", Q_NULLPTR));
        btnConnectMidi->setText(QApplication::translate("Widget", "Apply", Q_NULLPTR));
        groupBox_3->setTitle(QApplication::translate("Widget", "Status", Q_NULLPTR));
        status->setText(QApplication::translate("Widget", "TextLabel", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
