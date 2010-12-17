/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Designer of the Qt Toolkit.
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

#include "seeksliderplugin.h"

#include <phonon/seekslider.h>

static const char *toolTipC = "Phonon Seek Slider";

QT_BEGIN_NAMESPACE

SeekSliderPlugin::SeekSliderPlugin(const QString &group, QObject *parent) :
    QObject(parent),
    m_group(group),
    m_initialized(false)
{
}

QString SeekSliderPlugin::name() const
{
    return QLatin1String("Phonon::SeekSlider");
}

QString SeekSliderPlugin::group() const
{
    return m_group;
}

QString SeekSliderPlugin::toolTip() const
{
    return tr(toolTipC);
}

QString SeekSliderPlugin::whatsThis() const
{
    return tr(toolTipC);
}

QString SeekSliderPlugin::includeFile() const
{
    return QLatin1String("<phonon/seekslider.h>");
}

QIcon SeekSliderPlugin::icon() const
{
    return QIcon(QLatin1String(":/trolltech/phononwidgets/images/seekslider.png"));
}

bool SeekSliderPlugin::isContainer() const
{
    return false;
}

QWidget *SeekSliderPlugin::createWidget(QWidget *parent)
{
    return new Phonon::SeekSlider(parent);
}

bool SeekSliderPlugin::isInitialized() const
{
    return m_initialized;
}

void SeekSliderPlugin::initialize(QDesignerFormEditorInterface *)
{
    if (m_initialized)
        return;
    m_initialized = true;
}

QString SeekSliderPlugin::domXml() const
{
    return QLatin1String("\
    <ui language=\"c++\">\
        <widget class=\"Phonon::SeekSlider\" name=\"seekSlider\"/>\
    </ui>");
}

QT_END_NAMESPACE
