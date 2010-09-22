/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the QtDeclarative module of the Qt Toolkit.
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

#include "private/qdeclarativelistview_p.h"

#include "private/qdeclarativeflickable_p_p.h"
#include "private/qdeclarativevisualitemmodel_p.h"

#include "private/qdeclarativesmoothedanimation_p_p.h"
#include <qdeclarativeexpression.h>
#include <qdeclarativeengine.h>
#include <qdeclarativeguard_p.h>
#include <qdeclarativeinfo.h>

#include <qlistmodelinterface_p.h>
#include <qmath.h>
#include <QKeyEvent>

QT_BEGIN_NAMESPACE

void QDeclarativeViewSection::setProperty(const QString &property)
{
    if (property != m_property) {
        m_property = property;
        emit propertyChanged();
    }
}

void QDeclarativeViewSection::setCriteria(QDeclarativeViewSection::SectionCriteria criteria)
{
    if (criteria != m_criteria) {
        m_criteria = criteria;
        emit criteriaChanged();
    }
}

void QDeclarativeViewSection::setDelegate(QDeclarativeComponent *delegate)
{
    if (delegate != m_delegate) {
        m_delegate = delegate;
        emit delegateChanged();
    }
}

QString QDeclarativeViewSection::sectionString(const QString &value)
{
    if (m_criteria == FirstCharacter)
        return value.isEmpty() ? QString() : value.at(0);
    else
        return value;
}

//----------------------------------------------------------------------------

class FxListItem
{
public:
    FxListItem(QDeclarativeItem *i, QDeclarativeListView *v) : item(i), section(0), view(v) {
        attached = static_cast<QDeclarativeListViewAttached*>(qmlAttachedPropertiesObject<QDeclarativeListView>(item));
        if (attached)
            attached->m_view = view;
    }
    ~FxListItem() {}
    qreal position() const {
        if (section)
            return (view->orientation() == QDeclarativeListView::Vertical ? section->y() : section->x());
        else
            return (view->orientation() == QDeclarativeListView::Vertical ? item->y() : item->x());
    }
    qreal size() const {
        if (section)
            return (view->orientation() == QDeclarativeListView::Vertical ? item->height()+section->height() : item->width()+section->height());
        else
            return (view->orientation() == QDeclarativeListView::Vertical ? item->height() : item->width());
    }
    qreal endPosition() const {
        return (view->orientation() == QDeclarativeListView::Vertical
                                        ? item->y() + (item->height() > 0 ? item->height() : 1)
                                        : item->x() + (item->width() > 0 ? item->width() : 1)) - 1;
    }
    void setPosition(qreal pos) {
        if (view->orientation() == QDeclarativeListView::Vertical) {
            if (section) {
                section->setY(pos);
                pos += section->height();
            }
            item->setY(pos);
        } else {
            if (section) {
                section->setX(pos);
                pos += section->width();
            }
            item->setX(pos);
        }
    }
    bool contains(int x, int y) const {
        return (x >= item->x() && x < item->x() + item->width() &&
                y >= item->y() && y < item->y() + item->height());
    }

    QDeclarativeItem *item;
    QDeclarativeItem *section;
    QDeclarativeListView *view;
    QDeclarativeListViewAttached *attached;
    int index;
};

//----------------------------------------------------------------------------

class QDeclarativeListViewPrivate : public QDeclarativeFlickablePrivate
{
    Q_DECLARE_PUBLIC(QDeclarativeListView)

public:
    QDeclarativeListViewPrivate()
        : currentItem(0), orient(QDeclarativeListView::Vertical)
        , visiblePos(0), visibleIndex(0)
        , averageSize(100.0), currentIndex(-1), requestedIndex(-1)
        , itemCount(0), highlightRangeStart(0), highlightRangeEnd(0)
        , highlightComponent(0), highlight(0), trackedItem(0)
        , moveReason(Other), buffer(0), highlightPosAnimator(0), highlightSizeAnimator(0)
        , sectionCriteria(0), spacing(0.0)
        , highlightMoveSpeed(400), highlightMoveDuration(-1)
        , highlightResizeSpeed(400), highlightResizeDuration(-1), highlightRange(QDeclarativeListView::NoHighlightRange)
        , snapMode(QDeclarativeListView::NoSnap), overshootDist(0.0)
        , footerComponent(0), footer(0), headerComponent(0), header(0)
        , bufferMode(BufferBefore | BufferAfter)
        , ownModel(false), wrap(false), autoHighlight(true), haveHighlightRange(false)
        , correctFlick(false), inFlickCorrection(false), lazyRelease(false)
        , deferredRelease(false), layoutScheduled(false), minExtentDirty(true), maxExtentDirty(true)
    {}

    void init();
    void clear();
    FxListItem *createItem(int modelIndex);
    void releaseItem(FxListItem *item);

    FxListItem *visibleItem(int modelIndex) const {
        if (modelIndex >= visibleIndex && modelIndex < visibleIndex + visibleItems.count()) {
            for (int i = modelIndex - visibleIndex; i < visibleItems.count(); ++i) {
                FxListItem *item = visibleItems.at(i);
                if (item->index == modelIndex)
                    return item;
            }
        }
        return 0;
    }

    FxListItem *firstVisibleItem() const {
        const qreal pos = position();
        for (int i = 0; i < visibleItems.count(); ++i) {
            FxListItem *item = visibleItems.at(i);
            if (item->index != -1 && item->endPosition() > pos)
                return item;
        }
        return visibleItems.count() ? visibleItems.first() : 0;
    }

    FxListItem *nextVisibleItem() const {
        const qreal pos = position();
        bool foundFirst = false;
        for (int i = 0; i < visibleItems.count(); ++i) {
            FxListItem *item = visibleItems.at(i);
            if (item->index != -1) {
                if (foundFirst)
                    return item;
                else if (item->position() < pos && item->endPosition() > pos)
                    foundFirst = true;
            }
        }
        return 0;
    }

    qreal position() const {
        Q_Q(const QDeclarativeListView);
        return orient == QDeclarativeListView::Vertical ? q->contentY() : q->contentX();
    }
    void setPosition(qreal pos) {
        Q_Q(QDeclarativeListView);
        if (orient == QDeclarativeListView::Vertical)
            q->QDeclarativeFlickable::setContentY(pos);
        else
            q->QDeclarativeFlickable::setContentX(pos);
    }
    qreal size() const {
        Q_Q(const QDeclarativeListView);
        return orient == QDeclarativeListView::Vertical ? q->height() : q->width();
    }

    qreal startPosition() const {
        qreal pos = 0;
        if (!visibleItems.isEmpty()) {
            pos = (*visibleItems.constBegin())->position();
            if (visibleIndex > 0)
                pos -= visibleIndex * (averageSize + spacing);
        }
        return pos;
    }

    qreal endPosition() const {
        qreal pos = 0;
        if (!visibleItems.isEmpty()) {
            int invisibleCount = visibleItems.count() - visibleIndex;
            for (int i = visibleItems.count()-1; i >= 0; --i) {
                if (visibleItems.at(i)->index != -1) {
                    invisibleCount = model->count() - visibleItems.at(i)->index - 1;
                    break;
                }
            }
            pos = (*(--visibleItems.constEnd()))->endPosition() + invisibleCount * (averageSize + spacing);
        }
        return pos;
    }

    qreal positionAt(int modelIndex) const {
        if (FxListItem *item = visibleItem(modelIndex))
            return item->position();
        if (!visibleItems.isEmpty()) {
            if (modelIndex < visibleIndex) {
                int count = visibleIndex - modelIndex;
                return (*visibleItems.constBegin())->position() - count * (averageSize + spacing);
            } else {
                int idx = visibleItems.count() - 1;
                while (idx >= 0 && visibleItems.at(idx)->index == -1)
                    --idx;
                if (idx < 0)
                    idx = visibleIndex;
                else
                    idx = visibleItems.at(idx)->index;
                int count = modelIndex - idx - 1;
                return (*(--visibleItems.constEnd()))->endPosition() + spacing + count * (averageSize + spacing) + 1;
            }
        }
        return 0;
    }

    qreal endPositionAt(int modelIndex) const {
        if (FxListItem *item = visibleItem(modelIndex))
            return item->endPosition();
        if (!visibleItems.isEmpty()) {
            if (modelIndex < visibleIndex) {
                int count = visibleIndex - modelIndex;
                return (*visibleItems.constBegin())->position() - (count - 1) * (averageSize + spacing) - spacing - 1;
            } else {
                int idx = visibleItems.count() - 1;
                while (idx >= 0 && visibleItems.at(idx)->index == -1)
                    --idx;
                if (idx < 0)
                    idx = visibleIndex;
                else
                    idx = visibleItems.at(idx)->index;
                int count = modelIndex - idx - 1;
                return (*(--visibleItems.constEnd()))->endPosition() + count * (averageSize + spacing);
            }
        }
        return 0;
    }

    QString sectionAt(int modelIndex) {
        if (FxListItem *item = visibleItem(modelIndex))
            return item->attached->section();

        QString section;
        if (sectionCriteria) {
            QString propValue = model->stringValue(modelIndex, sectionCriteria->property());
            section = sectionCriteria->sectionString(propValue);
        }

        return section;
    }

    bool isValid() const {
        return model && model->count() && model->isValid();
    }

    int snapIndex() {
        int index = currentIndex;
        for (int i = 0; i < visibleItems.count(); ++i) {
            FxListItem *item = visibleItems[i];
            if (item->index == -1)
                continue;
            qreal itemTop = item->position();
            if (itemTop >= highlight->position()-item->size()/2 && itemTop < highlight->position()+item->size()/2)
                return item->index;
        }
        return index;
    }

    qreal snapPosAt(qreal pos) {
        for (int i = 0; i < visibleItems.count(); ++i) {
            FxListItem *item = visibleItems[i];
            if (item->index == -1)
                continue;
            qreal itemTop = item->position();
            if (itemTop+item->size()/2 >= pos && itemTop <= pos)
                return item->position();
        }
        if (visibleItems.count()) {
            qreal firstPos = visibleItems.first()->position();
            qreal endPos = visibleItems.last()->position();
            if (pos < firstPos) {
                return firstPos - qRound((firstPos - pos) / averageSize) * averageSize;
            } else if (pos > endPos)
                return endPos + qRound((pos - endPos) / averageSize) * averageSize;
        }
        return qRound((pos - startPosition()) / averageSize) * averageSize + startPosition();
    }

    FxListItem *snapItemAt(qreal pos) {
        for (int i = 0; i < visibleItems.count(); ++i) {
            FxListItem *item = visibleItems[i];
            if (item->index == -1)
                continue;
            qreal itemTop = item->position();
            if (item->index == model->count()-1 || (itemTop+item->size()/2 >= pos))
                return item;
        }
        if (visibleItems.count() && visibleItems.first()->position() <= pos)
            return visibleItems.first();
        return 0;
    }

    int lastVisibleIndex() const {
        int lastIndex = -1;
        for (int i = visibleItems.count()-1; i >= 0; --i) {
            FxListItem *listItem = visibleItems.at(i);
            if (listItem->index != -1) {
                lastIndex = listItem->index;
                break;
            }
        }
        return lastIndex;
    }

    // map a model index to visibleItems index.
    // These may differ if removed items are still present in the visible list,
    // e.g. doing a removal animation
    int mapFromModel(int modelIndex) const {
        if (modelIndex < visibleIndex || modelIndex >= visibleIndex + visibleItems.count())
            return -1;
        for (int i = 0; i < visibleItems.count(); ++i) {
            FxListItem *listItem = visibleItems.at(i);
            if (listItem->index == modelIndex)
                return i + visibleIndex;
            if (listItem->index > modelIndex)
                return -1;
        }
        return -1; // Not in visibleList
    }

    bool mapRangeFromModel(int &index, int &count) const {
        if (index + count < visibleIndex)
            return false;

        int lastIndex = -1;
        for (int i = visibleItems.count()-1; i >= 0; --i) {
            FxListItem *listItem = visibleItems.at(i);
            if (listItem->index != -1) {
                lastIndex = listItem->index;
                break;
            }
        }

        if (index > lastIndex)
            return false;

        int last = qMin(index + count - 1, lastIndex);
        index = qMax(index, visibleIndex);
        count = last - index + 1;

        return true;
    }

    void updateViewport() {
        Q_Q(QDeclarativeListView);
        if (orient == QDeclarativeListView::Vertical) {
            q->setContentHeight(endPosition() - startPosition() + 1);
        } else {
            q->setContentWidth(endPosition() - startPosition() + 1);
        }
    }

    void itemGeometryChanged(QDeclarativeItem *item, const QRectF &newGeometry, const QRectF &oldGeometry) {
        Q_Q(QDeclarativeListView);
        QDeclarativeFlickablePrivate::itemGeometryChanged(item, newGeometry, oldGeometry);
        if (item != contentItem && (!highlight || item != highlight->item)) {
            if ((orient == QDeclarativeListView::Vertical && newGeometry.height() != oldGeometry.height())
                || (orient == QDeclarativeListView::Horizontal && newGeometry.width() != oldGeometry.width())) {
                scheduleLayout();
            }
        }
        if ((header && header->item == item) || (footer && footer->item == item)) {
            updateHeader();
            updateFooter();
        }
        if (currentItem && currentItem->item == item)
            updateHighlight();
        if (trackedItem && trackedItem->item == item)
            q->trackedPositionChanged();
    }

