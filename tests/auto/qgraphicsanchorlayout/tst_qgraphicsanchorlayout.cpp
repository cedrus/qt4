/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the test suite of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions
** contained in the Technology Preview License Agreement accompanying
** this package.
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
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
**
**
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtTest/QtTest>
#include <QtGui/qgraphicsanchorlayout.h>
#include <private/qgraphicsanchorlayout_p.h>
#include <QtGui/qgraphicswidget.h>
#include <QtGui/qgraphicsproxywidget.h>
#include <QtGui/qgraphicsview.h>

class tst_QGraphicsAnchorLayout : public QObject {
    Q_OBJECT;

private slots:
    void simple();
    void simple_center();
    void simple_semifloat();
    void layoutDirection();
    void diagonal();
    void parallel();
    void parallel2();
    void snake();
    void snakeOppositeDirections();
    void fairDistribution();
    void fairDistributionOppositeDirections();
    void proportionalPreferred();
    void example();
    void setSpacing();
    void hardComplexS60();
    void stability();
    void delete_anchor();
    void conflicts();
    void sizePolicy();
};

class RectWidget : public QGraphicsWidget
{
public:
    RectWidget(QGraphicsItem *parent = 0) : QGraphicsWidget(parent){}

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
    {
        Q_UNUSED(option);
        Q_UNUSED(widget);
        painter->drawRoundRect(rect());
        painter->drawLine(rect().topLeft(), rect().bottomRight());
        painter->drawLine(rect().bottomLeft(), rect().topRight());
    }
};

static QGraphicsWidget *createItem(const QSizeF &minimum = QSizeF(100.0, 100.0),
                                   const QSizeF &preferred = QSize(150.0, 100.0),
                                   const QSizeF &maximum = QSizeF(200.0, 100.0),
                                   const QString &name = QString())
{
    QGraphicsWidget *w = new RectWidget;
    w->setMinimumSize(minimum);
    w->setPreferredSize(preferred);
    w->setMaximumSize(maximum);
    w->setData(0, name);
    return w;
}

static void setAnchor(QGraphicsAnchorLayout *l,
                      QGraphicsLayoutItem *firstItem,
                      Qt::AnchorPoint firstEdge,
                      QGraphicsLayoutItem *secondItem,
                      Qt::AnchorPoint secondEdge,
                      qreal spacing = 0)
{
    QGraphicsAnchor *anchor = l->addAnchor(firstItem, firstEdge, secondItem, secondEdge);
    anchor->setSpacing(spacing);
}

static bool checkReverseDirection(QGraphicsWidget *w)
{
    QGraphicsLayout *l = w->layout();
    Q_ASSERT(l);
    qreal left, top, right, bottom;
    l->getContentsMargins(&left, &top, &right, &bottom);
    w->setLayoutDirection(Qt::LeftToRight);
    QApplication::processEvents();
    const QRectF lg = l->geometry();
    QMap<QGraphicsLayoutItem *, QRectF> geometries;
    for (int i = 0; i < l->count(); ++i) {
        QGraphicsLayoutItem *w = l->itemAt(i);
        geometries.insert(w, w->geometry());
    }
    w->setLayoutDirection(Qt::RightToLeft);
    QApplication::processEvents();
    lg.adjusted(+right, +top, -left, -bottom);
    for (int i = 0; i < l->count(); ++i) {
        QGraphicsLayoutItem *w = l->itemAt(i);
        const QRectF rtlGeom = w->geometry();
        const QRectF ltrGeom = geometries.value(w);
        QRectF expectedGeom = ltrGeom;
        expectedGeom.moveRight(lg.right() - (0 + ltrGeom.left()));
        if (expectedGeom != rtlGeom) {
            qDebug() << "layout->geometry():" << lg
                     << "expected:" << expectedGeom
                     << "actual:" << rtlGeom;
            return false;
        }
    }
    return true;
}

static bool layoutHasConflict(QGraphicsAnchorLayout *l)
{
    return QGraphicsAnchorLayoutPrivate::get(l)->hasConflicts();
}

void tst_QGraphicsAnchorLayout::simple()
{
    QGraphicsWidget *w1 = createItem();
    QGraphicsWidget *w2 = createItem();

    QGraphicsAnchorLayout *l = new QGraphicsAnchorLayout;
    l->setContentsMargins(0, 0, 0, 0);
    l->addAnchor(w1, Qt::AnchorRight, w2, Qt::AnchorLeft);

    QGraphicsWidget p;
    p.setLayout(l);

    QCOMPARE(l->count(), 2);
}

void tst_QGraphicsAnchorLayout::simple_center()
{
    QSizeF min(10, 10);
    QSizeF pref(50, 10);
    QSizeF max(100, 10);

    QGraphicsWidget *a = createItem(min, pref, max, "a");
    QGraphicsWidget *b = createItem(min, pref, max, "b");
    QGraphicsWidget *c = createItem(min, pref, max, "c");

    QGraphicsAnchorLayout *l = new QGraphicsAnchorLayout;
    l->setContentsMargins(0, 0, 0, 0);
    // horizontal
    setAnchor(l, l, Qt::AnchorLeft, a, Qt::AnchorLeft, 0);
    setAnchor(l, a, Qt::AnchorRight, b, Qt::AnchorLeft, 0);
    setAnchor(l, b, Qt::AnchorRight, l, Qt::AnchorRight, 0);
    setAnchor(l, a, Qt::AnchorHorizontalCenter, c, Qt::AnchorLeft, 0);
    setAnchor(l, c, Qt::AnchorRight, b, Qt::AnchorHorizontalCenter, 0);

    // vertical
    setAnchor(l, l, Qt::AnchorTop, a, Qt::AnchorTop, 0);
    setAnchor(l, l, Qt::AnchorTop, b, Qt::AnchorTop, 0);
    setAnchor(l, a, Qt::AnchorBottom, c, Qt::AnchorTop, 0);
    setAnchor(l, b, Qt::AnchorBottom, c, Qt::AnchorTop, 0);
    setAnchor(l, c, Qt::AnchorBottom, l, Qt::AnchorBottom, 0);

    QCOMPARE(l->count(), 3);

    QGraphicsWidget *p = new QGraphicsWidget(0, Qt::Window);
    p->setLayout(l);

    QSizeF layoutMinimumSize = l->effectiveSizeHint(Qt::MinimumSize);
    QCOMPARE(layoutMinimumSize, QSizeF(20, 20));

    QSizeF layoutMaximumSize = l->effectiveSizeHint(Qt::MaximumSize);
    QCOMPARE(layoutMaximumSize, QSizeF(200, 20));

    delete p;
}

void tst_QGraphicsAnchorLayout::simple_semifloat()
{
    // Useful for testing simplification between A_left and B_left.
    // Unfortunately the only way to really test that now is to manually inspect the
    // simplified graph.
    QSizeF min(10, 10);
    QSizeF pref(50, 10);
    QSizeF max(100, 10);

    QGraphicsWidget *A = createItem(min, pref, max, "A");
    QGraphicsWidget *B = createItem(min, pref, max, "B");
    QGraphicsWidget *a = createItem(min, pref, max, "a");
    QGraphicsWidget *b = createItem(min, pref, max, "b");

    QGraphicsAnchorLayout *l = new QGraphicsAnchorLayout;
    l->setContentsMargins(0, 0, 0, 0);

    // horizontal
    setAnchor(l, l, Qt::AnchorLeft, A, Qt::AnchorLeft, 0);
    setAnchor(l, A, Qt::AnchorRight, B, Qt::AnchorLeft, 0);
    setAnchor(l, B, Qt::AnchorRight, l, Qt::AnchorRight, 0);

    setAnchor(l, A, Qt::AnchorLeft, a, Qt::AnchorLeft, 0);
    setAnchor(l, B, Qt::AnchorLeft, b, Qt::AnchorLeft, 0);

    // vertical
    setAnchor(l, l, Qt::AnchorTop, A, Qt::AnchorTop, 0);
    setAnchor(l, l, Qt::AnchorTop, B, Qt::AnchorTop, 0);
    setAnchor(l, A, Qt::AnchorBottom, a, Qt::AnchorTop, 0);
    setAnchor(l, B, Qt::AnchorBottom, b, Qt::AnchorTop, 0);
    setAnchor(l, a, Qt::AnchorBottom, l, Qt::AnchorBottom, 0);
    setAnchor(l, b, Qt::AnchorBottom, l, Qt::AnchorBottom, 0);

    QCOMPARE(l->count(), 4);

    QGraphicsWidget *p = new QGraphicsWidget(0, Qt::Window);
    p->setLayout(l);

    QSizeF layoutMinimumSize = l->effectiveSizeHint(Qt::MinimumSize);
    QCOMPARE(layoutMinimumSize, QSizeF(20, 20));

    QCOMPARE(l->effectiveSizeHint(Qt::PreferredSize), QSizeF(100, 20));

    QSizeF layoutMaximumSize = l->effectiveSizeHint(Qt::MaximumSize);
    QCOMPARE(layoutMaximumSize, QSizeF(200, 20));

    delete p;
}

