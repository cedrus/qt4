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
#include <QtGui/qgraphicseffect.h>
#include <QtGui/qgraphicsview.h>
#include <QtGui/qgraphicsscene.h>
#include <QtGui/qgraphicsitem.h>
#include <QtGui/qstyleoption.h>

#include <private/qgraphicseffect_p.h>

#include "../../shared/util.h"

//TESTED_CLASS=
//TESTED_FILES=

class CustomItem : public QGraphicsRectItem
{
public:
    CustomItem(qreal x, qreal y, qreal width, qreal height)
        : QGraphicsRectItem(x, y, width, height), numRepaints(0),
          m_painter(0)
    {}

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
    {
        m_painter = painter;
        ++numRepaints;
        QGraphicsRectItem::paint(painter, option, widget);
    }

    void reset()
    {
        numRepaints = 0;
        m_painter = 0;
    }

    int numRepaints;
    QPainter *m_painter;
};

class CustomEffect : public QGraphicsEffect
{
public:
    CustomEffect()
        : QGraphicsEffect(), numRepaints(0), m_margin(10), m_sourceChanged(false),
          m_sourceBoundingRectChanged(false), doNothingInDraw(false),
          storeDeviceDependentStuff(false),
          m_painter(0), m_source(0)
    {}

    QRectF boundingRectFor(const QRectF &rect) const
    { return rect.adjusted(-m_margin, -m_margin, m_margin, m_margin); }

    void reset()
    {
        numRepaints = 0;
        m_sourceChanged = false;
        m_sourceBoundingRectChanged = false;
        m_painter = 0;
        m_source = 0;
        deviceCoordinatesPixmap = QPixmap();
        deviceRect = QRect();
        sourceDeviceBoundingRect = QRectF();
    }

    void setMargin(int margin)
    {
        m_margin = margin;
        updateBoundingRect();
    }

    int margin() const
    { return m_margin; }

    void draw(QPainter *painter)
    {
        ++numRepaints;
        if (storeDeviceDependentStuff) {
            deviceCoordinatesPixmap = source()->pixmap(Qt::DeviceCoordinates);
            deviceRect = QRect(0, 0, painter->device()->width(), painter->device()->height());
            sourceDeviceBoundingRect = source()->boundingRect(Qt::DeviceCoordinates);
        }
        if (doNothingInDraw)
            return;
        m_source = source();
        m_painter = painter;
        source()->draw(painter);
    }

    void sourceChanged()
    { m_sourceChanged = true; }

    void sourceBoundingRectChanged()
    { m_sourceBoundingRectChanged = true; }

    int numRepaints;
    int m_margin;
    bool m_sourceChanged;
    bool m_sourceBoundingRectChanged;
    bool doNothingInDraw;
    bool storeDeviceDependentStuff;
    QPainter *m_painter;
    QGraphicsEffectSource *m_source;
    QPixmap deviceCoordinatesPixmap;
    QRect deviceRect;
    QRectF sourceDeviceBoundingRect;
};

class tst_QGraphicsEffectSource : public QObject
{
    Q_OBJECT
public slots:
    void initTestCase();
    void cleanupTestCase();
    void init();

private slots:
    void graphicsItem();
    void styleOption();
    void isPixmap();
    void draw();
    void update();
    void boundingRect();
    void deviceRect();
    void pixmap();

    void pixmapPadding_data();
    void pixmapPadding();

private:
    QGraphicsView *view;
    QGraphicsScene *scene;
    CustomItem *item;
    CustomEffect *effect;
};

void tst_QGraphicsEffectSource::initTestCase()
{
    scene = new QGraphicsScene;
    item = new CustomItem(0, 0, 100, 100);
    effect = new CustomEffect;
    item->setGraphicsEffect(effect);
    scene->addItem(item);
    view = new QGraphicsView(scene);
    view->show();
#ifdef Q_WS_X11
    qt_x11_wait_for_window_manager(view);
#endif
    QTest::qWait(200);
}

void tst_QGraphicsEffectSource::cleanupTestCase()
{
    delete view;
}

void tst_QGraphicsEffectSource::init()
{
    QVERIFY(effect);
    QVERIFY(item);
    QVERIFY(effect->source());
    effect->reset();
    effect->storeDeviceDependentStuff = false;
    effect->doNothingInDraw = false;
    item->reset();
}

