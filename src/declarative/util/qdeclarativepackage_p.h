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

#ifndef QDECLARATIVEPACKAGE_H
#define QDECLARATIVEPACKAGE_H

#include <qdeclarative.h>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

QT_MODULE(Declarative)

class QDeclarativePackagePrivate;
class QDeclarativePackageAttached;
class Q_AUTOTEST_EXPORT QDeclarativePackage : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QDeclarativePackage)

    Q_CLASSINFO("DefaultProperty", "data")
    Q_PROPERTY(QDeclarativeListProperty<QObject> data READ data SCRIPTABLE false)

public:
    QDeclarativePackage(QObject *parent=0);
    virtual ~QDeclarativePackage();

    QDeclarativeListProperty<QObject> data();

    QObject *part(const QString & = QString());
    bool hasPart(const QString &);

    static QDeclarativePackageAttached *qmlAttachedProperties(QObject *);
};

class QDeclarativePackageAttached : public QObject
{
Q_OBJECT
Q_PROPERTY(QString name READ name WRITE setName)
public:
    QDeclarativePackageAttached(QObject *parent);
    virtual ~QDeclarativePackageAttached();

    QString name() const;
    void setName(const QString &n);

    static QHash<QObject *, QDeclarativePackageAttached *> attached;
private:
    QString _name;
};

QT_END_NAMESPACE

QML_DECLARE_TYPE(QDeclarativePackage)
QML_DECLARE_TYPEINFO(QDeclarativePackage, QML_HAS_ATTACHED_PROPERTIES)

QT_END_HEADER

#endif // QDECLARATIVEPACKAGE_H