    // for debugging only
    void checkVisible() const {
        int skip = 0;
        for (int i = 0; i < visibleItems.count(); ++i) {
            FxListItem *listItem = visibleItems.at(i);
            if (listItem->index == -1) {
                ++skip;
            } else if (listItem->index != visibleIndex + i - skip) {
                qFatal("index %d %d %d", visibleIndex, i, listItem->index);
            }
        }
    }

    void refill(qreal from, qreal to, bool doBuffer = false);
    void scheduleLayout();
    void layout();
    void updateUnrequestedIndexes();
    void updateUnrequestedPositions();
    void updateTrackedItem();
    void createHighlight();
    void updateHighlight();
    void createSection(FxListItem *);
    void updateSections();
    void updateCurrentSection();
    void updateCurrent(int);
    void updateAverage();
    void updateHeader();
    void updateFooter();
    void fixupPosition();
    virtual void fixup(AxisData &data, qreal minExtent, qreal maxExtent);
    virtual void flick(QDeclarativeFlickablePrivate::AxisData &data, qreal minExtent, qreal maxExtent, qreal vSize,
                        QDeclarativeTimeLineCallback::Callback fixupCallback, qreal velocity);

    QDeclarativeGuard<QDeclarativeVisualModel> model;
    QVariant modelVariant;
    QList<FxListItem*> visibleItems;
    QHash<QDeclarativeItem*,int> unrequestedItems;
    FxListItem *currentItem;
    QDeclarativeListView::Orientation orient;
    qreal visiblePos;
    int visibleIndex;
    qreal averageSize;
    int currentIndex;
    int requestedIndex;
    int itemCount;
    qreal highlightRangeStart;
    qreal highlightRangeEnd;
    QDeclarativeComponent *highlightComponent;
    FxListItem *highlight;
    FxListItem *trackedItem;
    enum MovementReason { Other, SetIndex, Mouse };
    MovementReason moveReason;
    int buffer;
    QSmoothedAnimation *highlightPosAnimator;
    QSmoothedAnimation *highlightSizeAnimator;
    QDeclarativeViewSection *sectionCriteria;
    QString currentSection;
    static const int sectionCacheSize = 4;
    QDeclarativeItem *sectionCache[sectionCacheSize];
    qreal spacing;
    qreal highlightMoveSpeed;
    int highlightMoveDuration;
    qreal highlightResizeSpeed;
    int highlightResizeDuration;
    QDeclarativeListView::HighlightRangeMode highlightRange;
    QDeclarativeListView::SnapMode snapMode;
    qreal overshootDist;
    QDeclarativeComponent *footerComponent;
    FxListItem *footer;
    QDeclarativeComponent *headerComponent;
    FxListItem *header;
    enum BufferMode { NoBuffer = 0x00, BufferBefore = 0x01, BufferAfter = 0x02 };
    int bufferMode;
    mutable qreal minExtent;
    mutable qreal maxExtent;

    bool ownModel : 1;
    bool wrap : 1;
    bool autoHighlight : 1;
    bool haveHighlightRange : 1;
    bool correctFlick : 1;
    bool inFlickCorrection : 1;
    bool lazyRelease : 1;
    bool deferredRelease : 1;
    bool layoutScheduled : 1;
    mutable bool minExtentDirty : 1;
    mutable bool maxExtentDirty : 1;
};

void QDeclarativeListViewPrivate::init()
{
    Q_Q(QDeclarativeListView);
    q->setFlag(QGraphicsItem::ItemIsFocusScope);
    addItemChangeListener(this, Geometry);
    QObject::connect(q, SIGNAL(movementEnded()), q, SLOT(animStopped()));
    q->setFlickableDirection(QDeclarativeFlickable::VerticalFlick);
    ::memset(sectionCache, 0, sizeof(QDeclarativeItem*) * sectionCacheSize);
}

void QDeclarativeListViewPrivate::clear()
{
    timeline.clear();
    for (int i = 0; i < visibleItems.count(); ++i)
        releaseItem(visibleItems.at(i));
    visibleItems.clear();
    for (int i = 0; i < sectionCacheSize; ++i) {
        delete sectionCache[i];
        sectionCache[i] = 0;
    }
    visiblePos = header ? header->size() : 0;
    visibleIndex = 0;
    releaseItem(currentItem);
    currentItem = 0;
    createHighlight();
    trackedItem = 0;
    minExtentDirty = true;
    maxExtentDirty = true;
    itemCount = 0;
}

FxListItem *QDeclarativeListViewPrivate::createItem(int modelIndex)
{
    Q_Q(QDeclarativeListView);
    // create object
    requestedIndex = modelIndex;
    FxListItem *listItem = 0;
    if (QDeclarativeItem *item = model->item(modelIndex, false)) {
        listItem = new FxListItem(item, q);
        listItem->index = modelIndex;
        // initialise attached properties
        if (sectionCriteria) {
            QString propValue = model->stringValue(modelIndex, sectionCriteria->property());
            listItem->attached->m_section = sectionCriteria->sectionString(propValue);
            if (modelIndex > 0) {
                if (FxListItem *item = visibleItem(modelIndex-1))
                    listItem->attached->m_prevSection = item->attached->section();
                else
                    listItem->attached->m_prevSection = sectionAt(modelIndex-1);
            }
            if (modelIndex < model->count()-1) {
                if (FxListItem *item = visibleItem(modelIndex+1))
                    listItem->attached->m_nextSection = item->attached->section();
                else
                    listItem->attached->m_nextSection = sectionAt(modelIndex+1);
            }
        }
        if (model->completePending()) {
            // complete
            listItem->item->setZValue(1);
            listItem->item->setParentItem(q->contentItem());
            model->completeItem();
        } else {
            listItem->item->setParentItem(q->contentItem());
        }
        QDeclarativeItemPrivate *itemPrivate = static_cast<QDeclarativeItemPrivate*>(QGraphicsItemPrivate::get(item));
        itemPrivate->addItemChangeListener(this, QDeclarativeItemPrivate::Geometry);
        if (sectionCriteria && sectionCriteria->delegate()) {
            if (listItem->attached->m_prevSection != listItem->attached->m_section)
                createSection(listItem);
        }
        unrequestedItems.remove(listItem->item);
    }
    requestedIndex = -1;

    return listItem;
}

void QDeclarativeListViewPrivate::releaseItem(FxListItem *item)
{
    Q_Q(QDeclarativeListView);
    if (!item || !model)
        return;
    if (trackedItem == item)
        trackedItem = 0;
    QDeclarativeItemPrivate *itemPrivate = static_cast<QDeclarativeItemPrivate*>(QGraphicsItemPrivate::get(item->item));
    itemPrivate->removeItemChangeListener(this, QDeclarativeItemPrivate::Geometry);
    if (model->release(item->item) == 0) {
        // item was not destroyed, and we no longer reference it.
        unrequestedItems.insert(item->item, model->indexOf(item->item, q));
    }
    if (item->section) {
        int i = 0;
        do {
            if (!sectionCache[i]) {
                sectionCache[i] = item->section;
                sectionCache[i]->setVisible(false);
                item->section = 0;
                break;
            }
            ++i;
        } while (i < sectionCacheSize);
        delete item->section;
    }
    delete item;
}

void QDeclarativeListViewPrivate::refill(qreal from, qreal to, bool doBuffer)
{
    Q_Q(QDeclarativeListView);
    if (!isValid() || !q->isComponentComplete())
        return;
    itemCount = model->count();
    qreal bufferFrom = from - buffer;
    qreal bufferTo = to + buffer;
    qreal fillFrom = from;
    qreal fillTo = to;
    if (doBuffer && (bufferMode & BufferAfter))
        fillTo = bufferTo;
    if (doBuffer && (bufferMode & BufferBefore))
        fillFrom = bufferFrom;

    int modelIndex = visibleIndex;
    qreal itemEnd = visiblePos-1;
    if (!visibleItems.isEmpty()) {
        visiblePos = (*visibleItems.constBegin())->position();
        itemEnd = (*(--visibleItems.constEnd()))->endPosition() + spacing;
        int i = visibleItems.count() - 1;
        while (i > 0 && visibleItems.at(i)->index == -1)
            --i;
        modelIndex = visibleItems.at(i)->index + 1;
    }

    bool changed = false;
    FxListItem *item = 0;
    qreal pos = itemEnd + 1;
    while (modelIndex < model->count() && pos <= fillTo) {
//        qDebug() << "refill: append item" << modelIndex << "pos" << pos;
        if (!(item = createItem(modelIndex)))
            break;
        item->setPosition(pos);
        pos += item->size() + spacing;
        visibleItems.append(item);
        ++modelIndex;
        changed = true;
        if (doBuffer) // never buffer more than one item per frame
            break;
    }
    while (visibleIndex > 0 && visibleIndex <= model->count() && visiblePos-1 >= fillFrom) {
//        qDebug() << "refill: prepend item" << visibleIndex-1 << "current top pos" << visiblePos;
        if (!(item = createItem(visibleIndex-1)))
            break;
        --visibleIndex;
        visiblePos -= item->size() + spacing;
        item->setPosition(visiblePos);
        visibleItems.prepend(item);
        changed = true;
        if (doBuffer) // never buffer more than one item per frame
            break;
    }

    if (!lazyRelease || !changed || deferredRelease) { // avoid destroying items in the same frame that we create
        while (visibleItems.count() > 1 && (item = visibleItems.first()) && item->endPosition() < bufferFrom) {
            if (item->attached->delayRemove())
                break;
//            qDebug() << "refill: remove first" << visibleIndex << "top end pos" << item->endPosition();
            if (item->index != -1)
                visibleIndex++;
            visibleItems.removeFirst();
            releaseItem(item);
            changed = true;
        }
        while (visibleItems.count() > 1 && (item = visibleItems.last()) && item->position() > bufferTo) {
            if (item->attached->delayRemove())
                break;
//            qDebug() << "refill: remove last" << visibleIndex+visibleItems.count()-1 << item->position();
            visibleItems.removeLast();
            releaseItem(item);
            changed = true;
        }
        deferredRelease = false;
    } else {
        deferredRelease = true;
    }
    if (changed) {
        minExtentDirty = true;
        maxExtentDirty = true;
        if (visibleItems.count())
            visiblePos = (*visibleItems.constBegin())->position();
        updateAverage();
        if (sectionCriteria)
            updateCurrentSection();
        if (header)
            updateHeader();
        if (footer)
            updateFooter();
        updateViewport();
        updateUnrequestedPositions();
    } else if (!doBuffer && buffer && bufferMode != NoBuffer) {
        refill(from, to, true);
    }
    lazyRelease = false;
}

void QDeclarativeListViewPrivate::scheduleLayout()
{
    Q_Q(QDeclarativeListView);
    if (!layoutScheduled) {
        layoutScheduled = true;
        QCoreApplication::postEvent(q, new QEvent(QEvent::User), Qt::HighEventPriority);
    }
}

void QDeclarativeListViewPrivate::layout()
{
    Q_Q(QDeclarativeListView);
    layoutScheduled = false;
    if (!isValid() && !visibleItems.count()) {
        clear();
        setPosition(0);
        return;
    }
    updateSections();
    if (!visibleItems.isEmpty()) {
        qreal oldEnd = visibleItems.last()->endPosition();
        qreal pos = visibleItems.first()->endPosition() + spacing + 1;
        for (int i=1; i < visibleItems.count(); ++i) {
            FxListItem *item = visibleItems.at(i);
            item->setPosition(pos);
            pos += item->size() + spacing;
        }
        // move current item if it is after the visible items.
        if (currentItem && currentIndex > lastVisibleIndex())
            currentItem->setPosition(currentItem->position() + (visibleItems.last()->endPosition() - oldEnd));
    }
    q->refill();
    minExtentDirty = true;
    maxExtentDirty = true;
    updateHighlight();
    fixupPosition();
    q->refill();
    if (header)
        updateHeader();
    if (footer)
        updateFooter();
    updateViewport();
}

void QDeclarativeListViewPrivate::updateUnrequestedIndexes()
{
    Q_Q(QDeclarativeListView);
    QHash<QDeclarativeItem*,int>::iterator it;
    for (it = unrequestedItems.begin(); it != unrequestedItems.end(); ++it)
        *it = model->indexOf(it.key(), q);
}

void QDeclarativeListViewPrivate::updateUnrequestedPositions()
{
    Q_Q(QDeclarativeListView);
    if (unrequestedItems.count()) {
        qreal pos = position();
        QHash<QDeclarativeItem*,int>::const_iterator it;
        for (it = unrequestedItems.begin(); it != unrequestedItems.end(); ++it) {
            QDeclarativeItem *item = it.key();
            if (orient == QDeclarativeListView::Vertical) {
                if (item->y() + item->height() > pos && item->y() < pos + q->height())
                    item->setY(positionAt(*it));
            } else {
                if (item->x() + item->width() > pos && item->x() < pos + q->width())
                    item->setX(positionAt(*it));
            }
        }
    }
}

