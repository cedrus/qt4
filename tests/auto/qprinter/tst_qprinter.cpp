/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the test suite of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
** $QT_END_LICENSE$
**
****************************************************************************/


#include <QtTest/QtTest>



#include <qprinter.h>
#include <qpagesetupdialog.h>
#include <qpainter.h>
#include <qprintdialog.h>
#include <qprinterinfo.h>
#ifdef QT3_SUPPORT
#include <q3paintdevicemetrics.h>
#endif
#include <qvariant.h>
#include <qpainter.h>
#include <qprintengine.h>

#include <math.h>

#ifdef Q_WS_WIN
#include <windows.h>
#endif

Q_DECLARE_METATYPE(QRect)

QT_FORWARD_DECLARE_CLASS(QPrinter)

//TESTED_CLASS=
//TESTED_FILES=

#ifndef QT_NO_PRINTER

class tst_QPrinter : public QObject
{
    Q_OBJECT

public:
    tst_QPrinter();
    virtual ~tst_QPrinter();


public slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();
private slots:
    void getSetCheck();
// Add your testfunctions and testdata create functions here
    void testPageSize();
    void testPageRectAndPaperRect();
    void testPageRectAndPaperRect_data();
    void testSetOptions();
    void testMargins_data();
    void testMargins();
    void testNonExistentPrinter();
    void testPageSetupDialog();
    void testMulitpleSets_data();
    void testMulitpleSets();
    void testPageMargins_data();
    void testPageMargins();
    void changingOutputFormat();
    void outputFormatFromSuffix();
    void setGetPaperSize();
    void valuePreservation();
    void errorReporting();
    void testCustomPageSizes();
    void printDialogCompleter();

    void testCopyCount();
    void testCurrentPage();

    void taskQTBUG4497_reusePrinterOnDifferentFiles();
    void testPdfTitle();

private:
};

// Testing get/set functions
void tst_QPrinter::getSetCheck()
{
    QPrinter obj1;
    // OutputFormat QPrinter::outputFormat()
    // void QPrinter::setOutputFormat(OutputFormat)
    obj1.setOutputFormat(QPrinter::OutputFormat(QPrinter::PdfFormat));
    QCOMPARE(QPrinter::OutputFormat(QPrinter::PdfFormat), obj1.outputFormat());

    // bool QPrinter::collateCopies()
    // void QPrinter::setCollateCopies(bool)
    obj1.setCollateCopies(false);
    QCOMPARE(false, obj1.collateCopies());
    obj1.setCollateCopies(true);
    QCOMPARE(true, obj1.collateCopies());

    obj1.setColorMode(QPrinter::GrayScale);
    QCOMPARE(obj1.colorMode(), QPrinter::GrayScale);
    obj1.setColorMode(QPrinter::Color);
    QCOMPARE(obj1.colorMode(), QPrinter::Color);

    obj1.setCreator(QString::fromLatin1("RandomQtUser"));
    QCOMPARE(obj1.creator(), QString::fromLatin1("RandomQtUser"));

    obj1.setDocName(QString::fromLatin1("RandomQtDocument"));
    QCOMPARE(obj1.docName(), QString::fromLatin1("RandomQtDocument"));

    obj1.setDoubleSidedPrinting(true);
    QCOMPARE(obj1.doubleSidedPrinting(), true);
    obj1.setDoubleSidedPrinting(false);
    QCOMPARE(obj1.doubleSidedPrinting(), false);

    obj1.setFromTo(1, 4);
    QCOMPARE(obj1.fromPage(), 1);
    QCOMPARE(obj1.toPage(), 4);

    obj1.setFullPage(true);
    QCOMPARE(obj1.fullPage(), true);
    obj1.setFullPage(false);
    QCOMPARE(obj1.fullPage(), false);

    obj1.setOrientation(QPrinter::Landscape);
    QCOMPARE(obj1.orientation(), QPrinter::Landscape);
    obj1.setOrientation(QPrinter::Portrait);
    QCOMPARE(obj1.orientation(), QPrinter::Portrait);

    obj1.setOutputFileName(QString::fromLatin1("RandomQtName"));
    QCOMPARE(obj1.outputFileName(), QString::fromLatin1("RandomQtName"));

    obj1.setPageOrder(QPrinter::FirstPageFirst);
    QCOMPARE(obj1.pageOrder(), QPrinter::FirstPageFirst);
    obj1.setPageOrder(QPrinter::LastPageFirst);
    QCOMPARE(obj1.pageOrder(), QPrinter::LastPageFirst);

    obj1.setPaperSource(QPrinter::Cassette);
    QCOMPARE(obj1.paperSource(), QPrinter::Cassette);
    obj1.setPaperSource(QPrinter::Middle);
    QCOMPARE(obj1.paperSource(), QPrinter::Middle);

#ifdef Q_OS_UNIX
    obj1.setPrintProgram(QString::fromLatin1("/bin/true"));
    QCOMPARE(obj1.printProgram(), QString::fromLatin1("/bin/true"));

    obj1.setPrinterSelectionOption(QString::fromLatin1("--option"));
    QCOMPARE(obj1.printerSelectionOption(), QString::fromLatin1("--option"));
#endif

    obj1.setPrinterName(QString::fromLatin1("myPrinter"));
    QCOMPARE(obj1.printerName(), QString::fromLatin1("myPrinter"));

    // bool QPrinter::fontEmbeddingEnabled()
    // void QPrinter::setFontEmbeddingEnabled(bool)
    obj1.setFontEmbeddingEnabled(false);
    QCOMPARE(false, obj1.fontEmbeddingEnabled());
    obj1.setFontEmbeddingEnabled(true);
    QCOMPARE(true, obj1.fontEmbeddingEnabled());

    // PageSize QPrinter::pageSize()
    // void QPrinter::setPageSize(PageSize)
    obj1.setPageSize(QPrinter::PageSize(QPrinter::A4));
    QCOMPARE(QPrinter::PageSize(QPrinter::A4), obj1.pageSize());
    obj1.setPageSize(QPrinter::PageSize(QPrinter::Letter));
    QCOMPARE(QPrinter::PageSize(QPrinter::Letter), obj1.pageSize());
    obj1.setPageSize(QPrinter::PageSize(QPrinter::Legal));
    QCOMPARE(QPrinter::PageSize(QPrinter::Legal), obj1.pageSize());

    // PrintRange QPrinter::printRange()
    // void QPrinter::setPrintRange(PrintRange)
    obj1.setPrintRange(QPrinter::PrintRange(QPrinter::AllPages));
    QCOMPARE(QPrinter::PrintRange(QPrinter::AllPages), obj1.printRange());
    obj1.setPrintRange(QPrinter::PrintRange(QPrinter::Selection));
    QCOMPARE(QPrinter::PrintRange(QPrinter::Selection), obj1.printRange());
    obj1.setPrintRange(QPrinter::PrintRange(QPrinter::PageRange));
    QCOMPARE(QPrinter::PrintRange(QPrinter::PageRange), obj1.printRange());
}