void tst_QGraphicsAnchorLayout::layoutDirection()
{
    QSizeF min(10, 10);
    QSizeF pref(50, 10);
    QSizeF max(100, 10);

    QGraphicsWidget *a = createItem(min, pref, max, "a");
    QGraphicsWidget *b = createItem(min, pref, max, "b");
    QGraphicsWidget *c = createItem(min, pref, QSizeF(100, 20), "c");

    a->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    b->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    c->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);


    QGraphicsAnchorLayout *l = new QGraphicsAnchorLayout;
    l->setContentsMargins(0, 5, 10, 15);
    // horizontal
    setAnchor(l, l, Qt::AnchorLeft, a, Qt::AnchorLeft, 0);
    setAnchor(l, a, Qt::AnchorRight, b, Qt::AnchorLeft, 0);
    setAnchor(l, b, Qt::AnchorRight, l, Qt::AnchorRight, 0);
    setAnchor(l, a, Qt::AnchorHorizontalCenter, c, Qt::AnchorLeft, 0);
    setAnchor(l, c, Qt::AnchorRight, b, Qt::AnchorHorizontalCenter, 0);

    // vertical
    setAnchor(l, l, Qt::AnchorTop, a, Qt::AnchorTop, 0);
    setAnchor(l, l, Qt::AnchorTop, b, Qt::AnchorTop, 0);
    setAnchor(l, a, Qt::AnchorBottom, c, Qt::AnchorTop, 0);
    setAnchor(l, b, Qt::AnchorBottom, c, Qt::AnchorTop, 0);
    setAnchor(l, c, Qt::AnchorBottom, l, Qt::AnchorBottom, 0);

    QCOMPARE(l->count(), 3);

    QGraphicsWidget *p = new QGraphicsWidget(0, Qt::Window);
    p->setLayoutDirection(Qt::LeftToRight);
    p->setLayout(l);

    QGraphicsScene scene;
    QGraphicsView *view = new QGraphicsView(&scene);
    scene.addItem(p);
    p->show();
    view->show();

    QCOMPARE(checkReverseDirection(p), true);

    delete p;
    delete view;
}

void tst_QGraphicsAnchorLayout::diagonal()
{
    QSizeF min(10, 100);
    QSizeF pref(70, 100);
    QSizeF max(100, 100);

    QGraphicsWidget *a = createItem(min, pref, max, "A");
    QGraphicsWidget *b = createItem(min, pref, max, "B");
    QGraphicsWidget *c = createItem(min, pref, max, "C");
    QGraphicsWidget *d = createItem(min, pref, max, "D");
    QGraphicsWidget *e = createItem(min, pref, max, "E");

    QGraphicsAnchorLayout *l = new QGraphicsAnchorLayout;
    l->setContentsMargins(0, 0, 0, 0);
    l->setSpacing(0);

    // vertical
    l->addAnchor(a, Qt::AnchorTop, l, Qt::AnchorTop);
    l->addAnchor(b, Qt::AnchorTop, l, Qt::AnchorTop);
    l->addAnchor(c, Qt::AnchorTop, a, Qt::AnchorBottom);
    l->addAnchor(c, Qt::AnchorTop, b, Qt::AnchorBottom);
    l->addAnchor(c, Qt::AnchorBottom, d, Qt::AnchorTop);
    l->addAnchor(c, Qt::AnchorBottom, e, Qt::AnchorTop);
    l->addAnchor(d, Qt::AnchorBottom, l, Qt::AnchorBottom);
    l->addAnchor(e, Qt::AnchorBottom, l, Qt::AnchorBottom);

    // horizontal
    l->addAnchor(l, Qt::AnchorLeft, a, Qt::AnchorLeft);
    l->addAnchor(l, Qt::AnchorLeft, d, Qt::AnchorLeft);
    l->addAnchor(a, Qt::AnchorRight, b, Qt::AnchorLeft);

    l->addAnchor(a, Qt::AnchorRight, c, Qt::AnchorLeft);
    l->addAnchor(c, Qt::AnchorRight, e, Qt::AnchorLeft);

    l->addAnchor(b, Qt::AnchorRight, l, Qt::AnchorRight);
    l->addAnchor(e, Qt::AnchorRight, l, Qt::AnchorRight);
    l->addAnchor(d, Qt::AnchorRight, e, Qt::AnchorLeft);

    QCOMPARE(l->count(), 5);

    QGraphicsWidget p;
    p.setLayout(l);

    QSizeF layoutMinimumSize = l->effectiveSizeHint(Qt::MinimumSize);
    QSizeF layoutMaximumSize = l->effectiveSizeHint(Qt::MaximumSize);
    QSizeF layoutPreferredSize = l->effectiveSizeHint(Qt::PreferredSize);

    QCOMPARE(layoutMinimumSize, QSizeF(30.0, 300.0));
    QCOMPARE(layoutPreferredSize, QSizeF(170.0, 300.0));
    QCOMPARE(layoutMaximumSize, QSizeF(190.0, 300.0));

    p.resize(layoutMinimumSize);
    QCOMPARE(a->geometry(), QRectF(0.0, 0.0, 10.0, 100.0));
    QCOMPARE(b->geometry(), QRectF(10.0, 0.0, 20.0, 100.0));
    QCOMPARE(c->geometry(), QRectF(10.0, 100.0, 10.0, 100.0));
    QCOMPARE(d->geometry(), QRectF(0.0, 200.0, 20.0, 100.0));
    QCOMPARE(e->geometry(), QRectF(20.0, 200.0, 10.0, 100.0));
    QCOMPARE(p.size(), layoutMinimumSize);

    p.resize(layoutPreferredSize);
    QCOMPARE(a->geometry(), QRectF(0.0, 0.0, 70.0, 100.0));
    QCOMPARE(b->geometry(), QRectF(70.0, 0.0, 100.0, 100.0));
    QCOMPARE(c->geometry(), QRectF(70.0, 100.0, 30.0, 100.0));
    QCOMPARE(d->geometry(), QRectF(0.0, 200.0, 100.0, 100.0));
    QCOMPARE(e->geometry(), QRectF(100.0, 200.0, 70.0, 100.0));
    QCOMPARE(p.size(), layoutPreferredSize);

    p.resize(layoutMaximumSize);
    QCOMPARE(a->geometry(), QRectF(0.0, 0.0, 90.0, 100.0));
    QCOMPARE(b->geometry(), QRectF(90.0, 0.0, 100.0, 100.0));
    QCOMPARE(c->geometry(), QRectF(90.0, 100.0, 10.0, 100.0));
    QCOMPARE(d->geometry(), QRectF(0.0, 200.0, 100.0, 100.0));
    QCOMPARE(e->geometry(), QRectF(100.0, 200.0, 90.0, 100.0));
    QCOMPARE(p.size(), layoutMaximumSize);

    QSizeF testA(175.0, 300.0);
    p.resize(testA);
    QCOMPARE(a->geometry(), QRectF(0.0, 0.0, 75.0, 100.0));
    QCOMPARE(b->geometry(), QRectF(75.0, 0.0, 100.0, 100.0));
    QCOMPARE(c->geometry(), QRectF(75.0, 100.0, 25.0, 100.0));
    QCOMPARE(d->geometry(), QRectF(0.0, 200.0, 100.0, 100.0));
    QCOMPARE(e->geometry(), QRectF(100.0, 200.0, 75.0, 100.0));
    QCOMPARE(p.size(), testA);

    QCOMPARE(checkReverseDirection(&p), true);

    c->setMinimumWidth(300);
    QVERIFY(layoutHasConflict(l));
}

