load(qttest_p4)
SOURCES  += tst_qftp.cpp


QT = core network

wince*: {
   addFiles.sources = rfc3252.txt
   addFiles.path = .
   DEPLOYMENT += addFiles
   DEFINES += SRCDIR=\\\"\\\"
} else:symbian {
   addFiles.sources = rfc3252.txt
   addFiles.path = .
   DEPLOYMENT += addFiles
   TARGET.EPOCHEAPSIZE="0x100 0x1000000"
   TARGET.CAPABILITY = NetworkServices
} else {
   DEFINES += SRCDIR=\\\"$$PWD/\\\"
}
