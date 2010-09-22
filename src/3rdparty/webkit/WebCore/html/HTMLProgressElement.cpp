/*
 * Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 */

#include "config.h"
#if ENABLE(PROGRESS_TAG)
#include "HTMLProgressElement.h"

#include "EventNames.h"
#include "FormDataList.h"
#include "HTMLFormElement.h"
#include "HTMLNames.h"
#include "MappedAttribute.h"
#include "RenderProgress.h"
#include <wtf/StdLibExtras.h>

namespace WebCore {

using namespace HTMLNames;

HTMLProgressElement::HTMLProgressElement(const QualifiedName& tagName, Document* document, HTMLFormElement* form)
    : HTMLFormControlElement(tagName, document, form, CreateElement)
{
    ASSERT(hasTagName(progressTag));
}

PassRefPtr<HTMLProgressElement> HTMLProgressElement::create(const QualifiedName& tagName, Document* document, HTMLFormElement* form)
{
    return adoptRef(new HTMLProgressElement(tagName, document, form));
}

RenderObject* HTMLProgressElement::createRenderer(RenderArena* arena, RenderStyle* style)
{
    return new (arena) RenderProgress(this);
}

const AtomicString& HTMLProgressElement::formControlType() const
{
    DEFINE_STATIC_LOCAL(const AtomicString, progress, ("progress"));
    return progress;
}

void HTMLProgressElement::parseMappedAttribute(MappedAttribute* attribute)
{
    if (attribute->name() == valueAttr) {
        if (renderer())
            renderer()->updateFromElement();
    } else if (attribute->name() == maxAttr) {
        if (renderer())
            renderer()->updateFromElement();
    } else
        HTMLFormControlElement::parseMappedAttribute(attribute);
}

double HTMLProgressElement::value() const
{
    const AtomicString& valueString = getAttribute(valueAttr);
    bool ok;
    double value = valueString.toDouble(&ok);
    if (!ok || value < 0)
        return valueString.isNull() ? 1 : 0;
    return (value > max()) ? max() : value;
}

void HTMLProgressElement::setValue(double value)
{
    setAttribute(valueAttr, String::number(value >= 0 ? value : 0));
}

double HTMLProgressElement::max() const
{
    bool ok;
    double max = getAttribute(maxAttr).toDouble(&ok);
    if (!ok || max <= 0)
        return 1;
    return max;
}

void HTMLProgressElement::setMax(double max)
{
    setAttribute(maxAttr, String::number(max > 0 ? max : 1));
}

double HTMLProgressElement::position() const
{
    if (!hasAttribute(valueAttr))
        return -1;
    return value() / max();
}

} // namespace
#endif
