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
#include <QtGui/qgraphicsscene.h>
#include <private/qgraphicsscenebsptreeindex_p.h>
#include <private/qgraphicssceneindex_p.h>
#include <private/qgraphicsscenelinearindex_p.h>
#include "../../shared/util.h"


//TESTED_CLASS=
//TESTED_FILES=

class tst_QGraphicsSceneIndex : public QObject
{
    Q_OBJECT
public slots:
    void initTestCase();

private slots:
    void customIndex_data();
    void customIndex();
    void scatteredItems_data();
    void scatteredItems();
    void overlappedItems_data();
    void overlappedItems();
    void movingItems_data();
    void movingItems();
    void connectedToSceneRectChanged();
    void items();
    void removeItems();
    void clear();

private:
    void common_data();
    QGraphicsSceneIndex *createIndex(const QString &name);
};

void tst_QGraphicsSceneIndex::initTestCase()
{
}

void tst_QGraphicsSceneIndex::common_data()
{
    QTest::addColumn<QString>("indexMethod");

    QTest::newRow("BSP") << QString("bsp");
    QTest::newRow("Linear") << QString("linear");
}

QGraphicsSceneIndex *tst_QGraphicsSceneIndex::createIndex(const QString &indexMethod)
{
    QGraphicsSceneIndex *index = 0;
    QGraphicsScene *scene = new QGraphicsScene();
    if (indexMethod == "bsp")
        index = new QGraphicsSceneBspTreeIndex(scene);

    if (indexMethod == "linear")
        index = new QGraphicsSceneLinearIndex(scene);

    return index;
}

void tst_QGraphicsSceneIndex::customIndex_data()
{
    common_data();
}

void tst_QGraphicsSceneIndex::customIndex()
{
#if 0
    QFETCH(QString, indexMethod);
    QGraphicsSceneIndex *index = createIndex(indexMethod);

    QGraphicsScene scene;
    scene.setSceneIndex(index);

    scene.addRect(0, 0, 30, 40);
    QCOMPARE(scene.items(QRectF(0, 0, 10, 10)).count(), 1);
#endif
}

void tst_QGraphicsSceneIndex::scatteredItems_data()
{
    common_data();
}

void tst_QGraphicsSceneIndex::scatteredItems()
{
    QFETCH(QString, indexMethod);

    QGraphicsScene scene;
#if 1
    scene.setItemIndexMethod(indexMethod == "linear" ? QGraphicsScene::NoIndex : QGraphicsScene::BspTreeIndex);
#else
    QGraphicsSceneIndex *index = createIndex(indexMethod);
    scene.setSceneIndex(index);
#endif

    for (int i = 0; i < 10; ++i)
        scene.addRect(i*50, i*50, 40, 35);

    QCOMPARE(scene.items(QPointF(5, 5)).count(), 1);
    QCOMPARE(scene.items(QPointF(55, 55)).count(), 1);
    QCOMPARE(scene.items(QPointF(-100, -100)).count(), 0);

    QCOMPARE(scene.items(QRectF(0, 0, 10, 10)).count(), 1);
    QCOMPARE(scene.items(QRectF(0, 0, 1000, 1000)).count(), 10);
    QCOMPARE(scene.items(QRectF(-100, -1000, 0, 0)).count(), 0);
}

void tst_QGraphicsSceneIndex::overlappedItems_data()
{
    common_data();
}

void tst_QGraphicsSceneIndex::overlappedItems()
{
    QFETCH(QString, indexMethod);

    QGraphicsScene scene;
#if 1
    scene.setItemIndexMethod(indexMethod == "linear" ? QGraphicsScene::NoIndex : QGraphicsScene::BspTreeIndex);
#else
    QGraphicsSceneIndex *index = createIndex(indexMethod);
    scene.setSceneIndex(index);
#endif

    for (int i = 0; i < 10; ++i)
        for (int j = 0; j < 10; ++j)
            scene.addRect(i*50, j*50, 200, 200);

    QCOMPARE(scene.items(QPointF(5, 5)).count(), 1);
    QCOMPARE(scene.items(QPointF(55, 55)).count(), 4);
    QCOMPARE(scene.items(QPointF(105, 105)).count(), 9);
    QCOMPARE(scene.items(QPointF(-100, -100)).count(), 0);

    QCOMPARE(scene.items(QRectF(0, 0, 1000, 1000)).count(), 100);
    QCOMPARE(scene.items(QRectF(-100, -1000, 0, 0)).count(), 0);
    QCOMPARE(scene.items(QRectF(0, 0, 200, 200)).count(), 16);
    QCOMPARE(scene.items(QRectF(0, 0, 100, 100)).count(), 4);
    QCOMPARE(scene.items(QRectF(0, 0, 1, 100)).count(), 2);
    QCOMPARE(scene.items(QRectF(0, 0, 1, 1000)).count(), 10);
}

void tst_QGraphicsSceneIndex::movingItems_data()
{
    common_data();
}

void tst_QGraphicsSceneIndex::movingItems()
{
    QFETCH(QString, indexMethod);

    QGraphicsScene scene;
#if 1
    scene.setItemIndexMethod(indexMethod == "linear" ? QGraphicsScene::NoIndex : QGraphicsScene::BspTreeIndex);
#else
    QGraphicsSceneIndex *index = createIndex(indexMethod);
    scene.setSceneIndex(index);
#endif

    for (int i = 0; i < 10; ++i)
        scene.addRect(i*50, i*50, 40, 35);

    QGraphicsRectItem *box = scene.addRect(0, 0, 10, 10);
    QCOMPARE(scene.items(QPointF(5, 5)).count(), 2);
    QCOMPARE(scene.items(QPointF(-1, -1)).count(), 0);
    QCOMPARE(scene.items(QRectF(0, 0, 5, 5)).count(), 2);

    box->setPos(10, 10);
    QCOMPARE(scene.items(QPointF(9, 9)).count(), 1);
    QCOMPARE(scene.items(QPointF(15, 15)).count(), 2);
    QCOMPARE(scene.items(QRectF(0, 0, 1, 1)).count(), 1);

    box->setPos(-5, -5);
    QCOMPARE(scene.items(QPointF(-1, -1)).count(), 1);
    QCOMPARE(scene.items(QRectF(0, 0, 1, 1)).count(), 2);

    QCOMPARE(scene.items(QRectF(0, 0, 1000, 1000)).count(), 11);
}

