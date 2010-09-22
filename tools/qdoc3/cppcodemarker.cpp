/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the tools applications of the Qt Toolkit.
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

/*
  cppcodemarker.cpp
*/

#include <qdebug.h>
#include "atom.h"
#include "cppcodemarker.h"
#include "node.h"
#include "text.h"
#include "tree.h"

QT_BEGIN_NAMESPACE

static int insertTagAround(QString &result, int pos, int len, const QString &tagName,
                           const QString &attributes = QString())
{
    QString s;
    //s.reserve(result.size() + tagName.size() * 2 + attributes.size() + 20);
    s += result.midRef(0, pos);
    s += QLatin1Char('<');
    s += tagName;
    if (!attributes.isEmpty()) {
        s += QLatin1Char(' ');
        s += attributes;
    }
    s += QLatin1Char('>');
    s += result.midRef(pos, len);
    s += QLatin1String("</");
    s += tagName;
    s += QLatin1Char('>');
    s += result.midRef(pos + len);
    int diff = s.length() - result.length();
    result = s;
    return diff;
}

/*!
  The constructor does nothing.
 */
CppCodeMarker::CppCodeMarker()
{
    // nothing.
}

/*!
  The destructor does nothing.
 */
CppCodeMarker::~CppCodeMarker()
{
    // nothing.
}

/*!
  Returns true.
 */
bool CppCodeMarker::recognizeCode(const QString & /* code */)
{
    return true;
}

/*!
  Returns true if \a ext is any of a list of file extensions
  for the C++ language.
 */
bool CppCodeMarker::recognizeExtension(const QString& ext)
{
    return ext == "c" ||
        ext == "c++" ||
        ext == "cc" ||
        ext == "cpp" ||
        ext == "cxx" ||
        ext == "ch" ||
        ext == "h" ||
        ext == "h++" ||
        ext == "hh" ||
        ext == "hpp" ||
        ext == "hxx";
}

/*!
  Returns true if \a lang is either "C" or "Cpp".
 */
bool CppCodeMarker::recognizeLanguage(const QString &lang)
{
    return lang == "C" || lang == "Cpp";
}

/*!
  Returns the \a node name, or "()" if \a node is a
  Node::Function node.
 */
QString CppCodeMarker::plainName(const Node *node)
{
    QString name = node->name();
    if (node->type() == Node::Function)
	name += "()";
    return name;
}

QString CppCodeMarker::plainFullName(const Node *node, const Node *relative)
{
    if (node->name().isEmpty()) {
	return "global";
    }
    else {
	QString fullName;
	while (node) {
	    fullName.prepend(plainName(node));
	    if (node->parent() == relative || node->parent()->name().isEmpty())
		break;
	    fullName.prepend("::");
	    node = node->parent();
        }
        return fullName;
    }
}

QString CppCodeMarker::markedUpCode(const QString &code,
                                    const Node *relative,
				    const QString &dirPath)
{
    return addMarkUp(protect(code), relative, dirPath);
}

