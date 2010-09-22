/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the test suite of the Qt Toolkit.
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


#include <QtTest/QtTest>

#include <QtScript/qscriptengine.h>
#include <QtScript/qscriptclass.h>
#include <QtScript/qscriptclasspropertyiterator.h>
#include <QtScript/qscriptstring.h>
#include <QtScript/qscriptvalueiterator.h>

Q_DECLARE_METATYPE(QScriptContext*)
Q_DECLARE_METATYPE(QScriptValueList)
Q_DECLARE_METATYPE(QScriptValue)

//TESTED_CLASS=
//TESTED_FILES=script/qscriptclass.h script/qscriptclass.cpp script/qscriptclasspropertyiterator.h script/qscriptclasspropertyiterator.cpp

class tst_QScriptClass : public QObject
{
    Q_OBJECT

public:
    tst_QScriptClass();
    virtual ~tst_QScriptClass();

private slots:
    void newInstance();
    void getAndSetProperty();
    void enumerate();
    void extension();
};

tst_QScriptClass::tst_QScriptClass()
{
}

tst_QScriptClass::~tst_QScriptClass()
{
}



class TestClass : public QScriptClass
{
public:
    struct CustomProperty {
        QueryFlags qflags;
        uint id;
        QScriptValue::PropertyFlags pflags;
        QScriptValue value;

        CustomProperty(QueryFlags qf, uint i, QScriptValue::PropertyFlags pf,
                       const QScriptValue &val)
            : qflags(qf), id(i), pflags(pf), value(val) { }
    };

    enum CallableMode {
        NotCallable,
        CallableReturnsSum,
        CallableReturnsArgument,
        CallableReturnsInvalidVariant,
        CallableReturnsGlobalObject,
        CallableReturnsThisObject,
        CallableReturnsCallee,
        CallableReturnsArgumentsObject,
        CallableInitializesThisObject
    };

    TestClass(QScriptEngine *engine);
    ~TestClass();

    void addCustomProperty(const QScriptString &name, QueryFlags qflags,
                           uint id, QScriptValue::PropertyFlags pflags,
                           const QScriptValue &value);
    void removeCustomProperty(const QScriptString &name);

    QueryFlags queryProperty(const QScriptValue &object,
                             const QScriptString &name,
                             QueryFlags flags, uint *id);

    QScriptValue property(const QScriptValue &object,
                          const QScriptString &name, uint id);

    void setProperty(QScriptValue &object, const QScriptString &name,
                     uint id, const QScriptValue &value);

    QScriptValue::PropertyFlags propertyFlags(
        const QScriptValue &object, const QScriptString &name, uint id);

    QScriptClassPropertyIterator *newIterator(const QScriptValue &object);

    QScriptValue prototype() const;

    QString name() const;

    bool supportsExtension(Extension extension) const;
    QVariant extension(Extension extension,
                       const QVariant &argument = QVariant());

    QScriptValue lastQueryPropertyObject() const;
    QScriptString lastQueryPropertyName() const;
    QueryFlags lastQueryPropertyFlags() const;

    QScriptValue lastPropertyObject() const;
    QScriptString lastPropertyName() const;
    uint lastPropertyId() const;

    QScriptValue lastSetPropertyObject() const;
    QScriptString lastSetPropertyName() const;
    uint lastSetPropertyId() const;
    QScriptValue lastSetPropertyValue() const;

    QScriptValue lastPropertyFlagsObject() const;
    QScriptString lastPropertyFlagsName() const;
    uint lastPropertyFlagsId() const;

    QScriptClass::Extension lastExtensionType() const;
    QVariant lastExtensionArgument() const;

    void clearReceivedArgs();

    void setIterationEnabled(bool enable);
    bool isIterationEnabled() const;

    void setCallableMode(CallableMode mode);
    CallableMode callableMode() const;

    void setHasInstance(bool hasInstance);
    bool hasInstance() const;

private:
    CustomProperty *findCustomProperty(const QScriptString &name);

    QHash<QScriptString, CustomProperty*> customProperties;

    QScriptValue m_lastQueryPropertyObject;
    QScriptString m_lastQueryPropertyName;
    QScriptClass::QueryFlags m_lastQueryPropertyFlags;

    QScriptValue m_lastPropertyObject;
    QScriptString m_lastPropertyName;
    uint m_lastPropertyId;