tst_QPrinter::tst_QPrinter()
{
}

tst_QPrinter::~tst_QPrinter()
{
}

// initTestCase will be executed once before the first testfunction is executed.
void tst_QPrinter::initTestCase()
{
// TODO: Add testcase generic initialization code here.
// suggestion:
//    testWidget = new QPrinter(0,"testWidget");
//    testWidget->setFixedSize(200, 200);
//    qApp->setMainWidget(testWidget);
//    testWidget->show();
}

// cleanupTestCase will be executed once after the last testfunction is executed.
void tst_QPrinter::cleanupTestCase()
{
// TODO: Add testcase generic cleanup code here.
// suggestion:
//    testWidget->hide();
//    qApp->setMainWidget(0);
//    delete testWidget;
}

// init() will be executed immediately before each testfunction is run.
void tst_QPrinter::init()
{
// TODO: Add testfunction specific initialization code here.
}

// cleanup() will be executed immediately after each testfunction is run.
void tst_QPrinter::cleanup()
{
// TODO: Add testfunction specific cleanup code here.
}

#define MYCOMPARE(a, b) QCOMPARE(QVariant((int)a), QVariant((int)b))

void tst_QPrinter::testPageSetupDialog()
{
    // Make sure this doesn't crash at least
    {
        QPrinter printer;
        QPageSetupDialog dialog(&printer);
    }
}

void tst_QPrinter::testPageSize()
{
#if defined (Q_WS_WIN)
    QPrinter prn;

    prn.setPageSize(QPrinter::Letter);
    MYCOMPARE(prn.pageSize(), QPrinter::Letter);
    MYCOMPARE(prn.winPageSize(), DMPAPER_LETTER);

    prn.setPageSize(QPrinter::A4);
    MYCOMPARE(prn.pageSize(), QPrinter::A4);
    MYCOMPARE(prn.winPageSize(), DMPAPER_A4);

    prn.setWinPageSize(DMPAPER_LETTER);
    MYCOMPARE(prn.winPageSize(), DMPAPER_LETTER);
    MYCOMPARE(prn.pageSize(), QPrinter::Letter);

    prn.setWinPageSize(DMPAPER_A4);
    MYCOMPARE(prn.winPageSize(), DMPAPER_A4);
    MYCOMPARE(prn.pageSize(), QPrinter::A4);
#else
    QSKIP("QPrinter::winPageSize() does not exist for nonwindows platforms", SkipAll);
#endif
}

void tst_QPrinter::testPageRectAndPaperRect_data()
{
    QTest::addColumn<int>("orientation");
    QTest::addColumn<bool>("withPainter");
    QTest::addColumn<int>("resolution");
    QTest::addColumn<bool>("doPaperRect");

    // paperrect
    QTest::newRow("paperRect0") << int(QPrinter::Portrait) << true << 300 << true;
    QTest::newRow("paperRect1") << int(QPrinter::Portrait) << false << 300 << true;
    QTest::newRow("paperRect2") << int(QPrinter::Landscape) << true << 300 << true;
    QTest::newRow("paperRect3") << int(QPrinter::Landscape) << false << 300 << true;
    QTest::newRow("paperRect4") << int(QPrinter::Portrait) << true << 600 << true;
    QTest::newRow("paperRect5") << int(QPrinter::Portrait) << false << 600 << true;
    QTest::newRow("paperRect6") << int(QPrinter::Landscape) << true << 600 << true;
    QTest::newRow("paperRect7") << int(QPrinter::Landscape) << false << 600 << true;
    QTest::newRow("paperRect8") << int(QPrinter::Portrait) << true << 1200 << true;
    QTest::newRow("paperRect9") << int(QPrinter::Portrait) << false << 1200 << true;
    QTest::newRow("paperRect10") << int(QPrinter::Landscape) << true << 1200 << true;
    QTest::newRow("paperRect11") << int(QPrinter::Landscape) << false << 1200 << true;

    // page rect
    QTest::newRow("pageRect0") << int(QPrinter::Portrait) << true << 300 << false;
    QTest::newRow("pageRect1") << int(QPrinter::Portrait) << false << 300 << false;
    QTest::newRow("pageRect2") << int(QPrinter::Landscape) << true << 300 << false;
    QTest::newRow("pageRect3") << int(QPrinter::Landscape) << false << 300 << false;
    QTest::newRow("pageRect4") << int(QPrinter::Portrait) << true << 600 << false;
    QTest::newRow("pageRect5") << int(QPrinter::Portrait) << false << 600 << false;
    QTest::newRow("pageRect6") << int(QPrinter::Landscape) << true << 600 << false;
    QTest::newRow("pageRect7") << int(QPrinter::Landscape) << false << 600 << false;
    QTest::newRow("pageRect8") << int(QPrinter::Portrait) << true << 1200 << false;
    QTest::newRow("pageRect9") << int(QPrinter::Portrait) << false << 1200 << false;
    QTest::newRow("pageRect10") << int(QPrinter::Landscape) << true << 1200 << false;
    QTest::newRow("pageRect11") << int(QPrinter::Landscape) << false << 1200 << false;
}