void tst_QGraphicsAnchorLayout::parallel()
{
    QGraphicsWidget *a = createItem(QSizeF(100, 100),
                                    QSizeF(150, 100),
                                    QSizeF(200, 100), "A");

    QGraphicsWidget *b = createItem(QSizeF(100, 100),
                                    QSizeF(150, 100),
                                    QSizeF(300, 100), "B");

    QGraphicsWidget *c = createItem(QSizeF(100, 100),
                                    QSizeF(200, 100),
                                    QSizeF(350, 100), "C");

    QGraphicsWidget *d = createItem(QSizeF(100, 100),
                                    QSizeF(170, 100),
                                    QSizeF(200, 100), "D");

    QGraphicsWidget *e = createItem(QSizeF(150, 100),
                                    QSizeF(150, 100),
                                    QSizeF(200, 100), "E");

    QGraphicsWidget *f = createItem(QSizeF(100, 100),
                                    QSizeF(150, 100),
                                    QSizeF(200, 100), "F");

    QGraphicsAnchorLayout *l = new QGraphicsAnchorLayout;
    l->setContentsMargins(0, 0, 0, 0);
    l->setSpacing(0);

    l->addAnchor(l, Qt::AnchorTop, a, Qt::AnchorTop);
    l->addAnchor(a, Qt::AnchorBottom, b, Qt::AnchorTop);
    l->addAnchor(b, Qt::AnchorBottom, c, Qt::AnchorTop);
    l->addAnchor(c, Qt::AnchorBottom, d, Qt::AnchorTop);
    l->addAnchor(d, Qt::AnchorBottom, e, Qt::AnchorTop);
    l->addAnchor(e, Qt::AnchorBottom, f, Qt::AnchorTop);
    l->addAnchor(f, Qt::AnchorBottom, l, Qt::AnchorBottom);

    l->addAnchor(l, Qt::AnchorLeft, a, Qt::AnchorLeft);
    l->addAnchor(a, Qt::AnchorRight, b, Qt::AnchorLeft);
    l->addAnchor(a, Qt::AnchorRight, c, Qt::AnchorLeft);
    l->addAnchor(b, Qt::AnchorRight, d, Qt::AnchorLeft);
    l->addAnchor(b, Qt::AnchorRight, e, Qt::AnchorLeft);
    l->addAnchor(c, Qt::AnchorRight, f, Qt::AnchorLeft);
    l->addAnchor(d, Qt::AnchorRight, f, Qt::AnchorLeft);
    l->addAnchor(e, Qt::AnchorRight, f, Qt::AnchorLeft);
    l->addAnchor(f, Qt::AnchorRight, l, Qt::AnchorRight);

    QCOMPARE(l->count(), 6);

    QGraphicsWidget p;
    p.setLayout(l);

    QSizeF layoutMinimumSize = l->effectiveSizeHint(Qt::MinimumSize);
    QSizeF layoutPreferredSize = l->effectiveSizeHint(Qt::PreferredSize);
    QSizeF layoutMaximumSize = l->effectiveSizeHint(Qt::MaximumSize);

    QCOMPARE(layoutMinimumSize, QSizeF(450, 600));
    QCOMPARE(layoutPreferredSize, QSizeF(620, 600));
    QCOMPARE(layoutMaximumSize, QSizeF(750, 600));

    p.resize(layoutMinimumSize);
    QCOMPARE(a->geometry(), QRectF(0, 0, 100, 100));
    QCOMPARE(b->geometry(), QRectF(100, 100, 100, 100));
    QCOMPARE(c->geometry(), QRectF(100, 200, 250, 100));
    QCOMPARE(d->geometry(), QRectF(200, 300, 150, 100));
    QCOMPARE(e->geometry(), QRectF(200, 400, 150, 100));
    QCOMPARE(f->geometry(), QRectF(350, 500, 100, 100));
    QCOMPARE(p.size(), layoutMinimumSize);

    p.resize(layoutPreferredSize);
    QCOMPARE(a->geometry(), QRectF(0, 0, 150, 100));
    QCOMPARE(b->geometry(), QRectF(150, 100, 150, 100));
    QCOMPARE(c->geometry(), QRectF(150, 200, 320, 100));
    QCOMPARE(d->geometry(), QRectF(300, 300, 170, 100));
    QCOMPARE(e->geometry(), QRectF(300, 400, 170, 100));
    QCOMPARE(f->geometry(), QRectF(470, 500, 150, 100));
    QCOMPARE(p.size(), layoutPreferredSize);

    // Maximum size depends on simplification / fair distribution
    // Without that, test may or may not pass, depending on the
    // solution found by the solver at runtime.
    p.resize(layoutMaximumSize);
    QCOMPARE(a->geometry(), QRectF(0, 0, 200, 100));
    QCOMPARE(b->geometry(), QRectF(200, 100, 175, 100));
    QCOMPARE(c->geometry(), QRectF(200, 200, 350, 100));
    QCOMPARE(d->geometry(), QRectF(375, 300, 175, 100));
    QCOMPARE(e->geometry(), QRectF(375, 400, 175, 100));
    QCOMPARE(f->geometry(), QRectF(550, 500, 200, 100));
    QCOMPARE(p.size(), layoutMaximumSize);
}

void tst_QGraphicsAnchorLayout::parallel2()
{
    QGraphicsWidget *a = createItem(QSizeF(70.0, 100.0),
                                    QSizeF(100.0, 100.0),
                                    QSizeF(200.0, 100.0), "A");

    QGraphicsWidget *b = createItem(QSizeF(100.0, 100.0),
                                    QSizeF(150.0, 100.0),
                                    QSizeF(190.0, 100.0), "B");

    QGraphicsAnchorLayout *l = new QGraphicsAnchorLayout;
    l->setContentsMargins(0, 0, 0, 0);
    l->setSpacing(0);

    l->addAnchor(l, Qt::AnchorTop, a, Qt::AnchorTop);
    l->addAnchor(a, Qt::AnchorBottom, b, Qt::AnchorTop);
    l->addAnchor(b, Qt::AnchorBottom, l, Qt::AnchorBottom);

    l->addAnchors(l, a, Qt::Horizontal);
    l->addAnchor(l, Qt::AnchorLeft, b, Qt::AnchorLeft);
    l->addAnchor(b, Qt::AnchorRight, a, Qt::AnchorRight);

    QCOMPARE(l->count(), 2);

    QGraphicsWidget p;
    p.setLayout(l);

    QSizeF layoutMinimumSize = l->effectiveSizeHint(Qt::MinimumSize);
    QSizeF layoutPreferredSize = l->effectiveSizeHint(Qt::PreferredSize);
    QSizeF layoutMaximumSize = l->effectiveSizeHint(Qt::MaximumSize);

    QCOMPARE(layoutMinimumSize, QSizeF(100.0, 200.0));
    QCOMPARE(layoutPreferredSize, QSizeF(150.0, 200.0));
    QCOMPARE(layoutMaximumSize, QSizeF(190.0, 200.0));

    p.resize(layoutMinimumSize);
    QCOMPARE(p.size(), layoutMinimumSize);

    p.resize(layoutPreferredSize);
    QCOMPARE(p.size(), layoutPreferredSize);

    p.resize(layoutMaximumSize);
    QCOMPARE(p.size(), layoutMaximumSize);
}