    QScriptValue m_lastSetPropertyObject;
    QScriptString m_lastSetPropertyName;
    uint m_lastSetPropertyId;
    QScriptValue m_lastSetPropertyValue;

    QScriptValue m_lastPropertyFlagsObject;
    QScriptString m_lastPropertyFlagsName;
    uint m_lastPropertyFlagsId;

    QScriptClass::Extension m_lastExtensionType;
    QVariant m_lastExtensionArgument;

    QScriptValue m_prototype;
    bool m_iterationEnabled;
    CallableMode m_callableMode;
    bool m_hasInstance;
};

class TestClassPropertyIterator : public QScriptClassPropertyIterator
{
public:
    TestClassPropertyIterator(const QHash<QScriptString, TestClass::CustomProperty*> &props,
                      const QScriptValue &object);
    ~TestClassPropertyIterator();

    bool hasNext() const;
    void next();

    bool hasPrevious() const;
    void previous();

    void toFront();
    void toBack();

    QScriptString name() const;
    uint id() const;
    QScriptValue::PropertyFlags flags() const;

private:
    int m_index;
    int m_last;
    QHash<QScriptString, TestClass::CustomProperty*> m_props;
};



TestClass::TestClass(QScriptEngine *engine)
    : QScriptClass(engine), m_iterationEnabled(true),
      m_callableMode(NotCallable), m_hasInstance(false)
{
    m_prototype = engine->newObject();
    clearReceivedArgs();
}

TestClass::~TestClass()
{
    qDeleteAll(customProperties);
}

TestClass::CustomProperty* TestClass::findCustomProperty(const QScriptString &name)
{
    QHash<QScriptString, CustomProperty*>::const_iterator it;
    it = customProperties.constFind(name);
    if (it == customProperties.constEnd())
        return 0;
    return it.value();

}

void TestClass::addCustomProperty(const QScriptString &name, QueryFlags qflags,
                                  uint id, QScriptValue::PropertyFlags pflags,
                                  const QScriptValue &value)
{
    customProperties.insert(name, new CustomProperty(qflags, id, pflags, value));
}

void TestClass::removeCustomProperty(const QScriptString &name)
{
    CustomProperty *prop = customProperties.take(name);
    if (prop)
        delete prop;
}

QScriptClass::QueryFlags TestClass::queryProperty(const QScriptValue &object,
                                    const QScriptString &name,
                                    QueryFlags flags, uint *id)
{
    m_lastQueryPropertyObject = object;
    m_lastQueryPropertyName = name;
    m_lastQueryPropertyFlags = flags;
    CustomProperty *prop = findCustomProperty(name);
    if (!prop)
        return 0;
    *id = prop->id;
    return prop->qflags & flags;
}

QScriptValue TestClass::property(const QScriptValue &object,
                                 const QScriptString &name, uint id)
{
    m_lastPropertyObject = object;
    m_lastPropertyName = name;
    m_lastPropertyId = id;
    CustomProperty *prop = findCustomProperty(name);
    if (!prop)
        return QScriptValue();
    return prop->value;
}

void TestClass::setProperty(QScriptValue &object, const QScriptString &name,
                            uint id, const QScriptValue &value)
{
    m_lastSetPropertyObject = object;
    m_lastSetPropertyName = name;
    m_lastSetPropertyId = id;
    m_lastSetPropertyValue = value;
    CustomProperty *prop = findCustomProperty(name);
    if (!prop)
        return;
    prop->value = value;
}

QScriptValue::PropertyFlags TestClass::propertyFlags(
    const QScriptValue &object, const QScriptString &name, uint id)
{
    m_lastPropertyFlagsObject = object;
    m_lastPropertyFlagsName = name;
    m_lastPropertyFlagsId = id;
    CustomProperty *prop = findCustomProperty(name);
    if (!prop)
        return 0;
    return prop->pflags;
}

QScriptClassPropertyIterator *TestClass::newIterator(const QScriptValue &object)
{
    if (!m_iterationEnabled)
        return 0;
    return new TestClassPropertyIterator(customProperties, object);
}

QScriptValue TestClass::prototype() const
{
    return m_prototype;
}

QString TestClass::name() const
{
    return QLatin1String("TestClass");
}

