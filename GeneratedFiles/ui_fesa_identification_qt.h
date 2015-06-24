/********************************************************************************
** Form generated from reading UI file 'fesa_identification_qt.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FESA_IDENTIFICATION_QT_H
#define UI_FESA_IDENTIFICATION_QT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCommandLinkButton>
#include <QtGui/QFrame>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QProgressBar>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Fesa_Identification_QtClass
{
public:
    QFrame *line;
    QFrame *line_2;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer_8;
    QCommandLinkButton *commandLinkButton;
    QSpacerItem *verticalSpacer_2;
    QCommandLinkButton *commandLinkButton_2;
    QSpacerItem *verticalSpacer;
    QCommandLinkButton *commandLinkButton_3;
    QSpacerItem *verticalSpacer_7;
    QWidget *verticalLayoutWidget_2;
    QVBoxLayout *verticalLayout_2;
    QLabel *label;
    QWidget *verticalLayoutWidget_6;
    QVBoxLayout *verticalLayout_6;
    QGroupBox *groupBox;
    QWidget *verticalLayoutWidget_7;
    QVBoxLayout *verticalLayout_7;
    QLabel *label_3;
    QLabel *label_4;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QLabel *label_5;
    QWidget *verticalLayoutWidget_5;
    QVBoxLayout *verticalLayout_5;
    QProgressBar *progressBar;
    QLabel *label_2;

    void setupUi(QWidget *Fesa_Identification_QtClass)
    {
        if (Fesa_Identification_QtClass->objectName().isEmpty())
            Fesa_Identification_QtClass->setObjectName(QString::fromUtf8("Fesa_Identification_QtClass"));
        Fesa_Identification_QtClass->resize(1024, 520);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Fesa_Identification_QtClass->sizePolicy().hasHeightForWidth());
        Fesa_Identification_QtClass->setSizePolicy(sizePolicy);
        Fesa_Identification_QtClass->setMinimumSize(QSize(1024, 520));
        Fesa_Identification_QtClass->setMaximumSize(QSize(1024, 520));
        line = new QFrame(Fesa_Identification_QtClass);
        line->setObjectName(QString::fromUtf8("line"));
        line->setGeometry(QRect(-10, 500, 1061, 41));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        line_2 = new QFrame(Fesa_Identification_QtClass);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setGeometry(QRect(680, 0, 20, 521));
        line_2->setFrameShape(QFrame::VLine);
        line_2->setFrameShadow(QFrame::Sunken);
        verticalLayoutWidget = new QWidget(Fesa_Identification_QtClass);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(690, 20, 331, 191));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        verticalSpacer_8 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_8);

        commandLinkButton = new QCommandLinkButton(verticalLayoutWidget);
        commandLinkButton->setObjectName(QString::fromUtf8("commandLinkButton"));
        QFont font;
        font.setFamily(QString::fromUtf8("Segoe UI"));
        font.setPointSize(14);
        commandLinkButton->setFont(font);

        verticalLayout->addWidget(commandLinkButton);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        commandLinkButton_2 = new QCommandLinkButton(verticalLayoutWidget);
        commandLinkButton_2->setObjectName(QString::fromUtf8("commandLinkButton_2"));
        commandLinkButton_2->setFont(font);

        verticalLayout->addWidget(commandLinkButton_2);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        commandLinkButton_3 = new QCommandLinkButton(verticalLayoutWidget);
        commandLinkButton_3->setObjectName(QString::fromUtf8("commandLinkButton_3"));
        commandLinkButton_3->setFont(font);

        verticalLayout->addWidget(commandLinkButton_3);

        verticalSpacer_7 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_7);

        verticalLayoutWidget_2 = new QWidget(Fesa_Identification_QtClass);
        verticalLayoutWidget_2->setObjectName(QString::fromUtf8("verticalLayoutWidget_2"));
        verticalLayoutWidget_2->setGeometry(QRect(30, 10, 644, 510));
        verticalLayout_2 = new QVBoxLayout(verticalLayoutWidget_2);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(verticalLayoutWidget_2);
        label->setObjectName(QString::fromUtf8("label"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Maximum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy1);
        label->setMaximumSize(QSize(640, 480));
        label->setLayoutDirection(Qt::LeftToRight);
        label->setFrameShape(QFrame::Panel);
        label->setFrameShadow(QFrame::Sunken);
        label->setPixmap(QPixmap(QString::fromUtf8(":/Fesa_Identification_Qt/Resources/font_label.bmp")));
        label->setAlignment(Qt::AlignCenter);
        label->setTextInteractionFlags(Qt::NoTextInteraction);

        verticalLayout_2->addWidget(label);

        verticalLayoutWidget_6 = new QWidget(Fesa_Identification_QtClass);
        verticalLayoutWidget_6->setObjectName(QString::fromUtf8("verticalLayoutWidget_6"));
        verticalLayoutWidget_6->setGeometry(QRect(690, 220, 331, 291));
        verticalLayout_6 = new QVBoxLayout(verticalLayoutWidget_6);
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setContentsMargins(11, 11, 11, 11);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        verticalLayout_6->setContentsMargins(0, 0, 0, 0);
        groupBox = new QGroupBox(verticalLayoutWidget_6);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Times New Roman"));
        font1.setPointSize(14);
        font1.setBold(false);
        font1.setItalic(false);
        font1.setWeight(50);
        groupBox->setFont(font1);
        groupBox->setCheckable(false);
        verticalLayoutWidget_7 = new QWidget(groupBox);
        verticalLayoutWidget_7->setObjectName(QString::fromUtf8("verticalLayoutWidget_7"));
        verticalLayoutWidget_7->setGeometry(QRect(140, 50, 181, 81));
        verticalLayout_7 = new QVBoxLayout(verticalLayoutWidget_7);
        verticalLayout_7->setSpacing(6);
        verticalLayout_7->setContentsMargins(11, 11, 11, 11);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        verticalLayout_7->setContentsMargins(0, 0, 0, 0);
        label_3 = new QLabel(verticalLayoutWidget_7);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setLayoutDirection(Qt::LeftToRight);
        label_3->setInputMethodHints(Qt::ImhDialableCharactersOnly);
        label_3->setAlignment(Qt::AlignCenter);

        verticalLayout_7->addWidget(label_3);

        label_4 = new QLabel(verticalLayoutWidget_7);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setLayoutDirection(Qt::LeftToRight);
        label_4->setInputMethodHints(Qt::ImhDialableCharactersOnly);
        label_4->setAlignment(Qt::AlignCenter);

        verticalLayout_7->addWidget(label_4);

        horizontalLayoutWidget = new QWidget(groupBox);
        horizontalLayoutWidget->setObjectName(QString::fromUtf8("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(20, 40, 101, 101));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label_5 = new QLabel(horizontalLayoutWidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setCursor(QCursor(Qt::CrossCursor));
        label_5->setAutoFillBackground(true);
        label_5->setFrameShape(QFrame::StyledPanel);
        label_5->setTextFormat(Qt::AutoText);
        label_5->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(label_5);

        verticalLayoutWidget_5 = new QWidget(groupBox);
        verticalLayoutWidget_5->setObjectName(QString::fromUtf8("verticalLayoutWidget_5"));
        verticalLayoutWidget_5->setGeometry(QRect(20, 170, 191, 61));
        verticalLayout_5 = new QVBoxLayout(verticalLayoutWidget_5);
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        verticalLayout_5->setContentsMargins(0, 0, 0, 0);
        progressBar = new QProgressBar(verticalLayoutWidget_5);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setMaximum(100);
        progressBar->setValue(0);
        progressBar->setAlignment(Qt::AlignCenter);
        progressBar->setTextVisible(true);
        progressBar->setOrientation(Qt::Horizontal);
        progressBar->setInvertedAppearance(false);

        verticalLayout_5->addWidget(progressBar);

        label_2 = new QLabel(verticalLayoutWidget_5);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setAlignment(Qt::AlignCenter);

        verticalLayout_5->addWidget(label_2);


        verticalLayout_6->addWidget(groupBox);


        retranslateUi(Fesa_Identification_QtClass);

        QMetaObject::connectSlotsByName(Fesa_Identification_QtClass);
    } // setupUi

    void retranslateUi(QWidget *Fesa_Identification_QtClass)
    {
        Fesa_Identification_QtClass->setWindowTitle(QApplication::translate("Fesa_Identification_QtClass", "\320\230\320\264\320\265\320\275\321\202\320\270\321\204\320\270\320\272\320\260\321\206\320\270\321\217 \320\277\320\276 \321\201\320\275\320\270\320\274\320\272\321\203 \320\273\320\270\321\206\320\260. V 1.0", 0, QApplication::UnicodeUTF8));
        commandLinkButton->setText(QApplication::translate("Fesa_Identification_QtClass", "\320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214 \320\276\320\261\321\212\320\265\320\272\321\202", 0, QApplication::UnicodeUTF8));
        commandLinkButton_2->setText(QApplication::translate("Fesa_Identification_QtClass", "\320\230\320\264\320\265\320\275\321\202\320\270\321\204\320\270\320\272\320\260\321\206\320\270\321\217", 0, QApplication::UnicodeUTF8));
        commandLinkButton_3->setText(QApplication::translate("Fesa_Identification_QtClass", "\320\222\321\213\321\205\320\276\320\264", 0, QApplication::UnicodeUTF8));
        label->setText(QString());
        groupBox->setTitle(QApplication::translate("Fesa_Identification_QtClass", "\320\240\320\265\320\267\321\203\320\273\321\214\321\202\320\260\321\202 \320\270\320\264\320\265\320\275\321\202\320\270\321\204\320\270\320\272\320\260\321\206\320\270\320\270:", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("Fesa_Identification_QtClass", "\320\244\320\260\320\274\320\270\320\273\320\270\321\217", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("Fesa_Identification_QtClass", "\320\230\320\274\321\217", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("Fesa_Identification_QtClass", "\320\244\320\276\321\202\320\276", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("Fesa_Identification_QtClass", "\320\230\320\264\320\265\320\275\321\202\320\270\321\207\320\275\320\276\321\201\321\202\321\214", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Fesa_Identification_QtClass: public Ui_Fesa_Identification_QtClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FESA_IDENTIFICATION_QT_H