void tst_QPrinter::testPageRectAndPaperRect()
{
    QFETCH(bool,  withPainter);
    QFETCH(int,  orientation);
    QFETCH(int, resolution);
    QFETCH(bool, doPaperRect);

    QPainter *painter = 0;
    QPrinter printer(QPrinter::HighResolution);
    printer.setOrientation(QPrinter::Orientation(orientation));
    printer.setOutputFileName("silly");

    QRect pageRect = doPaperRect ? printer.paperRect() : printer.pageRect();
    float inchesX = float(pageRect.width()) / float(printer.resolution());
    float inchesY = float(pageRect.height()) / float(printer.resolution());
    printer.setResolution(resolution);
    if (withPainter)
        painter = new QPainter(&printer);

    QRect otherRect = doPaperRect ? printer.paperRect() : printer.pageRect();
    float otherInchesX = float(otherRect.width()) / float(printer.resolution());
    float otherInchesY = float(otherRect.height()) / float(printer.resolution());
    if (painter != 0)
        delete painter;

    QVERIFY(qAbs(otherInchesX - inchesX) < 0.01);
    QVERIFY(qAbs(otherInchesY - inchesY) < 0.01);

    QVERIFY(printer.orientation() == QPrinter::Portrait || pageRect.width() > pageRect.height());
    QVERIFY(printer.orientation() != QPrinter::Portrait || pageRect.width() < pageRect.height());
}

void tst_QPrinter::testSetOptions()
{
    QPrinter prn;
    QPrintDialog dlg(&prn);

    // Verify default values
    MYCOMPARE(dlg.isOptionEnabled(QAbstractPrintDialog::PrintToFile), TRUE);
    MYCOMPARE(dlg.isOptionEnabled(QAbstractPrintDialog::PrintSelection), FALSE);
    MYCOMPARE(dlg.isOptionEnabled(QAbstractPrintDialog::PrintPageRange), TRUE);

    dlg.setEnabledOptions(QAbstractPrintDialog::PrintPageRange);
    MYCOMPARE(dlg.isOptionEnabled(QAbstractPrintDialog::PrintToFile), FALSE);
    MYCOMPARE(dlg.isOptionEnabled(QAbstractPrintDialog::PrintSelection), FALSE);
    MYCOMPARE(dlg.isOptionEnabled(QAbstractPrintDialog::PrintPageRange), TRUE);

    dlg.setEnabledOptions((QAbstractPrintDialog::PrintDialogOptions(QAbstractPrintDialog::PrintSelection
                                                                    | QAbstractPrintDialog::PrintPageRange)));
    MYCOMPARE(dlg.isOptionEnabled(QAbstractPrintDialog::PrintToFile), FALSE);
    MYCOMPARE(dlg.isOptionEnabled(QAbstractPrintDialog::PrintSelection), TRUE);
    MYCOMPARE(dlg.isOptionEnabled(QAbstractPrintDialog::PrintPageRange), TRUE);

    dlg.setEnabledOptions(QAbstractPrintDialog::PrintSelection);
    MYCOMPARE(dlg.isOptionEnabled(QAbstractPrintDialog::PrintToFile), FALSE);
    MYCOMPARE(dlg.isOptionEnabled(QAbstractPrintDialog::PrintSelection), TRUE);
    MYCOMPARE(dlg.isOptionEnabled(QAbstractPrintDialog::PrintPageRange), FALSE);
}

void tst_QPrinter::testMargins_data()
{
    QTest::addColumn<int>("orientation");
    QTest::addColumn<bool>("fullpage");
    QTest::addColumn<int>("pagesize");
    QTest::addColumn<int>("width");
    QTest::addColumn<int>("height");
    QTest::addColumn<bool>("withPainter");

    QTest::newRow("data0") << int(QPrinter::Portrait) << true << int(QPrinter::A4) << 210 << 297 << false;
    QTest::newRow("data1") << int(QPrinter::Landscape) << true << int(QPrinter::A4) << 297 << 210 << false;
    QTest::newRow("data2") << int(QPrinter::Landscape) << false << int(QPrinter::A4) << 297 << 210 << false;
    QTest::newRow("data3") << int(QPrinter::Portrait) << false << int(QPrinter::A4) << 210 << 297 << false;
    QTest::newRow("data4") << int(QPrinter::Portrait) << true << int(QPrinter::A4) << 210 << 297 << true;
    QTest::newRow("data5") << int(QPrinter::Landscape) << true << int(QPrinter::A4) << 297 << 210 << true;
    QTest::newRow("data6") << int(QPrinter::Landscape) << false << int(QPrinter::A4) << 297 << 210 << true;
    QTest::newRow("data7") << int(QPrinter::Portrait) << false << int(QPrinter::A4) << 210 << 297 << true;
}

