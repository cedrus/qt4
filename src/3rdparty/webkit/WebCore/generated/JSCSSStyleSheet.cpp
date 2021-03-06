/*
    This file is part of the WebKit open source project.
    This file has been generated by generate-bindings.pl. DO NOT MODIFY!

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#include "config.h"
#include "JSCSSStyleSheet.h"

#include "CSSRule.h"
#include "CSSRuleList.h"
#include "CSSStyleSheet.h"
#include "JSCSSRule.h"
#include "JSCSSRuleList.h"
#include <runtime/Error.h>
#include <runtime/JSNumberCell.h>
#include <wtf/GetPtr.h>

using namespace JSC;

namespace WebCore {

ASSERT_CLASS_FITS_IN_CELL(JSCSSStyleSheet);

/* Hash table */

static const HashTableValue JSCSSStyleSheetTableValues[5] =
{
    { "ownerRule", DontDelete|ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsCSSStyleSheetOwnerRule), (intptr_t)0 },
    { "cssRules", DontDelete|ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsCSSStyleSheetCssRules), (intptr_t)0 },
    { "rules", DontDelete|ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsCSSStyleSheetRules), (intptr_t)0 },
    { "constructor", DontEnum|ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsCSSStyleSheetConstructor), (intptr_t)0 },
    { 0, 0, 0, 0 }
};

static JSC_CONST_HASHTABLE HashTable JSCSSStyleSheetTable =
#if ENABLE(PERFECT_HASH_SIZE)
    { 31, JSCSSStyleSheetTableValues, 0 };
#else
    { 9, 7, JSCSSStyleSheetTableValues, 0 };
#endif

/* Hash table for constructor */

static const HashTableValue JSCSSStyleSheetConstructorTableValues[1] =
{
    { 0, 0, 0, 0 }
};

static JSC_CONST_HASHTABLE HashTable JSCSSStyleSheetConstructorTable =
#if ENABLE(PERFECT_HASH_SIZE)
    { 0, JSCSSStyleSheetConstructorTableValues, 0 };
#else
    { 1, 0, JSCSSStyleSheetConstructorTableValues, 0 };
#endif

class JSCSSStyleSheetConstructor : public DOMConstructorObject {
public:
    JSCSSStyleSheetConstructor(ExecState* exec, JSDOMGlobalObject* globalObject)
        : DOMConstructorObject(JSCSSStyleSheetConstructor::createStructure(globalObject->objectPrototype()), globalObject)
    {
        putDirect(exec->propertyNames().prototype, JSCSSStyleSheetPrototype::self(exec, globalObject), None);
    }
    virtual bool getOwnPropertySlot(ExecState*, const Identifier&, PropertySlot&);
    virtual bool getOwnPropertyDescriptor(ExecState*, const Identifier&, PropertyDescriptor&);
    virtual const ClassInfo* classInfo() const { return &s_info; }
    static const ClassInfo s_info;

    static PassRefPtr<Structure> createStructure(JSValue proto) 
    { 
        return Structure::create(proto, TypeInfo(ObjectType, StructureFlags), AnonymousSlotCount); 
    }
    
protected:
    static const unsigned StructureFlags = OverridesGetOwnPropertySlot | ImplementsHasInstance | DOMConstructorObject::StructureFlags;
};

const ClassInfo JSCSSStyleSheetConstructor::s_info = { "CSSStyleSheetConstructor", 0, &JSCSSStyleSheetConstructorTable, 0 };

bool JSCSSStyleSheetConstructor::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    return getStaticValueSlot<JSCSSStyleSheetConstructor, DOMObject>(exec, &JSCSSStyleSheetConstructorTable, this, propertyName, slot);
}

bool JSCSSStyleSheetConstructor::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& descriptor)
{
    return getStaticValueDescriptor<JSCSSStyleSheetConstructor, DOMObject>(exec, &JSCSSStyleSheetConstructorTable, this, propertyName, descriptor);
}

/* Hash table for prototype */