QString CppCodeMarker::markedUpSynopsis(const Node *node,
                                        const Node * /* relative */,
					SynopsisStyle style)
{
    const int MaxEnumValues = 6;
    const FunctionNode *func;
    const PropertyNode *property;
    const VariableNode *variable;
    const EnumNode *enume;
    const TypedefNode *typedeff;
    QString synopsis;
    QString extra;
    QString name;

    name = taggedNode(node);
    if (style != Detailed)
	name = linkTag(node, name);
    name = "<@name>" + name + "</@name>";

    if (style == Detailed && !node->parent()->name().isEmpty() &&
        node->type() != Node::Property)
	name.prepend(taggedNode(node->parent()) + "::");

    switch (node->type()) {
    case Node::Namespace:
	synopsis = "namespace " + name;
	break;
    case Node::Class:
	synopsis = "class " + name;
	break;
    case Node::Function:
    case Node::QmlSignal:
    case Node::QmlMethod:
	func = (const FunctionNode *) node;
	if (style != SeparateList && !func->returnType().isEmpty())
	    synopsis = typified(func->returnType()) + " ";
	synopsis += name;
        if (func->metaness() != FunctionNode::MacroWithoutParams) {
            synopsis += " (";
	    if (!func->parameters().isEmpty()) {
	        synopsis += " ";
	        QList<Parameter>::ConstIterator p = func->parameters().begin();
	        while (p != func->parameters().end()) {
		    if (p != func->parameters().begin())
		        synopsis += ", ";
		    synopsis += typified((*p).leftType());
                    if (style != SeparateList && !(*p).name().isEmpty())
                        synopsis +=
                            " <@param>" + protect((*p).name()) + "</@param>";
                    synopsis += protect((*p).rightType());
		    if (style != SeparateList && !(*p).defaultValue().isEmpty())
		        synopsis += " = " + protect((*p).defaultValue());
		    ++p;
	        }
	        synopsis += " ";
	    }
	    synopsis += ")";
        }
	if (func->isConst())
	    synopsis += " const";

	if (style == Summary || style == Accessors) {
	    if (func->virtualness() != FunctionNode::NonVirtual)
		synopsis.prepend("virtual ");
	    if (func->virtualness() == FunctionNode::PureVirtual)
		synopsis.append(" = 0");
	}
        else if (style == SeparateList) {
            if (!func->returnType().isEmpty() && func->returnType() != "void")
                synopsis += " : " + typified(func->returnType());
        }
        else {
	    QStringList bracketed;
	    if (func->isStatic()) {
		bracketed += "static";
	    }
            else if (func->virtualness() != FunctionNode::NonVirtual) {
		if (func->virtualness() == FunctionNode::PureVirtual)
		    bracketed += "pure";
		bracketed += "virtual";
	    }

	    if (func->access() == Node::Protected) {
		bracketed += "protected";
	    }
            else if (func->access() == Node::Private) {
		bracketed += "private";
	    }

	    if (func->metaness() == FunctionNode::Signal) {
		bracketed += "signal";
	    }
            else if (func->metaness() == FunctionNode::Slot) {
		bracketed += "slot";
	    }
	    if (!bracketed.isEmpty())
		extra += " [" + bracketed.join(" ") + "]";
	}
	break;
    case Node::Enum:
	enume = static_cast<const EnumNode *>(node);
	synopsis = "enum " + name;
        if (style == Summary) {
            synopsis += " { ";

            QStringList documentedItems = enume->doc().enumItemNames();
            if (documentedItems.isEmpty()) {
                foreach (const EnumItem &item, enume->items())
                    documentedItems << item.name();
            }
            QStringList omitItems = enume->doc().omitEnumItemNames();
            foreach (const QString &item, omitItems)
                documentedItems.removeAll(item);

            if (documentedItems.size() <= MaxEnumValues) {
                for (int i = 0; i < documentedItems.size(); ++i) {
	            if (i != 0)
		        synopsis += ", ";
		    synopsis += documentedItems.at(i);
                }
            }
            else {
                for (int i = 0; i < documentedItems.size(); ++i) {
		    if (i < MaxEnumValues-2 || i == documentedItems.size()-1) {
	                if (i != 0)
		            synopsis += ", ";
		        synopsis += documentedItems.at(i);
		    }
                    else if (i == MaxEnumValues - 1) {
		        synopsis += ", ...";
		    }
                }
            }
	    if (!documentedItems.isEmpty())
		synopsis += " ";
	    synopsis += "}";
	}
	break;
    case Node::Typedef:
        typedeff = static_cast<const TypedefNode *>(node);
        if (typedeff->associatedEnum()) {
            synopsis = "flags " + name;
        }
        else {
            synopsis = "typedef " + name;
        }
	break;
    case Node::Property:
	property = static_cast<const PropertyNode *>(node);
	synopsis = name + " : " + typified(property->qualifiedDataType());
	break;
    case Node::Variable:
	variable = static_cast<const VariableNode *>(node);
        if (style == SeparateList) {
            synopsis = name + " : " + typified(variable->dataType());
        }
        else {
            synopsis = typified(variable->leftType()) + " " +
                name + protect(variable->rightType());
        }
	break;
    default:
	synopsis = name;
    }

    if (style == Summary) {
	if (node->status() == Node::Preliminary) {
	    extra += " (preliminary)";
	}
        else if (node->status() == Node::Deprecated) {
	    extra += " (deprecated)";
	}
        else if (node->status() == Node::Obsolete) {
	    extra += " (obsolete)";
	}
    }

    if (!extra.isEmpty()) {
	extra.prepend("<@extra>");
	extra.append("</@extra>");
    }
    return synopsis + extra;
}

#ifdef QDOC_QML
/*!
 */
QString CppCodeMarker::markedUpQmlItem(const Node* node, bool summary)
{
    QString name = taggedQmlNode(node);
    if (summary) {
	name = linkTag(node,name);
    } else if (node->type() == Node::QmlProperty) {
        const QmlPropertyNode* pn = static_cast<const QmlPropertyNode*>(node);
        if (pn->isAttached())
            name.prepend(pn->element() + QLatin1Char('.'));
    }
    name = "<@name>" + name + "</@name>";
    QString synopsis = name;
    if (node->type() == Node::QmlProperty) {
        const QmlPropertyNode* pn = static_cast<const QmlPropertyNode*>(node);
        synopsis += " : " + typified(pn->dataType());
    }

    QString extra;
    if (summary) {
	if (node->status() == Node::Preliminary) {
	    extra += " (preliminary)";
	}
        else if (node->status() == Node::Deprecated) {
	    extra += " (deprecated)";
	}
        else if (node->status() == Node::Obsolete) {
	    extra += " (obsolete)";
	}
    }

    if (!extra.isEmpty()) {
	extra.prepend("<@extra>");
	extra.append("</@extra>");
    }
    return synopsis + extra;
}
#endif

QString CppCodeMarker::markedUpName(const Node *node)
{
    QString name = linkTag(node, taggedNode(node));
    if (node->type() == Node::Function)
	name += "()";
    return name;
}