void QDeclarativeListViewPrivate::updateTrackedItem()
{
    Q_Q(QDeclarativeListView);
    FxListItem *item = currentItem;
    if (highlight)
        item = highlight;
    trackedItem = item;
    if (trackedItem)
        q->trackedPositionChanged();
}

void QDeclarativeListViewPrivate::createHighlight()
{
    Q_Q(QDeclarativeListView);
    bool changed = false;
    if (highlight) {
        if (trackedItem == highlight)
            trackedItem = 0;
        delete highlight->item;
        delete highlight;
        highlight = 0;
        delete highlightPosAnimator;
        delete highlightSizeAnimator;
        highlightPosAnimator = 0;
        highlightSizeAnimator = 0;
        changed = true;
    }

    if (currentItem) {
        QDeclarativeItem *item = 0;
        if (highlightComponent) {
            QDeclarativeContext *highlightContext = new QDeclarativeContext(qmlContext(q));
            QObject *nobj = highlightComponent->create(highlightContext);
            if (nobj) {
                QDeclarative_setParent_noEvent(highlightContext, nobj);
                item = qobject_cast<QDeclarativeItem *>(nobj);
                if (!item)
                    delete nobj;
            } else {
                delete highlightContext;
            }
        } else {
            item = new QDeclarativeItem;
        }
        if (item) {
            QDeclarative_setParent_noEvent(item, q->contentItem());
            item->setParentItem(q->contentItem());
            highlight = new FxListItem(item, q);
            if (currentItem && autoHighlight) {
                if (orient == QDeclarativeListView::Vertical) {
                    highlight->item->setHeight(currentItem->item->height());
                } else {
                    highlight->item->setWidth(currentItem->item->width());
                }
            }
            QDeclarativeItemPrivate *itemPrivate = static_cast<QDeclarativeItemPrivate*>(QGraphicsItemPrivate::get(item));
            itemPrivate->addItemChangeListener(this, QDeclarativeItemPrivate::Geometry);
            const QLatin1String posProp(orient == QDeclarativeListView::Vertical ? "y" : "x");
            highlightPosAnimator = new QSmoothedAnimation(q);
            highlightPosAnimator->target = QDeclarativeProperty(highlight->item, posProp);
            highlightPosAnimator->velocity = highlightMoveSpeed;
            highlightPosAnimator->userDuration = highlightMoveDuration;
            const QLatin1String sizeProp(orient == QDeclarativeListView::Vertical ? "height" : "width");
            highlightSizeAnimator = new QSmoothedAnimation(q);
            highlightSizeAnimator->velocity = highlightResizeSpeed;
            highlightSizeAnimator->userDuration = highlightResizeDuration;
            highlightSizeAnimator->target = QDeclarativeProperty(highlight->item, sizeProp);
            if (autoHighlight) {
                highlightPosAnimator->restart();
                highlightSizeAnimator->restart();
            }
            changed = true;
        }
    }
    if (changed)
        emit q->highlightItemChanged();
}

void QDeclarativeListViewPrivate::updateHighlight()
{
    if ((!currentItem && highlight) || (currentItem && !highlight))
        createHighlight();
    if (currentItem && autoHighlight && highlight && !movingHorizontally && !movingVertically) {
        // auto-update highlight
        highlightPosAnimator->to = currentItem->position();
        highlightSizeAnimator->to = currentItem->size();
        if (orient == QDeclarativeListView::Vertical) {
            if (highlight->item->width() == 0)
                highlight->item->setWidth(currentItem->item->width());
        } else {
            if (highlight->item->height() == 0)
                highlight->item->setHeight(currentItem->item->height());
        }
        highlightPosAnimator->restart();
        highlightSizeAnimator->restart();
    }
    updateTrackedItem();
}

void QDeclarativeListViewPrivate::createSection(FxListItem *listItem)
{
    Q_Q(QDeclarativeListView);
    if (!sectionCriteria || !sectionCriteria->delegate())
        return;
    if (listItem->attached->m_prevSection != listItem->attached->m_section) {
        if (!listItem->section) {
            int i = sectionCacheSize-1;
            while (i >= 0 && !sectionCache[i])
                --i;
            if (i >= 0) {
                listItem->section = sectionCache[i];
                sectionCache[i] = 0;
                listItem->section->setVisible(true);
                QDeclarativeContext *context = QDeclarativeEngine::contextForObject(listItem->section)->parentContext();
                context->setContextProperty(QLatin1String("section"), listItem->attached->m_section);
            } else {
                QDeclarativeContext *context = new QDeclarativeContext(qmlContext(q));
                context->setContextProperty(QLatin1String("section"), listItem->attached->m_section);
                QObject *nobj = sectionCriteria->delegate()->create(context);
                if (nobj) {
                    QDeclarative_setParent_noEvent(context, nobj);
                    listItem->section = qobject_cast<QDeclarativeItem *>(nobj);
                    if (!listItem->section) {
                        delete nobj;
                    } else {
                        listItem->section->setZValue(1);
                        QDeclarative_setParent_noEvent(listItem->section, q->contentItem());
                        listItem->section->setParentItem(q->contentItem());
                    }
                } else {
                    delete context;
                }
            }
        }
    } else if (listItem->section) {
        int i = 0;
        do {
            if (!sectionCache[i]) {
                sectionCache[i] = listItem->section;
                sectionCache[i]->setVisible(false);
                listItem->section = 0;
                return;
            }
            ++i;
        } while (i < sectionCacheSize);
        delete listItem->section;
        listItem->section = 0;
    }
}

void QDeclarativeListViewPrivate::updateSections()
{
    if (sectionCriteria) {
        QString prevSection;
        if (visibleIndex > 0)
            prevSection = sectionAt(visibleIndex-1);
        QDeclarativeListViewAttached *prevAtt = 0;
        int idx = -1;
        for (int i = 0; i < visibleItems.count(); ++i) {
            if (visibleItems.at(i)->index != -1) {
                QDeclarativeListViewAttached *attached = visibleItems.at(i)->attached;
                attached->setPrevSection(prevSection);
                if (prevAtt)
                    prevAtt->setNextSection(attached->section());
                createSection(visibleItems.at(i));
                prevSection = attached->section();
                prevAtt = attached;
                idx = visibleItems.at(i)->index;
            }
        }
        if (prevAtt) {
            if (idx > 0 && idx < model->count()-1)
                prevAtt->setNextSection(sectionAt(idx+1));
            else
                prevAtt->setNextSection(QString());
        }
    }
}

void QDeclarativeListViewPrivate::updateCurrentSection()
{
    if (!sectionCriteria || visibleItems.isEmpty()) {
        currentSection = QString();
        return;
    }
    int index = 0;
    while (visibleItems.at(index)->endPosition() < position() && index < visibleItems.count())
        ++index;

    if (index < visibleItems.count())
        currentSection = visibleItems.at(index)->attached->section();
    else
        currentSection = visibleItems.first()->attached->section();
}

void QDeclarativeListViewPrivate::updateCurrent(int modelIndex)
{
    Q_Q(QDeclarativeListView);
    if (!q->isComponentComplete() || !isValid() || modelIndex < 0 || modelIndex >= model->count()) {
        if (currentItem) {
            currentItem->attached->setIsCurrentItem(false);
            releaseItem(currentItem);
            currentItem = 0;
            currentIndex = -1;
            updateHighlight();
            emit q->currentIndexChanged();
        }
        return;
    }

    if (currentItem && currentIndex == modelIndex) {
        updateHighlight();
        return;
    }
    FxListItem *oldCurrentItem = currentItem;
    currentIndex = modelIndex;
    currentItem = createItem(modelIndex);
    if (oldCurrentItem && (!currentItem || oldCurrentItem->item != currentItem->item))
        oldCurrentItem->attached->setIsCurrentItem(false);
    if (currentItem) {
        if (modelIndex == visibleIndex - 1 && visibleItems.count()) {
            // We can calculate exact postion in this case
            currentItem->setPosition(visibleItems.first()->position() - currentItem->size() - spacing);
        } else {
            // Create current item now and position as best we can.
            // Its position will be corrected when it becomes visible.
            currentItem->setPosition(positionAt(modelIndex));
        }
        currentItem->item->setFocus(true);
        currentItem->attached->setIsCurrentItem(true);
        // Avoid showing section delegate twice.  We still need the section heading so that
        // currentItem positioning works correctly.
        // This is slightly sub-optimal, but section heading caching minimizes the impact.
        if (currentItem->section)
            currentItem->section->setVisible(false);
    }
    updateHighlight();
    emit q->currentIndexChanged();
    // Release the old current item
    releaseItem(oldCurrentItem);
}

void QDeclarativeListViewPrivate::updateAverage()
{
    if (!visibleItems.count())
        return;
    qreal sum = 0.0;
    for (int i = 0; i < visibleItems.count(); ++i)
        sum += visibleItems.at(i)->size();
    averageSize = qRound(sum / visibleItems.count());
}

void QDeclarativeListViewPrivate::updateFooter()
{
    Q_Q(QDeclarativeListView);
    if (!footer && footerComponent) {
        QDeclarativeItem *item = 0;
        QDeclarativeContext *context = new QDeclarativeContext(qmlContext(q));
        QObject *nobj = footerComponent->create(context);
        if (nobj) {
            QDeclarative_setParent_noEvent(context, nobj);
            item = qobject_cast<QDeclarativeItem *>(nobj);
            if (!item)
                delete nobj;
        } else {
            delete context;
        }
        if (item) {
            QDeclarative_setParent_noEvent(item, q->contentItem());
            item->setParentItem(q->contentItem());
            item->setZValue(1);
            QDeclarativeItemPrivate *itemPrivate = static_cast<QDeclarativeItemPrivate*>(QGraphicsItemPrivate::get(item));
            itemPrivate->addItemChangeListener(this, QDeclarativeItemPrivate::Geometry);
            footer = new FxListItem(item, q);
        }
    }
    if (footer) {
        if (visibleItems.count()) {
            qreal endPos = endPosition() + 1;
            if (lastVisibleIndex() == model->count()-1) {
                footer->setPosition(endPos);
            } else {
                qreal visiblePos = position() + q->height();
                if (endPos <= visiblePos || footer->position() < endPos)
                    footer->setPosition(endPos);
            }
        } else {
            footer->setPosition(visiblePos);
        }
    }
}

void QDeclarativeListViewPrivate::updateHeader()
{
    Q_Q(QDeclarativeListView);
    if (!header && headerComponent) {
        QDeclarativeItem *item = 0;
        QDeclarativeContext *context = new QDeclarativeContext(qmlContext(q));
        QObject *nobj = headerComponent->create(context);
        if (nobj) {
            QDeclarative_setParent_noEvent(context, nobj);
            item = qobject_cast<QDeclarativeItem *>(nobj);
            if (!item)
                delete nobj;
        } else {
            delete context;
        }
        if (item) {
            QDeclarative_setParent_noEvent(item, q->contentItem());
            item->setParentItem(q->contentItem());
            item->setZValue(1);
            QDeclarativeItemPrivate *itemPrivate = static_cast<QDeclarativeItemPrivate*>(QGraphicsItemPrivate::get(item));
            itemPrivate->addItemChangeListener(this, QDeclarativeItemPrivate::Geometry);
            header = new FxListItem(item, q);
            if (visibleItems.isEmpty())
                visiblePos = header->size();
        }
    }
    if (header) {
        if (visibleItems.count()) {
            qreal startPos = startPosition();
            if (visibleIndex == 0) {
                header->setPosition(startPos - header->size());
            } else {
                if (position() <= startPos || header->position() > startPos - header->size())
                    header->setPosition(startPos - header->size());
            }
        } else {
            header->setPosition(0);
        }
    }
}

void QDeclarativeListViewPrivate::fixupPosition()
{
    if ((haveHighlightRange && highlightRange == QDeclarativeListView::StrictlyEnforceRange)
        || snapMode != QDeclarativeListView::NoSnap)
        moveReason = Other;
    if (orient == QDeclarativeListView::Vertical)
        fixupY();
    else
        fixupX();
}

