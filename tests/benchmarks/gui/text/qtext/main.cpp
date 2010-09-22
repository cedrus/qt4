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

#include <QDebug>
#include <QTextDocument>
#include <QTextDocumentWriter>
#include <QTextLayout>
#include <QTextCursor>
#include <private/qtextcontrol_p.h>
#include <qmath.h>
#include <QFile>
#include <QPainter>
#include <QBuffer>
#include <qtest.h>

#ifdef Q_OS_SYMBIAN
// In Symbian OS test data is located in applications private dir
// Application private dir is default serach path for files, so SRCDIR can be set to empty
#define SRCDIR ""
#endif

Q_DECLARE_METATYPE(QTextDocument*)

class tst_QText: public QObject
{
    Q_OBJECT
public:
    tst_QText() {
        m_lorem = QString::fromLatin1("Lorem ipsum dolor sit amet, consectetuer adipiscing elit, sed diam nonummy nibh euismod tincidunt ut laoreet dolore magna aliquam erat volutpat. Ut wisi enim ad minim veniam, quis nostrud exerci tation ullamcorper suscipit lobortis nisl ut aliquip ex ea commodo consequat. Duis autem vel eum iriure dolor in hendrerit in vulputate velit esse molestie consequat, vel illum dolore eu feugiat nulla facilisis at vero eros et accumsan et iusto odio dignissim qui blandit praesent luptatum zzril delenit augue duis dolore te feugait nulla facilisi.");
        m_shortLorem = QString::fromLatin1("Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.");
    }

private slots:
    void loadHtml_data();
    void loadHtml();

    void shaping_data();
    void shaping();

    void odfWriting_empty();
    void odfWriting_text();
    void odfWriting_images();

    void constructControl();
    void constructDocument();

    void newLineReplacement();
    void formatManipulation();

    void layout_data();
    void layout();
    void paintLayoutToPixmap();
    void paintLayoutToPixmap_painterFill();

    void document();
    void paintDocToPixmap();
    void paintDocToPixmap_painterFill();

    void control();
    void paintControlToPixmap();
    void paintControlToPixmap_painterFill();

private:
    QSize setupTextLayout(QTextLayout *layout, bool wrap = true, int wrapWidth = 100);

    QString m_lorem;
    QString m_shortLorem;
};

void tst_QText::loadHtml_data()
{
    QTest::addColumn<QString>("source");
    QTest::newRow("empty") << QString();
    QTest::newRow("simple") << QString::fromLatin1("<html><b>Foo</b></html>");
    QTest::newRow("simple2") << QString::fromLatin1("<b>Foo</b>");

    QString parag = QString::fromLatin1("<p>%1</p>").arg(m_lorem);
    QString header = QString::fromLatin1("<html><head><title>test</title></head><body>");
    QTest::newRow("long") << QString::fromLatin1("<html><head><title>test</title></head><body>") + parag + parag + parag
        + parag + parag + parag + parag + parag + parag + parag + parag + parag + parag + parag + parag + parag + parag
        + QString::fromLatin1("</html>");
    QTest::newRow("table") <<  header + QLatin1String("<table border=\"1\"1><tr><td>xx</td></tr><tr><td colspan=\"2\">")
        + parag + QLatin1String("</td></tr></table></html");
    QTest::newRow("crappy") <<  header + QLatin1String("<table border=\"1\"1><tr><td>xx</td></tr><tr><td colspan=\"2\">")
        + parag;
}

void tst_QText::loadHtml()
{
    QFETCH(QString, source);
    QTextDocument doc;
    QBENCHMARK {
        doc.setHtml(source);
    }
}

void tst_QText::shaping_data()
{
    QTest::addColumn<QString>("parag");
    QTest::newRow("empty") << QString();
    QTest::newRow("lorem") << m_lorem;
    QTest::newRow("short") << QString::fromLatin1("Lorem ipsum dolor sit amet");

#if !defined(Q_OS_SYMBIAN)
    QFile file(QString::fromLatin1(SRCDIR) + QLatin1String("/bidi.txt"));
#else
    QFile file( SRCDIR "bidi.txt" );
#endif
    QVERIFY(file.open(QFile::ReadOnly));
    QByteArray data = file.readAll();
    QVERIFY(data.count() > 1000);
    QStringList list = QString::fromUtf8(data.data()).split(QLatin1Char('\n'), QString::SkipEmptyParts);
    QVERIFY(list.count() %2 == 0); // even amount as we have title and then content.
    for (int i=0; i < list.count(); i+=2) {
        QTest::newRow(list.at(i).toLatin1()) << list.at(i+1);
    }
}