void tst_QPrinter::testMargins()
{
    QFETCH(bool,  withPainter);
    QFETCH(int,  orientation);
    QFETCH(int,  pagesize);
    QFETCH(int,  width);
    QFETCH(int,  height);
    QFETCH(bool, fullpage);
    QPrinter printer;
    QPainter *painter = 0;
    printer.setOutputFileName("silly");
    printer.setOrientation((QPrinter::Orientation)orientation);
    printer.setFullPage(fullpage);
    printer.setPageSize((QPrinter::PageSize)pagesize);
    if (withPainter)
        painter = new QPainter(&printer);

#ifdef QT3_SUPPORT
    Q3PaintDeviceMetrics metrics(&printer);
    int pwidth = metrics.width();
    int pheight = metrics.height();

    if (orientation == QPrinter::Portrait) {
        QVERIFY(pheight >= pwidth);
    } else {
        QVERIFY(pwidth >= pheight);
    }

    if (fullpage) {
        QCOMPARE(metrics.widthMM(), width);
        QCOMPARE(metrics.heightMM(), height);
    }
#endif

    if (painter)
        delete painter;
}

void tst_QPrinter::testNonExistentPrinter()
{
#if defined(Q_WS_X11) || defined(Q_WS_QWS) || defined(Q_OS_MAC)
    QSKIP("QPrinter::testNonExistentPrinter() is not relevant for X11/Embedded/Mac", SkipAll);
#else
    QPrinter printer;
    QPainter painter;

    // Make sure it doesn't crash on setting or getting properties
    printer.setPrinterName("some non existing printer");
    printer.setPageSize(QPrinter::A4);
    printer.setOrientation(QPrinter::Portrait);
    printer.setFullPage(true);
    printer.pageSize();
    printer.orientation();
    printer.fullPage();
    printer.setCopyCount(1);
    printer.printerName();

    // nor metrics
    QCOMPARE(printer.printEngine()->metric(QPaintDevice::PdmWidth), 0);
    QCOMPARE(printer.printEngine()->metric(QPaintDevice::PdmHeight), 0);
    QCOMPARE(printer.printEngine()->metric(QPaintDevice::PdmWidthMM), 0);
    QCOMPARE(printer.printEngine()->metric(QPaintDevice::PdmHeightMM), 0);
    QCOMPARE(printer.printEngine()->metric(QPaintDevice::PdmNumColors), 0);
    QCOMPARE(printer.printEngine()->metric(QPaintDevice::PdmDepth), 0);
    QCOMPARE(printer.printEngine()->metric(QPaintDevice::PdmDpiX), 0);
    QCOMPARE(printer.printEngine()->metric(QPaintDevice::PdmDpiY), 0);
    QCOMPARE(printer.printEngine()->metric(QPaintDevice::PdmPhysicalDpiX), 0);
    QCOMPARE(printer.printEngine()->metric(QPaintDevice::PdmPhysicalDpiY), 0);

    QVERIFY(!painter.begin(&printer));
#endif
}


void tst_QPrinter::testMulitpleSets_data()
{
    QTest::addColumn<int>("resolution");
    QTest::addColumn<int>("pageSize");
    QTest::addColumn<int>("widthMMAfter");
    QTest::addColumn<int>("heightMMAfter");


    QTest::newRow("lowRes") << int(QPrinter::ScreenResolution) << int(QPrinter::A4) << 210 << 297;
    QTest::newRow("lowResLetter") << int(QPrinter::ScreenResolution) << int(QPrinter::Letter) << 216 << 279;
    QTest::newRow("lowResA5") << int(QPrinter::ScreenResolution) << int(QPrinter::A5) << 148 << 210;
    QTest::newRow("midRes") << int(QPrinter::PrinterResolution) << int(QPrinter::A4) << 210 << 297;
    QTest::newRow("midResLetter") << int(QPrinter::PrinterResolution) << int(QPrinter::Letter) << 216 << 279;
    QTest::newRow("midResA5") << int(QPrinter::PrinterResolution) << int(QPrinter::A5) << 148 << 210;
    QTest::newRow("highRes") << int(QPrinter::HighResolution) << int(QPrinter::A4) << 210 << 297;
    QTest::newRow("highResLetter") << int(QPrinter::HighResolution) << int(QPrinter::Letter) << 216 << 279;
    QTest::newRow("highResA5") << int(QPrinter::HighResolution) << int(QPrinter::A5) << 148 << 210;
}

static void computePageValue(const QPrinter &printer, int &retWidth, int &retHeight)
{
    const double Inch2MM = 25.4;

    double width = double(printer.paperRect().width()) / printer.logicalDpiX() * Inch2MM;
    double height = double(printer.paperRect().height()) / printer.logicalDpiY() * Inch2MM;
    retWidth = qRound(width);
    retHeight = qRound(height);
}

void tst_QPrinter::testMulitpleSets()
{
    // A very simple test, but Mac needs to have its format "validated" if the format is changed
    // This takes care of that.
    QFETCH(int, resolution);
    QFETCH(int, pageSize);
    QFETCH(int, widthMMAfter);
    QFETCH(int, heightMMAfter);


    QPrinter::PrinterMode mode = QPrinter::PrinterMode(resolution);
    QPrinter::PageSize printerPageSize = QPrinter::PageSize(pageSize);
    QPrinter printer(mode);
    printer.setFullPage(true);

    int paperWidth, paperHeight;
    //const int Tolerance = 2;

    computePageValue(printer, paperWidth, paperHeight);
    printer.setPageSize(printerPageSize);

    if (printer.pageSize() != printerPageSize) {
        QSKIP("Current page size is not supported on this printer", SkipSingle);
        return;
    }

    QCOMPARE(printer.widthMM(), widthMMAfter);
    QCOMPARE(printer.heightMM(), heightMMAfter);

    computePageValue(printer, paperWidth, paperHeight);

    QVERIFY(qAbs(paperWidth - widthMMAfter) <= 2);
    QVERIFY(qAbs(paperHeight - heightMMAfter) <= 2);

    // Set it again and see if it still works.
    printer.setPageSize(printerPageSize);
    QCOMPARE(printer.widthMM(), widthMMAfter);
    QCOMPARE(printer.heightMM(), heightMMAfter);

    printer.setOrientation(QPrinter::Landscape);
    computePageValue(printer, paperWidth, paperHeight);
    QVERIFY(qAbs(paperWidth - heightMMAfter) <= 2);
    QVERIFY(qAbs(paperHeight - widthMMAfter) <= 2);
}

