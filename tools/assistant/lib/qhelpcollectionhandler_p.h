/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Assistant of the Qt Toolkit.
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

#ifndef QHELPCOLLECTIONHANDLER_H
#define QHELPCOLLECTIONHANDLER_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API. It exists for the convenience
// of the help generator tools. This header file may change from version
// to version without notice, or even be removed.
//
// We mean it.
//

#include <QtCore/QList>
#include <QtCore/QString>
#include <QtCore/QObject>
#include <QtCore/QVariant>
#include <QtCore/QStringList>

#include <QtSql/QSqlQuery>

QT_BEGIN_NAMESPACE

class QHelpCollectionHandler : public QObject
{
    Q_OBJECT

public:
    struct DocInfo
    {
        QString fileName;
        QString folderName;
        QString namespaceName;
    };
    typedef QList<DocInfo> DocInfoList;

    explicit QHelpCollectionHandler(const QString &collectionFile,
        QObject *parent = 0);
    ~QHelpCollectionHandler();

    QString collectionFile() const;

    bool openCollectionFile();
    bool copyCollectionFile(const QString &fileName);

    QStringList customFilters() const;
    bool removeCustomFilter(const QString &filterName);
    bool addCustomFilter(const QString &filterName,
        const QStringList &attributes);

    DocInfoList registeredDocumentations() const;
    bool registerDocumentation(const QString &fileName);
    bool unregisterDocumentation(const QString &namespaceName);

    bool removeCustomValue(const QString &key);
    QVariant customValue(const QString &key, const QVariant &defaultValue) const;
    bool setCustomValue(const QString &key, const QVariant &value);

    bool addFilterAttributes(const QStringList &attributes);
    QStringList filterAttributes() const;
    QStringList filterAttributes(const QString &filterName) const;
    
    int registerNamespace(const QString &nspace, const QString &fileName);
    bool registerVirtualFolder(const QString &folderName, int namespaceId);
    void optimizeDatabase(const QString &fileName);

signals:
    void error(const QString &msg);

private:
    bool isDBOpened();
    bool createTables(QSqlQuery *query);

    bool m_dbOpened;
    QString m_collectionFile;
    QString m_connectionName;
    mutable QSqlQuery m_query;
};

QT_END_NAMESPACE

#endif  //QHELPCOLLECTIONHANDLER_H
