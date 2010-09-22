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
#include <qfont.h>
#include <qfontmetrics.h>
#include <qfontdatabase.h>
#include <qstringlist.h>
#include <qlist.h>



//TESTED_CLASS=
//TESTED_FILES=

class tst_QFontMetrics : public QObject
{
Q_OBJECT

public:
    tst_QFontMetrics();
    virtual ~tst_QFontMetrics();

public slots:
    void init();
    void cleanup();
private slots:
    void same();
    void metrics();
    void boundingRect();
    void elidedText_data();
    void elidedText();
    void veryNarrowElidedText();
    void averageCharWidth();
    void bypassShaping();
    void elidedMultiLength();
    void elidedMultiLengthF();
};

tst_QFontMetrics::tst_QFontMetrics()

{
}

tst_QFontMetrics::~tst_QFontMetrics()
{

}

void tst_QFontMetrics::init()
{
}

void tst_QFontMetrics::cleanup()
{
}

void tst_QFontMetrics::same()
{
    QFont font;
    font.setBold(true);
    QFontMetrics fm(font);
    const QString text = QLatin1String("Some stupid STRING");
    QCOMPARE(fm.size(0, text), fm.size(0, text)) ;

    {
        QImage image;
        QFontMetrics fm2(font, &image);
        QString text2 =  QLatin1String("Foo Foo");
        QCOMPARE(fm2.size(0, text2), fm2.size(0, text2));  //used to crash
    }

    {
        QImage image;
        QFontMetricsF fm3(font, &image);
        QString text2 =  QLatin1String("Foo Foo");
        QCOMPARE(fm3.size(0, text2), fm3.size(0, text2));  //used to crash
    }
}


void tst_QFontMetrics::metrics()
{
    QFont font;
    QFontDatabase fdb;

    // Query the QFontDatabase for a specific font, store the
    // result in family, style and size.
    QStringList families = fdb.families();
    if (families.isEmpty())
        return;

    QStringList::ConstIterator f_it, f_end = families.end();
    for (f_it = families.begin(); f_it != f_end; ++f_it) {
        const QString &family = *f_it;

        QStringList styles = fdb.styles(family);
        QStringList::ConstIterator s_it, s_end = styles.end();
        for (s_it = styles.begin(); s_it != s_end; ++s_it) {
            const QString &style = *s_it;

            if (fdb.isSmoothlyScalable(family, style)) {
                // smoothly scalable font... don't need to load every pointsize
                font = fdb.font(family, style, 12);

                QFontMetrics fontmetrics(font);
                QCOMPARE(fontmetrics.ascent() + fontmetrics.descent() + 1,
                        fontmetrics.height());

                QCOMPARE(fontmetrics.height() + fontmetrics.leading(),
                        fontmetrics.lineSpacing());
            } else {
                QList<int> sizes = fdb.pointSizes(family, style);
                QVERIFY(!sizes.isEmpty());
                QList<int>::ConstIterator z_it, z_end = sizes.end();
                for (z_it = sizes.begin(); z_it != z_end; ++z_it) {
                    const int size = *z_it;

                    // Initialize the font, and check if it is an exact match
                    font = fdb.font(family, style, size);

                    QFontMetrics fontmetrics(font);
                    QCOMPARE(fontmetrics.ascent() + fontmetrics.descent() + 1,
                            fontmetrics.height());
                    QCOMPARE(fontmetrics.height() + fontmetrics.leading(),
                            fontmetrics.lineSpacing());
                }
            }
	}
    }
}

void tst_QFontMetrics::boundingRect()
{
    QFont f;
    f.setPointSize(24);
    QFontMetrics fm(f);
    QRect r = fm.boundingRect(QChar('Y'));
    QVERIFY(r.top() < 0);
    r = fm.boundingRect(QString("Y"));
    QVERIFY(r.top() < 0);
}

void tst_QFontMetrics::elidedText_data()
{
    QTest::addColumn<QFont>("font");
    QTest::addColumn<QString>("text");

    QTest::newRow("helvetica hello") << QFont("helvetica",10) << QString("hello") ;
    QTest::newRow("helvetica hello &Bye") << QFont("helvetica",10) << QString("hello&Bye") ;
}


void tst_QFontMetrics::elidedText()
{
    QFETCH(QFont, font);
    QFETCH(QString, text);
    QFontMetrics fm(font);
    int w = fm.width(text);
    QString newtext = fm.elidedText(text,Qt::ElideRight,w+1, 0);
    QCOMPARE(text,newtext); // should not elide
    newtext = fm.elidedText(text,Qt::ElideRight,w-1, 0);
    QVERIFY(text != newtext); // should elide
}

void tst_QFontMetrics::veryNarrowElidedText()
{
    QFont f;
    QFontMetrics fm(f);
    QString text("hello");
    QCOMPARE(fm.elidedText(text, Qt::ElideRight, 0), QString());
}

void tst_QFontMetrics::averageCharWidth()
{
    QFont f;
    QFontMetrics fm(f);
    QVERIFY(fm.averageCharWidth() != 0);
    QFontMetricsF fmf(f);
    QVERIFY(fmf.averageCharWidth() != 0);
}

void tst_QFontMetrics::bypassShaping()
{
    QFont f;
    f.setStyleStrategy(QFont::ForceIntegerMetrics);
    QFontMetrics fm(f);
    QString text = " A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z, a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z";
    int textWidth = fm.width(text, -1, Qt::TextBypassShaping);
    QVERIFY(textWidth != 0);
    int charsWidth = 0;
    for (int i = 0; i < text.size(); ++i)
        charsWidth += fm.width(text[i]);
    // This assertion is needed in QtWebKit's WebCore::Font::offsetForPositionForSimpleText
    QCOMPARE(textWidth, charsWidth);
}

template<class FontMetrics> void elidedMultiLength_helper()
{
    QString text1 = "Long Text 1\x9cShorter\x9csmall";
    QString text1_long = "Long Text 1";
    QString text1_short = "Shorter";
    QString text1_small = "small";
    FontMetrics fm = FontMetrics(QFont());
    int width_long = fm.size(0, text1_long).width();
    QCOMPARE(fm.elidedText(text1,Qt::ElideRight, 8000), text1_long);
    QCOMPARE(fm.elidedText(text1,Qt::ElideRight, width_long + 1), text1_long);
    QCOMPARE(fm.elidedText(text1,Qt::ElideRight, width_long - 1), text1_short);
    int width_short = fm.size(0, text1_short).width();
    QCOMPARE(fm.elidedText(text1,Qt::ElideRight, width_short + 1), text1_short);
    QCOMPARE(fm.elidedText(text1,Qt::ElideRight, width_short - 1), text1_small);

    // Not even wide enough for "small" - should use ellipsis
    QChar ellipsisChar(0x2026);
    QString text1_el = QString::fromLatin1("s") + ellipsisChar;
    int width_small = fm.width(text1_el);
    QCOMPARE(fm.elidedText(text1,Qt::ElideRight, width_small + 1), text1_el);
}

void tst_QFontMetrics::elidedMultiLength()
{
    elidedMultiLength_helper<QFontMetrics>();
}

void tst_QFontMetrics::elidedMultiLengthF()
{
    elidedMultiLength_helper<QFontMetricsF>();
}

QTEST_MAIN(tst_QFontMetrics)
#include "tst_qfontmetrics.moc"
