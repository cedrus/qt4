QT += network \
    webkit
HEADERS += src/BrowserWindow.h \
    src/BrowserView.h \
    src/TitleBar.h \
    src/HomeView.h \
    src/AddressBar.h \
    src/BookmarksView.h \
    src/flickcharm.h \
    src/ZoomStrip.h \
    src/ControlStrip.h
SOURCES += src/Main.cpp \
    src/BrowserWindow.cpp \
    src/BrowserView.cpp \
    src/TitleBar.cpp \
    src/HomeView.cpp \
    src/AddressBar.cpp \
    src/BookmarksView.cpp \
    src/flickcharm.cpp \
    src/ZoomStrip.cpp \
    src/ControlStrip.cpp
RESOURCES += src/anomaly.qrc

include($$QT_SOURCE_TREE/demos/demobase.pri)

symbian {
    HEADERS += $$QT_SOURCE_TREE/examples/network/ftp/sym_iap_util.h
    LIBS += -lesock -lconnmon
    TARGET.CAPABILITY = NetworkServices
    TARGET.EPOCHEAPSIZE = 0x20000 0x2000000
}