void tst_QText::shaping()
{
    QFETCH(QString, parag);

    QTextLayout lay(parag);
    lay.setCacheEnabled(false);

    // do one run to make sure any fonts are loaded.
    lay.beginLayout();
    lay.createLine();
    lay.endLayout();

    QBENCHMARK {
        lay.beginLayout();
        lay.createLine();
        lay.endLayout();
    }
}

void tst_QText::odfWriting_empty()
{
    QVERIFY(QTextDocumentWriter::supportedDocumentFormats().contains("ODF")); // odf compiled in
    QTextDocument *doc = new QTextDocument();
    // write it
    QBENCHMARK {
        QBuffer buffer;
        buffer.open(QIODevice::WriteOnly);
        QTextDocumentWriter writer(&buffer, "ODF");
        writer.write(doc);
    }
    delete doc;
}

void tst_QText::odfWriting_text()
{
    QTextDocument *doc = new QTextDocument();
    QTextCursor cursor(doc);
    QTextBlockFormat bf;
    bf.setIndent(2);
    cursor.insertBlock(bf);
    cursor.insertText(m_lorem);
    bf.setTopMargin(10);
    cursor.insertBlock(bf);
    cursor.insertText(m_lorem);
    bf.setRightMargin(30);
    cursor.insertBlock(bf);
    cursor.insertText(m_lorem);

    // write it
    QBENCHMARK {
        QBuffer buffer;
        buffer.open(QIODevice::WriteOnly);
        QTextDocumentWriter writer(&buffer, "ODF");
        writer.write(doc);
    }
    delete doc;
}

void tst_QText::odfWriting_images()
{
    QTextDocument *doc = new QTextDocument();
    QTextCursor cursor(doc);
    cursor.insertText(m_lorem);
    QImage image(400, 200, QImage::Format_ARGB32_Premultiplied);
    cursor.insertImage(image);
    cursor.insertText(m_lorem);

    // write it
    QBENCHMARK {
        QBuffer buffer;
        buffer.open(QIODevice::WriteOnly);
        QTextDocumentWriter writer(&buffer, "ODF");
        writer.write(doc);
    }
    delete doc;
}

QSize tst_QText::setupTextLayout(QTextLayout *layout, bool wrap, int wrapWidth)
{
    layout->setCacheEnabled(true);

    int height = 0;
    qreal widthUsed = 0;
    qreal lineWidth = 0;

    //set manual width
    if (wrap)
        lineWidth = wrapWidth;

    layout->beginLayout();
    while (1) {
        QTextLine line = layout->createLine();
        if (!line.isValid())
            break;

        if (wrap)
            line.setLineWidth(lineWidth);
    }
    layout->endLayout();

    for (int i = 0; i < layout->lineCount(); ++i) {
        QTextLine line = layout->lineAt(i);
        widthUsed = qMax(widthUsed, line.naturalTextWidth());
        line.setPosition(QPointF(0, height));
        height += int(line.height());
    }
    return QSize(qCeil(widthUsed), height);
}

void tst_QText::constructControl()
{
    QTextControl *control = new QTextControl;
    delete control;

    QBENCHMARK {
        QTextControl *control = new QTextControl;
        delete control;
    }
}

void tst_QText::constructDocument()
{
    QTextDocument *doc = new QTextDocument;
    delete doc;

    QBENCHMARK {
        QTextDocument *doc = new QTextDocument;
        delete doc;
    }
}

//this step is needed before giving the string to a QTextLayout
void tst_QText::newLineReplacement()
{
    QString text = QString::fromLatin1("H\ne\nl\nl\no\n\nW\no\nr\nl\nd");

    QBENCHMARK {
        QString tmp = text;
        tmp.replace(QLatin1Char('\n'), QChar::LineSeparator);
    }
}

void tst_QText::formatManipulation()
{
    QFont font;

    QBENCHMARK {
        QTextCharFormat format;
        format.setFont(font);
    }
}

void tst_QText::layout_data()
{
    QTest::addColumn<bool>("wrap");
    QTest::newRow("wrap") << true;
    QTest::newRow("nowrap") << false;
}