bool TestClass::supportsExtension(Extension extension) const
{
    if (extension == Callable)
        return (m_callableMode != NotCallable);
    if (extension == HasInstance)
        return m_hasInstance;
    return false;
}

QVariant TestClass::extension(Extension extension,
                              const QVariant &argument)
{
    m_lastExtensionType = extension;
    m_lastExtensionArgument = argument;
    if (extension == Callable) {
        Q_ASSERT(m_callableMode != NotCallable);
        QScriptContext *ctx = qvariant_cast<QScriptContext*>(argument);
        if (m_callableMode == CallableReturnsSum) {
            qsreal sum = 0;
            for (int i = 0; i < ctx->argumentCount(); ++i)
                sum += ctx->argument(i).toNumber();
            QScriptValueIterator it(ctx->callee());
            while (it.hasNext()) {
                it.next();
                sum += it.value().toNumber();
            }
            return sum;
        } else if (m_callableMode == CallableReturnsArgument) {
            return qVariantFromValue(ctx->argument(0));
        } else if (m_callableMode == CallableReturnsInvalidVariant) {
            return QVariant();
        } else if (m_callableMode == CallableReturnsGlobalObject) {
            return qVariantFromValue(engine()->globalObject());
        } else if (m_callableMode == CallableReturnsThisObject) {
            return qVariantFromValue(ctx->thisObject());
        } else if (m_callableMode == CallableReturnsCallee) {
            return qVariantFromValue(ctx->callee());
        } else if (m_callableMode == CallableReturnsArgumentsObject) {
            return qVariantFromValue(ctx->argumentsObject());
        } else if (m_callableMode == CallableInitializesThisObject) {
            engine()->newQObject(ctx->thisObject(), engine());
            return QVariant();
        }
    } else if (extension == HasInstance) {
        Q_ASSERT(m_hasInstance);
        QScriptValueList args = qvariant_cast<QScriptValueList>(argument);
        QScriptValue obj = args.at(0);
        QScriptValue value = args.at(1);
        return value.property("foo").equals(obj.property("foo"));
    }
    return QVariant();
}

QScriptValue TestClass::lastQueryPropertyObject() const
{
    return m_lastQueryPropertyObject;
}

QScriptString TestClass::lastQueryPropertyName() const
{
    return m_lastQueryPropertyName;
}

QScriptClass::QueryFlags TestClass::lastQueryPropertyFlags() const
{
    return m_lastQueryPropertyFlags;
}

QScriptValue TestClass::lastPropertyObject() const
{
    return m_lastPropertyObject;
}

QScriptString TestClass::lastPropertyName() const
{
    return m_lastPropertyName;
}

uint TestClass::lastPropertyId() const
{
    return m_lastPropertyId;
}

QScriptValue TestClass::lastSetPropertyObject() const
{
    return m_lastSetPropertyObject;
}

QScriptString TestClass::lastSetPropertyName() const
{
    return m_lastSetPropertyName;
}

uint TestClass::lastSetPropertyId() const
{
    return m_lastSetPropertyId;
}

QScriptValue TestClass::lastSetPropertyValue() const
{
    return m_lastSetPropertyValue;
}

QScriptValue TestClass::lastPropertyFlagsObject() const
{
    return m_lastPropertyFlagsObject;
}

QScriptString TestClass::lastPropertyFlagsName() const
{
    return m_lastPropertyFlagsName;
}

uint TestClass::lastPropertyFlagsId() const
{
    return m_lastPropertyFlagsId;
}

QScriptClass::Extension TestClass::lastExtensionType() const
{
    return m_lastExtensionType;
}

QVariant TestClass::lastExtensionArgument() const
{
    return m_lastExtensionArgument;
}

void TestClass::clearReceivedArgs()
{
    m_lastQueryPropertyObject = QScriptValue();
    m_lastQueryPropertyName = QScriptString();
    m_lastQueryPropertyFlags = 0;

    m_lastPropertyObject = QScriptValue();
    m_lastPropertyName = QScriptString();
    m_lastPropertyId = uint(-1);

    m_lastSetPropertyObject = QScriptValue();
    m_lastSetPropertyName = QScriptString();
    m_lastSetPropertyId = uint(-1);
    m_lastSetPropertyValue = QScriptValue();

    m_lastPropertyFlagsObject = QScriptValue();
    m_lastPropertyFlagsName = QScriptString();
    m_lastPropertyFlagsId = uint(-1);

    m_lastExtensionType = static_cast<QScriptClass::Extension>(-1);
    m_lastExtensionArgument = QVariant();
}