void tst_QGraphicsSceneIndex::connectedToSceneRectChanged()
{

    class MyScene : public QGraphicsScene
    {
    public:
#ifdef Q_CC_RVCT
        //using keyword doesn't change visibility to public in RVCT2.2 compiler
        inline int receivers(const char* signal) const
        {
            return QGraphicsScene::receivers(signal);
        }
#else
        using QGraphicsScene::receivers;
#endif
    };

    MyScene scene; // Uses QGraphicsSceneBspTreeIndex by default.
    QCOMPARE(scene.receivers(SIGNAL(sceneRectChanged(const QRectF&))), 1);

    scene.setItemIndexMethod(QGraphicsScene::NoIndex); // QGraphicsSceneLinearIndex
    QCOMPARE(scene.receivers(SIGNAL(sceneRectChanged(const QRectF&))), 1);
}

void tst_QGraphicsSceneIndex::items()
{
    QGraphicsScene scene;
    QGraphicsItem *item1 = scene.addRect(0, 0, 10, 10);
    QGraphicsItem *item2 = scene.addRect(10, 10, 10, 10);
    QCOMPARE(scene.items().size(), 2);

    // Move from unindexed items into bsp tree.
    QTest::qWait(50);
    QCOMPARE(scene.items().size(), 2);

    // Add untransformable item.
    QGraphicsItem *item3 = new QGraphicsRectItem(QRectF(20, 20, 10, 10));
    item3->setFlag(QGraphicsItem::ItemIgnoresTransformations);
    scene.addItem(item3);
    QCOMPARE(scene.items().size(), 3);

    // Move from unindexed items into untransformable items.
    QTest::qWait(50);
    QCOMPARE(scene.items().size(), 3);

    // Move from untransformable items into unindexed items.
    item3->setFlag(QGraphicsItem::ItemIgnoresTransformations, false);
    QCOMPARE(scene.items().size(), 3);
    QTest::qWait(50);
    QCOMPARE(scene.items().size(), 3);

    // Make all items untransformable.
    item1->setFlag(QGraphicsItem::ItemIgnoresTransformations);
    item2->setParentItem(item1);
    item3->setParentItem(item2);
    QCOMPARE(scene.items().size(), 3);

    // Move from unindexed items into untransformable items.
    QTest::qWait(50);
    QCOMPARE(scene.items().size(), 3);
}

class RectWidget : public QGraphicsWidget
{
    Q_OBJECT
public:
    RectWidget(QGraphicsItem *parent = 0) : QGraphicsWidget(parent)
    {
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
    {
        painter->setBrush(brush);
        painter->drawRect(boundingRect());
    }
public:
    QBrush brush;
};

void tst_QGraphicsSceneIndex::removeItems()
{
     QGraphicsScene scene;

    RectWidget *parent = new RectWidget;
    parent->brush = QBrush(QColor(Qt::magenta));
    parent->setGeometry(250, 250, 400, 400);

    RectWidget *widget = new RectWidget(parent);
    widget->brush = QBrush(QColor(Qt::blue));
    widget->setGeometry(10, 10, 200, 200);

    RectWidget *widgetChild1 = new RectWidget(widget);
    widgetChild1->brush = QBrush(QColor(Qt::green));
    widgetChild1->setGeometry(20, 20, 100, 100);

    RectWidget *widgetChild2 = new RectWidget(widgetChild1);
    widgetChild2->brush = QBrush(QColor(Qt::yellow));
    widgetChild2->setGeometry(25, 25, 50, 50);

    scene.addItem(parent);

    QGraphicsView view(&scene);
    view.resize(600, 600);
    view.show();
    QApplication::setActiveWindow(&view);
    QTest::qWaitForWindowShown(&view);

    QApplication::processEvents();

    scene.removeItem(widgetChild1);

    delete widgetChild1;

    //We move the parent
    scene.items(295, 295, 50, 50);

    //This should not crash
}

void tst_QGraphicsSceneIndex::clear()
{
    class MyItem : public QGraphicsItem
    {
    public:
        MyItem(QGraphicsItem *parent = 0) : QGraphicsItem(parent), numPaints(0) {}
        int numPaints;
    protected:
        QRectF boundingRect() const { return QRectF(0, 0, 10, 10); }
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
        { ++numPaints; }
    };

    QGraphicsScene scene;
    scene.setSceneRect(0, 0, 100, 100);
    scene.addItem(new MyItem);

    QGraphicsView view(&scene);
    view.show();
#ifdef Q_WS_X11
    qt_x11_wait_for_window_manager(&view);
#endif
    QTest::qWait(250);
    scene.clear();

    // Make sure the index is re-generated after QGraphicsScene::clear();
    // otherwise no items will be painted.
    MyItem *item = new MyItem;
    scene.addItem(item);
    qApp->processEvents();
    QTRY_COMPARE(item->numPaints, 1);
}

QTEST_MAIN(tst_QGraphicsSceneIndex)
#include "tst_qgraphicssceneindex.moc"