void tst_QGraphicsEffectSource::graphicsItem()
{
    QVERIFY(effect->source());
    QCOMPARE(effect->source()->graphicsItem(), (const QGraphicsItem*)item);
}

void tst_QGraphicsEffectSource::styleOption()
{
    // We don't have style options unless the source is drawing.
    QVERIFY(effect->source());
    QVERIFY(!effect->source()->styleOption());

    // Trigger an update and check that the style option in QGraphicsEffect::draw
    // was the same as the one in QGraphicsItem::paint.
    QCOMPARE(item->numRepaints, 0);
    QCOMPARE(effect->numRepaints, 0);
    item->update();
    QTRY_COMPARE(item->numRepaints, 1);
    QTRY_COMPARE(effect->numRepaints, 1);
}

void tst_QGraphicsEffectSource::isPixmap()
{
    // Current source is a CustomItem (which is not a pixmap item).
    QVERIFY(!effect->source()->isPixmap());

    // Make sure isPixmap() returns true for QGraphicsPixmapItem.
    QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem;
    CustomEffect *anotherEffect = new CustomEffect;
    pixmapItem->setGraphicsEffect(anotherEffect);
    QVERIFY(anotherEffect->source());
    QCOMPARE(anotherEffect->source()->graphicsItem(), static_cast<const QGraphicsItem *>(pixmapItem));
    QVERIFY(anotherEffect->source()->isPixmap());
    delete pixmapItem;
}

void tst_QGraphicsEffectSource::draw()
{
    // The source can only draw as a result of QGraphicsEffect::draw.
    QTest::ignoreMessage(QtWarningMsg, "QGraphicsEffectSource::draw: Can only begin as a result of QGraphicsEffect::draw");
    QPixmap dummyPixmap(10, 10);
    QPainter dummyPainter(&dummyPixmap);
    effect->source()->draw(&dummyPainter);
}

void tst_QGraphicsEffectSource::update()
{
    QCOMPARE(item->numRepaints, 0);
    QCOMPARE(effect->numRepaints, 0);

    effect->source()->update();

    QTRY_COMPARE(item->numRepaints, 1);
    QTRY_COMPARE(effect->numRepaints, 1);
}

void tst_QGraphicsEffectSource::boundingRect()
{
    QTest::ignoreMessage(QtWarningMsg, "QGraphicsEffectSource::boundingRect: Not yet implemented, lacking device context");
    QCOMPARE(effect->source()->boundingRect(Qt::DeviceCoordinates), QRectF());

    QRectF itemBoundingRect = item->boundingRect();
    if (!item->children().isEmpty())
        itemBoundingRect |= item->childrenBoundingRect();

    // We can at least check that the device bounding rect was correct in QGraphicsEffect::draw.
    effect->storeDeviceDependentStuff = true;
    effect->source()->update();
    const QTransform deviceTransform = item->deviceTransform(view->viewportTransform());
    QTRY_COMPARE(effect->sourceDeviceBoundingRect, deviceTransform.mapRect(itemBoundingRect));

    // Bounding rect in logical coordinates is of course fine.
    QTRY_COMPARE(effect->source()->boundingRect(Qt::LogicalCoordinates), itemBoundingRect);
    // Make sure default value is Qt::LogicalCoordinates.
    QTRY_COMPARE(effect->source()->boundingRect(), itemBoundingRect);
}

void tst_QGraphicsEffectSource::deviceRect()
{
    effect->storeDeviceDependentStuff = true;
    effect->source()->update();
    QTRY_COMPARE(effect->deviceRect, view->viewport()->rect());
}

void tst_QGraphicsEffectSource::pixmap()
{
    QTest::ignoreMessage(QtWarningMsg, "QGraphicsEffectSource::pixmap: Not yet implemented, lacking device context");
    QCOMPARE(effect->source()->pixmap(Qt::DeviceCoordinates), QPixmap());

    // We can at least verify a valid pixmap from QGraphicsEffect::draw.
    effect->storeDeviceDependentStuff = true;
    effect->source()->update();
    QTRY_VERIFY(!effect->deviceCoordinatesPixmap.isNull());

    // Pixmaps in logical coordinates we can do fine.
    QPixmap pixmap1 = effect->source()->pixmap(Qt::LogicalCoordinates);
    QVERIFY(!pixmap1.isNull());

    // Make sure default value is Qt::LogicalCoordinates.
    QPixmap pixmap2 = effect->source()->pixmap();
    QCOMPARE(pixmap1, pixmap2);
}

