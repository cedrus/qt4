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

#include "private/rewriter_p.h"

#include <qdeclarativejsast_p.h>

QT_QML_BEGIN_NAMESPACE

using namespace QDeclarativeJS;

void Rewriter::replace(const AST::SourceLocation &loc, const QString &text)
{ replace(loc.offset, loc.length, text); }

void Rewriter::remove(const AST::SourceLocation &loc)
{ return replace(loc.offset, loc.length, QString()); }

void Rewriter::remove(const AST::SourceLocation &firstLoc, const AST::SourceLocation &lastLoc)
{ return replace(firstLoc.offset, lastLoc.offset + lastLoc.length - firstLoc.offset, QString()); }

void Rewriter::insertTextBefore(const AST::SourceLocation &loc, const QString &text)
{ replace(loc.offset, 0, text); }

void Rewriter::insertTextAfter(const AST::SourceLocation &loc, const QString &text)
{ replace(loc.offset + loc.length, 0, text); }

void Rewriter::replace(int offset, int length, const QString &text)
{ textWriter.replace(offset, length, text); }

void Rewriter::insertText(int offset, const QString &text)
{ replace(offset, 0, text); }

void Rewriter::removeText(int offset, int length)
{ replace(offset, length, QString()); }

QString Rewriter::textAt(const AST::SourceLocation &loc) const
{ return _code.mid(loc.offset, loc.length); }

QString Rewriter::textAt(const AST::SourceLocation &firstLoc, const AST::SourceLocation &lastLoc) const
{ return _code.mid(firstLoc.offset, lastLoc.offset + lastLoc.length - firstLoc.offset); }

void Rewriter::accept(QDeclarativeJS::AST::Node *node)
{ QDeclarativeJS::AST::Node::acceptChild(node, this); }

void Rewriter::moveTextBefore(const AST::SourceLocation &firstLoc,
                              const AST::SourceLocation &lastLoc,
                              const AST::SourceLocation &loc)
{
    move(firstLoc.offset, lastLoc.offset + lastLoc.length - firstLoc.offset, loc.offset);
}

void Rewriter::moveTextAfter(const AST::SourceLocation &firstLoc,
                             const AST::SourceLocation &lastLoc,
                             const AST::SourceLocation &loc)
{
    move(firstLoc.offset, lastLoc.offset + lastLoc.length - firstLoc.offset, loc.offset + loc.length);
}

void Rewriter::move(int pos, int length, int to)
{
    textWriter.move(pos, length, to);
}

QT_QML_END_NAMESPACE
