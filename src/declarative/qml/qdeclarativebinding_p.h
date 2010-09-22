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

#ifndef QDECLARATIVEBINDING_P_H
#define QDECLARATIVEBINDING_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "qdeclarative.h"
#include "qdeclarativepropertyvaluesource.h"
#include "qdeclarativeexpression.h"
#include "qdeclarativeproperty.h"
#include "private/qdeclarativeproperty_p.h"

#include <QtCore/QObject>
#include <QtCore/QMetaProperty>

QT_BEGIN_NAMESPACE

class Q_DECLARATIVE_EXPORT QDeclarativeAbstractBinding
{
public:
    QDeclarativeAbstractBinding();

    virtual void destroy();

    virtual QString expression() const;

    enum Type { PropertyBinding, ValueTypeProxy };
    virtual Type bindingType() const { return PropertyBinding; }

    void setEnabled(bool e) { setEnabled(e, QDeclarativePropertyPrivate::DontRemoveBinding); }
    virtual void setEnabled(bool, QDeclarativePropertyPrivate::WriteFlags) = 0;
    virtual int propertyIndex() = 0;

    void update() { update(QDeclarativePropertyPrivate::DontRemoveBinding); }
    virtual void update(QDeclarativePropertyPrivate::WriteFlags) = 0;

    void addToObject(QObject *);
    void removeFromObject();

protected:
    virtual ~QDeclarativeAbstractBinding();
    void clear();

private:

    friend class QDeclarativeData;
    friend class QDeclarativeValueTypeProxyBinding;
    friend class QDeclarativePropertyPrivate;
    friend class QDeclarativeVME;

    QObject *m_object;
    QDeclarativeAbstractBinding **m_mePtr;
    QDeclarativeAbstractBinding **m_prevBinding;
    QDeclarativeAbstractBinding  *m_nextBinding;
};

class QDeclarativeValueTypeProxyBinding : public QDeclarativeAbstractBinding
{
public:
    QDeclarativeValueTypeProxyBinding(QObject *o, int coreIndex);

    virtual Type bindingType() const { return ValueTypeProxy; }

    virtual void setEnabled(bool, QDeclarativePropertyPrivate::WriteFlags);
    virtual int propertyIndex();
    virtual void update(QDeclarativePropertyPrivate::WriteFlags);

    QDeclarativeAbstractBinding *binding(int propertyIndex);

protected:
    ~QDeclarativeValueTypeProxyBinding();

private:
    void recursiveEnable(QDeclarativeAbstractBinding *, QDeclarativePropertyPrivate::WriteFlags);
    void recursiveDisable(QDeclarativeAbstractBinding *);

    friend class QDeclarativeAbstractBinding;
    QObject *m_object;
    int m_index;
    QDeclarativeAbstractBinding *m_bindings;
};

class QDeclarativeContext;
class QDeclarativeBindingPrivate;
class Q_DECLARATIVE_EXPORT QDeclarativeBinding : public QDeclarativeExpression, public QDeclarativeAbstractBinding
{
Q_OBJECT
public:
    QDeclarativeBinding(const QString &, QObject *, QDeclarativeContext *, QObject *parent=0);
    QDeclarativeBinding(const QString &, QObject *, QDeclarativeContextData *, QObject *parent=0);
    QDeclarativeBinding(void *, QDeclarativeRefCount *, QObject *, QDeclarativeContextData *, 
                        const QString &, int, QObject *parent);

    void setTarget(const QDeclarativeProperty &);
    QDeclarativeProperty property() const;

    bool enabled() const;

    // Inherited from  QDeclarativeAbstractBinding
    virtual void setEnabled(bool, QDeclarativePropertyPrivate::WriteFlags flags);
    virtual int propertyIndex();
    virtual void update(QDeclarativePropertyPrivate::WriteFlags flags);
    virtual QString expression() const;

public Q_SLOTS:
    void update() { update(QDeclarativePropertyPrivate::DontRemoveBinding); }

protected:
    ~QDeclarativeBinding();
    void emitValueChanged();

private:
    Q_DECLARE_PRIVATE(QDeclarativeBinding)
};

QT_END_NAMESPACE

Q_DECLARE_METATYPE(QDeclarativeBinding*)

#endif // QDECLARATIVEBINDING_P_H