void QDeclarativeListViewPrivate::fixup(AxisData &data, qreal minExtent, qreal maxExtent)
{
    if ((orient == QDeclarativeListView::Horizontal && &data == &vData)
        || (orient == QDeclarativeListView::Vertical && &data == &hData))
        return;

    int oldDuration = fixupDuration;
    fixupDuration = moveReason == Mouse ? fixupDuration : 0;

    if (snapMode != QDeclarativeListView::NoSnap) {
        FxListItem *topItem = snapItemAt(position()+highlightRangeStart);
        FxListItem *bottomItem = snapItemAt(position()+highlightRangeEnd);
        qreal pos;
        if (topItem && bottomItem && haveHighlightRange && highlightRange == QDeclarativeListView::StrictlyEnforceRange) {
            qreal topPos = qMin(topItem->position() - highlightRangeStart, -maxExtent);
            qreal bottomPos = qMax(bottomItem->position() - highlightRangeEnd, -minExtent);
            pos = qAbs(data.move + topPos) < qAbs(data.move + bottomPos) ? topPos : bottomPos;
        } else if (topItem) {
            pos = qMax(qMin(topItem->position() - highlightRangeStart, -maxExtent), -minExtent);
        } else if (bottomItem) {
            pos = qMax(qMin(bottomItem->position() - highlightRangeStart, -maxExtent), -minExtent);
        } else {
            fixupDuration = oldDuration;
            return;
        }
        if (currentItem && haveHighlightRange && highlightRange == QDeclarativeListView::StrictlyEnforceRange) {
            updateHighlight();
            qreal currPos = currentItem->position();
            if (pos < currPos + currentItem->size() - highlightRangeEnd)
                pos = currPos + currentItem->size() - highlightRangeEnd;
            if (pos > currPos - highlightRangeStart)
                pos = currPos - highlightRangeStart;
        }

        qreal dist = qAbs(data.move + pos);
        if (dist > 0) {
            timeline.reset(data.move);
            if (fixupDuration)
                timeline.move(data.move, -pos, QEasingCurve(QEasingCurve::InOutQuad), fixupDuration/2);
            else
                timeline.set(data.move, -pos);
            vTime = timeline.time();
        }
    } else if (haveHighlightRange && highlightRange == QDeclarativeListView::StrictlyEnforceRange) {
        if (currentItem) {
            updateHighlight();
            qreal pos = currentItem->position();
            qreal viewPos = position();
            if (viewPos < pos + currentItem->size() - highlightRangeEnd)
                viewPos = pos + currentItem->size() - highlightRangeEnd;
            if (viewPos > pos - highlightRangeStart)
                viewPos = pos - highlightRangeStart;

            timeline.reset(data.move);
            if (viewPos != position()) {
                if (fixupDuration)
                    timeline.move(data.move, -viewPos, QEasingCurve(QEasingCurve::InOutQuad), fixupDuration/2);
                else
                    timeline.set(data.move, -viewPos);
            }
            vTime = timeline.time();
        }
    } else {
        QDeclarativeFlickablePrivate::fixup(data, minExtent, maxExtent);
    }
    fixupDuration = oldDuration;
}

void QDeclarativeListViewPrivate::flick(AxisData &data, qreal minExtent, qreal maxExtent, qreal vSize,
                                        QDeclarativeTimeLineCallback::Callback fixupCallback, qreal velocity)
{
    Q_Q(QDeclarativeListView);

    moveReason = Mouse;
    if ((!haveHighlightRange || highlightRange != QDeclarativeListView::StrictlyEnforceRange) && snapMode == QDeclarativeListView::NoSnap) {
        correctFlick = true;
        QDeclarativeFlickablePrivate::flick(data, minExtent, maxExtent, vSize, fixupCallback, velocity);
        return;
    }
    qreal maxDistance = 0;
    // -ve velocity means list is moving up/left
    if (velocity > 0) {
        if (data.move.value() < minExtent) {
            if (snapMode == QDeclarativeListView::SnapOneItem) {
                if (FxListItem *item = firstVisibleItem())
                    maxDistance = qAbs(item->position() + data.move.value());
            } else {
                maxDistance = qAbs(minExtent - data.move.value());
            }
        }
        if (snapMode == QDeclarativeListView::NoSnap && highlightRange != QDeclarativeListView::StrictlyEnforceRange)
            data.flickTarget = minExtent;
    } else {
        if (data.move.value() > maxExtent) {
            if (snapMode == QDeclarativeListView::SnapOneItem) {
                if (FxListItem *item = nextVisibleItem())
                    maxDistance = qAbs(item->position() + data.move.value());
            } else {
                maxDistance = qAbs(maxExtent - data.move.value());
            }
        }
        if (snapMode == QDeclarativeListView::NoSnap && highlightRange != QDeclarativeListView::StrictlyEnforceRange)
            data.flickTarget = maxExtent;
    }
    bool overShoot = boundsBehavior == QDeclarativeFlickable::DragAndOvershootBounds;
    if (maxDistance > 0 || overShoot) {
        // These modes require the list to stop exactly on an item boundary.
        // The initial flick will estimate the boundary to stop on.
        // Since list items can have variable sizes, the boundary will be
        // reevaluated and adjusted as we approach the boundary.
        qreal v = velocity;
        if (maxVelocity != -1 && maxVelocity < qAbs(v)) {
            if (v < 0)
                v = -maxVelocity;
            else
                v = maxVelocity;
        }
        if (!flickingHorizontally && !flickingVertically) {
            // the initial flick - estimate boundary
            qreal accel = deceleration;
            qreal v2 = v * v;
            overshootDist = 0.0;
            // + averageSize/4 to encourage moving at least one item in the flick direction
            qreal dist = v2 / (accel * 2.0) + averageSize/4;
            if (maxDistance > 0)
                dist = qMin(dist, maxDistance);
            if (v > 0)
                dist = -dist;
            if ((maxDistance > 0.0 && v2 / (2.0f * maxDistance) < accel) || snapMode == QDeclarativeListView::SnapOneItem) {
                data.flickTarget = -snapPosAt(-(data.move.value() - highlightRangeStart) + dist) + highlightRangeStart;
                if (overShoot) {
                    if (data.flickTarget >= minExtent) {
                        overshootDist = overShootDistance(v, vSize);
                        data.flickTarget += overshootDist;
                    } else if (data.flickTarget <= maxExtent) {
                        overshootDist = overShootDistance(v, vSize);
                        data.flickTarget -= overshootDist;
                    }
                }
                qreal adjDist = -data.flickTarget + data.move.value();
                if (qAbs(adjDist) > qAbs(dist)) {
                    // Prevent painfully slow flicking - adjust velocity to suit flickDeceleration
                    qreal adjv2 = accel * 2.0f * qAbs(adjDist);
                    if (adjv2 > v2) {
                        v2 = adjv2;
                        v = qSqrt(v2);
                        if (dist > 0)
                            v = -v;
                    }
                }
                dist = adjDist;
                accel = v2 / (2.0f * qAbs(dist));
            } else if (overShoot) {
                data.flickTarget = data.move.value() - dist;
                if (data.flickTarget >= minExtent) {
                    overshootDist = overShootDistance(v, vSize);
                    data.flickTarget += overshootDist;
                } else if (data.flickTarget <= maxExtent) {
                    overshootDist = overShootDistance(v, vSize);
                    data.flickTarget -= overshootDist;
                }
            }
            timeline.reset(data.move);
            timeline.accel(data.move, v, accel, maxDistance + overshootDist);
            timeline.callback(QDeclarativeTimeLineCallback(&data.move, fixupCallback, this));
            if (!flickingHorizontally && q->xflick()) {
                flickingHorizontally = true;
                emit q->flickingChanged();
                emit q->flickingHorizontallyChanged();
                emit q->flickStarted();
            }
            if (!flickingVertically && q->yflick()) {
                flickingVertically = true;
                emit q->flickingChanged();
                emit q->flickingVerticallyChanged();
                emit q->flickStarted();
            }
            correctFlick = true;
        } else {
            // reevaluate the target boundary.
            qreal newtarget = data.flickTarget;
            if (snapMode != QDeclarativeListView::NoSnap || highlightRange == QDeclarativeListView::StrictlyEnforceRange)
                newtarget = -snapPosAt(-(data.flickTarget - highlightRangeStart)) + highlightRangeStart;
            if (velocity < 0 && newtarget < maxExtent)
                newtarget = maxExtent;
            else if (velocity > 0 && newtarget > minExtent)
                newtarget = minExtent;
            if (newtarget == data.flickTarget) // boundary unchanged - nothing to do
                return;
            data.flickTarget = newtarget;
            qreal dist = -newtarget + data.move.value();
            if ((v < 0 && dist < 0) || (v > 0 && dist > 0)) {
                correctFlick = false;
                timeline.reset(data.move);
                fixup(data, minExtent, maxExtent);
                return;
            }
            timeline.reset(data.move);
            timeline.accelDistance(data.move, v, -dist + (v < 0 ? -overshootDist : overshootDist));
            timeline.callback(QDeclarativeTimeLineCallback(&data.move, fixupCallback, this));
        }
    } else {
        correctFlick = false;
        timeline.reset(data.move);
        fixup(data, minExtent, maxExtent);
    }
}

//----------------------------------------------------------------------------

/*!
    \qmlclass ListView QDeclarativeListView
    \since 4.7
    \inherits Flickable
    \brief The ListView item provides a list view of items provided by a model.

    A ListView displays data from models created from built-in QML elements like ListModel
    and XmlListModel, or custom model classes defined in C++ that inherit from
    QAbstractListModel.

    A ListView has a \l model, which defines the data to be displayed, and
    a \l delegate, which defines how the data should be displayed. Items in a 
    ListView are laid out horizontally or vertically. List views are inherently flickable
    as ListView inherits from \l Flickable.

    For example, if there is a simple list model defined in a file \c ContactModel.qml like this:

    \snippet doc/src/snippets/declarative/listview/ContactModel.qml 0

    Another component can display this model data in a ListView, like this:

    \snippet doc/src/snippets/declarative/listview/listview.qml import
    \codeline
    \snippet doc/src/snippets/declarative/listview/listview.qml classdocs simple

    \image listview-simple.png

    Here, the ListView creates a \c ContactModel component for its model, and a \l Text element
    for its delegate. The view will create a new \l Text component for each item in the model. Notice
    the delegate is able to access the model's \c name and \c number data directly.

    An improved list view is shown below. The delegate is visually improved and is moved 
    into a separate \c contactDelegate component.
    
    \snippet doc/src/snippets/declarative/listview/listview.qml classdocs advanced
    \image listview-highlight.png

    The currently selected item is highlighted with a blue \l Rectangle using the \l highlight property,
    and \c focus is set to \c true to enable keyboard navigation for the list view.
    The list view itself is a focus scope (see \l{qmlfocus#Acquiring Focus and Focus Scopes}{the focus documentation page} for more details).

    Delegates are instantiated as needed and may be destroyed at any time.
    State should \e never be stored in a delegate.

    \note Views do not enable \e clip automatically.  If the view
    is not clipped by another item or the screen, it will be necessary
    to set \e {clip: true} in order to have the out of view items clipped
    nicely.

    \sa {Data Models}, GridView, {declarative/modelviews/listview}{ListView examples}
*/

QDeclarativeListView::QDeclarativeListView(QDeclarativeItem *parent)
    : QDeclarativeFlickable(*(new QDeclarativeListViewPrivate), parent)
{
    Q_D(QDeclarativeListView);
    d->init();
}

QDeclarativeListView::~QDeclarativeListView()
{
    Q_D(QDeclarativeListView);
    d->clear();
    if (d->ownModel)
        delete d->model;
    delete d->header;
    delete d->footer;
}

/*!
    \qmlattachedproperty bool ListView::isCurrentItem
    This attached property is true if this delegate is the current item; otherwise false.

    It is attached to each instance of the delegate.

    This property may be used to adjust the appearance of the current item, for example:

    \snippet doc/src/snippets/declarative/listview/listview.qml isCurrentItem
*/

/*!
    \qmlattachedproperty ListView ListView::view
    This attached property holds the view that manages this delegate instance.

    It is attached to each instance of the delegate.
*/

/*!
    \qmlattachedproperty string ListView::previousSection
    This attached property holds the section of the previous element.

    It is attached to each instance of the delegate.

    The section is evaluated using the \l {ListView::section.property}{section} properties.
*/

/*!
    \qmlattachedproperty string ListView::nextSection
    This attached property holds the section of the next element.

    It is attached to each instance of the delegate.

    The section is evaluated using the \l {ListView::section.property}{section} properties.
*/

/*!
    \qmlattachedproperty string ListView::section
    This attached property holds the section of this element.

    It is attached to each instance of the delegate.

    The section is evaluated using the \l {ListView::section.property}{section} properties.
*/

/*!
    \qmlattachedproperty bool ListView::delayRemove
    This attached property holds whether the delegate may be destroyed.

    It is attached to each instance of the delegate.

    It is sometimes necessary to delay the destruction of an item
    until an animation completes.

    The example delegate below ensures that the animation completes before
    the item is removed from the list.

    \snippet doc/src/snippets/declarative/listview/listview.qml delayRemove
*/

/*!
    \qmlattachedsignal ListView::onAdd()
    This attached handler is called immediately after an item is added to the view.
*/

/*!
    \qmlattachedsignal ListView::onRemove()
    This attached handler is called immediately before an item is removed from the view.
*/

/*!
    \qmlproperty model ListView::model
    This property holds the model providing data for the list.

    The model provides the set of data that is used to create the items
    in the view. Models can be created directly in QML using \l ListModel, \l XmlListModel
    or \l VisualItemModel, or provided by C++ model classes. If a C++ model class is
    used, it must be a subclass of \l QAbstractItemModel or a simple list.

    \sa {qmlmodels}{Data Models}
*/
QVariant QDeclarativeListView::model() const
{
    Q_D(const QDeclarativeListView);
    return d->modelVariant;
}