class PaddingEffect : public QGraphicsEffect
{
public:
    PaddingEffect(QObject *parent) : QGraphicsEffect(parent)
    {
    }

    QRectF boundingRectFor(const QRectF &src) const {
        return src.adjusted(-10, -10, 10, 10);
    }

    void draw(QPainter *) {
        pix = source()->pixmap(coordinateMode, &offset, padMode);
    }

    QPixmap pix;
    QPoint offset;
    QGraphicsEffect::PixmapPadMode padMode;
    Qt::CoordinateSystem coordinateMode;
};

void tst_QGraphicsEffectSource::pixmapPadding_data()
{
    QTest::addColumn<int>("coordinateMode");
    QTest::addColumn<int>("padMode");
    QTest::addColumn<QSize>("size");
    QTest::addColumn<QPoint>("offset");
    QTest::addColumn<uint>("ulPixel");

    QTest::newRow("log,nopad") << int(Qt::LogicalCoordinates)
                               << int(QGraphicsEffect::NoPad)
                               << QSize(10, 10) << QPoint(0, 0)
                               << 0xffff0000u;

    QTest::newRow("log,transparent") << int(Qt::LogicalCoordinates)
                                     << int(QGraphicsEffect::PadToTransparentBorder)
                                     << QSize(14, 14) << QPoint(-2, -2)
                                     << 0x00000000u;

    QTest::newRow("log,effectrect") << int(Qt::LogicalCoordinates)
                                    << int(QGraphicsEffect::PadToEffectiveBoundingRect)
                                    << QSize(20, 20) << QPoint(-5, -5)
                                    << 0x00000000u;

    QTest::newRow("dev,nopad") << int(Qt::DeviceCoordinates)
                               << int(QGraphicsEffect::NoPad)
                               << QSize(20, 20) << QPoint(40, 40)
                               << 0xffff0000u;

    QTest::newRow("dev,transparent") << int(Qt::DeviceCoordinates)
                                     << int(QGraphicsEffect::PadToTransparentBorder)
                                     << QSize(24, 24) << QPoint(38, 38)
                                     << 0x00000000u;

    QTest::newRow("dev,effectrect") << int(Qt::DeviceCoordinates)
                                    << int(QGraphicsEffect::PadToEffectiveBoundingRect)
                                    << QSize(40, 40) << QPoint(30, 30)
                                    << 0x00000000u;

}

void tst_QGraphicsEffectSource::pixmapPadding()
{
    QPixmap dummyTarget(100, 100);
    QPainter dummyPainter(&dummyTarget);
    dummyPainter.translate(40, 40);
    dummyPainter.scale(2, 2);

    QPixmap pm(10, 10);
    pm.fill(Qt::red);

    QGraphicsScene *scene = new QGraphicsScene();
    PaddingEffect *effect = new PaddingEffect(scene);
    QGraphicsPixmapItem *pmItem = new QGraphicsPixmapItem(pm);
    scene->addItem(pmItem);
    pmItem->setGraphicsEffect(effect);

    QFETCH(int, coordinateMode);
    QFETCH(int, padMode);
    QFETCH(QPoint, offset);
    QFETCH(QSize, size);
    QFETCH(uint, ulPixel);

    effect->padMode = (QGraphicsEffect::PixmapPadMode) padMode;
    effect->coordinateMode = (Qt::CoordinateSystem) coordinateMode;

    scene->render(&dummyPainter, scene->itemsBoundingRect(), scene->itemsBoundingRect());

    QCOMPARE(effect->pix.size(), size);
    QCOMPARE(effect->offset, offset);
    QCOMPARE(effect->pix.toImage().pixel(0, 0), ulPixel);

    // ### Fix corruption in scene destruction, then enable...
    // delete scene;
}

QTEST_MAIN(tst_QGraphicsEffectSource)
#include "tst_qgraphicseffectsource.moc"