void tst_QPrinter::changingOutputFormat()
{
    QPrinter p;
    p.setOutputFormat(QPrinter::PostScriptFormat);
    p.setPageSize(QPrinter::A8);
    p.setOutputFormat(QPrinter::PdfFormat);
    QCOMPARE(p.pageSize(), QPrinter::A8);
}

void tst_QPrinter::outputFormatFromSuffix()
{
    if (QPrinterInfo::availablePrinters().size() == 0)
        QSKIP("No printers available.", SkipAll);
    QPrinter p;
    QVERIFY(p.outputFormat() == QPrinter::NativeFormat);
    p.setOutputFileName("test.ps");
    QVERIFY(p.outputFormat() == QPrinter::PostScriptFormat);
    p.setOutputFileName("test.pdf");
    QVERIFY(p.outputFormat() == QPrinter::PdfFormat);
    p.setOutputFileName(QString());
    QVERIFY(p.outputFormat() == QPrinter::NativeFormat);
}

void tst_QPrinter::setGetPaperSize()
{
    QPrinter p;
    p.setOutputFormat(QPrinter::PdfFormat);
    QSizeF size(500, 10);
    p.setPaperSize(size, QPrinter::Millimeter);
    QCOMPARE(p.paperSize(QPrinter::Millimeter), size);
    QSizeF ptSize = p.paperSize(QPrinter::Point);
    //qDebug() << ptSize;
    QVERIFY(qAbs(ptSize.width() - size.width() * (72/25.4)) < 1E-4);
    QVERIFY(qAbs(ptSize.height() - size.height() * (72/25.4)) < 1E-4);
}

void tst_QPrinter::testPageMargins_data()
{
    QTest::addColumn<qreal>("left");
    QTest::addColumn<qreal>("top");
    QTest::addColumn<qreal>("right");
    QTest::addColumn<qreal>("bottom");
    QTest::addColumn<int>("unit");

    QTest::newRow("data0") << 5.5 << 6.5 << 7.5 << 8.5 << static_cast<int>(QPrinter::Millimeter);
    QTest::newRow("data1") << 5.5 << 6.5 << 7.5 << 8.5 << static_cast<int>(QPrinter::Point);
    QTest::newRow("data2") << 5.5 << 6.5 << 7.5 << 8.5 << static_cast<int>(QPrinter::Inch);
    QTest::newRow("data3") << 5.5 << 6.5 << 7.5 << 8.5 << static_cast<int>(QPrinter::Pica);
    QTest::newRow("data4") << 5.5 << 6.5 << 7.5 << 8.5 << static_cast<int>(QPrinter::Didot);
    QTest::newRow("data5") << 5.5 << 6.5 << 7.5 << 8.5 << static_cast<int>(QPrinter::Cicero);
}

void tst_QPrinter::testPageMargins()
{
    QPrinter obj1;

    qreal toMillimeters[6];
    toMillimeters[QPrinter::Millimeter] = 1;
    toMillimeters[QPrinter::Point] = 0.352777778;
    toMillimeters[QPrinter::Inch] = 25.4;
    toMillimeters[QPrinter::Pica] = 4.23333333;
    toMillimeters[QPrinter::Didot] = 0.376;
    toMillimeters[QPrinter::Cicero] = 4.51166667;

    QFETCH(qreal, left);
    QFETCH(qreal, top);
    QFETCH(qreal, right);
    QFETCH(qreal, bottom);
    QFETCH(int, unit);

    qreal nLeft, nTop, nRight, nBottom;

    obj1.setPageMargins(left, top, right, bottom, static_cast<QPrinter::Unit>(unit));

    qreal tolerance = 0.05;

    obj1.getPageMargins(&nLeft, &nTop, &nRight, &nBottom, QPrinter::Millimeter);
    QVERIFY(fabs(left*toMillimeters[unit] - nLeft*toMillimeters[QPrinter::Millimeter]) < tolerance);
    QVERIFY(fabs(top*toMillimeters[unit] - nTop*toMillimeters[QPrinter::Millimeter]) < tolerance);
    QVERIFY(fabs(right*toMillimeters[unit] - nRight*toMillimeters[QPrinter::Millimeter]) < tolerance);
    QVERIFY(fabs(bottom*toMillimeters[unit] - nBottom*toMillimeters[QPrinter::Millimeter]) < tolerance);

    obj1.getPageMargins(&nLeft, &nTop, &nRight, &nBottom, QPrinter::Point);
    QVERIFY(fabs(left*toMillimeters[unit] - nLeft*toMillimeters[QPrinter::Point]) < tolerance);
    QVERIFY(fabs(top*toMillimeters[unit] - nTop*toMillimeters[QPrinter::Point]) < tolerance);
    QVERIFY(fabs(right*toMillimeters[unit] - nRight*toMillimeters[QPrinter::Point]) < tolerance);
    QVERIFY(fabs(bottom*toMillimeters[unit] - nBottom*toMillimeters[QPrinter::Point]) < tolerance);

    obj1.getPageMargins(&nLeft, &nTop, &nRight, &nBottom, QPrinter::Inch);
    QVERIFY(fabs(left*toMillimeters[unit] - nLeft*toMillimeters[QPrinter::Inch]) < tolerance);
    QVERIFY(fabs(top*toMillimeters[unit] - nTop*toMillimeters[QPrinter::Inch]) < tolerance);
    QVERIFY(fabs(right*toMillimeters[unit] - nRight*toMillimeters[QPrinter::Inch]) < tolerance);
    QVERIFY(fabs(bottom*toMillimeters[unit] - nBottom*toMillimeters[QPrinter::Inch]) < tolerance);

    obj1.getPageMargins(&nLeft, &nTop, &nRight, &nBottom, QPrinter::Pica);
    QVERIFY(fabs(left*toMillimeters[unit] - nLeft*toMillimeters[QPrinter::Pica]) < tolerance);
    QVERIFY(fabs(top*toMillimeters[unit] - nTop*toMillimeters[QPrinter::Pica]) < tolerance);
    QVERIFY(fabs(right*toMillimeters[unit] - nRight*toMillimeters[QPrinter::Pica]) < tolerance);
    QVERIFY(fabs(bottom*toMillimeters[unit] - nBottom*toMillimeters[QPrinter::Pica]) < tolerance);

    obj1.getPageMargins(&nLeft, &nTop, &nRight, &nBottom, QPrinter::Didot);
    QVERIFY(fabs(left*toMillimeters[unit] - nLeft*toMillimeters[QPrinter::Didot]) < tolerance);
    QVERIFY(fabs(top*toMillimeters[unit] - nTop*toMillimeters[QPrinter::Didot]) < tolerance);
    QVERIFY(fabs(right*toMillimeters[unit] - nRight*toMillimeters[QPrinter::Didot]) < tolerance);
    QVERIFY(fabs(bottom*toMillimeters[unit] - nBottom*toMillimeters[QPrinter::Didot]) < tolerance);

    obj1.getPageMargins(&nLeft, &nTop, &nRight, &nBottom, QPrinter::Cicero);
    QVERIFY(fabs(left*toMillimeters[unit] - nLeft*toMillimeters[QPrinter::Cicero]) < tolerance);
    QVERIFY(fabs(top*toMillimeters[unit] - nTop*toMillimeters[QPrinter::Cicero]) < tolerance);
    QVERIFY(fabs(right*toMillimeters[unit] - nRight*toMillimeters[QPrinter::Cicero]) < tolerance);
    QVERIFY(fabs(bottom*toMillimeters[unit] - nBottom*toMillimeters[QPrinter::Cicero]) < tolerance);
}