void QDeclarativeListView::setModel(const QVariant &model)
{
    Q_D(QDeclarativeListView);
    if (d->modelVariant == model)
        return;
    if (d->model) {
        disconnect(d->model, SIGNAL(itemsInserted(int,int)), this, SLOT(itemsInserted(int,int)));
        disconnect(d->model, SIGNAL(itemsRemoved(int,int)), this, SLOT(itemsRemoved(int,int)));
        disconnect(d->model, SIGNAL(itemsMoved(int,int,int)), this, SLOT(itemsMoved(int,int,int)));
        disconnect(d->model, SIGNAL(modelReset()), this, SLOT(modelReset()));
        disconnect(d->model, SIGNAL(createdItem(int, QDeclarativeItem*)), this, SLOT(createdItem(int,QDeclarativeItem*)));
        disconnect(d->model, SIGNAL(destroyingItem(QDeclarativeItem*)), this, SLOT(destroyingItem(QDeclarativeItem*)));
    }
    d->clear();
    QDeclarativeVisualModel *oldModel = d->model;
    d->model = 0;
    d->setPosition(0);
    d->modelVariant = model;
    QObject *object = qvariant_cast<QObject*>(model);
    QDeclarativeVisualModel *vim = 0;
    if (object && (vim = qobject_cast<QDeclarativeVisualModel *>(object))) {
        if (d->ownModel) {
            delete oldModel;
            d->ownModel = false;
        }
        d->model = vim;
    } else {
        if (!d->ownModel) {
            d->model = new QDeclarativeVisualDataModel(qmlContext(this), this);
            d->ownModel = true;
        } else {
            d->model = oldModel;
        }
        if (QDeclarativeVisualDataModel *dataModel = qobject_cast<QDeclarativeVisualDataModel*>(d->model))
            dataModel->setModel(model);
    }
    if (d->model) {
        d->bufferMode = QDeclarativeListViewPrivate::BufferBefore | QDeclarativeListViewPrivate::BufferAfter;
        if (isComponentComplete()) {
            refill();
            if (d->currentIndex >= d->model->count() || d->currentIndex < 0) {
                setCurrentIndex(0);
            } else {
                d->moveReason = QDeclarativeListViewPrivate::SetIndex;
                d->updateCurrent(d->currentIndex);
                if (d->highlight && d->currentItem) {
                    d->highlight->setPosition(d->currentItem->position());
                    d->updateTrackedItem();
                }
            }
        }
        connect(d->model, SIGNAL(itemsInserted(int,int)), this, SLOT(itemsInserted(int,int)));
        connect(d->model, SIGNAL(itemsRemoved(int,int)), this, SLOT(itemsRemoved(int,int)));
        connect(d->model, SIGNAL(itemsMoved(int,int,int)), this, SLOT(itemsMoved(int,int,int)));
        connect(d->model, SIGNAL(modelReset()), this, SLOT(modelReset()));
        connect(d->model, SIGNAL(createdItem(int, QDeclarativeItem*)), this, SLOT(createdItem(int,QDeclarativeItem*)));
        connect(d->model, SIGNAL(destroyingItem(QDeclarativeItem*)), this, SLOT(destroyingItem(QDeclarativeItem*)));
        emit countChanged();
    }
    emit modelChanged();
}

/*!
    \qmlproperty Component ListView::delegate

    The delegate provides a template defining each item instantiated by the view.
    The index is exposed as an accessible \c index property.  Properties of the
    model are also available depending upon the type of \l {qmlmodels}{Data Model}.

    The number of elements in the delegate has a direct effect on the
    flicking performance of the view.  If at all possible, place functionality
    that is not needed for the normal display of the delegate in a \l Loader which
    can load additional elements when needed.

    The ListView will lay out the items based on the size of the root item
    in the delegate.

    It is recommended that the delagate's size be a whole number to avoid sub-pixel
    alignment of items.

    \note Delegates are instantiated as needed and may be destroyed at any time.
    State should \e never be stored in a delegate.
*/
QDeclarativeComponent *QDeclarativeListView::delegate() const
{
    Q_D(const QDeclarativeListView);
    if (d->model) {
        if (QDeclarativeVisualDataModel *dataModel = qobject_cast<QDeclarativeVisualDataModel*>(d->model))
            return dataModel->delegate();
    }

    return 0;
}

void QDeclarativeListView::setDelegate(QDeclarativeComponent *delegate)
{
    Q_D(QDeclarativeListView);
    if (delegate == this->delegate())
        return;
    if (!d->ownModel) {
        d->model = new QDeclarativeVisualDataModel(qmlContext(this));
        d->ownModel = true;
    }
    if (QDeclarativeVisualDataModel *dataModel = qobject_cast<QDeclarativeVisualDataModel*>(d->model)) {
        dataModel->setDelegate(delegate);
        if (isComponentComplete()) {
            for (int i = 0; i < d->visibleItems.count(); ++i)
                d->releaseItem(d->visibleItems.at(i));
            d->visibleItems.clear();
            d->releaseItem(d->currentItem);
            d->currentItem = 0;
            refill();
            d->moveReason = QDeclarativeListViewPrivate::SetIndex;
            d->updateCurrent(d->currentIndex);
            if (d->highlight && d->currentItem) {
                d->highlight->setPosition(d->currentItem->position());
                d->updateTrackedItem();
            }
        }
    }
    emit delegateChanged();
}

/*!
    \qmlproperty int ListView::currentIndex
    \qmlproperty Item ListView::currentItem

    The \c currentIndex property holds the index of the current item, and
    \c currentItem holds the current item. 

    If highlightFollowsCurrentItem is \c true, setting either of these 
    properties will smoothly scroll the ListView so that the current 
    item becomes visible.
    
    Note that the position of the current item
    may only be approximate until it becomes visible in the view.
*/
int QDeclarativeListView::currentIndex() const
{
    Q_D(const QDeclarativeListView);
    return d->currentIndex;
}

void QDeclarativeListView::setCurrentIndex(int index)
{
    Q_D(QDeclarativeListView);
    if (d->requestedIndex >= 0)  // currently creating item
        return;
    if (isComponentComplete() && d->isValid() && index != d->currentIndex && index < d->model->count() && index >= 0) {
        d->moveReason = QDeclarativeListViewPrivate::SetIndex;
        d->updateCurrent(index);
    } else if (index != d->currentIndex) {
        d->currentIndex = index;
        emit currentIndexChanged();
    }
}

QDeclarativeItem *QDeclarativeListView::currentItem()
{
    Q_D(QDeclarativeListView);
    if (!d->currentItem)
        return 0;
    return d->currentItem->item;
}

/*!
  \qmlproperty Item ListView::highlightItem

    This holds the highlight item created from the \l highlight component.

  The \c highlightItem is managed by the view unless
  \l highlightFollowsCurrentItem is set to false.

  \sa highlight, highlightFollowsCurrentItem
*/
QDeclarativeItem *QDeclarativeListView::highlightItem()
{
    Q_D(QDeclarativeListView);
    if (!d->highlight)
        return 0;
    return d->highlight->item;
}

/*!
  \qmlproperty int ListView::count
  This property holds the number of items in the view.
*/
int QDeclarativeListView::count() const
{
    Q_D(const QDeclarativeListView);
    if (d->model)
        return d->model->count();
    return 0;
}

/*!
    \qmlproperty Component ListView::highlight
    This property holds the component to use as the highlight.

    An instance of the highlight component is created for each list.
    The geometry of the resulting component instance is managed by the list
    so as to stay with the current item, unless the highlightFollowsCurrentItem
    property is false.

    \sa highlightItem, highlightFollowsCurrentItem, {declarative/modelviews/listview}{ListView examples}
*/
QDeclarativeComponent *QDeclarativeListView::highlight() const
{
    Q_D(const QDeclarativeListView);
    return d->highlightComponent;
}

void QDeclarativeListView::setHighlight(QDeclarativeComponent *highlight)
{
    Q_D(QDeclarativeListView);
    if (highlight != d->highlightComponent) {
        d->highlightComponent = highlight;
        d->createHighlight();
        if (d->currentItem)
            d->updateHighlight();
        emit highlightChanged();
    }
}

/*!
    \qmlproperty bool ListView::highlightFollowsCurrentItem
    This property holds whether the highlight is managed by the view.

    If this property is true (the default value), the highlight is moved smoothly
    to follow the current item.  Otherwise, the
    highlight is not moved by the view, and any movement must be implemented
    by the highlight.  
    
    Here is a highlight with its motion defined by a \l {SpringAnimation} item:

    \snippet doc/src/snippets/declarative/listview/listview.qml highlightFollowsCurrentItem

    Note that the highlight animation also affects the way that the view
    is scrolled.  This is because the view moves to maintain the
    highlight within the preferred highlight range (or visible viewport).

    \sa highlight, highlightMoveSpeed
*/
bool QDeclarativeListView::highlightFollowsCurrentItem() const
{
    Q_D(const QDeclarativeListView);
    return d->autoHighlight;
}

void QDeclarativeListView::setHighlightFollowsCurrentItem(bool autoHighlight)
{
    Q_D(QDeclarativeListView);
    if (d->autoHighlight != autoHighlight) {
        d->autoHighlight = autoHighlight;
        if (autoHighlight) {
            d->updateHighlight();
        } else {
            if (d->highlightPosAnimator)
                d->highlightPosAnimator->stop();
            if (d->highlightSizeAnimator)
                d->highlightSizeAnimator->stop();
        }
        emit highlightFollowsCurrentItemChanged();
    }
}

//###Possibly rename these properties, since they are very useful even without a highlight?
/*!
    \qmlproperty real ListView::preferredHighlightBegin
    \qmlproperty real ListView::preferredHighlightEnd
    \qmlproperty enumeration ListView::highlightRangeMode

    These properties define the preferred range of the highlight (for the current item)
    within the view. The \c preferredHighlightBegin value must be less than the
    \c preferredHighlightEnd value. 

    These properties affect the position of the current item when the list is scrolled.
    For example, if the currently selected item should stay in the middle of the
    list when the view is scrolled, set the \c preferredHighlightBegin and 
    \c preferredHighlightEnd values to the top and bottom coordinates of where the middle 
    item would be. If the \c currentItem is changed programmatically, the list will
    automatically scroll so that the current item is in the middle of the view.
    Furthermore, the behavior of the current item index will occur whether or not a
    highlight exists.

    Valid values for \c highlightRangeMode are:

    \list
    \o ListView.ApplyRange - the view attempts to maintain the highlight within the range.
       However, the highlight can move outside of the range at the ends of the list or due
       to mouse interaction.
    \o ListView.StrictlyEnforceRange - the highlight never moves outside of the range.
       The current item changes if a keyboard or mouse action would cause the highlight to move
       outside of the range.
    \o ListView.NoHighlightRange - this is the default value.
    \endlist
*/
qreal QDeclarativeListView::preferredHighlightBegin() const
{
    Q_D(const QDeclarativeListView);
    return d->highlightRangeStart;
}

void QDeclarativeListView::setPreferredHighlightBegin(qreal start)
{
    Q_D(QDeclarativeListView);
    if (d->highlightRangeStart == start)
        return;
    d->highlightRangeStart = start;
    d->haveHighlightRange = d->highlightRange != NoHighlightRange && d->highlightRangeStart <= d->highlightRangeEnd;
    emit preferredHighlightBeginChanged();
}

qreal QDeclarativeListView::preferredHighlightEnd() const
{
    Q_D(const QDeclarativeListView);
    return d->highlightRangeEnd;
}

void QDeclarativeListView::setPreferredHighlightEnd(qreal end)
{
    Q_D(QDeclarativeListView);
    if (d->highlightRangeEnd == end)
        return;
    d->highlightRangeEnd = end;
    d->haveHighlightRange = d->highlightRange != NoHighlightRange && d->highlightRangeStart <= d->highlightRangeEnd;
    emit preferredHighlightEndChanged();
}

QDeclarativeListView::HighlightRangeMode QDeclarativeListView::highlightRangeMode() const
{
    Q_D(const QDeclarativeListView);
    return d->highlightRange;
}

void QDeclarativeListView::setHighlightRangeMode(HighlightRangeMode mode)
{
    Q_D(QDeclarativeListView);
    if (d->highlightRange == mode)
        return;
    d->highlightRange = mode;
    d->haveHighlightRange = d->highlightRange != NoHighlightRange && d->highlightRangeStart <= d->highlightRangeEnd;
    emit highlightRangeModeChanged();
}

/*!
    \qmlproperty real ListView::spacing

    This property holds the spacing between items.

    The default value is 0.
*/
qreal QDeclarativeListView::spacing() const
{
    Q_D(const QDeclarativeListView);
    return d->spacing;
}

void QDeclarativeListView::setSpacing(qreal spacing)
{
    Q_D(QDeclarativeListView);
    if (spacing != d->spacing) {
        d->spacing = spacing;
        d->layout();
        emit spacingChanged();
    }
}

/*!
    \qmlproperty enumeration ListView::orientation
    This property holds the orientation of the list.

    Possible values:

    \list
    \o ListView.Horizontal - Items are laid out horizontally
    \o ListView.Vertical (default) - Items are laid out vertically
    \endlist

    \table
    \row
    \o Horizontal orientation:
    \image ListViewHorizontal.png

    \row
    \o Vertical orientation:
    \image listview-highlight.png
    \endtable
*/
QDeclarativeListView::Orientation QDeclarativeListView::orientation() const
{
    Q_D(const QDeclarativeListView);
    return d->orient;
}

