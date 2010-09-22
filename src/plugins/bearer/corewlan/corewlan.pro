TARGET = qcorewlanbearer
include(../../qpluginbase.pri)

QT = core gui network
LIBS += -framework Foundation -framework SystemConfiguration

contains(QT_CONFIG, corewlan) {
    isEmpty(QMAKE_MAC_SDK)|contains(QMAKE_MAC_SDK, "/Developer/SDKs/MacOSX10.6.sdk") {
         LIBS += -framework CoreWLAN -framework Security
         DEFINES += MAC_SDK_10_6
    }
}

HEADERS += qcorewlanengine.h \
           ../qnetworksession_impl.h \
           ../qbearerengine_impl.h

SOURCES += main.cpp \
           qcorewlanengine.mm \
           ../qnetworksession_impl.cpp

QTDIR_build:DESTDIR = $$QT_BUILD_TREE/plugins/bearer
target.path += $$[QT_INSTALL_PLUGINS]/bearer
INSTALLS += target