void tst_QGraphicsAnchorLayout::snake()
{
    QGraphicsWidget *a = createItem(QSizeF(50.0, 100.0),
                                    QSizeF(70.0, 100.0),
                                    QSizeF(100.0, 100.0), "A");

    QGraphicsWidget *b = createItem(QSizeF(10.0, 100.0),
                                    QSizeF(20.0, 100.0),
                                    QSizeF(40.0, 100.0), "B");

    QGraphicsWidget *c = createItem(QSizeF(50.0, 100.0),
                                    QSizeF(70.0, 100.0),
                                    QSizeF(100.0, 100.0), "C");

    QGraphicsAnchorLayout *l = new QGraphicsAnchorLayout;
    l->setContentsMargins(0, 0, 0, 0);
    l->setSpacing(0);

    l->addAnchor(l, Qt::AnchorTop, a, Qt::AnchorTop);
    l->addAnchor(a, Qt::AnchorBottom, b, Qt::AnchorTop);
    l->addAnchor(b, Qt::AnchorBottom, c, Qt::AnchorTop);
    l->addAnchor(c, Qt::AnchorBottom, l, Qt::AnchorBottom);

    l->addAnchor(l, Qt::AnchorLeft, a, Qt::AnchorLeft);
    l->addAnchor(a, Qt::AnchorRight, b, Qt::AnchorRight);
    l->addAnchor(b, Qt::AnchorLeft, c, Qt::AnchorLeft);
    l->addAnchor(c, Qt::AnchorRight, l, Qt::AnchorRight);

    QCOMPARE(l->count(), 3);

    QGraphicsWidget p;
    p.setLayout(l);

    QSizeF layoutMinimumSize = l->effectiveSizeHint(Qt::MinimumSize);
    QSizeF layoutMaximumSize = l->effectiveSizeHint(Qt::MaximumSize);
    QSizeF layoutPreferredSize = l->effectiveSizeHint(Qt::PreferredSize);

    QCOMPARE(layoutMinimumSize, QSizeF(60.0, 300.0));
    QCOMPARE(layoutPreferredSize, QSizeF(120.0, 300.0));
    QCOMPARE(layoutMaximumSize, QSizeF(190.0, 300.0));

    p.resize(layoutMinimumSize);
    QCOMPARE(a->geometry(), QRectF(0.0, 0.0, 50.0, 100.0));
    QCOMPARE(b->geometry(), QRectF(10.0, 100.0, 40.0, 100.0));
    QCOMPARE(c->geometry(), QRectF(10.0, 200.0, 50.0, 100.0));
    QCOMPARE(p.size(), layoutMinimumSize);

    p.resize(layoutPreferredSize);
    QCOMPARE(a->geometry(), QRectF(0.0, 0.0, 70.0, 100.0));
    QCOMPARE(b->geometry(), QRectF(50.0, 100.0, 20.0, 100.0));
    QCOMPARE(c->geometry(), QRectF(50.0, 200.0, 70.0, 100.0));
    QCOMPARE(p.size(), layoutPreferredSize);

    p.resize(layoutMaximumSize);
    QCOMPARE(a->geometry(), QRectF(0.0, 0.0, 100.0, 100.0));
    QCOMPARE(b->geometry(), QRectF(90.0, 100.0, 10.0, 100.0));
    QCOMPARE(c->geometry(), QRectF(90.0, 200.0, 100.0, 100.0));
    QCOMPARE(p.size(), layoutMaximumSize);
}

void tst_QGraphicsAnchorLayout::snakeOppositeDirections()
{
    QGraphicsWidget *a = createItem(QSizeF(50.0, 100.0),
                                    QSizeF(70.0, 100.0),
                                    QSizeF(100.0, 100.0), "A");

    QGraphicsWidget *b = createItem(QSizeF(10.0, 100.0),
                                    QSizeF(20.0, 100.0),
                                    QSizeF(40.0, 100.0), "B");

    QGraphicsWidget *c = createItem(QSizeF(50.0, 100.0),
                                    QSizeF(70.0, 100.0),
                                    QSizeF(100.0, 100.0), "C");

    QGraphicsAnchorLayout *l = new QGraphicsAnchorLayout;
    l->setContentsMargins(0, 0, 0, 0);
    l->setSpacing(0);

    l->addAnchor(l, Qt::AnchorTop, a, Qt::AnchorTop);
    l->addAnchor(a, Qt::AnchorBottom, b, Qt::AnchorTop);
    l->addAnchor(b, Qt::AnchorBottom, c, Qt::AnchorTop);
    l->addAnchor(c, Qt::AnchorBottom, l, Qt::AnchorBottom);

    l->addAnchor(l, Qt::AnchorLeft, a, Qt::AnchorLeft);

    // Both a and c are 'pointing' to b
    l->addAnchor(a, Qt::AnchorRight, b, Qt::AnchorRight);
    l->addAnchor(c, Qt::AnchorLeft, b, Qt::AnchorLeft);

    l->addAnchor(c, Qt::AnchorRight, l, Qt::AnchorRight);

    QCOMPARE(l->count(), 3);

    QGraphicsWidget p;
    p.setLayout(l);

    QSizeF layoutMinimumSize = l->effectiveSizeHint(Qt::MinimumSize);
    QSizeF layoutMaximumSize = l->effectiveSizeHint(Qt::MaximumSize);
    QSizeF layoutPreferredSize = l->effectiveSizeHint(Qt::PreferredSize);

    QCOMPARE(layoutMinimumSize, QSizeF(60.0, 300.0));
    QCOMPARE(layoutPreferredSize, QSizeF(120.0, 300.0));
    QCOMPARE(layoutMaximumSize, QSizeF(190.0, 300.0));

    p.resize(layoutMinimumSize);
    QCOMPARE(a->geometry(), QRectF(0.0, 0.0, 50.0, 100.0));
    QCOMPARE(b->geometry(), QRectF(10.0, 100.0, 40.0, 100.0));
    QCOMPARE(c->geometry(), QRectF(10.0, 200.0, 50.0, 100.0));
    QCOMPARE(p.size(), layoutMinimumSize);

    p.resize(layoutPreferredSize);
    QCOMPARE(a->geometry(), QRectF(0.0, 0.0, 70.0, 100.0));
    QCOMPARE(b->geometry(), QRectF(50.0, 100.0, 20.0, 100.0));
    QCOMPARE(c->geometry(), QRectF(50.0, 200.0, 70.0, 100.0));
    QCOMPARE(p.size(), layoutPreferredSize);

    p.resize(layoutMaximumSize);
    QCOMPARE(a->geometry(), QRectF(0.0, 0.0, 100.0, 100.0));
    QCOMPARE(b->geometry(), QRectF(90.0, 100.0, 10.0, 100.0));
    QCOMPARE(c->geometry(), QRectF(90.0, 200.0, 100.0, 100.0));
    QCOMPARE(p.size(), layoutMaximumSize);

    QCOMPARE(checkReverseDirection(&p), true);
}