QString CppCodeMarker::markedUpFullName(const Node *node, const Node *relative)
{
    if (node->name().isEmpty()) {
	return "global";
    }
    else {
	QString fullName;
	for (;;) {
	    fullName.prepend(markedUpName(node));
	    if (node->parent() == relative || node->parent()->name().isEmpty())
		break;
	    fullName.prepend("<@op>::</@op>");
	    node = node->parent();
        }
        return fullName;
    }
}

QString CppCodeMarker::markedUpEnumValue(const QString &enumValue,
                                         const Node *relative)
{
    const Node *node = relative->parent();
    QString fullName;
    while (node->parent()) {
	fullName.prepend(markedUpName(node));
	if (node->parent() == relative || node->parent()->name().isEmpty())
	    break;
	fullName.prepend("<@op>::</@op>");
	node = node->parent();
    }
    if (!fullName.isEmpty())
        fullName.append("<@op>::</@op>");
    fullName.append(enumValue);
    return fullName;
}

QString CppCodeMarker::markedUpIncludes(const QStringList& includes)
{
    QString code;

    QStringList::ConstIterator inc = includes.begin();
    while (inc != includes.end()) {
	code += "#include &lt;<@headerfile>" + *inc + "</@headerfile>&gt;\n";
	++inc;
    }
    return addMarkUp(code, 0, "");
}

QString CppCodeMarker::functionBeginRegExp(const QString& funcName)
{
    return "^" + QRegExp::escape(funcName) + "$";

}

QString CppCodeMarker::functionEndRegExp(const QString& /* funcName */)
{
    return "^\\}$";
}

#if 0
	    FastSection privateReimpFuncs(classe,
                                          "Private Reimplemented Functions",
                                          "private reimplemented function",
                                          "private reimplemented functions");
	    FastSection protectedReimpFuncs(classe,
                                            "Protected Reimplemented Functions",
                                            "protected reimplemented function",
                                            "protected reimplemented functions");
	    FastSection publicReimpFuncs(classe,
                                         "Public Reimplemented Functions",
                                         "public reimplemented function",
                                         "public reimplemented functions");
#endif

