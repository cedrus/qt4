TEMPLATE      = subdirs
SUBDIRS       = \
                addressbook \
                modelview


# install
sources.files = README *.pro
sources.path = $$[QT_INSTALL_EXAMPLES]/tutorials
INSTALLS += sources

symbian: include($$QT_SOURCE_TREE/examples/symbianpkgrules.pri)