void TestClass::setIterationEnabled(bool enable)
{
    m_iterationEnabled = enable;
}

bool TestClass::isIterationEnabled() const
{
    return m_iterationEnabled;
}

void TestClass::setCallableMode(CallableMode mode)
{
    m_callableMode = mode;
}

TestClass::CallableMode TestClass::callableMode() const
{
    return m_callableMode;
}

void TestClass::setHasInstance(bool hasInstance)
{
    m_hasInstance = hasInstance;
}

bool TestClass::hasInstance() const
{
    return m_hasInstance;
}


TestClassPropertyIterator::TestClassPropertyIterator(const QHash<QScriptString, TestClass::CustomProperty*> &props,
                                     const QScriptValue &object)
    : QScriptClassPropertyIterator(object)
{
    m_props = props;
    toFront();
}

TestClassPropertyIterator::~TestClassPropertyIterator()
{
}

bool TestClassPropertyIterator::hasNext() const
{
    return m_index < m_props.size();
}

void TestClassPropertyIterator::next()
{
    m_last = m_index;
    ++m_index;
}

bool TestClassPropertyIterator::hasPrevious() const
{
    return m_index > 0;
}

void TestClassPropertyIterator::previous()
{
    --m_index;
    m_last = m_index;
}

void TestClassPropertyIterator::toFront()
{
    m_index = 0;
    m_last = -1;
}

void TestClassPropertyIterator::toBack()
{
    m_index = m_props.size();
    m_last = -1;
}

QScriptString TestClassPropertyIterator::name() const
{
    return m_props.keys().value(m_last);
}

uint TestClassPropertyIterator::id() const
{
    QScriptString key = m_props.keys().value(m_last);
    if (!key.isValid())
        return 0;
    TestClass::CustomProperty *prop = m_props.value(key);
    return prop->id;
}

QScriptValue::PropertyFlags TestClassPropertyIterator::flags() const
{
    QScriptString key = m_props.keys().value(m_last);
    if (!key.isValid())
        return 0;
    TestClass::CustomProperty *prop = m_props.value(key);
    return prop->pflags;
}



void tst_QScriptClass::newInstance()
{
    QScriptEngine eng;

    TestClass cls(&eng);

    QScriptValue obj1 = eng.newObject(&cls);
    QVERIFY(!obj1.data().isValid());
    QVERIFY(obj1.prototype().strictlyEquals(cls.prototype()));
    QEXPECT_FAIL("", "classname is not implemented", Continue);
    QCOMPARE(obj1.toString(), QString::fromLatin1("[object TestClass]"));
    QCOMPARE(obj1.scriptClass(), (QScriptClass*)&cls);

    QScriptValue num(&eng, 456);
    QScriptValue obj2 = eng.newObject(&cls, num);
    QVERIFY(obj2.data().strictlyEquals(num));
    QVERIFY(obj2.prototype().strictlyEquals(cls.prototype()));
    QCOMPARE(obj2.scriptClass(), (QScriptClass*)&cls);

    QScriptValue obj3 = eng.newObject();
    QCOMPARE(obj3.scriptClass(), (QScriptClass*)0);
    obj3.setScriptClass(&cls);
    QCOMPARE(obj3.scriptClass(), (QScriptClass*)&cls);

    obj3.setScriptClass(0);
    QCOMPARE(obj3.scriptClass(), (QScriptClass*)0);
    obj3.setScriptClass(&cls);
    QCOMPARE(obj3.scriptClass(), (QScriptClass*)&cls);

    TestClass cls2(&eng);
    obj3.setScriptClass(&cls2);
    QCOMPARE(obj3.scriptClass(), (QScriptClass*)&cls2);

    // undefined behavior really, but shouldn't crash
    QScriptValue arr = eng.newArray();
    QVERIFY(arr.isArray());
    QCOMPARE(arr.scriptClass(), (QScriptClass*)0);
    QTest::ignoreMessage(QtWarningMsg, "QScriptValue::setScriptClass() failed: cannot change class of non-QScriptObject");
    arr.setScriptClass(&cls);
    QEXPECT_FAIL("", "Changing class of arbitrary script object is not allowed (it's OK)", Continue);
    QCOMPARE(arr.scriptClass(), (QScriptClass*)&cls);
    QEXPECT_FAIL("", "Changing class of arbitrary script object is not allowed (it's OK)", Continue);
    QVERIFY(!arr.isArray());
    QVERIFY(arr.isObject());
}