QList<Section> CppCodeMarker::sections(const InnerNode *inner,
                                       SynopsisStyle style,
                                       Status status)
{
    QList<Section> sections;

    if (inner->type() == Node::Class) {
        const ClassNode *classe = static_cast<const ClassNode *>(inner);

        if (style == Summary) {
	    FastSection privateFunctions(classe,
                                         "Private Functions",
                                         "",
                                         "private function",
				         "private functions");
	    FastSection privateSlots(classe, "Private Slots", "", "private slot", "private slots");
	    FastSection privateTypes(classe, "Private Types", "", "private type", "private types");
	    FastSection protectedFunctions(classe,
                                           "Protected Functions",
                                           "",
                                           "protected function",
				           "protected functions");
	    FastSection protectedSlots(classe,
                                       "Protected Slots",
                                       "",
                                       "protected slot",
                                       "protected slots");
	    FastSection protectedTypes(classe,
                                       "Protected Types",
                                       "",
                                       "protected type",
                                       "protected types");
	    FastSection protectedVariables(classe,
                                           "Protected Variables",
                                           "",
                                           "protected type",
                                           "protected variables");
	    FastSection publicFunctions(classe,
                                        "Public Functions",
                                        "",
                                        "public function",
                                        "public functions");
	    FastSection publicSignals(classe, "Signals", "", "signal", "signals");
	    FastSection publicSlots(classe, "Public Slots", "", "public slot", "public slots");
	    FastSection publicTypes(classe, "Public Types", "", "public type", "public types");
	    FastSection publicVariables(classe,
                                        "Public Variables",
                                        "",
                                        "public variable",
                                        "public variables");
	    FastSection properties(classe, "Properties", "", "property", "properties");
	    FastSection relatedNonMembers(classe,
                                          "Related Non-Members",
                                          "",
                                          "related non-member",
                                          "related non-members");
	    FastSection staticPrivateMembers(classe,
                                             "Static Private Members",
                                             "",
                                             "static private member",
					     "static private members");
	    FastSection staticProtectedMembers(classe,
                                               "Static Protected Members",
                                               "",
					       "static protected member",
                                               "static protected members");
	    FastSection staticPublicMembers(classe,
                                            "Static Public Members",
                                            "",
                                            "static public member",
					    "static public members");
            FastSection macros(inner, "Macros", "", "macro", "macros");

	    NodeList::ConstIterator r = classe->relatedNodes().begin();
            while (r != classe->relatedNodes().end()) {
                if ((*r)->type() == Node::Function) {
                    FunctionNode *func = static_cast<FunctionNode *>(*r);
                    if (func->isMacro())
                        insert(macros, *r, style, status);
                    else
                        insert(relatedNonMembers, *r, style, status);
                }
                else {
                    insert(relatedNonMembers, *r, style, status);
                }
	        ++r;
            }

	    QStack<const ClassNode *> stack;
	    stack.push(classe);

	    while (!stack.isEmpty()) {
	        const ClassNode *ancestorClass = stack.pop();

	        NodeList::ConstIterator c = ancestorClass->childNodes().begin();
	        while (c != ancestorClass->childNodes().end()) {
	            bool isSlot = false;
	            bool isSignal = false;
	            bool isStatic = false;
	            if ((*c)->type() == Node::Function) {
		        const FunctionNode *func = (const FunctionNode *) *c;
		        isSlot = (func->metaness() == FunctionNode::Slot);
		        isSignal = (func->metaness() == FunctionNode::Signal);
		        isStatic = func->isStatic();
	            }
                    else if ((*c)->type() == Node::Variable) {
                        const VariableNode *var = static_cast<const VariableNode *>(*c);
                        isStatic = var->isStatic();
                    }

	            switch ((*c)->access()) {
	            case Node::Public:
		        if (isSlot) {
		            insert(publicSlots, *c, style, status);
		        }
                        else if (isSignal) {
		            insert(publicSignals, *c, style, status);
		        }
                        else if (isStatic) {
                            if ((*c)->type() != Node::Variable
                                    || !(*c)->doc().isEmpty())
		                insert(staticPublicMembers,*c,style,status);
		        }
                        else if ((*c)->type() == Node::Property) {
                            insert(properties, *c, style, status);
		        }
                        else if ((*c)->type() == Node::Variable) {
                            if (!(*c)->doc().isEmpty())
                                insert(publicVariables, *c, style, status);
		        }
                        else if ((*c)->type() == Node::Function) {
                            if (!insertReimpFunc(publicFunctions,*c,status))
                                insert(publicFunctions, *c, style, status);
		        }
                        else {
		            insert(publicTypes, *c, style, status);
		        }
		        break;
	            case Node::Protected:
		        if (isSlot) {
		            insert(protectedSlots, *c, style, status);
		        }
                        else if (isStatic) {
                            if ((*c)->type() != Node::Variable
                                    || !(*c)->doc().isEmpty())
		                insert(staticProtectedMembers,*c,style,status);
		        }
                        else if ((*c)->type() == Node::Variable) {
                            if (!(*c)->doc().isEmpty())
                                insert(protectedVariables,*c,style,status);
		        }
                        else if ((*c)->type() == Node::Function) {
                            if (!insertReimpFunc(protectedFunctions,*c,status))
                                insert(protectedFunctions, *c, style, status);
		        }
                        else {
		            insert(protectedTypes, *c, style, status);
		        }
		        break;
	            case Node::Private:
		        if (isSlot) {
		            insert(privateSlots, *c, style, status);
		        }
                        else if (isStatic) {
                            if ((*c)->type() != Node::Variable
                                    || !(*c)->doc().isEmpty())
		                insert(staticPrivateMembers,*c,style,status);
		        }
                        else if ((*c)->type() == Node::Function) {
                            if (!insertReimpFunc(privateFunctions,*c,status))
                                insert(privateFunctions, *c, style, status);
		        }
                        else {
		            insert(privateTypes,*c,style,status);
		        }
	            }
	            ++c;
	        }

	        QList<RelatedClass>::ConstIterator r =
                    ancestorClass->baseClasses().begin();
	        while (r != ancestorClass->baseClasses().end()) {
		    stack.prepend((*r).node);
		    ++r;
	        }
	    }

	    append(sections, publicTypes);
	    append(sections, properties);
	    append(sections, publicFunctions);
	    append(sections, publicSlots);
	    append(sections, publicSignals);
	    append(sections, publicVariables);
	    append(sections, staticPublicMembers);
	    append(sections, protectedTypes);
	    append(sections, protectedFunctions);
	    append(sections, protectedSlots);
	    append(sections, protectedVariables);
	    append(sections, staticProtectedMembers);
	    append(sections, privateTypes);
	    append(sections, privateFunctions);
	    append(sections, privateSlots);
	    append(sections, staticPrivateMembers);
	    append(sections, relatedNonMembers);
            append(sections, macros);
        }
        else if (style == Detailed) {
	    FastSection memberFunctions(classe,"Member Function Documentation","func","member","members");
	    FastSection memberTypes(classe,"Member Type Documentation","types","member","members");
	    FastSection memberVariables(classe,"Member Variable Documentation","vars","member","members");
	    FastSection properties(classe,"Property Documentation","prop","member","members");
	    FastSection relatedNonMembers(classe,"Related Non-Members","relnonmem","member","members");
	    FastSection macros(classe,"Macro Documentation","macros","member","members");

	    NodeList::ConstIterator r = classe->relatedNodes().begin();
            while (r != classe->relatedNodes().end()) {
                if ((*r)->type() == Node::Function) {
                    FunctionNode *func = static_cast<FunctionNode *>(*r);
                    if (func->isMacro())
                        insert(macros, *r, style, status);
                    else
                        insert(relatedNonMembers, *r, style, status);
                }
                else {
                    insert(relatedNonMembers, *r, style, status);
                }
	        ++r;
            }

	    NodeList::ConstIterator c = classe->childNodes().begin();
	    while (c != classe->childNodes().end()) {
	        if ((*c)->type() == Node::Enum ||
                    (*c)->type() == Node::Typedef) {
		    insert(memberTypes, *c, style, status);
	        }
                else if ((*c)->type() == Node::Property) {
		    insert(properties, *c, style, status);
	        }
                else if ((*c)->type() == Node::Variable) {
                    if (!(*c)->doc().isEmpty())
		        insert(memberVariables, *c, style, status);
	        }
                else if ((*c)->type() == Node::Function) {
		    FunctionNode *function = static_cast<FunctionNode *>(*c);
                    if (!function->associatedProperty())
		        insert(memberFunctions, function, style, status);
	        }
	        ++c;
	    }

	    append(sections, memberTypes);
	    append(sections, properties);
	    append(sections, memberFunctions);
	    append(sections, memberVariables);
	    append(sections, relatedNonMembers);
	    append(sections, macros);
        }
        else {
	    FastSection all(classe,"","","member","members");

	    QStack<const ClassNode *> stack;
	    stack.push(classe);

	    while (!stack.isEmpty()) {
	        const ClassNode *ancestorClass = stack.pop();

	        NodeList::ConstIterator c = ancestorClass->childNodes().begin();
	        while (c != ancestorClass->childNodes().end()) {
		    if ((*c)->access() != Node::Private &&
                        (*c)->type() != Node::Property)
		        insert(all, *c, style, status);
		    ++c;
	        }

	        QList<RelatedClass>::ConstIterator r =
                    ancestorClass->baseClasses().begin();
	        while (r != ancestorClass->baseClasses().end()) {
		    stack.prepend((*r).node);
		    ++r;
	        }
	    }
	    append(sections, all);
        }
    }
    else {
        if (style == Summary || style == Detailed) {
	    FastSection namespaces(inner,
                                   "Namespaces",
                                   style == Detailed ? "nmspace" : "",
                                   "namespace",
                                   "namespaces");
            FastSection classes(inner,
                                "Classes",
                                style == Detailed ? "classes" : "",
                                "class",
                                "classes");
            FastSection types(inner,
                              style == Summary ? "Types" : "Type Documentation",
                              style == Detailed ? "types" : "",
                              "type",
			      "types");
            FastSection functions(inner,
                                  style == Summary ?
                                  "Functions" : "Function Documentation",
                                  style == Detailed ? "func" : "",
			          "function",
                                  "functions");
            FastSection macros(inner,
                               style == Summary ?
                               "Macros" : "Macro Documentation",
                               style == Detailed ? "macros" : "",
                               "macro",
                               "macros");

	    NodeList nodeList = inner->childNodes();
            nodeList += inner->relatedNodes();

	    NodeList::ConstIterator n = nodeList.begin();
            while (n != nodeList.end()) {
	        switch ((*n)->type()) {
                case Node::Namespace:
		    insert(namespaces, *n, style, status);
                    break;
	        case Node::Class:
		    insert(classes, *n, style, status);
                    break;
	        case Node::Enum:
	        case Node::Typedef:
		    insert(types, *n, style, status);
                    break;
	        case Node::Function:
                    {
                        FunctionNode *func = static_cast<FunctionNode *>(*n);
                        if (func->isMacro())
		            insert(macros, *n, style, status);
                        else
		            insert(functions, *n, style, status);
                    }
                    break;
	        default:
		    ;
	        }
	        ++n;
            }
            append(sections, namespaces);
            append(sections, classes);
            append(sections, types);
            append(sections, functions);
            append(sections, macros);
        }
    }

    return sections;
}

