/*
 * Copyright (C) 2006, 2007 Apple Inc.  All rights reserved.
 * Copyright (C) 2008 Collabora Ltd. All rights reserved.
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
#include "PluginPackage.h"

#include "CString.h"
#include "MIMETypeRegistry.h"
#include "npruntime_impl.h"
#include "PluginDatabase.h"
#include "PluginDebug.h"

namespace WebCore {

typedef void gtkInitFunc(int *argc, char ***argv);

bool PluginPackage::fetchInfo()
{
    if (!load())
        return false;

    NPP_GetValueProcPtr gv = (NPP_GetValueProcPtr)m_module->resolve("NP_GetValue");
    typedef char *(*NPP_GetMIMEDescriptionProcPtr)();
    NPP_GetMIMEDescriptionProcPtr gm =
        (NPP_GetMIMEDescriptionProcPtr)m_module->resolve("NP_GetMIMEDescription");
    if (!gm || !gv)
        return false;

    char *buf = 0;
    NPError err = gv(0, NPPVpluginNameString, (void*) &buf);
    if (err != NPERR_NO_ERROR)
        return false;

    m_name = buf;
    err = gv(0, NPPVpluginDescriptionString, (void*) &buf);
    if (err != NPERR_NO_ERROR)
        return false;

    m_description = buf;
    determineModuleVersionFromDescription();

    String s = gm();
    Vector<String> types;
    s.split(UChar(';'), false, types);
    for (unsigned i = 0; i < types.size(); ++i) {
        Vector<String> mime;
        types[i].split(UChar(':'), true, mime);
        if (mime.size() > 0) {
            Vector<String> exts;
            if (mime.size() > 1)
                mime[1].split(UChar(','), false, exts);
            determineQuirks(mime[0]);
            m_mimeToExtensions.add(mime[0], exts);
            if (mime.size() > 2)
                m_mimeToDescriptions.add(mime[0], mime[2]);
        }
    }

    return true;
}

static NPError staticPluginQuirkRequiresGtkToolKit_NPN_GetValue(NPP instance, NPNVariable variable, void* value)
{
    if (variable == NPNVToolkit) {
        *static_cast<uint32*>(value) = 2;
        return NPERR_NO_ERROR;
    }

    return NPN_GetValue(instance, variable, value);
}

bool PluginPackage::load()
{
    if (m_isLoaded) {
        m_loadCount++;
        return true;
    }

    m_module = new QLibrary((QString)m_path);
    m_module->setLoadHints(QLibrary::ResolveAllSymbolsHint);
    if (!m_module->load()) {
        LOG(Plugins, "%s not loaded (%s)", m_path.utf8().data(),
                m_module->errorString().toLatin1().constData());
        return false;
    }

    m_isLoaded = true;

    NP_InitializeFuncPtr NP_Initialize;
    NPError npErr;
    gtkInitFunc* gtkInit;

    NP_Initialize = (NP_InitializeFuncPtr)m_module->resolve("NP_Initialize");
    m_NPP_Shutdown = (NPP_ShutdownProcPtr)m_module->resolve("NP_Shutdown");

    if (!NP_Initialize || !m_NPP_Shutdown)
        goto abort;

    memset(&m_pluginFuncs, 0, sizeof(m_pluginFuncs));
    m_pluginFuncs.size = sizeof(m_pluginFuncs);

    initializeBrowserFuncs();

    if (m_path.contains("npwrapper.")) {
        // nspluginwrapper relies on the toolkit value to know if glib is available
        // It does so in NP_Initialize with a null instance, therefore it is done this way:
        m_browserFuncs.getvalue = staticPluginQuirkRequiresGtkToolKit_NPN_GetValue;
    }

    // WORKAROUND: Prevent gtk based plugin crashes such as BR# 40567 by
    // explicitly forcing the initializing of Gtk, i.e. calling gtk_init,
    // whenver the symbol is present in the plugin library loaded above.
    // Note that this workaround is based on code from the NSPluginClass ctor
    // in KDE's kdebase/apps/nsplugins/viewer/nsplugin.cpp file.
    gtkInit = (gtkInitFunc*)m_module->resolve("gtk_init");
    if (gtkInit) {
        // Prevent gtk_init() from replacing the X error handlers, since the Gtk
        // handlers abort when they receive an X error, thus killing the viewer.
#ifdef Q_WS_X11
        int (*old_error_handler)(Display*, XErrorEvent*) = XSetErrorHandler(0);
        int (*old_io_error_handler)(Display*) = XSetIOErrorHandler(0);
#endif
        gtkInit(0, 0);
#ifdef Q_WS_X11
        XSetErrorHandler(old_error_handler);
        XSetIOErrorHandler(old_io_error_handler);
#endif
    }

#if defined(XP_UNIX)
    npErr = NP_Initialize(&m_browserFuncs, &m_pluginFuncs);
#else
    npErr = NP_Initialize(&m_browserFuncs);
#endif
    if (npErr != NPERR_NO_ERROR)
        goto abort;

    m_loadCount++;
    return true;

abort:
    unloadWithoutShutdown();
    return false;
}

uint16 PluginPackage::NPVersion() const
{
    return NP_VERSION_MINOR;
}
}