void QDeclarativeListView::setOrientation(QDeclarativeListView::Orientation orientation)
{
    Q_D(QDeclarativeListView);
    if (d->orient != orientation) {
        d->orient = orientation;
        if (d->orient == QDeclarativeListView::Vertical) {
            setContentWidth(-1);
            setFlickableDirection(VerticalFlick);
        } else {
            setContentHeight(-1);
            setFlickableDirection(HorizontalFlick);
        }
        d->clear();
        d->setPosition(0);
        refill();
        emit orientationChanged();
        d->updateCurrent(d->currentIndex);
    }
}

/*!
    \qmlproperty bool ListView::keyNavigationWraps
    This property holds whether the list wraps key navigation. 

    If this is true, key navigation that would move the current item selection
    past the end of the list instead wraps around and moves the selection to
    the start of the list, and vice-versa.

    By default, key navigation is not wrapped.
*/
bool QDeclarativeListView::isWrapEnabled() const
{
    Q_D(const QDeclarativeListView);
    return d->wrap;
}

void QDeclarativeListView::setWrapEnabled(bool wrap)
{
    Q_D(QDeclarativeListView);
    if (d->wrap == wrap)
        return;
    d->wrap = wrap;
    emit keyNavigationWrapsChanged();
}

/*!
    \qmlproperty int ListView::cacheBuffer
    This property determines whether delegates are retained outside the
    visible area of the view.

    If this value is non-zero, the view keeps as many delegates
    instantiated as it can fit within the buffer specified.  For example,
    if in a vertical view the delegate is 20 pixels high and \c cacheBuffer is
    set to 40, then up to 2 delegates above and 2 delegates below the visible
    area may be retained.

    Note that cacheBuffer is not a pixel buffer - it only maintains additional
    instantiated delegates.

    Setting this value can improve the smoothness of scrolling behavior at the expense
    of additional memory usage.  It is not a substitute for creating efficient
    delegates; the fewer elements in a delegate, the faster a view can be
    scrolled.
*/
int QDeclarativeListView::cacheBuffer() const
{
    Q_D(const QDeclarativeListView);
    return d->buffer;
}

void QDeclarativeListView::setCacheBuffer(int b)
{
    Q_D(QDeclarativeListView);
    if (d->buffer != b) {
        d->buffer = b;
        if (isComponentComplete()) {
            d->bufferMode = QDeclarativeListViewPrivate::BufferBefore | QDeclarativeListViewPrivate::BufferAfter;
            refill();
        }
        emit cacheBufferChanged();
    }
}

/*!
    \qmlproperty string ListView::section.property
    \qmlproperty enumeration ListView::section.criteria
    \qmlproperty Component ListView::section.delegate

    These properties hold the expression to be evaluated for the \l section attached property.

    The \l section attached property enables a ListView to be visually
    separated into different parts. These properties determine how sections
    are created.
    
    \c section.property holds the name of the property that is the basis
    of each section.

    \c section.criteria holds the criteria for forming each section based on
    \c section.property. This value can be one of:

    \list
    \o ViewSection.FullString (default) - sections are created based on the 
    \c section.property value.
    \o ViewSection.FirstCharacter - sections are created based on the first
    character of the \c section.property value (for example, 'A', 'B', 'C' 
    sections, etc. for an address book)
    \endlist

    \c section.delegate holds the delegate component for each section.

    Each item in the list has attached properties named \c ListView.section,
    \c ListView.previousSection and \c ListView.nextSection.  These may be
    used to place a section header for related items.

    For example, here is a ListView that displays a list of animals, separated 
    into sections. Each item in the ListView is placed in a different section 
    depending on the "size" property of the model item. The \c sectionHeading
    delegate component provides the light blue bar that marks the beginning of
    each section.
       
    \snippet examples/declarative/modelviews/listview/sections.qml 0

    \image qml-listview-sections-example.png

    \sa {declarative/modelviews/listview}{ListView examples}
*/
QDeclarativeViewSection *QDeclarativeListView::sectionCriteria()
{
    Q_D(QDeclarativeListView);
    if (!d->sectionCriteria)
        d->sectionCriteria = new QDeclarativeViewSection(this);
    return d->sectionCriteria;
}

/*!
    \qmlproperty string ListView::currentSection
    This property holds the section that is currently at the beginning of the view.
*/
QString QDeclarativeListView::currentSection() const
{
    Q_D(const QDeclarativeListView);
    return d->currentSection;
}

/*!
    \qmlproperty real ListView::highlightMoveSpeed
    \qmlproperty int ListView::highlightMoveDuration
    \qmlproperty real ListView::highlightResizeSpeed
    \qmlproperty int ListView::highlightResizeDuration

    These properties hold the move and resize animation speed of the highlight delegate.

    \l highlightFollowsCurrentItem must be true for these properties
    to have effect.

    The default value for the speed properties is 400 pixels/second.
    The default value for the duration properties is -1, i.e. the
    highlight will take as much time as necessary to move at the set speed.

    These properties have the same characteristics as a SmoothedAnimation.

    \sa highlightFollowsCurrentItem
*/
qreal QDeclarativeListView::highlightMoveSpeed() const
{
    Q_D(const QDeclarativeListView);\
    return d->highlightMoveSpeed;
}

void QDeclarativeListView::setHighlightMoveSpeed(qreal speed)
{
    Q_D(QDeclarativeListView);\
    if (d->highlightMoveSpeed != speed) {
        d->highlightMoveSpeed = speed;
        if (d->highlightPosAnimator)
            d->highlightPosAnimator->velocity = d->highlightMoveSpeed;
        emit highlightMoveSpeedChanged();
    }
}

int QDeclarativeListView::highlightMoveDuration() const
{
    Q_D(const QDeclarativeListView);
    return d->highlightMoveDuration;
}

void QDeclarativeListView::setHighlightMoveDuration(int duration)
{
    Q_D(QDeclarativeListView);\
    if (d->highlightMoveDuration != duration) {
        d->highlightMoveDuration = duration;
        if (d->highlightPosAnimator)
            d->highlightPosAnimator->userDuration = d->highlightMoveDuration;
        emit highlightMoveDurationChanged();
    }
}

qreal QDeclarativeListView::highlightResizeSpeed() const
{
    Q_D(const QDeclarativeListView);\
    return d->highlightResizeSpeed;
}

void QDeclarativeListView::setHighlightResizeSpeed(qreal speed)
{
    Q_D(QDeclarativeListView);\
    if (d->highlightResizeSpeed != speed) {
        d->highlightResizeSpeed = speed;
        if (d->highlightSizeAnimator)
            d->highlightSizeAnimator->velocity = d->highlightResizeSpeed;
        emit highlightResizeSpeedChanged();
    }
}

int QDeclarativeListView::highlightResizeDuration() const
{
    Q_D(const QDeclarativeListView);
    return d->highlightResizeDuration;
}

void QDeclarativeListView::setHighlightResizeDuration(int duration)
{
    Q_D(QDeclarativeListView);\
    if (d->highlightResizeDuration != duration) {
        d->highlightResizeDuration = duration;
        if (d->highlightSizeAnimator)
            d->highlightSizeAnimator->userDuration = d->highlightResizeDuration;
        emit highlightResizeDurationChanged();
    }
}

/*!
    \qmlproperty enumeration ListView::snapMode

    This property determines how the view scrolling will settle following a drag or flick.
    The possible values are:

    \list
    \o ListView.NoSnap (default) - the view stops anywhere within the visible area.
    \o ListView.SnapToItem - the view settles with an item aligned with the start of
    the view.
    \o ListView.SnapOneItem - the view settles no more than one item away from the first
    visible item at the time the mouse button is released.  This mode is particularly
    useful for moving one page at a time.
    \endlist

    \c snapMode does not affect the \l currentIndex.  To update the
    \l currentIndex as the list is moved, set \l highlightRangeMode
    to \c ListView.StrictlyEnforceRange.

    \sa highlightRangeMode
*/
QDeclarativeListView::SnapMode QDeclarativeListView::snapMode() const
{
    Q_D(const QDeclarativeListView);
    return d->snapMode;
}

void QDeclarativeListView::setSnapMode(SnapMode mode)
{
    Q_D(QDeclarativeListView);
    if (d->snapMode != mode) {
        d->snapMode = mode;
        emit snapModeChanged();
    }
}

/*!
    \qmlproperty Component ListView::footer
    This property holds the component to use as the footer.

    An instance of the footer component is created for each view.  The
    footer is positioned at the end of the view, after any items.

    \sa header
*/
QDeclarativeComponent *QDeclarativeListView::footer() const
{
    Q_D(const QDeclarativeListView);
    return d->footerComponent;
}

void QDeclarativeListView::setFooter(QDeclarativeComponent *footer)
{
    Q_D(QDeclarativeListView);
    if (d->footerComponent != footer) {
        if (d->footer) {
            delete d->footer;
            d->footer = 0;
        }
        d->footerComponent = footer;
        d->minExtentDirty = true;
        d->maxExtentDirty = true;
        d->updateFooter();
        d->updateViewport();
        emit footerChanged();
    }
}

/*!
    \qmlproperty Component ListView::header
    This property holds the component to use as the header.

    An instance of the header component is created for each view.  The
    header is positioned at the beginning of the view, before any items.

    \sa footer
*/
QDeclarativeComponent *QDeclarativeListView::header() const
{
    Q_D(const QDeclarativeListView);
    return d->headerComponent;
}

void QDeclarativeListView::setHeader(QDeclarativeComponent *header)
{
    Q_D(QDeclarativeListView);
    if (d->headerComponent != header) {
        if (d->header) {
            delete d->header;
            d->header = 0;
        }
        d->headerComponent = header;
        d->minExtentDirty = true;
        d->maxExtentDirty = true;
        d->updateHeader();
        d->updateFooter();
        d->updateViewport();
        emit headerChanged();
    }
}

void QDeclarativeListView::setContentX(qreal pos)
{
    Q_D(QDeclarativeListView);
    // Positioning the view manually should override any current movement state
    d->moveReason = QDeclarativeListViewPrivate::Other;
    QDeclarativeFlickable::setContentX(pos);
}

void QDeclarativeListView::setContentY(qreal pos)
{
    Q_D(QDeclarativeListView);
    // Positioning the view manually should override any current movement state
    d->moveReason = QDeclarativeListViewPrivate::Other;
    QDeclarativeFlickable::setContentY(pos);
}

bool QDeclarativeListView::event(QEvent *event)
{
    Q_D(QDeclarativeListView);
    if (event->type() == QEvent::User) {
        d->layout();
        return true;
    }

    return QDeclarativeFlickable::event(event);
}

void QDeclarativeListView::viewportMoved()
{
    Q_D(QDeclarativeListView);
    QDeclarativeFlickable::viewportMoved();
    d->lazyRelease = true;
    refill();
    if (d->flickingHorizontally || d->flickingVertically || d->movingHorizontally || d->movingVertically)
        d->moveReason = QDeclarativeListViewPrivate::Mouse;
    if (d->moveReason != QDeclarativeListViewPrivate::SetIndex) {
        if (d->haveHighlightRange && d->highlightRange == StrictlyEnforceRange && d->highlight) {
            // reposition highlight
            qreal pos = d->highlight->position();
            qreal viewPos = d->position();
            if (pos > viewPos + d->highlightRangeEnd - d->highlight->size())
                pos = viewPos + d->highlightRangeEnd - d->highlight->size();
            if (pos < viewPos + d->highlightRangeStart)
                pos = viewPos + d->highlightRangeStart;
            d->highlight->setPosition(qRound(pos));

            // update current index
            int idx = d->snapIndex();
            if (idx >= 0 && idx != d->currentIndex)
                d->updateCurrent(idx);
        }
    }

    if ((d->flickingHorizontally || d->flickingVertically) && d->correctFlick && !d->inFlickCorrection) {
        d->inFlickCorrection = true;
        // Near an end and it seems that the extent has changed?
        // Recalculate the flick so that we don't end up in an odd position.
        if (yflick()) {
            if (d->vData.velocity > 0) {
                const qreal minY = minYExtent();
                if ((minY - d->vData.move.value() < height()/2 || d->vData.flickTarget - d->vData.move.value() < height()/2)
                    && minY != d->vData.flickTarget)
                    d->flickY(-d->vData.smoothVelocity.value());
                d->bufferMode = QDeclarativeListViewPrivate::BufferBefore;
            } else if (d->vData.velocity < 0) {
                const qreal maxY = maxYExtent();
                if ((d->vData.move.value() - maxY < height()/2 || d->vData.move.value() - d->vData.flickTarget < height()/2)
                    && maxY != d->vData.flickTarget)
                    d->flickY(-d->vData.smoothVelocity.value());
                d->bufferMode = QDeclarativeListViewPrivate::BufferAfter;
            }
        }

        if (xflick()) {
            if (d->hData.velocity > 0) {
                const qreal minX = minXExtent();
                if ((minX - d->hData.move.value() < width()/2 || d->hData.flickTarget - d->hData.move.value() < width()/2)
                    && minX != d->hData.flickTarget)
                    d->flickX(-d->hData.smoothVelocity.value());
                d->bufferMode = QDeclarativeListViewPrivate::BufferBefore;
            } else if (d->hData.velocity < 0) {
                const qreal maxX = maxXExtent();
                if ((d->hData.move.value() - maxX < width()/2 || d->hData.move.value() - d->hData.flickTarget < width()/2)
                    && maxX != d->hData.flickTarget)
                    d->flickX(-d->hData.smoothVelocity.value());
                d->bufferMode = QDeclarativeListViewPrivate::BufferAfter;
            }
        }
        d->inFlickCorrection = false;
    }
}