const Node *CppCodeMarker::resolveTarget(const QString& target,
                                         const Tree* tree,
                                         const Node* relative,
                                         const Node* self)
{
    if (target.endsWith("()")) {
        const FunctionNode *func;
        QString funcName = target;
        funcName.chop(2);

        QStringList path = funcName.split("::");
        if ((func = tree->findFunctionNode(path,
                                           relative,
                                           Tree::SearchBaseClasses))
                && func->metaness() != FunctionNode::MacroWithoutParams)
            return func;
    }
    else if (target.contains("#")) {
        // ### this doesn't belong here; get rid of TargetNode hack
        int hashAt = target.indexOf("#");
        QString link = target.left(hashAt);
        QString ref = target.mid(hashAt + 1);
        const Node *node;
        if (link.isEmpty()) {
            node = relative;
        }
        else {
            QStringList path(link);
            node = tree->findNode(path, tree->root(), Tree::SearchBaseClasses);
        }
        if (node && node->isInnerNode()) {
            const Atom *atom = node->doc().body().firstAtom();
            while (atom) {
                if (atom->type() == Atom::Target && atom->string() == ref) {
                    Node *parentNode = const_cast<Node *>(node);
                    return new TargetNode(static_cast<InnerNode*>(parentNode),
                                          ref);
                }
                atom = atom->next();
            }
        }
    }
    else {
        QStringList path = target.split("::");
        const Node *node;
        int flags = Tree::SearchBaseClasses |
            Tree::SearchEnumValues |
            Tree::NonFunction;
        if ((node = tree->findNode(path,
                                   relative,
                                   flags,
                                   self)))
            return node;
    }
    return 0;
}