void tst_QGraphicsAnchorLayout::fairDistribution()
{
    QGraphicsWidget *a = createItem(QSizeF(10.0, 100.0),
                                    QSizeF(50.0, 100.0),
                                    QSizeF(100.0, 100.0), "A");

    QGraphicsWidget *b = createItem(QSizeF(10.0, 100.0),
                                    QSizeF(50.0, 100.0),
                                    QSizeF(100.0, 100.0), "B");

    QGraphicsWidget *c = createItem(QSizeF(10.0, 100.0),
                                    QSizeF(50.0, 100.0),
                                    QSizeF(100.0, 100.0), "C");

    QGraphicsWidget *d = createItem(QSizeF(60.0, 100.0),
                                    QSizeF(165.0, 100.0),
                                    QSizeF(600.0, 100.0), "D");


    QGraphicsAnchorLayout *l = new QGraphicsAnchorLayout;
    l->setContentsMargins(0, 0, 0, 0);
    l->setSpacing(0);

    l->addAnchor(l, Qt::AnchorTop, a, Qt::AnchorTop);
    l->addAnchor(a, Qt::AnchorBottom, b, Qt::AnchorTop);
    l->addAnchor(b, Qt::AnchorBottom, c, Qt::AnchorTop);
    l->addAnchor(c, Qt::AnchorBottom, d, Qt::AnchorTop);
    l->addAnchor(d, Qt::AnchorBottom, l, Qt::AnchorBottom);

    l->addAnchor(l, Qt::AnchorLeft, a, Qt::AnchorLeft);
    l->addAnchor(a, Qt::AnchorRight, b, Qt::AnchorLeft);
    l->addAnchor(b, Qt::AnchorRight, c, Qt::AnchorLeft);
    l->addAnchor(c, Qt::AnchorRight, l, Qt::AnchorRight);
    l->addAnchor(l, Qt::AnchorLeft, d, Qt::AnchorLeft);
    l->addAnchor(d, Qt::AnchorRight, l, Qt::AnchorRight);

    QCOMPARE(l->count(), 4);

    QGraphicsWidget p;
    p.setLayout(l);

    QSizeF layoutMinimumSize = l->effectiveSizeHint(Qt::MinimumSize);
    QSizeF layoutMaximumSize = l->effectiveSizeHint(Qt::MaximumSize);
    QSizeF layoutPreferredSize = l->effectiveSizeHint(Qt::PreferredSize);

    QCOMPARE(layoutMinimumSize, QSizeF(60.0, 400.0));
    QCOMPARE(layoutPreferredSize, QSizeF(165.0, 400.0));
    QCOMPARE(layoutMaximumSize, QSizeF(300.0, 400.0));

    p.resize(layoutMinimumSize);
    QCOMPARE(a->geometry(), QRectF(0.0, 0.0, 20.0, 100.0));
    QCOMPARE(b->geometry(), QRectF(20.0, 100.0, 20.0, 100.0));
    QCOMPARE(c->geometry(), QRectF(40.0, 200.0, 20.0, 100.0));
    QCOMPARE(d->geometry(), QRectF(0.0, 300.0, 60.0, 100.0));
    QCOMPARE(p.size(), layoutMinimumSize);

    p.resize(layoutPreferredSize);
    QCOMPARE(a->geometry(), QRectF(0.0, 0.0, 55.0, 100.0));
    QCOMPARE(b->geometry(), QRectF(55.0, 100.0, 55.0, 100.0));
    QCOMPARE(c->geometry(), QRectF(110.0, 200.0, 55.0, 100.0));
    QCOMPARE(d->geometry(), QRectF(0.0, 300.0, 165.0, 100.0));
    QCOMPARE(p.size(), layoutPreferredSize);

    p.resize(layoutMaximumSize);
    QCOMPARE(a->geometry(), QRectF(0.0, 0.0, 100.0, 100.0));
    QCOMPARE(b->geometry(), QRectF(100.0, 100.0, 100.0, 100.0));
    QCOMPARE(c->geometry(), QRectF(200.0, 200.0, 100.0, 100.0));
    QCOMPARE(d->geometry(), QRectF(0.0, 300.0, 300.0, 100.0));
    QCOMPARE(p.size(), layoutMaximumSize);
}

void tst_QGraphicsAnchorLayout::fairDistributionOppositeDirections()
{
    QGraphicsWidget *a = createItem(QSizeF(10.0, 100.0),
                                    QSizeF(50.0, 100.0),
                                    QSizeF(100.0, 100.0), "A");

    QGraphicsWidget *b = createItem(QSizeF(10.0, 100.0),
                                    QSizeF(50.0, 100.0),
                                    QSizeF(100.0, 100.0), "B");

    QGraphicsWidget *c = createItem(QSizeF(10.0, 100.0),
                                    QSizeF(50.0, 100.0),
                                    QSizeF(100.0, 100.0), "C");

    QGraphicsWidget *d = createItem(QSizeF(10.0, 100.0),
                                    QSizeF(50.0, 100.0),
                                    QSizeF(100.0, 100.0), "D");

    QGraphicsWidget *e = createItem(QSizeF(60.0, 100.0),
                                    QSizeF(220.0, 100.0),
                                    QSizeF(600.0, 100.0), "E");


    QGraphicsAnchorLayout *l = new QGraphicsAnchorLayout;
    l->setContentsMargins(0, 0, 0, 0);
    l->setSpacing(0);

    l->addAnchor(l, Qt::AnchorTop, a, Qt::AnchorTop);
    l->addAnchor(a, Qt::AnchorBottom, b, Qt::AnchorTop);
    l->addAnchor(b, Qt::AnchorBottom, c, Qt::AnchorTop);
    l->addAnchor(c, Qt::AnchorBottom, d, Qt::AnchorTop);
    l->addAnchor(d, Qt::AnchorBottom, e, Qt::AnchorTop);
    l->addAnchor(e, Qt::AnchorBottom, l, Qt::AnchorBottom);

    l->addAnchor(a, Qt::AnchorLeft, l, Qt::AnchorLeft);
    l->addAnchor(b, Qt::AnchorLeft, a, Qt::AnchorRight);
    l->addAnchor(c, Qt::AnchorLeft, b, Qt::AnchorRight);
    l->addAnchor(d, Qt::AnchorLeft, c, Qt::AnchorRight);
    l->addAnchor(d, Qt::AnchorRight, l, Qt::AnchorRight);
    l->addAnchors(l, e, Qt::Horizontal);

    QCOMPARE(l->count(), 5);

    QGraphicsWidget p;
    p.setLayout(l);

    QSizeF layoutMinimumSize = l->effectiveSizeHint(Qt::MinimumSize);
    QSizeF layoutMaximumSize = l->effectiveSizeHint(Qt::MaximumSize);
    QSizeF layoutPreferredSize = l->effectiveSizeHint(Qt::PreferredSize);

    QCOMPARE(layoutMinimumSize, QSizeF(60.0, 500.0));
    QCOMPARE(layoutPreferredSize, QSizeF(220.0, 500.0));
    QCOMPARE(layoutMaximumSize, QSizeF(400.0, 500.0));

    p.resize(layoutMinimumSize);
    QCOMPARE(a->size(), b->size());
    QCOMPARE(a->size(), c->size());
    QCOMPARE(a->size(), d->size());
    QCOMPARE(e->size().width(), 4 * a->size().width());
    QCOMPARE(p.size(), layoutMinimumSize);

    p.resize(layoutPreferredSize);
    QCOMPARE(a->size(), b->size());
    QCOMPARE(a->size(), c->size());
    QCOMPARE(a->size(), d->size());
    QCOMPARE(e->size().width(), 4 * a->size().width());
    QCOMPARE(p.size(), layoutPreferredSize);

    p.resize(layoutMaximumSize);
    QCOMPARE(a->size(), b->size());
    QCOMPARE(a->size(), c->size());
    QCOMPARE(a->size(), d->size());
    QCOMPARE(e->size().width(), 4 * a->size().width());
    QCOMPARE(p.size(), layoutMaximumSize);
}