qreal QDeclarativeListView::minYExtent() const
{
    Q_D(const QDeclarativeListView);
    if (d->orient == QDeclarativeListView::Horizontal)
        return QDeclarativeFlickable::minYExtent();
    if (d->minExtentDirty) {
        d->minExtent = -d->startPosition();
        if (d->header && d->visibleItems.count())
            d->minExtent += d->header->size();
        if (d->haveHighlightRange && d->highlightRange == StrictlyEnforceRange) {
            d->minExtent += d->highlightRangeStart;
            d->minExtent = qMax(d->minExtent, -(d->endPositionAt(0) - d->highlightRangeEnd + 1));
        }
        d->minExtentDirty = false;
    }

    return d->minExtent;
}

qreal QDeclarativeListView::maxYExtent() const
{
    Q_D(const QDeclarativeListView);
    if (d->orient == QDeclarativeListView::Horizontal)
        return height();
    if (d->maxExtentDirty) {
        if (!d->model || !d->model->count()) {
            d->maxExtent = 0;
        } else if (d->haveHighlightRange && d->highlightRange == StrictlyEnforceRange) {
            d->maxExtent = -(d->positionAt(d->model->count()-1) - d->highlightRangeStart);
            if (d->highlightRangeEnd != d->highlightRangeStart)
                d->maxExtent = qMin(d->maxExtent, -(d->endPosition() - d->highlightRangeEnd + 1));
        } else {
            d->maxExtent = -(d->endPosition() - height() + 1);
        }
        if (d->footer)
            d->maxExtent -= d->footer->size();
        qreal minY = minYExtent();
        if (d->maxExtent > minY)
            d->maxExtent = minY;
        d->maxExtentDirty = false;
    }
    return d->maxExtent;
}

qreal QDeclarativeListView::minXExtent() const
{
    Q_D(const QDeclarativeListView);
    if (d->orient == QDeclarativeListView::Vertical)
        return QDeclarativeFlickable::minXExtent();
    if (d->minExtentDirty) {
        d->minExtent = -d->startPosition();
        if (d->header)
            d->minExtent += d->header->size();
        if (d->haveHighlightRange && d->highlightRange == StrictlyEnforceRange) {
            d->minExtent += d->highlightRangeStart;
            d->minExtent = qMax(d->minExtent, -(d->endPositionAt(0) - d->highlightRangeEnd + 1));
        }
        d->minExtentDirty = false;
    }

    return d->minExtent;
}

qreal QDeclarativeListView::maxXExtent() const
{
    Q_D(const QDeclarativeListView);
    if (d->orient == QDeclarativeListView::Vertical)
        return width();
    if (d->maxExtentDirty) {
        if (!d->model || !d->model->count()) {
            d->maxExtent = 0;
        } else if (d->haveHighlightRange && d->highlightRange == StrictlyEnforceRange) {
            d->maxExtent = -(d->positionAt(d->model->count()-1) - d->highlightRangeStart);
            if (d->highlightRangeEnd != d->highlightRangeStart)
                d->maxExtent = qMin(d->maxExtent, -(d->endPosition() - d->highlightRangeEnd + 1));
        } else {
            d->maxExtent = -(d->endPosition() - width() + 1);
        }
        if (d->footer)
            d->maxExtent -= d->footer->size();
        qreal minX = minXExtent();
        if (d->maxExtent > minX)
            d->maxExtent = minX;
        d->maxExtentDirty = false;
    }

    return d->maxExtent;
}

void QDeclarativeListView::keyPressEvent(QKeyEvent *event)
{
    Q_D(QDeclarativeListView);
    keyPressPreHandler(event);
    if (event->isAccepted())
        return;

    if (d->model && d->model->count() && d->interactive) {
        if ((d->orient == QDeclarativeListView::Horizontal && event->key() == Qt::Key_Left)
                    || (d->orient == QDeclarativeListView::Vertical && event->key() == Qt::Key_Up)) {
            if (currentIndex() > 0 || (d->wrap && !event->isAutoRepeat())) {
                decrementCurrentIndex();
                event->accept();
                return;
            } else if (d->wrap) {
                event->accept();
                return;
            }
        } else if ((d->orient == QDeclarativeListView::Horizontal && event->key() == Qt::Key_Right)
                    || (d->orient == QDeclarativeListView::Vertical && event->key() == Qt::Key_Down)) {
            if (currentIndex() < d->model->count() - 1 || (d->wrap && !event->isAutoRepeat())) {
                incrementCurrentIndex();
                event->accept();
                return;
            } else if (d->wrap) {
                event->accept();
                return;
            }
        }
    }
    event->ignore();
    QDeclarativeFlickable::keyPressEvent(event);
}

/*!
    \qmlmethod ListView::incrementCurrentIndex()

    Increments the current index.  The current index will wrap
    if keyNavigationWraps is true and it is currently at the end.

    \bold Note: methods should only be called after the Component has completed.
*/
void QDeclarativeListView::incrementCurrentIndex()
{
    Q_D(QDeclarativeListView);
    if (currentIndex() < d->model->count() - 1 || d->wrap) {
        d->moveReason = QDeclarativeListViewPrivate::SetIndex;
        int index = currentIndex()+1;
        d->updateCurrent(index < d->model->count() ? index : 0);
    }
}

/*!
    \qmlmethod ListView::decrementCurrentIndex()

    Decrements the current index.  The current index will wrap
    if keyNavigationWraps is true and it is currently at the beginning.

    \bold Note: methods should only be called after the Component has completed.
*/
void QDeclarativeListView::decrementCurrentIndex()
{
    Q_D(QDeclarativeListView);
    if (currentIndex() > 0 || d->wrap) {
        d->moveReason = QDeclarativeListViewPrivate::SetIndex;
        int index = currentIndex()-1;
        d->updateCurrent(index >= 0 ? index : d->model->count()-1);
    }
}

/*!
    \qmlmethod ListView::positionViewAtIndex(int index, PositionMode mode)

    Positions the view such that the \a index is at the position specified by
    \a mode:

    \list
    \o ListView.Beginning - position item at the top (or left for horizontal orientation) of the view.
    \o ListView.Center - position item in the center of the view.
    \o ListView.End - position item at bottom (or right for horizontal orientation) of the view.
    \o ListView.Visible - if any part of the item is visible then take no action, otherwise
    bring the item into view.
    \o ListView.Contain - ensure the entire item is visible.  If the item is larger than
    the view the item is positioned at the top (or left for horizontal orientation) of the view.
    \endlist

    If positioning the view at \a index would cause empty space to be displayed at
    the beginning or end of the view, the view will be positioned at the boundary.

    It is not recommended to use \l {Flickable::}{contentX} or \l {Flickable::}{contentY} to position the view
    at a particular index.  This is unreliable since removing items from the start
    of the list does not cause all other items to be repositioned, and because
    the actual start of the view can vary based on the size of the delegates.
    The correct way to bring an item into view is with \c positionViewAtIndex.

    \bold Note: methods should only be called after the Component has completed.  To position
    the view at startup, this method should be called by Component.onCompleted.  For
    example, to position the view at the end:

    \code
    Component.onCompleted: positionViewAtIndex(count - 1, ListView.Beginning)
    \endcode
*/
void QDeclarativeListView::positionViewAtIndex(int index, int mode)
{
    Q_D(QDeclarativeListView);
    if (!d->isValid() || index < 0 || index >= d->model->count())
        return;
    if (mode < Beginning || mode > Contain)
        return;

    if (d->layoutScheduled)
        d->layout();
    qreal pos = d->position();
    FxListItem *item = d->visibleItem(index);
    if (!item) {
        int itemPos = d->positionAt(index);
        // save the currently visible items in case any of them end up visible again
        QList<FxListItem*> oldVisible = d->visibleItems;
        d->visibleItems.clear();
        d->visiblePos = itemPos;
        d->visibleIndex = index;
        d->setPosition(itemPos);
        // now release the reference to all the old visible items.
        for (int i = 0; i < oldVisible.count(); ++i)
            d->releaseItem(oldVisible.at(i));
        item = d->visibleItem(index);
    }
    if (item) {
        const qreal itemPos = item->position();
        switch (mode) {
        case Beginning:
            pos = itemPos;
            break;
        case Center:
            pos = itemPos - (d->size() - item->size())/2;
            break;
        case End:
            pos = itemPos - d->size() + item->size();
            break;
        case Visible:
            if (itemPos > pos + d->size())
                pos = itemPos - d->size() + item->size();
            else if (item->endPosition() < pos)
                pos = itemPos;
            break;
        case Contain:
            if (item->endPosition() > pos + d->size())
                pos = itemPos - d->size() + item->size();
            if (itemPos < pos)
                pos = itemPos;
        }
        qreal maxExtent = d->orient == QDeclarativeListView::Vertical ? -maxYExtent() : -maxXExtent();
        pos = qMin(pos, maxExtent);
        qreal minExtent = d->orient == QDeclarativeListView::Vertical ? -minYExtent() : -minXExtent();
        pos = qMax(pos, minExtent);
        d->moveReason = QDeclarativeListViewPrivate::Other;
        cancelFlick();
        d->setPosition(pos);
    }
    d->fixupPosition();
}

/*!
    \qmlmethod int ListView::indexAt(int x, int y)

    Returns the index of the visible item containing the point \a x, \a y in content
    coordinates.  If there is no item at the point specified, or the item is
    not visible -1 is returned.

    If the item is outside the visible area, -1 is returned, regardless of
    whether an item will exist at that point when scrolled into view.

    \bold Note: methods should only be called after the Component has completed.
*/
int QDeclarativeListView::indexAt(int x, int y) const
{
    Q_D(const QDeclarativeListView);
    for (int i = 0; i < d->visibleItems.count(); ++i) {
        const FxListItem *listItem = d->visibleItems.at(i);
        if(listItem->contains(x, y))
            return listItem->index;
    }

    return -1;
}

void QDeclarativeListView::componentComplete()
{
    Q_D(QDeclarativeListView);
    QDeclarativeFlickable::componentComplete();
    if (d->isValid()) {
        refill();
        d->moveReason = QDeclarativeListViewPrivate::SetIndex;
        if (d->currentIndex < 0)
            d->updateCurrent(0);
        else
            d->updateCurrent(d->currentIndex);
        if (d->highlight && d->currentItem) {
            d->highlight->setPosition(d->currentItem->position());
            d->updateTrackedItem();
        }
        d->moveReason = QDeclarativeListViewPrivate::Other;
        d->fixupPosition();
    }
}

void QDeclarativeListView::refill()
{
    Q_D(QDeclarativeListView);
    d->refill(d->position(), d->position()+d->size()-1);
}

void QDeclarativeListView::trackedPositionChanged()
{
    Q_D(QDeclarativeListView);
    if (!d->trackedItem || !d->currentItem)
        return;
    if (d->moveReason == QDeclarativeListViewPrivate::SetIndex) {
        const qreal trackedPos = qCeil(d->trackedItem->position());
        const qreal viewPos = d->position();
        qreal pos = viewPos;
        if (d->haveHighlightRange) {
            if (d->highlightRange == StrictlyEnforceRange) {
                if (trackedPos > pos + d->highlightRangeEnd - d->trackedItem->size())
                    pos = trackedPos - d->highlightRangeEnd + d->trackedItem->size();
                if (trackedPos < pos + d->highlightRangeStart)
                    pos = trackedPos - d->highlightRangeStart;
            } else {
                if (trackedPos < d->startPosition() + d->highlightRangeStart) {
                    pos = d->startPosition();
                } else if (d->trackedItem->endPosition() > d->endPosition() - d->size() + d->highlightRangeEnd) {
                    pos = d->endPosition() - d->size();
                    if (pos < d->startPosition())
                        pos = d->startPosition();
                } else {
                    if (trackedPos < viewPos + d->highlightRangeStart) {
                        pos = trackedPos - d->highlightRangeStart;
                    } else if (trackedPos > viewPos + d->highlightRangeEnd - d->trackedItem->size()) {
                        pos = trackedPos - d->highlightRangeEnd + d->trackedItem->size();
                    }
                }
            }
        } else {
            if (trackedPos < viewPos && d->currentItem->position() < viewPos) {
                pos = d->currentItem->position() < trackedPos ? trackedPos : d->currentItem->position();
            } else if (d->trackedItem->endPosition() > viewPos + d->size()
                        && d->currentItem->endPosition() > viewPos + d->size()) {
                if (d->trackedItem->endPosition() < d->currentItem->endPosition()) {
                    pos = d->trackedItem->endPosition() - d->size();
                    if (d->trackedItem->size() > d->size())
                        pos = trackedPos;
                } else {
                    pos = d->currentItem->endPosition() - d->size();
                    if (d->currentItem->size() > d->size())
                        pos = d->currentItem->position();
                }
            }
        }
        if (viewPos != pos) {
            cancelFlick();
            d->setPosition(pos);
        }
    }
}