static const HashTableValue JSCSSStyleSheetPrototypeTableValues[5] =
{
    { "insertRule", DontDelete|Function, (intptr_t)static_cast<NativeFunction>(jsCSSStyleSheetPrototypeFunctionInsertRule), (intptr_t)2 },
    { "deleteRule", DontDelete|Function, (intptr_t)static_cast<NativeFunction>(jsCSSStyleSheetPrototypeFunctionDeleteRule), (intptr_t)1 },
    { "addRule", DontDelete|Function, (intptr_t)static_cast<NativeFunction>(jsCSSStyleSheetPrototypeFunctionAddRule), (intptr_t)3 },
    { "removeRule", DontDelete|Function, (intptr_t)static_cast<NativeFunction>(jsCSSStyleSheetPrototypeFunctionRemoveRule), (intptr_t)1 },
    { 0, 0, 0, 0 }
};

static JSC_CONST_HASHTABLE HashTable JSCSSStyleSheetPrototypeTable =
#if ENABLE(PERFECT_HASH_SIZE)
    { 7, JSCSSStyleSheetPrototypeTableValues, 0 };
#else
    { 8, 7, JSCSSStyleSheetPrototypeTableValues, 0 };
#endif

const ClassInfo JSCSSStyleSheetPrototype::s_info = { "CSSStyleSheetPrototype", 0, &JSCSSStyleSheetPrototypeTable, 0 };

JSObject* JSCSSStyleSheetPrototype::self(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMPrototype<JSCSSStyleSheet>(exec, globalObject);
}

bool JSCSSStyleSheetPrototype::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    return getStaticFunctionSlot<JSObject>(exec, &JSCSSStyleSheetPrototypeTable, this, propertyName, slot);
}

bool JSCSSStyleSheetPrototype::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& descriptor)
{
    return getStaticFunctionDescriptor<JSObject>(exec, &JSCSSStyleSheetPrototypeTable, this, propertyName, descriptor);
}

const ClassInfo JSCSSStyleSheet::s_info = { "CSSStyleSheet", &JSStyleSheet::s_info, &JSCSSStyleSheetTable, 0 };

JSCSSStyleSheet::JSCSSStyleSheet(NonNullPassRefPtr<Structure> structure, JSDOMGlobalObject* globalObject, PassRefPtr<CSSStyleSheet> impl)
    : JSStyleSheet(structure, globalObject, impl)
{
}

JSObject* JSCSSStyleSheet::createPrototype(ExecState* exec, JSGlobalObject* globalObject)
{
    return new (exec) JSCSSStyleSheetPrototype(JSCSSStyleSheetPrototype::createStructure(JSStyleSheetPrototype::self(exec, globalObject)));
}

bool JSCSSStyleSheet::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    return getStaticValueSlot<JSCSSStyleSheet, Base>(exec, &JSCSSStyleSheetTable, this, propertyName, slot);
}

bool JSCSSStyleSheet::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& descriptor)
{
    return getStaticValueDescriptor<JSCSSStyleSheet, Base>(exec, &JSCSSStyleSheetTable, this, propertyName, descriptor);
}

JSValue jsCSSStyleSheetOwnerRule(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSCSSStyleSheet* castedThis = static_cast<JSCSSStyleSheet*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    CSSStyleSheet* imp = static_cast<CSSStyleSheet*>(castedThis->impl());
    JSValue result = toJS(exec, castedThis->globalObject(), WTF::getPtr(imp->ownerRule()));
    return result;
}

JSValue jsCSSStyleSheetCssRules(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSCSSStyleSheet* castedThis = static_cast<JSCSSStyleSheet*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    CSSStyleSheet* imp = static_cast<CSSStyleSheet*>(castedThis->impl());
    JSValue result = toJS(exec, castedThis->globalObject(), WTF::getPtr(imp->cssRules()));
    return result;
}

JSValue jsCSSStyleSheetRules(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSCSSStyleSheet* castedThis = static_cast<JSCSSStyleSheet*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    CSSStyleSheet* imp = static_cast<CSSStyleSheet*>(castedThis->impl());
    JSValue result = toJS(exec, castedThis->globalObject(), WTF::getPtr(imp->rules()));
    return result;
}

