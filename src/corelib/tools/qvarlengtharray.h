/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the QtCore module of the Qt Toolkit.
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

#ifndef QVARLENGTHARRAY_H
#define QVARLENGTHARRAY_H

#include <QtCore/qcontainerfwd.h>
#include <QtCore/qglobal.h>
#include <new>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

QT_MODULE(Core)

template<class T, int Prealloc>
class QPodList;

// Prealloc = 256 by default, specified in qcontainerfwd.h
template<class T, int Prealloc>
class QVarLengthArray
{
public:
    inline explicit QVarLengthArray(int size = 0);

    inline QVarLengthArray(const QVarLengthArray<T, Prealloc> &other)
        : a(Prealloc), s(0), ptr(reinterpret_cast<T *>(array))
    {
        append(other.constData(), other.size());
    }

    inline ~QVarLengthArray() {
        if (QTypeInfo<T>::isComplex) {
            T *i = ptr + s;
            while (i-- != ptr)
                i->~T();
        }
        if (ptr != reinterpret_cast<T *>(array))
            qFree(ptr);
    }
    inline QVarLengthArray<T, Prealloc> &operator=(const QVarLengthArray<T, Prealloc> &other)
    {
        if (this != &other) {
            clear();
            append(other.constData(), other.size());
        }
        return *this;
    }

    inline void removeLast() {
        Q_ASSERT(s > 0);
        realloc(s - 1, a);
    }
    inline int size() const { return s; }
    inline int count() const { return s; }
    inline bool isEmpty() const { return (s == 0); }
    inline void resize(int size);
    inline void clear() { resize(0); }

    inline int capacity() const { return a; }
    inline void reserve(int size);

    inline T &operator[](int idx) {
        Q_ASSERT(idx >= 0 && idx < s);
        return ptr[idx];
    }
    inline const T &operator[](int idx) const {
        Q_ASSERT(idx >= 0 && idx < s);
        return ptr[idx];
    }
    inline const T &at(int idx) const { return operator[](idx); }

    T value(int i) const;
    T value(int i, const T &defaultValue) const;

    inline void append(const T &t) {
        if (s == a)   // i.e. s != 0
            realloc(s, s<<1);
        const int idx = s++;
        if (QTypeInfo<T>::isComplex) {
            new (ptr + idx) T(t);
        } else {
            ptr[idx] = t;
        }
    }
    void append(const T *buf, int size);

    inline T *data() { return ptr; }
    inline const T *data() const { return ptr; }
    inline const T * constData() const { return ptr; }
    typedef int size_type;
    typedef T value_type;
    typedef value_type *pointer;
    typedef const value_type *const_pointer;
    typedef value_type &reference;
    typedef const value_type &const_reference;
    typedef qptrdiff difference_type;

private:
    friend class QPodList<T, Prealloc>;
    void realloc(int size, int alloc);

    int a;      // capacity
    int s;      // size
    T *ptr;     // data
    union {
        // ### Qt 5: Use 'Prealloc * sizeof(T)' as array size
        char array[sizeof(qint64) * (((Prealloc * sizeof(T)) / sizeof(qint64)) + 1)];
        qint64 q_for_alignment_1;
        double q_for_alignment_2;
    };
};

template <class T, int Prealloc>
Q_INLINE_TEMPLATE QVarLengthArray<T, Prealloc>::QVarLengthArray(int asize)
    : s(asize) {
    if (s > Prealloc) {
        ptr = reinterpret_cast<T *>(qMalloc(s * sizeof(T)));
        Q_CHECK_PTR(ptr);
        a = s;
    } else {
        ptr = reinterpret_cast<T *>(array);
        a = Prealloc;
    }
    if (QTypeInfo<T>::isComplex) {
        T *i = ptr + s;
        while (i != ptr)
            new (--i) T;
    }
}

template <class T, int Prealloc>
Q_INLINE_TEMPLATE void QVarLengthArray<T, Prealloc>::resize(int asize)
{ realloc(asize, qMax(asize, a)); }

template <class T, int Prealloc>
Q_INLINE_TEMPLATE void QVarLengthArray<T, Prealloc>::reserve(int asize)
{ if (asize > a) realloc(s, asize); }

template <class T, int Prealloc>
Q_OUTOFLINE_TEMPLATE void QVarLengthArray<T, Prealloc>::append(const T *abuf, int increment)
{
    Q_ASSERT(abuf);
    if (increment <= 0)
        return;

    const int asize = s + increment;

    if (asize >= a)
        realloc(s, qMax(s*2, asize));

    if (QTypeInfo<T>::isComplex) {
        // call constructor for new objects (which can throw)
        while (s < asize)
            new (ptr+(s++)) T(*abuf++);
    } else {
        qMemCopy(&ptr[s], abuf, increment * sizeof(T));
        s = asize;
    }
}

template <class T, int Prealloc>
Q_OUTOFLINE_TEMPLATE void QVarLengthArray<T, Prealloc>::realloc(int asize, int aalloc)
{
    Q_ASSERT(aalloc >= asize);
    T *oldPtr = ptr;
    int osize = s;

    const int copySize = qMin(asize, osize);
    if (aalloc != a) {
        ptr = reinterpret_cast<T *>(qMalloc(aalloc * sizeof(T)));
        Q_CHECK_PTR(ptr);
        if (ptr) {
            s = 0;
            a = aalloc;

            if (QTypeInfo<T>::isStatic) {
                QT_TRY {
                    // copy all the old elements
                    while (s < copySize) {
                        new (ptr+s) T(*(oldPtr+s));
                        (oldPtr+s)->~T();
                        s++;
                    }
                } QT_CATCH(...) {
                    // clean up all the old objects and then free the old ptr
                    int sClean = s;
                    while (sClean < osize)
                        (oldPtr+(sClean++))->~T();
                    if (oldPtr != reinterpret_cast<T *>(array) && oldPtr != ptr)
                        qFree(oldPtr);
                    QT_RETHROW;
                }
            } else {
                qMemCopy(ptr, oldPtr, copySize * sizeof(T));
            }
        } else {
            ptr = oldPtr;
            return;
        }
    }
    s = copySize;

    if (QTypeInfo<T>::isComplex) {
        // destroy remaining old objects
        while (osize > asize)
            (oldPtr+(--osize))->~T();
    }

    if (oldPtr != reinterpret_cast<T *>(array) && oldPtr != ptr)
        qFree(oldPtr);

    if (QTypeInfo<T>::isComplex) {
        // call default constructor for new objects (which can throw)
        while (s < asize)
            new (ptr+(s++)) T;
    } else {
        s = asize;
    }
}

template <class T, int Prealloc>
Q_OUTOFLINE_TEMPLATE T QVarLengthArray<T, Prealloc>::value(int i) const
{
    if (i < 0 || i >= size()) {
        return T();
    }
    return at(i);
}
template <class T, int Prealloc>
Q_OUTOFLINE_TEMPLATE T QVarLengthArray<T, Prealloc>::value(int i, const T &defaultValue) const
{
    return (i < 0 || i >= size()) ? defaultValue : at(i);
}


QT_END_NAMESPACE

QT_END_HEADER

#endif // QVARLENGTHARRAY_H