void tst_QScriptClass::getAndSetProperty()
{
    QScriptEngine eng;

    TestClass cls(&eng);

    QScriptValue obj1 = eng.newObject(&cls);
    QScriptValue obj2 = eng.newObject(&cls);
    QScriptString foo = eng.toStringHandle("foo");
    QScriptString bar = eng.toStringHandle("bar");
    QScriptValue num(&eng, 123);

    // should behave just like normal
    for (int x = 0; x < 2; ++x) {
        QScriptValue &o = (x == 0) ? obj1 : obj2;
        for (int y = 0; y < 2; ++y) {
            QScriptString &s = (y == 0) ? foo : bar;

            // read property
            cls.clearReceivedArgs();
            QScriptValue ret = o.property(s);
            QVERIFY(!ret.isValid());
            QVERIFY(cls.lastQueryPropertyObject().strictlyEquals(o));
            QVERIFY(cls.lastQueryPropertyName() == s);
            QVERIFY(!cls.lastPropertyObject().isValid());
            QVERIFY(!cls.lastSetPropertyObject().isValid());
            QVERIFY(cls.lastQueryPropertyFlags() == QScriptClass::HandlesReadAccess);

            // write property
            cls.clearReceivedArgs();
            o.setProperty(s, num);
            QVERIFY(cls.lastQueryPropertyObject().strictlyEquals(o));
            QVERIFY(cls.lastQueryPropertyName() == s);
            QVERIFY(!cls.lastPropertyObject().isValid());
            QVERIFY(!cls.lastSetPropertyObject().isValid());
            QVERIFY(cls.lastQueryPropertyFlags() == QScriptClass::HandlesWriteAccess);

            // re-read property
            // When a QScriptClass doesn't want to handle a property write,
            // that property becomes a normal property and the QScriptClass
            // shall not be queried about it again.
            cls.clearReceivedArgs();
            QVERIFY(o.property(s).strictlyEquals(num));
            QVERIFY(!cls.lastQueryPropertyObject().isValid());
        }
    }

    // add a custom property
    QScriptString foo2 = eng.toStringHandle("foo2");
    const uint foo2Id = 123;
    const QScriptValue::PropertyFlags foo2Pflags = QScriptValue::Undeletable;
    QScriptValue foo2Value(&eng, 456);
    cls.addCustomProperty(foo2, QScriptClass::HandlesReadAccess | QScriptClass::HandlesWriteAccess,
                          foo2Id, foo2Pflags, foo2Value);

    {
        // read property
        cls.clearReceivedArgs();
        {
            QScriptValue ret = obj1.property(foo2);
            QVERIFY(ret.strictlyEquals(foo2Value));
        }
        QVERIFY(cls.lastQueryPropertyObject().strictlyEquals(obj1));
        QVERIFY(cls.lastQueryPropertyName() == foo2);
        QVERIFY(cls.lastPropertyObject().strictlyEquals(obj1));
        QVERIFY(cls.lastPropertyName() == foo2);
        QCOMPARE(cls.lastPropertyId(), foo2Id);

        // read flags
        cls.clearReceivedArgs();
        QCOMPARE(obj1.propertyFlags(foo2), foo2Pflags);
        QVERIFY(cls.lastQueryPropertyObject().strictlyEquals(obj1));
        QVERIFY(cls.lastQueryPropertyName() == foo2);
        QEXPECT_FAIL("", "classObject.getOwnPropertyDescriptor() reads the property value", Continue);
        QVERIFY(!cls.lastPropertyObject().isValid());
        QVERIFY(cls.lastPropertyFlagsObject().strictlyEquals(obj1));
        QVERIFY(cls.lastPropertyFlagsName() == foo2);
        QCOMPARE(cls.lastPropertyFlagsId(), foo2Id);

        // write property
        cls.clearReceivedArgs();
        QScriptValue newFoo2Value(&eng, 789);
        obj1.setProperty(foo2, newFoo2Value);
        QVERIFY(cls.lastQueryPropertyObject().strictlyEquals(obj1));
        QVERIFY(cls.lastQueryPropertyName() == foo2);

        // read property again
        cls.clearReceivedArgs();
        {
            QScriptValue ret = obj1.property(foo2);
            QVERIFY(ret.strictlyEquals(newFoo2Value));
        }
        QVERIFY(cls.lastQueryPropertyObject().strictlyEquals(obj1));
        QVERIFY(cls.lastQueryPropertyName() == foo2);
        QVERIFY(cls.lastPropertyObject().strictlyEquals(obj1));
        QVERIFY(cls.lastPropertyName() == foo2);
        QCOMPARE(cls.lastPropertyId(), foo2Id);
    }

    // remove script class; normal properties should remain
    obj1.setScriptClass(0);
    QCOMPARE(obj1.scriptClass(), (QScriptClass*)0);
    QVERIFY(obj1.property(foo).equals(num));
    QVERIFY(obj1.property(bar).equals(num));
    obj1.setProperty(foo, QScriptValue());
    QVERIFY(!obj1.property(foo).isValid());
    obj1.setProperty(bar, QScriptValue());
    QVERIFY(!obj1.property(bar).isValid());
}