QString CppCodeMarker::addMarkUp(const QString& protectedCode,
                                 const Node * /* relative */,
                                 const QString& /* dirPath */)
{
    static QRegExp globalInclude("#include +&lt;([^<>&]+)&gt;");
    static QRegExp yHasTypeX("(?:^|\n *)([a-zA-Z_][a-zA-Z_0-9]*)"
	                     "(?:&lt;[^;{}]+&gt;)?(?: *(?:\\*|&amp;) *| +)"
	                     "([a-zA-Z_][a-zA-Z_0-9]*)? *[,;()=]");
    static QRegExp xNewY("([a-zA-Z_][a-zA-Z_0-9]*) *= *new +([a-zA-Z_0-9]+)");
    static QRegExp xDotY("\\b([a-zA-Z_][a-zA-Z_0-9]*) *(?:\\.|-&gt;|,[ \n]*S(?:IGNAL|LOT)\\() *"
	                 "([a-zA-Z_][a-zA-Z_0-9]*)(?= *\\()");
    static QRegExp xIsStaticZOfY("[\n:;{(=] *(([a-zA-Z_0-9]+)::([a-zA-Z_0-9]+))(?= *\\()");
    static QRegExp classX("[:,][ \n]*(?:p(?:ublic|r(?:otected|ivate))[ \n]+)?"
                          "([a-zA-Z_][a-zA-Z_0-9]*)");
    static QRegExp globalX("[\n{()=] *([a-zA-Z_][a-zA-Z_0-9]*)[ \n]*\\(");
    static QRegExp multiLineComment("/(?:( )?\\*(?:[^*]+|\\*(?! /))*\\*\\1/)");
    multiLineComment.setMinimal(true);
    static QRegExp singleLineComment("[^:]//(?!!)[^!\\n]*");
    static QRegExp preprocessor("(?:^|\n)(#[ \t]*(?:include|if|elif|endif|error|pragma|define"
                                "|warning)(?:(?:\\\\\n|\\n#)[^\n]*)*)");
    static QRegExp literals("&quot;(?:[^\\\\&]|\\\\[^\n]|&(?!quot;))*&quot;"
                            "|'(?:[^\\\\]|\\\\(?:[^x0-9']|x[0-9a-f]{1,4}|[0-9]{1,3}))'");

    QString result = protectedCode;
    int pos;

    if (!hurryUp()) {
        /*
          Mark global includes. For example:

          #include &lt;<@headerfile>QString</@headerfile>
        */
        pos = 0;
        while ((pos = result.indexOf(globalInclude, pos)) != -1)
            pos += globalInclude.matchedLength()
                   + insertTagAround(result,
                                     globalInclude.pos(1),
                                     globalInclude.cap(1).length(),
                                     "@headerfile");

        /*
            Look for variable definitions and similar constructs, mark
            the data type, and remember the type of the variable.
        */
        QMap<QString, QSet<QString> > typesForVariable;
        pos = 0;
        while ((pos = yHasTypeX.indexIn(result, pos)) != -1) {
	    QString x = yHasTypeX.cap(1);
	    QString y = yHasTypeX.cap(2);

	    if (!y.isEmpty())
	        typesForVariable[y].insert(x);

	    /*
                Without the minus one at the end, 'void member(Class
                var)' would give 'member' as a variable of type 'void',
                but would ignore 'Class var'. (### Is that true?)
	    */
            pos += yHasTypeX.matchedLength()
                   + insertTagAround(result,
                                     yHasTypeX.pos(1),
                                     x.length(),
                                     "@type") - 1;
        }

        /*
            Do syntax highlighting of preprocessor directives.
        */
        pos = 0;
        while ((pos = preprocessor.indexIn(result, pos)) != -1)
            pos += preprocessor.matchedLength()
                   + insertTagAround(result,
                                     preprocessor.pos(1),
                                     preprocessor.cap(1).length(),
                                     "@preprocessor");

        /*
            Deal with string and character literals.
        */
        pos = 0;
        while ((pos = literals.indexIn(result, pos)) != -1)
            pos += literals.matchedLength()
                   + insertTagAround(result,
                                     pos,
                                     literals.matchedLength(),
                                     result.at(pos) ==
                                       QLatin1Char(' ') ? "@string" : "@char");

        /*
            Look for 'var = new Class'.
        */
        pos = 0;
        while ((pos = xNewY.indexIn(result, pos)) != -1) {
	    QString x = xNewY.cap(1);
	    QString y = xNewY.cap(2);
	    typesForVariable[x].insert(y);

	    pos += xNewY.matchedLength() + insertTagAround(result,
                                                           xNewY.pos(2),
                                                           y.length(),
                                                           "@type");
        }

        /*
            Insert some stuff that cannot harm.
        */
        typesForVariable["qApp"].insert("QApplication");

        /*
            Add link to ': Class'.
        */
        pos = 0;
        while ((pos = classX.indexIn(result, pos)) != -1)
	    pos += classX.matchedLength()
                   + insertTagAround(result,
                                     classX.pos(1),
                                     classX.cap(1).length(),
                                     "@type") - 1;

        /*
            Find use of any of

                var.method()
	        var->method()
	        var, SIGNAL(method())
	        var, SLOT(method()).
        */
        pos = 0;
        while ((pos = xDotY.indexIn(result, pos)) != -1) {
	    QString x = xDotY.cap(1);
	    QString y = xDotY.cap(2);

	    QSet<QString> types = typesForVariable.value(x);
            pos += xDotY.matchedLength()
                   + insertTagAround(result,
                                     xDotY.pos(2),
                                     xDotY.cap(2).length(),
                                     "@func",
                                     (types.count() == 1) ? "target=\""
                                        + protect(*types.begin() + "::" + y)
                                        + "()\"" : QString());
        }

        /*
            Add link to 'Class::method()'.
        */
        pos = 0;
        while ((pos = xIsStaticZOfY.indexIn(result, pos)) != -1) {
	    QString x = xIsStaticZOfY.cap(1);
	    QString z = xIsStaticZOfY.cap(3);

            pos += insertTagAround(result,
                                   xIsStaticZOfY.pos(3),
                                   z.length(),
                                   "@func",
                                   "target=\"" + protect(x) + "()\"");
            pos += insertTagAround(result,
                                   xIsStaticZOfY.pos(2),
                                   xIsStaticZOfY.cap(2).length(),
                                   "@type");
            pos += xIsStaticZOfY.matchedLength() - 1;
        }

        /*
            Add link to 'globalFunction()'.
        */
        pos = 0;
        while ((pos = globalX.indexIn(result, pos)) != -1) {
            QString x = globalX.cap(1);
	    if (x != "QT_FORWARD_DECLARE_CLASS") {
                pos += globalX.matchedLength()
                       + insertTagAround(result,
                                         globalX.pos(1),
                                         x.length(),
                                         "@func",
                                         "target=\"" + protect(x) + "()\"") - 1;
            }
            else
                pos += globalX.matchedLength();
        }
    }

    /*
        Do syntax highlighting of comments. Also alter the code in a
        minor way, so that we can include comments in documentation
        comments.
    */
    pos = 0;
    while (pos != -1) {
        int mlpos;
        int slpos;
        int len;
        slpos = singleLineComment.indexIn(result, pos);
        mlpos = multiLineComment.indexIn(result, pos);

        if (slpos == -1 && mlpos == -1)
            break;

        if (slpos == -1) {
            pos = mlpos;
            len = multiLineComment.matchedLength();
        }
        else if (mlpos == -1) {
            pos = slpos + 1;
            len = singleLineComment.matchedLength() - 1;
        }
        else {
            if (slpos < mlpos) {
                pos = slpos + 1;
                len = singleLineComment.matchedLength() - 1;
            }
            else {
                pos = mlpos;
                len = multiLineComment.matchedLength();
            }
        }

        if (result.at(pos + 1) == QLatin1Char(' ')) {
            result.remove(pos + len - 2, 1);
            result.remove(pos + 1, 1);
            len -= 2;

            forever {
                int endcodePos = result.indexOf("\\ endcode", pos);
                if (endcodePos == -1 || endcodePos >= pos + len)
                    break;
                result.remove(endcodePos + 1, 1);
                len -= 1;
            }
        }
        pos += len + insertTagAround(result, pos, len, "@comment");
    }

    return result;
}

