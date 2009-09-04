/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the plugins of the Qt Toolkit.
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
** In addition, as a special exception, Nokia gives you certain
** additional rights.  These rights are described in the Nokia Qt LGPL
** Exception version 1.1, included in the file LGPL_EXCEPTION.txt in this
** package.
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

#include <qmousedriverplugin_qws.h>
#include <qmousevr41xx_qws.h>

QT_BEGIN_NAMESPACE

class QVr41xxMouseDriver : public QMouseDriverPlugin
{
public:
    QVr41xxMouseDriver();

    QStringList keys() const;
    QWSMouseHandler* create(const QString &driver, const QString &device);
};

QVr41xxMouseDriver::QVr41xxMouseDriver()
    : QMouseDriverPlugin()
{
}

QStringList QVr41xxMouseDriver::keys() const
{
    return (QStringList() << QLatin1String("VR41xx"));
}

QWSMouseHandler* QVr41xxMouseDriver::create(const QString &driver,
                                            const QString &device)
{
    if (driver.compare(QLatin1String("VR41xx"), Qt::CaseInsensitive))
        return 0;
    return new QWSVr41xxMouseHandler(driver, device);
}

Q_EXPORT_PLUGIN2(qwsvr41xxmousehandler, QVr41xxMouseDriver)

QT_END_NAMESPACE