void tst_QGraphicsAnchorLayout::proportionalPreferred()
{
    QSizeF min(0, 100);

    QGraphicsWidget *a = createItem(min, QSizeF(10, 100), QSizeF(20, 100), "A");
    QGraphicsWidget *b = createItem(min, QSizeF(20, 100), QSizeF(30, 100), "B");
    QGraphicsWidget *c = createItem(min, QSizeF(14, 100), QSizeF(20, 100), "C");
    QGraphicsWidget *d = createItem(min, QSizeF(10, 100), QSizeF(20, 100), "D");

    QGraphicsAnchorLayout *l = new QGraphicsAnchorLayout;
    l->setContentsMargins(0, 0, 0, 0);
    l->setSpacing(0);

    l->addAnchor(l, Qt::AnchorTop, a, Qt::AnchorTop);
    l->addAnchor(a, Qt::AnchorBottom, b, Qt::AnchorTop);
    l->addAnchor(b, Qt::AnchorBottom, c, Qt::AnchorTop);
    l->addAnchor(c, Qt::AnchorBottom, d, Qt::AnchorTop);
    l->addAnchor(d, Qt::AnchorBottom, l, Qt::AnchorBottom);

    l->addAnchor(l, Qt::AnchorLeft, a, Qt::AnchorLeft);
    l->addAnchor(l, Qt::AnchorLeft, b, Qt::AnchorLeft);
    l->addAnchor(a, Qt::AnchorRight, c, Qt::AnchorLeft);
    l->addAnchor(a, Qt::AnchorRight, d, Qt::AnchorLeft);
    l->addAnchor(b, Qt::AnchorRight, l, Qt::AnchorRight);
    l->addAnchor(c, Qt::AnchorRight, l, Qt::AnchorRight);
    l->addAnchor(d, Qt::AnchorRight, l, Qt::AnchorRight);

    QCOMPARE(l->count(), 4);

    QGraphicsWidget p;
    p.setLayout(l);

    QSizeF layoutMinimumSize = l->effectiveSizeHint(Qt::MinimumSize);
    QSizeF layoutPreferredSize = l->effectiveSizeHint(Qt::PreferredSize);
    QSizeF layoutMaximumSize = l->effectiveSizeHint(Qt::MaximumSize);

    QCOMPARE(layoutMinimumSize, QSizeF(0, 400));
    QCOMPARE(layoutPreferredSize, QSizeF(24, 400));
    QCOMPARE(layoutMaximumSize, QSizeF(30, 400));

    p.resize(layoutMinimumSize);
    QCOMPARE(p.size(), layoutMinimumSize);

    p.resize(layoutPreferredSize);
    QCOMPARE(c->size().width(), d->size().width());
    QCOMPARE(p.size(), layoutPreferredSize);

    p.resize(layoutMaximumSize);
    QCOMPARE(p.size(), layoutMaximumSize);

    p.resize(QSizeF(12, 400));

    // Proportionality between size given and preferred size, this
    // should be respected in this graph for (a) and (b)|(c).
    qreal factor = 12.0 / 24.0;

    QCOMPARE(c->size().width(), d->size().width());
    QCOMPARE(a->size().width(), 10 * factor);
    QCOMPARE(c->size().width(), 14 * factor);
    QCOMPARE(p.size(), QSizeF(12, 400));
}

void tst_QGraphicsAnchorLayout::example()
{
    QSizeF min(30, 100);
    QSizeF pref(210, 100);
    QSizeF max(300, 100);

    QGraphicsWidget *a = createItem(min, pref, max, "A");
    QGraphicsWidget *b = createItem(min, pref, max, "B");
    QGraphicsWidget *c = createItem(min, pref, max, "C");
    QGraphicsWidget *d = createItem(min, pref, max, "D");
    QGraphicsWidget *e = createItem(min, pref, max, "E");
    QGraphicsWidget *f = createItem(QSizeF(30, 50), QSizeF(150, 50), max, "F");
    QGraphicsWidget *g = createItem(QSizeF(30, 50), QSizeF(30, 100), max, "G");

    QGraphicsAnchorLayout *l = new QGraphicsAnchorLayout;
    l->setContentsMargins(0, 0, 0, 0);
    l->setSpacing(0);

    // vertical
    l->addAnchor(a, Qt::AnchorTop, l, Qt::AnchorTop);
    l->addAnchor(b, Qt::AnchorTop, l, Qt::AnchorTop);

    l->addAnchor(c, Qt::AnchorTop, a, Qt::AnchorBottom);
    l->addAnchor(c, Qt::AnchorTop, b, Qt::AnchorBottom);
    l->addAnchor(c, Qt::AnchorBottom, d, Qt::AnchorTop);
    l->addAnchor(c, Qt::AnchorBottom, e, Qt::AnchorTop);

    l->addAnchor(d, Qt::AnchorBottom, l, Qt::AnchorBottom);
    l->addAnchor(e, Qt::AnchorBottom, l, Qt::AnchorBottom);

    l->addAnchor(c, Qt::AnchorTop, f, Qt::AnchorTop);
    l->addAnchor(c, Qt::AnchorVerticalCenter, f, Qt::AnchorBottom);
    l->addAnchor(f, Qt::AnchorBottom, g, Qt::AnchorTop);
    l->addAnchor(c, Qt::AnchorBottom, g, Qt::AnchorBottom);

    // horizontal
    l->addAnchor(l, Qt::AnchorLeft, a, Qt::AnchorLeft);
    l->addAnchor(l, Qt::AnchorLeft, d, Qt::AnchorLeft);
    l->addAnchor(a, Qt::AnchorRight, b, Qt::AnchorLeft);

    l->addAnchor(a, Qt::AnchorRight, c, Qt::AnchorLeft);
    l->addAnchor(c, Qt::AnchorRight, e, Qt::AnchorLeft);

    l->addAnchor(b, Qt::AnchorRight, l, Qt::AnchorRight);
    l->addAnchor(e, Qt::AnchorRight, l, Qt::AnchorRight);
    l->addAnchor(d, Qt::AnchorRight, e, Qt::AnchorLeft);

    l->addAnchor(l, Qt::AnchorLeft, f, Qt::AnchorLeft);
    l->addAnchor(l, Qt::AnchorLeft, g, Qt::AnchorLeft);
    l->addAnchor(f, Qt::AnchorRight, g, Qt::AnchorRight);

    QCOMPARE(l->count(), 7);

    QGraphicsWidget p;
    p.setLayout(l);

    QSizeF layoutMinimumSize = l->effectiveSizeHint(Qt::MinimumSize);
    QSizeF layoutMaximumSize = l->effectiveSizeHint(Qt::MaximumSize);
    QSizeF layoutPreferredSize = l->effectiveSizeHint(Qt::PreferredSize);

    QCOMPARE(layoutMinimumSize, QSizeF(90.0, 300.0));
    QCOMPARE(layoutPreferredSize, QSizeF(510.0, 300.0));
    QCOMPARE(layoutMaximumSize, QSizeF(570.0, 300.0));

    p.resize(layoutMinimumSize);
    QCOMPARE(p.size(), layoutMinimumSize);
    QCOMPARE(a->size(), e->size());
    QCOMPARE(b->size(), d->size());
    QCOMPARE(f->size(), g->size());

    p.resize(layoutPreferredSize);
    QCOMPARE(p.size(), layoutPreferredSize);
    QCOMPARE(a->size(), e->size());
    QCOMPARE(b->size(), d->size());
    QCOMPARE(f->size(), g->size());

    p.resize(layoutMaximumSize);
    QCOMPARE(p.size(), layoutMaximumSize);
    QCOMPARE(a->size(), e->size());
    QCOMPARE(b->size(), d->size());
    QCOMPARE(f->size(), g->size());
}

void tst_QGraphicsAnchorLayout::setSpacing()
{
    QSizeF min(10, 10);
    QSizeF pref(20, 20);
    QSizeF max(50, 50);

    QGraphicsWidget *a = createItem(min, pref, max);
    QGraphicsWidget *b = createItem(min, pref, max);
    QGraphicsWidget *c = createItem(min, pref, max);

    QGraphicsAnchorLayout *l = new QGraphicsAnchorLayout;
    l->addCornerAnchors(l, Qt::TopLeftCorner, a, Qt::TopLeftCorner);
    l->addCornerAnchors(b, Qt::TopRightCorner, l, Qt::TopRightCorner);
    l->addAnchor(a, Qt::AnchorRight, b, Qt::AnchorLeft);

    l->addAnchors(l, c, Qt::Horizontal);

    l->addAnchor(a, Qt::AnchorBottom, c, Qt::AnchorTop);
    l->addAnchor(c, Qt::AnchorBottom, l, Qt::AnchorBottom);

    QGraphicsWidget *p = new QGraphicsWidget(0, Qt::Window);

    p->setLayout(l);
    l->setSpacing(1);

    // don't let the style influence the test.
    l->setContentsMargins(0, 0, 0, 0);

    QGraphicsScene scene;
    scene.addItem(p);
    QGraphicsView *view = new QGraphicsView(&scene);
    view->show();
    p->show();

    QApplication::processEvents();
#ifdef Q_WS_MAC
    QTest::qWait(200);
#endif
    QCOMPARE(a->geometry(), QRectF(0, 0, 20, 20));
    QCOMPARE(b->geometry(), QRectF(21, 0, 20, 20));
    QCOMPARE(c->geometry(), QRectF(0, 21, 41, 20));

    l->setHorizontalSpacing(4);
    QApplication::processEvents();
    p->adjustSize();
    QCOMPARE(a->geometry(), QRectF(0, 0, 20, 20));
    QCOMPARE(b->geometry(), QRectF(24, 0, 20, 20));
    QCOMPARE(c->geometry(), QRectF(0, 21, 44, 20));

    l->setVerticalSpacing(0);
    QApplication::processEvents();
    p->adjustSize();
    QCOMPARE(a->geometry(), QRectF(0, 0, 20, 20));
    QCOMPARE(b->geometry(), QRectF(24, 0, 20, 20));
    QCOMPARE(c->geometry(), QRectF(0, 20, 44, 20));

    delete p;
    delete view;
}

