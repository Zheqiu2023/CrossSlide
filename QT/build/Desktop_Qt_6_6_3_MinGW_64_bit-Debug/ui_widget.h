/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 6.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QGroupBox *groupBox_19;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_66;
    QComboBox *port_name;
    QHBoxLayout *horizontalLayout_10;
    QLabel *label_65;
    QComboBox *baudrate;
    QPushButton *openSerialPort;
    QGroupBox *groupBox;
    QWidget *layoutWidget1;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label_4;
    QSpinBox *Jog_Vel;
    QLabel *label_5;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QDoubleSpinBox *XPos;
    QLabel *label_6;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_2;
    QDoubleSpinBox *YPos;
    QLabel *label_7;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_3;
    QDoubleSpinBox *ZPos;
    QLabel *label_8;
    QWidget *layoutWidget2;
    QVBoxLayout *verticalLayout_4;
    QPushButton *startJog;
    QPushButton *stopJog;
    QGroupBox *groupBox_3;
    QWidget *layoutWidget3;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_33;
    QDoubleSpinBox *trajPeriod;
    QHBoxLayout *horizontalLayout_4;
    QComboBox *traj;
    QPushButton *readTraj;
    QHBoxLayout *horizontalLayout_7;
    QPushButton *startTraj;
    QPushButton *stopTraj;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName("Widget");
        Widget->resize(800, 600);
        groupBox_19 = new QGroupBox(Widget);
        groupBox_19->setObjectName("groupBox_19");
        groupBox_19->setGeometry(QRect(100, 40, 151, 131));
        layoutWidget = new QWidget(groupBox_19);
        layoutWidget->setObjectName("layoutWidget");
        layoutWidget->setGeometry(QRect(10, 30, 130, 90));
        verticalLayout_2 = new QVBoxLayout(layoutWidget);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        label_66 = new QLabel(layoutWidget);
        label_66->setObjectName("label_66");
        QFont font;
        font.setFamilies({QString::fromUtf8("AR PL UKai CN")});
        label_66->setFont(font);

        horizontalLayout_5->addWidget(label_66);

        port_name = new QComboBox(layoutWidget);
        port_name->setObjectName("port_name");

        horizontalLayout_5->addWidget(port_name);


        verticalLayout_2->addLayout(horizontalLayout_5);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName("horizontalLayout_10");
        label_65 = new QLabel(layoutWidget);
        label_65->setObjectName("label_65");
        label_65->setFont(font);

        horizontalLayout_10->addWidget(label_65);

        baudrate = new QComboBox(layoutWidget);
        baudrate->addItem(QString());
        baudrate->addItem(QString());
        baudrate->addItem(QString());
        baudrate->setObjectName("baudrate");

        horizontalLayout_10->addWidget(baudrate);


        verticalLayout_2->addLayout(horizontalLayout_10);

        openSerialPort = new QPushButton(layoutWidget);
        openSerialPort->setObjectName("openSerialPort");

        verticalLayout_2->addWidget(openSerialPort);

        groupBox = new QGroupBox(Widget);
        groupBox->setObjectName("groupBox");
        groupBox->setGeometry(QRect(270, 60, 251, 141));
        layoutWidget1 = new QWidget(groupBox);
        layoutWidget1->setObjectName("layoutWidget1");
        layoutWidget1->setGeometry(QRect(10, 10, 139, 124));
        verticalLayout = new QVBoxLayout(layoutWidget1);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        label_4 = new QLabel(layoutWidget1);
        label_4->setObjectName("label_4");

        horizontalLayout->addWidget(label_4);

        Jog_Vel = new QSpinBox(layoutWidget1);
        Jog_Vel->setObjectName("Jog_Vel");
        Jog_Vel->setMinimum(-10000);
        Jog_Vel->setMaximum(10000);
        Jog_Vel->setValue(3000);

        horizontalLayout->addWidget(Jog_Vel);

        label_5 = new QLabel(layoutWidget1);
        label_5->setObjectName("label_5");

        horizontalLayout->addWidget(label_5);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        label = new QLabel(layoutWidget1);
        label->setObjectName("label");

        horizontalLayout_2->addWidget(label);

        XPos = new QDoubleSpinBox(layoutWidget1);
        XPos->setObjectName("XPos");
        XPos->setDecimals(3);
        XPos->setMinimum(-500.000000000000000);
        XPos->setMaximum(500.000000000000000);
        XPos->setSingleStep(1.000000000000000);

        horizontalLayout_2->addWidget(XPos);

        label_6 = new QLabel(layoutWidget1);
        label_6->setObjectName("label_6");

        horizontalLayout_2->addWidget(label_6);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName("horizontalLayout_6");
        label_2 = new QLabel(layoutWidget1);
        label_2->setObjectName("label_2");

        horizontalLayout_6->addWidget(label_2);

        YPos = new QDoubleSpinBox(layoutWidget1);
        YPos->setObjectName("YPos");
        YPos->setDecimals(3);
        YPos->setMinimum(-500.000000000000000);
        YPos->setMaximum(500.000000000000000);
        YPos->setSingleStep(1.000000000000000);

        horizontalLayout_6->addWidget(YPos);

        label_7 = new QLabel(layoutWidget1);
        label_7->setObjectName("label_7");

        horizontalLayout_6->addWidget(label_7);


        verticalLayout->addLayout(horizontalLayout_6);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        label_3 = new QLabel(layoutWidget1);
        label_3->setObjectName("label_3");

        horizontalLayout_3->addWidget(label_3);

        ZPos = new QDoubleSpinBox(layoutWidget1);
        ZPos->setObjectName("ZPos");
        ZPos->setDecimals(3);
        ZPos->setMinimum(-500.000000000000000);
        ZPos->setMaximum(500.000000000000000);
        ZPos->setSingleStep(1.000000000000000);

        horizontalLayout_3->addWidget(ZPos);

        label_8 = new QLabel(layoutWidget1);
        label_8->setObjectName("label_8");

        horizontalLayout_3->addWidget(label_8);


        verticalLayout->addLayout(horizontalLayout_3);

        layoutWidget2 = new QWidget(groupBox);
        layoutWidget2->setObjectName("layoutWidget2");
        layoutWidget2->setGeometry(QRect(160, 40, 82, 56));
        verticalLayout_4 = new QVBoxLayout(layoutWidget2);
        verticalLayout_4->setObjectName("verticalLayout_4");
        verticalLayout_4->setContentsMargins(0, 0, 0, 0);
        startJog = new QPushButton(layoutWidget2);
        startJog->setObjectName("startJog");
        startJog->setFocusPolicy(Qt::StrongFocus);

        verticalLayout_4->addWidget(startJog);

        stopJog = new QPushButton(layoutWidget2);
        stopJog->setObjectName("stopJog");
        stopJog->setFocusPolicy(Qt::StrongFocus);

        verticalLayout_4->addWidget(stopJog);

        groupBox_3 = new QGroupBox(Widget);
        groupBox_3->setObjectName("groupBox_3");
        groupBox_3->setGeometry(QRect(100, 210, 191, 141));
        layoutWidget3 = new QWidget(groupBox_3);
        layoutWidget3->setObjectName("layoutWidget3");
        layoutWidget3->setGeometry(QRect(12, 32, 171, 101));
        verticalLayout_3 = new QVBoxLayout(layoutWidget3);
        verticalLayout_3->setObjectName("verticalLayout_3");
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName("horizontalLayout_8");
        label_33 = new QLabel(layoutWidget3);
        label_33->setObjectName("label_33");
        label_33->setFont(font);

        horizontalLayout_8->addWidget(label_33);

        trajPeriod = new QDoubleSpinBox(layoutWidget3);
        trajPeriod->setObjectName("trajPeriod");
        trajPeriod->setDecimals(3);
        trajPeriod->setMaximum(10.000000000000000);
        trajPeriod->setSingleStep(0.100000000000000);
        trajPeriod->setValue(0.020000000000000);

        horizontalLayout_8->addWidget(trajPeriod);


        verticalLayout_3->addLayout(horizontalLayout_8);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        traj = new QComboBox(layoutWidget3);
        traj->addItem(QString());
        traj->setObjectName("traj");

        horizontalLayout_4->addWidget(traj);

        readTraj = new QPushButton(layoutWidget3);
        readTraj->setObjectName("readTraj");

        horizontalLayout_4->addWidget(readTraj);


        verticalLayout_3->addLayout(horizontalLayout_4);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName("horizontalLayout_7");
        startTraj = new QPushButton(layoutWidget3);
        startTraj->setObjectName("startTraj");

        horizontalLayout_7->addWidget(startTraj);

        stopTraj = new QPushButton(layoutWidget3);
        stopTraj->setObjectName("stopTraj");

        horizontalLayout_7->addWidget(stopTraj);


        verticalLayout_3->addLayout(horizontalLayout_7);


        retranslateUi(Widget);

        baudrate->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QCoreApplication::translate("Widget", "Widget", nullptr));
        groupBox_19->setTitle(QCoreApplication::translate("Widget", "\344\270\262\345\217\243", nullptr));
        label_66->setText(QCoreApplication::translate("Widget", "\344\270\262\345\217\243\345\217\267\357\274\232", nullptr));
        label_65->setText(QCoreApplication::translate("Widget", "\346\263\242\347\211\271\347\216\207\357\274\232", nullptr));
        baudrate->setItemText(0, QCoreApplication::translate("Widget", "115200", nullptr));
        baudrate->setItemText(1, QCoreApplication::translate("Widget", "500000", nullptr));
        baudrate->setItemText(2, QString());

        openSerialPort->setText(QCoreApplication::translate("Widget", "\346\211\223\345\274\200", nullptr));
        groupBox->setTitle(QString());
        label_4->setText(QCoreApplication::translate("Widget", "VEL : ", nullptr));
        label_5->setText(QCoreApplication::translate("Widget", "rpm", nullptr));
        label->setText(QCoreApplication::translate("Widget", "X : ", nullptr));
        label_6->setText(QCoreApplication::translate("Widget", "mm", nullptr));
        label_2->setText(QCoreApplication::translate("Widget", "Y : ", nullptr));
        label_7->setText(QCoreApplication::translate("Widget", "mm", nullptr));
        label_3->setText(QCoreApplication::translate("Widget", "Z : ", nullptr));
        label_8->setText(QCoreApplication::translate("Widget", "mm", nullptr));
        startJog->setText(QCoreApplication::translate("Widget", "\345\274\200\345\247\213", nullptr));
        stopJog->setText(QCoreApplication::translate("Widget", "\345\201\234\346\255\242", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("Widget", "\350\275\250\350\277\271", nullptr));
        label_33->setText(QCoreApplication::translate("Widget", "<html><head/><body><p align=\"center\">\345\221\250\346\234\237:</p></body></html>", nullptr));
        trajPeriod->setSuffix(QCoreApplication::translate("Widget", "s", nullptr));
        traj->setItemText(0, QCoreApplication::translate("Widget", "Robot_CoM", nullptr));

        readTraj->setText(QCoreApplication::translate("Widget", "\350\257\273\345\217\226\350\275\250\350\277\271", nullptr));
        startTraj->setText(QCoreApplication::translate("Widget", "\345\274\200\345\247\213", nullptr));
        stopTraj->setText(QCoreApplication::translate("Widget", "\345\201\234\346\255\242", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
