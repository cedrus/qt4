TEMPLATE = app
TARGET = flightinfo
SOURCES = flightinfo.cpp
FORMS += form.ui
RESOURCES = flightinfo.qrc
QT += network

symbian {
    TARGET.UID3 = 0xA000CF74
    include($$QT_SOURCE_TREE/demos/symbianpkgrules.pri)
    HEADERS += $$QT_SOURCE_TREE/examples/network/ftp/sym_iap_util.h
    LIBS += -lesock  -lconnmon -linsock
    TARGET.CAPABILITY = NetworkServices
}