void tst_QPrinter::valuePreservation()
{
    QPrinter::OutputFormat oldFormat = QPrinter::PdfFormat;
    QPrinter::OutputFormat newFormat = QPrinter::PostScriptFormat;

    {
        QPrinter printer;
        printer.setOutputFormat(oldFormat);
        bool status = printer.collateCopies();
        printer.setOutputFormat(newFormat);
        printer.setOutputFormat(oldFormat);
        QCOMPARE(printer.collateCopies(), status);

        printer.setCollateCopies(!status);
        printer.setOutputFormat(newFormat);
        QCOMPARE(printer.collateCopies(), !status);
        printer.setOutputFormat(oldFormat);
        QCOMPARE(printer.collateCopies(), !status);
    }
    {
        QPrinter printer;
        printer.setOutputFormat(oldFormat);
        QPrinter::ColorMode status = printer.colorMode();
        printer.setOutputFormat(newFormat);
        printer.setOutputFormat(oldFormat);
        QCOMPARE(printer.colorMode(), status);

        printer.setColorMode(QPrinter::ColorMode(!status));
        printer.setOutputFormat(newFormat);
        QCOMPARE(printer.colorMode(), QPrinter::ColorMode(!status));
        printer.setOutputFormat(oldFormat);
        QCOMPARE(printer.colorMode(), QPrinter::ColorMode(!status));
    }
    {
        QPrinter printer;
        printer.setOutputFormat(oldFormat);
        QString status = printer.creator();
        printer.setOutputFormat(newFormat);
        printer.setOutputFormat(oldFormat);
        QCOMPARE(printer.creator(), status);

        status = QString::fromLatin1("Mr. Test");
        printer.setCreator(status);
        printer.setOutputFormat(newFormat);
        QCOMPARE(printer.creator(), status);
        printer.setOutputFormat(oldFormat);
        QCOMPARE(printer.creator(), status);
    }
    {
        QPrinter printer;
        printer.setOutputFormat(oldFormat);
        QString status = printer.docName();
        printer.setOutputFormat(newFormat);
        printer.setOutputFormat(oldFormat);
        QCOMPARE(printer.docName(), status);

        status = QString::fromLatin1("Test document");
        printer.setDocName(status);
        printer.setOutputFormat(newFormat);
        QCOMPARE(printer.docName(), status);
        printer.setOutputFormat(oldFormat);
        QCOMPARE(printer.docName(), status);
    }
    {
        QPrinter printer;
        printer.setOutputFormat(oldFormat);
        bool status = printer.doubleSidedPrinting();
        printer.setOutputFormat(newFormat);
        printer.setOutputFormat(oldFormat);
        QCOMPARE(printer.doubleSidedPrinting(), status);

        printer.setDoubleSidedPrinting(!status);
        printer.setOutputFormat(newFormat);
        QCOMPARE(printer.doubleSidedPrinting(), !status);
        printer.setOutputFormat(oldFormat);
        QCOMPARE(printer.doubleSidedPrinting(), !status);
    }
    {
        QPrinter printer;
        printer.setOutputFormat(oldFormat);
        bool status = printer.fontEmbeddingEnabled();
        printer.setOutputFormat(newFormat);
        printer.setOutputFormat(oldFormat);
        QCOMPARE(printer.fontEmbeddingEnabled(), status);

        printer.setFontEmbeddingEnabled(!status);
        printer.setOutputFormat(newFormat);
        QCOMPARE(printer.fontEmbeddingEnabled(), !status);
        printer.setOutputFormat(oldFormat);
        QCOMPARE(printer.fontEmbeddingEnabled(), !status);
    }
    {
        QPrinter printer;
        printer.setOutputFormat(oldFormat);
        bool status = printer.fullPage();
        printer.setOutputFormat(newFormat);
        printer.setOutputFormat(oldFormat);
        QCOMPARE(printer.fullPage(), status);

        printer.setFullPage(!status);
        printer.setOutputFormat(newFormat);
        QCOMPARE(printer.fullPage(), !status);
        printer.setOutputFormat(oldFormat);
        QCOMPARE(printer.fullPage(), !status);
    }
    {
        QPrinter printer;
        printer.setOutputFormat(oldFormat);
        QPrinter::Orientation status = printer.orientation();
        printer.setOutputFormat(newFormat);
        printer.setOutputFormat(oldFormat);
        QCOMPARE(printer.orientation(), status);

        printer.setOrientation(QPrinter::Orientation(!status));
        printer.setOutputFormat(newFormat);
        QCOMPARE(printer.orientation(), QPrinter::Orientation(!status));
        printer.setOutputFormat(oldFormat);
        QCOMPARE(printer.orientation(), QPrinter::Orientation(!status));
    }
    {
        QPrinter printer;
        printer.setOutputFormat(oldFormat);
        QString status = printer.outputFileName();
        printer.setOutputFormat(newFormat);
        printer.setOutputFormat(oldFormat);
        QCOMPARE(printer.outputFileName(), status);

        status = QString::fromLatin1("Test file");
        printer.setOutputFileName(status);
        printer.setOutputFormat(newFormat);
        QCOMPARE(printer.outputFileName(), status);
        printer.setOutputFormat(oldFormat);
        QCOMPARE(printer.outputFileName(), status);
    }
    {
        QPrinter printer;
        printer.setOutputFormat(oldFormat);
        QPrinter::PageOrder status = printer.pageOrder();
        printer.setOutputFormat(newFormat);
        printer.setOutputFormat(oldFormat);
        QCOMPARE(printer.pageOrder(), status);

        printer.setPageOrder(QPrinter::PageOrder(!status));
        printer.setOutputFormat(newFormat);
        QCOMPARE(printer.pageOrder(), QPrinter::PageOrder(!status));
        printer.setOutputFormat(oldFormat);
        QCOMPARE(printer.pageOrder(), QPrinter::PageOrder(!status));
    }
    {
        QPrinter printer;
        printer.setOutputFormat(oldFormat);
        QPrinter::PageSize status = printer.pageSize();
        printer.setOutputFormat(newFormat);
        printer.setOutputFormat(oldFormat);
        QCOMPARE(printer.pageSize(), status);

        printer.setPageSize(QPrinter::B5);
        printer.setOutputFormat(newFormat);
        QCOMPARE(printer.pageSize(), QPrinter::B5);
        printer.setOutputFormat(oldFormat);
        QCOMPARE(printer.pageSize(), QPrinter::B5);
    }
    {
        QPrinter printer;
        printer.setOutputFormat(oldFormat);
        QPrinter::PaperSource status = printer.paperSource();
        printer.setOutputFormat(newFormat);
        printer.setOutputFormat(oldFormat);
        QCOMPARE(printer.paperSource(), status);

        printer.setPaperSource(QPrinter::Manual);
        printer.setOutputFormat(newFormat);
        QCOMPARE(printer.paperSource(), QPrinter::Manual);
        printer.setOutputFormat(oldFormat);
        QCOMPARE(printer.paperSource(), QPrinter::Manual);
    }
    {
        QPrinter printer;
        printer.setOutputFormat(oldFormat);
        QString status = printer.printProgram();
        printer.setOutputFormat(newFormat);
        printer.setOutputFormat(oldFormat);
        QCOMPARE(printer.printProgram(), status);

        status = QString::fromLatin1("/usr/local/bin/lpr");
        printer.setPrintProgram(status);
        printer.setOutputFormat(newFormat);
        QCOMPARE(printer.printProgram(), status);
        printer.setOutputFormat(oldFormat);
        QCOMPARE(printer.printProgram(), status);
    }
    {
        QPrinter printer;
        printer.setOutputFormat(oldFormat);
        QPrinter::PrintRange status = printer.printRange();
        printer.setOutputFormat(newFormat);
        printer.setOutputFormat(oldFormat);
        QCOMPARE(printer.printRange(), status);

        printer.setPrintRange(QPrinter::PrintRange(!status));
        printer.setOutputFormat(newFormat);
        QCOMPARE(printer.printRange(), QPrinter::PrintRange(!status));
        printer.setOutputFormat(oldFormat);
        QCOMPARE(printer.printRange(), QPrinter::PrintRange(!status));
    }
    {
        QPrinter printer;
        printer.setOutputFormat(oldFormat);
        QString status = printer.printerName();
        printer.setOutputFormat(newFormat);
        printer.setOutputFormat(oldFormat);
        QCOMPARE(printer.printerName(), status);

        status = QString::fromLatin1("SuperDuperPrinter");
        printer.setPrinterName(status);
        printer.setOutputFormat(newFormat);
        QCOMPARE(printer.printerName(), status);
        printer.setOutputFormat(oldFormat);
        QCOMPARE(printer.printerName(), status);
    }
#ifndef Q_OS_WIN
    {
        QPrinter printer;
        printer.setOutputFormat(oldFormat);
        QString status = printer.printerSelectionOption();
        printer.setOutputFormat(newFormat);
        printer.setOutputFormat(oldFormat);
        QCOMPARE(printer.printerSelectionOption(), status);

        status = QString::fromLatin1("Optional option");
        printer.setPrinterSelectionOption(status);
        printer.setOutputFormat(newFormat);
        QCOMPARE(printer.printerSelectionOption(), status);
        printer.setOutputFormat(oldFormat);
        QCOMPARE(printer.printerSelectionOption(), status);
    }
#endif
    {
        QPrinter printer;
        printer.setOutputFormat(oldFormat);
        int status = printer.resolution();
        printer.setOutputFormat(newFormat);
        printer.setOutputFormat(oldFormat);
        QCOMPARE(printer.resolution(), status);

        printer.setResolution(status-150);
        printer.setOutputFormat(newFormat);
        QCOMPARE(printer.resolution(), status-150);
        printer.setOutputFormat(oldFormat);
        QCOMPARE(printer.resolution(), status-150);
    }
}

