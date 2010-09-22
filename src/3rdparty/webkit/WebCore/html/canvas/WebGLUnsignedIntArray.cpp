/*
 * Copyright (C) 2009 Apple Inc. All rights reserved.
 * Copyright (C) 2009 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */

#include "config.h"

#if ENABLE(3D_CANVAS)

#include "WebGLArrayBuffer.h"
#include "WebGLUnsignedIntArray.h"

namespace WebCore {

PassRefPtr<WebGLUnsignedIntArray> WebGLUnsignedIntArray::create(unsigned length)
{
    RefPtr<WebGLArrayBuffer> buffer = WebGLArrayBuffer::create(length, sizeof(unsigned int));
    return create(buffer, 0, length);
}

PassRefPtr<WebGLUnsignedIntArray> WebGLUnsignedIntArray::create(unsigned int* array, unsigned length)
{
    RefPtr<WebGLUnsignedIntArray> a = WebGLUnsignedIntArray::create(length);
    for (unsigned i = 0; i < length; ++i)
        a->set(i, array[i]);
    return a;
}

PassRefPtr<WebGLUnsignedIntArray> WebGLUnsignedIntArray::create(PassRefPtr<WebGLArrayBuffer> buffer,
                                                                unsigned byteOffset,
                                                                unsigned length)
{
    RefPtr<WebGLArrayBuffer> buf(buffer);
    if (!verifySubRange<unsigned int>(buf, byteOffset, length))
        return 0;

    return adoptRef(new WebGLUnsignedIntArray(buf, byteOffset, length));
}

WebGLUnsignedIntArray::WebGLUnsignedIntArray(PassRefPtr<WebGLArrayBuffer> buffer, unsigned byteOffset, unsigned length)
        : WebGLArray(buffer, byteOffset)
        , m_size(length)
{
}

unsigned WebGLUnsignedIntArray::length() const {
    return m_size;
}

unsigned WebGLUnsignedIntArray::byteLength() const {
    return m_size * sizeof(unsigned int);
}

PassRefPtr<WebGLArray> WebGLUnsignedIntArray::slice(int start, int end)
{
    unsigned offset, length;
    calculateOffsetAndLength(start, end, m_size, &offset, &length);
    unsigned fullOffset = m_byteOffset + offset * sizeof(unsigned int);
    clampOffsetAndNumElements<unsigned int>(buffer(), &fullOffset, &length);
    return create(buffer(), fullOffset, length);
}

void WebGLUnsignedIntArray::set(WebGLUnsignedIntArray* array, unsigned offset, ExceptionCode& ec) {
    setImpl(array, offset * sizeof(unsigned int), ec);
}

}

#endif // ENABLE(3D_CANVAS)