/*!
    Taken from "hard" complex case, found at
    https://cwiki.nokia.com/S60QTUI/AnchorLayoutComplexCases

    This layout has a special property, since it has two possible solutions for its minimum size.

    For instance, when it is in its minimum size - the layout have two possible solutions:
    1. c.width == 10, e.width == 10 and g.width == 10
       (all others have width 0)
    2. d.width == 10 and g.width == 10
       (all others have width 0)

    It also has several solutions for preferred size.
*/
static QGraphicsAnchorLayout *createAmbiguousS60Layout()
{
    QGraphicsAnchorLayout *l = new QGraphicsAnchorLayout;
    l->setContentsMargins(0, 0, 0, 0);
    l->setSpacing(0);

    QSizeF min(0, 10);
    QSizeF pref(50, 10);
    QSizeF max(100, 10);

    QGraphicsWidget *a = createItem(min, pref, max, "a");
    QGraphicsWidget *b = createItem(min, pref, max, "b");
    QGraphicsWidget *c = createItem(min, pref, max, "c");
    QGraphicsWidget *d = createItem(min, pref, max, "d");
    QGraphicsWidget *e = createItem(min, pref, max, "e");
    QGraphicsWidget *f = createItem(min, pref, max, "f");
    QGraphicsWidget *g = createItem(min, pref, max, "g");

    //<!-- Trunk -->
    setAnchor(l, l, Qt::AnchorLeft, a, Qt::AnchorLeft, 10);
    setAnchor(l, a, Qt::AnchorRight, b, Qt::AnchorLeft, 10);
    setAnchor(l, b, Qt::AnchorRight, c, Qt::AnchorLeft, 10);
    setAnchor(l, c, Qt::AnchorRight, d, Qt::AnchorLeft, 10);
    setAnchor(l, d, Qt::AnchorRight, l, Qt::AnchorRight, 10);

    //<!-- Above trunk -->
    setAnchor(l, b, Qt::AnchorLeft, e, Qt::AnchorLeft, 10);
    setAnchor(l, e, Qt::AnchorRight, d, Qt::AnchorLeft, 10);

    //<!-- Below trunk -->
    setAnchor(l, a, Qt::AnchorHorizontalCenter, g, Qt::AnchorLeft, 10);
    setAnchor(l, g, Qt::AnchorRight, f, Qt::AnchorHorizontalCenter, 10);
    setAnchor(l, c, Qt::AnchorLeft, f, Qt::AnchorLeft, 10);
    setAnchor(l, f, Qt::AnchorRight, d, Qt::AnchorRight, 10);

    //<!-- vertical is simpler -->
    setAnchor(l, l, Qt::AnchorTop, e, Qt::AnchorTop, 0);
    setAnchor(l, e, Qt::AnchorBottom, a, Qt::AnchorTop, 0);
    setAnchor(l, e, Qt::AnchorBottom, b, Qt::AnchorTop, 0);
    setAnchor(l, e, Qt::AnchorBottom, c, Qt::AnchorTop, 0);
    setAnchor(l, e, Qt::AnchorBottom, d, Qt::AnchorTop, 0);
    setAnchor(l, a, Qt::AnchorBottom, f, Qt::AnchorTop, 0);
    setAnchor(l, a, Qt::AnchorBottom, b, Qt::AnchorBottom, 0);
    setAnchor(l, a, Qt::AnchorBottom, c, Qt::AnchorBottom, 0);
    setAnchor(l, a, Qt::AnchorBottom, d, Qt::AnchorBottom, 0);
    setAnchor(l, f, Qt::AnchorBottom, g, Qt::AnchorTop, 0);
    setAnchor(l, g, Qt::AnchorBottom, l, Qt::AnchorBottom, 0);
    return l;
}

void tst_QGraphicsAnchorLayout::hardComplexS60()
{
    QGraphicsAnchorLayout *l = createAmbiguousS60Layout();
    QCOMPARE(l->count(), 7);

    QGraphicsWidget *p = new QGraphicsWidget(0, Qt::Window);
    p->setLayout(l);

    QSizeF layoutMinimumSize = l->effectiveSizeHint(Qt::MinimumSize);
    QCOMPARE(layoutMinimumSize, QSizeF(60, 40));
    // expected preferred might be wrong, (haven't manually verified it)
    QSizeF layoutPreferredSize = l->effectiveSizeHint(Qt::PreferredSize);
    QCOMPARE(layoutPreferredSize, QSizeF(220, 40));
    QSizeF layoutMaximumSize = l->effectiveSizeHint(Qt::MaximumSize);
    QCOMPARE(layoutMaximumSize, QSizeF(240, 40));

    delete p;
}

void tst_QGraphicsAnchorLayout::stability()
{
    QVector<QRectF> geometries;
    geometries.resize(7);
    QGraphicsWidget *p = new QGraphicsWidget(0, Qt::Window);
    bool sameAsPreviousArrangement = true;
    // it usually fails after 3-4 iterations
    for (int pass = 0; pass < 20 && sameAsPreviousArrangement; ++pass) {
        // In case we need to "scramble" the heap allocator to provoke this bug.
        //static const int primes[] = {2, 3, 5, 13, 89, 233, 1597, 28657, 514229}; // fibo primes
        //const int primeCount = sizeof(primes)/sizeof(int);
        //int alloc = primes[pass % primeCount] + pass;
        //void *mem = qMalloc(alloc);
        //qFree(mem);
        QGraphicsAnchorLayout *l = createAmbiguousS60Layout();
        p->setLayout(l);
        QSizeF layoutMinimumSize = l->effectiveSizeHint(Qt::MinimumSize);
        l->setGeometry(QRectF(QPointF(0,0), layoutMinimumSize));
        QApplication::processEvents();
        for (int i = l->count() - 1; i >=0 && sameAsPreviousArrangement; --i) {
            QRectF geom = l->itemAt(i)->geometry();
            if (pass != 0) {
                sameAsPreviousArrangement = (geometries[i] == geom);
            }
            geometries[i] = geom;
        }
        p->setLayout(0);    // uninstalls and deletes the layout
        QApplication::processEvents();
    }
    delete p;
    QEXPECT_FAIL("", "The layout have several solutions, but which solution it picks is not stable", Continue);
    QCOMPARE(sameAsPreviousArrangement, true);
}