void tst_QText::layout()
{
    QFETCH(bool,wrap);
    QTextLayout layout(m_shortLorem);
    setupTextLayout(&layout, wrap);

    QBENCHMARK {
        QTextLayout layout(m_shortLorem);
        setupTextLayout(&layout, wrap);
    }
}

//### requires tst_QText to be a friend of QTextLayout
/*void tst_QText::stackTextLayout()
{
    QStackTextEngine engine(m_shortLorem, qApp->font());
    QTextLayout layout(&engine);
    setupTextLayout(&layout);

    QBENCHMARK {
        QStackTextEngine engine(m_shortLorem, qApp->font());
        QTextLayout layout(&engine);
        setupTextLayout(&layout);
    }
}*/

void tst_QText::paintLayoutToPixmap()
{
    QTextLayout layout(m_shortLorem);
    QSize size = setupTextLayout(&layout);

    QBENCHMARK {
        QPixmap img(size);
        img.fill(Qt::transparent);
        QPainter p(&img);
        layout.draw(&p, QPointF(0, 0));
    }
}

void tst_QText::paintLayoutToPixmap_painterFill()
{
    QTextLayout layout(m_shortLorem);
    QSize size = setupTextLayout(&layout);

    QBENCHMARK {
        QPixmap img(size);
        QPainter p(&img);
        p.setCompositionMode(QPainter::CompositionMode_Source);
        p.fillRect(0, 0, img.width(), img.height(), Qt::transparent);
        p.setCompositionMode(QPainter::CompositionMode_SourceOver);
        layout.draw(&p, QPointF(0, 0));
    }
}

void tst_QText::document()
{
    QTextDocument *doc = new QTextDocument;

    QBENCHMARK {
        QTextDocument *doc = new QTextDocument;
        doc->setHtml(m_shortLorem);
    }
}

void tst_QText::paintDocToPixmap()
{
    QTextDocument *doc = new QTextDocument;
    doc->setHtml(m_shortLorem);
    doc->setTextWidth(300);
    QSize size = doc->size().toSize();

    QBENCHMARK {
        QPixmap img(size);
        img.fill(Qt::transparent);
        QPainter p(&img);
        doc->drawContents(&p);
    }
}

void tst_QText::paintDocToPixmap_painterFill()
{
    QTextDocument *doc = new QTextDocument;
    doc->setHtml(m_shortLorem);
    doc->setTextWidth(300);
    QSize size = doc->size().toSize();

    QBENCHMARK {
        QPixmap img(size);
        QPainter p(&img);
        p.setCompositionMode(QPainter::CompositionMode_Source);
        p.fillRect(0, 0, img.width(), img.height(), Qt::transparent);
        p.setCompositionMode(QPainter::CompositionMode_SourceOver);
        doc->drawContents(&p);
    }
}

void tst_QText::control()
{
    QTextControl *control = new QTextControl(m_shortLorem);

    QBENCHMARK {
        QTextControl *control = new QTextControl;
        QTextDocument *doc = control->document();
        doc->setHtml(m_shortLorem);
    }
}

void tst_QText::paintControlToPixmap()
{
    QTextControl *control = new QTextControl;
    QTextDocument *doc = control->document();
    doc->setHtml(m_shortLorem);
    doc->setTextWidth(300);
    QSize size = doc->size().toSize();

    QBENCHMARK {
        QPixmap img(size);
        img.fill(Qt::transparent);
        QPainter p(&img);
        control->drawContents(&p, QRectF(QPointF(0, 0), QSizeF(size)));
    }
}

void tst_QText::paintControlToPixmap_painterFill()
{
    QTextControl *control = new QTextControl;
    QTextDocument *doc = control->document();
    doc->setHtml(m_shortLorem);
    doc->setTextWidth(300);
    QSize size = doc->size().toSize();

    QBENCHMARK {
        QPixmap img(size);
        QPainter p(&img);
        p.setCompositionMode(QPainter::CompositionMode_Source);
        p.fillRect(0, 0, img.width(), img.height(), Qt::transparent);
        p.setCompositionMode(QPainter::CompositionMode_SourceOver);
        control->drawContents(&p, QRectF(QPointF(0, 0), QSizeF(size)));
    }
}

QTEST_MAIN(tst_QText)

#include "main.moc"