void tst_QScriptClass::enumerate()
{
    QScriptEngine eng;

    TestClass cls(&eng);

    QScriptValue obj = eng.newObject(&cls);
    QScriptString foo = eng.toStringHandle("foo");
    obj.setProperty(foo, QScriptValue(&eng, 123));

    cls.setIterationEnabled(false);
    {
        QScriptValueIterator it(obj);
        QVERIFY(it.hasNext());
        it.next();
        QVERIFY(it.scriptName() == foo);
        QVERIFY(!it.hasNext());
    }

    // add a custom property
    QScriptString foo2 = eng.toStringHandle("foo2");
    const uint foo2Id = 123;
    const QScriptValue::PropertyFlags foo2Pflags = QScriptValue::Undeletable;
    QScriptValue foo2Value(&eng, 456);
    cls.addCustomProperty(foo2, QScriptClass::HandlesReadAccess | QScriptClass::HandlesWriteAccess,
                          foo2Id, foo2Pflags, QScriptValue());

    cls.setIterationEnabled(true);
    QScriptValueIterator it(obj);
    for (int x = 0; x < 2; ++x) {
        QVERIFY(it.hasNext());
        it.next();
        QEXPECT_FAIL("", "", Abort);
        QVERIFY(it.scriptName() == foo);
        QVERIFY(it.hasNext());
        it.next();
        QVERIFY(it.scriptName() == foo2);
        QCOMPARE(it.flags(), foo2Pflags);
        QVERIFY(!it.hasNext());

        QVERIFY(it.hasPrevious());
        it.previous();
        QVERIFY(it.scriptName() == foo2);
        QCOMPARE(it.flags(), foo2Pflags);
        QVERIFY(it.hasPrevious());
        it.previous();
        QVERIFY(it.scriptName() == foo);
        QVERIFY(!it.hasPrevious());
    }
}

