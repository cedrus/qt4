TARGET = bearermonitor
QT = core gui network

HEADERS = sessionwidget.h \
          bearermonitor.h

SOURCES = main.cpp \
          bearermonitor.cpp \
          sessionwidget.cpp

maemo5|maemo6 {
  FORMS = bearermonitor_maemo.ui \
          sessionwidget_maemo.ui
} else {
  FORMS = bearermonitor_240_320.ui \
          bearermonitor_640_480.ui \
          sessionwidget.ui
}

win32:!wince*:LIBS += -lWs2_32
wince*:LIBS += -lWs2

CONFIG += console

symbian:TARGET.CAPABILITY = NetworkServices ReadUserData