void tst_QPrinter::errorReporting()
{
    QPrinter p;
    p.setOutputFormat(QPrinter::PdfFormat);
    QCOMPARE(p.isValid(), true);
    QPainter painter;
#ifndef Q_WS_WIN
    // not sure how to choose a never-writable file on windows.  But its QFile behavior anyway, so lets rely on it failing elsewhere
    p.setOutputFileName("/foobar/nonwritable.pdf");
    QCOMPARE(painter.begin(&p), false); // it should check the output file is writable
#endif
    p.setOutputFileName("test.pdf");
    QCOMPARE(painter.begin(&p), true); // it should check the output
    QCOMPARE(p.isValid(), true);
    painter.end();
}

void tst_QPrinter::testCustomPageSizes()
{
    QPrinter p;

    QSizeF customSize(8.5, 11.0);
    p.setPaperSize(customSize, QPrinter::Inch);

    QSizeF paperSize = p.paperSize(QPrinter::Inch);
    QCOMPARE(paperSize, customSize);

    QPrinter p2(QPrinter::HighResolution);
    p2.setPaperSize(customSize, QPrinter::Inch);
    paperSize = p.paperSize(QPrinter::Inch);
    QCOMPARE(paperSize, customSize);
}

void tst_QPrinter::printDialogCompleter()
{
#if defined(QT_NO_COMPLETER) || defined(QT_NO_FILEDIALOG)
    QSKIP("QT_NO_COMPLETER || QT_NO_FILEDIALOG: Auto-complete turned off in QPrinterDialog.", QTest::SkipAll);
#else
    QPrintDialog dialog;
    dialog.printer()->setOutputFileName("file.pdf");
    dialog.setEnabledOptions(QAbstractPrintDialog::PrintToFile);
    dialog.show();

    QTest::qWait(100);

    QTest::keyClick(&dialog, Qt::Key_Tab);
    QTest::keyClick(&dialog, 'P');
    // The test passes if it doesn't crash.
#endif
}

