/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QDate>
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableView>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QTextBrowser *tytulAplikacji;
    QFrame *line;
    QFrame *line_2;
    QPushButton *ExportToXML;
    QTableView *DB_table;
    QPushButton *prevButton;
    QPushButton *nextButton;
    QPushButton *ExitButton;
    QLabel *PageNumber;
    QLabel *FiltrLabael;
    QLabel *dataODLabel;
    QLabel *dataDOLabel;
    QDateEdit *dataStart;
    QLabel *plecLabel;
    QDateEdit *dataEnd;
    QSpinBox *wiekMIN;
    QLabel *wiekODLabel;
    QLabel *wiekDOLabel;
    QSpinBox *wiekMAX;
    QLabel *modalnoscLabel;
    QPushButton *filterButton;
    QLabel *dopasowaniaLabel;
    QLCDNumber *dopasowaniaNr;
    QPushButton *ResetButton;
    QFrame *line_4;
    QFrame *line_5;
    QCheckBox *plecK;
    QCheckBox *plecM;
    QCheckBox *plecO;
    QCheckBox *modaNM;
    QCheckBox *modaCT;
    QCheckBox *modaMRI;
    QCheckBox *modaRTG;
    QCheckBox *modaUSG;
    QTextBrowser *statusWindow;
    QPushButton *StatsButton;
    QLineEdit *nameEdit;
    QFrame *line_6;
    QLabel *nameLabel;
    QLabel *xmlLabel;
    QFrame *line_7;
    QLabel *statusLabel;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1172, 613);
        QFont font;
        font.setPointSize(11);
        MainWindow->setFont(font);
        MainWindow->setInputMethodHints(Qt::ImhNone);
        MainWindow->setToolButtonStyle(Qt::ToolButtonIconOnly);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        tytulAplikacji = new QTextBrowser(centralwidget);
        tytulAplikacji->setObjectName("tytulAplikacji");
        tytulAplikacji->setGeometry(QRect(50, 20, 1101, 61));
        line = new QFrame(centralwidget);
        line->setObjectName("line");
        line->setGeometry(QRect(50, 90, 1101, 20));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        line_2 = new QFrame(centralwidget);
        line_2->setObjectName("line_2");
        line_2->setGeometry(QRect(500, 116, 21, 461));
        line_2->setFrameShape(QFrame::VLine);
        line_2->setFrameShadow(QFrame::Sunken);
        ExportToXML = new QPushButton(centralwidget);
        ExportToXML->setObjectName("ExportToXML");
        ExportToXML->setGeometry(QRect(270, 550, 221, 31));
        DB_table = new QTableView(centralwidget);
        DB_table->setObjectName("DB_table");
        DB_table->setGeometry(QRect(530, 120, 621, 421));
        QFont font1;
        font1.setPointSize(13);
        font1.setStyleStrategy(QFont::PreferDefault);
        DB_table->setFont(font1);
        DB_table->viewport()->setProperty("cursor", QVariant(QCursor(Qt::ArrowCursor)));
        DB_table->setLayoutDirection(Qt::LeftToRight);
        DB_table->setTextElideMode(Qt::ElideLeft);
        DB_table->setShowGrid(true);
        DB_table->verticalHeader()->setVisible(false);
        DB_table->verticalHeader()->setHighlightSections(true);
        prevButton = new QPushButton(centralwidget);
        prevButton->setObjectName("prevButton");
        prevButton->setGeometry(QRect(740, 550, 51, 31));
        nextButton = new QPushButton(centralwidget);
        nextButton->setObjectName("nextButton");
        nextButton->setGeometry(QRect(890, 550, 51, 31));
        ExitButton = new QPushButton(centralwidget);
        ExitButton->setObjectName("ExitButton");
        ExitButton->setGeometry(QRect(50, 550, 101, 31));
        PageNumber = new QLabel(centralwidget);
        PageNumber->setObjectName("PageNumber");
        PageNumber->setGeometry(QRect(800, 550, 81, 31));
        PageNumber->setAlignment(Qt::AlignCenter);
        FiltrLabael = new QLabel(centralwidget);
        FiltrLabael->setObjectName("FiltrLabael");
        FiltrLabael->setGeometry(QRect(50, 180, 441, 21));
        QFont font2;
        font2.setPointSize(13);
        font2.setBold(false);
        FiltrLabael->setFont(font2);
        FiltrLabael->setAlignment(Qt::AlignCenter);
        dataODLabel = new QLabel(centralwidget);
        dataODLabel->setObjectName("dataODLabel");
        dataODLabel->setGeometry(QRect(50, 218, 91, 31));
        QFont font3;
        font3.setPointSize(11);
        font3.setBold(false);
        dataODLabel->setFont(font3);
        dataODLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        dataDOLabel = new QLabel(centralwidget);
        dataDOLabel->setObjectName("dataDOLabel");
        dataDOLabel->setGeometry(QRect(280, 218, 81, 31));
        dataDOLabel->setFont(font3);
        dataDOLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        dataStart = new QDateEdit(centralwidget);
        dataStart->setObjectName("dataStart");
        dataStart->setGeometry(QRect(150, 218, 110, 31));
        dataStart->setFont(font);
        dataStart->setCalendarPopup(true);
        plecLabel = new QLabel(centralwidget);
        plecLabel->setObjectName("plecLabel");
        plecLabel->setGeometry(QRect(90, 319, 91, 31));
        plecLabel->setFont(font3);
        plecLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        dataEnd = new QDateEdit(centralwidget);
        dataEnd->setObjectName("dataEnd");
        dataEnd->setGeometry(QRect(370, 218, 110, 31));
        dataEnd->setDateTime(QDateTime(QDate(2050, 12, 31), QTime(0, 0, 0)));
        dataEnd->setMinimumDate(QDate(1900, 1, 1));
        dataEnd->setCalendarPopup(true);
        wiekMIN = new QSpinBox(centralwidget);
        wiekMIN->setObjectName("wiekMIN");
        wiekMIN->setGeometry(QRect(150, 268, 111, 31));
        wiekMIN->setMinimum(-1000);
        wiekMIN->setMaximum(1000);
        wiekODLabel = new QLabel(centralwidget);
        wiekODLabel->setObjectName("wiekODLabel");
        wiekODLabel->setGeometry(QRect(50, 268, 91, 31));
        wiekODLabel->setFont(font3);
        wiekODLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        wiekDOLabel = new QLabel(centralwidget);
        wiekDOLabel->setObjectName("wiekDOLabel");
        wiekDOLabel->setGeometry(QRect(270, 268, 91, 31));
        wiekDOLabel->setFont(font3);
        wiekDOLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        wiekMAX = new QSpinBox(centralwidget);
        wiekMAX->setObjectName("wiekMAX");
        wiekMAX->setGeometry(QRect(370, 268, 111, 31));
        wiekMAX->setMinimum(-1000);
        wiekMAX->setMaximum(1000);
        wiekMAX->setValue(99);
        modalnoscLabel = new QLabel(centralwidget);
        modalnoscLabel->setObjectName("modalnoscLabel");
        modalnoscLabel->setGeometry(QRect(50, 360, 131, 31));
        modalnoscLabel->setFont(font3);
        modalnoscLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        filterButton = new QPushButton(centralwidget);
        filterButton->setObjectName("filterButton");
        filterButton->setGeometry(QRect(280, 400, 211, 21));
        dopasowaniaLabel = new QLabel(centralwidget);
        dopasowaniaLabel->setObjectName("dopasowaniaLabel");
        dopasowaniaLabel->setGeometry(QRect(170, 447, 181, 31));
        dopasowaniaNr = new QLCDNumber(centralwidget);
        dopasowaniaNr->setObjectName("dopasowaniaNr");
        dopasowaniaNr->setGeometry(QRect(370, 449, 121, 31));
        dopasowaniaNr->setLayoutDirection(Qt::LeftToRight);
        dopasowaniaNr->setAutoFillBackground(false);
        dopasowaniaNr->setDigitCount(6);
        dopasowaniaNr->setSegmentStyle(QLCDNumber::Flat);
        ResetButton = new QPushButton(centralwidget);
        ResetButton->setObjectName("ResetButton");
        ResetButton->setGeometry(QRect(50, 400, 211, 21));
        line_4 = new QFrame(centralwidget);
        line_4->setObjectName("line_4");
        line_4->setGeometry(QRect(50, 150, 441, 20));
        line_4->setFrameShape(QFrame::HLine);
        line_4->setFrameShadow(QFrame::Sunken);
        line_5 = new QFrame(centralwidget);
        line_5->setObjectName("line_5");
        line_5->setGeometry(QRect(50, 430, 441, 21));
        line_5->setFrameShape(QFrame::HLine);
        line_5->setFrameShadow(QFrame::Sunken);
        plecK = new QCheckBox(centralwidget);
        plecK->setObjectName("plecK");
        plecK->setEnabled(false);
        plecK->setGeometry(QRect(200, 320, 31, 24));
        QFont font4;
        font4.setPointSize(10);
        font4.setBold(false);
        plecK->setFont(font4);
        plecK->setChecked(false);
        plecM = new QCheckBox(centralwidget);
        plecM->setObjectName("plecM");
        plecM->setEnabled(false);
        plecM->setGeometry(QRect(260, 320, 41, 24));
        plecM->setFont(font4);
        plecM->setChecked(false);
        plecO = new QCheckBox(centralwidget);
        plecO->setObjectName("plecO");
        plecO->setEnabled(false);
        plecO->setGeometry(QRect(320, 320, 41, 24));
        plecO->setFont(font4);
        plecO->setChecked(false);
        modaNM = new QCheckBox(centralwidget);
        modaNM->setObjectName("modaNM");
        modaNM->setEnabled(false);
        modaNM->setGeometry(QRect(200, 360, 41, 24));
        modaNM->setFont(font4);
        modaNM->setChecked(false);
        modaCT = new QCheckBox(centralwidget);
        modaCT->setObjectName("modaCT");
        modaCT->setEnabled(false);
        modaCT->setGeometry(QRect(260, 360, 41, 24));
        modaCT->setFont(font4);
        modaMRI = new QCheckBox(centralwidget);
        modaMRI->setObjectName("modaMRI");
        modaMRI->setEnabled(false);
        modaMRI->setGeometry(QRect(320, 360, 51, 24));
        modaMRI->setFont(font4);
        modaRTG = new QCheckBox(centralwidget);
        modaRTG->setObjectName("modaRTG");
        modaRTG->setEnabled(false);
        modaRTG->setGeometry(QRect(380, 360, 51, 24));
        modaRTG->setFont(font4);
        modaUSG = new QCheckBox(centralwidget);
        modaUSG->setObjectName("modaUSG");
        modaUSG->setEnabled(false);
        modaUSG->setGeometry(QRect(440, 360, 51, 24));
        modaUSG->setFont(font4);
        statusWindow = new QTextBrowser(centralwidget);
        statusWindow->setObjectName("statusWindow");
        statusWindow->setGeometry(QRect(120, 120, 371, 22));
        QFont font5;
        font5.setFamilies({QString::fromUtf8("Monospace")});
        font5.setPointSize(9);
        statusWindow->setFont(font5);
        statusWindow->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        statusWindow->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        StatsButton = new QPushButton(centralwidget);
        StatsButton->setObjectName("StatsButton");
        StatsButton->setGeometry(QRect(160, 550, 101, 31));
        nameEdit = new QLineEdit(centralwidget);
        nameEdit->setObjectName("nameEdit");
        nameEdit->setGeometry(QRect(200, 505, 251, 20));
        line_6 = new QFrame(centralwidget);
        line_6->setObjectName("line_6");
        line_6->setGeometry(QRect(50, 480, 441, 20));
        line_6->setFrameShape(QFrame::HLine);
        line_6->setFrameShadow(QFrame::Sunken);
        nameLabel = new QLabel(centralwidget);
        nameLabel->setObjectName("nameLabel");
        nameLabel->setGeometry(QRect(50, 498, 141, 31));
        nameLabel->setFont(font3);
        nameLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        xmlLabel = new QLabel(centralwidget);
        xmlLabel->setObjectName("xmlLabel");
        xmlLabel->setGeometry(QRect(460, 498, 31, 31));
        xmlLabel->setFont(font3);
        xmlLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        line_7 = new QFrame(centralwidget);
        line_7->setObjectName("line_7");
        line_7->setGeometry(QRect(50, 530, 441, 20));
        line_7->setFrameShape(QFrame::HLine);
        line_7->setFrameShadow(QFrame::Sunken);
        statusLabel = new QLabel(centralwidget);
        statusLabel->setObjectName("statusLabel");
        statusLabel->setGeometry(QRect(50, 115, 61, 31));
        statusLabel->setFont(font3);
        statusLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "DICOM -> XML", nullptr));
        tytulAplikacji->setHtml(QCoreApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Ubuntu Sans'; font-size:11pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:26pt;\">APLIKACJA DO EKSPORTU PLIK\303\223W XML</span></p></body></html>", nullptr));
        ExportToXML->setText(QCoreApplication::translate("MainWindow", "EKSPORTUJ DO XML", nullptr));
        prevButton->setText(QCoreApplication::translate("MainWindow", "<", nullptr));
        nextButton->setText(QCoreApplication::translate("MainWindow", ">", nullptr));
        ExitButton->setText(QCoreApplication::translate("MainWindow", "ZAMKNIJ", nullptr));
        PageNumber->setText(QCoreApplication::translate("MainWindow", "1 / 1", nullptr));
        FiltrLabael->setText(QCoreApplication::translate("MainWindow", "FILTROWANIE BAZY DANYCH", nullptr));
        dataODLabel->setText(QCoreApplication::translate("MainWindow", "DATA OD : ", nullptr));
        dataDOLabel->setText(QCoreApplication::translate("MainWindow", "DATA DO : ", nullptr));
        dataStart->setDisplayFormat(QCoreApplication::translate("MainWindow", "yyyy/MM/dd", nullptr));
        plecLabel->setText(QCoreApplication::translate("MainWindow", "P\305\201E\304\206 : ", nullptr));
        dataEnd->setDisplayFormat(QCoreApplication::translate("MainWindow", "yyyy/MM/dd", nullptr));
        wiekODLabel->setText(QCoreApplication::translate("MainWindow", "WIEK OD : ", nullptr));
        wiekDOLabel->setText(QCoreApplication::translate("MainWindow", "WIEK DO : ", nullptr));
        modalnoscLabel->setText(QCoreApplication::translate("MainWindow", "MODALNO\305\232\304\206 : ", nullptr));
        filterButton->setText(QCoreApplication::translate("MainWindow", "FILTRUJ", nullptr));
        dopasowaniaLabel->setText(QCoreApplication::translate("MainWindow", "Znalezionych dopasowa\305\204 : ", nullptr));
        ResetButton->setText(QCoreApplication::translate("MainWindow", "RESETUJ FILTRY", nullptr));
        plecK->setText(QCoreApplication::translate("MainWindow", "K", nullptr));
        plecM->setText(QCoreApplication::translate("MainWindow", "M", nullptr));
        plecO->setText(QCoreApplication::translate("MainWindow", "O", nullptr));
        modaNM->setText(QCoreApplication::translate("MainWindow", "NM", nullptr));
        modaCT->setText(QCoreApplication::translate("MainWindow", "CT", nullptr));
        modaMRI->setText(QCoreApplication::translate("MainWindow", "MRI", nullptr));
        modaRTG->setText(QCoreApplication::translate("MainWindow", "RTG", nullptr));
        modaUSG->setText(QCoreApplication::translate("MainWindow", "USG", nullptr));
        StatsButton->setText(QCoreApplication::translate("MainWindow", "STATYSTYKI", nullptr));
        nameLabel->setText(QCoreApplication::translate("MainWindow", "NAZWA PLIKU XML: ", nullptr));
        xmlLabel->setText(QCoreApplication::translate("MainWindow", ". xml", nullptr));
        statusLabel->setText(QCoreApplication::translate("MainWindow", "STATUS : ", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