#ifdef QDOC_QML
/*!
  This function is for documenting QML properties. It returns
  the list of documentation sections for the children of the
  \a qmlClassNode.

  Currently, it only handles QML property groups.
 */
QList<Section> CppCodeMarker::qmlSections(const QmlClassNode* qmlClassNode,
                                          SynopsisStyle style,
                                          const Tree* tree)
{
    QList<Section> sections;
    if (qmlClassNode) {
        if (style == Summary) {
	    FastSection qmlproperties(qmlClassNode,
                                      "Properties",
                                      "",
                                      "property",
                                      "properties");
	    FastSection qmlattachedproperties(qmlClassNode,
                                              "Attached Properties",
                                              "",
                                              "property",
                                              "properties");
	    FastSection qmlsignals(qmlClassNode,
                                   "Signals",
                                   "",
                                   "signal",
                                   "signals");
	    FastSection qmlattachedsignals(qmlClassNode,
                                           "Attached Signals",
                                           "",
                                           "signal",
                                           "signals");
	    FastSection qmlmethods(qmlClassNode,
                                   "Methods",
                                   "",
                                   "method",
                                   "methods");
	    FastSection qmlattachedmethods(qmlClassNode,
                                           "Attached Methods",
                                           "",
                                           "method",
                                           "methods");

            NodeList::ConstIterator c = qmlClassNode->childNodes().begin();
            while (c != qmlClassNode->childNodes().end()) {
                if ((*c)->subType() == Node::QmlPropertyGroup) {
                    const QmlPropGroupNode* qpgn = static_cast<const QmlPropGroupNode*>(*c);
                    NodeList::ConstIterator p = qpgn->childNodes().begin();
                    while (p != qpgn->childNodes().end()) {
                        if ((*p)->type() == Node::QmlProperty) {
                            const QmlPropertyNode* pn = static_cast<const QmlPropertyNode*>(*p);
                            if (pn->isAttached())
                                insert(qmlattachedproperties,*p,style,Okay);
                            else
                                insert(qmlproperties,*p,style,Okay);
                        }
                        ++p;
                    }
                }
                else if ((*c)->type() == Node::QmlSignal) {
                    const FunctionNode* sn = static_cast<const FunctionNode*>(*c);
                    if (sn->isAttached())
                        insert(qmlattachedsignals,*c,style,Okay);
                    else
                        insert(qmlsignals,*c,style,Okay);
                }
                else if ((*c)->type() == Node::QmlMethod) {
                    const FunctionNode* mn = static_cast<const FunctionNode*>(*c);
                    if (mn->isAttached())
                        insert(qmlattachedmethods,*c,style,Okay);
                    else
                        insert(qmlmethods,*c,style,Okay);
                }
                ++c;
            }
	    append(sections,qmlproperties);
	    append(sections,qmlattachedproperties);
	    append(sections,qmlsignals);
	    append(sections,qmlattachedsignals);
	    append(sections,qmlmethods);
	    append(sections,qmlattachedmethods);
        }
        else if (style == Detailed) {
            FastSection qmlproperties(qmlClassNode, "Property Documentation","qmlprop","member","members");
	    FastSection qmlattachedproperties(qmlClassNode,"Attached Property Documentation","qmlattprop",
                                              "member","members");
            FastSection qmlsignals(qmlClassNode,"Signal Documentation","qmlsig","member","members");
	    FastSection qmlattachedsignals(qmlClassNode,"Attached Signal Documentation","qmlattsig",
                                           "member","members");
            FastSection qmlmethods(qmlClassNode,"Method Documentation","qmlmeth","member","members");
	    FastSection qmlattachedmethods(qmlClassNode,"Attached Method Documentation","qmlattmeth",
                                           "member","members");
	    NodeList::ConstIterator c = qmlClassNode->childNodes().begin();
	    while (c != qmlClassNode->childNodes().end()) {
                if ((*c)->subType() == Node::QmlPropertyGroup) {
                    const QmlPropGroupNode* pgn = static_cast<const QmlPropGroupNode*>(*c);
                    if (pgn->isAttached())
                        insert(qmlattachedproperties,*c,style,Okay);
                    else
                        insert(qmlproperties,*c,style,Okay);
	        }
                else if ((*c)->type() == Node::QmlSignal) {
                    const FunctionNode* sn = static_cast<const FunctionNode*>(*c);
                    if (sn->isAttached())
                        insert(qmlattachedsignals,*c,style,Okay);
                    else
                        insert(qmlsignals,*c,style,Okay);
                }
                else if ((*c)->type() == Node::QmlMethod) {
                    const FunctionNode* mn = static_cast<const FunctionNode*>(*c);
                    if (mn->isAttached())
                        insert(qmlattachedmethods,*c,style,Okay);
                    else
                        insert(qmlmethods,*c,style,Okay);
                }
	        ++c;
	    }
	    append(sections,qmlproperties);
	    append(sections,qmlattachedproperties);
	    append(sections,qmlsignals);
	    append(sections,qmlattachedsignals);
	    append(sections,qmlmethods);
	    append(sections,qmlattachedmethods);
        }
        else {
	    FastSection all(qmlClassNode,"","","member","members");

	    QStack<const QmlClassNode*> stack;
	    stack.push(qmlClassNode);

	    while (!stack.isEmpty()) {
	        const QmlClassNode* ancestorClass = stack.pop();

	        NodeList::ConstIterator c = ancestorClass->childNodes().begin();
	        while (c != ancestorClass->childNodes().end()) {
                    //		    if ((*c)->access() != Node::Private)
                    if ((*c)->subType() == Node::QmlPropertyGroup) {
                        const QmlPropGroupNode* qpgn = static_cast<const QmlPropGroupNode*>(*c);
                        NodeList::ConstIterator p = qpgn->childNodes().begin();
                        while (p != qpgn->childNodes().end()) {
                            if ((*p)->type() == Node::QmlProperty) {
                                insert(all,*p,style,Okay);
                            }
                            ++p;
                        }
                    }
                    else
                        insert(all,*c,style,Okay);
                    ++c;
                }

                if (!ancestorClass->links().empty()) {
                    if (ancestorClass->links().contains(Node::InheritsLink)) {
                        QPair<QString,QString> linkPair;
                        linkPair = ancestorClass->links()[Node::InheritsLink];
                        QStringList strList(linkPair.first);
                        const Node* n = tree->findNode(strList,Node::Fake);
                        if (n && n->subType() == Node::QmlClass) {
                            const QmlClassNode* qcn = static_cast<const QmlClassNode*>(n);
                            stack.prepend(qcn);
                        }
                    }
                }
	    }
	    append(sections, all);
        }
    }

    return sections;
}
#endif

QT_END_NAMESPACE