void tst_QGraphicsAnchorLayout::delete_anchor()
{
    QGraphicsScene scene;
    QSizeF minSize(0, 0);
    QSizeF prefSize(50, 50);
    QSizeF maxSize(100, 100);
    QGraphicsWidget *w1 = createItem(minSize, prefSize, maxSize, "w1");
    QGraphicsWidget *w2 = createItem(minSize, prefSize, maxSize, "w2");
    QGraphicsWidget *w3 = createItem(minSize, prefSize, maxSize, "w3");

    QGraphicsAnchorLayout *l = new QGraphicsAnchorLayout;
    l->setSpacing(0);
    l->setContentsMargins(0, 0, 0, 0);
    l->addAnchor(l, Qt::AnchorLeft, w1, Qt::AnchorLeft);
    l->addAnchor(w1, Qt::AnchorRight, w2, Qt::AnchorLeft);
    l->addAnchor(w2, Qt::AnchorRight, l, Qt::AnchorRight);
    l->addAnchor(w1, Qt::AnchorRight, w3, Qt::AnchorLeft);
    l->addAnchor(w3, Qt::AnchorRight, l, Qt::AnchorRight);

    QGraphicsAnchor *anchor = l->anchor(w3, Qt::AnchorRight, l, Qt::AnchorRight);
    anchor->setSpacing(10);

    QGraphicsWidget *p = new QGraphicsWidget;
    p->setLayout(l);

    QCOMPARE(l->count(), 3);

    scene.addItem(p);
    QGraphicsView *view = new QGraphicsView(&scene);
    QApplication::processEvents();
    // Should now be simplified
    QCOMPARE(l->effectiveSizeHint(Qt::PreferredSize).width(), qreal(110));
    QGraphicsAnchor *anchor1 = l->anchor(w3, Qt::AnchorRight, l, Qt::AnchorRight);
    QVERIFY(anchor1);
    QGraphicsAnchor *anchor2 = l->anchor(w3, Qt::AnchorRight, l, Qt::AnchorRight);
    QVERIFY(anchor2);
    QGraphicsAnchor *anchor3 = l->anchor(l, Qt::AnchorRight, w3, Qt::AnchorRight);
    QVERIFY(anchor3);
    QGraphicsAnchor *anchor4 = l->anchor(l, Qt::AnchorRight, w3, Qt::AnchorRight);
    QVERIFY(anchor4);

    // should all be the same object
    QCOMPARE(anchor1, anchor2);
    QCOMPARE(anchor2, anchor3);
    QCOMPARE(anchor3, anchor4);

    // check if removal works
    delete anchor1;

    QApplication::processEvents();

    // it should also change the preferred size of the layout
    QCOMPARE(l->effectiveSizeHint(Qt::PreferredSize).width(), qreal(100));

    delete p;
    delete view;
}

void tst_QGraphicsAnchorLayout::sizePolicy()
{
    QGraphicsScene scene;
    QSizeF minSize(0, 0);
    QSizeF prefSize(50, 50);
    QSizeF maxSize(100, 100);
    QGraphicsWidget *w1 = createItem(minSize, prefSize, maxSize, "w1");

    QGraphicsAnchorLayout *l = new QGraphicsAnchorLayout;
    l->setSpacing(0);
    l->setContentsMargins(0, 0, 0, 0);

    // horizontal
    QGraphicsAnchor *anchor = l->addAnchor(l, Qt::AnchorLeft, w1, Qt::AnchorLeft);
    anchor->setSpacing(0);

    anchor = l->addAnchor(w1, Qt::AnchorRight, l, Qt::AnchorRight);
    anchor->setSpacing(0);

    // vertical
    anchor = l->addAnchor(l, Qt::AnchorTop, w1, Qt::AnchorTop);
    anchor->setSpacing(0);

    anchor = l->addAnchor(w1, Qt::AnchorBottom, l, Qt::AnchorBottom);
    anchor->setSpacing(0);

    QGraphicsWidget *p = new QGraphicsWidget;
    p->setLayout(l);

    scene.addItem(p);
    QGraphicsView *view = new QGraphicsView(&scene);

    // QSizePolicy::Minimum
    w1->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    QApplication::processEvents();
    w1->adjustSize();

    QCOMPARE(l->effectiveSizeHint(Qt::MinimumSize), QSizeF(50, 50));
    QCOMPARE(l->effectiveSizeHint(Qt::PreferredSize), QSizeF(50, 50));
    QCOMPARE(l->effectiveSizeHint(Qt::MaximumSize), QSizeF(100, 100));

    // QSizePolicy::Maximum
    w1->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    QApplication::processEvents();
    w1->adjustSize();

    QCOMPARE(l->effectiveSizeHint(Qt::MinimumSize), QSizeF(0, 0));
    QCOMPARE(l->effectiveSizeHint(Qt::PreferredSize), QSizeF(50, 50));
    QCOMPARE(l->effectiveSizeHint(Qt::MaximumSize), QSizeF(50, 50));

    // QSizePolicy::Fixed
    w1->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    QApplication::processEvents();
    w1->adjustSize();

    QCOMPARE(l->effectiveSizeHint(Qt::MinimumSize), QSizeF(50, 50));
    QCOMPARE(l->effectiveSizeHint(Qt::PreferredSize), QSizeF(50, 50));
    QCOMPARE(l->effectiveSizeHint(Qt::MaximumSize), QSizeF(50, 50));

    // QSizePolicy::Preferred
    w1->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    QApplication::processEvents();
    w1->adjustSize();

    QCOMPARE(l->effectiveSizeHint(Qt::MinimumSize), QSizeF(0, 0));
    QCOMPARE(l->effectiveSizeHint(Qt::PreferredSize), QSizeF(50, 50));
    QCOMPARE(l->effectiveSizeHint(Qt::MaximumSize), QSizeF(100, 100));

    // QSizePolicy::Ignored
    w1->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    QApplication::processEvents();
    w1->adjustSize();

    QCOMPARE(l->effectiveSizeHint(Qt::MinimumSize), QSizeF(0, 0));
    QCOMPARE(l->effectiveSizeHint(Qt::PreferredSize), QSizeF(0, 0));
    QCOMPARE(l->effectiveSizeHint(Qt::MaximumSize), QSizeF(100, 100));

    delete p;
    delete view;
}

/*!
    \internal

    Uses private API. (We have decided to pull hasConflicts() out of the API). However, it also
    tests some tight conditions (almost-in-conflict) that we really want to test.
*/
void tst_QGraphicsAnchorLayout::conflicts()
{
    QGraphicsWidget *a = createItem(QSizeF(80,10), QSizeF(90,10), QSizeF(100,10), "a");
    QGraphicsWidget *b = createItem(QSizeF(10,10), QSizeF(20,10), QSizeF(30,10), "b");
    QGraphicsWidget *c = createItem(QSizeF(10,10), QSizeF(20,10), QSizeF(30,10), "c");

    QGraphicsAnchorLayout *l;
    QGraphicsWidget *p = new QGraphicsWidget(0, Qt::Window);

    l = new QGraphicsAnchorLayout;
    l->setContentsMargins(0, 0, 0, 0);

    // with the following setup, 'a' cannot be larger than 30 we will first have a Simplex conflict

    // horizontal
    setAnchor(l, l, Qt::AnchorLeft, b, Qt::AnchorLeft);
    setAnchor(l, b, Qt::AnchorRight, c, Qt::AnchorLeft);
    setAnchor(l, c, Qt::AnchorRight, l, Qt::AnchorRight);
    setAnchor(l, b, Qt::AnchorHorizontalCenter, a, Qt::AnchorLeft);
    setAnchor(l, a, Qt::AnchorRight, c, Qt::AnchorHorizontalCenter);

    // vertical
    setAnchor(l, l, Qt::AnchorTop, a, Qt::AnchorTop);
    setAnchor(l, a, Qt::AnchorBottom, b, Qt::AnchorTop);
    setAnchor(l, a, Qt::AnchorBottom, c, Qt::AnchorTop);
    setAnchor(l, b, Qt::AnchorBottom, l, Qt::AnchorBottom);
    setAnchor(l, c, Qt::AnchorBottom, l, Qt::AnchorBottom);

    p->setLayout(l);

    QCOMPARE(layoutHasConflict(l), true);

    a->setMinimumSize(QSizeF(29,10));
    QCOMPARE(layoutHasConflict(l), false);

    a->setMinimumSize(QSizeF(30,10));
    QCOMPARE(layoutHasConflict(l), false);

    delete p;
}

QTEST_MAIN(tst_QGraphicsAnchorLayout)
#include "tst_qgraphicsanchorlayout.moc"