TARGET = mv_edit

TEMPLATE = app

SOURCES += main.cpp \
           mainwindow.cpp \
           mymodel.cpp

HEADERS += mainwindow.h \
           mymodel.h

# install
target.path = $$[QT_INSTALL_EXAMPLES]/tutorials/modelview/5_edit
sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS 5_edit.pro
sources.path = $$[QT_INSTALL_EXAMPLES]/tutorials/modelview/5_edit
INSTALLS += target sources

symbian: include($$QT_SOURCE_TREE/examples/symbianpkgrules.pri)