JSValue jsCSSStyleSheetConstructor(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSCSSStyleSheet* domObject = static_cast<JSCSSStyleSheet*>(asObject(slotBase));
    return JSCSSStyleSheet::getConstructor(exec, domObject->globalObject());
}
JSValue JSCSSStyleSheet::getConstructor(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMConstructor<JSCSSStyleSheetConstructor>(exec, static_cast<JSDOMGlobalObject*>(globalObject));
}

JSValue JSC_HOST_CALL jsCSSStyleSheetPrototypeFunctionInsertRule(ExecState* exec, JSObject*, JSValue thisValue, const ArgList& args)
{
    UNUSED_PARAM(args);
    if (!thisValue.inherits(&JSCSSStyleSheet::s_info))
        return throwError(exec, TypeError);
    JSCSSStyleSheet* castedThisObj = static_cast<JSCSSStyleSheet*>(asObject(thisValue));
    CSSStyleSheet* imp = static_cast<CSSStyleSheet*>(castedThisObj->impl());
    ExceptionCode ec = 0;
    const UString& rule = args.at(0).toString(exec);
    unsigned index = args.at(1).toInt32(exec);


    JSC::JSValue result = jsNumber(exec, imp->insertRule(rule, index, ec));
    setDOMException(exec, ec);
    return result;
}

JSValue JSC_HOST_CALL jsCSSStyleSheetPrototypeFunctionDeleteRule(ExecState* exec, JSObject*, JSValue thisValue, const ArgList& args)
{
    UNUSED_PARAM(args);
    if (!thisValue.inherits(&JSCSSStyleSheet::s_info))
        return throwError(exec, TypeError);
    JSCSSStyleSheet* castedThisObj = static_cast<JSCSSStyleSheet*>(asObject(thisValue));
    CSSStyleSheet* imp = static_cast<CSSStyleSheet*>(castedThisObj->impl());
    ExceptionCode ec = 0;
    unsigned index = args.at(0).toInt32(exec);

    imp->deleteRule(index, ec);
    setDOMException(exec, ec);
    return jsUndefined();
}

JSValue JSC_HOST_CALL jsCSSStyleSheetPrototypeFunctionAddRule(ExecState* exec, JSObject*, JSValue thisValue, const ArgList& args)
{
    UNUSED_PARAM(args);
    if (!thisValue.inherits(&JSCSSStyleSheet::s_info))
        return throwError(exec, TypeError);
    JSCSSStyleSheet* castedThisObj = static_cast<JSCSSStyleSheet*>(asObject(thisValue));
    CSSStyleSheet* imp = static_cast<CSSStyleSheet*>(castedThisObj->impl());
    ExceptionCode ec = 0;
    const UString& selector = args.at(0).toString(exec);
    const UString& style = args.at(1).toString(exec);

    int argsCount = args.size();
    if (argsCount < 3) {

        JSC::JSValue result = jsNumber(exec, imp->addRule(selector, style, ec));
        setDOMException(exec, ec);
        return result;
    }

    unsigned index = args.at(2).toInt32(exec);


    JSC::JSValue result = jsNumber(exec, imp->addRule(selector, style, index, ec));
    setDOMException(exec, ec);
    return result;
}

JSValue JSC_HOST_CALL jsCSSStyleSheetPrototypeFunctionRemoveRule(ExecState* exec, JSObject*, JSValue thisValue, const ArgList& args)
{
    UNUSED_PARAM(args);
    if (!thisValue.inherits(&JSCSSStyleSheet::s_info))
        return throwError(exec, TypeError);
    JSCSSStyleSheet* castedThisObj = static_cast<JSCSSStyleSheet*>(asObject(thisValue));
    CSSStyleSheet* imp = static_cast<CSSStyleSheet*>(castedThisObj->impl());
    ExceptionCode ec = 0;
    unsigned index = args.at(0).toInt32(exec);

    imp->removeRule(index, ec);
    setDOMException(exec, ec);
    return jsUndefined();
}


}