void QDeclarativeListView::itemsInserted(int modelIndex, int count)
{
    Q_D(QDeclarativeListView);
    if (!isComponentComplete())
        return;
    d->updateUnrequestedIndexes();
    d->moveReason = QDeclarativeListViewPrivate::Other;
    if (!d->visibleItems.count() || d->model->count() <= 1) {
        d->scheduleLayout();
        if (d->currentIndex >= modelIndex) {
            // adjust current item index
            d->currentIndex += count;
            if (d->currentItem)
                d->currentItem->index = d->currentIndex;
            emit currentIndexChanged();
        } else if (d->currentIndex < 0) {
            d->updateCurrent(0);
        }
        d->itemCount += count;
        emit countChanged();
        return;
    }

    int overlapCount = count;
    if (!d->mapRangeFromModel(modelIndex, overlapCount)) {
        int i = d->visibleItems.count() - 1;
        while (i > 0 && d->visibleItems.at(i)->index == -1)
            --i;
        if (d->visibleItems.at(i)->index + 1 == modelIndex
            && d->visibleItems.at(i)->endPosition() < d->buffer+d->position()+d->size()-1) {
            // Special case of appending an item to the model.
            modelIndex = d->visibleIndex + d->visibleItems.count();
        } else {
            if (modelIndex < d->visibleIndex) {
                // Insert before visible items
                d->visibleIndex += count;
                for (int i = 0; i < d->visibleItems.count(); ++i) {
                    FxListItem *listItem = d->visibleItems.at(i);
                    if (listItem->index != -1 && listItem->index >= modelIndex)
                        listItem->index += count;
                }
            }
            if (d->currentIndex >= modelIndex) {
                // adjust current item index
                d->currentIndex += count;
                if (d->currentItem)
                    d->currentItem->index = d->currentIndex;
                emit currentIndexChanged();
            }
            d->scheduleLayout();
            d->itemCount += count;
            emit countChanged();
            return;
        }
    }

    // At least some of the added items will be visible

    int index = modelIndex - d->visibleIndex;
    // index can be the next item past the end of the visible items list (i.e. appended)
    int pos = index < d->visibleItems.count() ? d->visibleItems.at(index)->position()
                                                : d->visibleItems.at(index-1)->endPosition()+d->spacing+1;
    int initialPos = pos;
    int diff = 0;
    QList<FxListItem*> added;
    bool addedVisible = false;
    FxListItem *firstVisible = d->firstVisibleItem();
    if (firstVisible && pos < firstVisible->position()) {
        // Insert items before the visible item.
        int insertionIdx = index;
        int i = 0;
        int from = d->position() - d->buffer;
        for (i = count-1; i >= 0 && pos > from; --i) {
            if (!addedVisible) {
                d->scheduleLayout();
                addedVisible = true;
            }
            FxListItem *item = d->createItem(modelIndex + i);
            d->visibleItems.insert(insertionIdx, item);
            pos -= item->size() + d->spacing;
            item->setPosition(pos);
            index++;
        }
        if (i >= 0) {
            // If we didn't insert all our new items - anything
            // before the current index is not visible - remove it.
            while (insertionIdx--) {
                FxListItem *item = d->visibleItems.takeFirst();
                if (item->index != -1)
                    d->visibleIndex++;
                d->releaseItem(item);
            }
        } else {
            // adjust pos of items before inserted items.
            for (int i = insertionIdx-1; i >= 0; i--) {
                FxListItem *listItem = d->visibleItems.at(i);
                listItem->setPosition(listItem->position() - (initialPos - pos));
            }
        }
    } else {
        int i = 0;
        int to = d->buffer+d->position()+d->size()-1;
        for (i = 0; i < count && pos <= to; ++i) {
            if (!addedVisible) {
                d->scheduleLayout();
                addedVisible = true;
            }
            FxListItem *item = d->createItem(modelIndex + i);
            d->visibleItems.insert(index, item);
            item->setPosition(pos);
            added.append(item);
            pos += item->size() + d->spacing;
            ++index;
        }
        if (i != count) {
            // We didn't insert all our new items, which means anything
            // beyond the current index is not visible - remove it.
            while (d->visibleItems.count() > index)
                d->releaseItem(d->visibleItems.takeLast());
        }
        diff = pos - initialPos;
    }
    if (d->currentIndex >= modelIndex) {
        // adjust current item index
        d->currentIndex += count;
        if (d->currentItem) {
            d->currentItem->index = d->currentIndex;
            d->currentItem->setPosition(d->currentItem->position() + diff);
        }
        emit currentIndexChanged();
    }
    // Update the indexes of the following visible items.
    for (; index < d->visibleItems.count(); ++index) {
        FxListItem *listItem = d->visibleItems.at(index);
        if (d->currentItem && listItem->item != d->currentItem->item)
            listItem->setPosition(listItem->position() + diff);
        if (listItem->index != -1)
            listItem->index += count;
    }
    // everything is in order now - emit add() signal
    for (int j = 0; j < added.count(); ++j)
        added.at(j)->attached->emitAdd();

    d->itemCount += count;
    emit countChanged();
}

void QDeclarativeListView::itemsRemoved(int modelIndex, int count)
{
    Q_D(QDeclarativeListView);
    if (!isComponentComplete())
        return;
    d->moveReason = QDeclarativeListViewPrivate::Other;
    d->updateUnrequestedIndexes();
    d->itemCount -= count;

    FxListItem *firstVisible = d->firstVisibleItem();
    int preRemovedSize = 0;
    bool removedVisible = false;
    // Remove the items from the visible list, skipping anything already marked for removal
    QList<FxListItem*>::Iterator it = d->visibleItems.begin();
    while (it != d->visibleItems.end()) {
        FxListItem *item = *it;
        if (item->index == -1 || item->index < modelIndex) {
            // already removed, or before removed items
            ++it;
        } else if (item->index >= modelIndex + count) {
            // after removed items
            item->index -= count;
            ++it;
        } else {
            // removed item
            if (!removedVisible) {
                d->scheduleLayout();
                removedVisible = true;
            }
            item->attached->emitRemove();
            if (item->attached->delayRemove()) {
                item->index = -1;
                connect(item->attached, SIGNAL(delayRemoveChanged()), this, SLOT(destroyRemoved()), Qt::QueuedConnection);
                ++it;
            } else {
                if (item == firstVisible)
                    firstVisible = 0;
                if (firstVisible && item->position() < firstVisible->position())
                    preRemovedSize += item->size();
                it = d->visibleItems.erase(it);
                d->releaseItem(item);
            }
        }
    }

    if (firstVisible && d->visibleItems.first() != firstVisible)
        d->visibleItems.first()->setPosition(d->visibleItems.first()->position() + preRemovedSize);

    // fix current
    if (d->currentIndex >= modelIndex + count) {
        d->currentIndex -= count;
        if (d->currentItem)
            d->currentItem->index -= count;
        emit currentIndexChanged();
    } else if (d->currentIndex >= modelIndex && d->currentIndex < modelIndex + count) {
        // current item has been removed.
        d->currentItem->attached->setIsCurrentItem(false);
        d->releaseItem(d->currentItem);
        d->currentItem = 0;
        d->currentIndex = -1;
        if (d->itemCount)
            d->updateCurrent(qMin(modelIndex, d->itemCount-1));
    }

    // update visibleIndex
    for (it = d->visibleItems.begin(); it != d->visibleItems.end(); ++it) {
        if ((*it)->index != -1) {
            d->visibleIndex = (*it)->index;
            break;
        }
    }

    if (removedVisible && d->visibleItems.isEmpty()) {
        d->visibleIndex = 0;
        d->visiblePos = d->header ? d->header->size() : 0;
        d->timeline.clear();
        d->setPosition(0);
        if (d->itemCount == 0) {
            d->updateHeader();
            d->updateFooter();
            update();
        }
    }

    emit countChanged();
}

void QDeclarativeListView::destroyRemoved()
{
    Q_D(QDeclarativeListView);
    for (QList<FxListItem*>::Iterator it = d->visibleItems.begin();
            it != d->visibleItems.end();) {
        FxListItem *listItem = *it;
        if (listItem->index == -1 && listItem->attached->delayRemove() == false) {
            d->releaseItem(listItem);
            it = d->visibleItems.erase(it);
        } else {
            ++it;
        }
    }

    // Correct the positioning of the items
    d->layout();
}

void QDeclarativeListView::itemsMoved(int from, int to, int count)
{
    Q_D(QDeclarativeListView);
    if (!isComponentComplete())
        return;
    d->updateUnrequestedIndexes();

    if (d->visibleItems.isEmpty()) {
        refill();
        return;
    }

    d->moveReason = QDeclarativeListViewPrivate::Other;
    FxListItem *firstVisible = d->firstVisibleItem();
    qreal firstItemPos = firstVisible->position();
    QHash<int,FxListItem*> moved;
    int moveBy = 0;

    QList<FxListItem*>::Iterator it = d->visibleItems.begin();
    while (it != d->visibleItems.end()) {
        FxListItem *item = *it;
        if (item->index >= from && item->index < from + count) {
            // take the items that are moving
            item->index += (to-from);
            moved.insert(item->index, item);
            if (item->position() < firstItemPos)
                moveBy += item->size();
            it = d->visibleItems.erase(it);
        } else {
            // move everything after the moved items.
            if (item->index > from && item->index != -1)
                item->index -= count;
            ++it;
        }
    }

    int remaining = count;
    int endIndex = d->visibleIndex;
    it = d->visibleItems.begin();
    while (it != d->visibleItems.end()) {
        FxListItem *item = *it;
        if (remaining && item->index >= to && item->index < to + count) {
            // place items in the target position, reusing any existing items
            FxListItem *movedItem = moved.take(item->index);
            if (!movedItem)
                movedItem = d->createItem(item->index);
            if (item->index <= firstVisible->index)
                moveBy -= movedItem->size();
            it = d->visibleItems.insert(it, movedItem);
            ++it;
            --remaining;
        } else {
            if (item->index != -1) {
                if (item->index >= to) {
                    // update everything after the moved items.
                    item->index += count;
                }
                endIndex = item->index;
            }
            ++it;
        }
    }

    // If we have moved items to the end of the visible items
    // then add any existing moved items that we have
    while (FxListItem *item = moved.take(endIndex+1)) {
        d->visibleItems.append(item);
        ++endIndex;
    }

    // update visibleIndex
    for (it = d->visibleItems.begin(); it != d->visibleItems.end(); ++it) {
        if ((*it)->index != -1) {
            d->visibleIndex = (*it)->index;
            break;
        }
    }

    // Fix current index
    if (d->currentIndex >= 0 && d->currentItem) {
        int oldCurrent = d->currentIndex;
        d->currentIndex = d->model->indexOf(d->currentItem->item, this);
        if (oldCurrent != d->currentIndex) {
            d->currentItem->index = d->currentIndex;
            emit currentIndexChanged();
        }
    }

    // Whatever moved items remain are no longer visible items.
    while (moved.count()) {
        int idx = moved.begin().key();
        FxListItem *item = moved.take(idx);
        if (item->item == d->currentItem->item)
            item->setPosition(d->positionAt(idx));
        d->releaseItem(item);
    }

    // Ensure we don't cause an ugly list scroll.
    d->visibleItems.first()->setPosition(d->visibleItems.first()->position() + moveBy);

    d->layout();
}

void QDeclarativeListView::modelReset()
{
    Q_D(QDeclarativeListView);
    d->clear();
    d->setPosition(0);
    refill();
    d->moveReason = QDeclarativeListViewPrivate::SetIndex;
    d->updateCurrent(d->currentIndex);
    if (d->highlight && d->currentItem) {
        d->highlight->setPosition(d->currentItem->position());
        d->updateTrackedItem();
    }
    d->moveReason = QDeclarativeListViewPrivate::Other;
    emit countChanged();
}

void QDeclarativeListView::createdItem(int index, QDeclarativeItem *item)
{
    Q_D(QDeclarativeListView);
    if (d->requestedIndex != index) {
        item->setParentItem(contentItem());
        d->unrequestedItems.insert(item, index);
        if (d->orient == QDeclarativeListView::Vertical)
            item->setY(d->positionAt(index));
        else
            item->setX(d->positionAt(index));
    }
}

void QDeclarativeListView::destroyingItem(QDeclarativeItem *item)
{
    Q_D(QDeclarativeListView);
    d->unrequestedItems.remove(item);
}

void QDeclarativeListView::animStopped()
{
    Q_D(QDeclarativeListView);
    d->bufferMode = QDeclarativeListViewPrivate::NoBuffer;
    if (d->haveHighlightRange && d->highlightRange == QDeclarativeListView::StrictlyEnforceRange)
        d->updateHighlight();
}

QDeclarativeListViewAttached *QDeclarativeListView::qmlAttachedProperties(QObject *obj)
{
    return new QDeclarativeListViewAttached(obj);
}

QT_END_NAMESPACE
