TEMPLATE = app
TARGET = weatherinfo
SOURCES = weatherinfo.cpp
RESOURCES = weatherinfo.qrc
QT += network svg

symbian {
    TARGET.UID3 = 0xA000CF77
    include($$QT_SOURCE_TREE/demos/symbianpkgrules.pri)
    INCLUDEPATH += $$QT_SOURCE_TREE/examples/network/qftp/
    LIBS += -lesock -lcommdb -linsock # For IAP selection
    TARGET.CAPABILITY = NetworkServices
}

target.path = $$[QT_INSTALL_DEMOS]/embedded/weatherinfo
sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS *.pro
sources.path = $$[QT_INSTALL_DEMOS]/embedded/weatherinfo
INSTALLS += target sources