void tst_QPrinter::testCopyCount()
{
    QPrinter p;
    p.setCopyCount(15);
    QCOMPARE(p.copyCount(), 15);
}

static void printPage(QPainter *painter)
{
    painter->setPen(QPen(Qt::black, 4));
    painter->drawRect(50, 60, 70, 80);
}

void tst_QPrinter::taskQTBUG4497_reusePrinterOnDifferentFiles()
{
    QPrinter printer;
    {

        printer.setOutputFileName("out1.ps");
        QPainter painter(&printer);
        printPage(&painter);

    }
    {

        printer.setOutputFileName("out2.ps");
        QPainter painter(&printer);
        printPage(&painter);

    }
    QFile file1("out1.ps");
    QVERIFY(file1.open(QIODevice::ReadOnly));

    QFile file2("out2.ps");
    QVERIFY(file2.open(QIODevice::ReadOnly));

    QCOMPARE(file1.readAll(), file2.readAll());
}

void tst_QPrinter::testCurrentPage()
{
    QPrinter printer;
    printer.setFromTo(1, 10);

    // Test set print range
    printer.setPrintRange(QPrinter::CurrentPage);
    QCOMPARE(printer.printRange(), QPrinter::CurrentPage);
    QCOMPARE(printer.fromPage(), 1);
    QCOMPARE(printer.toPage(), 10);

    QPrintDialog dialog(&printer);

    // Test default Current Page option to off
    QCOMPARE(dialog.isOptionEnabled(QPrintDialog::PrintCurrentPage), false);

    // Test enable Current Page option
    dialog.setOption(QPrintDialog::PrintCurrentPage);
    QCOMPARE(dialog.isOptionEnabled(QPrintDialog::PrintCurrentPage), true);

}

void tst_QPrinter::testPdfTitle()
{
    // Check the document name is represented correctly in produced pdf
    {
        QPainter painter;
        QPrinter printer;
        // This string is just the UTF-8 encoding of the string: \()f &oslash; hiragana o
        const char title[]={0x5c, 0x28, 0x29, 0x66, 0xc3, 0xb8, 0xe3, 0x81, 0x8a, 0x00};
        printer.setOutputFileName("file.pdf");
        printer.setDocName(QString::fromUtf8(title));
        painter.begin(&printer);
        painter.end();
    }
    QFile file("file.pdf");
    QVERIFY(file.open(QIODevice::ReadOnly));
    // The we expect the title to appear in the PDF as:
    // ASCII('\title (') UTF16(\\\(\)f &oslash; hiragana o) ASCII(')').
    // which has the following binary representation
    const char expected[] = {
        0x2f, 0x54, 0x69, 0x74, 0x6c, 0x65, 0x20, 0x28, 0xfe,
        0xff, 0x00, 0x5c, 0x5c, 0x00, 0x5c, 0x28, 0x00, 0x5c,
        0x29, 0x00, 0x66, 0x00, 0xf8, 0x30, 0x4a, 0x29};
    QVERIFY(file.readAll().contains(QByteArray(expected, 26)));
}

QTEST_MAIN(tst_QPrinter)
#include "tst_qprinter.moc"

#else //QT_NO_PRINTER

QTEST_NOOP_MAIN

#endif //QT_NO_PRINTER