void tst_QScriptClass::extension()
{
    QScriptEngine eng;
    {
        TestClass cls(&eng);
        cls.setCallableMode(TestClass::NotCallable);
        QVERIFY(!cls.supportsExtension(QScriptClass::Callable));
        QVERIFY(!cls.supportsExtension(QScriptClass::HasInstance));
        QScriptValue obj = eng.newObject(&cls);
        QVERIFY(!obj.call().isValid());
        QCOMPARE((int)cls.lastExtensionType(), -1);
        QVERIFY(!obj.instanceOf(obj));
        QCOMPARE((int)cls.lastExtensionType(), -1);
    }
    // Callable
    {
        TestClass cls(&eng);
        cls.setCallableMode(TestClass::CallableReturnsSum);
        QVERIFY(cls.supportsExtension(QScriptClass::Callable));

        QScriptValue obj = eng.newObject(&cls);
        eng.globalObject().setProperty("obj", obj);
        obj.setProperty("one", QScriptValue(&eng, 1));
        obj.setProperty("two", QScriptValue(&eng, 2));
        obj.setProperty("three", QScriptValue(&eng, 3));
        // From C++
        cls.clearReceivedArgs();
        {
            QScriptValueList args;
            args << QScriptValue(&eng, 4) << QScriptValue(&eng, 5);
            QScriptValue ret = obj.call(obj, args);
            QCOMPARE(cls.lastExtensionType(), QScriptClass::Callable);
            QCOMPARE(cls.lastExtensionArgument().userType(), qMetaTypeId<QScriptContext*>());
            QVERIFY(ret.isNumber());
            QCOMPARE(ret.toNumber(), qsreal(15));
        }
        // From JS
        cls.clearReceivedArgs();
        {
            QScriptValue ret = eng.evaluate("obj(4, 5)");
            QCOMPARE(cls.lastExtensionType(), QScriptClass::Callable);
            QCOMPARE(cls.lastExtensionArgument().userType(), qMetaTypeId<QScriptContext*>());
            QVERIFY(ret.isNumber());
            QCOMPARE(ret.toNumber(), qsreal(15));
        }

        cls.setCallableMode(TestClass::CallableReturnsArgument);
        // From C++
        cls.clearReceivedArgs();
        {
            QScriptValue ret = obj.call(obj, QScriptValueList() << 123);
            QCOMPARE(cls.lastExtensionType(), QScriptClass::Callable);
            QCOMPARE(cls.lastExtensionArgument().userType(), qMetaTypeId<QScriptContext*>());
            QVERIFY(ret.isNumber());
            QCOMPARE(ret.toInt32(), 123);
        }
        cls.clearReceivedArgs();
        {
            QScriptValue ret = obj.call(obj, QScriptValueList() << true);
            QCOMPARE(cls.lastExtensionType(), QScriptClass::Callable);
            QCOMPARE(cls.lastExtensionArgument().userType(), qMetaTypeId<QScriptContext*>());
            QVERIFY(ret.isBoolean());
            QCOMPARE(ret.toBoolean(), true);
        }
        {
            QScriptValue ret = obj.call(obj, QScriptValueList() << QString::fromLatin1("ciao"));
            QVERIFY(ret.isString());
            QCOMPARE(ret.toString(), QString::fromLatin1("ciao"));
        }
        {
            QScriptValue objobj = eng.newObject();
            QScriptValue ret = obj.call(obj, QScriptValueList() << objobj);
            QVERIFY(ret.isObject());
            QVERIFY(ret.strictlyEquals(objobj));
        }
        {
            QScriptValue ret = obj.call(obj, QScriptValueList() << QScriptValue());
            QVERIFY(ret.isUndefined());
        }
        // From JS
        cls.clearReceivedArgs();
        {
            QScriptValue ret = eng.evaluate("obj(123)");
            QVERIFY(ret.isNumber());
            QCOMPARE(ret.toInt32(), 123);
        }

        cls.setCallableMode(TestClass::CallableReturnsInvalidVariant);
        {
            QScriptValue ret = obj.call(obj);
            QVERIFY(ret.isUndefined());
        }

        cls.setCallableMode(TestClass::CallableReturnsThisObject);
        // From C++
        {
            QScriptValue ret = obj.call(obj);
            QVERIFY(ret.isObject());
            QVERIFY(ret.strictlyEquals(obj));
        }
        // From JS
        {
            QScriptValue ret = eng.evaluate("obj()");
            QVERIFY(ret.isObject());
            QVERIFY(ret.strictlyEquals(eng.globalObject()));
        }

        cls.setCallableMode(TestClass::CallableReturnsCallee);
        // From C++
        {
            QScriptValue ret = obj.call();
            QVERIFY(ret.isObject());
            QVERIFY(ret.strictlyEquals(obj));
        }
        // From JS
        {
            QScriptValue ret = eng.evaluate("obj()");
            QVERIFY(ret.isObject());
            QVERIFY(ret.strictlyEquals(obj));
        }

        cls.setCallableMode(TestClass::CallableReturnsArgumentsObject);
        // From C++
        {
            QScriptValue ret = obj.call(obj, QScriptValueList() << 123);
            QVERIFY(ret.isObject());
            QVERIFY(ret.property("length").isNumber());
            QCOMPARE(ret.property("length").toInt32(), 1);
            QVERIFY(ret.property(0).isNumber());
            QCOMPARE(ret.property(0).toInt32(), 123);
        }
        // From JS
        {
            QScriptValue ret = eng.evaluate("obj(123)");
            QVERIFY(ret.isObject());
            QVERIFY(ret.property("length").isNumber());
            QCOMPARE(ret.property("length").toInt32(), 1);
            QVERIFY(ret.property(0).isNumber());
            QCOMPARE(ret.property(0).toInt32(), 123);
        }

        // construct()
        // From C++
        cls.clearReceivedArgs();
        cls.setCallableMode(TestClass::CallableReturnsGlobalObject);
        {
            QScriptValue ret = obj.construct();
            QCOMPARE(cls.lastExtensionType(), QScriptClass::Callable);
            QCOMPARE(cls.lastExtensionArgument().userType(), qMetaTypeId<QScriptContext*>());
            QVERIFY(ret.isObject());
            QVERIFY(ret.strictlyEquals(eng.globalObject()));
        }
        // From JS
        cls.clearReceivedArgs();
        {
            QScriptValue ret = eng.evaluate("new obj()");
            QCOMPARE(cls.lastExtensionType(), QScriptClass::Callable);
            QCOMPARE(cls.lastExtensionArgument().userType(), qMetaTypeId<QScriptContext*>());
            QVERIFY(ret.isObject());
            QVERIFY(ret.strictlyEquals(eng.globalObject()));
        }
        // From C++
        cls.clearReceivedArgs();
        cls.setCallableMode(TestClass::CallableInitializesThisObject);
        {
            QScriptValue ret = obj.construct();
            QCOMPARE(cls.lastExtensionType(), QScriptClass::Callable);
            QCOMPARE(cls.lastExtensionArgument().userType(), qMetaTypeId<QScriptContext*>());
            QVERIFY(ret.isQObject());
            QCOMPARE(ret.toQObject(), (QObject*)&eng);
        }
        // From JS
        cls.clearReceivedArgs();
        {
            QScriptValue ret = eng.evaluate("new obj()");
            QCOMPARE(cls.lastExtensionType(), QScriptClass::Callable);
            QCOMPARE(cls.lastExtensionArgument().userType(), qMetaTypeId<QScriptContext*>());
            QVERIFY(ret.isQObject());
            QCOMPARE(ret.toQObject(), (QObject*)&eng);
        }
    }
    // HasInstance
    {
        TestClass cls(&eng);
        cls.setHasInstance(true);
        QVERIFY(cls.supportsExtension(QScriptClass::HasInstance));

        QScriptValue obj = eng.newObject(&cls);
        obj.setProperty("foo", QScriptValue(&eng, 123));
        QScriptValue plain = eng.newObject();
        QVERIFY(!plain.instanceOf(obj));

        eng.globalObject().setProperty("HasInstanceTester", obj);
        eng.globalObject().setProperty("hasInstanceValue", plain);
        cls.clearReceivedArgs();
        {
            QScriptValue ret = eng.evaluate("hasInstanceValue instanceof HasInstanceTester");
            QCOMPARE(cls.lastExtensionType(), QScriptClass::HasInstance);
            QCOMPARE(cls.lastExtensionArgument().userType(), qMetaTypeId<QScriptValueList>());
            QScriptValueList lst = qvariant_cast<QScriptValueList>(cls.lastExtensionArgument());
            QCOMPARE(lst.size(), 2);
            QVERIFY(lst.at(0).strictlyEquals(obj));
            QVERIFY(lst.at(1).strictlyEquals(plain));
            QVERIFY(ret.isBoolean());
            QVERIFY(!ret.toBoolean());
        }

        plain.setProperty("foo", QScriptValue(&eng, 456));
        QVERIFY(!plain.instanceOf(obj));
        {
            QScriptValue ret = eng.evaluate("hasInstanceValue instanceof HasInstanceTester");
            QVERIFY(ret.isBoolean());
            QVERIFY(!ret.toBoolean());
        }

        plain.setProperty("foo", obj.property("foo"));
        QVERIFY(plain.instanceOf(obj));
        {
            QScriptValue ret = eng.evaluate("hasInstanceValue instanceof HasInstanceTester");
            QVERIFY(ret.isBoolean());
            QVERIFY(ret.toBoolean());
        }
    }
}

QTEST_MAIN(tst_QScriptClass)
#include "tst_qscriptclass.moc"
